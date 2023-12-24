#include <iostream>
#include <cmath>
#include <iomanip>
#include "gvAbcMgr.h"
#include "base/abc/abc.h"
#include "gvAbcNtk.h"

Vec_Ptr_t* DFSWithLevel(Abc_Obj_t*, int);
Abc_Obj_t* ChoosePropagate(Abc_Obj_t*);