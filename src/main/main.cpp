#include <iostream>
#include "gvAbcMgr.h"
#include "base/abc/abc.h"
#include "gvAbcNtk.h"

#include "./../est/exec.h"
#include "./../SA/change_node.h"
#include "./../sim/simulate.h"

#include "sat/cnf/cnf.h"

extern "C"{ 
  Aig_Man_t* Abc_NtkToDar( Abc_Ntk_t * pNtk, int fExors, int fRegisters );
}

extern AbcMgr* abcMgr;

void abccmd(string);



int main(int argc, char** argv) {
    abcMgr = new AbcMgr;
    abccmd("read ./testcases/mcnc.genlib");
    string lib = "./testcases/mcnc.genlib";
    string map = "-m ./testcases/c1908.blif";
    // abcMgr->abcReadDesign(lib);
    abcMgr->abcReadDesign(map);

    Abc_Ntk_t* pNtk = abcMgr->get_Abc_Frame_t()->pNtkCur;
    abccmd("strash");
    Abc_Ntk_t* pNtk_orig = abcMgr->get_Abc_Frame_t()->pNtkCur;
    pNtk = Abc_NtkDup(pNtk_orig);

    // Abc_Obj_t* miter_node = SelectChoiceNode(pNtk);
    
    // abccmd("show");
    // abccmd("show");
    
    Abc_Obj_t* pNode;
    Abc_Obj_t* pNodeReplace;

    // // calling abc's command
    // // abccmd("write_cnf temp.cnf");
    int obj_num = Abc_NtkObjNum(pNtk);
    // pNode = Abc_NtkFindNode(pNtk, "n12");
    // pNode = Abc_NtkObj(pNtk, obj_num/2);
    // pNode = Abc_ObjFanout0(Abc_ObjFanout0(Abc_ObjFanout0(Abc_NtkPi(pNtk,5))));
    pNode = (Abc_NtkPi(pNtk,18));
    for (int j = 0; j < 6; j++){
      pNode = Abc_ObjFanout0(pNode);
    }
    
    // Abc_Aig_t* abc_aig = static_cast <Abc_Aig_t *> (pConeDup->pManFunc);
    // Abc_Obj_t * pXor = Abc_AigXor(abc_aig, (Abc_ObjChild0(pNodeReplace)), (Abc_ObjChild1(pNodeReplace)));
    // pXor = Abc_ObjRegular(pXor);
    // Abc_AigReplace(abc_aig, pNodeReplace, pXor, 1);

    // Aig_Man_t *pAig = Abc_NtkToDar(cone_k, 0, 1);//aig manager
    // Cnf_Dat_t *pCnf = Cnf_Derive(pAig, 1);
    // UpdateNtk_toggle_input(pNtk, pNode, 1, 0);
    UpdateNtk_const0_propagate(pNtk, pNode, 1);
    Simulation(pNtk_orig, pNtk, argv[1]);
    // Abc_FrameReplaceCurrentNetwork(abcMgr->get_Abc_Frame_t(), pConeDup);
    // abccmd("show");
    // cout << Abc_ObjIsComplement(Abc_ObjFanin0(pNode));
    // cout << Abc_ObjIsComplement(Abc_ObjFanin1(pNode));

    // test_function(pNtk, miter_node);
    cout << Abc_AigLevel(pNtk) << endl;

    return 0;
}




