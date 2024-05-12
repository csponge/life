#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ROWS 10
#define COLS 20

#define clear() printf("\033[H\033[J")

void display(char **grid) {
  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      printf("%c", grid[row][col]);
    }
    printf("\n");
  }
}

char **generate_seed(int rows, int cols) {
  char **seed = calloc(rows, sizeof(char *));
  for (int row = 0; row < rows; row++) {
    seed[row] = calloc(cols, sizeof(char));
  }

  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      int isAlive = rand() % (10 + 1);

      if (isAlive <= 3) {
        seed[row][col] = 'O';
      } else {
        seed[row][col] = ' ';
      }
    }
  }

  return seed;
}

int count_live_neighbors(char **grid, int row, int col) {
  int live_neighbors = 8;

  // top
  if ((row > 0 && col > 0 ? grid[row - 1][col - 1] : ' ') == ' ')
    live_neighbors--;

  if ((row > 0 ? grid[row - 1][col] : ' ') == ' ')
    live_neighbors--;

  if ((row > 0 && col < (COLS - 1) ? grid[row - 1][col + 1] : ' ') == ' ')
    live_neighbors--;

  // middle
  if ((col > 0 ? grid[row][col - 1] : ' ') == ' ')
    live_neighbors--;

  if ((col < (COLS - 1) ? grid[row][col + 1] : ' ') == ' ')
    live_neighbors--;

  // bottom
  if ((row < (ROWS - 1) && col > 0 ? grid[row + 1][col - 1] : ' ') == ' ')
    live_neighbors--;

  if ((row < (ROWS - 1) ? grid[row + 1][col] : ' ') == ' ')
    live_neighbors--;

  if ((row < (ROWS - 1) && col < (COLS - 1) ? grid[row + 1][col + 1] : ' ') ==
      ' ')
    live_neighbors--;

  return live_neighbors;
}

void evaluate(char **grid) {
  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      int live_neighbors = count_live_neighbors(grid, row, col);
      
      // handle dead cells
      if (grid[row][col] == ' ') {
        if (live_neighbors == 3) {
          grid[row][col] = 'O';
        }
        continue;
      }

      // handle alive cells
      if (live_neighbors < 2 || live_neighbors > 3) {
        grid[row][col] = ' ';
      }
    }
  }
}

int main() {
  // generate seed
  char **seed = generate_seed(ROWS, COLS);

  // main loop
  while (1) {
    evaluate(seed);
    display(seed);
    sleep(1);
    clear();
  }

  // cleanup
  for (int row = 0; row < ROWS; row++) {
    free(seed[row]);
  }
  free(seed);

  return 0;
}
