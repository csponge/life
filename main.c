#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ROWS 25
#define COLS 50

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
      int isAlive = rand() % (1 + 1);

      if (isAlive == 1) {
        seed[row][col] = 'O';
      } else {
        seed[row][col] = ' ';
      }
    }
  }

  return seed;
}

void evaluate(char **grid) {
  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      int live_neighbors = 8;

      // top
      char topl = row > 0 && col > 0 ? grid[row - 1][col - 1] : ' ';
      if (topl == ' ')
        live_neighbors--;

      char top = row > 0 ? grid[row - 1][col] : ' ';
      if (top == ' ')
        live_neighbors--;

      char topr = row > 0 && col < (COLS - 1) ? grid[row - 1][col + 1] : ' ';
      if (topr == ' ')
        live_neighbors--;

      // middle
      char left = col > 0 ? grid[row][col - 1] : ' ';
      if (left == ' ')
        live_neighbors--;

      char right = col < (COLS - 1) ? grid[row][col + 1] : ' ';
      if (right == ' ')
        live_neighbors--;

      // bottom
      char botl = row < (ROWS - 1) && col > 0 ? grid[row + 1][col - 1] : ' ';
      if (botl == ' ')
        live_neighbors--;

      char bot = row < (ROWS - 1) ? grid[row + 1][col] : ' ';
      if (bot == ' ')
        live_neighbors--;

      char botr =
          row < (ROWS - 1) && col < (COLS - 1) ? grid[row + 1][col + 1] : ' ';
      if (botr == ' ')
        live_neighbors--;

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
