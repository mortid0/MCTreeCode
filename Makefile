OPT= -O3
WARN = -Wall

main: 	main.o io.o stack.o tree.o treegrav.o
	cc -o main *.o -lc -lm $(WARN) $(OPT)
main.o: main.c
	cc -c main.c $(WARN) $(OPT)
io.o: 	io.c
	cc -c io.c $(WARN) $(OPT)
stack.o: stack.c
	cc -c stack.c $(WARN) $(OPT)
tree.o: 	tree.c
	cc -c tree.c $(WARN) $(OPT)
treegrav.o:	treegrav.c
	cc -c treegrav.c $(WARN) $(OPT)

clean:
	rm *.o
	rm ./main
