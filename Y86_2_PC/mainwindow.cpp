#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <pthread.h>

#include <QTimer>
#include <QFileDialog>
#include <QTextStream>

Client client;
Info *now;
CpuInfo *all;

QTimer *timer = new QTimer();
int msec = 1000;

/********To String**********/

QString changeColor(QString temp,QLabel *l){
    if(now->cycle == 0) return "<font color=black>"+temp+"<\font>";
    if("<font color=RoyalBlue><b>"+temp+"<\b><\font>"==l->text()||"<font color=black>"+temp+"<\font>"==l->text())
    return "<font color=black>"+temp+"<\font>";
    return "<font color=RoyalBlue><b>"+temp+"<\b><\font>";
}

QString intToString(int a,QLabel *l){
    QString temp=QString("%1").arg(a,8,16,QChar('0')).toUpper();
    temp=temp.right(8);
    return changeColor(temp,l);
}


QString memIntToString(int a){
    QString temp=QString("%1").arg(a,8,16,QChar('0')).toUpper();
    temp=temp.right(8);
    return temp;
}

QString intToStringDec(int a){
    return QString("%1").arg(a,4,10,QChar('0'));
}

QString icodeToString (char a,bool i=0){
    QString b="NOP";
    switch(a){
    case IHALT:b="HALT";break;
    case INOP:b="NOP";break;
    case IRRMOVL:b="RRMOVL";break;
    case IIRMOVL:b="IRMOVL";break;
    case IRMMOVL:b="RMMVOL";break;
    case IMRMOVL:b="MRMOVL";break;
    case IOPL:b="OPL";break;
    case IJXX:b="JXX";break;
    case ICALL:b="CALL";break;
    case IRET:b="RET";break;
    case IPUSHL:b="PUSHL";break;
    case IPOPL:b="POPL";
    }
    if(i||now->cycle == 0)
    return "<font color=black>"+b+"<\font>";
    return "<font color=RoyalBlue><b>"+b+"<\b><\font>";
}

QString ifunToString (char icode,char a,bool i=0){
    QString b="——";
    if(a==FNONE&&icode!=IJXX&&icode!=IOPL&&icode!=IRRMOVL)
    {
        if(a==FNONE)
        b="——";
        else
        b="ERR";
    }
    else {
        switch(icode){
        case IJXX:switch(a){
                case JMP: b="JMP";break;
                case JLE:b="JLE";break;
                case JL:b="JL";break;
                case JE:b="JE";break;
                case JNE:b="JNE";break;
                case JGE:b="JGE";break;
                case JG:b="JG";break;
            }break;
        case IOPL:switch(a){
                case ADDL:b="ADDL";break;
                case SUBL:b="SUBL";break;
                case ANDL:b="ANDL";break;
                case XORL:b="XORL";break;
            }break;
        case IRRMOVL:switch(a){
                case RRMOVL:b="——";break;
                case CMOVLE:b="CMOVLE";break;
                case CMOVL:b="CMOVL";break;
                case CMOVE:b="CMOVE";break;
                case CMOVNE:b="CMOVNE";break;
                case CMOVGE:b="CMOVGE";break;
                case CMOVG:b="CMOVG";break;
        }
        }
    }
    if(i|| now->cycle == 0)
    return "<font color=black>"+b+"<\font>";
    return "<font color=RoyalBlue><b>"+b+"<\b><\font>";
}
QString boolToString(bool Cnd,QLabel *l){
    QString temp;
    if(Cnd) temp="1";
    else temp="0";
    if(now->cycle == 0) return "<font color=black>"+temp+"<\font>";
    if("<font color=RoyalBlue><b>"+temp+"<\b><\font>"==l->text()||"<font color=black>"+temp+"<\font>"==l->text())
        return "<font color=black>"+temp+"<\font>";
    else return "<font color=RoyalBlue><b>"+temp+"<\b><\font>";
}

QString regToString(char a,QLabel *l){
    QString b;
    switch (a){
    case RNONE: b= "——";break;
    case REAX: b= "EAX";break;
    case RECX: b= "ECX";break;
    case REDX: b= "EDX";break;
    case REBX: b= "EBX";break;
    case REBP: b= "EBP";break;
    case RESP: b= "ESP";break;
    case RESI: b= "ESI";break;
    case REDI: b= "EDI";break;
    }
    return changeColor(b,l);
}

QString statToString(char a,bool stall=0){
    QString b;
    switch (a){
    case SBUB: b= "<font color=blue><b>SBUB<\b><\font>" ;break;
    case SAOK: b= "SAOK";break;
    case SADR: b= "<font color=red><b>SADR<\b><\font>";break;
    case SHLT: b= "<font color=purple><b>SHLT<b><\font>";break;
    case SINS: b= "<font color=red><b>SINS<\b><\font>";break;
    }
    return b;
}

QString statToStringPanel(char a){
    QString b;
    switch (a){
    case SAOK: b= "SAOK";break;
    case SADR: b= "SADR";break;
    case SHLT: b= "<font color=RoyalBlue><b>SHLT<\b><\font>";break;
    case SINS: b= "SINS";break;
    }
    return b;
}

int addr = 0;
void MainWindow::displayMem(int n) {
    ui->La_ADR1->setText(memIntToString(n + 0*4)+":");
    ui->La_ADR2->setText(memIntToString(n + 1*4)+":");
    ui->La_ADR3->setText(memIntToString(n + 2*4)+":");
    ui->La_ADR4->setText(memIntToString(n + 3*4)+":");
    ui->La_ADR5->setText(memIntToString(n + 4*4)+":");
    ui->La_ADR6->setText(memIntToString(n + 5*4)+":");
    ui->La_ADR7->setText(memIntToString(n + 6*4)+":");
    ui->La_VAL1->setText(memIntToString(now->memory[n + 0*4]));
    ui->La_VAL2->setText(memIntToString(now->memory[n + 1*4]));
    ui->La_VAL3->setText(memIntToString(now->memory[n + 2*4]));
    ui->La_VAL4->setText(memIntToString(now->memory[n + 3*4]));
    ui->La_VAL5->setText(memIntToString(now->memory[n + 4*4]));
    ui->La_VAL6->setText(memIntToString(now->memory[n + 5*4]));
    ui->La_VAL7->setText(memIntToString(now->memory[n + 6*4]));
}

void MainWindow::displayAll(int n) {
    now = client.cpuinfo->info[n];
    displayMem(addr);
    ui->AddOneSecond->display(intToStringDec(now->cycle));
    ui->LaF_predPC->setText(intToString(now->F_predPC,ui->LaF_predPC));
    ui->LaD_icode->setText(icodeToString(now->D_icode,now->D_stall||now->D_stat==SBUB));
    ui->LaE_icode->setText(icodeToString(now->E_icode,now->E_stat==SBUB));
    ui->LaM_icode->setText(icodeToString(now->M_icode,now->M_stat==SBUB));
    ui->LaW_icode->setText(icodeToString(now->W_icode,now->W_stall||now->W_stat==SBUB));
    ui->LaD_ifun->setText(ifunToString(now->D_icode,now->D_ifun,now->D_stall||now->D_stat==SBUB));
    ui->LaE_ifun->setText(ifunToString(now->E_icode,now->E_ifun,now->E_stat==SBUB));
    ui->LaM_Cnd->setText(boolToString(now->M_Cnd,ui->LaM_Cnd));
    ui->LaD_valC->setText(intToString(now->D_valC,ui->LaD_valC));
    ui->LaD_valP->setText(intToString(now->D_valP,ui->LaD_valP));
    ui->LaE_valC->setText(intToString(now->E_valC,ui->LaE_valC));
    ui->LaE_valA->setText(intToString(now->E_valA,ui->LaE_valA));
    ui->LaE_valB->setText(intToString(now->E_valB,ui->LaE_valB));
    ui->LaM_valE->setText(intToString(now->M_valE,ui->LaM_valE));
    ui->LaM_valA->setText(intToString(now->M_valA,ui->LaM_valA));
    ui->LaW_valE->setText(intToString(now->W_valE,ui->LaW_valE));
    ui->LaW_valM->setText(intToString(now->W_valM,ui->LaW_valM));
    ui->LaD_rA->setText(regToString(now->D_rA,ui->LaD_rA));
    ui->LaD_rB->setText(regToString(now->D_rB,ui->LaD_rB));
    ui->LaE_dstE->setText(regToString(now->E_dstE,ui->LaE_dstE));
    ui->LaE_dstM->setText(regToString(now->E_dstM,ui->LaE_dstM));
    ui->LaE_srcA->setText(regToString(now->E_srcA,ui->LaE_srcA));
    ui->LaE_srcB->setText(regToString(now->E_srcB,ui->LaE_srcB));
    ui->LaM_dstE->setText(regToString(now->M_dstE,ui->LaM_dstE));
    ui->LaM_dstM->setText(regToString(now->M_dstM,ui->LaM_dstM));
    ui->LaW_dstE->setText(regToString(now->W_dstE,ui->LaW_dstE));
    ui->LaW_dstM->setText(regToString(now->W_dstM,ui->LaW_dstM));
    ui->LaW_stat->setText(statToString(now->W_stat,now->W_stall));
    ui->LaM_stat->setText(statToString(now->M_stat));
    ui->LaE_stat->setText(statToString(now->E_stat));
    ui->LaD_stat->setText(statToString(now->D_stat,now->D_stall));
    ui->La_REAX->setText(intToString(now->reg[REAX],ui->La_REAX));
    ui->La_RECX->setText(intToString(now->reg[RECX],ui->La_RECX));
    ui->La_REDX->setText(intToString(now->reg[REDX],ui->La_REDX));
    ui->La_REBX->setText(intToString(now->reg[REBX],ui->La_REBX));
    ui->La_RESP->setText(intToString(now->reg[RESP],ui->La_RESP));
    ui->La_REBP->setText(intToString(now->reg[REBP],ui->La_REBP));
    ui->La_RESI->setText(intToString(now->reg[RESI],ui->La_RESI));
    ui->La_REDI->setText(intToString(now->reg[REDI],ui->La_REDI));
    ui->La_ZF->setText(boolToString(now->CC[ZF],ui->La_ZF));
    ui->La_SF->setText(boolToString(now->CC[SF],ui->La_SF));
    ui->La_OF->setText(boolToString(now->CC[OVF],ui->La_OF));
    ui->La_PC->setText(memIntToString(now->pc));
    ui->La_stat->setText(statToStringPanel(now->stat));

    if(now->stat == SHLT || now->stat == SADR) {
        if(timer->isActive()) timer->stop();
        ui->Bu_START->setText("START");
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    this->setWindowIcon(QIcon(":/cpu.ico"));
    connect(timer,SIGNAL(timeout()),this,SLOT(cycleDisplay()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void * Receive(void *fd) {
    int sock = *((int *) fd);
    client.RecvInfo(sock);
    printf("sock = %d\n",sock);
    return NULL;
}

int clk=0;

void MainWindow::cycleDisplay(){
    if(now->stat==SHLT) {timer->stop();return;}
     if(client.update > clk)
         displayAll(++clk);
}

void MainWindow::openText(){
    QString f="out.yo";
    QFile file(f);
    QTextStream in(&file);
    ui->textBrowser->setText(in.readAll());
}

void MainWindow::on_actionOpen_o_triggered()
{
    client.iniClient();

    QString str=QFileDialog::getOpenFileName(this," ","asm(*.s,*.txt,*.asm)");

    char* filename;
    QByteArray ba = str.toLatin1();
    filename = ba.data();

    int serv_sock;
    serv_sock = client.Connect("127.0.0.1",5583);
    client.SendFile(serv_sock, filename);

    openText();

    displayAll(0);

    pthread_t tid;
    pthread_create(&tid, NULL, Receive, &serv_sock);

}

void MainWindow::on_Bu_RESET_clicked()
{
    displayAll(0);
    clk = 0;
    timer->stop();
}



void MainWindow::on_Bu_STEP_clicked()
{
    if(client.update > clk) displayAll(++clk);
}

void MainWindow::on_Bu_START_clicked()
{
    if(ui->Bu_START->text() == "START" && (now->stat == SAOK || now->stat == SBUB)){
        timer->start(msec);
        ui->Bu_START->setText("STOP");
    }
    else if(ui->Bu_START->text() == "STOP" || (now->stat != SAOK && now->stat != SBUB)){
        if(timer->isActive()) timer->stop();
        ui->Bu_START->setText("START");
    }
}

void MainWindow::on_Sl_speed_valueChanged(int value)
{
    msec=800/value;
    timer->stop();
    if(ui->Bu_START->text()=="STOP")
    timer->start(msec);
}

void MainWindow::on_Bu_BACK_clicked()
{
    if(clk > 0) displayAll(--clk);
}
