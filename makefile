sudoku: sudoku.o main.o
	g++ -Wall -g sudoku.o main.o -o sudoku

main.o: main.cpp sudoku.h
	g++ -c main.cpp sudoku.h 

sudoku.o: sudoku.cpp sudoku.h
	g++ -c sudoku.cpp sudoku.h 