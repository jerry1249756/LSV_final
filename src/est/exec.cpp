#include <exec.h>

std::string exec(const char* cmd) {
    // execute the command line and return the string printed on the console
    std::array<char, 128> buffer;
    std::string result;

    #ifdef __linux__ 
    //linux code goes here
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose); 
    #elif _WIN32
    // windows code goes here
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
    #endif

    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::string modelCount(std::string KCBoxPath, std::string FilePath){
    std::string command = KCBoxPath + " ExactMC " + FilePath;
    std::string out = exec(command.c_str());

    auto result = std::vector<std::string>{};
    auto ss = std::stringstream{out};

    for (std::string line; std::getline(ss, line, '\n');)
        result.push_back(line);
    
    std::string countMes = result[result.size()-1]; //extract the model counting message
    countMes = countMes.substr(18);
    return countMes;
}

void test_function(Abc_Ntk_t* pNtk, Abc_Obj_t* pNode){

  //pNode is the XOR node of original and the new one 

  if ( !Abc_ObjIsPi( pNode )){
    Abc_Ntk_t *cone_k = Abc_NtkCreateCone(pNtk, pNode, "New_cone", 0);
    int coneInputNum = Abc_NtkPiNum( cone_k );

    // Abc_FrameReplaceCurrentNetwork(abcMgr->get_Abc_Frame_t(), cone_k);
    // abccmd("show");
    Aig_Man_t *pAig = Abc_NtkToDar(cone_k, 0, 1);//aig manager
    Cnf_Dat_t *pCnf = Cnf_Derive(pAig, 1);//cnf, nOutputs: number of outputs

    Aig_Obj_t * pObj = Aig_ManCo(pAig, 0); //output node

    char* pFileName = "./testcases/output.cnf";            
    Cnf_DataWriteIntoFile( pCnf, pFileName, 0, NULL, NULL );

    //assert output to be 1
    FILE * pFile;
    pFile = fopen( pFileName, "a" );
    fprintf( pFile, "-%d 0 \n",  pCnf->pVarNums[pObj->Id] );
    fprintf( pFile, "-%d 0 \n",  pCnf->pVarNums[pObj->pFanin0->Id] );
    fclose( pFile );

    Cnf_DataFree( pCnf );
    Cnf_ManFree();
    Aig_ManStop( pAig );

    // run KC for model counting
    int count = stoi( modelCount("./build/KCBox", "./testcases/output.cnf"));
    std::cout << "model count: " << count << "\n";
    float err_rate = count / pow(2, coneInputNum);
    std::cout << "error rate: " << err_rate << "\n";

  }else{
    printf("The node is a PI which cannot create CNF.\n");
  }
}