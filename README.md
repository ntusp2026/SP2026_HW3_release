# SP2026 HW3 — User-Level Worker Scheduling

Complete a C scheduler that assigns tasks to workers, handles scheduling signals, and lets tasks wait for other tasks' results. The assignment is worth **7 points**.

## Latest assignment specification

[HW3 Specification on HackMD](https://hackmd.io/@ntusp2026/rknGDsZJmx)

The specification covers scheduling rules, input and output formats, and grading.

| Part | Work | Points |
| --- | --- | ---: |
| 1 | Task and worker setup, scheduling, SIGALRM, and task completion | 2 |
| 2 | Factorial and Find Nth Prime | 1 |
| 3 | Adder and task dependencies with `worker_join()` | 2 |
| 4 | SIGTSTP and priority queues | 2 |

## Build and develop

Use the course Linux environment with GCC, make, and Bash.

```sh
make
./main test/test3.txt          # Run the program on sample 3
./sample_execution.sh 3       # Check sample 3
./sample_execution.sh all     # Check all five public samples
```

Complete the TODOs before running the corresponding samples. The checker compares stdout with `test/expect<N>.txt`.

## Provided versus student work

Fibonacci is provided. Complete the TODOs in `scheduler.c`, `threadtools.h`, and `threads.c`. Keep the supplied `main.c`, `queue.c`, `queue.h`, and `Makefile` unchanged.

## Submission

Submit `<student_id>_hw3.zip` on NTU COOL. Place the following files directly at the top level of the ZIP:

```text
b14902xxx_hw3.zip
├── Makefile
├── main.c
├── queue.c
├── queue.h
├── scheduler.c
├── threads.c
└── threadtools.h
```

After extraction, `make` must produce `main`. Keep the provided `main.c`, `queue.c`, `queue.h`, and `Makefile` unchanged. Exclude executables, object files, test outputs, the checker, and Git history.

See the HW3 announcement for the deadline and late policy.

## Questions and updates

Check the HackMD specification and existing [GitHub Issues](https://github.com/ntusp2026/SP2026_HW3_release/issues) before opening a new issue. Include the error and what you tried.
