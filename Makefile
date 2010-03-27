FLAGS = -Wall -Wextra -pedantic -pipe -march=native

.PHONY: all
all : downsample-box demux elementwise-sum write-test-pattern

downsample-box : downsample-box.o
	gcc downsample-box.o -o downsample-box
downsample-box.o : downsample-box.c
	gcc $(FLAGS) -std=c99 -c downsample-box.c

demux : demux.o
	gcc demux.o -o demux
demux.o : demux.c
	gcc $(FLAGS) -std=c99 -c demux.c

elementwise-sum : elementwise-sum.o
	g++ elementwise-sum.o -o elementwise-sum
elementwise-sum.o : elementwise-sum.C elementwise-sum.H
	g++ $(FLAGS) -c elementwise-sum.C

write-test-pattern : write-test-pattern.o
	g++ write-test-pattern.o -o write-test-pattern
write-test-pattern.o : write-test-pattern.C write-test-pattern.H
	g++ $(FLAGS) -c write-test-pattern.C

clean :
	rm *.o
