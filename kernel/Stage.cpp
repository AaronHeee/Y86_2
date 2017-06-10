//
// Created by aaron on 17-6-6.
//

#include "Stage.h"

void Stage::init(){

    F_predPC = 0x0;

    f_pc     = 0x0;
    f_predPC = 0x0;
    f_icode  = INOP;
    f_ifun   = FNONE;
    f_valC   = 0x0;
    f_valP   = 0x0;
    f_stat   = SBUB;
    f_rA     = RNONE;
    f_rB     = RNONE;
    imem_error  = false;
    instr_valid = false;

    D_stat  = SBUB;
    D_icode = INOP;
    D_ifun  = FNONE;
    D_rA    = RNONE;
    D_rB    = RNONE;
    D_valC  = 0x0;
    D_valP  = 0x0;

    d_srcA  = RNONE;
    d_srcB  = RNONE;
    d_dstE  = RNONE;
    d_dstM  = RNONE;
    d_rvalA = 0x0;
    d_rvalB = 0x0;
    d_valA  = 0x0;
    d_valB  = 0x0;

    E_stat  = SBUB;
    E_icode = INOP;
    E_ifun  = FNONE;
    E_valC  = 0x0;
    E_valA  = 0x0;
    E_valB  = 0x0;
    E_dstE  = RNONE;
    E_dstM  = RNONE;
    E_srcA  = RNONE;
    E_srcB  = RNONE;

    e_valE  = 0x0;
    e_Cnd   = false;
    set_cc  = false;
    e_dstE  = RNONE;
    alufun  = ADDL;

    M_stat  = SBUB;
    M_icode = INOP;
    M_valE  = 0x0;
    M_valA  = 0x0;
    M_dstE  = RNONE;
    M_dstM  = RNONE;
    M_Cnd   = false;

    m_valM  = 0x0;
    m_stat  = SBUB;
    dmem_error = false;


    W_stat  = SBUB;
    W_icode = INOP;
    W_valE  = 0x0;
    W_valM  = 0x0;
    W_dstE  = RNONE;
    W_dstM  = RNONE;

    w_valE  = 0x0;
    w_valM  = 0x0;
    w_dstE  = RNONE;
    w_dstM  = RNONE;

    stat    = SBUB;

    CC[0]   = true;
    CC[1]   = false;
    CC[2]   = false;

    F_stall = false;
    D_stall = false;
    W_stall = false;

    D_bubble = false;
    E_bubble = false;
    M_bubble = false;

    memory.clear();
    regFile.clear();
}

void Stage::Fetch() {
    /* Register Control */

//    if (F_stall)
//        return;

    /* Select PC */

    if (M_icode == IJXX&&!M_Cnd)
        f_pc = M_valA;
    else if (W_icode == IRET)
        f_pc = W_valM;
    else f_pc = F_predPC;

    /* Read instruction */
    imem_error = f_pc > M_MAX;
    instr_valid = !((f_icode < IHALT || f_icode > IPOPL) || (f_icode == IOPL && (f_ifun < FNONE || f_ifun > XORL)) ||
            (( f_icode == IJXX || f_icode == IRRMOVL) && (f_ifun < JMP || f_ifun > JG)));

    if (imem_error) {
        f_stat = SADR;
        f_icode = INOP;
        f_ifun = FNONE;
        f_rA=f_rA = RNONE;
        f_valC = 0;
        f_valP = f_pc + 1;
        return;
    }
    else {
        char temp_icode_ifun = memory.fetchChar(f_pc);
        f_icode = (temp_icode_ifun & (char)0xF0) >> 4;
        f_ifun = temp_icode_ifun & (char)0xF;

        if(!instr_valid)
            f_stat = SINS;
        else if(f_icode == IHALT)
            f_stat = SHLT;
        else f_stat = SAOK;
    }

    bool need_regids = ((f_icode >= IRRMOVL && f_icode <= IOPL) || (f_icode >= IPUSHL && f_icode <= IPOPL));
    bool need_valC = (f_icode >= IIRMOVL && f_icode <= ICALL && f_icode != IOPL);

    f_valP=f_pc;

    if (need_regids) {
        char rA_rB = memory.fetchChar(f_pc+1);
        f_rA = (rA_rB & 0xF0) >> 4;
        f_rB = rA_rB & 0xF;
        instr_valid = !(f_rA < 0||f_rB < 0||f_rA > 8||f_rB > 8);

        if(!need_valC) {
            f_valC = 0;
            f_valP += 2;
        }
        else {
            f_valC = memory.fetchInt(f_pc + 2);
            f_valP += 6;
        }
    }
    else {
        if (!need_valC) {
            f_rA = f_rB = RNONE;
            f_valC = 0;
            f_valP += 1;
        }
        else {
            f_rA = f_rB = RNONE;
            f_valC = memory.fetchInt(f_pc + 1);
            f_valP += 5;
        }
    }

    /* Predict PC */
    if (f_icode == IJXX || f_icode == ICALL) f_predPC = f_valC;
    else f_predPC = f_valP;
}

void Stage::Decode() {

    /* Register_Control */

//    if (D_stall)
//        return;

    /* set ID */

    if(D_icode == IRRMOVL || D_icode == IRMMOVL || D_icode == IOPL || D_icode == IPUSHL)
        d_srcA = D_rA;
    else if(D_icode == IPOPL || D_icode == IRET)
        d_srcA = RESP;
    else d_srcA = RNONE;

    if(D_icode == IOPL || D_icode == IRMMOVL || D_icode == IMRMOVL)
        d_srcB = D_rB;
    else if(D_icode == IPUSHL || D_icode == IPOPL || D_icode == ICALL || D_icode == IRET)
        d_srcB = RESP;
    else d_srcB = RNONE;

    if(D_icode == IRRMOVL || D_icode==IIRMOVL || D_icode == IOPL)
        d_dstE = D_rB;
    else if(D_icode == IPUSHL || D_icode == IPOPL || D_icode == ICALL || D_icode == IRET)
        d_dstE = RESP;
    else d_dstE = RNONE;

    if(D_icode == IMRMOVL || D_icode == IPOPL)
        d_dstM = D_rA;
    else d_dstM = RNONE;

    /* Read Register File */
    d_rvalA = regFile.read(d_srcA);
    d_rvalB = regFile.read(d_srcB);

    /* Select and Forward */
    if(D_icode == ICALL || D_icode == IJXX) d_valA = D_valP;
    else if(d_srcA == e_dstE) d_valA = e_valE;
    else if(d_srcA == M_dstM) d_valA = m_valM;
    else if(d_srcA == M_dstE) d_valA = M_valE;
    else if(d_srcA == W_dstM) d_valA = W_valM;
    else if(d_srcA == W_dstE) d_valA = W_valE;
    else d_valA = d_rvalA;

    if(d_srcB == e_dstE) d_valB = e_valE;
    else if(d_srcB == M_dstM) d_valB = m_valM;
    else if(d_srcB == M_dstE) d_valB = M_valE;
    else if(d_srcB == W_dstM) d_valB = W_valM;
    else if(d_srcB == W_dstE) d_valB = W_valE;
    else d_valB = d_rvalB;
}

void Stage::Execute() {

    int aluA;
    int aluB;

    /* ALU Operate */
    if(E_icode == IRRMOVL || E_icode == IOPL)
        aluA = E_valA;
    else if(E_icode == IIRMOVL || E_icode == IRMMOVL || E_icode == IMRMOVL)
        aluA = E_valC;
    else if(E_icode == ICALL || E_icode == IPUSHL)
        aluA = -4;
    else if(E_icode == IRET||E_icode == IPOPL)
        aluA = 4;
    else aluA = 0;

    if(E_icode == IRMMOVL || E_icode == IMRMOVL || E_icode == IOPL || E_icode == ICALL
       || E_icode == IPUSHL || E_icode == IRET || E_icode == IPOPL)
        aluB=E_valB;
    else aluB=0;

    alufun = E_icode == IOPL ? E_ifun : ALUADD;
    set_cc = (E_icode == IOPL);
    e_valE = alu.op(set_cc, aluA, alufun, aluB);
    alu.cc(CC);

    /* Set e_Cnd */
    if(E_icode == IJXX||E_icode == IRRMOVL) {
        switch(E_ifun) {
            case JMP:
                e_Cnd = true;
                break;
            case JLE:
                e_Cnd = (CC[SF]^CC[OVF])|CC[ZF];
                break;
            case JL:
                e_Cnd = CC[SF]^CC[OVF];
                break;
            case JE:
                e_Cnd = CC[ZF];
                break;
            case JNE:
                e_Cnd=!CC[ZF];
                break;
            case JGE:
                e_Cnd=~(CC[SF]^CC[OVF]);
                break;
            case JG:
                e_Cnd=(!(CC[SF]^CC[OVF]))&&(!CC[ZF]);
                break;
        }
    }
    else e_Cnd=false;

    /* Set dstE */
    e_dstE = (E_icode == IRRMOVL && !e_Cnd) ? RNONE : E_dstE;

}

void Stage::Memory() {

    /* Register Control */

    if (M_bubble) {
        m_valM = 0;
    }

    int mem_addr;
    bool mem_read = false;
    bool mem_write = false;

    if(M_icode == IRMMOVL || M_icode == IPUSHL || M_icode == ICALL || M_icode == IMRMOVL)
        mem_addr = M_valE;

    else if(M_icode == IPOPL || M_icode == IRET) mem_addr = M_valA;

    dmem_error = (mem_addr<0) || (mem_addr >= M_MAX);
    m_stat = dmem_error ? SADR : M_stat;

    if(M_icode == IMRMOVL || M_icode == IPOPL || M_icode == IRET)
        mem_read = true;
    else if(M_icode == IRMMOVL || M_icode == IPUSHL || M_icode == ICALL)
        mem_write = true;

    if((!dmem_error) && mem_read) m_valM = memory.fetchInt(mem_addr);
    else m_valM = 0;

    if((!dmem_error) && mem_write) memory.write( M_valA, mem_addr);
}

void Stage::WriteBack() {
    w_dstE = W_dstE;
    w_valE = W_valE;
    w_dstM = W_dstM;
    w_valM = W_valM;

    stat = W_stat == SBUB ? SAOK : W_stat; //Y86的状态

    regFile.write(w_valE,w_dstE);
    regFile.write(w_valM,w_dstM);
}

void Stage::Update_PipeReg() {

    /* Write Back */

    if (!W_stall) {
        W_stat  = m_stat;
        W_icode = M_icode;
        W_valE  = M_valE;
        W_valM  = m_valM;
        W_dstE  = M_dstE;
        W_dstM  = M_dstM;
    }


    /* Memory */

    if (M_bubble)
        M_stat = SBUB;
    else
        M_stat = E_stat;

    if (M_stat != SBUB) {
        M_icode = E_icode;
        M_Cnd = e_Cnd;
        M_valE = e_valE;
        M_valA = E_valA;
        M_dstE = e_dstE;
        M_dstM = E_dstM;
    }
    else {
        M_icode = INOP;
        M_Cnd = 0;
        M_valE = 0;
        M_valA = 0;
        M_dstE = RNONE;
        M_dstM = RNONE;
    }


    /* Execute */

    if(E_bubble)
        E_stat = SBUB;
    else
        E_stat = D_stat;

    if (E_stat != SBUB) {
        E_icode = D_icode;
        E_ifun = D_ifun;
        E_valC = D_valC;
        E_valA = d_valA;
        E_valB = d_valB;
        E_dstE = d_dstE;
        E_dstM = d_dstM;
        E_srcA = d_srcA;
        E_srcB = d_srcB;
    }
    else {
        E_icode = INOP;
        E_ifun = FNONE;
        E_valC = 0;
        E_valA = 0;
        E_valB = 0;
        E_dstE = RNONE;
        E_dstM = RNONE;
        E_srcA = RNONE;
        E_srcB = RNONE;
    }


    /* Decode */

    if (!D_stall) {
        if (D_bubble)
            D_stat = SBUB;
        else
            D_stat = f_stat;

        if (D_stat != SBUB) {
            D_icode = f_icode;
            D_ifun = f_ifun;
            D_rA = f_rA;
            D_rB = f_rB;
            D_valC = f_valC;
            D_valP = f_valP;
        }
        else {
            D_icode = INOP;
            D_ifun = FNONE;
            D_rA = RNONE;
            D_rB = RNONE;
            D_valC = 0;
            D_valP = 0;
        }
    }

    /* Fetch */
    if (!F_stall)
        F_predPC = f_predPC;


}

void Stage::Register_Control() {

    F_stall = ((E_icode == IMRMOVL || E_icode == IPOPL) &&
               (E_dstM == d_srcA || E_dstM == d_srcB) ) ||
              (D_icode == IRET || E_icode == IRET || M_icode == IRET);

    D_stall = ( E_icode == IMRMOVL || E_icode == IPOPL) &&
              (E_dstM == d_srcA || E_dstM == d_srcB);

    D_bubble = ( E_icode == IJXX && !e_Cnd ) || !(  (E_icode == IMRMOVL || E_icode == IPOPL) &&
               (E_dstM == d_srcA || E_dstM == d_srcB) )  && ( D_icode == IRET || E_icode == IRET || M_icode == IRET);

    E_bubble = (E_icode == IJXX && !e_Cnd) || ( (E_icode == IMRMOVL|| E_icode == IPOPL) &&
               (E_dstM == d_srcA || E_dstM == d_srcB));

    M_bubble = (m_stat == SADR || m_stat == SINS || m_stat == SHLT) ||
               (W_stat == SADR || W_stat == SINS || W_stat == SHLT);

    W_stall = W_stat == SADR || W_stat == SINS || W_stat == SHLT;

}

void Stage::output() {

    printf("\n");
    printf("Stat = %02x\n", stat);
    /* Write Back */
    printf("\n");
    printf("Write:");
    printf("   W_stat = %02x", W_stat);
    printf("  W_icode = %02x", W_icode);
    printf("  W_valE = %08x", W_valE);
    printf("  W_valM = %08x", W_valM);
    printf("  W_dstE = %02x", W_dstE);
    printf("  W_dstM = %02x", W_dstM);
    printf("\n");

    /* Memory */

    printf("\n");
    printf("       ");
    printf("  m_stat = %02x", stat);
    printf("  m_valM = %08x", m_valM);
    printf("  dmem_error = %01x", dmem_error);
    printf("\n");

    printf("\n");
    printf("Memory:");
    printf("  M_stat = %02x", M_stat);
    printf("  M_icode = %02x", M_icode);
    printf("  M_Cnd = %01x", M_Cnd);
    printf("  M_valE = %08x", M_valE);
    printf("  M_valA = %08x", M_valA);
    printf("  M_dstE = %02x", M_dstE);
    printf("  M_dstM = %02x", M_dstM);
    printf("\n");

    /* Execute */

    printf("\n");
    printf("       ");
    printf("  alufun = %02x", alufun);
    printf("  e_valE = %08x", e_valE);
    printf("  e_dstE = %02x", e_dstE);
    printf("  e_Cnd  = %01x", e_Cnd);
    printf("  set_cc = %01x", set_cc);
    printf("  ZF = %01x  SF = %01x  OF = %01x",CC[0],CC[1],CC[2]);
    printf("\n");

    printf("\n");
    printf("Execute:");
    printf(" E_stat = %02x", E_stat);
    printf("  E_icode= %02x", E_icode);
    printf("  E_ifun = %02x", E_ifun);
    printf("  E_valC = %08x", E_valC);
    printf("  E_valA = %08x", E_valA);
    printf("  E_valB = %08x", E_valB);
    printf("\n       ");
    printf("  E_dstE = %02x", E_dstE);
    printf("  E_dstM = %02x", E_dstM);
    printf("  E_srcA = %02x", E_srcA);
    printf("  E_srcB = %02x", E_srcB);
    printf("\n");

    /* Decode */

    printf("\n");
    printf("       ");
    printf("  d_dstE = %02x", d_dstE);
    printf("  d_dstM = %02x", d_dstM);
    printf("  d_srcA = %02x", d_srcA);
    printf("  d_srcB = %02x", d_srcB);
    printf("  d_rvalA = %08x", d_rvalA);
    printf("  d_rvalB = %08x", d_rvalB);
    printf("  d_valA = %08x", d_valA);
    printf("  d_valB = %08x", d_valB);
    printf("\n");

    printf("\n");
    printf("Decode:");
    printf("  D_stat = %02x", D_stat);
    printf("  D_icode = %02x", D_icode);
    printf("  D_ifun = %02x", D_ifun);
    printf("  D_rA = %02x", D_rA);
    printf("  D_rB = %02x", D_rB);
    printf("  D_valC = %08x", D_valC);
    printf("  D_valP = %08x", D_valP);
    printf("\n");


    /* Fetch */

    printf("\n");
    printf("       ");
    printf("  f_stat = %02x", f_stat);
    printf("  f_icode = %02x", f_icode);
    printf("  f_ifun = %02x", f_ifun);
    printf("  f_rA = %02x", f_rA);
    printf("  f_rB = %02x", f_rB);
    printf("  f_valC = %08x", f_valC);
    printf("  f_valP = %08x", f_valP);
    printf("  f_predPC = %08x", f_predPC);
    printf("\n       ");
    printf("  instr_valid = %01x", instr_valid);
    printf("\n");

    printf("\n");
    printf("Fetch:");
    printf("   F_predPC = %08x", F_predPC);
    printf("\n");

    /* Register Control */
    printf("\n");
    printf("  F_stall  = %01x\n", F_stall);
    printf("  D_stall  = %01x\n", D_stall);
    printf("  D_bubble = %01x\n", D_bubble);
    printf("  E_bubble = %01x\n", E_bubble);
    printf("  M_bubble = %01x\n", M_bubble);
    printf("  W_stall  = %01x\n", W_stall);
    printf("\n");

}