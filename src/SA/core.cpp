#include "core.h"

INST get_action(){
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int result = std::rand() % 7; // get random actions
    // int node_Nums = Abc_NtkNodeNum(pNtk);
    // int node = std::rand() % node_Nums;
    cout << result << "\n";
    return  static_cast<INST>(result);
}

double error_func(double error_rate) {
    return log(1 + exp(50*(error_rate - 0.09))) - 0.01;
}

double cost_diff(double rate_orig, double rate_after, int node_nums_orig, int node_nums_after) {
    double error = error_func(rate_after) - error_func(rate_orig);
    double node_reduce_per = (node_nums_after - node_nums_orig) / node_nums_orig;
    return error + node_reduce_per;
}

void simulated_annealing(Abc_Ntk_t* pOrig, Abc_Ntk_t* pNew){
    // parameters
    float T = 1000;
    float T_low = 10;
    float r = 0.5;
    int iters = 20;

    while(T>T_low){
        for(int i=0; i<iters; i++){
            double error_orig = 0;
            INST action = get_action();
            std::cout << "New: " << pNew << " frame Backup: " << abcMgr->get_Abc_Frame_t()->pNtkBackup << endl;
            int node_nums_orig = Abc_NtkNodeNum(pNew);
            if(abcMgr->get_Abc_Frame_t()->pNtkBackup){
                Abc_NtkDelete( abcMgr->get_Abc_Frame_t()->pNtkBackup);
            }
            abcMgr->get_Abc_Frame_t()->pNtkBackup =  Abc_NtkDup(pNew);
            
           
            // Abc_Ntk_t* pNewDup = Abc_NtkDup(pNew);
            std::cout << "New: " << pNew << " frame Backup: " << abcMgr->get_Abc_Frame_t()->pNtkBackup << endl;
            // Abc_FrameReplaceCurrentNetwork(abcMgr->get_Abc_Frame_t(), pNewDup);
            // std::cout << "New: " << pNew << " New Dup: " << pNewDup << " New Backup: " << pNewBackUp << endl;
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<int> dist(0,Abc_NtkObjNum(pNew));
            Abc_Obj_t* pNode;
            while (!Abc_ObjIsNode(pNode = Abc_NtkObj(pNew,dist(gen))));

            switch(action){
                case INST::RESUB:
                    abccmd("resub");
                break;
                case INST::REFACTOR:
                    abccmd("refactor");
                break;
                case INST::REWRITE:
                    abccmd("rewrite");
                break;
                case INST::BALANCE:
                    abccmd("balance");
                break;
                case INST::CONST1_L:
                    UpdateNtk_const1_propagate(pNew, pNode, 0);
                break;
                case INST::CONST1_R:
                    UpdateNtk_const1_propagate(pNew, pNode, 1);
                break;
                case INST::CONST0_L:
                    UpdateNtk_const0_propagate(pNew, pNode, 0);
                break;
                case INST::CONST0_R:
                    UpdateNtk_const0_propagate(pNew, pNode, 1);
                break;
                case INST::LEFT_NEG:
                    UpdateNtk_toggle_input(pNew, pNode, 1, 0);
                break;
                case INST::RIGHT_NEG:
                    UpdateNtk_toggle_input(pNew, pNode, 0, 1);
                break;
                case INST::DOUBLE_NEG:
                    UpdateNtk_toggle_input(pNew, pNode, 1, 1);
                break;
            }
            std::cout << "New: " << pNew << " frame Backup: " << abcMgr->get_Abc_Frame_t()->pNtkBackup << endl;
            double error_after;
            if (action == INST::RESUB || action == INST::REFACTOR || action == INST::REWRITE || action == INST::BALANCE){
                error_after = error_orig;
            }
            else {
                error_after = Simulation(pOrig, pNew, "nmed");
            }
            int node_nums_after = Abc_NtkNodeNum(pNew);
            std::cout << "node_num (orig/after): " << node_nums_orig << " " << node_nums_after << "\n"; 
            double diff = cost_diff(error_orig, error_after, node_nums_orig, node_nums_after);
            mt19937 gen2(rd());
            uniform_real_distribution<double> dist2(0,1);
            if (diff < 0 || dist2(gen2) < exp(-diff / T)) {
                error_orig = error_after;
                node_nums_orig = node_nums_after;
            }
            else {
                pNew = abcMgr->get_Abc_Frame_t()->pNtkBackup;
                abcMgr->get_Abc_Frame_t()->pNtkBackup = nullptr;
            }
            // Abc_NtkDelete(pNewDup);
            // Abc_NtkDelete(pNewBackUp);

        }
        // double area = Abc_NtkGetMappedArea(pOrig);

        T = r*T;    
    }

    

}