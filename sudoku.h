void load_board(const char* filename, char board[9][9]);
void display_board(const char board[9][9]);
bool is_complete(const char board[9][9]);
bool make_move(const char position[], const char digit, char board[9][9]);
bool save_board(const char* filename, const char board[9][9]);
bool solve_board(char board[9][9]);
bool has_repeat_in_row(const int row, const char digit, const char board[9][9]);
bool has_repeat_in_col(const int col, const char digit, const char board[9][9]);
bool has_repeat_in_nonet(const int row, const int col, char digit, const char board[9][9]);
void get_nonet_range(int position, int& min, int& max);
void get_nonet_values(int row, int col, const char board[9][9], char nonet_values[9]);
bool solve_by_mode(char board[9][9], int num_of_trials);
bool solve_by_row(char board[9][9]);
bool solve_by_col(char board[9][9]);
bool solve_by_nonet(char board[9][9]);
