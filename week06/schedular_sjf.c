#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#define PS_MAX 10

// holds the scheduling data of one process
typedef struct
{
    int idx;                     // process idx (index)
    int at, bt, rt, wt, ct, tat; // arrival time, burst time, response time, waiting time, completion time, turnaround time.
    int burst;                   // remaining burst (this should decrement when the process is being executed)
} ProcessData;

// the idx of the running process
int running_process = -1; // -1 means no running processes

// the total time of the timer
unsigned total_time; // should increment one second at a time by the scheduler

// data of the processes
ProcessData data[PS_MAX]; // array of process data

// array of all process pids
pid_t ps[PS_MAX]; // zero valued pids - means the process is terminated or not created yet

// size of data array
unsigned data_size;

void read_file(FILE *file)
{

    // Initialize ps array to zeros (processes not created yet).
    memset(ps, 0, sizeof(ps));
    data_size = 0;
    while (fscanf(file, "%d %d", &data[data_size].at, &data[data_size].bt) == 2)
    {
        data[data_size].idx = data_size;
        data[data_size].rt = data[data_size].bt;
        data[data_size].burst = data[data_size].bt;
        data_size++;
    }
}

// reports the metrics and simulation results
void report()
{
    printf("Simulation results.....\n");
    int sum_wt = 0;
    int sum_tat = 0;
    for (int i = 0; i < data_size; i++)
    {
        printf("process %d: \n", i);
        printf("	at=%d\n", data[i].at);
        printf("	bt=%d\n", data[i].bt);
        printf("	ct=%d\n", data[i].ct);
        printf("	wt=%d\n", data[i].wt);
        printf("	tat=%d\n", data[i].tat);
        printf("	rt=%d\n", data[i].rt);
        sum_wt += data[i].wt;
        sum_tat += data[i].tat;
    }

    printf("data size = %d\n", data_size);
    float avg_wt = (float)sum_wt / data_size;
    float avg_tat = (float)sum_tat / data_size;
    printf("Average results for this run:\n");
    printf("	avg_wt=%f\n", avg_wt);
    printf("	avg_tat=%f\n", avg_tat);
}

void check_burst()
{

    for (int i = 0; i < data_size; i++)
        if (data[i].burst > 0)
            return;

    // report simulation results
    report();

    // terminate the scheduler :)
    exit(EXIT_SUCCESS);
}

void resume(pid_t process)
{
    if (process > 0 && ps[process] > 0)
    {
        kill(process, SIGCONT);
    }
}

void suspend(pid_t process)
{
    if (process > 0 && ps[process] > 0)
    {
        kill(process, SIGTSTP);
    }
}

void terminate(pid_t process)
{
    if (process > 0 && ps[process] > 0)
    {
        kill(process, SIGTERM);
    }
}
void create_process(int new_process)
{
    // Stop the currently running process if it exists
    if (running_process >= 0 && ps[running_process] > 0)
    {
        suspend(ps[running_process]);
        printf("Scheduler: Stopping Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
    }

    pid_t new_pid = fork();
    if (new_pid == 0)
    {
        // Child process (worker)
        char process_idx[10];
        snprintf(process_idx, sizeof(process_idx), "%d", new_process);
        execlp("./worker", "./worker", process_idx, NULL);
        perror("Exec failed");
        exit(EXIT_FAILURE);
    }
    else if (new_pid > 0)
    {
        // Parent process
        ps[new_process] = new_pid;                                // Store the child's PID
        running_process = new_process;                            // Set the running process
        data[new_process].rt = total_time - data[new_process].at; // Calculate response time
        printf("Scheduler: Starting Process %d (Remaining Time: %d)\n", new_process, data[new_process].burst);
    }
    else
    {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
}

ProcessData find_next_process()
{
    int location = -1;       // Initialize to an invalid value
    int min_burst = 1000000; // Initialize to a very high value

    for (int i = 0; i < data_size; i++)
    {
        if (data[i].burst > 0 && data[i].at <= total_time)
        {
            if (data[i].burst < min_burst)
            {
                min_burst = data[i].burst;
                location = i;
            }
        }
    }

    if (location == -1)
    {
        // No process has arrived yet or is ready to run.
        printf("Scheduler: Runtime: %u seconds.\nNo processes are ready to run.\n", total_time);
        total_time++;
        return find_next_process(); // Recursively call to wait for processes to arrive.
    }
    return data[location];
}

void schedule_handler(int signum)
{
    total_time++; // Increment the total time

    // Check if there's a running process and update its burst time
    if (running_process >= 0 && ps[running_process] > 0)
    {
        data[running_process].burst--;
        printf("Scheduler: Runtime: %u seconds\n", total_time);
        printf("Process %d is running with %d seconds left\n", running_process, data[running_process].burst);

        // Check if the process has finished its burst time
        if (data[running_process].burst == 0)
        {
            printf("Scheduler: Terminating Process %d (Remaining Time: 0)\n", running_process);
            waitpid(ps[running_process], NULL, 0); 
            data[running_process].ct = total_time;

            // Calculate Turnaround Time (TAT)
            data[running_process].tat = data[running_process].ct - data[running_process].at;

            // Calculate Waiting Time (WT)
            data[running_process].wt = data[running_process].tat - data[running_process].burst;
        }
    }

    // Find the next process to run
    ProcessData next_process = find_next_process();
    create_process(next_process.idx);
    // Call check_burst() to determine if all processes have completed.
    check_burst();
}

int main(int argc, char *argv[])
{

    // read the data file
    FILE *in_file = fopen(argv[1], "r");
    if (in_file == NULL)
    {
        printf("File is not found or cannot open it!\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        read_file(in_file);
    }

    // set a timer
    struct itimerval timer;

    // the timer goes off 1 second after reset
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;

    // timer increments 1 second at a time
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;

    // this counts down and sends SIGALRM to the scheduler process after expiration.
    setitimer(ITIMER_REAL, &timer, NULL);

    // register the handler for SIGALRM signal
    signal(SIGALRM, schedule_handler);

    // Wait till all processes finish
    while (1)
        ; // infinite loop
}