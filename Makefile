main: 	main.o io.o stack.o
	cc -o main main.o io.o stack.o -lstd
main.o: main.c
	cc -c main.c
io.o: 	io.c
	cc -c io.c
stack.o: stack.c
	cc -c stack.c

clean:
	rm *.o
