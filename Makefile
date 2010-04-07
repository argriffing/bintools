CFLAGS = -Wall -Wextra -pedantic -pipe -std=c99
CPPFLAGS = -Wall -Wextra -pedantic -pipe -std=c++0x

.PHONY: all
all : downsample-box demux elementwise-sum write-test-pattern sum-axis-0

downsample-box : downsample-box.o
	gcc downsample-box.o -o downsample-box
downsample-box.o : downsample-box.c
	gcc $(CFLAGS) -std=c99 -c downsample-box.c

write-test-pattern : write-test-pattern.o
	gcc write-test-pattern.o -o write-test-pattern
write-test-pattern.o : write-test-pattern.c
	gcc $(CFLAGS) -std=c99 -c write-test-pattern.c

demux : demux.o
	gcc demux.o -o demux
demux.o : demux.c
	gcc $(CFLAGS) -std=c99 -c demux.c

elementwise-sum : elementwise-sum.o
	g++ elementwise-sum.o -o elementwise-sum
elementwise-sum.o : elementwise-sum.C elementwise-sum.H
	g++ $(CPPFLAGS) -c elementwise-sum.C

sum-axis-0 : sum-axis-0.o
	g++ sum-axis-0.o -o sum-axis-0
sum-axis-0.o : sum-axis-0.C sum-axis-0.H
	g++ $(CPPFLAGS) -c sum-axis-0.C

clean :
	rm *.o
