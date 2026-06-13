#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

// ===========================
// Process Structure
// ===========================
typedef struct {
    int pid;              // Process ID
    int arrivalTime;      // Time when process arrives in the ready queue
    int burstTime;        // Total CPU time required
    int remainingTime;    // Remaining CPU time (used in Round Robin)
    int waitingTime;      // Time spent waiting in ready queue
    int turnaroundTime;   // Total time from arrival to completion
    int responseTime;     // Time from arrival to first CPU allocation
    int completionTime;   // Time when process finishes execution
    int startTime;        // First time process gets CPU (-1 if not started)
} Process;

// Initialize process
void initProcess(Process* p, int id, int arrival, int burst) {
    p->pid = id;
    p->arrivalTime = arrival;
    p->burstTime = burst;
    p->remainingTime = burst;
    p->waitingTime = 0;
    p->turnaroundTime = 0;
    p->responseTime = 0;
    p->completionTime = 0;
    p->startTime = -1;
}

// ===========================
// Queue Implementation (for Round Robin)
// ===========================
typedef struct {
    int* items;
    int front;
    int rear;
    int capacity;
    int size;
} Queue;

Queue* createQueue(int capacity) {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->capacity = capacity;
    q->front = 0;
    q->size = 0;
    q->rear = capacity - 1;
    q->items = (int*)malloc(q->capacity * sizeof(int));
    return q;
}

bool isFull(Queue* q) { return (q->size == q->capacity); }
bool isEmpty(Queue* q) { return (q->size == 0); }

void enqueue(Queue* q, int item) {
    if (isFull(q)) return;
    q->rear = (q->rear + 1) % q->capacity;
    q->items[q->rear] = item;
    q->size = q->size + 1;
}

int dequeue(Queue* q) {
    if (isEmpty(q)) return INT_MIN;
    int item = q->items[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size = q->size - 1;
    return item;
}

void freeQueue(Queue* q) {
    free(q->items);
    free(q);
}

// ===========================
// Helper Functions
// ===========================

// Comparison function for qsort (sort by arrival time)
int compareArrivalTime(const void* a, const void* b) {
    Process* p1 = (Process*)a;
    Process* p2 = (Process*)b;
    return p1->arrivalTime - p2->arrivalTime;
}

void calculateAverages(Process* processes, int n, double* avgWaiting, double* avgResponse, double* avgTurnaround) {
    double totalWaiting = 0, totalResponse = 0, totalTurnaround = 0;
    for (int i = 0; i < n; i++) {
        totalWaiting += processes[i].waitingTime;
        totalResponse += processes[i].responseTime;
        totalTurnaround += processes[i].turnaroundTime;
    }
    *avgWaiting = totalWaiting / n;
    *avgResponse = totalResponse / n;
    *avgTurnaround = totalTurnaround / n;
}

void displayMetrics(const char* name, Process* processes, int n) {
    printf("\n========================================\n");
    printf("   %s SCHEDULING RESULTS\n", name);
    printf("========================================\n\n");
    
    printf("%6s %10s %10s %10s %10s %10s %12s\n", 
           "PID", "Arrival", "Burst", "Complete", "Waiting", "Response", "Turnaround");
    printf("--------------------------------------------------------------------\n");
    
    for (int i = 0; i < n; i++) {
        printf("%6d %10d %10d %10d %10d %10d %12d\n",
               processes[i].pid, processes[i].arrivalTime, processes[i].burstTime,
               processes[i].completionTime, processes[i].waitingTime,
               processes[i].responseTime, processes[i].turnaroundTime);
    }
    
    double avgWaiting, avgResponse, avgTurnaround;
    calculateAverages(processes, n, &avgWaiting, &avgResponse, &avgTurnaround);
    
    printf("--------------------------------------------------------------------\n");
    printf("\nAverage Waiting Time:     %.2f units\n", avgWaiting);
    printf("Average Response Time:    %.2f units\n", avgResponse);
    printf("Average Turnaround Time:  %.2f units\n", avgTurnaround);
    printf("\n");
}

// ===========================
// Scheduling Algorithms
// ===========================

void scheduleFCFS(Process* processes, int n) {
    qsort(processes, n, sizeof(Process), compareArrivalTime);
    int currentTime = 0;
    for (int i = 0; i < n; i++) {
        if (currentTime < processes[i].arrivalTime) {
            currentTime = processes[i].arrivalTime;
        }
        processes[i].startTime = currentTime;
        currentTime += processes[i].burstTime;
        processes[i].completionTime = currentTime;
        processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
        processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime;
        processes[i].responseTime = processes[i].startTime - processes[i].arrivalTime;
    }
}

void scheduleSJF(Process* processes, int n) {
    bool* completed = (bool*)calloc(n, sizeof(bool));
    int currentTime = 0;
    int completedCount = 0;
    
    while (completedCount < n) {
        int shortestIndex = -1;
        int shortestBurst = INT_MAX;
        
        for (int i = 0; i < n; i++) {
            if (!completed[i] && processes[i].arrivalTime <= currentTime) {
                if (processes[i].burstTime < shortestBurst ||
                    (processes[i].burstTime == shortestBurst && 
                     processes[i].arrivalTime < processes[shortestIndex].arrivalTime)) {
                    shortestBurst = processes[i].burstTime;
                    shortestIndex = i;
                }
            }
        }
        
        if (shortestIndex == -1) {
            int nextArrival = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (!completed[i] && processes[i].arrivalTime > currentTime) {
                    if (processes[i].arrivalTime < nextArrival)
                        nextArrival = processes[i].arrivalTime;
                }
            }
            currentTime = nextArrival;
            continue;
        }
        
        Process* p = &processes[shortestIndex];
        p->startTime = currentTime;
        currentTime += p->burstTime;
        p->completionTime = currentTime;
        p->turnaroundTime = p->completionTime - p->arrivalTime;
        p->waitingTime = p->turnaroundTime - p->burstTime;
        p->responseTime = p->startTime - p->arrivalTime;
        
        completed[shortestIndex] = true;
        completedCount++;
    }
    free(completed);
}

void scheduleRoundRobin(Process* processes, int n, int timeQuantum) {
    qsort(processes, n, sizeof(Process), compareArrivalTime);
    Queue* readyQueue = createQueue(n * 2); // Buffer size
    int currentTime = 0;
    int completedCount = 0;
    bool* inQueue = (bool*)calloc(n, sizeof(bool));
    int nextProcessIndex = 0;
    
    while (nextProcessIndex < n && processes[nextProcessIndex].arrivalTime <= currentTime) {
        enqueue(readyQueue, nextProcessIndex);
        inQueue[nextProcessIndex] = true;
        nextProcessIndex++;
    }
    
    while (completedCount < n) {
        if (isEmpty(readyQueue)) {
            if (nextProcessIndex < n) {
                currentTime = processes[nextProcessIndex].arrivalTime;
                enqueue(readyQueue, nextProcessIndex);
                inQueue[nextProcessIndex] = true;
                nextProcessIndex++;
            }
        }
        
        int idx = dequeue(readyQueue);
        Process* p = &processes[idx];
        
        if (p->startTime == -1) {
            p->startTime = currentTime;
        }
        
        int executeTime = (p->remainingTime < timeQuantum) ? p->remainingTime : timeQuantum;
        currentTime += executeTime;
        p->remainingTime -= executeTime;
        
        while (nextProcessIndex < n && processes[nextProcessIndex].arrivalTime <= currentTime) {
            enqueue(readyQueue, nextProcessIndex);
            inQueue[nextProcessIndex] = true;
            nextProcessIndex++;
        }
        
        if (p->remainingTime == 0) {
            p->completionTime = currentTime;
            p->turnaroundTime = p->completionTime - p->arrivalTime;
            p->waitingTime = p->turnaroundTime - p->burstTime;
            p->responseTime = p->startTime - p->arrivalTime;
            completedCount++;
        } else {
            enqueue(readyQueue, idx);
        }
    }
    freeQueue(readyQueue);
    free(inQueue);
}

// ===========================
// Main Function
// ===========================
int main() {
    printf("========================================\n");
    printf("   CPU SCHEDULING SIMULATOR (C Version)\n");
    printf("========================================\n\n");
    
    FILE* file = fopen("processes.txt", "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open 'processes.txt'\n");
        return 1;
    }
    
    Process* originalProcesses = NULL;
    int n = 0;
    int capacity = 10;
    originalProcesses = (Process*)malloc(capacity * sizeof(Process));
    
    int pid, arrival, burst;
    while (fscanf(file, "%d %d %d", &pid, &arrival, &burst) == 3) {
        if (n >= capacity) {
            capacity *= 2;
            originalProcesses = (Process*)realloc(originalProcesses, capacity * sizeof(Process));
        }
        initProcess(&originalProcesses[n++], pid, arrival, burst);
    }
    fclose(file);
    
    if (n == 0) {
        fprintf(stderr, "No processes to schedule. Exiting.\n");
        free(originalProcesses);
        return 1;
    }
    
    printf("Successfully loaded %d process(es) from 'processes.txt'\n", n);
    
    // Run FCFS
    Process* p_fcfs = (Process*)malloc(n * sizeof(Process));
    memcpy(p_fcfs, originalProcesses, n * sizeof(Process));
    scheduleFCFS(p_fcfs, n);
    displayMetrics("FCFS (First Come First Serve)", p_fcfs, n);
    free(p_fcfs);
    
    // Run SJF
    Process* p_sjf = (Process*)malloc(n * sizeof(Process));
    memcpy(p_sjf, originalProcesses, n * sizeof(Process));
    scheduleSJF(p_sjf, n);
    displayMetrics("SJF (Shortest Job First - Non-Preemptive)", p_sjf, n);
    free(p_sjf);
    
    // Run Round Robin
    Process* p_rr = (Process*)malloc(n * sizeof(Process));
    memcpy(p_rr, originalProcesses, n * sizeof(Process));
    scheduleRoundRobin(p_rr, n, 2);
    char rr_name[50];
    sprintf(rr_name, "Round Robin (Time Quantum = 2)");
    displayMetrics(rr_name, p_rr, n);
    free(p_rr);
    
    free(originalProcesses);
    return 0;
}