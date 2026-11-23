#!/bin/bash

set -u
cd -- "$(dirname -- "$0")" || exit 1

case "${1:-}" in
    1|2|3|4|5|all) ;;
    *) printf 'Usage: %s {1|2|3|4|5|all}\n' "$0" >&2; exit 2 ;;
esac

make || exit 1
child=''
watchdog=''
cleanup() {
    if [ -n "$watchdog" ]; then
        kill "$watchdog" 2>/dev/null || :
        wait "$watchdog" 2>/dev/null || :
    fi
    if [ -n "$child" ]; then
        kill "$child" 2>/dev/null || :
        wait "$child" 2>/dev/null || :
    fi
}
trap cleanup EXIT
trap 'exit 130' INT
trap 'exit 143' TERM

send_tstp() {
    sleep "$1" && kill -TSTP "$child" 2>/dev/null
}

run_sample() {
    local n="$1"
    local signals=0 program=0 output=0
    printf 'Running Test %s...\n' "$n"
    ./main "test/test$n.txt" > "test/out$n.txt" &
    child=$!
    # Bound the run time without changing the child PID used for SIGTSTP.
    (
        timer=''
        trap 'if [ -n "$timer" ]; then kill "$timer" 2>/dev/null || :; fi; exit 0' TERM INT
        sleep 120 &
        timer=$!
        wait "$timer" && kill -KILL "$child" 2>/dev/null
    ) &
    watchdog=$!
    case "$n" in
        2) send_tstp 2.5 || signals=1 ;;
        4) { send_tstp 3.5 && send_tstp 1; } || signals=1 ;;
        5) { send_tstp 1.5 && send_tstp 2; } || signals=1 ;;
    esac
    wait "$child" || program=$?
    kill "$watchdog" 2>/dev/null || :
    wait "$watchdog" 2>/dev/null || :
    child=''
    watchdog=''
    diff -u "test/expect$n.txt" "test/out$n.txt" || output=$?
    if [ "$signals" -eq 0 ] && [ "$program" -eq 0 ] && [ "$output" -eq 0 ]; then
        printf 'Test %s Passed!\n' "$n"
        return 0
    fi
    printf 'Test %s Failed (program=%s, signal delivery=%s, diff=%s).\n' "$n" "$program" "$signals" "$output" >&2
    return 1
}

if [ "$1" = all ]; then
    failed=0
    for n in 1 2 3 4 5; do
        run_sample "$n" || failed=1
    done
    exit "$failed"
fi
run_sample "$1"
