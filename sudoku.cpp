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
      if (board[row][col] == '.' || board[row][col] < '1' || board[row][col] > '9') return false;
    }
  } 
  return true;
}

bool make_move(const char position[], const char digit, char board[9][9]) {
  int row, col;
  row = position[0] - 'A';
  col = position[1] - '1';
  
  // coordinates out of range
  if(row < 0 || row > 8 || col < 0 || col > 8)
    return false;

  if(digit < '1' || digit > '9')
    return false;
  
  if(has_repeat_in_row(row, digit, board) || has_repeat_in_col(col, digit, board) || has_repeat_in_nonet(row, col, digit, board))
    return false;

  // input to board
  board[row][col] = digit;
  return true;
}

bool save_board(const char* filename, const char board[9][9]) {

  cout << "Saving Sudoku board to file '" << filename << "'... ";

  ofstream out(filename);
  if (!out) { 
    cout << "Failed!" << '\n';
    return false;
  }
  assert(out);

  int row = 0;
  while (out && row < 9) {
    for (int n=0; n<9; n++) {
      assert(board[row][n] == '.' || isdigit(board[row][n]));
      out.put(board[row][n]);
    }
    out << endl;
    row++;    
  }

  assert(row == 9);
  if (row == 9) {
    cout << "Success!\n";
    return true;
  } else {
    cout << "Failed!\n";
    return false;
  }
}

bool solve_board(char board[9][9], int total_blanks) {
  static int num_of_trials = 0, retry_guess_count = 0;
  num_of_trials++;
  // make a copy of original board
  if(num_of_trials == 1) {
    remove("board-copy.dat");
    save_board("board-copy.dat", board);
  }
  
  int initial_total_blanks = total_blanks;
  // solve by nonet / row / col
  int final_total_blanks = solve_by_mode(board, num_of_trials);

   // check progress
  bool no_progress = false;
  static int no_progress_count = 0;
  if (final_total_blanks >= initial_total_blanks)
    no_progress_count++;
  else {
   reset_no_progress(no_progress, no_progress_count);
  }

  // threshold to identify the need of making a guess 
  // around 2 rounds of row -> col -> nonet
  if (no_progress_count > 5) {
    num_of_trials = 0;
    no_progress = true;
  }

  if (is_complete(board)) {
    num_of_trials = 0;
    retry_guess_count = 0;
    return true;
  }
  else if(!no_progress) {
    return solve_board(board, final_total_blanks);
  }
  else {
    // make a guess when no progress
    if(make_a_guess(board, final_total_blanks)) {
      reset_no_progress(no_progress, no_progress_count);
      final_total_blanks--;
      return solve_board(board, final_total_blanks);
    }
    else if(retry_guess_count < 5) {
      // when making a guess fails to solve, retry guessing on the next blank 
      retry_guess_count++;
      if(make_a_guess(board, final_total_blanks, retry_guess_count)) {
        reset_no_progress(no_progress, no_progress_count);
        final_total_blanks--;
        return solve_board(board, final_total_blanks);
      }
    }
  } 
  // if fails, reset board and static variables
  load_board("board-copy.dat", board);
  num_of_trials = 0;
  retry_guess_count = 0;
  return false;
}

/* other helper functions */

// function to check data of row
bool has_repeat_in_row(const int row, const char digit, const char board[9][9]) {
 for(int c=0; c<9; c++) {
    if(digit == board[row][c])
      return true;
  }
 return false;
}

// function to check data of col
bool has_repeat_in_col(const int col, const char digit, const char board[9][9]) {
  for(int r=0; r<9; r++) {
    if(digit == board[r][col])
      return true;
  }
  return false;
}

// function to check data of nonet
bool has_repeat_in_nonet(const int row, const int col, const char digit, const char board[9][9]) {

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

int solve_by_mode(char board[9][9], int num_of_trials) {
  if (is_complete(board))
    return 0;
  
  int total_blanks;
  // alternate among nonet, row and col on each trial
  if (!(num_of_trials % 3))
    total_blanks = solve_by_nonet(board);


  else if(!(num_of_trials % 2))
    total_blanks = solve_by_row(board);
  
  else total_blanks = solve_by_col(board);
  return total_blanks;
}


int solve_by_row(char board[9][9]) {

  int total_blank_count = 0;
  int curr_row = 0;
  while (curr_row < 9) {
    int blank_count = 0;
    char remaining_values[9] = {};
    
    // 1. get remaining values and blank count in the row
    //int n=0;
    for(char num='1'; num<='9'; num++) {
      if (!has_repeat_in_row(curr_row, num, board))
	remaining_values[blank_count++] = num;
    }
    total_blank_count += blank_count;

    // 2. get no. of possible positions for each value
    for(int count=0; count < blank_count; count++) {
      char curr_digit = remaining_values[count];
      
      int num_of_possible_positions = 0;
      for(int col=0; col<9; col++) {
	if(board[curr_row][col] == '.') {
	  if(!has_repeat_in_col(col, curr_digit, board) && !has_repeat_in_nonet(curr_row, col, curr_digit, board)) {
	    num_of_possible_positions++;
	    if(num_of_possible_positions > 1) break;
	  }
	}
      }
  
      // 3. if only 1 possible position, make move
      if(num_of_possible_positions == 1) {
	for(int c=0; c<9; c++) {
	  if(board[curr_row][c] == '.') {
	    char position[2];
	    get_position(curr_row, c, position);
	    if(make_move(position, curr_digit, board)) 
	      break;
	  }
	}	  
      }
    }
    curr_row++;
  }
 
  return total_blank_count;
}


int solve_by_col(char board[9][9]) {

  int total_blank_count = 0;
  
  int curr_col = 0;
  while (curr_col < 9) {
    int blank_count = 0;
    char remaining_values[9] = {};
    // 1. get remaining values and blank count in the col
    //int n=0;
    for(char num='1'; num<='9'; num++) {
      if(!has_repeat_in_col(curr_col, num, board))
      	remaining_values[blank_count++] = num;
    }

    total_blank_count += blank_count;

    // 2. get no. of possible positions for each value
    for(int count=0; count < blank_count; count++) {
      char curr_digit = remaining_values[count];

      int num_of_possible_positions = 0;
      for(int row=0; row<9; row++) {
	if(board[row][curr_col] == '.') {
	  if(!has_repeat_in_row(row, curr_digit, board) && !has_repeat_in_nonet(row, curr_col, curr_digit, board)) {
	    num_of_possible_positions++;
	    if(num_of_possible_positions > 1) break;
	  }
	}
      }
  
      // 3. if only 1 possible position, make move
      if(num_of_possible_positions == 1) {
	for(int r=0; r<9; r++) {
	  if(board[r][curr_col] == '.') {
	    char position[2];
	    get_position(r, curr_col, position);
	    if(make_move(position, curr_digit, board)) 
	      break;
	  }
	}	  
      }
    }
    curr_col++;
  }

  return total_blank_count;
}


int solve_by_nonet(char board[9][9]) {

  int total_blank_count = 0;
  
  int start_row = 0;
  while (start_row <= 6) {
    int start_col = 0;
    while (start_col <=6) {
      int max_row = start_row + 2;
      int max_col = start_col + 2;
      int blank_count = 0;
      char remaining_values[9] = {};
      // 1. get remaining values and blank count in the row
      // get existing values in current nonet

      // get remaining values in current nonet
      
      for(char num='1'; num<='9'; num++) {
	      if(!has_repeat_in_nonet(start_row, start_col, num, board))
	        remaining_values[blank_count++] = num;
        }
      
      total_blank_count += blank_count;

      // 2. get no. of possible positions for each value
      for(int count=0; count < blank_count; count++) {
	char curr_digit = remaining_values[count];

	int num_of_possible_positions = 0;
	int r = start_row;
	while(r <= max_row) {
	  int c = start_col;
	  while(c <= max_col) {
	    if (board[r][c] == '.') {
	      if(!has_repeat_in_row(r, curr_digit, board) && !has_repeat_in_col(c, curr_digit, board)) {
		num_of_possible_positions++;
		if(num_of_possible_positions > 1) break;
	      }
	    }
	    c++;
	  }
	  r++;
	}

	// 3. if only 1 possible position, make move
	if(num_of_possible_positions == 1) {
	  int row = start_row;
	  while(row <= max_row) {
	    int col = start_col;
	    while(col <= max_col) {
	      if (board[row][col] == '.') {
		char position[2];
		get_position(row, col, position);
		if(make_move(position, curr_digit, board)) 
		  break;
	      }
	      col++;
	    }
	    row++;
	  }
	}
     
      }
      start_col += 3;
    }
    start_row += 3;
  }

  return total_blank_count;
}

bool make_a_guess(char board[9][9], int final_total_blanks, int retry_guess_count) {
  // double check static int **
  static int guess_count = 0, call = 0;
  static int row = 0, col = 0, max_guess = 0;
  if(retry_guess_count >=1) {
    col = retry_guess_count;
    load_board("board-without-guess.dat", board);    
    call = 0;
  }
  char possible_values[9];
  int index = 0;
  static int final_blanks_array[9];
  
  if(call == 0) {
    remove("board-without-guess.dat");
    save_board("board-without-guess.dat", board);
  }
    call++;

  if(guess_count == 0) {
    // save filled board
    remove("filled-board-copy.dat");
    save_board("filled-board-copy.dat", board);
    // find first blank
    for (; row<9; row++) {
      for (; col<9; col++) {
	      if (board[row][col] == '.')
	        break;
      }
      // break the loop if found a blank
      if(col != 9) break;
    }
    // use guess count as index, i.e. first guess use first value etc
    index = guess_count;

  } else if (guess_count >= 1 && guess_count < max_guess) {
    // save previous blanks left
    final_blanks_array[guess_count - 1] = final_total_blanks;

    // reset filled board
    load_board("filled-board-copy.dat", board);
    index = guess_count;

  } else if (guess_count == max_guess) {
    // save blanks 
    final_blanks_array[guess_count - 1] = final_total_blanks;
    // reset board
    load_board("filled-board-copy.dat", board);
    // loop through final_blanks, take min value's index
    int min_final_blanks = final_blanks_array[0];
    for(int i=0; i<max_guess; i++) {
      if(final_blanks_array[i] < min_final_blanks) {
	      min_final_blanks = final_blanks_array[i];
	      index = i;
      }
    }
  } else if (guess_count > max_guess) {
    // reset static int 
    guess_count = 0;

    // reset static array
    for(int i=0; i<max_guess; i++) {
      if(final_blanks_array[i])
	      final_blanks_array[i] = 0;
    }
    max_guess = 0;
    // reset prev blank
    board[row][col] = '.';
    // try again on the current blank
    if(final_total_blanks < 5) 
      return make_a_guess(board, final_total_blanks);
    else {
      call = 0;
      // guess next blank 
      col++;
      return false;
    }
      
  }
  // increment count for next call
  guess_count++;
  // identify max guess / number of possible values in that blank
  int n = 0;
  for (char num='1'; num<='9'; num++) {
    if(!has_repeat_in_row(row, num, board) && !has_repeat_in_col(col, num, board) && !has_repeat_in_nonet(row, col, num, board)){
      possible_values[n++] = num;
    }
  }
  max_guess = n;

  if (max_guess < 1) {
    // reset static array
    for(int i=0; i<max_guess; i++) {
      if(final_blanks_array[i])
        final_blanks_array[i] = 0;
    }
    // reset static int 
    max_guess = 0;
    call = 0;
    guess_count = 0;      
    return false;
  }

  // make move with possible values
  char position[2];
  get_position(row, col, position);
  char digit = possible_values[index];

  // return true after make move successful
  if(make_move(position, digit, board))
    return true;
  else {
    // reset static int 
    guess_count = 0;
    // reset static array
    for(int i=0; i<max_guess; i++) {
      if(final_blanks_array[i])
	      final_blanks_array[i] = 0;
    }
    max_guess = 0;
    call = 0;
    return false;
  }
}

void reset_no_progress(bool& no_progress, int& no_progress_count) {
  no_progress_count = 0;
  no_progress = false;
}

void get_position(int row, int col, char position[2]) {
  position[0] = row + 'A';
  position[1] = col + '1';
}
