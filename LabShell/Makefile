badsh: main.o exec.so libswapper.a
	gcc -Wall main.o -L. -lexec -lswapper -o badsh

libswapper.a: swap.o transfer.o swap_files.o
	ar -rs libswapper.a swap.o transfer.o swap_files.o

swap_files.o: swap_files.c
	gcc -Wall swap_files.c -c -o swap_files.o

transfer.o: transfer.c
	gcc -Wall transfer.c -c -o transfer.o

swap.o: swap.c
	gcc -Wall swap.c -c -o swap.o

exec.o: exec.c
	gcc -Wall -fPIC exec.c -c -o exec.o

exec.so: exec.o
	gcc -Wall -shared exec.o -o libexec.so

main.o: main.c
	gcc -Wall main.c -c -o main.o