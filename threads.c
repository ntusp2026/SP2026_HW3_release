#include "threadtools.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Tasks must explicitly call worker_return(val), worker_yield(), or worker_join(task_id). No natural returns allowed.

void worker_main(int worker_id) {
    // worker init
    worker_init(worker_id);
    while(1) {
        // worker got task id, task type, task args
        if (CURRENT_TASK->type == 1) {
            // Fibonacci
            CURRENT_TASK->i++;
            if (CURRENT_TASK->i <= 2) {
                CURRENT_TASK->x = CURRENT_TASK->y = 1;
            } else {
                /* We don't need to save tmp, so it's safe to declare it locally. */
                int tmp = CURRENT_TASK->y;
                CURRENT_TASK->y = CURRENT_TASK->x + CURRENT_TASK->y;
                CURRENT_TASK->x = tmp;
            }
            printf("[Worker %d]Task %d (Fibonacci): f(%d) = %d\n", CURRENT_WORKER_ID, CURRENT_TASK->id, CURRENT_TASK->i, CURRENT_TASK->y);
            sleep(1);
            if (CURRENT_TASK->i == CURRENT_TASK->arg) {
                worker_return(CURRENT_TASK->y);
            } else {
                worker_yield();
            }
        } else if (CURRENT_TASK->type == 2) {
            // Factorial
            // TODO:
            
        } else if (CURRENT_TASK->type == 3) {
            // Find nth prime
            // TODO:
            
        } else if (CURRENT_TASK->type == 4) {
            // Adder
            // TODO:
            
        } else {
            fprintf(stderr, "Error: Unknown task type %d.\n", CURRENT_TASK->type);
            worker_return(-1);
        }
    }

}