void load_board(const char* filename, char board[9][9]);
void display_board(const char board[9][9]);
bool is_complete(const char board[9][9]);
bool make_move(const char position[], const char digit, char board[9][9]);
bool save_board(const char* filename, const char board[9][9]);
bool is_in_nonet(int row, int col, char digit, const char board[9][9]);
void get_nonet_range(int position, int& min, int& max);
void get_nonet_values(int row, int col, const char board[9][9], char nonet_values[9]);
