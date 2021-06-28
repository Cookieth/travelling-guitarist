CC = clang++

main: main.cpp chord.cpp chordprogression.cpp
	$(CC) -std=c++11 -o main main.cpp chord.cpp chordprogression.cpp
	./main