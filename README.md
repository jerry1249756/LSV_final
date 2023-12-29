# LSV_final

## Installation


### File Hierachy

1. Clone and build this directory.
2. Download and build the following tools _outside_ this directory:
   - Synthesis tool `ABC`, which can be found in https://github.com/berkeley-abc/abc.


## Setup Environment
use script SETUP.sh in this directory to setup environment (the `abc` tool)

    ./SETUP.sh


<!-- ## How to use ABC in main.cpp
1. the pNtk in abc is abcMgr->get_Abc_Frame_t()->pNtkCur
2. then you can use pNtk and function in abc to do something -->

## Build Approximate
generate a executable program

    make
It will gernerate a program named `approximate` in `bin`  
the benchmarks are in `BLIF` format in the folder `benchmark`  
there are two kinds of circuit, `arithmetic` and `random_control`  
there are two kinds of library file in folder `library`, `mcnc.genlib` and `mcnc-aig.genlib`  
for `arithmetic`, you need to use `mcnc.genlib` to map the circuit  
for `random_control`, you can use both library to map the circuit

use the command below to execute our program.
we support two error type, `er` and `nmed`.

 `./bin/approximate LIB_FILE INPUT_FILE ERROR_TYPE ERROR_RATE`

 in this case, the approximate circuit is saved into `result/`.

