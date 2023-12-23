#include "core.h"

INST get_action(Abc_Ntk_t* pNtk){
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    INST result = static_cast<INST>( std::rand() % 9 ); // get random actions
    // int node_Nums = Abc_NtkNodeNum(pNtk);
    // int node = std::rand() % node_Nums;

    return result;
}

void simulated_annealing(Abc_Ntk_t* pOrig, Abc_Ntk_t* pNew){
    // parameters
    float T = 1000;
    float T_low = 10;
    float r = 0.5;
    int iters = 10;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0,8); // actions

    while(T>T_low){
        for(int i=0; i<iters; i++){
            int action = dist(gen);
            Abc_Ntk_t* pNewDup = pNew->pCopy;
            switch(action){
                case 0: 
                    break;
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    break;
                case 4:
                    break;
                case 5:
                    break;
                case 6:
                    break;
                case 7:
                    break;
                case 8:
                    break;
            }

            Simulation(pOrig, pNewDup);
        }
        double area = Abc_NtkGetMappedArea(pOrig);

        T = r*T;    
    }

    

}