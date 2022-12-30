default:
	g++ src/main.cpp -o output.out src/define/*.cpp -pthread -lsfml-window
	./output.out

debug:
	g++ src/main.cpp -o output.out -g src/define/*.cpp -pthread
	gdb ./output.out

clean:
	rm output.out
