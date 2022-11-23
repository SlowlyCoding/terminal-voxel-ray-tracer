default:
	g++ src/main.cpp -o output.out src/define/*.cpp -std=c++11 -pthread 
	./output.out

debug:
	g++ src/main.cpp -o output.out -g src/define/*.cpp -std=c++11 -pthread 
	gdb ./output.out

clean:
	rm output.out
