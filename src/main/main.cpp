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
    abcMgr = new AbcMgr;
    // abccmd("read ./testcases/mcnc.genlib");
    // string lib = "./testcases/mcnc.genlib";
    // string map = "-m ./testcases/c1908.blif";
    string map = "./testcases/square.blif";

    // abcMgr->abcReadDesign(lib);
    abcMgr->abcReadDesign(map);
    abccmd("strash");

    Abc_Ntk_t* pNtk_cur = abcMgr->get_Abc_Frame_t()->pNtkCur;
    Abc_Ntk_t* pNtk_orig = Abc_NtkDup(pNtk_cur);
    simulated_annealing(pNtk_orig, pNtk_cur);
    // test_function(pNtk, miter_node);

    return 0;
}




