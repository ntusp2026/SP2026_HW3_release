#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "threadtools.h"

int timeslice;
int num_worker;
int num_task;
sigset_t base_mask, tstp_mask, alrm_mask;
Queue ready_queue_high;
Queue ready_queue_low;
Queue waiting_queue;
struct tcb *CURRENT_TASK;
int CURRENT_WORKER_ID = -1;
struct worker *workers[WORKER_MAX];
int task_completed[TASK_MAX];
int task_return_values[TASK_MAX];
jmp_buf sched_buf;

void worker_main(int);

/*
 * This function turns stdin, stdout, and stderr into unbuffered I/O, so:
 *   - you see everything your program prints in case it crashes
 *   - the program behaves the same if its stdout doesn't connect to a terminal
 */
void unbuffered_io() {
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
}

void init_signal() {
    /* initialize the signal masks */
    sigemptyset(&base_mask);
    sigaddset(&base_mask, SIGTSTP);
    sigaddset(&base_mask, SIGALRM);
    sigemptyset(&tstp_mask);
    sigaddset(&tstp_mask, SIGTSTP);
    sigemptyset(&alrm_mask);
    sigaddset(&alrm_mask, SIGALRM);
    /* initialize the signal handlers */
    signal(SIGTSTP, sighandler);
    signal(SIGALRM, sighandler);
    /* block both SIGTSTP and SIGALRM */
    sigprocmask(SIG_SETMASK, &base_mask, NULL);
}

void start_workers() {
    alarm(timeslice);
    scheduler();
}

struct TaskConfig {
    int id;
    int type;
    int arg1;
    int arg2;
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s [test_file.txt]\n", argv[0]);
        exit(1);
    }
    
    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("Failed to open test file");
        exit(1);
    }

    if (fscanf(fp, "%d", &timeslice) != 1 || fscanf(fp, "%d", &num_worker) != 1 || fscanf(fp, "%d", &num_task) != 1) {
        fprintf(stderr, "File format error: Expected timeslice, worker count, and task count.\n");
        fclose(fp);
        exit(1);
    }

    if (num_worker > WORKER_MAX) {
        fprintf(stderr, "Error: Worker count exceeds WORKER_MAX.\n");
        fclose(fp);
        exit(1);
    }

    if (num_worker < 1) {
        fprintf(stderr, "Error: Worker count must be at least 1.\n");
        fclose(fp);
        exit(1);
    }

    if (num_task > TASK_MAX) {
        fprintf(stderr, "Error: Task count exceeds TASK_MAX.\n");
        fclose(fp);
        exit(1);
    }

    struct TaskConfig configs[TASK_MAX];
    for (int i = 0; i < num_task; i++) {
        if (fscanf(fp, "%d %d", &configs[i].id, &configs[i].type) != 2) {
            fprintf(stderr, "File format error: Expected task config for task %d.\n", i);
            fclose(fp);
            exit(1);
        }
        
        if (i != configs[i].id){
            fprintf(stderr, "Error: Task ID %d does not match expected order#%d.\n", configs[i].id, i);
            fclose(fp);
            exit(1);
        }

        if (configs[i].id < 0 || configs[i].id >= TASK_MAX) {
            fprintf(stderr, "Error: Invalid task ID %d. Must be between 0 and %d.\n", configs[i].id, TASK_MAX - 1);
            fclose(fp);
            exit(1);
        }
        if (configs[i].type < 1 || configs[i].type > 4) {
            fprintf(stderr, "Error: Invalid task type %d for task %d.\n", configs[i].type, configs[i].id);
            fclose(fp);
            exit(1);
        }

        if (configs[i].type == 4) {
            fscanf(fp, "%d %d", &configs[i].arg1, &configs[i].arg2);
            if (configs[i].arg1 < 0 || configs[i].arg1 >= TASK_MAX) {
                fprintf(stderr, "Error: Invalid target task ID %d for adder task %d.\n", configs[i].arg1, configs[i].id);
                fclose(fp);
                exit(1);
            }
            if (configs[i].arg2 < 0 || configs[i].arg2 >= TASK_MAX) {
                fprintf(stderr, "Error: Invalid source task ID %d for adder task %d.\n", configs[i].arg2, configs[i].id);
                fclose(fp);
                exit(1);
            }
        } else {
            fscanf(fp, "%d", &configs[i].arg1);
        }
    }
    
    fclose(fp);

    unbuffered_io();
    init_signal();

    queue_init(&ready_queue_high);
    queue_init(&ready_queue_low);
    queue_init(&waiting_queue);

    for (int i = 0; i < num_worker; i++) {
        workers[i] = malloc(sizeof(struct worker));
        workers[i]->worker_id = i;
        workers[i]->has_job = 0;
        workers[i]->current_task = NULL;
        assign_jmp_buf(&workers[i]->tmp_env, NULL);
        worker_create(i);
        printf("Worker %d created\n", workers[i]->worker_id);
    }

    for (int i = 0; i < num_task; i++) {
        if (configs[i].type == 1) {
            tcb_create(configs[i].type, configs[i].id, configs[i].arg1, 0);
        } 
        else if (configs[i].type == 2) {
            tcb_create(configs[i].type, configs[i].id, configs[i].arg1, 0);
        }
        else if (configs[i].type == 3) {
            tcb_create(configs[i].type, configs[i].id, configs[i].arg1, 0);
        } 
        else if (configs[i].type == 4) {
            int packed_arg = (configs[i].arg1 << 16) | (configs[i].arg2 & 0xFFFF);
            tcb_create(configs[i].type, configs[i].id, packed_arg, 0);
        }
        task_completed[configs[i].id] = 0;
        task_return_values[configs[i].id] = 0;
        printf("Task %d created\n", configs[i].id);
    }
    
    start_workers();
}