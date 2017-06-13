#include "parser.h"

/********To String**********/

QString changeColor(QString temp,QLabel *l){
    if(ini) return "<font color=black>"+temp+"<\font>";
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
    if(i||ini)
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
    if(i||ini)
    return "<font color=black>"+b+"<\font>";
    return "<font color=RoyalBlue><b>"+b+"<\b><\font>";
}
QString boolToString(bool Cnd,QLabel *l){
    QString temp;
    if(Cnd) temp="1";
    else temp="0";
    if(ini) return "<font color=black>"+temp+"<\font>";
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
