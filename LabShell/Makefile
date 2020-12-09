badsh: main.o exec.so
	gcc -Wall main.o -L. -lexec -o badsh

exec.o: exec.c
	gcc -Wall -fPIC exec.c -c -o exec.o

exec.so: exec.o
	gcc -Wall -shared exec.o -o libexec.so

main.o: main.c
	gcc -Wall main.c -c -o main.o