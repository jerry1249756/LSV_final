#include <iostream>
#include "gvAbcMgr.h"
#include "base/abc/abc.h"
#include "bdd/cudd/cudd.h"
#include "gvAbcNtk.h"
#include "base/main/main.h"
#include "map/mio/mio.h"

#include "sat/cnf/cnf.h" 
extern "C"{ 
  Aig_Man_t* Abc_NtkToDar( Abc_Ntk_t * pNtk, int fExors, int fRegisters );
}


// Abc_Obj_t * Ckt_UpdateNetwork(Abc_Obj_t * pObj, Vec_Ptr_t * vFanins, Aig_Obj_t * pFunc)
// {
//     Abc_Obj_t * pObjNew, * pFanin;
//     int k;
//     // create the new node
//     pObjNew = Abc_NtkCreateNode( pObj->pNtk );
//     // pObjNew->pData = pFunc;
//     Vec_PtrForEachEntry( Abc_Obj_t *, vFanins, pFanin, k )
//         Abc_ObjAddFanin( pObjNew, pFanin );
//     // replace the old node by the new node
//     Abc_ObjReplace( pObj, pObjNew );
//     return pObjNew;
// }

void * Ckt_UpdateNetwork(Aig_Man_t * pMan, Aig_Obj_t * pObj, Aig_Obj_t * pFunc)
{
    Aig_Obj_t * pObjNew, * pFanin;
    int k;
    // create the new node
    pObjNew = Aig_ManConst1(pMan);
    // pObjNew = Abc_NtkCreateNode( pObj->pNtk );
    // pObjNew->pData = pFunc;
        
    // replace the old node by the new node
    Aig_ObjReplace(pMan, pObj, pObjNew, 0);
    // return pObjNew;
}


char * Abc_NtkPrintSop( char * pSop ) 
{
    static char Buffer[1000];
    char * pGet, * pSet;
    pSet = Buffer;
    for ( pGet = pSop; *pGet; pGet++ )
    {        
        if ( *pGet == '\n' )
        {
            *pSet++ = '\\';
            *pSet++ = 'n';
        }
        else
            *pSet++ = *pGet;
    }
    *(pSet-2) = 0;
    return Buffer;
}

extern AbcMgr* abcMgr;

void abccmd(string);
void Lsv_NtkSymSat(Abc_Ntk_t* pNtk, char* output, char* input1, char* input2);

int main(int argc, char** argv) {
    abcMgr = new AbcMgr;
    string a = "testcases/test.blif";
    abcMgr->abcReadDesign(a);
    // abccmd("strash");

    int i;
    char * sop;
    Abc_Ntk_t* pNtk = abcMgr->get_Abc_Frame_t()->pNtkCur;
    Abc_Obj_t * pNode;
    Abc_NtkForEachObj(pNtk,pNode,i){
        cout << i << " ";
        cout << Abc_ObjName(pNode) << endl;
    }
    
    abccmd("strash");
    pNtk = abcMgr->get_Abc_Frame_t()->pNtkCur;
    // assert(Abc_NtkToDar(pNtk,0,0));
    // abccmd("show");
    // assert(Abc_NtkIsLogic(pNtk));
    // assert(Abc_NtkToAig(pNtk));
    // assert(Abc_NtkHasAig(pNtk));
    cout << "ori: " << Abc_NtkNodeNum(pNtk) << endl;
    // abccmd("aig");
    // Hop_Man_t * pHopMan = static_cast <Hop_Man_t *> (pNtk->pManFunc);
    // Hop_Obj_t * pFunc = Hop_ManConst0(pHopMan);
    // Hop_Obj_t * pHopNode;
    // pNtk = abcMgr->get_Abc_Frame_t()->pNtkCur;
    // // pFunc = Hop_IthVar(pHopMan, 0);
    // // cout << "Hop: " << Hop_ManObjNum(pHopMan) << endl;
    // cout << "Hop: " << pHopMan->nObjs[0] << endl;
    // cout << "Hop: " << pHopMan->nObjs[1] << endl;
    // cout << "Hop: " << pHopMan->nObjs[2] << endl;
    // cout << "Hop: " << pHopMan->nObjs[3] << endl;
    // cout << "Hop: " << pHopMan->nObjs[4] << endl;
    // cout << "Hop: " << pHopMan->nObjs[5] << endl;
    // cout << "Hop: " << pHopMan->nObjs[6] << endl;
    // int var = 2;
    // // Hop_ManForEachNode(pHopMan, pHopNode, i) {
    //     Hop_Obj_t * pAnd = Hop_ManConst1(pHopMan);
    //     pAnd = Hop_And( pHopMan, pAnd , Hop_IthVar(pHopMan, var) );
    //     pFunc = Hop_Or( pHopMan, pFunc, pAnd );
    //     cout << Abc_NtkObjNum(pNtk) << endl;
    //     pNode = Abc_NtkObj(pNtk, 7);
    //     cout << "pnode: " << Abc_ObjName(pNode) << endl;
    //     Vec_Ptr_t * vFanins = Vec_PtrAlloc(10);
    //     int nFanins = Abc_ObjFaninNum(pNode);
    //     for (int j = 0; j < nFanins; ++j) {
    //       Vec_PtrPush(vFanins, Abc_ObjFanin(pNode, j));
    //     }
    //     Abc_Obj_t * pObjNew = Ckt_UpdateNetwork(pNode, vFanins, pFunc);
    //     if (pFunc->Id)
    //       cout << pFunc->Id << endl;
    //     else {
    //       cout << "0\n";
    //   }
    // }
    int var = 0;
    Aig_Man_t* pAigMan = Abc_NtkToDar(pNtk,0,0);
    Aig_Obj_t* p_aig_node;
    Aig_Obj_t * pFunc = Aig_ManConst0(pAigMan);
    // Aig_ManForEachNode(pAigMan, p_aig_node, i) {
        Aig_Obj_t * pConst1 = Aig_ManConst1(pAigMan);
        // Aig_Obj_t * pAnd = Aig_ManConst1(pAigMan);
        // pAnd = Aig_And( pAigMan, pAnd , Aig_IthVar(pAigMan, var) );
        // pFunc = Aig_Or( pAigMan, pFunc, pAnd );
        cout << Abc_NtkObjNum(pNtk) << endl;
        Aig_ManForEachNode(pAigMan,p_aig_node, i) {
          if (i == 5) break;
        }
        cout << "is and: " << Aig_ObjIsAnd(p_aig_node);
        // pNode = Abc_NtkObj(pNtk, 6);
        // cout << "pnode: " << Abc_ObjName(pNode) << endl;
        Vec_Ptr_t * vFanins = Vec_PtrAlloc(10);
        int nFanins = Abc_ObjFaninNum(pNode);
        cout << "fanin " << nFanins << endl;
        // for (int j = 0; j < nFanins; ++j) {
        //   Vec_PtrPush(vFanins, Abc_ObjFanin(pNode, j));
        // }
        Aig_ObjReplace(pAigMan, p_aig_node, pConst1, 1);
        // Ckt_UpdateNetwork(pAigMan, p_aig_node, pAnd);
        if (pFunc->Id)
          cout << pFunc->Id << endl;
        else {
          cout << "0\n";
      }
    Aig_ManForEachObj(pAigMan, p_aig_node, i) {

      cout << "pData: " << p_aig_node->Type << endl;
    }
    // abccmd("fraig");
    
    // abccmd("show");
    // Abc_NtkForEachNode(pNtk, pNode, i) {
    //   cout << Abc_ObjName(pNode) << endl;
    //   sop = Abc_NtkPrintSop((char *)pNode->pData);
    //   if (strcmp(Abc_ObjName(pNode), "y0") == 0) {
    //     char* temp = (char*)pNode->pData;
    //     cout << "abc: " << temp << endl;
    //     sop[1] = '0';
    //     temp[1] = '0';
        // pNode->pData = sop;
      // }
      
      // if (i == 1)
      
      // cout << sop << endl;
      // printf("%s\n",sop);
    // }
    abccmd("fraig");
    // abccmd("strash");
    abccmd("show");

    return 0;
}

void abccmd(string command){
    char Command[1024], abcCmd[128];
    strcpy(abcCmd, command.c_str());
    sprintf(Command, "%s", abcCmd);
    Cmd_CommandExecute(abcMgr->get_Abc_Frame_t(), Command);
}

void Lsv_NtkSymSat(Abc_Ntk_t* pNtk, char* output, char* input1, char* input2) {
  int k = stoi(output);
  int i = stoi(input1);
  int j = stoi(input2);
  int m;
  printf("k:%d\n",k);
  Abc_Obj_t* Po = Abc_NtkPo(pNtk, k);
  Abc_Obj_t* Po_node = Abc_ObjFanin0(Po);
  Abc_Ntk_t* output_cone = Abc_NtkCreateCone(pNtk, Po_node, Abc_ObjName(Po), 1);
  Aig_Man_t* aig_1 = Abc_NtkToDar(output_cone, 0, 0);
  sat_solver* solver = sat_solver_new();
  Cnf_Dat_t* cnf_1 =  Cnf_Derive(aig_1, 1);
  Cnf_Dat_t* cnf_2 =  Cnf_Derive(aig_1, 1);
  Cnf_DataLift(cnf_2, cnf_2->nVars);
  // printf("vars2: %d\n", cnf_2->nVars);
  solver = (sat_solver*)Cnf_DataWriteIntoSolverInt(solver, cnf_1, 1, 0);
  solver = (sat_solver*)Cnf_DataWriteIntoSolverInt(solver, cnf_2, 1, 0);
  // solver = (sat_solver*)Cnf_DataWriteIntoSolverInt(solver, cnf_2, 1, 0);

  // Aig_Obj_t* Pi = Aig_ManCi(aig_1, i);
  // Aig_Obj_t* Pj = Aig_ManCi(aig_1, j);
  // Aig_Obj_t* Pk = Aig_ManCo(aig_1, k);
  // int var_1_out = cnf_2->pVarNums[Pk->Id];
  // int var_1_i = cnf_2->pVarNums[Pi->Id];
  // int var_1_j = cnf_2->pVarNums[Pj->Id];

  // Abc_Obj_t* Pi;
  // Abc_Obj_t* Pj;
  // Abc_Obj_t* Pk;
  Aig_Obj_t* Pi;
  Aig_Obj_t* Pj;
  Aig_Obj_t* Pk;
  Aig_ManForEachCi(aig_1, Pi, m) {
  // Abc_NtkForEachCi(pNtk, Pi, m) {
    if (m != i && m != j) {
      int array[] = {toLitCond(cnf_1->pVarNums[Pi->Id], 1), toLitCond(cnf_2->pVarNums[Pi->Id], 0)};
      int array1[] = {toLitCond(cnf_1->pVarNums[Pi->Id], 0), toLitCond(cnf_2->pVarNums[Pi->Id], 1)};
      sat_solver_addclause(solver, array, array + sizeof(array) / sizeof(int));
      sat_solver_addclause(solver, array1, array1 + sizeof(array1) / sizeof(int));
    }
  }
  Pi = Aig_ManCi(aig_1, i);
  Pj = Aig_ManCi(aig_1, j);
  Pk = Aig_ManCo(aig_1, 0);
  // Pi = Abc_NtkCi(pNtk, i);
  // Pj = Abc_NtkCi(pNtk, j);
  // Pk = Abc_NtkCo(pNtk, k);
  
  int array[] = {toLitCond(cnf_1->pVarNums[Pi->Id], 1), toLitCond(cnf_2->pVarNums[Pj->Id], 0)};
  int array1[] = {toLitCond(cnf_1->pVarNums[Pi->Id], 0), toLitCond(cnf_2->pVarNums[Pj->Id], 1)};
  sat_solver_addclause(solver, array, array + sizeof(array) / sizeof(int));
  sat_solver_addclause(solver, array1, array1 + sizeof(array1) / sizeof(int));
  int array2[] = {toLitCond(cnf_1->pVarNums[Pj->Id], 1), toLitCond(cnf_2->pVarNums[Pi->Id], 0)};
  int array3[] = {toLitCond(cnf_1->pVarNums[Pj->Id], 0), toLitCond(cnf_2->pVarNums[Pi->Id], 1)};
  sat_solver_addclause(solver, array2, array2 + sizeof(array2) / sizeof(int));
  sat_solver_addclause(solver, array3, array3 + sizeof(array3) / sizeof(int));
  int miter = sat_solver_addvar(solver);
  int array4[] = {toLitCond(cnf_1->pVarNums[Pk->Id], 1), toLitCond(cnf_2->pVarNums[Pk->Id], 0), toLitCond(miter, 0)};
  int array5[] = {toLitCond(cnf_1->pVarNums[Pk->Id], 0), toLitCond(cnf_2->pVarNums[Pk->Id], 1), toLitCond(miter, 0)};
  int array6[] = {toLitCond(cnf_1->pVarNums[Pk->Id], 0), toLitCond(cnf_2->pVarNums[Pk->Id], 0), toLitCond(miter, 1)};
  int array7[] = {toLitCond(cnf_1->pVarNums[Pk->Id], 1), toLitCond(cnf_2->pVarNums[Pk->Id], 1), toLitCond(miter, 1)};
  int array8[] = {toLit(miter)};
  sat_solver_addclause(solver, array4, array4 + sizeof(array4) / sizeof(int));
  sat_solver_addclause(solver, array5, array5 + sizeof(array5) / sizeof(int));
  sat_solver_addclause(solver, array6, array6 + sizeof(array6) / sizeof(int));
  sat_solver_addclause(solver, array7, array7 + sizeof(array7) / sizeof(int));
  sat_solver_addclause(solver, array8, array8 + sizeof(array8) / sizeof(int));
  // int var_2_i = cnf_2->pVarNums[Pi->Id];
  // int var_2_j = cnf_2->pVarNums[Pj->Id];
  int status = sat_solver_solve_internal(solver);
  if (status == 1) {
    printf("asymmetric\n");
    // int t = sat_solver_var_value(solver, cnf_1->pVarNums[Pk->Id]);
    // printf("id: %d ", cnf_1->pVarNums[Pk->Id]);
    // printf("value: %d\n", t);
    // int t1 = sat_solver_var_value(solver, cnf_2->pVarNums[Pk->Id]);
    // printf("id: %d ", cnf_2->pVarNums[Pk->Id]);
    // printf("value: %d\n", t1);
    Aig_ManForEachCi(aig_1, Pi, m) {
      int v = sat_solver_var_value(solver, cnf_1->pVarNums[Pi->Id]);
      printf("%d", v);
    // Abc_NtkForEachCi(pNtk, Pi, m) {
      // printf("cnf12_id: %d ",cnf_2->pVarNums[Pi->Id]);
      // printf("value: %d\n", v);

      // printf("cnf11_id: %d ",cnf_1->pVarNums[Pi->Id]);
      // int v1 = sat_solver_var_value(solver, cnf_1->pVarNums[Pi->Id]);
      // printf("value: %d\n", v1);
    }
    printf("\n");
    Aig_ManForEachCi(aig_1, Pi, m) {
      int v = sat_solver_var_value(solver, cnf_2->pVarNums[Pi->Id]);
      printf("%d", v);
    }
    printf("\n");
  }
  else if (status == -1) {
    printf("symmetric\n");
  }
}

