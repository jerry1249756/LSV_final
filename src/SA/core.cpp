#include "core.h"


INST get_action(){
    int result = std::rand() % 12; // get random actions
    // int node_Nums = Abc_NtkNodeNum(pNtk);
    // int node = std::rand() % node_Nums;
    // cout << result << "\n";
    std::cout << inst_strings[result] << "\n";
    return  static_cast<INST>(result);
}

double error_func(double error_rate) {
    // return log(1 + exp(100*(error_rate - 0.06))) - 0.001;
    return 2*error_rate;
}

double cost_diff(double rate_orig, double rate_after, int node_nums_orig, int node_nums_after) {
    double error = error_func(rate_after) - error_func(rate_orig);
    double node_reduce_per = (node_nums_after - node_nums_orig) / node_nums_orig;
    return error + node_reduce_per;
}

void simulated_annealing(Abc_Ntk_t* pOrig, Abc_Ntk_t* pNew){
    // parameters
    float T = 300;
    float T_low = 1;
    float r = 0.7;
    int iters = 2000;

    double error_orig = 0;
    while(T>T_low){
        for(int i=0; i<iters; i++){
            // INST action;
            // if (i % 2 == 0) {
            //     action = INST::RESUB;
            //     cout << "resub" << endl;
            // }
            // else {
            //     action = INST::CONST1_L;
            //     cout << "myoper" << endl;
                
            // }

            INST action = get_action();
            int node_nums_orig = Abc_NtkNodeNum(pNew);
            if(abcMgr->get_Abc_Frame_t()->pNtkBackup){
                Abc_NtkDelete( abcMgr->get_Abc_Frame_t()->pNtkBackup);
            }
            Abc_Ntk_t* temp_back = Abc_NtkDup(pNew);
             
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<int> dist(0,Abc_NtkObjNum(pNew));
            Abc_Obj_t* pNode;
            while (!Abc_ObjIsNode(pNode = Abc_NtkObj(pNew,dist(gen))));
            cout << Abc_ObjName(pNode) << endl;
            switch(action){
                case INST::RESUB:
                    abccmd("orchestrate");
                break;
                case INST::REFACTOR:
                    abccmd("orchestrate -z");
                break;
                case INST::REWRITE:
                    abccmd("orchestrate -l");
                break;
                case INST::BALANCE:
                    abccmd("balance");
                break;
                case INST::DC2:
                    abccmd("dc2");
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
            abccmd("fraig");
            abccmd("logic");
            abccmd("mfs2");
            abccmd("sweep");
            abccmd("strash");
            pNew = Abc_FrameReadNtk(abcMgr->get_Abc_Frame_t());
            double fast_error;
            bool skip = false;
                
            if (!(action == INST::RESUB || action == INST::REFACTOR || action == INST::REWRITE || action == INST::BALANCE)){
                fast_error = Simulation(pOrig, pNew, "nmed", 500);
                cout << "fast_error " << fast_error << endl;
                cout << "error_orig " << error_orig << endl;
                if (fast_error - error_orig > 0.01) skip = true;
            }
            if (skip) {
                cout << "skip " << endl;
                pNew = temp_back;
                Abc_FrameReplaceCurrentNetwork(abcMgr->get_Abc_Frame_t(), pNew);
                // abcMgr->get_Abc_Frame_t()->pNtkBackup = nullptr;
                continue;
            }

            double error_after, error_new;

            if (action == INST::RESUB || action == INST::REFACTOR || action == INST::REWRITE || action == INST::BALANCE){
                error_after = error_orig;
            }
            else {
                error_new = Simulation(pOrig, pNew, "nmed", 30000);
                error_after = error_new;
            }
            int node_nums_after = Abc_NtkNodeNum(pNew);
            double diff = 15000* cost_diff(error_orig, error_after, node_nums_orig, node_nums_after);
            std::cout << "cost prob:" << exp(-diff / T)  << "\n"; 
            mt19937 gen2(rd());
            uniform_real_distribution<double> dist2(0,1);

            if (diff < 0 || dist2(gen2) < exp(-diff / T)) {
                std::cout << "Replace! node_num (orig/after): " << node_nums_orig << " " << node_nums_after << "\n"; 
                error_orig = error_after;
                node_nums_orig = node_nums_after;
            }
            else {
                // pNew = abcMgr->get_Abc_Frame_t()->pNtkBackup;
                pNew = temp_back;
                Abc_FrameReplaceCurrentNetwork(abcMgr->get_Abc_Frame_t(), pNew);
                // abcMgr->get_Abc_Frame_t()->pNtkBackup = nullptr;
            }
            // Abc_NtkDelete(pNewDup);
            // Abc_NtkDelete(pNewBackUp);

        }
        // double area = Abc_NtkGetMappedArea(pOrig);
        // std::cout << "area:" << area << "\n"; 
        
        T = r*T;    
    }

    

}