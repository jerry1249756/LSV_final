#include <iostream>
#include <cmath>
#include <iomanip>
#include <chrono>
#include "gvAbcMgr.h"
#include "base/abc/abc.h"
#include "gvAbcNtk.h"
#include "../src/util/rnGen.h"

void PrintBinary(int);
int* GenPattern(int);
int* SimPattern(Abc_Ntk_t*, Vec_Ptr_t* , int*);
int CountOne(int);

double Simulation(Abc_Ntk_t*, Abc_Ntk_t*, string, int, Vec_Ptr_t*);