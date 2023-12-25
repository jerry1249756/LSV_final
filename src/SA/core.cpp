#include "core.h"


INST get_action(){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0,10);
    int result = dist(gen); // get random actions
    // // cout << "result: " << result << "\n";
    std::cout << inst_strings[result] << "\n";
    return  static_cast<INST>(result);
}

double error_func(double rate) {
    // return 2*error_rate;
    // smoothened piecewise linear function with slope from 1 -> 5
    return 2*(log(exp(-2*rate+10)+1)+2*rate-10)+rate;
}

double area_func(int node_curr, int node_orig){
    return 0.5*pow((node_curr-node_orig) / node_orig, 4);
}

double cost_diff(double rate_orig, double rate_after, int nums_orig, int nums_after, int nums_init) {
    //cost function z = 0.1x^2 + ITE(y<5, y , 3y-10), x: "total" area reduction rate, y: error rate
    double error_cost = error_func(rate_after) - error_func(rate_orig);
    double area_cost = area_func(nums_after, nums_init) - area_func(nums_orig, nums_init);
    return 20000*(error_cost + area_cost);
}

void simulated_annealing(Abc_Ntk_t* pOrig, Abc_Ntk_t* pNew){
    // parameters
    float T = 500;
    float T_low = 0.5;
    float r = 0.85;
    int iters = 40;
    double error_orig = 0;
    std::string error_type = "nmed";
    Vec_Ptr_t* vNodes_org = Abc_NtkDfsIter(pOrig, 0);
    int nodes_init = Abc_NtkNodeNum(pNew);
    while(T>T_low){
        abccmd("map");
        double area = Abc_NtkGetMappedArea(Abc_FrameReadNtk(abcMgr->get_Abc_Frame_t()));
        // std::cout << "area:" << area << ", current error:"  << error_orig << "\n"; 
        std::cout <<  "(" << area << ", " <<  error_orig << "),\n";
        abccmd("strash");
        pNew = Abc_FrameReadNtk(abcMgr->get_Abc_Frame_t());
        for(int i=0; i<iters; i++){
            bool real_do = false;
            INST action = get_action();
            // INST action = INST(i%2 + 1);
            // std::cout << inst_strings[int(action)] << "\n";
            int round = 1;
            int node_nums_orig = Abc_NtkNodeNum(pNew);
            while (!real_do && round < 20) { //tolerance threshold
                // if (node_nums_orig < 20) return;
                if(abcMgr->get_Abc_Frame_t()->pNtkBackup){
                    Abc_NtkDelete( abcMgr->get_Abc_Frame_t()->pNtkBackup);
                }
                Abc_Ntk_t* temp_back = Abc_NtkDup(pNew);
                
                random_device rd;
                mt19937 gen1(rd());
                uniform_int_distribution<int> dist1(0,Abc_NtkObjNum(pNew)-1);
                mt19937 gen2(rd());
                uniform_int_distribution<int> dist2(0,Abc_NtkPiNum(pNew)-1);
                Abc_Obj_t* pNode;
                while (!Abc_ObjIsNode(pNode = Abc_NtkObj(pNew,dist1(gen1))));
                // cout << Abc_ObjName(pNode) << endl;
                switch(action){
                    case INST::ORCHESTRATE:
                        abccmd("orchestrate -l");
                    break;
                    case INST::CSWEEP:
                        abccmd("csweep");
                    break;
                    case INST::DFRAIG:
                        abccmd("dfraig");
                    break;
                    case INST::BALANCE:
                        abccmd("balance");
                    break;
                    case INST::DC2:
                        abccmd("dc2");
                    break;
                    case INST::CONST1:
                        UpdateNtk_const1_propagate(pNew);
                    break;
                    case INST::CONST0:
                        UpdateNtk_const0_propagate(pNew);
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
                    case INST::ADDANDNODE:
                        UpdateNtk_add_And_node(pNew, Abc_ObjFanout0(pNode), pNode, Abc_NtkPi(pNew,dist2(gen2)));
                    break;
                    case INST::ADDXORNODE:
                        UpdateNtk_add_Xor_node(pNew, Abc_ObjFanout0(pNode), pNode, Abc_NtkPi(pNew,dist2(gen2)));
                    break;
                    case INST::ADDORNODE:
                        UpdateNtk_add_Or_node(pNew, Abc_ObjFanout0(pNode), pNode, Abc_NtkPi(pNew,dist2(gen2)));
                    break;
                }
                // abccmd("ifraig");
                // abccmd("logic");
                // abccmd("mfs2");
                // abccmd("strash");
                // abccmd("csweep");
                pNew = Abc_FrameReadNtk(abcMgr->get_Abc_Frame_t());
                double fast_error;
                bool skip = false;
                    
                if (!(action == INST::ORCHESTRATE || action == INST::CSWEEP || action == INST::DFRAIG || action == INST::BALANCE || action == INST::DC2)){
                    fast_error = Simulation(pOrig, pNew, error_type, 500, vNodes_org);
                    if (fast_error - error_orig > 0.0005 * round) skip = true;
                }
                if (skip) {
                    // cout << "skip " << endl;
                    round++;
                    pNew = temp_back;
                    Abc_FrameReplaceCurrentNetwork(abcMgr->get_Abc_Frame_t(), pNew);
                    continue;
                }
                real_do = true;

                double error_after, error_new;

                if (action == INST::ORCHESTRATE || action == INST::CSWEEP || action == INST::DFRAIG || action == INST::BALANCE || action == INST::DC2){
                    error_after = error_orig;
                }
                else {
                    error_new = Simulation(pOrig, pNew, "er", 30000, vNodes_org);
                    error_after = error_new;
                }
                int node_nums_after = Abc_NtkNodeNum(pNew);
                double diff = 15000* cost_diff(error_orig, error_after, node_nums_orig, node_nums_after);
                // std::cout << "cost prob:" << exp(-diff / T)  << "\n"; 
                mt19937 gen2(rd());
                uniform_real_distribution<double> dist2(0,1);

                if (diff < 0 || dist2(gen2) < exp(-diff / T)) {
                    std::cout << "Replace! node_num (orig/after): " << node_nums_orig << " " << node_nums_after << " " << node_nums_orig-node_nums_after << "\n"; 
                    error_orig = error_after;
                    node_nums_orig = node_nums_after;
                }
                else {
                    // pNew = abcMgr->get_Abc_Frame_t()->pNtkBackup;
                    pNew = temp_back;
                    Abc_FrameReplaceCurrentNetwork(abcMgr->get_Abc_Frame_t(), pNew);
                    // abcMgr->get_Abc_Frame_t()->pNtkBackup = nullptr;
                }
            }
        }
        T = r*T;    
        if(T>200 && r<=0.95) r += 0.01;
        else if(r>=0.85) r -= 0.01;
    }
}