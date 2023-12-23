#include "simulate.h"

void PrintBinary(int in) {
  int a = 1;
  for (int i = 0; i < 32; ++i) {
    if ((in & (a << i)) == 0) cout << '0';
    else                      cout << '1';
    if (i%4 == 3 && i != 31) cout << '_';
  }
  cout << endl;
}

int* GenPattern(RandomNumGen myGen, int Pi_num) {
  int* ptn = new int[Pi_num];
  for (int i = 0; i < Pi_num; i++) {
    ptn[i] = myGen(INT_MAX);
  }
  return ptn;
}

int* SimPattern(Abc_Ntk_t* pNtk, int* ptn) {
  Vec_Ptr_t* vNodes = Abc_NtkDfsIter(pNtk, 0);
  Abc_Obj_t *pPi, *pNode, *pPo;
  int ithPi, ithNode, ithPo;
  Abc_NtkForEachPi(pNtk, pPi, ithPi) {
    // cout << Abc_ObjName(pPi) << ' ';
    pPi->iTemp = ptn[ithPi];
    // PrintBinary(pPi->iTemp);
  }
  Vec_PtrForEachEntry( Abc_Obj_t *, vNodes, pNode, ithNode) {
    int fin0res = (Abc_ObjFaninC0(pNode))? ~Abc_ObjFanin0(pNode)->iTemp : Abc_ObjFanin0(pNode)->iTemp;
    int fin1res = (Abc_ObjFaninC1(pNode))? ~Abc_ObjFanin1(pNode)->iTemp : Abc_ObjFanin1(pNode)->iTemp;
    int res = fin0res & fin1res;
    pNode->iTemp = res;
    // cout << Abc_ObjName(Abc_ObjFanin0(pNode)) << ' ';
    // PrintBinary(fin0res);
    // cout <<  Abc_ObjName(Abc_ObjFanin1(pNode)) << ' ';
    // PrintBinary(fin1res);
    // cout <<  Abc_ObjName(pNode) << ' ';
    // PrintBinary(res);
    // cout << endl;
  }
  int* out_res = new int[pNtk->vPos->nSize];
  Abc_NtkForEachPo(pNtk, pPo, ithPo) {
    Abc_Obj_t* pRoot = Abc_ObjFanin0(pPo);
    int ans = (pRoot->Id == 0)? -1 : pRoot->iTemp;
    if (!Abc_ObjFaninC0(pPo)) pPo->iTemp = ans;
    else                      pPo->iTemp = ~ans;
    // cout << Abc_ObjName(pPo) << ' ';
    out_res[ithPo] = pPo->iTemp;
    // PrintBinary(pPo->iTemp);
  }
  return out_res;
}

int CountOne(int in) {
  int one = 0;
  for (int i = 0; i < 32; ++i) {
    if (in & (0x01<<i)) one++;
  }
  return one;
}

void Simulation(Abc_Ntk_t* pOrgNtk, Abc_Ntk_t* pAftNtk, string err_type) {
  // check if two Network have same # input, output
  assert(pOrgNtk->vPis->nSize == pAftNtk->vPis->nSize);
  assert(pOrgNtk->vPos->nSize == pAftNtk->vPos->nSize);
  
  int Pi_Num = pOrgNtk->vPis->nSize;
  int Po_Num = pOrgNtk->vPos->nSize;
  int Sim_Num = 10000;
  int total_Ptn = 0;
  int total_Err = 0;
  unsigned seed = 20231222;
  RandomNumGen myGen(seed);
  
  for (int i = 0; i < Sim_Num; ++i) {
    int* ptn = GenPattern(myGen, Pi_Num);
    int* Org_res = SimPattern(pOrgNtk, ptn);
    int* Aft_res = SimPattern(pAftNtk, ptn);
    int Xor = 0;
    int err = 0;

    if (i < 3) {
      for (int j = 0; j < Pi_Num; ++j) {
        cout << j << ' ';
        PrintBinary(ptn[j]);
      }
    }

    if (err_type == "er" || err_type == "ER") {
      for (int j = 0; j < Po_Num; ++j) {
        Xor = Org_res[j] ^ Aft_res[j];
        err = err | Xor;
        if (i < 3) {
          // cout << "org[" << j << "]:";
          // PrintBinary(Org_res[j]);
          // cout << "aft[" << j << "]:";
          // PrintBinary(Aft_res[j]);
          cout << "xor[" << j << "]:";
          PrintBinary(Xor);
          cout << "err[" << j << "]:";
        PrintBinary(err);
      }
      }
      total_Ptn += 32;
      total_Err += CountOne(err);
    }
    else if (err_type == "nmed" || err_type == "NMED") {
      for (int j = 0; j < Po_Num; ++j) {
        Xor = Org_res[j] ^ Aft_res[j];
        total_Ptn += 32;
        total_Err += CountOne(Xor);
        if (i < 3) {
          cout << "org[" << j << "]:";
          PrintBinary(Org_res[j]);
          cout << "aft[" << j << "]:";
          PrintBinary(Aft_res[j]);
          cout << "xor[" << j << "]:";
          PrintBinary(Xor);
        }
      }
    }
    else {
      cout << "wrong error type!" << endl;
    }


    // cout << "iteration[" << i << "]" << "\r";
    // cout << "pattern count : " << total_Ptn << "\r";
    // cout << "error   count : " << total_Err << "\r";
    cout << "iter[" << i << "] : error rate : " << ((double)total_Err) / ((double)total_Ptn) << "   error   count : " << total_Err << "\r";
    delete ptn;
    delete Org_res;
    delete Aft_res;
  }
  cout << endl;
}