main: 	main.o io.o stack.o tree.o
	cc -o main main.o io.o stack.o -lc -lm
main.o: main.c
	cc -c main.c
io.o: 	io.c
	cc -c io.c
stack.o: stack.c
	cc -c stack.c
tree.o: 	tree.c
	cc -c tree.c

clean:
	rm *.o
	rm ./main
