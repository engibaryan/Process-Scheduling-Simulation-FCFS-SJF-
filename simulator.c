#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
    int response_time;
} Process;

void fcfs(Process *processes, int n) {
    int current_time = 0;
    double total_waiting_time = 0, total_turnaround_time = 0, total_response_time = 0;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (processes[i].arrival_time > processes[j].arrival_time) {
                Process temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }

    printf("=== First Come First Served (FCFS) ===\n");
    printf("Gantt Chart: ");
    for (int i = 0; i < n; i++) {
        if (current_time < processes[i].arrival_time)
            current_time = processes[i].arrival_time;

        processes[i].waiting_time = current_time - processes[i].arrival_time;
        processes[i].turnaround_time = processes[i].waiting_time + processes[i].burst_time;
        processes[i].response_time = processes[i].waiting_time;

        total_waiting_time += processes[i].waiting_time;
        total_turnaround_time += processes[i].turnaround_time;
        total_response_time += processes[i].response_time;

        printf("| P%d ", processes[i].pid);
        current_time += processes[i].burst_time;
    }
    printf("|\n");

    printf("PID\tAT\tBT\tWT\tTAT\tRT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               processes[i].pid,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].waiting_time,
               processes[i].turnaround_time,
               processes[i].response_time);
    }

    printf("Average Waiting Time: %.2f\n", total_waiting_time / n);
    printf("Average Turnaround Time: %.2f\n", total_turnaround_time / n);
    printf("Average Response Time: %.2f\n", total_response_time / n);
}
void sjf(Process *processes, int n) {
    int current_time = 0;
    double total_waiting_time = 0, total_turnaround_time = 0, total_response_time = 0;

    int completed = 0, shortest = 0, finish_time;
    int remaining[n];
    for (int i = 0; i < n; i++) {
        remaining[i] = processes[i].burst_time;
    }
    int minm = INT_MAX;
    int check = 0;

    while (completed != n) {
        for (int i = 0; i < n; i++) {
            if ((processes[i].arrival_time <= current_time) && (remaining[i] < minm) && remaining[i] > 0) {
                minm = remaining[i];
                shortest = i;
                check = 1;
            }
        }

        if (check == 0) {
            current_time++;
            continue;
        }

        remaining[shortest]--;
        minm = remaining[shortest] == 0 ? INT_MAX : remaining[shortest];

        if (remaining[shortest] == 0) {
            completed++;
            check = 0;
            finish_time = current_time + 1;

            processes[shortest].turnaround_time = finish_time - processes[shortest].arrival_time;
            processes[shortest].waiting_time = finish_time - processes[shortest].burst_time - processes[shortest].arrival_time;
            processes[shortest].response_time = processes[shortest].waiting_time;

            total_waiting_time += processes[shortest].waiting_time;
            total_turnaround_time += processes[shortest].turnaround_time;
            total_response_time += processes[shortest].response_time;
        }
        current_time++;
    }

    printf("=== Shortest Job First (SJF) ===\n");
    printf("Gantt Chart: ");
    for (int i = 0; i < n; i++) {
        printf("| P%d ", processes[i].pid);
        for (int j = 0; j < processes[i].burst_time; j++) printf(" ");
    }
    printf("|\n");

    printf("PID\tAT\tBT\tWT\tTAT\tRT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               processes[i].pid,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].waiting_time,
               processes[i].turnaround_time,
               processes[i].response_time);
    }

    printf("Average Waiting Time: %.2f\n", total_waiting_time / n);
    printf("Average Turnaround Time: %.2f\n", total_turnaround_time / n);
    printf("Average Response Time: %.2f\n", total_response_time / n);
}


int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    Process *processes = malloc(n * sizeof(Process));

    for (int i = 0; i < n; i++) {
        printf("Enter arrival and burst time for process %d: ", i + 1);
        scanf("%d %d", &processes[i].arrival_time, &processes[i].burst_time);
        processes[i].pid = i + 1;
    }

    fcfs(processes, n);
    sjf(processes, n);
    free(processes);
    return 0;
}

