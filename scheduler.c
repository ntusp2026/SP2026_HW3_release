#include "threadtools.h"
#include <sys/signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void assign_jmp_buf(jmp_buf *dest, jmp_buf *src) {
    // usage: assign_jmp_buf(&dest, &src);
    // If src is NULL, dest will be set to all zeros.
    // otherwise, dest will be set to the value of src.
    if (dest == NULL) {
        fprintf(stderr, "Error: NULL pointer passed to assign_jmp_buf\n");
        exit(EXIT_FAILURE);
    }
    if (src == NULL) {
        memset(dest, 0, sizeof(jmp_buf));
    }else{
        memcpy(dest, src, sizeof(jmp_buf));
    }
}

/*
 * Print out the signal you received.
 * If SIGALRM is received, reset the alarm here.
 * After handling a scheduling signal, hand control back to the scheduler.
 */
void sighandler(int signo) {
    // TODO:
    
}

/*
 * Prior to calling this function, both SIGTSTP and SIGALRM should be blocked.
 */
void scheduler() {
    // TODO:
    
}
