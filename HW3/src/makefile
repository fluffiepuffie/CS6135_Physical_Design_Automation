CC = g++
CFlags = -std=c++11 -O3
Include = -I ./

../bin/hw3:  class.o readFile.o floorplan.o  main.o 
	$(CC) $(CFlags) $(Include) $^  -g  -o ../bin/hw3

main.o: main.cpp 
	$(CC) $(CFlags) $(Include) $< -g -c -o $@

readFile.o: readFile.cpp
	$(CC) $(CFlags) $(Include) $<  -g -c -o $@

class.o: class.cpp
	$(CC) $(CFlags) $(Include) $< -g -c -o $@

floorplan.o: floorplan.cpp
	$(CC) $(CFlags) $(Include) $< -g -c -o $@

run:
	../bin/hw3 ../testcase/sample.txt ../output/sample.floorplan

run1:
	../bin/hw3 ../testcase/public1.txt ../output/public1.floorplan

run2:
	../bin/hw3 ../testcase/public2.txt ../output/public2.floorplan

run3:
	../bin/hw3 ../testcase/public3.txt ../output/public3.floorplan

run4:
	../bin/hw3 ../testcase/public4.txt ../output/public4.floorplan

clean: 
	rm *.o ../bin/hw3
