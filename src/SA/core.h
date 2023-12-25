#ifndef CORE_H
#define CORE_H

#include <cmath>
#include <ctime>
#include <random>
#include <utility>
#include "base/abc/abc.h"
#include "./change_node.h"
#include "../sim/simulate.h"

enum class INST{
    CONST1      = 0,
    CONST0      = 1,
    LEFT_NEG    = 2,
    RIGHT_NEG   = 3,
    DOUBLE_NEG  = 4,
    ORCHESTRATE = 5,
    CSWEEP      = 6,
    DFRAIG      = 7,
    BALANCE     = 8,
    DC2         = 9,
    ADDANDNODE  = 10,
    ADDXORNODE  = 11,
    ADDORNODE   = 12,
};
static const std::string inst_strings[] = {"const1", "const0", "left_neg", "right_neg", "double_neg",
 "orchestrate", "csweep", "dfraig", "balance", "dc2", "add_and_node", "add_Xor_node", "add_or_node" };

INST get_action();
void simulated_annealing(Abc_Ntk_t*, Abc_Ntk_t* );
#endif