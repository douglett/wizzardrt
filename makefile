HEAD = src/*.hpp

main.out: main.cpp $(HEAD)
	g++ -Wall -std=c++17 -o main.out main.cpp

run: main.out
	./main.out
	
clean:
	rm -rfv *.out
