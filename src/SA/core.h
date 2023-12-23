#include <cmath>
#include <ctime>
#include <random>
#include <utility>
#include "base/abc/abc.h"
#include "./change_node.h"
#include "../sim/simulate.h"

enum class INST{
    RESUB      = 7,
    REFACTOR   = 8,
    REWRITE    = 9,
    BALANCE    = 10,
    CONST1_L   = 0,
    CONST1_R   = 1,
    CONST0_L   = 2,
    CONST0_R   = 3,
    LEFT_NEG   = 4,
    RIGHT_NEG  = 5,
    DOUBLE_NEG = 6,
};
// static const std::string inst_strings[] = {"resub", "refactor", "rewrite", "balance" };

INST get_action();
void simulated_annealing(Abc_Ntk_t*, Abc_Ntk_t* );
