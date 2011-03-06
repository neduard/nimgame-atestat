testcase: testcase.o engine.o
	g++ -g -o testcase testcase.o engine.o

testcase.o: testcase.cpp
	g++ -g -c testcase.cpp

engine.o: engine.cpp
	g++ -g -c engine.cpp

clean:
	rm engine.o testcase.o testcase
