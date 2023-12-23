#include "core.h"

INST get_action(Abc_Ntk_t* pNtk){
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    INST result = static_cast<INST>( std::rand() % 9 ); // get random actions
    // int node_Nums = Abc_NtkNodeNum(pNtk);
    // int node = std::rand() % node_Nums;

    return result;
}