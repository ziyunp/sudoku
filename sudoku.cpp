#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cassert>
#include "sudoku.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* pre-supplied function to load a Sudoku board from a file */
void load_board(const char* filename, char board[9][9]) {

  cout << "Loading Sudoku board from file '" << filename << "'... ";

  ifstream in(filename);
  if (!in)
    cout << "Failed!" << '\n';
  assert(in);

  char buffer[512];

  int row = 0;
  in.getline(buffer,512);
  while (in && row < 9) {
    for (int n=0; n<9; n++) {
      assert(buffer[n] == '.' || isdigit(buffer[n]));
      board[row][n] = buffer[n];
    }
    row++;
    in.getline(buffer,512);
  }

  cout << ((row == 9) ? "Success!" : "Failed!") << '\n';
  assert(row == 9);
}

/* internal helper function */
void print_frame(int row) {
  if (!(row % 3))
    cout << "  +===========+===========+===========+" << '\n';
  else
    cout << "  +---+---+---+---+---+---+---+---+---+" << '\n';
}

/* internal helper function */
void print_row(const char* data, int row) {
  cout << (char) ('A' + row) << " ";
  for (int i=0; i<9; i++) {
    cout << ( (i % 3) ? ':' : '|' ) << " ";
    cout << ( (data[i]=='.') ? ' ' : data[i]) << " ";
  }
  cout << "|" << '\n';
}

/* pre-supplied function to display a Sudoku board */
void display_board(const char board[9][9]) {
  cout << "    ";
  for (int r=0; r<9; r++) 
    cout << (char) ('1'+r) << "   ";
  cout << '\n';
  for (int r=0; r<9; r++) {
    print_frame(r);
    print_row(board[r],r);
  }
  print_frame(9);
}

/* add your functions here */
bool is_complete(const char board[9][9]) {
  for (int row=0; row<9; row++) {
    for (int col=0; col<9; col++) {
      if (board[row][col] == '.') return false;
    }
  } 
  return true;
}

bool make_move(const char position[], const char digit, const char board[9][9]) {
  int row, col, add_to_row, add_to_col;
  // extract row from position (A-I)
  row = position[0] - 'A';
 
  // extract column from position (1-9)
  col = position[1] - '1';
  
  // coordinates out of range
  if(row < 0 || row > 8 || col < 0 || col > 8)
    return false;

  // digit out of range
  if(digit < '1' || digit > '9')
    return false;
  
  // check data of the row
  for(int c=0; c<9; c++) {
    if(digit == board[row][c])
      return false;
  }
  // check data of the column
  for(int r=0; r<9; r++) {
    if(digit == board[r][col])
      return false;
  } 
  cout << "Entering nonet: \n";

  // check nonet
  if(is_in_nonet(row, col, digit, board)) 
    return false;

  return true;
}

/* bool save_board(const char* filename, const char board[9][9]) {

  cout << "Loading Sudoku board from file '" << filename << "'... ";

  ofstream out(filename);
  if (!out)
    cout << "Failed!" << '\n';
  assert(out);

  int row = 0;
  while (out && row < 9) {
    for (int n=0; n<9; n++) {
      assert(board[row][n] == '.' || isdigit(board[row][n]));
      out.put(board[row][n]);
    }
    row++;
    
  }

  cout << ((row == 9) ? "Success!" : "Failed!") << '\n';
  assert(row == 9);

}
*/
// function to check data of nonet
bool is_in_nonet(int row, int col, char digit, const char board[9][9]) {

  char nonet_values[9];
  
  get_nonet_values(row, col, board, nonet_values);
  
  for(int i=0; i<9; i++) {
    if (nonet_values[i] == digit) 
      return true;
  }
  return false;
}

void get_nonet_range(int position, int& min, int& max) {
  int add_to_position;

  for (int i=0; i<3; i++) {
    if (!((position + i) % 3)) add_to_position = i;
  }

  if (add_to_position == 0) {
    min = position;
    max = position + 2;
  }

  if (add_to_position == 1) {
    min = position - 2;
    max = position;
  }

  if (add_to_position == 2) {
    min = position - 1;
    max = position + 1;
  }
}

void get_nonet_values(int row, int col, const char board[9][9], char nonet_values[9]) {
  int min_row=0, max_row=0, min_col=0, max_col=0;
  
  get_nonet_range(row, min_row, max_row);
  get_nonet_range(col, min_col, max_col);

  int i=0, r = min_row;
  while(i<9) {
    while(r <= max_row) {
      int c = min_col;
      while(c <= max_col) {
	nonet_values[i++] = board[r][c];
	c++;
      }
      r++;
    }
  }
}
