//
// Created by aaron on 17-6-6.
//

#include  "MemFile.h"

using namespace std;

char MemFile::fetchChar(int addr){
    if(addr>=M_MAX) return -1;
    return mem[addr];
}

int MemFile::display(int addr){
    if(addr>=M_MAX) return -1;
    int val=0;
    for(int i=0;i<4;i++){
        val<<=8;
        val+=((int) mem[addr+i])&255;
    }
    return val;
}

void MemFile::display() {
    for (int i = 0; i < 30; i++ ) {
        printf("  %08x ", display(8*i));
        printf("%08x\n", display(8*i+4));
    }
    printf("\n");
}

int MemFile::fetchInt(int addr){
    if(addr>=M_MAX) return -1;
    int val=0;
    int temp;
    temp=((((int)mem[addr+3])&255)<<24);
    val+=(((int)mem[addr])&255);
    val+=(((int)mem[addr+1])&255)<<8;
    val+=(((int)mem[addr+2])&255)<<16;
    val+=temp;

    return val;
}

bool MemFile::write(int val,int addr){

    if(addr>=M_MAX||addr<0) return false;
    for(int i=0;i<4;i++){
        mem[addr+i]=(char)val;
        val>>=8;
    }
    return true;
}

void MemFile::read(std::ifstream &in){
    char a,b,c;
    int st=2;
    while(in.get(a),in.get(b)){
        if(a=='\n'&&b!='\n'){in.unget();continue;}
        else if(a=='\n'&&b=='\n') continue;

        c=(char)((a<<4)+(b&0xF));
        printf("%02x ",c);
    }
}

void MemFile::clear(){
    for(int i=0;i<M_MAX;i++)
        mem[i]=0;
}

