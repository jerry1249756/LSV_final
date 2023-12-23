#include <iostream>
#include <cmath>
#include <iomanip>
#include <chrono>
#include "gvAbcMgr.h"
#include "base/abc/abc.h"
#include "gvAbcNtk.h"
#include "../src/util/rnGen.h"

void PrintBinary(int);
void GenPattern(int* &, int);
void SimPattern(int* &, Abc_Ntk_t*, Vec_Ptr_t* , int*, int);

double Simulation(Abc_Ntk_t*, Abc_Ntk_t*, string);