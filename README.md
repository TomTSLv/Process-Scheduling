#################################
#                               #
# OS Lab 2 Running Instructions # 
#                               #
#################################

Name: Tianshu Lyu
NetID: tl1443

The lab implements four main scheduling algorithms used in operating systems. They are First Come First Serve, Round Robin, Uniprogramming and Shortest Job First. The program takes in a simple txt file containing information for every process as input and gives out three levels of output, i.e. simple output only containing the result information of every process and summary data, more detailed result containing process status for every cycle additionally and a most detailed result also including the random number for calculating the CPU burst.

The following is the instruction for compiling and running the program:

1. Download the OSLab2 directory from access.cims.nyu.edu or from NYU Classes.
2. Open the terminal and use cd command to enter the directory which contains all C files. (e.g.: cd /Desktop/OSLab2)
3. Compile the C project with command: gcc -Wall -std=c99  main.c -o main
4. Run the program with command:
if you want the simple output: 
./main input-1.txt (can also be input-2.txt, input-3.txt,... to input-7.txt)

if you want the detailed result with process status:
./main --verbose input-1.txt

if you want the show-random result:
./main --show-random input-1.txt

(input-1.txt can also be input-2.txt, input-3.txt,... to input-7.txt)

5. Then the program will print out the proper result for every scheduling algorithm with order of FCFS, RR with quantum=2, Uniprogramming and SJF.