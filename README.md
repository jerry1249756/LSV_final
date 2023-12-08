# LSV_final

## Setup
1. execute SETUP.sh

## Installation


### File Hierachy

1. Clone and build this directory.
2. Download and build the following tools _outside_ this directory:
   - Knowledge compilation tool `KCBox`, which can be found in https://github.com/meelgroup/KCBox.
   - Synthesis tool `ABC`, which can be found in https://github.com/berkeley-abc/abc.
3. Modify the relative to the compiled binary in `main.config`.

# How to use ABC in main.cpp
1. the pNtk in abc is abcMgr->get_Abc_Frame_t()->pNtkCur
2. then you can use pNtk and function in abc to do something