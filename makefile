default:
	g++ src/main.cpp -o output.out src/define/*.cpp -pthread 
	./output.out

debug:
	g++ src/main.cpp -o output.out -g src/define/*.cpp -pthread 
	gdb ./output.out

clean:
	rm output.out
