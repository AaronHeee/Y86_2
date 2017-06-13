#include "client.h"

/* parse */
Info* Client::char2strc(char* buf) {
    Info* info = new Info;
    memcpy(info, buf, sizeof(*info));
    return info;
}

/* display */
int Client::display(int addr, char *buffer){
    int val=0;
    for(int i=0;i<4;i++){
        val<<=8;
        val+=((int) buffer[addr+i])&255;
    }
    return val;
}

void Client::display(char *buffer) {
    for (int i = 0; i < 30; i++ ) {
        printf("  %08x ", display(8*i, buffer));
        printf("%08x\n", display(8*i+4, buffer));
    }
    printf("\n");
}


/* IO */
ssize_t Client::rio_readn(int fd,char usrbuf[],size_t n)
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

ssize_t Client::rio_writen(int fd,char usrbuf[],size_t n)
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

int Client::Connect(char *ip,int port) {
    signal(SIGPIPE, SIG_IGN);

    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //create skocket

    struct sockaddr_in serv_addr; //create sockaddr_in for IPv4

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(port);

    int cnt = connect(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); //connect the skocket and the IP & port
    if (cnt) {
        perror("connect");
        exit(-1);
    }

    return serv_sock;
}

void Client::SendFile(int fd, char* filename) {
    char buffer[BUF_SIZE];

    Assembler assembler;

    ifstream in(filename);
    ifstream in_temp(filename);

    ofstream out("out.yo");
    assembler.compileFile(in, in_temp, buffer, out);

    display(buffer);

    int i = rio_writen(fd, buffer, BUF_SIZE);
    printf("write : %d\n",i);
}


void Client::RecvInfo(int fd){

    Info *info;

    int RecvSize = sizeof(*info);
    char *bufRecv = new char[RecvSize];

    for (int i = 1; rio_readn(fd, bufRecv, RecvSize); i++) {
        info = char2strc(bufRecv);
        update = i;
        cpuinfo->initCycle = i;
        cpuinfo->init = i/MAX;
        cpuinfo->info[i] = info;
    }

    close(fd);
}

Info* Client::iniInfo() {
    Info *i = new Info;
    i->F_predPC = 0x0;

    i->pc     = 0x0;
    i->D_stat  = SBUB;
    i->D_icode = INOP;
    i->D_ifun  = FNONE;
    i->D_rA    = RNONE;
    i->D_rB    = RNONE;
    i->D_valC  = 0x0;
    i->D_valP  = 0x0;

    i->E_stat  = SBUB;
    i->E_icode = INOP;
    i->E_ifun  = FNONE;
    i->E_valC  = 0x0;
    i->E_valA  = 0x0;
    i->E_valB  = 0x0;
    i->E_dstE  = RNONE;
    i->E_dstM  = RNONE;
    i->E_srcA  = RNONE;
    i->E_srcB  = RNONE;

    i->M_stat  = SBUB;
    i->M_icode = INOP;
    i->M_valE  = 0x0;
    i->M_valA  = 0x0;
    i->M_dstE  = RNONE;
    i->M_dstM  = RNONE;
    i->M_Cnd   = false;

    i->W_stat  = SBUB;
    i->W_icode = INOP;
    i->W_valE  = 0x0;
    i->W_valM  = 0x0;
    i->W_dstE  = RNONE;
    i->W_dstM  = RNONE;

    i->stat    = SBUB;

    i->CC[0]   = true;
    i->CC[1]   = false;
    i->CC[2]   = false;

    i->F_stall = false;
    i->D_stall = false;
    i->W_stall = false;

    for (int k = 0; k < BUF_SIZE; k++)
       i->memory[k] = 0;

    for (int k = 0; k < 8; k++)
       i->reg[8] = 0;

    return i;
}

void Client::iniClient(){
    free(cpuinfo);
    cpuinfo = new CpuInfo;
    cpuinfo->info[0] = iniInfo();
}

