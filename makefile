all:
	g++ src/*.cpp -o output.out -pthread -lsfml-window -std=c++14
	./output.out

build:
	g++ src/*.cpp -o output.out -pthread -lsfml-window -std=c++14

run:
	./output.out

debug:
	g++ src/*.cpp -g -o output.out -pthread -lsfml-window -std=c++14
	gdb ./output.out

clean:
	rm output.out
