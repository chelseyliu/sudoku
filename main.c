#include <stdio.h>
#include <stdarg.h>                                                             

// TODO - delete this
int DEFAULT_INPUT[9][9] = {
	{6, 2, 4, 5, 3, 9, 1, 8, 7},
	{5, 1, 9, 7, 2, 8, 6, 3, 4},
	{8, 3, 7, 6, 1, 4, 2, 9, 5},
	{1, 4, 3, 8, 6, 5, 7, 2, 9},
	{9, 5, 8, 2, 4, 7, 3, 6, 1},
	{7, 6, 2, 3, 9, 1, 4, 5, 8},
	{3, 7, 1, 9, 5, 6, 8, 4, 2},
	{4, 9, 6, 1, 8, 2, 5, 7, 3},
	{2, 8, 5, 4, 7, 3, 9, 1, 6}
};

// LOG level.
#define DEBUG 0
#define INFO 1
#define ERROR 2
#define FATAL 3

// ========= Paragram Paramters ========== //
int LOG_LEVLE = INFO;
// ======================================= //

// TODO - change it to all zeros and fill it by the input file.
// The input data. The program will read the input data and fill
// this 2d array.
int input[9][9] = {{0}};

// Current valid result. totally 27 checks.
int curr_valid = 0;

// Custom Log function. 
// Only log the message whose log level >= LOG_LEVEL.
void mylog(int level, const char* fmt, ...) {
  if (level >= LOG_LEVLE) {
    va_list arg_ptr;
    va_start(arg_ptr, fmt);                                                      
    vprintf(fmt, arg_ptr);
    printf("\n");                                                 
    va_end(arg_ptr); 
  }
}

// Check a grid of input[row : row + d_dow][col : col + d_col]
// d_row * d_col should equal to 9.
void check_grid(int row, int col, int d_row, int d_col) {
  if (d_row * d_col != 9) {
    mylog(ERROR, "Invalid d_row %d and d_col %d", d_row, d_col);
    return;
  }
  // A array to count freq of each number.
  // seen[i] -> freq of number i + 1
  int seen[9] = {0};

  int row_dest = row + d_row;
  int col_dest = col + d_col;
  mylog(ERROR, "Check dest (%d, %d)", row_dest, col_dest);
  int i;
  int j;
  for (i = row; i < row_dest; i++) {
    for (j = col; j < col_dest; j++) {
      if (i < 0 || i >= 9) {
        mylog(ERROR, "Invalid row index %d", i);
        return;
      };
      if (j < 0 || j >= 9) {
        mylog(ERROR, "Invalid col index %d", j);
        return;
      };
      seen[input[i][j] - 1]++;
    }
  }
  int valid = 1;
  for (i = 0; i < 9; ++i) {
    if (seen[i] != 1) {
      valid = 0;
    }
  }
  // TODO - record valid.
  mylog(INFO, "Check is %d", valid);
  curr_valid += valid;
}

// Trigger row, col and 3x3 grid checking.
void trigger_all_checking() {
  int row = 0;
  int col = 0;
  for (row = 0; row < 9; ++row) {
    for (col = 0; col < 9; ++col) {
      if (row == 0) { // checking col;
        mylog(DEBUG, "Position (%d, %d) Check col", row, col);
        check_grid(row, col, 9, 1);
      }
      if (col == 0) { // Checking row;
        mylog(DEBUG, "Position (%d, %d) Check row", row, col);
        check_grid(row, col, 1, 9);
      }
      if (row % 3 == 0 && col % 3 == 0) {
        mylog(DEBUG, "Position (%d, %d) Check 3x3 grid", row, col);
        check_grid(row, col, 3, 3);
      }
    }
  }
}

void fill_input() {
  mylog(DEBUG, "Filling input.");
  int i, j;
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      input[i][j] = DEFAULT_INPUT[i][j];
    }
  }
  mylog(DEBUG, "Fill input done.");
}

/* structure for passing data to threads */ 
typedef struct {
  int row;
  int column; 
} parameters;



int main(void) {
  fill_input();
  trigger_all_checking();
  mylog(INFO, "Total valid: %d", curr_valid);
  return 0;
}



