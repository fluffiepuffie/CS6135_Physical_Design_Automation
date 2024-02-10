CC = g++
CFlags = -std=c++11 -O3
Includes = -I./ -I/usr/local/include/boost/
.PHONY: run run1 run2 run3 run4 run5 run6 

../bin/hw2:  readFile.o class.h FM.o main.o
	$(CC) $(CFlags) $(Includes) $^ -o ../bin/hw2
main.o: main.cpp
	$(CC) $(CFlags) $(Includes) $< -c -o $@
FM.o: FM.cpp
	$(CC) $(CFlags) $(Includes) $< -c -o $@
readFile.o: readFile.cpp
	$(CC) $(CFlags) $(Includes) $< -c -o $@

clean:
	rm ../bin/hw2
