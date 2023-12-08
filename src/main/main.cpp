#include <iostream>
#include "gvAbcMgr.h"
#include "base/abc/abc.h"
#include "bdd/cudd/cudd.h"
#include "gvAbcNtk.h"

extern AbcMgr*              abcMgr;

void abccmd(string);


int main(int argc, char** argv) {
    abcMgr = new AbcMgr;
    string a = "adder.blif";
    abcMgr->abcReadDesign(a);

    // calling abc's command
    abccmd("strash");
    abccmd("write_cnf temp.cnf");

    return 0;
}

void abccmd(string command){
    char Command[1024], abcCmd[128];
    strcpy(abcCmd, command.c_str());
    sprintf(Command, "%s", abcCmd);
    Cmd_CommandExecute(abcMgr->get_Abc_Frame_t(), Command);
}