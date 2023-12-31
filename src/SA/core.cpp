#include "core.h"


INST get_action(){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0,13);
    int result = dist(gen); // get random actions
    // // cout << "result: " << result << "\n";
    // std::cout << inst_strings[result] << "\n";
    return  static_cast<INST>(result);
}

double error_func(double rate, float er_threshold) {
    // return 2*error_rate;
    // smoothened piecewise linear function with slope from 1 -> 5
    return 2*(log(exp(-2*rate+2*er_threshold)+1)+2*rate-2*er_threshold)+rate;
}

double area_func(int node_curr, int node_orig){
    return 0.5*pow((node_curr-node_orig) / node_orig, 4);
}

double cost_diff(double rate_orig, double rate_after, int nums_orig, int nums_after, int nums_init, float er_threshold) {
    //cost function z = 0.1x^2 + ITE(y<5, y , 3y-10), x: "total" area reduction rate, y: error rate
    double error_cost = error_func(rate_after,100*er_threshold) - error_func(rate_orig, 100*er_threshold);
    double area_cost = area_func(nums_after, nums_init) - area_func(nums_orig, nums_init);
    return 20000*(error_cost + area_cost);
}

Abc_Ntk_t* simulated_annealing(Abc_Ntk_t* pOrig, Abc_Ntk_t* pNew, string& error_type, float er_threshold, float& error_record){
    // parameters
    Abc_Ntk_t* pRecord;
    abccmd("map");
    int min_area = Abc_NtkGetMappedArea(Abc_FrameReadNtk(abcMgr->get_Abc_Frame_t()));;
    float T = 500;
    float T_low = 0.5;
    float r = 0.85;
    int iters = 40;
    double error_orig = 0;
    Vec_Ptr_t* vNodes_org = Abc_NtkDfsIter(pOrig, 0);
    int nodes_init = Abc_NtkNodeNum(pNew);
    vector<int> vec_area;
    vector<float> vec_error_rate;
    while(T>T_low){
        abccmd("map");
        double area = Abc_NtkGetMappedArea(Abc_FrameReadNtk(abcMgr->get_Abc_Frame_t()));
        vec_area.push_back(area);
        vec_error_rate.push_back(error_orig);  
        std::cout << "area: " << setw(6) << area << " error: " <<  error_orig << "\n";
        if (error_orig < 0.01) {
            if (area < min_area) {
                min_area = area;
                error_record = error_orig;
                pRecord = Abc_NtkDup(Abc_FrameReadNtk(abcMgr->get_Abc_Frame_t()));
            }
        }
        abccmd("strash");
        pNew = Abc_FrameReadNtk(abcMgr->get_Abc_Frame_t());
        for(int i=0; i<iters; i++){
            bool real_do = false;
            INST action = get_action();
            int round = 1;
            int node_nums_orig = Abc_NtkNodeNum(pNew);
            // if(iters%20==0){
            //     abccmd("map");
            //     double area = Abc_NtkGetMappedArea(Abc_FrameReadNtk(abcMgr->get_Abc_Frame_t()));
            //     // std::cout << "area:" << area << ", current error:"  << error_orig << "\n"; 
            //     std::cout << "(" << area << ", " <<  error_orig << "),\n";
            //     if (error_orig < 0.01) {
            //         if (area < min_area) {
            //             min_area = area;
            //             error_record = error_orig;
            //             pRecord = Abc_NtkDup(Abc_FrameReadNtk(abcMgr->get_Abc_Frame_t()));
            //         }
            //     }
            //     abccmd("strash");
            //     pNew = Abc_FrameReadNtk(abcMgr->get_Abc_Frame_t());
            // }
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
                    case INST::FEC:
                        UpdateNtk_using_FEC(pNew);
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
                    fast_error = Simulation(pOrig, pNew, error_type, 500, vNodes_org, er_threshold);
                    if (fast_error - error_orig > 0.0003 * round) skip = true;
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
                    error_new = Simulation(pOrig, pNew, error_type, 20000, vNodes_org, er_threshold);
                    error_after = error_new;
                }
                int node_nums_after = Abc_NtkNodeNum(pNew);
                double diff = cost_diff(error_orig, error_after, node_nums_orig, node_nums_after, nodes_init, er_threshold);
                // std::cout << "cost prob:" << exp(-diff / T)  << "\n"; 
                mt19937 gen3(rd());
                uniform_real_distribution<double> dist3(0,1);

                if (diff < 0 || dist3(gen3) < exp(-diff / T)) {
                    // std::cout << "Replace! node_num (orig/after): " << node_nums_orig << " " << node_nums_after << "\n"; 
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
    return pRecord;
}