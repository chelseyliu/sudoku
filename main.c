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
int LOG_LEVLE = DEBUG;
// ======================================= //

// TODO - change it to all zeros and fill it by the input file.
// The input data. The program will read the input data and fill
// this 2d array.
int input[9][9] = {{0}};


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
  mylog(DEBUG, "%d", input[0][0]);
  return 0;
}



