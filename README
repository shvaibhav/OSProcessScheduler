Process Scheduler
=================
Authors:
Erik Kessler
Vaibhav Sharma


Table of Contents
=================
1. Introduction
2. Compiling
3. Usage
4. Theory and Implementation


1 Introduction
=================
Process scheduler runs a simulation of a process scheduler. It reads a
description of processes from a text file. It prints statistics to the console
and optionally may generate a Gantt chart which may be viewed in a web browser.


2 Compiling
=================
Requires GCC 4.8 or later. Tested on Ubuntu 14.04.

1. Change directory to this folder.
2. Run "make clean && make".
3. The binary will be named "scheduler" and stored in your current directory.
   The "html" folder and the "output.html" file must be in the same
   directory as the binary to view the Gantt chart.

3 Usage
=================
Parameters
-h,--help     Print help information.
-q,--quantum  Specify the time quantum. The time quantum must be a positive
              integer greater than zero.
-i,--input    Specify input file. Must be a tab delimited ASCII file with the
              following columns: Process ID, Burst Time, Arrival Time, and
              Initial Priority.
-o,--out      Output Gantt chart. (default)
-n,--noout    Do not output Gantt chart. (suggested for large number of
                processes)
Examples:
./scheduler -q 10 -i process100
./scheduler -q 15 -i process100000 -n


4 Theory and Implementation
=================
There are 100 different priorities numbered 0-99. If a processes has an initial
priority between 50-99 the priority never changes. Processes with a priority
0-49 will have their priorities reduced by the time quantum after their time
quantum expires. These processes will also have their priority increased by 10
if they haven't been scheduled in the last 100 clock ticks. A process with an
initial priority between 0-49 will never have its priority increased to more
than 49.

The program executes as follows:
1.  Parse the data from the input file. Excluding lines that describe invalid
    processes.
2.  Remove processes if their PID is a duplicate of a previous process.
3.  Sort processes by arrival time and PID.
4.  Store the processes in a temporary file to reduce memory demand while
    running the scheduler.
5.  Start the scheduler loop. Run for every clock tick.
6.  The boost queue is stored as a linked list. The processes that need to be
    boosted first are stored at the head of the list. Remove all of the
    processes that need to be boosted this tick from the boost queue.
7.  Remove processes that need to be boosted from the run queue.
8.  Update the priority of boosted processes.
9.  Check if the time quantum has expired for the current running process.
10. Check if any new processes arrive this tick.
11. Merge all processes that are new or have changed priorities so they are in
    PID order.
12. Add these processes to their respective run queues and to the boost queue.
13. If no process is currently running, remove the first process from the
    highest priority run queue.
14. Loop until all processes have terminated.
