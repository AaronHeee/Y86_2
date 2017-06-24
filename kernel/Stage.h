//
// Created by aaron on 17-6-6.
//

#ifndef KERNEL_OPSTAGE_H
#define KERNEL_OPSTAGE_H

#include <iostream>
#include <string.h>

#include "ALU.h"
#include "MemFile.h"
#include "RegFile.h"

using namespace std;

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
    char  memory[M_MAX];
    int   reg[8];

    int   cycle;
};

class Stage{
public:

    Stage() { init(); }

    //TODO: 变量类型调整

    int   F_predPC;

    int   f_predPC;
    int   f_pc;
    char  f_icode;
    char  f_ifun;
    int   f_valC;
    int   f_valP;
    char  f_stat;
    char  f_rA;
    char  f_rB;
    bool  imem_error;
    bool  instr_valid;

    char  D_stat;
    char  D_icode;
    char  D_ifun;
    char  D_rA;
    char  D_rB;
    int   D_valC;
    int   D_valP;

    char  d_srcA;
    char  d_srcB;
    char  d_dstE;
    char  d_dstM;
    int   d_rvalA;
    int   d_rvalB;
    int   d_valA;
    int   d_valB;

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

    int   e_valE;
    bool  e_Cnd;
    char  e_dstE;
    bool  CC[3];
    bool  set_cc;
    char  alufun;

    char  M_stat;
    char  M_icode;
    int   M_valE;
    int   M_valA;
    char  M_dstE;
    char  M_dstM;
    bool  M_Cnd;

    bool  dmem_error;
    int   m_valM;
    char  m_stat;

    char  W_stat;
    char  W_icode;
    int   W_valE;
    int   W_valM;
    char  W_dstE;
    char  W_dstM;
    char  w_dstE;
    char  w_dstM;
    int   w_valE;
    int   w_valM;

    char  stat;

    bool F_stall;
    bool D_stall, D_bubble;
    bool E_bubble;
    bool M_bubble;
    bool W_stall;

    MemFile memory;
    RegFile regFile;
    ALU alu;

    Info *info;

    void init();

    void Fetch();
    void Decode();
    void Execute();
    void Memory();
    void WriteBack();

    void Update_PipeReg();  //Update Pipeline Register
    void Register_Control();  //Set the signals for Pipeline Register

    void output();
    void output(Info*);

    void  mapping(int cycle);
    char* strc2char(Info *info);
    Info* char2strc(char* buf);
};

#endif //KERNEL_OPSTAGE_H
