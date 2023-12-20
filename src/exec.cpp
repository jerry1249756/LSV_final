// #include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <vector>
#include <sstream>

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

int modelCount(std::string KCBoxPath, std::string FilePath){
    std::string command = KCBoxPath + " ExactMC " + FilePath;
    std::string out = exec(command.c_str());

    auto result = std::vector<std::string>{};
    auto ss = std::stringstream{out};

    for (std::string line; std::getline(ss, line, '\n');)
        result.push_back(line);
    
    std::string countMes = result[result.size()-1]; //extract the model counting message
    countMes = countMes.substr(18);
    // std::cout << countMes;
    return stoi(countMes);
}


int main(int argc, char const *argv[]) {
    int count = modelCount("./../KCBox/build/KCBox", "./../KCBox/testcases/top2.cnf");
    std::cout << count;
}