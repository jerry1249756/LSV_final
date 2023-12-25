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

int* GenPattern(int Pi_num) {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  RandomNumGen myGen(seed);
  int* ptn = new int[Pi_num];
  for (int i = 0; i < Pi_num; i++) {
    ptn[i] = myGen(INT_MAX);
  }
  return ptn;
}

int* SimPattern(Abc_Ntk_t* pNtk, Vec_Ptr_t* vNodes, int* ptn) {
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

double Simulation(Abc_Ntk_t* pOrgNtk, Abc_Ntk_t* pAftNtk, string err_type, int Sim_Num, Vec_Ptr_t* vNodes_org) {
  // cout << "start simulation\nerror type = " << err_type << endl;
  Vec_Ptr_t* vNodes_aft = Abc_NtkDfsIter(pAftNtk, 0);
  // check if two Network have same # input, output
  assert(pOrgNtk->vPis->nSize == pAftNtk->vPis->nSize);
  assert(pOrgNtk->vPos->nSize == pAftNtk->vPos->nSize);
  
  int Pi_Num = pOrgNtk->vPis->nSize;
  int Po_Num = pOrgNtk->vPos->nSize;
  // int Sim_Num = 100000; // the total number of simulation will be Sim_Num * 32
  int total_Ptn = 0;
  int total_Err = 0;
  double Err_rate = 0;
  int Past_iter = 300; // compare the past "Past_iter" error rate
  double* Past_Err = new double[Past_iter];
  
  for (int i = 0; i < Sim_Num; ++i) {
    int* ptn = GenPattern(Pi_Num);
    int* Org_res = SimPattern(pOrgNtk, vNodes_org ,ptn);
    int* Aft_res = SimPattern(pAftNtk, vNodes_aft ,ptn);
    int Xor = 0;
    int err = 0;
    bool early_stop = true;
    if (err_type == "er" || err_type == "ER") {
      for (int j = 0; j < Po_Num; ++j) {
        Xor = Org_res[j] ^ Aft_res[j];
        err = err | Xor;
      }
      total_Ptn += 32;
      total_Err += CountOne(err);
    }
    else if (err_type == "nmed" || err_type == "NMED") {
      for (int j = 0; j < Po_Num; ++j) {
        Xor = Org_res[j] ^ Aft_res[j];
        total_Ptn += 32;
        total_Err += CountOne(Xor);
      }
    }
    else {
      cout << "wrong error type!" << endl;
    }
    Err_rate = (double)total_Err / (double)total_Ptn;
    if (Err_rate > 0.051 && i >= 1000) {
      Err_rate = 1;
      break;
    }
    // cout << "[" << setw(5) << i << "] error rate: " << setw(8) << Err_rate*100 << " (%)\r";
    for (int j = 0; j < Past_iter; ++j) {
      if (abs(Err_rate-Past_Err[j]) > 0.00001) {
        early_stop = false;
        break;
      }
    }
    if (early_stop && i > 100) {
      // cout << "\nearly stop!";
      break;
    }
    else {
      for (int j = 0; j < Past_iter-1; ++j) {
        Past_Err[j] = Past_Err[j+1];
      }
      Past_Err[Past_iter-1] = Err_rate;
    }
    delete[] ptn;
    delete[] Org_res;
    delete[] Aft_res;
  }
  // cout  << endl;
  delete[] Past_Err;
  return Err_rate;
}