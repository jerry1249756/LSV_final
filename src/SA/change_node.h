#include <iostream>
#include "gvAbcMgr.h"
#include "base/abc/abc.h"
#include "gvAbcNtk.h"

#include "./../est/exec.h"
#include "sat/cnf/cnf.h"

Abc_Obj_t * SelectChoiceNode(Abc_Ntk_t*);