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
    string a = "./testcases/mul.blif";
    abcMgr->abcReadDesign(a);

    Abc_Ntk_t* pNtk = abcMgr->get_Abc_Frame_t()->pNtkCur;
    abccmd("strash");

    pNtk = abcMgr->get_Abc_Frame_t()->pNtkCur;
    Abc_Obj_t* miter_node = SelectChoiceNode(pNtk);

    // abccmd("show");
    // abccmd("show");
    
    Abc_Obj_t* pNode;
    Abc_Obj_t* pNodeReplace;

    // // calling abc's command
    // // abccmd("write_cnf temp.cnf");
    int obj_num = Abc_NtkObjNum(pNtk);
    pNode = Abc_NtkFindNode(pNtk, "n12");
    // pNode = Abc_NtkObj(pNtk, obj_num/2);
    
    Abc_Ntk_t *cone_k = Abc_NtkCreateCone(pNtk, pNode, "New_cone", 0);
    pNode = Abc_ObjFanin0(Abc_NtkPo(cone_k, 0));

    Abc_Ntk_t* pConeDup = Abc_NtkDup(cone_k);
    pNodeReplace = Abc_ObjFanin0(Abc_NtkPo(pConeDup,0));

    Abc_Aig_t* abc_aig = static_cast <Abc_Aig_t *> (pConeDup->pManFunc);
    Abc_Obj_t * pXor = Abc_AigXor(abc_aig, (Abc_ObjChild0(pNodeReplace)), (Abc_ObjChild1(pNodeReplace)));
    pXor = Abc_ObjRegular(pXor);
    Abc_AigReplace(abc_aig, pNodeReplace, pXor, 1);

    // Aig_Man_t *pAig = Abc_NtkToDar(cone_k, 0, 1);//aig manager
    // Cnf_Dat_t *pCnf = Cnf_Derive(pAig, 1);
    Simulation(cone_k, pConeDup);
    // Abc_FrameReplaceCurrentNetwork(abcMgr->get_Abc_Frame_t(), pConeDup);
    // abccmd("show");
    // cout << Abc_ObjIsComplement(Abc_ObjFanin0(pNode));
    // cout << Abc_ObjIsComplement(Abc_ObjFanin1(pNode));

    // test_function(pNtk, miter_node);

    return 0;
}




