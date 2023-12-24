#include <iostream>
#include "gvAbcMgr.h"
#include "base/abc/abc.h"
#include "gvAbcNtk.h"

#include "./../est/exec.h"
#include "./../sim/simulate.h"
#include "./../SA/core.h"


#include "sat/cnf/cnf.h"

extern "C"{ 
  Aig_Man_t* Abc_NtkToDar( Abc_Ntk_t * pNtk, int fExors, int fRegisters );
}

extern AbcMgr* abcMgr;

void abccmd(string);



int main(int argc, char** argv) {
    
    // abcMgr = new AbcMgr;
    // string map = "./testcases/mul.blif";

    // // abcMgr->abcReadDesign(lib);
    // abcMgr->abcReadDesign(map);
    // abccmd("strash");
    // Abc_Ntk_t* pNtk = abcMgr->get_Abc_Frame_t()->pNtkCur;
    // Abc_Obj_t* pNode = Abc_NtkFindNode(pNtk, "n10");
    // cout << "fanout: " << Abc_ObjFanoutNum(pNode) << endl;
    


    std::srand(static_cast<unsigned int>(std::time(nullptr))); 
    abcMgr = new AbcMgr;
    abccmd("read ./testcases/mcnc-aig.genlib");
    // string lib = "./testcases/mcnc.genlib";
    // string map = "-m ./testcases/c7552.blif";
    string map = "./testcases/mem_ctrl.blif";

    // abcMgr->abcReadDesign(lib);
    abcMgr->abcReadDesign(map);
    abccmd("strash");

    abccmd("show");
    Abc_Ntk_t* pNtk_cur = abcMgr->get_Abc_Frame_t()->pNtkCur;
    Abc_Ntk_t* pNtk_orig = Abc_NtkDup(pNtk_cur);

    simulated_annealing(pNtk_orig, pNtk_cur);
    return 0;
}




