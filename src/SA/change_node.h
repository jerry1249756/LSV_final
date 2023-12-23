#include <iostream>
#include "gvAbcMgr.h"
#include "base/abc/abc.h"
#include "gvAbcNtk.h"

extern "C"{
  Abc_Obj_t * Abc_AigAndCreate( Abc_Aig_t * pMan, Abc_Obj_t * p0, Abc_Obj_t * p1 );
}

Abc_Obj_t * SelectChoiceNode(Abc_Ntk_t*);
void UpdateNtk_toggle_input(Abc_Ntk_t*, Abc_Obj_t*, int, int);
void UpdateNtk_const1_propagate(Abc_Ntk_t*, Abc_Obj_t*, int);
void UpdateNtk_const0_propagate(Abc_Ntk_t*, Abc_Obj_t*, int);