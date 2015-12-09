#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <vector>
#include <sstream>
#include <string>
#include <assert.h>
#include <cstring>
using namespace std;

inline bool isInteger(const std::string & s) {
    if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;
    char * p ;
    strtol(s.c_str(), &p, 10) ;
    return (*p == 0) ;
}

/*
    Intel 8051 decoder
    Created in 09/11/2015 at HCMUT by Dedo

 *============================================================================*/

#include <assert.h>
#include <cstring>
#if defined(_MSC_VER) && _MSC_VER <= 1100
#include "signature.h"
#endif
#include "decoder.h"
#include "exp.h"
#include "prog.h"
#include "proc.h"
#include "_8051decoder.h"
#include "rtl.h"
#include "BinaryFile.h"     
#include "boomerang.h"

unsigned _8051Decoder::magic_process(std::string name) {

    if (name == "R0") return 0;
    else if (name == "R1") return 1;
    else if (name == "R2") return 2;
    else if (name == "R3") return 3;
    else if (name == "R4") return 4;
    else if (name == "R5") return 5;
    else if (name == "R6") return 6;
    else if (name == "R7") return 7;
    else if (name == "A") return 8;
    else if (name == "B") return 9;
    else if (name == "C") return 10;
    else if (name == "DPTR") return 11;
    else if (name == "AB") return 12;
    else if (name == "P0") return 13;
    else if (name == "P1") return 14;
    else if (name == "P2") return 15;
    else if (name == "P3") return 16;
    else if (name == "SP") return 17;
    else if (name == "DPL") return 18;
    else if (name == "DPH") return 19;
    else if (name == "PCON") return 20;
    else if (name == "TCON") return 21;
    else if (name == "TMOD") return 22;
    else if (name == "TL0") return 23;
    else if (name == "TL1") return 24;
    else if (name == "TH0") return 25;
    else if (name == "TH1") return 26;
    else if (name == "SCON") return 27;
    else if (name == "SBUF") return 28;
    else if (name == "IE") return 29;
    else if (name == "IP") return 30;
    else if (name == "PSW") return 31;
    else 
        return 999;
}

unsigned map_sfr(std::string name){

    if (name == "R0") return 0;
    else if (name == "R1") return 1;
    else if (name == "R2") return 2;
    else if (name == "R3") return 3;
    else if (name == "R4") return 4;
    else if (name == "R5") return 5;
    else if (name == "R6") return 6;
    else if (name == "R7") return 7;
    else if (name == "A") return 8;
    else if (name == "B") return 9;
    else if (name == "C") return 10;
    else if (name == "DPTR") return 11;
    else if (name == "AB") return 12;
    else if (name == "P0") return 13;
    else if (name == "P1") return 14;
    else if (name == "P2") return 15;
    else if (name == "P3") return 16;
    else if (name == "SP") return 17;
    else if (name == "DPL") return 18;
    else if (name == "DPH") return 19;
    else if (name == "PCON") return 20;
    else if (name == "TCON") return 21;
    else if (name == "TMOD") return 22;
    else if (name == "TL0") return 23;
    else if (name == "TL1") return 24;
    else if (name == "TH0") return 25;
    else if (name == "TH1") return 26;
    else if (name == "SCON") return 27;
    else if (name == "SBUF") return 28;
    else if (name == "IE") return 29;
    else if (name == "IP") return 30;
    else if (name == "PSW") return 31;
    else 
        return 999;
}
static DecodeResult result;


/*==============================================================================
 * FUNCTION:       _8051Decoder::decodeInstruction
 * OVERVIEW:       Attempt to decode the high level instruction at a given address and return the corresponding HL type
 *                  (e.g. CallStatement, GotoStatement etc). If no high level instruction exists at the given address,
 *                  then simply return the RTL for the low level instruction at this address. There is an option to also
 *                 include the low level statements for a HL instruction.
 * PARAMETERS:     pc - the native address of the pc
 *                 delta - the difference between the above address and the host address of the pc (i.e. the address
 *                  that the pc is at in the loaded object file)
 *                 proc - the enclosing procedure. This can be NULL for those of us who are using this method in an
 *                  interpreter
 * RETURNS:        a DecodeResult structure containing all the information gathered during decoding
 *============================================================================*/
DecodeResult&  _8051Decoder::decodeInstruction (ADDRESS pc, int delta){
  return result;
}
bool if_a_byte(char * reg){
    std::list<char*>::iterator br;
    for(br = bitReg.begin(); br != bitReg.end(); ++ br ){
        if(strcmp(reg,(*br)) == 0)
            return true;
    }  
    return false;
}
list<Statement*>* initial_bit_regs(){
    std::list<Statement*>* stmts = NULL;

    // Build a Union

    CompoundType* ct = new CompoundType();
    ct->addType(new SizeType(8), "bit0");  
    ct->addType(new SizeType(8), "bit1");
    ct->addType(new SizeType(8), "bit2");
    ct->addType(new SizeType(8), "bit3");
    ct->addType(new SizeType(8), "bit4");
    ct->addType(new SizeType(8), "bit5");
    ct->addType(new SizeType(8), "bit6");
    ct->addType(new SizeType(8), "bit7");
    UnionType * ut = new UnionType();
    ut->addType(new SizeType(8), "x");
    ut->addType(ct,"m");

    // A Register will represent a Union variable, i choose Reg0

    ImpRefStatement * i_s = new ImpRefStatement((Type*) ut, Location::regOf(0));
    std::cout << i_s->prints() << std::endl;

    // Now check in bitReg to match all Register that represents a byte
    std::list<char*>::iterator br;
    for(br = bitReg.begin(); br != bitReg.end(); ++ br ){
        unsigned num = map_sfr(std::string(*br));
        Assign * a_ss = new Assign((Type *) ut,(Exp *) Location::regOf(0),(Exp *) new TypedExp((Type*) ut, (Exp*) Location::regOf(num)), NULL);
        std::cout << "REPRESENT A BYTE " << a_ss->prints() << std::endl;
        stmts->push_back(a_ss);
    }
    
    return stmts;
}

bool first_line = true;

Exp* byte_present(char * reg){
    Exp* exp = NULL;
    unsigned num = map_sfr(reg);
    exp = new Binary(opMemberAccess,Location::regOf(num), new Const("x"));
    return exp;
}
Exp* access_bit(char * reg, unsigned pos){
    Exp* exp = NULL;
    unsigned num = map_sfr(reg);

    std::stringstream sstm;
    sstm << "bit" << pos;
    std::string name = sstm.str();
    char *bit =  new char[name.length() + 1];
    strcpy(bit, name.c_str());

    Exp * exp1 = new Binary(opMemberAccess,Location::regOf(num), new Const("x"));
    Exp * exp2 = new Binary(opMemberAccess,exp1, new Const("m"));
    exp = new Binary(opMemberAccess,exp2, new Const(bit));
    return exp;
}
DecodeResult& _8051Decoder::decodeAssembly(ADDRESS pc,std::string line, AssemblyLine* Line)
{
    static DecodeResult result;
    int delta = 0;
    ADDRESS hostPC = pc+delta;

    result.reset();
    std::list<Statement*>* stmts = NULL;

    //-------USELESS-----------
    ADDRESS nextPC = NO_ADDRESS;
    //ADDRESS nextPC = NO_ADDRESS;
    dword MATCH_p = hostPC;
    //-------------------------

    std::string opcode(Line->name);
    list<AssemblyExpression*>::iterator ei;

    if (opcode == "MOV" || opcode == "MOVC" || opcode == "MOVX") {
        ei = Line->expList->begin();
        AssemblyArgument* arg1 = (*ei)->argList.front();
        ++ei;
        AssemblyArgument* arg2 = (*ei)->argList.front(); 
        unsigned op1;
        unsigned op2;
        if(opcode == "MOV"){
            switch (arg1->kind){
                case 3: /* MOV INDIRECT */
                {   
                    op1 = magic_process(std::string(arg1->value.c));
                    switch (op1){
                        case 0: /* @R0 */
                        {    switch (arg2->kind){
                                case 6: /* ID */
                                {    op2 = magic_process(arg2->value.c);
                                    if((op2 >= 9 &&  op2 <= 10) ||  op2 >= 12)   /* DIRECT ID */
                                        stmts = instantiate(pc, "MOV_RI0_DIR", new Const(op2));
                                    else if(op2 == 8) /* A */
                                        stmts = instantiate(pc, "MOV_RI0_A");
                                    break;
                                }
                                case 4: /* IMMEDIATE INT */
                                {    stmts = instantiate(pc, "MOV_RI0_IMM" , new Const(arg2->value.i));
                                    break;
                                }
                                case 1: /* DIRECT INT */
                                {   stmts = instantiate(pc, "MOV_RI0_DIR", new Const(arg2->value.i));
                                    break;
                                }
                                default:break; 
                            }
                            break;
                        }
                        case 1: /* @R1 */
                        {    switch (arg2->kind){
                                case 6: /* ID */
                                    op2 = magic_process(arg2->value.c);
                                    if(op2 >= 13 && op2 <= 16)   /* DIRECT ID */
                                        stmts = instantiate(pc, "MOV_RI1_DIR", new Const(op2));
                                    else if(op2 == 8) /* A */
                                        stmts = instantiate(pc, "MOV_RI1_A");
                                    break;
                                case 4: /* IMMEDIATE INT */
                                    stmts = instantiate(pc, "MOV_RI1_IMM" , new Const(arg2->value.i));
                                    break;
                                case 1: /* DIRECT INT */
                                    stmts = instantiate(pc, "MOV_RI1_DIR", new Const(op2));
                                    break;
                                default:break; 
                            }
                            break;
                        }
                        case 11: /* @DPTR */
                            break;
                        default:
                            break;
                    }

                    break;
                }
                case 6: /* MOV Rn | A | DPTR | DIRECT */
                {       
                        op1 = magic_process(std::string(arg1->value.c));
                        if (op1 < 8){ /* MOV Rn */
                            std::string name = "MOV_R";
                            std::stringstream sstm;
                            sstm << name << op1;
                            switch(arg2->kind){
                                case 6: { /* ID */
                                    op2 = magic_process(std::string(arg2->value.c));
                                    if ((op2 >= 9 && op2 <= 10) || op2 >= 12){ /* RN, DIRECT */
                                        sstm << "_DIR";
                                        name = sstm.str();
                                        char *name_ =  new char[name.length() + 1];
                                        strcpy(name_, name.c_str());
                                        stmts = instantiate(pc, name_, new Const(op2));
                                    }
                                    else if (op2 == 8 ){ /* RN, A */
                                        sstm << "_A";
                                        name = sstm.str();
                                        char *name_ =  new char[name.length() + 1];
                                        strcpy(name_, name.c_str());
                                        stmts = instantiate(pc, name_);
                                    }
                                    break;
                                    }
                                case 1: /* RN, DIRECT */
                                    {sstm << "_DIR";
                                    name = sstm.str();
                                    char *name_ =  new char[name.length() + 1];
                                    strcpy(name_, name.c_str());
                                    stmts = instantiate(pc, name_, new Const(arg2->value.i));
                                    break;}
                                case 4: /* RN, IMM */
                                    {sstm << "_IMM";
                                    name = sstm.str();
                                    char *name_ =  new char[name.length() + 1];
                                    strcpy(name_, name.c_str());
                                    stmts = instantiate(pc, name_ , new Const(arg2->value.i));
                                    break;
                                    }
                                default:
                                    break;
                            }
                        }
                        else if (op1 == 8){ /* MOV A */ 
                            switch(arg2->kind){
                                case 3: /* A, INDIRECT */
                                {   op2 = magic_process(std::string(arg2->value.c));
                                    if (op2 == 0)
                                        stmts = instantiate(pc, "MOV_A_RI0");
                                    else if (op2 == 1)
                                        stmts = instantiate(pc, "MOV_A_RI1");
                                    break;
                                }
                                case 6: /* A, Rn | DIRECT */
                                {
                                    op2 = magic_process(std::string(arg2->value.c));
                                    if(op2 < 8){ /* A, Rn*/
                                            std::string name = "MOV_A_R";
                                            std::stringstream sstm;
                                            sstm << name << op2;
                                            name = sstm.str();
                                            char *name_ =  new char[name.length() + 1];
                                            strcpy(name_, name.c_str());
                                            stmts = instantiate(pc, name_);
                                    }
                                    else if ((op2 >= 9 && op2 <= 10) || op2 >= 12){ /* A, DIRECT */
                                        std::string name = "MOV_A_DIR";
                                        char *name_ =  new char[name.length() + 1];
                                        strcpy(name_, name.c_str());
                                         stmts = instantiate(pc, name_ ,new Const(op2));
                                    }
                                    break;
                                }
                                case 1: /* A, DIRECT */
                                {   std::string name = "MOV_A_DIR";
                                    char *name_ =  new char[name.length() + 1];
                                    strcpy(name_, name.c_str());
                                    stmts = instantiate(pc, name_ , new Const(arg2->value.i));
                                    break;
                                }
                                case 4: /* A, IMM */
                                {    stmts = instantiate(pc, "MOV_A_IMM", new Const(arg2->value.i));
                                    break;
                                }
                                default:
                                    break;
                            }
                        }
                        else if (op1 == 11 ){ /* MOV DPTR */
                            stmts = instantiate(pc,"MOV_DPTR_ADDR16", new Const(arg2->value.i)); 
                        }
                        else if ((op1 >= 9 && op1 <= 10) || op1 >= 12){ /* MOV DIRECT */
                            std::string name = "MOV_DIR_";
                            std::stringstream sstm;
                            sstm << name;
                            switch(arg2->kind){
                                case 3: /* DIRECT, INDIRECT*/
                                {    op2 = magic_process(std::string(arg2->value.c));
                                    if (op2 == 0)
                                        sstm << "RI" << op2;
                                    else if(op2 == 1)
                                        sstm << "RI" << op2;
                                    name = sstm.str();
                                    char *name_ =  new char[name.length() + 1];
                                    strcpy(name_, name.c_str());
                                    stmts = instantiate(pc, name_, new Const(op1)); 
                                    break;
                                }
                                case 6: /* DIRECT, Rn | DIRECT */
                                {    op2 = magic_process(std::string(arg2->value.c));
                                    if (op2 < 8 ){ /* DIRECT, Rn */
                                         sstm << "R" << op2;
                                        name = sstm.str();
                                        char *name_ =  new char[name.length() + 1];
                                        strcpy(name_, name.c_str());
                                        stmts = instantiate(pc, name_, new Const(op1));
                                    }
                                    else if (op2 == 8 ){ /* DIRECT, A */
                                        sstm << "A";
                                        name = sstm.str();
                                        char *name_ =  new char[name.length() + 1];
                                        strcpy(name_, name.c_str());
                                        stmts = instantiate(pc, name_, new Const(op1));
                                    }
                                    else if ((op2 >= 9 && op2 <= 10) || op2 >= 12){ /* DIRECT, DIRECT */
                                        sstm << "DIR";
                                        name = sstm.str();
                                        char *name_ =  new char[name.length() + 1];
                                        strcpy(name_, name.c_str());
                                        stmts = instantiate(pc, name_, new Const(op1), new Const(op2));
                                    }

                                    break;
                                }
                                case 1: /* DIRECT, DIRECT */
                                {    sstm << "DIR";
                                    name = sstm.str();
                                    char *name_ =  new char[name.length() + 1];
                                    strcpy(name_, name.c_str());
                                    stmts = instantiate(pc, name_, new Const(op1), new Const(arg2->value.i));
                                    break;
                                }
                                case 4: /* DIRECT, IMM */
                                {    sstm << "IMM";
                                    name = sstm.str();
                                    char *name_ =  new char[name.length() + 1];
                                    strcpy(name_, name.c_str());
                                    stmts = instantiate(pc, name_ , new Const(op1), new Const(arg2->value.i));
                                    break;
                                }
                                default:
                                    break;   
                                
                            }
                        }   
                    break;
                }
                case 1: /* MOV DIRECT */
                {   
                    std::string name = "MOV_DIR_";
                    std::stringstream sstm;
                    sstm << name;
                    switch(arg2->kind){
                        case 3: /* DIRECT, INDIRECT*/
                        {    op2 = magic_process(std::string(arg2->value.c));
                            if (op2 == 0)
                                sstm << "RI" << op2;
                            else if(op2 == 1)
                                sstm << "RI" << op2;
                            name = sstm.str();
                            char *name_ =  new char[name.length() + 1];
                            strcpy(name_, name.c_str());
                            stmts = instantiate(pc, name_, new Const(op1)); 
                            break;
                        }
                        case 6: /* DIRECT, Rn | DIRECT */
                        {    op2 = magic_process(std::string(arg2->value.c));
                            if (op2 < 8 ){ /* DIRECT, Rn */
                                 sstm << "R" << op2;
                                name = sstm.str();
                                char *name_ =  new char[name.length() + 1];
                                strcpy(name_, name.c_str());
                                stmts = instantiate(pc, name_, new Const(op1));
                            }
                            else if (op2 == 8 ){ /* DIRECT, A */
                                sstm << "A";
                                name = sstm.str();
                                char *name_ =  new char[name.length() + 1];
                                strcpy(name_, name.c_str());
                                stmts = instantiate(pc, name_, new Const(op1));
                            }
                            else if ((op2 >= 9 && op2 <= 10) || op2 >= 12){ /* DIRECT, DIRECT */
                                sstm << "DIR";
                                name = sstm.str();
                                char *name_ =  new char[name.length() + 1];
                                strcpy(name_, name.c_str());
                                stmts = instantiate(pc, name_, new Const(op1), new Const(op2));
                            }

                            break;
                        }
                        case 1: /* DIRECT, DIRECT */
                        {    sstm << "DIR";
                            name = sstm.str();
                            char *name_ =  new char[name.length() + 1];
                            strcpy(name_, name.c_str());
                            stmts = instantiate(pc, name_, new Const(op1), new Const(arg2->value.i));
                            break;
                        }
                        case 4: /* DIRECT, IMM */
                        {    sstm << "IMM";
                            name = sstm.str();
                            char *name_ =  new char[name.length() + 1];
                            strcpy(name_, name.c_str());
                            stmts = instantiate(pc, name_ , new Const(op1), new Const(arg2->value.i));
                            break;
                        }
                        default:
                            break;   
                        
                    }
                }
                default:
                    break;
            }
        }
        /*else if(tokens.at(0) == "MOVC"){
            if(tokens.at(4) == "DPTR")
            {
                stmts = instantiate(pc, "MOVC_A_AADDDPTR");
            }
            else
            if(tokens.at(4) == "PC")
            {
                stmts = instantiate(pc, "MOVC_A_AADDPC");
            }
        }*/
        else if(opcode == "MOVX"){
            switch(arg1->kind){
                case 3: /* MOVX INDIRECT, A*/
                    op1 = magic_process(std::string(arg1->value.c));
                    if (op1 == 0)
                        stmts = instantiate(pc, "MOVX_RI0_A");
                    else if (op1 == 1 )
                        stmts = instantiate(pc, "MOVX_RI1_A");
                    else if (op1 == 11)
                         stmts = instantiate(pc, "MOVX_DPTRA_A");
                    break;
                case 6: /*MOVX A, INDIRECT*/
                    op2 = magic_process(std::string(arg2->value.c));
                    if (op2 == 0)
                        stmts = instantiate(pc, "MOVX_A_RI0");
                    else if (op2 == 1 )
                        stmts = instantiate(pc, "MOVX_A_RI1");
                    else if (op2 == 11)
                        stmts = instantiate(pc, "MOVX_A_DPTRA");
                    break;
                case 0:
                case 1:
                case 2:
                case 5:
                case 4:
                case 7:
                case 8:
                    break;
                default:
                    break;
            }
        }
    }
    else if (opcode == "RET" || opcode == "RETI") {
        result.rtl = new RTL(pc, stmts);
        result.rtl->appendStmt(new ReturnStatement);
        result.type = DD;
    }
    else if (opcode == "JNB" || opcode == "JB" || opcode == "JBC") {
        ei = Line->expList->begin();
        AssemblyArgument* arg1 = (*ei)->argList.front();      
        if (opcode == "JB")
            stmts = instantiate(pc, "JB_DIR_IMM", access_bit(arg1->value.bit.reg,arg1->value.bit.pos), new Const(100));
        else if (opcode == "JNB")
            stmts = instantiate(pc, "JNB_DIR_IMM", access_bit(arg1->value.bit.reg,arg1->value.bit.pos), new Const(100));
        else if (opcode == "JBC"){} //TODO: 
            //stmts = instantiate(pc, "JBC_DIR_IMM", new Const(arg1->value.i), new Const(100));
 
        result.rtl = new RTL(pc, stmts); 
        BranchStatement* jump = new BranchStatement; 
        result.rtl->appendStmt(jump); 
        result.numBytes = 4; 
        jump->setDest(pc + (Line->offset+1)*4);
        jump->setCondType(BRANCH_JE);
    }
    else if (opcode == "SETB"){
        ei = Line->expList->begin();
        AssemblyArgument* arg1 = (*ei)->argList.front();
        stmts = instantiate(pc, "SETB_DIR", access_bit(arg1->value.bit.reg,arg1->value.bit.pos));
        
    }
    else if (opcode == "ORL" || opcode == "ANL" || opcode == "XRL") {
        ei = Line->expList->begin();
        AssemblyArgument* arg1 = (*ei)->argList.front();
        ei++;
        AssemblyArgument* arg2 = (*ei)->argList.front();
        unsigned op1, op2;

        std::stringstream sstm;
        sstm << opcode << "_";
        std::string name;
        switch(arg1->kind){
            case 6: /* A, C, DIRECT */
            {   op1 = magic_process(std::string(arg1->value.c));
                if (op1 == 10){ /*C*/
                    sstm << "C_DIR";
                    name = sstm.str();
                    char *name_ =  new char[name.length() + 1];
                    strcpy(name_, name.c_str());
                    stmts = instantiate(pc,name_, new Const(arg2->value.i));
                }
                else if (op1 == 8){ /*A*/
                    sstm << "A_";
                    switch(arg2->kind){
                        case 3: /*A, INDIRECT*/
                        {   op2 = magic_process(arg2->value.c);
                            if (op2 == 0)
                                sstm << "RI0";
                            else if (op2 == 1)
                                sstm << "RI1";
                                name = sstm.str();
                                char *name_ =  new char[name.length() + 1];
                                strcpy(name_, name.c_str());
                                stmts = instantiate(pc,name_);
                            break;
                        }
                        case 6: /*A, Rn | DIRECT*/
                        {   
                            op2 = magic_process(arg2->value.c);
                            if (op2 < 8){
                                sstm << "R" << op2;
                                name = sstm.str();
                                char *name_ =  new char[name.length() + 1];
                                strcpy(name_, name.c_str());
                                stmts = instantiate(pc, name_);
                            }
                            else if (op2 >= 9){
                                sstm << "DIR";
                                name = sstm.str();
                                char *name_ =  new char[name.length() + 1];
                                strcpy(name_, name.c_str());
                                stmts = instantiate(pc, name_, new Const(op2));
                            }
                            break;
                        }
                        case 1: /* A, DIRECT*/
                        {   
                            sstm << "DIR";
                            name = sstm.str();
                            char *name_ =  new char[name.length() + 1];
                            strcpy(name_, name.c_str());
                            stmts = instantiate(pc, name_, new Const(arg2->value.i));
                            break;
                        }
                        case 4: /*A, IMM */
                        {
                            sstm << "IMM";
                            name = sstm.str();
                            char *name_ =  new char[name.length() + 1];
                            strcpy(name_, name.c_str());
                            stmts = instantiate(pc, name_, new Const(arg2->value.i));
                            break;
                        }
                        default:
                            break;
                    }
                }
                else if (op1 >= 9){ /*DIRECT*/
                    sstm << "DIR_";
                    switch(arg2->kind){
                        case 6: /*JUST DIRECT, A*/
                        {   sstm << "A";
                            char *name_ =  new char[name.length() + 1];
                            strcpy(name_, name.c_str());
                            stmts = instantiate(pc, name_, new Const(op1));
                            break;
                        }
                        case 4: /*DIRECT, IMM*/
                        {
                            sstm << "IMM";
                            name = sstm.str();
                            char *name_ =  new char[name.length() + 1];
                            strcpy(name_, name.c_str());
                            stmts = instantiate(pc, name_,new Const(op1), new Const(arg2->value.i));
                            break;
                        }
                        default:
                            break;
                    }
                }    
                break;
            }
            case 1 : /*DIRECT*/
            {   sstm << "DIR_";
                switch(arg2->kind){
                    case 6: /*JUST DIRECT, A*/
                    {   sstm << "A";
                        char *name_ =  new char[name.length() + 1];
                        strcpy(name_, name.c_str());
                        stmts = instantiate(pc, name_, new Const(arg1->value.i));
                        break;
                    }
                    case 4: /*DIRECT, IMM*/
                    {
                        sstm << "IMM";
                        name = sstm.str();
                        char *name_ =  new char[name.length() + 1];
                        strcpy(name_, name.c_str());
                        stmts = instantiate(pc, name_,new Const(arg1->value.i), new Const(arg2->value.i));
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            default:
                break;
        }
    }
    else if (opcode == "CLR") {
        ei = Line->expList->begin();
        AssemblyArgument* arg1 = (*ei)->argList.front();
        unsigned op1;
        switch(arg1->kind){
            case 6: /* A, C*/
            {   op1 = magic_process(std::string(arg1->value.c));
                if(op1 == 8)
                    stmts = instantiate(pc, "CLR_A");
                else if(op1 == 10)
                    stmts = instantiate(pc, "CLR_C");
                break;
            }
            case 8: /* BIT */
            {   stmts = instantiate(pc, "CLR_DIR", access_bit(arg1->value.bit.reg,arg1->value.bit.pos));  
                break;
            }
            default:
                break;
        }
    }
    else if (opcode == "NOP") {

    }
    else if (opcode == "ACALL" || opcode == "LCALL") {
        ei = Line->expList->begin();
        AssemblyArgument* arg1 = (*ei)->argList.front();
        bool is_lib = false;
        ADDRESS address;
        std::map<ADDRESS, const char*>::iterator it;
       
        for(it = namesList.begin();it != namesList.end(); ++it ){
            if (strcmp(it->second,arg1->value.c) == 0){
                address = it->first;
                break;
            }
        }
        CallStatement* newCall = new CallStatement;
        ADDRESS nativeDest = address - delta;
        newCall->setDest(nativeDest);
        Proc* destProc;
        funcsType[address] = is_lib;
        destProc = prog->newProc(arg1->value.c, nativeDest, is_lib);
        newCall->setDestProc(destProc);
        result.rtl = new RTL(pc, stmts);
        result.rtl->appendStmt(newCall);
        result.type = SD;
    }
    else
    {
        std::cout << "ELSE " << opcode << std::endl;
        stmts = NULL;
        result.valid = false;
        result.numBytes = 4;
    }
    result.numBytes = 4;

    if(result.valid && result.rtl == 0)
        result.rtl = new RTL(pc, stmts);

    //ADDED ONE-BYTE REGISTER at the first time
    if(first_line){ 
        std::list<Statement*>* temp = initial_bit_regs();
        std::list<Statement*>::iterator li;
        for(li = temp->begin(); li != temp->end(); ++li){
            result.rtl->appendStmt((*li));
        }

        first_line = false;
    }
    return result;
}

_8051Decoder::_8051Decoder(Prog* prog) : NJMCDecoder(prog)
{
    std::string file = Boomerang::get()->getProgPath() + "frontend/machine/_8051/_8051.ssl";
    RTLDict.readSSLFile(file.c_str());
}
int _8051Decoder::decodeAssemblyInstruction(unsigned,int)
{
    return 0;
}
