// #include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <vector>
#include <sstream>
#include <cmath>
#include "base/abc/abc.h"
#include "sat/cnf/cnf.h" 
#include "gvAbcMgr.h"
#include "gvAbcNtk.h"

extern "C"{ 
  Aig_Man_t* Abc_NtkToDar( Abc_Ntk_t * pNtk, int fExors, int fRegisters );
}


std::string exec(const char*);
std::string modelCount(std::string, std::string);
void test_function(Abc_Ntk_t*, char*);