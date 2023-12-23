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