FLAGS = -Wall -Wextra -pedantic -pipe -march=native

.PHONY: all
all : downsample-box demux elementwise-sum

downsample-box : downsample-box.o
	gcc downsample-box.o -o downsample-box
downsample-box.o : downsample-box.c
	gcc $(FLAGS) -c downsample-box.c

demux : demux.o
	gcc demux.o -o demux
demux.o : demux.c
	gcc $(FLAGS) -c demux.c

elementwise-sum : elementwise-sum.o
	g++ elementwise-sum.o -o elementwise-sum
elementwise-sum.o : elementwise-sum.C elementwise-sum.H
	g++ $(FLAGS) -c elementwise-sum.C

clean :
	rm *.o
