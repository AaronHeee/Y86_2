//
// Created by aaron on 17-6-6.
//

#ifndef KERNEL_ALU_H
#define KERNEL_ALU_H

const char ADDL=0x0;
const char SUBL=0x1;
const char ANDL=0x2;
const char XORL=0x3;

class ALU{

public:

    bool cnd[3]={true,false,false};
    int op(bool set_cc,int aluA,char alufun,int aluB);
    void cc(bool* CC);

};

#endif //KERNEL_ALU_H
