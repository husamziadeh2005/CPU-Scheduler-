CPU Scheduling Simulator

A comprehensive CPU Scheduling Simulator implemented in C++, designed to demonstrate and compare various scheduling algorithms used in modern operating systems.

🚀 Features

This simulator provides implementations for the following scheduling algorithms:

•
First-Come, First-Served (FCFS): A non-preemptive algorithm that executes processes in the order they arrive.

•
Shortest Job First (SJF): A non-preemptive algorithm that selects the process with the smallest burst time.

•
Round Robin (RR): A preemptive algorithm that assigns a fixed time unit (quantum) to each process in a circular order.

📊 Metrics Tracked

The simulator calculates and displays key performance metrics for each algorithm:

•
Completion Time: The time at which a process finishes execution.

•
Waiting Time: The total time a process spends waiting in the ready queue.

•
Turnaround Time: The total time from process arrival to completion.

•
Response Time: The time from arrival to the first time the process is allocated to the CPU.

•
Averages: Overall average waiting, response, and turnaround times for the entire process set.

🛠️ Getting Started



A terminal or command prompt.

Bash:

Compile the source code:

Bash:

gcc cpu_scheduler.c -o scheduler


Running the Simulator

The simulator reads process data from a file named processes.txt. Ensure this file exists in the same directory as the executable.

Input File Format (processes.txt )

Each line should contain: PID ArrivalTime BurstTime

Plain Text:
1 0 5
2 1 3
3 2 8
4 3 6

Execution

Bash:
./scheduler


📝 Example Output

Plain Text

========================================
   FCFS (First Come First Serve) SCHEDULING RESULTS
========================================

   PID    Arrival      Burst   Complete    Waiting   Response   Turnaround
--------------------------------------------------------------------
     1          0          5          5          0          0            5
     2          1          3          8          4          4            7
     3          2          8         16          6          6           14
     4          3          6         22         13         13           19
--------------------------------------------------------------------

Average Waiting Time:     5.75 units
Average Response Time:    5.75 units
Average Turnaround Time:  11.25 units
