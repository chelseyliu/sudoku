#include <stdio.h>
#include <stdarg.h>  
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>



// TODO - delete this
// int DEFAULT_INPUT[9][9] = {
// 	{6, 2, 4, 5, 3, 9, 1, 8, 7},
// 	{5, 1, 9, 7, 2, 8, 6, 3, 4},
// 	{8, 3, 7, 6, 1, 4, 2, 9, 5},
// 	{1, 4, 3, 8, 6, 5, 7, 2, 9},
// 	{9, 5, 8, 2, 4, 7, 3, 6, 1},
// 	{7, 6, 2, 3, 9, 1, 4, 5, 8},
// 	{3, 7, 1, 9, 5, 6, 8, 4, 2},
// 	{4, 9, 6, 1, 8, 2, 5, 7, 3},
// 	{2, 8, 5, 4, 7, 3, 9, 1, 6}
// };

int DEFAULT_INPUT[9][9] = {
	{6, 2, 4, 5, 3, 9, 1, 8, 7},
	{5, 1, 9, 7, 2, 8, 6, 3, 4},
	{8, 3, 7, 6, 1, 4, 2, 9, 5},
	{1, 4, 3, 8, 6, 5, 7, 2, 9},
	{9, 5, 8, 2, 4, 7, 3, 6, 1},
	{7, 6, 2, 3, 9, 1, 4, 5, 8},
	{3, 7, 1, 9, 5, 6, 8, 4, 2},
	{4, 9, 6, 1, 8, 2, 5, 7, 3},
	{1, 1, 1, 1, 1, 1, 1, 1, 1}
};

// LOG level.
#define DEBUG 0
#define INFO 1
#define ERROR 2
#define FATAL 3
#define NUM_THREADS 27

// ========= Paragram Paramters ========== //
const int LOG_LEVLE = INFO;
// ======================================= //

/* structure for passing data to threads */ 
typedef struct {
  int row;
  int col;
  int d_row;
  int d_col;
  int thread_id;
} parameters;

// TODO - change it to all zeros and fill it by the input file.
// The input data. The program will read the input data and fill
// this 2d array.
int input[9][9] = {{0}};

// Current valid result. totally 27 checks.
int curr_valid = 0;

int thread_valid[27] = {0};

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
void *check_grid(void* data) {
  parameters *params = (parameters*) data;
  if (params->d_row * params->d_col != 9) {
    mylog(ERROR, "Invalid d_row %d and d_col %d", params->d_row, params->d_col);
    return;
  }
  // A array to count freq of each number.
  // seen[i] -> freq of number i + 1
  int seen[9] = {0};

  int row_dest = params->row + params->d_row;
  int col_dest = params->col + params->d_col;
  mylog(DEBUG, "Check dest (%d, %d)", row_dest, col_dest);
  int i;
  int j;
  for (i = params->row; i < row_dest; i++) {
    for (j = params->col; j < col_dest; j++) {
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
  mylog(DEBUG, "Check is %d", valid);
  curr_valid += valid;
  thread_valid[params->thread_id] = valid;
  if (valid == 0) {
    mylog(INFO, "Hint: Found invalid number in input[%d : %d][%d : %d]", params->row, params->row + params->d_row, params->col, params->col + params->d_col);
  }
}

// Trigger row, col and 3x3 grid checking parallely.
int trigger_all_checking_parallel() {
  int row = 0;
  int col = 0;
  
  pthread_t threads[NUM_THREADS];
  int thread_id = 0;

  for (row = 0; row < 9; ++row) {
    for (col = 0; col < 9; ++col) {
      if (row == 0) { // checking col;
        parameters *data = (parameters *) malloc(sizeof(parameters));
        data->row = row;
        data->col = col;
        data->d_row = 9;
        data->d_col = 1;
        data->thread_id = thread_id;
        mylog(DEBUG, "Position (%d, %d) Check col", row, col);
        pthread_create(&threads[thread_id], NULL, check_grid, data);
        thread_id++;
      }
      if (col == 0) { // Checking row;
        parameters *data = (parameters *) malloc(sizeof(parameters));
        data->row = row;
        data->col = col;
        data->d_row = 1;
        data->d_col = 9;
        data->thread_id = thread_id;
        mylog(DEBUG, "Position (%d, %d) Check row", row, col);
        pthread_create(&threads[thread_id], NULL, check_grid, data);
        thread_id++;
      }
      if (row % 3 == 0 && col % 3 == 0) {
        parameters *data = (parameters *) malloc(sizeof(parameters));
        data->row = row;
        data->col = col;
        data->d_row = 3;
        data->d_col = 3;
        data->thread_id = thread_id;
        mylog(DEBUG, "Position (%d, %d) Check 3x3 grid", row, col);
        pthread_create(&threads[thread_id], NULL, check_grid, data);
        thread_id++;
      }
    }
  }
  int i;
  for (i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

  // All threads finished now.
  for (i = 0; i < NUM_THREADS; i++) {
    if (thread_valid[i] == 0) {
      return 0;
    }
  }
  return 1;
}

void fill_input() {
  int i, j;
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      input[i][j] = DEFAULT_INPUT[i][j];
    }
  }
}




int main(void) {

  fill_input();
  int valid = trigger_all_checking_parallel();
  if (valid) {
    mylog(INFO, "Congratulations, the sudoku board is valid!");
  } else {
    mylog(INFO, "The given sudoku board is invalid, please fix by the give hint on above.");
  }
  return 0;
}
