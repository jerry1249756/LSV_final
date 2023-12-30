# LSV_final

## Approximate Logic Synthesis by Multi-Objective Simulated Annealing

We proposed a method for Approximate Logic Synthesis (ALS) by applying the local approximate change (LACs) with a Multi-Objective Simulated Annealing (MOSA) engine. Our method includes LACs which can reduce the error rate with probability. With an alternation between LACs and exact circuit optimizations, we can explore better solutions compared to the existing ALS tool ALSRAC on EPFL benchmarks.

## Installation

Please execute this code on Linux/ WSL platforms.

1. Clone this directory.
2. Run the script to install and compile the required packages. It will gernerate a program named `approximate` in `bin/` directory.

```
 ./SETUP.sh
 make
```

## Usage

Use the command to execute our program. We support two kinds of error type, `er` and `hd`.

```
./bin/approximate LIB_FILE INPUT_FILE ERROR_TYPE ERROR_RATE
```

We provide the cloned EPFL benchmarks in `benckmarks/`, which can be found in [link](https://github.com/lsils/benchmarks).

There are two kinds of library files in folder `library`, `mcnc.genlib` and `mcnc-aig.genlib`.

For `arithmetic` benchmarks, you need to use `mcnc.genlib` to map the circuit.
For `random_control` benchmarks, you can use both libraries to map the circuit.

The ALS result will be saved into `result/`.
