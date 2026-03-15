HEAD = src/wizzardrt.hpp src/1.show.hpp src/2.validate.hpp src/3.runtime.hpp

main.out: main.cpp $(HEAD)
	g++ -Wall -std=c++17 -o main.out main.cpp

run: main.out
	./main.out
	
clean:
	rm -rfv *.out
