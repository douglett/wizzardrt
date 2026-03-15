main.out: main.cpp
	g++ -Wall -std=c++17 -o main.out main.cpp

run: main.out
	./main.out
	
clean:
	rm -rfv *.out
