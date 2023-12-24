#include "propagate.h"

void DfsWithLv(Abc_Obj_t* pNode, Vec_Ptr_t* vNodes, int level, Vec_Ptr_t* vFins) {
  if (level > 0 && !Abc_ObjIsPi(pNode)) {
    Abc_Obj_t* fin0 = Abc_ObjFanin0(pNode);
    Abc_Obj_t* fin1 = Abc_ObjFanin1(pNode);
    if (Vec_PtrFind(vNodes, fin0) == -1) DfsWithLv(fin0, vNodes, level-1, vFins);
    if (Vec_PtrFind(vNodes, fin1) == -1) DfsWithLv(fin1, vNodes, level-1, vFins);
  }
  else {
    Vec_PtrPush(vFins, pNode);
  }
  Vec_PtrPush(vNodes, pNode);
}

void Sim(Abc_Obj_t* pRoot, Vec_Ptr_t* vNodes, Vec_Ptr_t* vFins) {
  int ithFin = 0;
  for (int i = 0; i < vNodes->nSize; ++i) {
    Abc_Obj_t* n = (Abc_Obj_t*) vNodes->pArray[i];
    if (Vec_PtrFind(vFins, n) != -1) {
      if (ithFin == 0)      n->iTemp = 0x0000aaaa;
      else if (ithFin == 1) n->iTemp = 0x0000cccc;
      else if (ithFin == 2) n->iTemp = 0x0000f0f0;
      else if (ithFin == 3) n->iTemp = 0x0000ff00;
      else                  cout << "more than 4 fanin!";
      ++ithFin;
    }
    else {
      int fin0res = (Abc_ObjFaninC0(n))? ~Abc_ObjFanin0(n)->iTemp : Abc_ObjFanin0(n)->iTemp;
      int fin1res = (Abc_ObjFaninC1(n))? ~Abc_ObjFanin1(n)->iTemp : Abc_ObjFanin1(n)->iTemp;
      int res = fin0res & fin1res;
      n->iTemp = res;
    }
  }
}

bool* CreateTT(int input, int Fin_Num) {
  int TT_Size = pow(2, Fin_Num);
  bool* TT = new bool[TT_Size];
  for (int i = 0; i < TT_Size; ++i) {
    if (input & (0x01<<i)) TT[i] = true;
    else                   TT[i] = false;
  }
  return TT;
}

int* CalCost(bool* TT, int Fin_Num) {
  int* cost = new int[Fin_Num];
  int TTSize = pow(2, Fin_Num);
  for (int i = 0; i < Fin_Num; ++i) {
    cost[i] = 0;
    for (int j = 0; j < TTSize; ++j) {
      if ((j % (int)pow(2,i+1)) < (int)pow(2,i)) {
        int neg = j;
        int pos = j + (int)pow(2,i);
        if (TT[neg] ^ TT[pos]) {
          cost[i]++;
        }
      }
    }
  }
  return cost;
}

int Best(int* cost, int Fin_Num) {
  int best = 0;
  int min_cost = 16;
  for (int i = 0; i < Fin_Num; ++i) {
    if (cost[i] < min_cost) {
      min_cost = cost[i];
      best = i;
    }
  }
  return best;
}

Abc_Obj_t* ChooseBest(Vec_Ptr_t* vFins, bool* TT) {
  int Fin_Num = vFins->nSize;
  int* cost = CalCost(TT, Fin_Num);
  int best = Best(cost, Fin_Num);
  Abc_Obj_t* best_fin = (Abc_Obj_t*) vFins->pArray[best];
  delete cost;
  return best_fin;
}

Abc_Obj_t* ChoosePropagate(Abc_Obj_t* pRoot) {
  Vec_Ptr_t* vNodes = Vec_PtrAlloc(8);
  Vec_Ptr_t* vFins = Vec_PtrAlloc(4);
  DfsWithLv(pRoot, vNodes, 2, vFins);
  Sim(pRoot, vNodes, vFins);
  int Fin_Num = vFins->nSize;
  bool* TT = CreateTT(pRoot->iTemp, Fin_Num);
  Abc_Obj_t* best_fin = ChooseBest(vFins, TT);
  delete[] TT;
  return best_fin;
}