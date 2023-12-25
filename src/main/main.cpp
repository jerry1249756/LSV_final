#include <iostream>
#include "gvAbcMgr.h"
#include "base/abc/abc.h"
#include "gvAbcNtk.h"

#include "./../est/exec.h"
#include "./../sim/simulate.h"
#include "./../SA/core.h"
#include "./../prp/propagate.h"
#include "./../fec/fec.h"


#include "sat/cnf/cnf.h"

extern "C"{ 
  Aig_Man_t* Abc_NtkToDar( Abc_Ntk_t * pNtk, int fExors, int fRegisters );
}

extern AbcMgr* abcMgr;

void abccmd(string);



int main(int argc, char** argv) {
    
    // abcMgr = new AbcMgr;
    // string map = "./testcases/mul.blif";
    // string map = "./testcases/test.blif";

    // // abcMgr->abcReadDesign(lib);
    // abcMgr->abcReadDesign(map);
    // abccmd("strash");
    // Abc_Ntk_t* pNtk = abcMgr->get_Abc_Frame_t()->pNtkCur;
    // abccmd("show");
    // Abc_Obj_t* pNode = Abc_NtkFindNode(pNtk, "n18");
    // Abc_Obj_t* pNode2 = ChoosePropagate(pNode);
    // cout << "replace: " << Abc_ObjName(pNode2) << endl;


    std::srand(static_cast<unsigned int>(std::time(nullptr))); 
    abcMgr = new AbcMgr;
    abccmd("read ./testcases/mcnc-aig.genlib");
    // string lib = "./testcases/mcnc.genlib";
    // string map = "-m ./testcases/c7552.blif";
    string map = "./testcases/mem_ctrl.blif";

    // abcMgr->abcReadDesign(lib);
    abcMgr->abcReadDesign(map);
    abccmd("strash");
    abccmd("fraig");
    Abc_Ntk_t* pNtk_cur = abcMgr->get_Abc_Frame_t()->pNtkCur;
    Abc_Ntk_t* pNtk_orig = Abc_NtkDup(pNtk_cur);
    vector<vector<Abc_Obj_t*>> fec_grps;
    vector<Abc_Obj_t*> fec_grp;
    Abc_Obj_t* pNode;
    int i;
    Abc_NtkForEachObj(pNtk_cur, pNode, i) {
      if (!Abc_ObjIsPo(pNode)) {
        fec_grp.push_back(pNode);
      }
    }
    fec_grps.push_back(fec_grp);
    Fec(pNtk_cur, fec_grps);

    simulated_annealing(pNtk_orig, pNtk_cur);
    return 0;
}




