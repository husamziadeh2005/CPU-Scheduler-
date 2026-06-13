# CPU Scheduling Test Cases

This directory contains multiple test files to thoroughly test all three scheduling algorithms.

## Test Files

### test1_basic.txt - **Basic Test**
```
PID  Arrival  Burst
1    0        5
2    1        3
3    2        8
4    3        6
```
- **Purpose**: Standard test with staggered arrivals
- **Tests**: Basic functionality of all algorithms

### test2_simultaneous.txt - **Simultaneous Arrivals**
```
PID  Arrival  Burst
1    0        8
2    0        4
3    0        2
4    0        6
5    0        3
```
- **Purpose**: All processes arrive at same time
- **Tests**: SJF should show order 3→5→2→4→1, FCFS uses PID order, RR cycles through all

### test3_idle_time.txt - **CPU Idle Time**
```
PID  Arrival  Burst
1    0        4
2    5        3
3    12       2
4    20       5
```
- **Purpose**: Large gaps between arrivals
- **Tests**: CPU idle time handling (waits at times 4-5, 8-12, 14-20)

### test4_equal_burst.txt - **Equal Burst Times**
```
PID  Arrival  Burst
1    0        5
2    1        5
3    2        5
4    3        5
```
- **Purpose**: All processes have same burst time
- **Tests**: Tie-breaking (should use arrival time - FCFS order)

### test5_mixed_lengths.txt - **Mixed Short/Long**
```
PID  Arrival  Burst
1    0        10
2    1        1
3    2        15
4    3        2
5    4        20
6    5        3
```
- **Purpose**: Mix of very short and very long processes
- **Tests**: SJF prioritizes short jobs (2, 4, 6 first), RR provides fairness

### test6_round_robin.txt - **Round Robin Intensive**
```
PID  Arrival  Burst
1    0        7
2    2        5
3    3        3
4    5        4
5    6        6
```
- **Purpose**: Multiple processes requiring several time slices
- **Tests**: Queue management, multiple cycles through processes

### test7_single.txt - **Single Process**
```
PID  Arrival  Burst
1    0        3
```
- **Purpose**: Edge case with only one process
- **Tests**: All algorithms should produce identical results

### test8_delayed_start.txt - **Delayed Start**
```
PID  Arrival  Burst
1    5        4
2    10       3
3    15       5
```
- **Purpose**: No process arrives at time 0
- **Tests**: Initial CPU idle, scheduler waits until first arrival

## How to Use

To test with a specific file, modify line 326 in `scheduler.cpp`:

```cpp
vector<Process> originalProcesses = readProcessesFromFile("test1_basic.txt");
```

Replace `"processes.txt"` with the desired test file name.

## Expected Behaviors

### FCFS (First Come First Serve)
- Executes in strict arrival order
- No preemption
- Simple but can cause long waiting for short jobs

### SJF (Shortest Job First)
- Selects shortest available job
- Minimizes average waiting time
- Can cause starvation for long jobs

### Round Robin (Quantum = 2)
- Fair time-sharing
- Each process gets 2 time units per turn
- Prevents starvation, good response time

## Validation Tips

1. **Manual Calculation**: Pick a simple test and calculate metrics by hand
2. **Compare Algorithms**: Same input should give different results across algorithms
3. **Check Metrics**: 
   - Turnaround Time = Completion - Arrival
   - Waiting Time = Turnaround - Burst
   - Response Time = Start - Arrival
4. **Verify Averages**: Sum all values and divide by number of processes
