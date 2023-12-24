#include "fec.h"

void Fraig(Abc_Ntk_t* pNtk, vector<vector<Abc_Obj_t*>> fecGrps) {
  Vec_Ptr_t* DfsList = Abc_NtkDfsIter(pNtk, 0);
  int Pi_Num = pNtk->vFins->nSize;
  int Sim_Num = 100;
  cout << "total node" << Abc_NtkObjNum(pNtk) << endl;
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
    for (int j = fecGrps.size()-1; j >= 0; --j) {
      if (max_size < fecGrps[j].size()) max_size = fecGrps[j].size();
      if (fecGrps[j].size() <= 1) {
        fecGrps.erase(fecGrps.begin()+j);
      }
    }
    cout << "iter[" << i << "], " << "fecGrps.size[" << fecGrps.size() << "], " << "max.size[" << max_size << "]" << endl;
  }
}