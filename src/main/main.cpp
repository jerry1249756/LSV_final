#include <iostream>
#include "gvAbcMgr.h"
#include "base/abc/abc.h"
#include "gvAbcNtk.h"

#include "./../est/exec.h"
#include "./../SA/change_node.cpp"
#include "./../sim/simulate.h"

#include "sat/cnf/cnf.h"

extern "C"{ 
  Aig_Man_t* Abc_NtkToDar( Abc_Ntk_t * pNtk, int fExors, int fRegisters );
}

extern AbcMgr* abcMgr;

void abccmd(string);



int main(int argc, char** argv) {
    abcMgr = new AbcMgr;
    string a = "./testcases/square.blif";
    abcMgr->abcReadDesign(a);

    Abc_Ntk_t* pNtk = abcMgr->get_Abc_Frame_t()->pNtkCur;
    Abc_Obj_t* miter_node = SelectChoiceNode(pNtk);

    // abccmd("show");
    abccmd("strash");
    pNtk = abcMgr->get_Abc_Frame_t()->pNtkCur;
    abccmd("show");
    

    // // calling abc's command
    // // abccmd("write_cnf temp.cnf");
    Simulation(pNtk, pNtk);
    test_function(pNtk, "n1220");

    return 0;
}

void abccmd(string command){
    char Command[1024], abcCmd[128];
    strcpy(abcCmd, command.c_str());
    sprintf(Command, "%s", abcCmd);
    Cmd_CommandExecute(abcMgr->get_Abc_Frame_t(), Command);
}


