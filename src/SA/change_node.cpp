#include "change_node.h"

Abc_Obj_t * SelectChoiceNode(Abc_Ntk_t* pNtk) {
  Abc_Obj_t * pNode;
  Abc_Obj_t * pNode2;
  Abc_Obj_t * pNode_ori;
  Abc_Obj_t * pNode_change;
  int obj_num = Abc_NtkObjNum(pNtk);
  pNode = Abc_NtkFindNode(pNtk, "n12");
  // cout << Abc_ObjIsNode(pNode) << endl;
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

void UpdateNtk_const1_propagate(Abc_Ntk_t* pNtk, int edge) {// edge = 0: left, edge = 1: right
  Abc_Obj_t * pNode;
  int i, Counter = 0;
  std::vector<Abc_Obj_t* > vCriticals;
  Abc_NtkStartReverseLevels( pNtk, 0 );
  Abc_NtkForEachNode( pNtk, pNode, i ) //find critical path
      if ( Abc_ObjRequiredLevel(pNode) - pNode->Level <= 1 )
          vCriticals.push_back(pNode);
  //random choose a node on the critical path
  std::random_device rd;
  std::mt19937 gen(rd());

  // std::uniform_int_distribution <> dist(0, vCriticals.size()-1);
  // pNode =  vCriticals[dist(gen)];
  
  std::uniform_int_distribution <> dist(0, Abc_NtkObjNum(pNtk)-1);
  while (!Abc_ObjIsNode(pNode = Abc_NtkObj(pNtk,dist(gen))));


  Abc_Aig_t* abc_aig = static_cast <Abc_Aig_t *> (pNtk->pManFunc);
  Abc_Obj_t* pAnd;
  if (edge == 0) {
    pAnd = Abc_AigAnd(abc_aig, Abc_AigConst1(pNtk), Abc_ObjChild1(pNode));
  }
  else {
    pAnd = Abc_AigAnd(abc_aig, Abc_ObjChild0(pNode), Abc_AigConst1(pNtk));
  }
  pAnd = Abc_ObjRegular(pAnd);
  Abc_AigReplace(abc_aig, pNode, pAnd, 0);
  Abc_NtkReassignIds(pNtk);
}

void UpdateNtk_const0_propagate(Abc_Ntk_t* pNtk, int edge) { // edge = 0: left, edge = 1: right
  Abc_Obj_t * pNode;
  int i, Counter = 0;
  std::vector<Abc_Obj_t* > vCriticals;
  Abc_NtkStartReverseLevels( pNtk, 0 );
  Abc_NtkForEachNode( pNtk, pNode, i ) //find critical path
      if ( Abc_ObjRequiredLevel(pNode) - pNode->Level <= 1 )
          vCriticals.push_back(pNode);
  //random choose a node on the critical path
  std::random_device rd;
  std::mt19937 gen(rd());

  // std::uniform_int_distribution <> dist(0, vCriticals.size()-1);
  // pNode =  vCriticals[dist(gen)];

  std::uniform_int_distribution <> dist(0, Abc_NtkObjNum(pNtk)-1);
  while (!Abc_ObjIsNode(pNode = Abc_NtkObj(pNtk,dist(gen))));

  
  Abc_Aig_t* abc_aig = static_cast <Abc_Aig_t *> (pNtk->pManFunc);
  Abc_Obj_t* pAnd;
  if (edge == 0) {
    pAnd = Abc_AigAnd(abc_aig, Abc_ObjNot(Abc_AigConst1(pNtk)), Abc_ObjChild1(pNode));
  }
  else {
    pAnd = Abc_AigAnd(abc_aig, Abc_ObjChild0(pNode), Abc_ObjNot(Abc_AigConst1(pNtk)));
  }
  pAnd = Abc_ObjRegular(pAnd);
  Abc_AigReplace(abc_aig, pNode, pAnd, 0);
  Abc_NtkReassignIds(pNtk);
}

void UpdateNtk_add_And_node(Abc_Ntk_t* pNtk, Abc_Obj_t* pNodeOut, Abc_Obj_t* pNodeIn1, Abc_Obj_t* pNodeIn2) {
  Abc_Aig_t* abc_aig = static_cast <Abc_Aig_t *> (pNtk->pManFunc);
  Abc_Obj_t* pAnd = Abc_AigAnd(abc_aig, pNodeIn1, pNodeIn2);
  pAnd = Abc_ObjRegular(pAnd);
  Abc_ObjPatchFanin(pNodeOut, pNodeIn1, pAnd);
  Abc_NtkReassignIds(pNtk);
}

void UpdateNtk_add_Xor_node(Abc_Ntk_t* pNtk, Abc_Obj_t* pNodeOut, Abc_Obj_t* pNodeIn1, Abc_Obj_t* pNodeIn2) {
  Abc_Aig_t* abc_aig = static_cast <Abc_Aig_t *> (pNtk->pManFunc);
  Abc_Obj_t* pAnd = Abc_AigXor(abc_aig, pNodeIn1, pNodeIn2);
  pAnd = Abc_ObjRegular(pAnd);
  Abc_ObjPatchFanin(pNodeOut, pNodeIn1, pAnd);
  Abc_NtkReassignIds(pNtk);
}

void UpdateNtk_add_Or_node(Abc_Ntk_t* pNtk, Abc_Obj_t* pNodeOut, Abc_Obj_t* pNodeIn1, Abc_Obj_t* pNodeIn2) {
  Abc_Aig_t* abc_aig = static_cast <Abc_Aig_t *> (pNtk->pManFunc);
  Abc_Obj_t* pAnd = Abc_AigOr(abc_aig, pNodeIn1, pNodeIn2);
  pAnd = Abc_ObjRegular(pAnd);
  Abc_ObjPatchFanin(pNodeOut, pNodeIn1, pAnd);
  Abc_NtkReassignIds(pNtk);
}