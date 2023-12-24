#include "propagate.h"

void DfsWithLv(Abc_Obj_t* pNode,Vec_Ptr_t* vNodes, int level) {
  if (level > 0 && !Abc_ObjIsPi(pNode)) {
    Abc_Obj_t* fin0 = Abc_ObjFanin0(pNode);
    Abc_Obj_t* fin1 = Abc_ObjFanin1(pNode);
    if (Vec_PtrFind(vNodes, fin0) == -1) DfsWithLv(fin0, vNodes, level-1);
    if (Vec_PtrFind(vNodes, fin1) == -1) DfsWithLv(fin1, vNodes, level-1);
  }
  Vec_PtrPush(vNodes, pNode);
}

void Sim(Abc_Obj_t* pRoot, Vec_Ptr_t* vNodes, Vec_Ptr_t* vFins) {
  int ithFin = 0;
  for (int i = 0; i < vNodes->nSize; ++i) {
    Abc_Obj_t* n = (Abc_Obj_t*) vNodes->pArray[i];
    if (Abc_ObjLevel(pRoot) - Abc_ObjLevel(n) == 2 || Abc_ObjIsPi(n)) {
      if (ithFin == 0)      n->iTemp = 0x0000aaaa;
      else if (ithFin == 1) n->iTemp = 0x0000cccc;
      else if (ithFin == 2) n->iTemp = 0x0000f0f0;
      else if (ithFin == 3) n->iTemp = 0x0000ff00;
      else                  cout << "more than 4 fanin!";
      ++ithFin;
      Vec_PtrPush(vFins, n);
    }
  }
  for (int i = 0; i < vNodes->nSize; ++i) {
    Abc_Obj_t* n = (Abc_Obj_t*) vNodes->pArray[i];
    if (Vec_PtrFind(vFins, n) == -1) {
      int fin0res = (Abc_ObjFaninC0(n))? ~Abc_ObjFanin0(n)->iTemp : Abc_ObjFanin0(n)->iTemp;
      int fin1res = (Abc_ObjFaninC1(n))? ~Abc_ObjFanin1(n)->iTemp : Abc_ObjFanin1(n)->iTemp;
      int res = fin0res & fin1res;
      n->iTemp = res;
    }
  }
}

bool* CreateTT(int input) {
  bool* TT = new bool[16];
  for (int i = 0; i < 16; ++i) {
    if (input & (0x01<<i)) TT[i] = true;
    else                   TT[i] = false;
  }
  return TT;
}

int* CalCost(bool* TT) {
  int* cost = new int[4];
  for (int i = 0; i < 4; ++i) {
    cost[i] = 0;
    for (int j = 0; j < 16; ++j) {
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

int Best(int* cost) {
  int best = 0;
  int min_cost = 16;
  for (int i = 0; i < 4; ++i) {
    if (cost[i] < min_cost) {
      min_cost = cost[i];
      best = i;
    }
  }
  return best;
}

Abc_Obj_t* ChooseBest(Vec_Ptr_t* vFins, bool* TT) {
  int* cost = CalCost(TT);
  int best = Best(cost);
  Abc_Obj_t* best_fin = (Abc_Obj_t*) vFins->pArray[best];
  delete cost;
  return best_fin;
}

Abc_Obj_t* ChoosePropagate(Abc_Obj_t* pRoot) {
  Vec_Ptr_t* vNodes = Vec_PtrAlloc(8);
  Vec_Ptr_t* vFins = Vec_PtrAlloc(4);
  DfsWithLv(pRoot, vNodes, 2);
  Sim(pRoot, vNodes, vFins);
  bool* TT = CreateTT(pRoot->iTemp);
  Abc_Obj_t* best_fin = ChooseBest(vFins, TT);
  delete[] TT;
  return best_fin;
}