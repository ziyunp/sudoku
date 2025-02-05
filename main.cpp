#include <iostream>
#include <cstdio>
#include <ctime>
#include "sudoku.h"

using namespace std;

int main() {

  char board[9][9];
  
  /* This section illustrates the use of the pre-supplied helper functions. */
  cout << "============= Pre-supplied functions =============" << "\n\n";

  cout << "Calling load_board():" << '\n';
  load_board("easy.dat", board);

  cout << '\n' << "Displaying Sudoku board with display_board():" << '\n';
  display_board(board);
  cout << "Done!" << "\n\n";
 
  cout << "=================== Question 1 ===================" << "\n\n";

  load_board("easy.dat", board);
  cout << "Board is ";
  if (!is_complete(board))
    cout << "NOT ";
  cout << "complete." << "\n\n";

  load_board("easy-solution.dat", board);
  cout << "Board is ";
  if (!is_complete(board))
    cout << "NOT ";
  cout << "complete." << "\n\n";

  cout << "=================== Question 2 ===================" << "\n\n";

  load_board("easy.dat", board);

  // Should be OK
  cout << "Putting '1' into I8 is ";
  if (!make_move("I8", '1', board)) 
    cout << "NOT ";
  cout << "a valid move. The board is:" << '\n';
  display_board(board);

  // write more tests

  cout << "=================== Question 3 ===================" << "\n\n";

  load_board("easy.dat", board);
  if (save_board("easy-copy.dat", board))
    cout << "Save board to 'easy-copy.dat' successful." << '\n';
  else
    cout << "Save board failed." << '\n';
  cout << '\n';

  cout << "=================== Question 4 ===================" << "\n\n";
    
    load_board("easy.dat", board);
    if (solve_board(board)) {
      cout << "The 'easy' board has a solution:" << '\n';
      display_board(board);
    } else 
      cout << "A solution cannot be found." << '\n';
    cout << '\n';
   
  load_board("medium.dat", board);
  if (solve_board(board)) {
    cout << "The 'medium' board has a solution:" << '\n';
    display_board(board);
  } else 
    cout << "A solution cannot be found." << '\n';
  cout << '\n';
 
  // write more tests
  cout << "=================== Question 5 ===================" << "\n\n";
   
  // write more tests
  
  // double sum = 0;
  // int n=0;
  // do { 
  // clock_t begin_1 = clock();  
  load_board("mystery1.dat", board);
  if (solve_board(board)) {
  cout << "The 'mystery1' board has a solution:" << '\n';
  display_board(board);
  } else 
  cout << "A solution cannot be found." << '\n';
  cout << '\n';
  // clock_t end_1 = clock();
  // double elapsed_secs_1 = double(end_1-begin_1)/CLOCKS_PER_SEC;
  // n++;
  // cout << "used time " << elapsed_secs_1 *1000 << " ms" << endl; 
  // sum += elapsed_secs_1 * 1000;
  // } while(n < 10);
  // cout << "sum: " << sum/10 << endl;
 
  load_board("mystery2.dat", board);
  if (solve_board(board)) {
  cout << "The 'mystery2' board has a solution:" << '\n';
  display_board(board);
  } else 
  cout << "A solution cannot be found." << '\n';
  cout << '\n';
  
  
  // clock_t begin_3 = clock();
  load_board("mystery3.dat", board);
  if (solve_board(board)) {
  cout << "The 'mystery3' board has a solution:" << '\n';
  display_board(board);
  } else 
  cout << "A solution cannot be found." << '\n';
  cout << '\n';
  
  // clock_t end_3 = clock();
  // double elapsed_secs_3 = double(end_3-begin_3)/CLOCKS_PER_SEC;
  // cout << "used time " << elapsed_secs_3 *1000 << " ms" << endl; 
  
  return 0;
}
