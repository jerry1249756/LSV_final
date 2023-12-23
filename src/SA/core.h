#include <cmath>
#include <ctime>
#include <random>
#include <utility>
#include "base/abc/abc.h"
#include "./change_node.h"
#include "../sim/simulate.h"

enum class INST{
    CONST1_L   = 0,
    CONST1_R   = 1,
    CONST0_L   = 2,
    CONST0_R   = 3,
    LEFT_NEG   = 4,
    RIGHT_NEG  = 5,
    DOUBLE_NEG = 6,
    RESUB      = 7,
    REFACTOR   = 8,
    REWRITE    = 9,
    BALANCE    = 10,
};
static const std::string inst_strings[] = {"const1_l", "const1_r",
 "const0_l", "const0_r", "left_neg", "right_neg", "double_neg", "resub", "refactor", "rewrite", "balance" };

INST get_action();
void simulated_annealing(Abc_Ntk_t*, Abc_Ntk_t* );
