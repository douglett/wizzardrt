HEAD = src/*.hpp
OUT = main.exe

$(OUT): main.cpp $(HEAD)
	g++ -std=c++17 -Wall -gdwarf -o $(OUT) main.cpp

all: $(OUT)

run: $(OUT)
	./$(OUT)
	
clean:
	rm -rfv *.out *.exe
