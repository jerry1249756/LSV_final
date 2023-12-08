#include <iostream>
#include "gvAbcMgr.h"
#include "base/abc/abc.h"
#include "bdd/cudd/cudd.h"
#include "gvAbcNtk.h"

#include "sat/cnf/cnf.h" 
extern "C"{ 
  Aig_Man_t* Abc_NtkToDar( Abc_Ntk_t * pNtk, int fExors, int fRegisters );
}

extern AbcMgr* abcMgr;

void abccmd(string);
void Lsv_NtkSymSat(Abc_Ntk_t* pNtk, char* output, char* input1, char* input2);

int main(int argc, char** argv) {
    abcMgr = new AbcMgr;
    string a = "adder.blif";
    abcMgr->abcReadDesign(a);
    abccmd("strash");
    Abc_Ntk_t* pNtk = abcMgr->get_Abc_Frame_t()->pNtkCur;
    int k = Abc_NtkObjNum(pNtk);
    printf("k: %d\n", k);
    cout << Abc_NtkIsStrash(pNtk) << endl;
    // abcMgr->get_abcNtkMgr()->printSummary();
    // Abc_Obj_t* Po = Abc_NtkPo(pNtk, 0);
    // cout << Po->pNtk << endl;
    Lsv_NtkSymSat(pNtk, argv[1], argv[2], argv[3]);    

    // calling abc's command
    abccmd("write_cnf temp.cnf");

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

