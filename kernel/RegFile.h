//
// Created by aaron on 17-6-6.
//

#ifndef KERNEL_REGFILE_H
#define KERNEL_REGFILE_H

#include <stdio.h>

class RegFile{

public:
    const char REAX=0x0;
    const char RECX=0x1;
    const char REDX=0x2;
    const char REBX=0x3;
    const char RESP=0x4;
    const char REBP=0x5;
    const char RESI=0x6;
    const char REDI=0x7;
    const char RNONE=0xF;

    int reg[8]={0};
    int read(char src);
    void write(int val,char src);
    void clear();
    void display();
};

#endif //KERNEL_REGFILE_H
