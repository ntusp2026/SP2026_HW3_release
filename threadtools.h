#ifndef THREADTOOL
#define THREADTOOL
#include <setjmp.h>
#include <signal.h>
#include "queue.h"

#define WORKER_MAX 16
#define TASK_MAX 16

struct tcb {
    int id;
    jmp_buf environment;
    int type;
    int arg;
    int i, x, y, z, num[11];
    int priority;  // 0 代表高優先權 (High)，1 代表低優先權 (Low)
    int waiting_for;
    int return_value;
    int has_env; // 0 代表沒有 checkpoint，1 代表有 checkpoint
    int completed;
};

struct worker {
    int worker_id;
    int has_job; // 0 代表空閒，1 代表忙碌
    struct tcb *current_task; // 指向當前正在執行的 tcb
    jmp_buf tmp_env;
};

extern void assign_jmp_buf(jmp_buf *dest, jmp_buf *src);

extern int timeslice;
extern int num_worker;
extern jmp_buf sched_buf;
extern Queue ready_queue_high;
extern Queue ready_queue_low;
extern Queue waiting_queue;
extern struct tcb *CURRENT_TASK;
extern int CURRENT_WORKER_ID;
extern struct worker *workers[WORKER_MAX];
extern int task_completed[TASK_MAX];
extern int task_return_values[TASK_MAX];
extern sigset_t base_mask, tstp_mask, alrm_mask;

void sighandler(int signo);
void scheduler();

// Signals are only unmasked and handled during worker_yield().

// INVARIANT: 任何巨集在 setjmp 回傳非 0 後都不得再執行程式碼，
// 因為其他 task 可能繼承這個 env，並預期進入下一輪worker迴圈。

// TODO:
#define tcb_create(_type, _id, _arg, _init_priority) { \
    struct tcb *new_task = malloc(sizeof(struct tcb)); \
}

#define worker_create(_worker_id) { \
}

#define worker_init(_worker_id) { \
}

#define worker_return(_val) { \
}

#define worker_yield() { \
}

#define worker_join(_task_id) { \
}

#endif // THREADTOOL
