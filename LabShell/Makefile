badsh: main.o exec.o exec_pipe.o libswapper.so
	gcc -Wall exec.o exec_pipe.o main.o -ldl -o badsh

libswapper.so: swap.o transfer.o swap_files.o
	gcc -Wall -shared swap.o transfer.o swap_files.o -o libswapper.so

swap_files.o: swap_files.c
	gcc -Wall -fPIC swap_files.c -c -o swap_files.o

transfer.o: transfer.c
	gcc -Wall -fPIC transfer.c -c -o transfer.o

swap.o: swap.c
	gcc -Wall -fPIC swap.c -c -o swap.o

exec_pipe.o: exec_pipe.c
	gcc -Wall exec_pipe.c -c -o exec_pipe.o

exec.o: exec.c
	gcc -Wall exec.c -c -o exec.o

main.o: main.c
	gcc -Wall main.c -c -o main.o

clean_txt:
	rm *.txt

clean_obj:
	rm *.o