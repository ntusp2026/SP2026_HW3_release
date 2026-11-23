# SP2026 HW3 — User-Level Worker Scheduling

Complete a C scheduler that assigns tasks to workers, handles scheduling signals, and lets tasks wait for other tasks' results. The assignment is worth **7 points**.

## Latest specification

[HW3 Specification on HackMD](https://hackmd.io/@ntusp2026/rknGDsZJmx)

The specification covers scheduling rules, input and output formats, and grading.

## Build and run

Use the course Linux environment with GCC, make, and Bash.

```sh
make
./main test/test3.txt          # Run the program on sample 3
./sample_execution.sh 3       # Check sample 3
./sample_execution.sh all     # Check all five public samples
```

Complete the TODOs before running the corresponding samples. The checker compares stdout with `test/expect<N>.txt`.

## Implementation

| Part | Work | Points |
| --- | --- | ---: |
| 1 | Task and worker setup, scheduling, SIGALRM, and task completion | 2 |
| 2 | Factorial and Find Nth Prime | 1 |
| 3 | Adder and task dependencies with `worker_join()` | 2 |
| 4 | SIGTSTP and priority queues | 2 |

Fibonacci is provided. Complete the TODOs in `scheduler.c`, `threadtools.h`, and `threads.c`. Keep the supplied `main.c`, `queue.c`, `queue.h`, and `Makefile` unchanged.

See the HW3 announcement on NTU COOL for the deadline and submission instructions.
