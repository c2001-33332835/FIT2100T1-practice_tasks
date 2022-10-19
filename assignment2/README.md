## About This Program

This is a program utility to simulate 3 different scheduling algorithms. written in C  
Given a list of processes, the program will simulate the execution,
printing important events to terminal standard output.  
When a process in simulation has completed execution, the prorgam will record the summary of the process to the respective output file.

This project is the attempt of an assignment. The assignment details are as follows:  

 * Course/Unit: Monash University C2001/FIT210
 * Assignment: Programming Assignment 2 (S2, 2022)
 * Student name: Xuanao Zhao
 * Student ID: `33332835`
 * Deadline: 9th of OCT, 2022
 * Contact: `xzha0399@student.monash.edu`

## How to Compile

Thie program only compiles in specific Linux environment.  

Tested Environment:  

 * Arch on WSL2  
 * Manjaro Linux (An arch based destribution)  
 * FITVM-S2-2020-v5 by Monash University (A Debian based distribution)  

##### Method 1

Using the build script. Execute the build script using `./build` command.  
The compiled executables will be in the form of linux binaries, in the name of `task1`, `task2` and `task3`.  
This script builds 3 main files into 3 seperate executables:
 * `task1_33332835.c`
 * `task2_33332835.c`
 * `task3_33332835.c`

##### Method 2

By using any C compiler, compile all C file into object files.  
To compile task 1 file, link all other object files with `task1_33332835.c`
To compile task 2 file, link all other object files with `task2_33332835.c`
To compile task 3 file, link all other object files with `task3_33332835.c`

> Note: The program will need to link the math library `math.c`. (use `-lm` if gcc is the compiler)  

## Prepare Simulation

Prepare a text file named `processes.txt`. Write the process information in the following format:  

 * Lines of processes, where each process takes one line.  
 * Each lines contains space seperated values.  
 * In each line:  
   * First value is process name (without space). Maximum 10 characters  
   * Second value is the entry time. Integer.  
   * Thrid value is the service time. Integer.  
   * Forth value is the deadline.   
 * Make sure the file is correctly define to prevent error.  

Here's a sample of the source file:  
```
P1 0 3 5
P2 1 6 7
P3 4 4 6
P4 6 2 2
P5 20 2 2
```

You can store multiple process information in seperated files with different name.  

## Run the Simulation

After preperation is done, run the executable file to start executing.  
To simulate the First Come First Serve (FCFS) algorithm, run `task1`  
To simulate the Round Robin (RR) with a time quantum of 2 algorithm, run `task2`  
To simulate the Urgent First (UF) algorithm, run `task3`  

With no arguments provided, the process information will be read from `processes.txt` file.  
To simulate processes defined in other files, provide the filename as the first argument.  

## Reading Result

The result of the simulation will be printed to stdout (Console Output). A copy of the summary will be written to the result file.  
The result files are different for different algorithms (task files):  

 * Result for FCFS: `results-1.txt`  
 * Result for RR: `results-2.txt`  
 * Result for UF: `results-3.txt`  

The result file is structured similarly to the source file: A spaced seperated values.  

 * First column: Process name  
 * Second column: Wait time  
 * Third column: Turnaround time  
 * Forth column: Indication of deadline meet. `1` for deadline meet, `0` for deadline miss.

## About the Urgent First (UF) Algorithm

The Urgent First Algorithm is an alogorithm is an algorithm designed and implemented by Xuanao Zhao (myself).  

##### How Does it Work

The way UF works is:  

 * In each round, check which process has a higher urgency  
 * If a process has higher urgency, the currently executing process will be suspended for the more urgent process to execute
 * If a process has higher urgency, but will definitely fail to meet deadline, the process will not be executed. The algorithm focus on processes has higher chance to meet its deadline. This is determined by checking if the remaining time to deadline has sufficient time to finish the process.  

The urgency formulae:  
Let $u$ be urgency of a process, $t$ is the current time iteration, $d$ is the deadline, $s$ is the service time, $t_a$ is the entry time.  
$u = t - (t_a + d)$  

##### Why Does it Work

The reason why this works is that it priotises on processes that is urgent and has a higher chance to complete with in deadline, thus increasing the probability of processes meeting deadlines.  

##### Tested Scenarios

Take this as a test:  
```
P1 0 3 5
P2 1 6 7
P3 4 4 6
P4 6 2 2
```

Here's the result for FCFS:  
```
P1 0 3 1
P2 2 8 0
P3 5 9 0
P4 7 9 0
```

Here's the result for RR:  
```
P1 0 5 1
P4 3 5 0
P3 1 9 0
P2 1 14 0
```

Here's the result for UF:  
```
P1 0 3 1
P4 0 2 1
P2 2 10 0
P3 7 11 0
```

As you can see, UF the algorithm focus on the urgent task of P4, and suspend P2 from execution, so P4 can be finished before deadline expires. 

## Notes

Makesure input file (source) exists and correctly defined, with correct read permission  
Makesure output file (result) has correct write permission.   

To discard the wait time in simulation, change the following line in `process_runner.c`  
```c
// change this
#define TIMEWAIT 1
// to this
#define TIMEWAIT 0
```

## License

Applies to all code in all files unless specified unless specified otherwise.
See LICENSE file.
