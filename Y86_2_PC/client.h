#ifndef CLIENT_H
#define CLIENT_H

#endif // CLIENT_H

#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#include "assembler.h"
#include "parameter.h"

#define BUF_SIZE 0xFFFF
#define MAX 200

const char IHALT=0x0;
const char INOP=0x1;
const char IRRMOVL=0x2;
const char IIRMOVL=0x3;
const char IRMMOVL=0x4;
const char IMRMOVL=0x5;
const char IOPL=0x6;
const char IJXX=0x7;
const char ICALL=0x8;
const char IRET=0x9;
const char IPUSHL=0xA;
const char IPOPL=0xB;
const char FNONE=0x0;

const char REAX=0x0;
const char RECX=0x1;
const char REDX=0x2;
const char REBX=0x3;
const char RESP=0x4;
const char REBP=0x5;
const char RESI=0x6;
const char REDI=0x7;
const char RNONE=0xF;

const char ALUADD=0x0;
const int  SIZE=0xFFFF;

const char SBUB=0x0;
const char SAOK=0x1;
const char SADR=0x2;
const char SINS=0x3;
const char SHLT=0x4;

const char JMP=0x0;
const char JLE=0x1;
const char JL =0x2;
const char JE =0x3;
const char JNE=0x4;
const char JGE=0x5;
const char JG =0x6;

const char RRMOVL=0x0;
const char CMOVLE=0x1;
const char CMOVL =0x2;
const char CMOVE =0x3;
const char CMOVNE=0x4;
const char CMOVGE=0x5;
const char CMOVG =0x6;

const int ZF=0x0;
const int SF=0x1;
const int OVF=0x2;

const char ADDL=0x0;
const char SUBL=0x1;
const char ANDL=0x2;
const char XORL=0x3;

struct Info {
    int   F_predPC;

    char  D_stat;
    char  D_icode;
    char  D_ifun;
    char  D_rA;
    char  D_rB;
    int   D_valC;
    int   D_valP;


    char  E_stat;
    char  E_icode;
    char  E_ifun;
    int   E_valC;
    int   E_valA;
    int   E_valB;
    char  E_dstE;
    char  E_dstM;
    char  E_srcA;
    char  E_srcB;

    char  M_stat;
    char  M_icode;
    int   M_valE;
    int   M_valA;
    char  M_dstE;
    char  M_dstM;
    bool  M_Cnd;

    char  W_stat;
    char  W_icode;
    int   W_valE;
    int   W_valM;
    char  W_dstE;
    char  W_dstM;

    bool  F_stall;
    bool  D_stall;
    bool  W_stall;

    int   pc;

    char  stat;

    bool  CC[3];
    char  memory[BUF_SIZE];
    int   reg[8];

    int   cycle;
};

struct CpuInfo{
    int   init;
    int   initCycle;
    Info* info[MAX];
};

class Client {
public:
    Client() {iniClient();}
    void iniClient();

    int update;
    CpuInfo *cpuinfo;

    Info *char2strc(char *buf);
    int display(int addr, char *buffer);
    void display(char *buffer);
    ssize_t rio_readn(int fd, char usrbuf[], size_t n);
    ssize_t rio_writen(int fd, char usrbuf[], size_t n);

    int Connect(char *ip, int port);
    void SendFile(int fd, char *filename);
    void RecvInfo(int fd);
    Info *iniInfo();
};


