OPT= -O3
WARN = -Wall
INC = 
LIB = -lc -lm -lgsl -lgslcblas

main: 	main.o io.o stack.o tree.o treegrav.o sl_list.o collision.o
	cc -o main *.o $(INC) $(LIB)  $(WARN) $(OPT)
main.o: main.c sl_list.c tree.c treegrav.c io.c
	cc -c main.c $(WARN) $(OPT)
io.o: 	io.c
	cc -c io.c $(WARN) $(OPT) $(INC)
stack.o: stack.c
	cc -c stack.c $(WARN) $(OPT)
tree.o: 	tree.c
	cc -c tree.c $(WARN) $(OPT)
treegrav.o:	treegrav.c
	cc -c treegrav.c $(WARN) $(OPT)
sl_list.o:	sl_list.c
	cc -c sl_list.c $(WARN) $(OPT)
collision.o:	collision.c collision.h
	cc -c collision.c $(WARN) $(OPT)

clean:
	rm *.o
	rm ./main
