#include <iostream>
#include <cmath>
#include <iomanip>
#include "gvAbcMgr.h"
#include "base/abc/abc.h"
#include "gvAbcNtk.h"

Vec_Ptr_t* DFSWithLevel(Abc_Obj_t*, int);
void Sim(Abc_Obj_t* , Vec_Ptr_t* , Vec_Ptr_t* );
bool* CreateTT(int );
int* CalCost(bool* );
int Best(int* );
Abc_Obj_t* ChooseBest(Vec_Ptr_t*, bool*);
Abc_Obj_t* ChoosePropagate(Abc_Obj_t*);
