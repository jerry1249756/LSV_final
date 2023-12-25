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

void UpdateNtk_const1_propagate(Abc_Ntk_t* pNtk) {
  // Abc_Obj_t * pNode;
  // int i, Counter = 0;
  // std::vector<Abc_Obj_t* > vCriticals;
  // Abc_NtkStartReverseLevels( pNtk, 0 );
  // Abc_NtkForEachNode( pNtk, pNode, i ) //find critical path
  //     if ( Abc_ObjRequiredLevel(pNode) - pNode->Level <= 1 )
  //         vCriticals.push_back(pNode);
  // //random choose a node on the critical path
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution <int> dist(0, Abc_NtkObjNum(pNtk)-1);
  Abc_Obj_t * pNode = Abc_NtkObj(pNtk,dist(gen));
  // cout << "isPi: " << Abc_ObjIsPi(pNode) << endl;
  // cout << "faninnum: " << Abc_ObjFaninNum(pNode) << endl;

  if (Abc_ObjIsPi(pNode) || Abc_ObjFaninNum(pNode) != 2) return;
  // while (Abc_ObjIsPi(pNode)) {
  //   int temp = dist(gen);
  //   pNode = Abc_NtkObj(pNtk,temp);
  //   cout << temp;
  // }
  // while (Abc_ObjIsPi(Abc_ObjFanin0(pNode)) || Abc_ObjIsPi(Abc_ObjFanin1(pNode))) {
  //   while (Abc_ObjIsPi(pNode = Abc_NtkObj(pNtk,dist(gen)))) {
  //     // int temp = dist(gen);
  //     // pNode = Abc_NtkObj(pNtk,temp);
  //     // cout << temp;
  //   }
  // }
  Abc_Obj_t* pChange = ChoosePropagate(pNode);
  cout << "type: " << Abc_ObjType(pChange) << endl;
  Abc_Obj_t* pReplace;
  Abc_Obj_t* pReserve;
  Abc_Obj_t* fanin0 = Abc_ObjFanin0(pNode);
  Abc_Obj_t* fanin1 = Abc_ObjFanin1(pNode);

  if (fanin0 == pChange) {
    pReplace = pNode;
    pReserve = fanin1;
  }
  else if (!Abc_ObjIsPi(fanin0)) {
    if (Abc_ObjFanin0(fanin0) == pChange) {
      pReplace = fanin0;
      pReserve = Abc_ObjFanin1(fanin0);
    }
  }
  if (fanin1 == pChange) {
    pReplace = pNode;
    pReserve = fanin0;
  }
  else  if (!Abc_ObjIsPi(fanin1)){
    if (Abc_ObjFanin0(fanin1) == pChange) {
      pReplace = fanin1;
      pReserve = Abc_ObjFanin1(fanin1);
    }
  }
  
  Abc_Aig_t* abc_aig = static_cast <Abc_Aig_t *> (pNtk->pManFunc);
  Abc_Obj_t* pAnd;
  
  pAnd = Abc_AigAnd(abc_aig, Abc_AigConst1(pNtk), pReserve);
  // pAnd = Abc_AigAnd(abc_aig, Abc_AigConst1(pNtk), Abc_ObjFanin0(pNode));
  pAnd = Abc_ObjRegular(pAnd);
  // Abc_AigReplace(abc_aig, pNode, pAnd, 0);
  Abc_AigReplace(abc_aig, pReplace, pAnd, 0);
  Abc_NtkReassignIds(pNtk);
}

void UpdateNtk_const0_propagate(Abc_Ntk_t* pNtk) {
  // int i, Counter = 0;
  // std::vector<Abc_Obj_t* > vCriticals;
  // Abc_NtkStartReverseLevels( pNtk, 0 );
  // Abc_NtkForEachNode( pNtk, pNode, i ) //find critical path
  //     if ( Abc_ObjRequiredLevel(pNode) - pNode->Level <= 1 )
  //         vCriticals.push_back(pNode);
  //random choose a node on the critical path
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution <int> dist(0, Abc_NtkObjNum(pNtk)-1);
  Abc_Obj_t * pNode = Abc_NtkObj(pNtk,dist(gen));
  // cout << "isPi: " << Abc_ObjIsPi(pNode) << endl;
  // cout << "faninnum: " << Abc_ObjFaninNum(pNode) << endl;
  if (Abc_ObjIsPi(pNode) || Abc_ObjFaninNum(pNode) != 2) return;
  //   pNode = Abc_NtkObj(pNtk,dist(gen));
  // while (Abc_ObjIsPi(Abc_ObjFanin0(pNode)) || Abc_ObjIsPi(Abc_ObjFanin1(pNode))) {
  //   while (Abc_ObjIsPi(pNode = Abc_NtkObj(pNtk,dist(gen)))){}
  //     // pNode = Abc_NtkObj(pNtk,dist(gen));
  // }
  Abc_Obj_t* pChange = ChoosePropagate(pNode);
  cout << "type: " << Abc_ObjType(pChange) << endl;
  Abc_Obj_t* pReplace;
  Abc_Obj_t* pReserve;
  Abc_Obj_t* fanin0 = Abc_ObjFanin0(pNode);
  Abc_Obj_t* fanin1 = Abc_ObjFanin1(pNode);

  if (fanin0 == pChange) {
    pReplace = pNode;
    pReserve = fanin1;
  }
  else  if (!Abc_ObjIsPi(fanin0)){
    if (Abc_ObjFanin0(fanin0) == pChange) {
      pReplace = fanin0;
      pReserve = Abc_ObjFanin1(fanin0);
    }
  }
  if (fanin1 == pChange) {
    pReplace = pNode;
    pReserve = fanin0;
  }
  else  if (!Abc_ObjIsPi(fanin1)){
    if (Abc_ObjFanin0(fanin1) == pChange) {
      pReplace = fanin1;
      pReserve = Abc_ObjFanin1(fanin1);
    }
  }
  
  Abc_Aig_t* abc_aig = static_cast <Abc_Aig_t *> (pNtk->pManFunc);
  Abc_Obj_t* pAnd;
  pAnd = Abc_AigAnd(abc_aig, Abc_ObjNot(Abc_AigConst1(pNtk)), pReserve);
  // pAnd = Abc_AigAnd(abc_aig, Abc_ObjNot(Abc_AigConst1(pNtk)), Abc_ObjFanin0(pNode));
  
  pAnd = Abc_ObjRegular(pAnd);
  // Abc_AigReplace(abc_aig, pNode, pAnd, 0);
  Abc_AigReplace(abc_aig, pReplace, pAnd, 0);
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

void UpdateNtk_using_FEC(Abc_Ntk_t* pNtk) {
  Abc_Aig_t* abc_aig = static_cast <Abc_Aig_t *> (pNtk->pManFunc);
  vector<vector<Abc_Obj_t*>> fec_grps;
  vector<Abc_Obj_t*> fec_grp;
  vector<float> const_grp(Abc_NtkObjNum(pNtk), 0);
  Abc_Obj_t* pNode;
  int i;
  Vec_Ptr_t* DfsList = Abc_NtkDfsIter(pNtk, 0);
  Abc_NtkForEachObj(pNtk, pNode, i) {
    assert(Abc_ObjType(pNode) != 0);
    if (!Abc_ObjIsPo(pNode)) {
      fec_grp.push_back(pNode);
    }
  }
  fec_grps.push_back(fec_grp);
  Fec(pNtk, fec_grps, const_grp, DfsList);
  for (i = 0; i < DfsList->nSize; i++) {
    Abc_Obj_t* n = (Abc_Obj_t*)DfsList->pArray[i];
    cout << const_grp[n->Id] << endl;
  }
  int times = 0;
  for (i = 0; i < fec_grps.size(); i++) {
    for (int j = 1; j < fec_grps[i].size(); j++) {
      if(Abc_ObjType(fec_grps[i][0]) != 0){
        Abc_AigReplace(abc_aig, fec_grps[i][j], fec_grps[i][0],0);
        // int node_nums_after = Abc_NtkNodeNum(pNtk);
          // cout << "after_node: " << node_nums_after << endl;
        // cout << times++ << "\r";
      }
    }
  }
  cout << "\n";
  Abc_NtkReassignIds(pNtk);
  
}