#include "fec.h"

void Fec(Abc_Ntk_t* pNtk, vector<vector<Abc_Obj_t*>>& fecGrps) {
  Vec_Ptr_t* DfsList = Abc_NtkDfsIter(pNtk, 0);
  int Pi_Num = Abc_NtkPiNum(pNtk);
  int Sim_Num = 50000;
  cout << "total node" << Abc_NtkObjNum(pNtk) << endl;
  for (int i = 0; i < DfsList->nSize; ++i) {
    Abc_Obj_t* n = (Abc_Obj_t*) DfsList->pArray[i];
    n->dTemp = 0;
  }
  for (int i = 0; i < Sim_Num; ++i) {
    int* ptn = GenPattern(Pi_Num);
    SimPattern(pNtk, DfsList, ptn);
    for (int j = 0; j < fecGrps.size(); ++j) {
      vector<Abc_Obj_t*> new_fecGrp;
      for (int k = fecGrps[j].size()-1; k > 0; --k) {
        if (fecGrps[j][0]->iTemp != fecGrps[j][k]->iTemp) {
          new_fecGrp.push_back(fecGrps[j][k]);
          fecGrps[j].erase(fecGrps[j].begin()+k);
        }
      }
      if (new_fecGrp.size() > 1) {
        fecGrps.push_back(new_fecGrp);
      }
    }
    int max_size = 0;
    int total_node = 0;
    for (int j = fecGrps.size()-1; j >= 0; --j) {
      total_node += fecGrps[j].size();
      if (max_size < fecGrps[j].size()) max_size = fecGrps[j].size();
      if (fecGrps[j].size() <= 1) {
        fecGrps.erase(fecGrps.begin()+j);
      }
    }
    cout << "iter[" << i << "], fecGrps[" << fecGrps.size() << "], max[" << max_size << "], total[" << total_node << "]" << endl;
  }
  for (int i = 0; i < DfsList->nSize; ++i) {
    Abc_Obj_t* n = (Abc_Obj_t*) DfsList->pArray[i];
    n->dTemp /= Sim_Num*32;
  }
  for (int i = 0; i < fecGrps.size(); ++i) {
    bool finish = false;
    for (int j = 0; j < DfsList->nSize; ++j) {
      for (int k = 0; k < fecGrps[i].size(); ++k) {
          if (DfsList->pArray[j] == fecGrps[i][k]) {
            Abc_Obj_t* temp = fecGrps[i][0];
            fecGrps[i][0] = fecGrps[i][k];
            fecGrps[i][k] = temp;
            finish = true;
            break;
          }
      }
      if (finish) break;
    }
  }
}