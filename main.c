#include <stdio.h>
#include <stdlib.h> 
#include "queue.h"
#include "readConfig.h"


Node* priorityQueue = NULL;
Node* cpuQueue = NULL;
Node* CPU = NULL;
Node* disk1Queue = NULL;
Node* disk1 = NULL;
Node* disk2Queue = NULL;
Node* disk2 = NULL;
Node* networkQueue = NULL;
Node* network = NULL;

int cpuJobsCompleted = 0;
int disk1JobsCompleted = 0;
int disk2JobsCompleted = 0;
int networkJobsCompleted = 0;
int cpuUtilized = 0;
int disk1Utilized = 0;
int disk2Utilized = 0;
int networkUtilized = 0;


int SEED;
int INIT_TIME;
int FIN_TIME;
int ARRIVE_MIN;
int ARRIVE_MAX;
int QUIT_PROB;
int NETWORK_PROB;
int CPU_MIN;
int CPU_MAX;
int DISK1_MIN;
int DISK1_MAX;
int DISK2_MIN;
int DISK2_MAX;
int NETWORK_MIN;
int NETWORK_MAX;

int currentTime;

int jobNumber = 1;

int randomNumGenerator(int min, int max)
{
    int num = (rand() % (max - min + 1)) + min;
    return num;
}

void disk2Control(int number, int priority) {
    pushFifo(&disk2Queue, number, priority);
    if(isEmpty(&disk2)) {
        push(&disk2, peek(&disk2Queue), getPriority(&disk2Queue));
        pop(&disk1Queue);
        int j = randomNumGenerator(DISK2_MIN, DISK2_MAX);
        if(currentTime + cpuUtilized + j <= FIN_TIME) {
            disk2Utilized += j;
            disk2JobsCompleted += 1;
        }
        if(currentTime + cpuUtilized + j <= FIN_TIME) {
            printf("At time %d, Job%d finishes at disk2.\n", currentTime + cpuUtilized + disk2Utilized, peek(&disk2));
        }
        push(&priorityQueue, peek(&disk2), getPriority(&disk2));
        pop(&disk2);
    }
}

void disk1Control(int number, int priority) {
    pushFifo(&disk1Queue, number, priority);
    if(isEmpty(&disk1)) {
        push(&disk1, peek(&disk1Queue), getPriority(&disk1Queue));
        pop(&disk1Queue);
        int j = randomNumGenerator(DISK1_MIN, DISK1_MAX);
        if(currentTime + cpuUtilized + j <= FIN_TIME) {
            disk1Utilized += j;
            disk1JobsCompleted += 1;
        }
        if(currentTime + cpuUtilized + j <= FIN_TIME) {
            printf("At time %d, Job%d finishes at disk1.\n", currentTime + cpuUtilized + disk1Utilized, peek(&disk1));
        }
        push(&priorityQueue, peek(&disk1), getPriority(&disk1));
        pop(&disk1);
    }
}

void networkControl(int number, int priority) {
    pushFifo(&networkQueue, number, priority);
    if(isEmpty(&network)) {
        push(&network, peek(&networkQueue), getPriority(&networkQueue));
        pop(&networkQueue);
        int k = randomNumGenerator(NETWORK_MIN, NETWORK_MAX);
        if(currentTime + cpuUtilized + k <= FIN_TIME) {
            networkUtilized += k;
            networkJobsCompleted += 1;
        }
        if(currentTime + cpuUtilized + k <= FIN_TIME) {
            printf("At time %d, Job%d finishes at network.\n", currentTime + cpuUtilized + networkUtilized, peek(&network));
        }
        push(&priorityQueue, peek(&network), getPriority(&network));
        pop(&network);
    }
}

void cpuControl(int number, int priority)
{
    pushFifo(&cpuQueue, number, priority);
    if(isEmpty(&CPU)) {
        push(&CPU, peek(&cpuQueue), getPriority(&cpuQueue));
        pop(&cpuQueue);
        int y = randomNumGenerator(CPU_MIN, CPU_MAX);
        if(currentTime + y) {
            cpuUtilized += y;
            cpuJobsCompleted += 1;
        }
        int z = randomNumGenerator(1, 100);
        if(z <= QUIT_PROB && (currentTime + y) <= FIN_TIME) {
            printf("At time %d, Job%d exits from CPU.\n", currentTime + cpuUtilized, peek(&CPU));
        }
        else if (z > QUIT_PROB && (currentTime + y) <= FIN_TIME){
            int i = randomNumGenerator(1, 100);
            if(i <= NETWORK_PROB) {
                networkControl(peek(&CPU), getPriority(&CPU));
            }
            else {
                if(queueSize(&disk1Queue) <= queueSize(&disk2Queue)) {
                    disk1Control(peek(&CPU), getPriority(&CPU));
                }
                else {
                    disk2Control(peek(&CPU), getPriority(&CPU));
                }
            }
        }
        pop(&CPU);
    }
}

int main() 
{
    SEED = readFile("config.h", 1);
    INIT_TIME = readFile("config.h", 2);
    FIN_TIME = readFile("config.h", 3);
    ARRIVE_MIN = readFile("config.h", 4);
    ARRIVE_MAX = readFile("config.h", 5);
    QUIT_PROB = readFile("config.h", 6);
    NETWORK_PROB = readFile("config.h", 7);
    CPU_MIN = readFile("config.h", 8);
    CPU_MAX = readFile("config.h", 9);
    DISK1_MIN = readFile("config.h", 10);
    DISK1_MAX = readFile("config.h", 11);
    DISK2_MIN = readFile("config.h", 12);
    DISK2_MAX = readFile("config.h", 13);
    NETWORK_MIN = readFile("config.h", 14);
    NETWORK_MAX = readFile("config.h", 15);
    currentTime = INIT_TIME;
    srand(SEED);
    push(&priorityQueue, jobNumber, currentTime);
    printf("At time %d, Job%d arrives.\n", currentTime, jobNumber);
    cpuControl(peek(&priorityQueue), getPriority(&priorityQueue));
    pop(&priorityQueue);
    jobNumber+= 1;
    while(currentTime <= FIN_TIME) {
        int x = randomNumGenerator(ARRIVE_MIN, ARRIVE_MAX);
        currentTime += x;
        if(currentTime <= FIN_TIME) {
            push(&priorityQueue, jobNumber, currentTime);
            printf("At time %d, Job%d arrives.\n", currentTime, jobNumber);
            cpuControl(peek(&priorityQueue), getPriority(&priorityQueue));
            pop(&priorityQueue);
            jobNumber+= 1;
        }
    }
    printf("CPU utilization was %f.\n", (double)cpuUtilized/FIN_TIME);
    printf("DISK1 utilization was %f.\n", (double)disk1Utilized/FIN_TIME);
    printf("DISK2 utilization was %f.\n", (double)disk2Utilized/FIN_TIME);
    printf("Network utilization was %f.\n", (double)networkUtilized/FIN_TIME);
    printf("Throughput of CPU was was %f.\n", (double)cpuJobsCompleted/FIN_TIME);
    printf("Throughput of DISK1 was was %f.\n", (double)disk1JobsCompleted/FIN_TIME);
    printf("Throughput of DISK2 was was %f.\n", (double)disk2JobsCompleted/FIN_TIME);
    printf("Throughput of Network was was %f.\n", (double)networkJobsCompleted/FIN_TIME);
}

