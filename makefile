sudoku: sudoku.o main.o
	g++ sudoku.o main.o -o sudoku

main.o: main.cpp sudoku.h
	g++ -Wall -g main.cpp sudoku.h 

sudoku.o: sudoku.cpp sudoku.h
	g++ -Wall -g sudoku.cpp sudoku.h 
