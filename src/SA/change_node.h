#ifndef CHANGE_NODE_H
#define CHANGE_NODE_H

#include <iostream>
#include <vector>
#include <random>
#include "gvAbcMgr.h"
#include "base/abc/abc.h"
#include "gvAbcNtk.h"
#include "../prp/propagate.h"

extern "C"{
  Abc_Obj_t * Abc_AigAndCreate( Abc_Aig_t * pMan, Abc_Obj_t * p0, Abc_Obj_t * p1 );
}

Abc_Obj_t * SelectChoiceNode(Abc_Ntk_t*);
void UpdateNtk_toggle_input(Abc_Ntk_t*, Abc_Obj_t*, int, int);
void UpdateNtk_const1_propagate(Abc_Ntk_t*);
void UpdateNtk_const0_propagate(Abc_Ntk_t*);
void UpdateNtk_add_And_node(Abc_Ntk_t*, Abc_Obj_t*, Abc_Obj_t*, Abc_Obj_t*);
void UpdateNtk_add_Xor_node(Abc_Ntk_t*, Abc_Obj_t*, Abc_Obj_t*, Abc_Obj_t*);
void UpdateNtk_add_Or_node(Abc_Ntk_t*, Abc_Obj_t*, Abc_Obj_t*, Abc_Obj_t*);
#endif