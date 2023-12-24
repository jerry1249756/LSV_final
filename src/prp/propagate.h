#include <iostream>
#include <cmath>
#include <iomanip>
#include "gvAbcMgr.h"
#include "base/abc/abc.h"
#include "gvAbcNtk.h"

void DfsWithLv(Abc_Obj_t* ,Vec_Ptr_t* , int , Vec_Ptr_t*);
void Sim(Abc_Obj_t* , Vec_Ptr_t* , Vec_Ptr_t*);
bool* CreateTT(int, int);
int* CalCost(bool*, int);
int Best(int*, int);
Abc_Obj_t* ChooseBest(Vec_Ptr_t*, bool*);
Abc_Obj_t* ChoosePropagate(Abc_Obj_t*);
