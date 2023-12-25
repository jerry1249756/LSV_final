#include <iostream>
#include "gvAbcMgr.h"
#include "base/abc/abc.h"
#include "gvAbcNtk.h"

#include "./../SA/change_node.h"
#include "./../SA/core.h"
extern AbcMgr* abcMgr;
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
    abccmd("read ./testcases/mcnc.genlib");
    // string lib = "./testcases/mcnc.genlib";
    // string map = "-m ./testcases/c7552.blif";
    string map = "./testcases/voter.blif";

    // abcMgr->abcReadDesign(lib);
    abcMgr->abcReadDesign(map);
    abccmd("strash");
    // abccmd("fraig");
    Abc_Ntk_t* pNtk_cur = abcMgr->get_Abc_Frame_t()->pNtkCur;
    Abc_Ntk_t* pNtk_orig = Abc_NtkDup(pNtk_cur);
    // UpdateNtk_using_FEC(pNtk_cur);
    // Vec_Ptr_t* vNodes_org = Abc_NtkDfsIter(pNtk_orig, 0);
    // cout << "error: " << Simulation(pNtk_orig, pNtk_cur, "nmed", 50000, vNodes_org) << "\n";
    int node_nums_orig = Abc_NtkNodeNum(pNtk_orig);
    int node_nums_after = Abc_NtkNodeNum(pNtk_cur);
    // std::cout << "node_num (orig/after): " << node_nums_orig << " " << node_nums_after << "\n"; 
    
    simulated_annealing(pNtk_orig, pNtk_cur);
    // std::cout << "node_num (orig/after): " << node_nums_orig << " " << node_nums_after << "\n"; 
    
    return 0;
}




