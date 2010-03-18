FLAGS = -W -Wall -pipe -march=native

.PHONY: all
all : downsample-box demux

downsample-box : downsample-box.o
	gcc downsample-box.o -o downsample-box
downsample-box.o : downsample-box.c
	gcc $(FLAGS) -c downsample-box.c

demux : demux.o
	gcc demux.o -o demux
demux.o : demux.c
	gcc $(FLAGS) -c demux.c

clean :
	rm *.o
