#include <iostream>
#include "gvAbcMgr.h"
#include "base/abc/abc.h"
#include "gvAbcNtk.h"

extern "C"{
  Abc_Obj_t * Abc_AigAndCreate( Abc_Aig_t * pMan, Abc_Obj_t * p0, Abc_Obj_t * p1 );
}

Abc_Obj_t * SelectChoiceNode(Abc_Ntk_t*);