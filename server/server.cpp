//
// Created by aaron on 17-6-10.
//

#include <unistd.h>
#include <cstring>

#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#include "Stage.h"
#include "Assembler.h"

#define BUF_SIZE 0xFFFF

Stage cpu;
Assembler assembler;

/* threads */
void *Execute(void *vargp) {
    cpu.Execute();
    return NULL;
}

void *Memory(void *vargp) {
    cpu.Memory();
    return NULL;
}

void *WriteBack(void *vargp) {
    cpu.WriteBack();
    return NULL;
}

/* process */
void wait4children(int signo) {
    int status;
    while(waitpid(-1, &status, WNOHANG) > 0);
}

/* display */
int display(int addr,char *buffer){
    int val=0;
    for(int i=0;i<4;i++){
        val<<=8;
        val+=((int) buffer[addr+i])&255;
    }
    return val;
}

void display(char *buffer) {
    for (int i = 0; i < 30; i++ ) {
        printf("  %08x ", display(8*i, buffer));
        printf("%08x\n", display(8*i+4, buffer));
    }
    printf("\n");
}


/* IO */
ssize_t rio_readn(int fd,char usrbuf[],size_t n)
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = usrbuf;

    while(nleft > 0){
        if((nread = read(fd,bufp,nleft)) < 0){
            if(errno == EINTR){/*interrupted by sig handler return*/
                nread = 0;
            }else{
                return -1;/*error*/
            }
        }else if(nread == 0){
            break;  /*EOF*/
        }else{/*read content*/
            nleft -= nread;
            bufp += nread;
        }
    }
    return (n - nleft);
}

ssize_t rio_writen(int fd,char usrbuf[],size_t n)
{
    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = usrbuf;

    while(nwritten = write(fd,bufp,nleft) <= 0){
        if(errno == EINTR){
            nwritten = 0;
        }else{
            return -1;
        }
        nleft -= nwritten;
        bufp += nwritten;
    }
    return n;
}

int main(int argc, char **argv) {

    /* sleep */
    int sec = 0;
    if (argc == 2) sec = atoi(argv[1]);

    printf("\nStart the server...\n");

    printf("\nsleep time = %d\n", sec);
    /* socket */

    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //create skocket

    struct sockaddr_in serv_addr; //create sockaddr_in for IPv4

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(5583);

    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); //bind the skocket and the IP & port

    int lstn = listen(serv_sock, SOMAXCONN);
    if (lstn == -1) {
        lstn = listen(serv_sock, SOMAXCONN);
//        perror("listen");
//        exit(-1);
    }

    printf("\nwaiting for you ...\n\n");

    signal(SIGCHLD, wait4children);

    while(1) {

        struct sockaddr_in clnt_addr; //create clnt_addr for Client?
        socklen_t clnt_addr_size = sizeof(clnt_addr);

        int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size); // accept the request from Client
        if(clnt_sock < 0) {
            perror("accept");
            close(serv_sock);
            exit(-1);
        }

        pid_t pid = fork();
        if (pid < 0 ) {
            perror("fork");
            exit(-1);
        }

        else if (pid == 0) {
            close(serv_sock);

            char buffer[BUF_SIZE];

            printf("\nI'm a Child \n\n");

            if (!rio_readn(clnt_sock, buffer, BUF_SIZE)) {
                perror("read");
                exit(-1);
            }

            display(buffer);

            cpu.init();

            cout<<endl<<"Instruction Memory:"<<endl;

            display(buffer);

            cpu.memory.copy(buffer);

            cpu.memory.display();

            for (int i = 0; cpu.stat != SHLT && cpu.stat != SADR ; i++) {
                if(sec) sleep(sec);

                cpu.Register_Control();
                cpu.Update_PipeReg();

                pthread_t tid_E, tid_M, tid_W;
                pthread_create(&tid_E, NULL, Execute, NULL);
                pthread_create(&tid_M, NULL, Memory, NULL);
                pthread_create(&tid_W, NULL, WriteBack, NULL);
                pthread_join(tid_E, NULL);
                pthread_join(tid_M, NULL);
                pthread_join(tid_W, NULL);

                cpu.Decode();
                cpu.Fetch();

                printf("第%d个循环:\n",i+1);
                cpu.output();
                cpu.regFile.display();

                cpu.mapping(i + 1);
                printf("stat = %02x\n",cpu.info->stat);
                printf("size of info = %d\n",sizeof(*(cpu.info)));

                char * buf = cpu.strc2char(cpu.info);

                rio_writen(clnt_sock, buf, sizeof(*(cpu.info)));
                printf("Resolve:\n");

                cpu.output(cpu.char2strc(buf));

            }

            close(clnt_sock);
            _exit(0);

        }

        else if(pid > 0) {
            close(clnt_sock);
        }
    }

    close(serv_sock);

    return 0;

}

