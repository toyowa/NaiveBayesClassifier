CC=/usr/bin/g++
CFLAGS=-O3
LDFLAG=

.PHONY: mnistbayes
mnistbayes: main.o MakeProb.o
	$(CC) $(CFLAGS) -o mnistbayes main.o MakeProb.o $(LDFLAG)

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp 

MakeProb.o: MakeProb.cpp
	$(CC) $(CFLAGS) -c MakeProb.cpp 

.PHONY: clean
clean:
	rm -f *.o mnistbayes
