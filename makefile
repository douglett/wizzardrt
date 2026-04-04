HEAD = src/*.hpp
OUT = main.exe

$(OUT): main.cpp $(HEAD)
	g++ -Wall -std=c++17 -o $(OUT) main.cpp

all: $(OUT)

run: $(OUT)
	./$(OUT)
	
clean:
	rm -rfv *.out *.exe
