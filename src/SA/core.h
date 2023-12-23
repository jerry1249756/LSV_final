#include <cmath>
#include <ctime>
#include <random>
#include <utility>
#include "base/abc/abc.h"
#include "../sim/simulate.h"

enum class INST{
    RESUB      = 0,
    REFACTOR   = 1,
    REWRITE    = 2,
    BALANCE    = 3,
    CONST1     = 4,
    CONST0     = 5,
    LEFT_NEG   = 6,
    RIGHT_NEG  = 7,
    DOUBLE_NEG = 8,
};
// static const std::string inst_strings[] = {"resub", "refactor", "rewrite", "balance" };

INST get_action(Abc_Ntk_t* );
void simulated_annealing(Abc_Ntk_t*, Abc_Ntk_t* );
