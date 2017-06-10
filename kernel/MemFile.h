//
// Created by aaron on 17-6-6.
//

#ifndef Y86_CORE_INSTRMEM_H
#define Y86_CORE_INSTRMEM_H

#include <fstream>
#include <stdio.h>

#define M_MAX 0xFFFF

class MemFile{

public:

    char mem[M_MAX]={0};
    char fetchChar(int addr);
    int fetchInt(int addr);
    int display(int addr);
    void display();
    bool write(int val,int addr);
    void read(std::ifstream &in);
    void clear();

};

#endif