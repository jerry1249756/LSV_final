#include "change_node.h"

Abc_Obj_t * SelectChoiceNode(Abc_Ntk_t* pNtk) {
  Abc_Obj_t * pNode;
  Abc_Obj_t * pNode2;
  Abc_Obj_t * pNode_ori;
  Abc_Obj_t * pNode_change;
  int obj_num = Abc_NtkObjNum(pNtk);
  pNode = Abc_NtkFindNode(pNtk, "n12");
  cout << Abc_ObjIsNode(pNode) << endl;
  Abc_Aig_t* abc_aig = static_cast <Abc_Aig_t *> (pNtk->pManFunc);
  Abc_Obj_t * pXor = Abc_AigXor(abc_aig, (Abc_ObjChild0(pNode)), (Abc_ObjChild1(pNode)));
  pXor = Abc_ObjRegular(pXor);
  Abc_Obj_t * pMiter = Abc_AigXor(abc_aig, pXor, pNode);
  pMiter = Abc_ObjRegular(pMiter);
  return pMiter;
}

void UpdateNtk_toggle_input(Abc_Ntk_t* pNtk, Abc_Obj_t* pNode, int l_neg, int r_neg) {
  Abc_Aig_t* abc_aig = static_cast <Abc_Aig_t *> (pNtk->pManFunc);
  Abc_Obj_t * pAnd = Abc_AigAnd(abc_aig, Abc_ObjNotCond(Abc_ObjChild0(pNode), l_neg), Abc_ObjNotCond(Abc_ObjChild1(pNode), r_neg));
  pAnd = Abc_ObjRegular(pAnd);
  Abc_AigReplace(abc_aig, pNode, pAnd, 1);
  Abc_NtkReassignIds(pNtk);
}

void UpdateNtk_const1_propagate(Abc_Ntk_t* pNtk, Abc_Obj_t* pNode, int edge) { // edge = 0: left, edge = 1: right
  Abc_Aig_t* abc_aig = static_cast <Abc_Aig_t *> (pNtk->pManFunc);
  Abc_Obj_t* pAnd;
  if (edge == 0) {
    pAnd = Abc_AigAnd(abc_aig, Abc_AigConst1(pNtk), Abc_ObjChild1(pNode));
  }
  else {
    pAnd = Abc_AigAnd(abc_aig, Abc_ObjChild0(pNode), Abc_AigConst1(pNtk));
  }
  pAnd = Abc_ObjRegular(pAnd);
  Abc_AigReplace(abc_aig, pNode, pAnd, 1);
  Abc_NtkReassignIds(pNtk);
}

void UpdateNtk_const0_propagate(Abc_Ntk_t* pNtk, Abc_Obj_t* pNode, int edge) { // edge = 0: left, edge = 1: right
  Abc_Aig_t* abc_aig = static_cast <Abc_Aig_t *> (pNtk->pManFunc);
  Abc_Obj_t* pAnd;
  if (edge == 0) {
    pAnd = Abc_AigAnd(abc_aig, Abc_ObjNot(Abc_AigConst1(pNtk)), Abc_ObjChild1(pNode));
  }
  else {
    pAnd = Abc_AigAnd(abc_aig, Abc_ObjChild0(pNode), Abc_ObjNot(Abc_AigConst1(pNtk)));
  }
  pAnd = Abc_ObjRegular(pAnd);
  Abc_AigReplace(abc_aig, pNode, pAnd, 1);
  Abc_NtkReassignIds(pNtk);
}

void UpdateNtk_add_node(Abc_Ntk_t* pNtk, Abc_Obj_t* pNodeOut, Abc_Obj_t* pNodeIn1, Abc_Obj_t* pNodeIn2) {
  Abc_Aig_t* abc_aig = static_cast <Abc_Aig_t *> (pNtk->pManFunc);
  Abc_Obj_t* pAnd = Abc_AigAnd(abc_aig, pNodeIn1, pNodeIn2);
  pAnd = Abc_ObjRegular(pAnd);
  Abc_ObjPatchFanin(pNodeOut, pNodeIn1, pAnd);
  Abc_NtkReassignIds(pNtk);
}