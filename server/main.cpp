//#include <pthread.h>
//
#include "Stage.h"
//#include "Assembler.h"
//
Stage cpu;
//
//Assembler assembler;
//
//    /* 串行部分 */
//

void cycle() {

    cpu.Register_Control();
    cpu.Update_PipeReg();

    cpu.WriteBack();
    cpu.Memory();
    cpu.Execute();
    cpu.Decode();
    cpu.Fetch();

    cpu.output();
}

//
//void *Execute(void *vargp) {
//    cpu.Execute();
//    return NULL;
//}
//
//void *Memory(void *vargp) {
//    cpu.Memory();
//    return NULL;
//}
//
//void *WriteBack(void *vargp) {
//    cpu.WriteBack();
//    return NULL;
//}
//
//int main() {
//
//    cpu.init();
//
//    cout<<endl<<"Instruction Memory:"<<endl;
//
//    char filename[] = "/home/aaron/Desktop/y86-code/asumr.ys";
//    ifstream in(filename);
//    ifstream in_temp(filename);
//
//    ofstream out("out.yo");
//    assembler.compileFile(in, in_temp, cpu.memory, out);
//
//    cpu.memory.display();
//
//    for (int i = 0; cpu.stat != SHLT && cpu.stat != SADR ; i++) {
//        cpu.Register_Control();
//        cpu.Update_PipeReg();
//
//        pthread_t tid_E, tid_M, tid_W;
//        pthread_create(&tid_E, NULL, Execute, NULL);
//        pthread_create(&tid_M, NULL, Memory, NULL);
//        pthread_create(&tid_W, NULL, WriteBack, NULL);
//        pthread_join(tid_E, NULL);
//        pthread_join(tid_M, NULL);
//        pthread_join(tid_W, NULL);
//
//        cpu.Decode();
//        cpu.Fetch();
//
//        printf("第%d个循环:\n",i+1);
//        cpu.output();
//        cpu.regFile.display();
//    }
//
//    /* 串行部分 */
//
////    //for(int i=0; i < 15; i++) {
////    for (int i=0; cpu.stat != SHLT; i++) {
////        printf("第%d个循环:\n",i+1);
////        cycle();
////    }
//
//}