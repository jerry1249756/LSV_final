#ifndef FEC_H
#define FEC_H

#include <iostream>
#include <cmath>
#include <iomanip>
#include "gvAbcMgr.h"
#include "base/abc/abc.h"
#include "gvAbcNtk.h"
#include "./../sim/simulate.h"

void Fec(Abc_Ntk_t* pNtk, vector<vector<Abc_Obj_t*>>& fecGrps);
#endif