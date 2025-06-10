#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_TASKS 100

typedef struct {
    char name[10];
    int priority;
    int cpuBurst;
    int completionTime;
    int waitingTime;
    int turnaroundTime;
} Task;

// Function prototypes for scheduling algorithms
void fcfs(Task tasks[], int n);
void sjf(Task tasks[], int n);
void priorityScheduling(Task tasks[], int n);
void roundRobin(Task tasks[], int n, int timeQuantum);
void priorityWithRoundRobin(Task tasks[], int n, int timeQuantum);

int main() {
    Task tasks[MAX_TASKS];
    int n = 0, choice, timeQuantum = 4;

    FILE *file = fopen("schedule.txt", "r");
    if (!file) {
        perror("Failed to open schedule.txt");
        return 1;
    }

    while (fscanf(file, "%[^,], %d, %d\n", tasks[n].name, &tasks[n].priority, &tasks[n].cpuBurst) == 3) {
        n++;
    }
    fclose(file);

    if (n == 0) {
        printf("No tasks found in schedule.txt.\n");
        return 1;
    }

    printf("Loaded %d tasks from schedule.txt.\n", n);

    do {
        printf("\n--- CPU Scheduling Menu ---\n");
        printf("1. First-Come-First-Served (FCFS)\n");
        printf("2. Shortest Job First (SJF)\n");
        printf("3. Priority Scheduling\n");
        printf("4. Round Robin (RR)\n");
        printf("5. Priority with Round Robin\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                fcfs(tasks, n);
                printf("\n\t--- First-Come-First-Served (FCFS) Results ---\n");
                break;
            case 2:
                sjf(tasks, n);
                printf("\n\t--- Shortest Job First (SJF) Results ---\n");
                break;
            case 3:
                priorityScheduling(tasks, n);
                printf("\n\t--- Priority Scheduling Results ---\n");
                break;
            case 4:
                printf("Enter time quantum: ");
                scanf("%d", &timeQuantum);
                roundRobin(tasks, n, timeQuantum);
                printf("\n\t--- Round Robin (RR) Results ---\n");
                break;
            case 5:
                printf("Enter time quantum: ");
                scanf("%d", &timeQuantum);
                priorityWithRoundRobin(tasks, n, timeQuantum);
                printf("\n\t--- Priority with Round Robin Results ---\n");
                break;
            case 6:
                printf("Exiting program.\n");
                return 0;
            default:
                printf("Invalid choice! Please try again.\n");
                continue;
        }

        // Display task results
        printf("+------+-----------+------------+----------------+----------------+\n");
        printf("| Task | CPU Burst | Completion | Waiting Time   | Turnaround Time |\n");
        printf("+------+-----------+------------+----------------+----------------+\n");

        double totalWaitingTime = 0, totalTurnaroundTime = 0;
        for (int i = 0; i < n; i++) {
            printf("| %-4s | %-9d | %-10d | %-14d | %-15d |\n",
                   tasks[i].name, tasks[i].cpuBurst, tasks[i].completionTime,
                   tasks[i].waitingTime, tasks[i].turnaroundTime);
            totalWaitingTime += tasks[i].waitingTime;
            totalTurnaroundTime += tasks[i].turnaroundTime;
        }
        printf("+------+-----------+------------+----------------+----------------+\n");
        printf("Average Waiting Time: %.2f ms\n", totalWaitingTime / n);
        printf("Average Turnaround Time: %.2f ms\n", totalTurnaroundTime / n);
    } while (choice != 6);

    return 0;
}

// Scheduling implementation
// First-Come-First-Served (FCFS)
void fcfs(Task tasks[], int n) {
    int currentTime = 0;

    printf("\nExecution Order (Gantt Chart): ");
    for (int i = 0; i < n; i++) {
        tasks[i].completionTime = currentTime + tasks[i].cpuBurst;
        tasks[i].turnaroundTime = tasks[i].completionTime;
        tasks[i].waitingTime = tasks[i].turnaroundTime - tasks[i].cpuBurst;
        currentTime = tasks[i].completionTime;

        printf("%s", tasks[i].name);
        if (i != n - 1) {
            printf(" -> ");
        }
    }
    printf("\n");
}

// Shortest Job First (SJF)
void sjf(Task tasks[], int n) {
    int completed = 0, currentTime = 0;
    int isCompleted[MAX_TASKS] = {0};

    printf("\nExecution Order (Gantt Chart): ");
    while (completed < n) {
        int idx = -1, minBurst = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!isCompleted[i] && tasks[i].cpuBurst < minBurst) {
                minBurst = tasks[i].cpuBurst;
                idx = i;
            }
        }

        if (idx != -1) {
            tasks[idx].completionTime = currentTime + tasks[idx].cpuBurst;
            tasks[idx].turnaroundTime = tasks[idx].completionTime;
            tasks[idx].waitingTime = tasks[idx].turnaroundTime - tasks[idx].cpuBurst;
            currentTime = tasks[idx].completionTime;
            isCompleted[idx] = 1;
            completed++;

            printf("%s", tasks[idx].name);
            if (completed < n) {
                printf(" -> ");
            }
        }
    }
    printf("\n");
}

// Priority Scheduling
void priorityScheduling(Task tasks[], int n) {
    int completed = 0, currentTime = 0;
    int isCompleted[MAX_TASKS] = {0};

    printf("\nExecution Order (Gantt Chart): ");
    while (completed < n) {
        int idx = -1, highestPriority = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!isCompleted[i] && tasks[i].priority < highestPriority) {
                highestPriority = tasks[i].priority;
                idx = i;
            }
        }

        if (idx != -1) {
            tasks[idx].completionTime = currentTime + tasks[idx].cpuBurst;
            tasks[idx].turnaroundTime = tasks[idx].completionTime;
            tasks[idx].waitingTime = tasks[idx].turnaroundTime - tasks[idx].cpuBurst;
            currentTime = tasks[idx].completionTime;
            isCompleted[idx] = 1;
            completed++;

            printf("%s", tasks[idx].name);
            if (completed < n) {
                printf(" -> ");
            }
        }
    }
    printf("\n");
}

// Round Robin (RR)
void roundRobin(Task tasks[], int n, int timeQuantum) {
    int remainingBurst[MAX_TASKS];
    for (int i = 0; i < n; i++) {
        remainingBurst[i] = tasks[i].cpuBurst;
    }

    int currentTime = 0, completed = 0;

    printf("\nExecution Order (Gantt Chart): ");
    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (remainingBurst[i] > 0) {
                int execTime = (remainingBurst[i] > timeQuantum) ? timeQuantum : remainingBurst[i];
                remainingBurst[i] -= execTime;
                currentTime += execTime;

                printf("%s", tasks[i].name);
                if (remainingBurst[i] == 0) {
                    tasks[i].completionTime = currentTime;
                    tasks[i].turnaroundTime = tasks[i].completionTime;
                    tasks[i].waitingTime = tasks[i].turnaroundTime - tasks[i].cpuBurst;
                    completed++;
                }
                if (completed < n || remainingBurst[i] > 0) {
                    printf(" -> ");
                }
            }
        }
    }
    printf("\n");
}

// Priority with Round Robin
void priorityWithRoundRobin(Task tasks[], int n, int timeQuantum) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (tasks[j].priority > tasks[j + 1].priority) {
                Task temp = tasks[j];
                tasks[j] = tasks[j + 1];
                tasks[j + 1] = temp;
            }
        }
    }
    roundRobin(tasks, n, timeQuantum);
}
