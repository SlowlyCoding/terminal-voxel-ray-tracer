default:
	g++ src/main.cpp -o output.out src/define/*.cpp -pthread -lsfml-window -std=c++20
	./output.out

build:
	g++ src/main.cpp -o output.out src/define/*.cpp -pthread -lsfml-window -std=c++20

run:
	./output.out

debug:
	g++ src/main.cpp -o output.out -g src/define/*.cpp -pthread -lsfml-window -std=c++20
	gdb ./output.out

clean:
	rm output.out
