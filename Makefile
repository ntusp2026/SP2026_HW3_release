# 強制指定 -O0 關閉最佳化，避免 setjmp/longjmp/sigsetjmp/siglongjmp 導致區域變數狀態異常
all: main.c scheduler.c queue.c
	gcc -Wall -g -O0 -o main main.c scheduler.c threads.c queue.c

clean:
	rm -f main