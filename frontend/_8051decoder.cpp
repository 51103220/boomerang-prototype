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
    std::string str = name;
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
    else if (isInteger(str)) {
        int i = std::atoi((str).c_str());
        if (i < 0) return i;
        else return 100 + i;
    }
}

unsigned map_sfr(std::string name){
    if (name == "A") return 224;
    else if (name == "B") return 240;
    else if (name == "P0") return 128;
    else if (name == "P1") return 144;
    else if (name == "P2") return 160;
    else if (name == "P3") return 176;
    else if (name == "SP") return 129;
    else if (name == "DPL") return 130;
    else if (name == "DPH") return 131;
    else if (name == "PCON") return 135;
    else if (name == "TCON") return 136;
    else if (name == "TMOD") return 137;
    else if (name == "TL0") return 138;
    else if (name == "TL1") return 139;
    else if (name == "TH0") return 140;
    else if (name == "TH1") return 141;
    else if (name == "SCON") return 152;
    else if (name == "SBUF") return 153;
    else if (name == "IE") return 168;
    else if (name == "IP") return 184;
    else if (name == "PSW") return 208;
    else
    return 0;
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

DecodeResult& _8051Decoder::decodeAssembly(ADDRESS pc,std::string line)
{
    static DecodeResult result;
    int delta = 0;
    ADDRESS hostPC = pc+delta;
    unsigned u_constant = 4000000000u;

    result.reset();
    std::list<Statement*>* stmts = NULL;
    std::string sentence = line;
    std::transform(sentence.begin(), sentence.end(),sentence.begin(), ::toupper);
    sentence.erase(std::remove(sentence.begin(), sentence.end(), ','), sentence.end());
    std::istringstream iss(sentence);
    std::vector <std::string> tokens;
    copy(istream_iterator<std::string>(iss),istream_iterator<std::string>(),back_inserter(tokens));
    ADDRESS nextPC = NO_ADDRESS;
    //ADDRESS nextPC = NO_ADDRESS;
    dword MATCH_p = hostPC;
    if (tokens.at(0) == "MOV" || tokens.at(0) == "MOVC" || tokens.at(0) == "MOVX") {
        unsigned op1 = magic_process(tokens.at(1));
        unsigned op2 = magic_process(tokens.at(2));
        if(tokens.at(0) == "MOV")
        {
            if(tokens.at(1) == "@R0")
            {
                if(op2 >= 13 && op2 <= 16) //TODO:
                {   
                    stmts = instantiate(pc, "MOV_RI0_DIR", Location::regOf(op2));
                }
                else
                if(op2 == 8)
                {
                    stmts = instantiate(pc, "MOV_RI0_A");
                }
                else
                if(op2 >= 100 )
                {   
                    unsigned new_constant = op2-4294967296;
                    if(op2 < u_constant)
                    stmts = instantiate(pc, "MOV_RI0_IMM" , new Const(op2-100));
                    else
                    stmts = instantiate(pc,  "MOV_RI0_IMM", new Const(new_constant));
                }
            }
            else
            if(tokens.at(1) == "@R1")
            {
                if(op2 < op2 >= 13 && op2 <= 16) //TODO:
                {
                    stmts = instantiate(pc, "MOV_RI1_DIR", Location::regOf(op2));
                }
                else
                if(op2 == 8)
                {
                    stmts = instantiate(pc, "MOV_RI1_A");
                }
                else
                if(op2 >= 100 )
                {   unsigned new_constant = op2-4294967296;
                    if(op2 < u_constant  )
                    stmts = instantiate(pc, "MOV_RI1_IMM" , new Const(op2-100));
                    else
                    stmts = instantiate(pc,  "MOV_RI1_IMM", new Const(new_constant));
                }
            }
            else
            if(op1 < 8)
            {
                std::string name = "MOV_R";
                std::stringstream sstm;
                sstm << name << op1;
                if(op2 < 8 )
                {
                    sstm << "_DIR";
                    name = sstm.str();
                    char *name_ =  new char[name.length() + 1];
                    strcpy(name_, name.c_str());
                    stmts = instantiate(pc, name_, Location::regOf(op2));
                }
                else
                if(op2 == 8 )
                {
                    sstm << "_A";
                    name = sstm.str();
                    char *name_ =  new char[name.length() + 1];
                    strcpy(name_, name.c_str());
                    stmts = instantiate(pc, name_);
                }
                else
                if(op2 >= 100 )
                {
                    sstm << "_IMM";
                    name = sstm.str();
                    unsigned new_constant = op2-4294967296;
                    char *name_ =  new char[name.length() + 1];
                    strcpy(name_, name.c_str());
                    if(op2 < u_constant  )
                    stmts = instantiate(pc, name_ , new Const(op2-100));
                    else
                    stmts = instantiate(pc, name_, new Const(new_constant));
                }
            }
            else
            if(op1 == 8)
            {
                if(tokens.at(2) == "@R0")
                {
                    stmts = instantiate(pc, "MOV_A_RI0");
                }
                else
                if(tokens.at(2) == "@R1")
                {
                    stmts = instantiate(pc, "MOV_A_RI1");
                }
                else
                if(op2 < 8)
                {
                    std::string name = "MOV_A_R";
                    std::stringstream sstm;
                    sstm << name << op2;
                    name = sstm.str();
                    char *name_ =  new char[name.length() + 1];
                    strcpy(name_, name.c_str());
                    stmts = instantiate(pc, name_);
                }
                else
                if(op2 >= 100)
                {   unsigned new_constant = op2-4294967296;
                    if(op2 < u_constant  )
                    stmts = instantiate(pc, "MOV_A_IMM", new Const(op2-100));
                    else
                    stmts = instantiate(pc, "MOV_A_IMM", new Const(new_constant));
                }
                else{
                    std::string name = "MOV_A_DIR";
                    char *name_ =  new char[name.length() + 1];
                    strcpy(name_, name.c_str());
                    stmts = instantiate(pc, name_ , Location::regOf(op2));
                }
            }
            else
            if(op1 >= 13 && op1 <= 16 )
            {
                std::string name = "MOV_DIR_";
                std::stringstream sstm;
                sstm << name;
                if(op2 < 8 )
                {
                    sstm << "R" << op2;
                    name = sstm.str();
                    char *name_ =  new char[name.length() + 1];
                    strcpy(name_, name.c_str());
                    stmts = instantiate(pc, name_, Location::regOf(op1));
                }
                else
                if(op2 == 8 )
                {
                    sstm << "A";
                    name = sstm.str();
                    char *name_ =  new char[name.length() + 1];
                    strcpy(name_, name.c_str());
                    stmts = instantiate(pc, name_, Location::regOf(op1) );
                }
                else
                if(op2 >= 100 )
                {   
                    unsigned new_constant = op2-4294967296;
                    sstm << "IMM";
                    name = sstm.str();
                    char *name_ =  new char[name.length() + 1];
                    strcpy(name_, name.c_str());
                    if(op2 < u_constant  )
                    stmts = instantiate(pc, name_ , Location::regOf(op1), new Const(op2-100));
                    else
                    stmts = instantiate(pc, name_,  Location::regOf(op1), new Const(new_constant));
                }
            }
            else {

                    std::string name = "MOV_REG_IMM";
                    char *name_ =  new char[name.length() + 1];
                    strcpy(name_, name.c_str());
                    unsigned new_constant = op2-4294967296;
                    if(op2 < u_constant  )
                    stmts = instantiate(pc, name_ , Location::regOf(op1), new Const(op2-100));
                    else
                    stmts = instantiate(pc, name_,  Location::regOf(op1), new Const(new_constant));
            }
        }
        else
        if(tokens.at(0) == "MOVC")
        {
            if(tokens.at(4) == "DPTR")
            {
                stmts = instantiate(pc, "MOVC_A_AADDDPTR");
            }
            else
            if(tokens.at(4) == "PC")
            {
                stmts = instantiate(pc, "MOVC_A_AADDPC");
            }
        }
        else
        if(tokens.at(0) == "MOVX")
        {
            if(tokens.at(1) == "@R0")
            {
                stmts = instantiate(pc, "MOVX_RI0_A");
            }
            else
            if(tokens.at(1) == "@R1")
            {
                stmts = instantiate(pc, "MOVX_RI1_A");
            }
            else
            if(op1 == 8)
            {
                if(tokens.at(2) == "@R0")
                {
                    stmts = instantiate(pc, "MOVX_A_RI0");
                }
                else
                if(tokens.at(2) == "@R1")
                {
                    stmts = instantiate(pc, "MOVX_A_RI1");
                }
                else
                if(tokens.at(2) == "@DPTR")
                {
                    stmts = instantiate(pc, "MOVX_A_DPTRA");
                }
            }
        }
    }
    else if (tokens.at(0) == "ACALL" || tokens.at(0) == "LCALL") {
        unsigned address = magic_process(tokens.at(1));
        bool is_lib = false;
        if(tokens.at(1) == "PRINTF" || tokens.at(1) == "PUTS")
        {
            address = 132912;
            is_lib = true;
        }
        else
        {
            address= 66752;
        }
        CallStatement* newCall = new CallStatement;
        ADDRESS nativeDest = address - delta;
        newCall->setDest(nativeDest);
        Proc* destProc;
        std::transform(tokens.at(1).begin(), tokens.at(1).end(),tokens.at(1).begin(), ::tolower);
        char *name =  new char[tokens.at(1).length() + 1];
        strcpy(name, tokens.at(1).c_str());
        namesList[address] = name;
        funcsType[address] = is_lib;
        destProc = prog->newProc(name, nativeDest, is_lib);
        newCall->setDestProc(destProc);
        result.rtl = new RTL(pc, stmts);
        result.rtl->appendStmt(newCall);
        result.type = SD;
    }
    else if (tokens.at(0) == "ADD" || tokens.at(0) == "ADDC" || tokens.at(0) == "SUBB") {
        unsigned op1 = magic_process(tokens.at(1));
        unsigned op2 = magic_process(tokens.at(2));
        std::stringstream sstm;
        sstm << tokens.at(0) << "_A_";
        std::string name;
        if(tokens.at(2) == "@R0")
        {
            sstm << "RI0";
            name = sstm.str();
            char *name_ =  new char[name.length() + 1];
            strcpy(name_, name.c_str());
            stmts = instantiate(pc,name_);
        }
        else
        if(tokens.at(2) == "@R1")
        {
            sstm << "RI1";
            name = sstm.str();
            char *name_ =  new char[name.length() + 1];
            strcpy(name_, name.c_str());
            stmts = instantiate(pc,name_);
        }
        else
        if(op2 < 8)
        {
            sstm << "R" << op2;
            name = sstm.str();
            char *name_ =  new char[name.length() + 1];
            strcpy(name_, name.c_str());
            stmts = instantiate(pc, name_);
        }
        else
        if(op2 >= 13 && op2 <= 16)
        {
            sstm << "DIR";
            name = sstm.str();
            char *name_ =  new char[name.length() + 1];
            strcpy(name_, name.c_str());
            stmts = instantiate(pc, name_, Location::regOf(op2));
        }
        else
        if(op2 >= 100)
        {   
            
            unsigned new_constant = op2-4294967296;
            sstm << "IMM";
            name = sstm.str();
            char *name_ =  new char[name.length() + 1];
            strcpy(name_, name.c_str());
            if(op2 < u_constant  )
            stmts = instantiate(pc, name_, new Const(op2-100));
            else
            stmts = instantiate(pc, name_, new Const(new_constant));
        }
    }
    else if (tokens.at(0) == "NOP" ) {
        result.type = NOP;
        stmts = instantiate(pc,  "NOP");
    }
    else if (tokens.at(0) == "LJMP" || tokens.at(0) == "AJMP" || tokens.at(0) == "SJMP") {
        unsigned address = magic_process(tokens.at(1));
        CaseStatement* jump = new CaseStatement;
        jump->setIsComputed();
        result.rtl = new RTL(pc, stmts);
        result.rtl->appendStmt(jump);
        result.type = DD;
        jump->setDest(address);
    }
    else if (tokens.at(0) == "JMP" || tokens.at(0) == "JMP") {
        stmts = instantiate(pc,  "JMP_AADDDPTR");
    }
    else if (tokens.at(0) == "RR" || tokens.at(0) == "RRC" || tokens.at(0) == "RLC") {
        unsigned op1 = magic_process(tokens.at(1));
        if(op1 == 8)
        {
            std::stringstream sstm;
            sstm << tokens.at(0) << "_A";
            std::string name;
            name = sstm.str();
            char *name_ =  new char[name.length() + 1];
            strcpy(name_, name.c_str());
            stmts = instantiate(pc,  name_);
        }
    }
    else if (tokens.at(0) == "DEC" || tokens.at(0) == "INC") {
        unsigned op1 = magic_process(tokens.at(1));
        std::stringstream sstm;
        sstm << tokens.at(0) << "_";
        std::string name;
        if(tokens.at(1) == "@R0")
        {
            sstm << "RI0";
            name = sstm.str();
            char *name_ =  new char[name.length() + 1];
            strcpy(name_, name.c_str());
            stmts = instantiate(pc,name_);
        }
        else
        if(tokens.at(1) == "@R1")
        {
            sstm << "RI1";
            name = sstm.str();
            char *name_ =  new char[name.length() + 1];
            strcpy(name_, name.c_str());
            stmts = instantiate(pc,name_);
        }
        else
        if(tokens.at(1) == "DPTR")
        {
            sstm << "DPTR";
            name = sstm.str();
            char *name_ =  new char[name.length() + 1];
            strcpy(name_, name.c_str());
            stmts = instantiate(pc, name_);
        }
        else
        if(op1 < 8)
        {
            sstm << "R" << op1;
            name = sstm.str();
            char *name_ =  new char[name.length() + 1];
            strcpy(name_, name.c_str());
            stmts = instantiate(pc, name_);
        }
        else
        if(op1 == 8)
        {
            sstm << "A";
            name = sstm.str();
            char *name_ =  new char[name.length() + 1];
            strcpy(name_, name.c_str());
            stmts = instantiate(pc, name_);
        }
        else
        if(op1 >= 100)
        {
            unsigned new_constant = op1-4294967296;
            sstm << "DIR";
            name = sstm.str();
            char *name_ =  new char[name.length() + 1];
            strcpy(name_, name.c_str());
            if(op1 < u_constant  )
            stmts = instantiate(pc, name_, new Const(op1-100));
            else
            stmts = instantiate(pc, name_, new Const(new_constant));
        }
    }
    else if (tokens.at(0) == "JNB" || tokens.at(0) == "JB" || tokens.at(0) == "JBC") {
        unsigned op1 = magic_process(tokens.at(1));
        unsigned op2 = magic_process(tokens.at(2));
        std::stringstream sstm;
        sstm << tokens.at(0) << "_DIR_IMM";
        std::string name;
        unsigned new_constant = op2-4294967296;
        name = sstm.str();
        char *name_ =  new char[name.length() + 1];
        strcpy(name_, name.c_str());
        if(op2 < u_constant  )
        stmts = instantiate(pc, name_, Location::regOf(op1), new Const(op2-100));
        else
        stmts = instantiate(pc, name_,  Location::regOf(op1),new Const(new_constant));
    }
    else if (tokens.at(0) == "LCALL" || tokens.at(0) == "LCALL") {
        unsigned address = magic_process(tokens.at(1));
        bool is_lib = false;
        if(tokens.at(1) == "PRINTF" || tokens.at(1) == "PUTS")
        {
            address = 132912;
            is_lib = true;
        }
        else
        {
            address= 66752;
        }
        CallStatement* newCall = new CallStatement;
        ADDRESS nativeDest = address - delta;
        newCall->setDest(nativeDest);
        Proc* destProc;
        std::transform(tokens.at(1).begin(), tokens.at(1).end(),tokens.at(1).begin(), ::tolower);
        char *name =  new char[tokens.at(1).length() + 1];
        strcpy(name, tokens.at(1).c_str());
        destProc = prog->newProc(name, nativeDest, is_lib);
        newCall->setDestProc(destProc);
        result.rtl = new RTL(pc, stmts);
        result.rtl->appendStmt(newCall);
        result.type = SD;
    }
    else if (tokens.at(0) == "RET" || tokens.at(0) == "RETI") {
        result.rtl = new RTL(pc, stmts);
        result.rtl->appendStmt(new ReturnStatement);
        result.type = DD;
    }
    else if (tokens.at(0) == "JC" || tokens.at(0) == "JC") {
        unsigned op1 = magic_process(tokens.at(1));
        unsigned new_constant = op1-4294967296;
        if(op1 < u_constant  )
        stmts = instantiate(pc, "JC_IMM", new Const(op1-100));
        else
        stmts = instantiate(pc, "JC_IMM", new Const(new_constant));
    }
    else if (tokens.at(0) == "JNC" || tokens.at(0) == "JNC") {
        unsigned op1 = magic_process(tokens.at(1));
        unsigned new_constant = op1-4294967296;
        if(op1 < u_constant  )
        stmts = instantiate(pc, "JC_IMM", new Const(op1-100));
        else
        stmts = instantiate(pc, "JC_IMM", new Const(new_constant));
    }
    else if (tokens.at(0) == "ORL" || tokens.at(0) == "ANL" || tokens.at(0) == "XRL") {
        unsigned op1 = magic_process(tokens.at(1));
        unsigned op2 = magic_process(tokens.at(2));
        std::stringstream sstm;
        sstm << tokens.at(0) << "_";
        std::string name;
        if(op1 == 10 )
        {
            sstm << "C_";
            name = sstm.str();
            char *name_ =  new char[name.length() + 1];
            strcpy(name_, name.c_str());
            stmts = instantiate(pc,name_, new Const(op2));
        }
        if(op1 == 8)
        {
            sstm << "A_";
            if(tokens.at(2) == "@R0")
            {
                sstm << "RI0";
                name = sstm.str();
                char *name_ =  new char[name.length() + 1];
                strcpy(name_, name.c_str());
                stmts = instantiate(pc,name_);
            }
            else
            if(tokens.at(2) == "@R1")
            {
                sstm << "RI1";
                name = sstm.str();
                char *name_ =  new char[name.length() + 1];
                strcpy(name_, name.c_str());
                stmts = instantiate(pc,name_);
            }
            else
            if(op2 < 8)
            {
                sstm << "R" << op2;
                name = sstm.str();
                char *name_ =  new char[name.length() + 1];
                strcpy(name_, name.c_str());
                stmts = instantiate(pc, name_);
            }
            else
            if(op2 >= 100)
            {
                unsigned new_constant = op2-4294967296;
                sstm << "IMM";
                name = sstm.str();
                char *name_ =  new char[name.length() + 1];
                strcpy(name_, name.c_str());
                if(op2 < u_constant  )
                stmts = instantiate(pc, name_, new Const(op2-100));
                else
                stmts = instantiate(pc, name_, new Const(new_constant));
            }
            else
            if(op2 >= 13 && op2 <= 16)
            {
                sstm << "DIR";
                name = sstm.str();
                char *name_ =  new char[name.length() + 1];
                strcpy(name_, name.c_str());
                stmts = instantiate(pc, name_, Location::regOf(op2));
            }
        }
        else
        if(op1 >= 13 && op1 <= 16 )
        {
            sstm << "DIR_";
            if(op2 == 8)
            {
                sstm << "A";
                char *name_ =  new char[name.length() + 1];
                strcpy(name_, name.c_str());
                stmts = instantiate(pc, name_, Location::regOf(op1));
            }
            else
            if(op2 >= 100)
            {
                unsigned new_constant = op2-4294967296;
                sstm << "IMM";
                name = sstm.str();
                char *name_ =  new char[name.length() + 1];
                strcpy(name_, name.c_str());
                if(op2 < u_constant  )
                stmts = instantiate(pc, name_,Location::regOf(op1), new Const(op2-100));
                else
                stmts = instantiate(pc, name_,Location::regOf(op1), new Const(new_constant));
            }
        }
    }
    else if (tokens.at(0) == "JZ" || tokens.at(0) == "JNZ") {
        unsigned address = magic_process(tokens.at(1));
        std::stringstream sstm;
        sstm << tokens.at(0) << "_IMM";
        std::string name;
        name = sstm.str();
        char *name_ =  new char[name.length() + 1];
        strcpy(name_, name.c_str());
        stmts = instantiate(pc,name_, new Const(address));
    }
    else if (tokens.at(0) == "DIV" || tokens.at(0) == "MUL") {
        unsigned op1 = magic_process(tokens.at(1));
        if(op1 == 12)
        {
            std::stringstream sstm;
            sstm << tokens.at(0) << "_AB";
            std::string name;
            name = sstm.str();
            char *name_ =  new char[name.length() + 1];
            strcpy(name_, name.c_str());
            stmts = instantiate(pc,name_);
        }
    }
    else if (tokens.at(0) == "CPL" || tokens.at(0) == "CPL") {
        unsigned op1 = magic_process(tokens.at(1));
        if(op1 == 8)
        {
            stmts = instantiate(pc,"CPL_A");
        }
        else
        if(op1 == 10)
        {
            stmts = instantiate(pc,"CPL_C");
        }
        else
        if(op1 >= 100)
        {
            unsigned new_constant = op1-4294967296;
            if(op1 < u_constant  )
            stmts = instantiate(pc, "CPL_DIR", new Const(op1-100));
            else
            stmts = instantiate(pc, "CPL_DIR", new Const(new_constant));
        }
    }
    else if (tokens.at(0) == "CJNE" || tokens.at(0) == "CJNE") {
        unsigned op1 = magic_process(tokens.at(1));
        unsigned op2 = magic_process(tokens.at(2));
        unsigned op3 = magic_process(tokens.at(3));
        if(tokens.at(0) == "@R0")
        {
            if(op2 >= 100 && op3 >= 100)
            {
                int operand2 = 0;
                int operand3 = 0;
                if(op2 < u_constant  )
                operand2 = op2 -100;
                else
                operand2 = op2 - 4294967296;
                if(op3 < u_constant  )
                operand3 = op3 -100;
                else
                operand3 = op3 - 4294967296;
                stmts = instantiate(pc, "CJNE_RI0_IMM_IMM", new Const(operand2), new Const(operand3));
            }
        }
        else
        if(tokens.at(0) == "@R1")
        {
            if(op2 >= 100 && op3 >= 100)
            {
                int operand2 = 0;
                int operand3 = 0;
                if(op2 < u_constant  )
                operand2 = op2 -100;
                else
                operand2 = op2 - 4294967296;
                if(op3 < u_constant  )
                operand3 = op3 -100;
                else
                operand3 = op3 - 4294967296;
                stmts = instantiate(pc, "CJNE_RI1_IMM_IMM", new Const(operand2), new Const(operand3));
            }
        }
        else
        if(op1 < 8)
        {
            std::stringstream sstm;
            sstm << "CJNE_R" << tokens.at(0) << "_IMM_IMM";
            std::string name;
            name = sstm.str();
            char *name_ =  new char[name.length() + 1];
            strcpy(name_, name.c_str());
            if(op2 >= 100 && op3 >= 100)
            {
                int operand2 = 0;
                int operand3 = 0;
                if(op2 < u_constant  )
                operand2 = op2 -100;
                else
                operand2 = op2 - 4294967296;
                if(op3 < u_constant  )
                operand3 = op3 -100;
                else
                operand3 = op3 - 4294967296;
                stmts = instantiate(pc,name_, new Const(operand2), new Const(operand3));
            }
        }
        else
        if(op1 == 8)
        {
            if(op2 >= 100 && op3 >= 100)
            {
                int operand2 = 0;
                int operand3 = 0;
                if(op2 < u_constant  )
                operand2 = op2 -100;
                else
                operand2 = op2 - 4294967296;
                if(op3 < u_constant  )
                operand3 = op3 -100;
                else
                operand3 = op3 - 4294967296;
                stmts = instantiate(pc, "CJNE_A_IMM_IMM", new Const(operand2), new Const(operand3));
            }
            else
            {
            }
        }
    }
    else if (tokens.at(0) == "PUSH" || tokens.at(0) == "POP") {
        unsigned op1 = magic_process(tokens.at(1));
        std::stringstream sstm;
        sstm << tokens.at(0) << "_DIR";
        std::string name;
        name = sstm.str();
        char *name_ =  new char[name.length() + 1];
        strcpy(name_, name.c_str());
        if(op1 < 8)
        {
            stmts = instantiate(pc, name_, Location::regOf(op1));
        }
        else
        {
            stmts = instantiate(pc, name_, new Const(op1));
        }
    }
    else if (tokens.at(0) == "CLR" || tokens.at(0) == "CLR") {
        unsigned op1 = magic_process(tokens.at(1));
        if(op1 == 8)
        {
            stmts = instantiate(pc, "CLR_A");
        }
        else
        if(op1 == 10)
        {
            stmts = instantiate(pc, "CLR_C");
        }
        else
        {
            stmts = instantiate(pc, "CLR_DIR", new Const(op1-100));
        }
    }
    else if (tokens.at(0) == "SETB" || tokens.at(0) == "SETB") {
        unsigned op1 = magic_process(tokens.at(1));
        if(op1 == 10)
        {
            stmts = instantiate(pc, "SETB_C");
        }
        else
        {
            stmts = instantiate(pc, "SETB_DIR", new Const(op1 - 100));
        }
    }
    else if (tokens.at(0) == "SWAP" || tokens.at(0) == "SWAP") {
        unsigned op1 = magic_process(tokens.at(1));
        if(op1 == 8)
        {
            stmts = instantiate(pc,"SWAP_A");
        }
    }
    else if (tokens.at(0) == "XCH" || tokens.at(0) == "XCHD") {
        unsigned op1 = magic_process(tokens.at(1));
        unsigned op2 = magic_process(tokens.at(2));
        std::stringstream sstm;
        sstm << tokens.at(0) << "_A_";
        std::string name;
        if(op1 == 8)
        {
            if(tokens.at(2) == "@R0")
            {
                sstm << "RI0";
                name = sstm.str();
                char *name_ =  new char[name.length() + 1];
                strcpy(name_, name.c_str());
                stmts = instantiate(pc, name_);
            }
            else
            if(tokens.at(2) == "@R1")
            {
                sstm << "RI1";
                name = sstm.str();
                char *name_ =  new char[name.length() + 1];
                strcpy(name_, name.c_str());
                stmts = instantiate(pc, name_);
            }
            else
            if(op2 < 8)
            {
                sstm << "R" << op2;
                name = sstm.str();
                char *name_ =  new char[name.length() + 1];
                strcpy(name_, name.c_str());
                stmts = instantiate(pc, name_);
            }
            else
            {
                sstm << "DIR";
                name = sstm.str();
                char *name_ =  new char[name.length() + 1];
                strcpy(name_, name.c_str());
                stmts = instantiate(pc, name_, new Const(op2-100));
            }
        }
    }
    else if (tokens.at(0) == "DA" || tokens.at(0) == "DA") {
        unsigned op1 = magic_process(tokens.at(1));
        if(op1 == 8)
        {
            stmts = instantiate(pc, "DA");
        }
    }
    else if (tokens.at(0) == "RL" || tokens.at(0) == "RL") {
        unsigned op1 = magic_process(tokens.at(1));
        if(op1 == 8)
        {
            stmts = instantiate(pc, "RL_A");
        }
    }
    else
    {
        std::cout << "ELSE\n";
        stmts = NULL;
        result.valid = false;
        result.numBytes = 4;
    }
    result.numBytes = 4;
    if(result.valid && result.rtl == 0)
    result.rtl = new RTL(pc, stmts);
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
