FLAGS = -W -Wall -pipe -march=native

.PHONY: all
all : downsample-box

downsample-box : downsample-box.o
	gcc downsample-box.o -o downsample-box
downsample-box.o : downsample-box.c
	gcc $(FLAGS) -c downsample-box.c

clean :
	rm *.o
