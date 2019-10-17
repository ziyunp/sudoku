sudoku: sudoku.o main.o
	g++ sudoku.o main.o -o sudoku

main.o: main.cpp sudoku.h
	g++ -Wall -g -c main.cpp

sudoku.o: sudoku.cpp sudoku.h
	g++ -Wall -g -c sudoku.cpp  
