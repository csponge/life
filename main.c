#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ROWS 10
#define COLS 20

#define clear() printf("\033[H\033[J")

int main() {
  char back[ROWS][COLS] = {' '};

  // generate seed
  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      int isAlive = rand() % (1 + 1);

      if (isAlive == 1) {
        back[row][col] = 'O';
      } else {
        back[row][col] = ' ';
      }
    }
  }

  while (1) {
    for (int row = 0; row < ROWS; row++) {
      for (int col = 0; col < COLS; col++) {
        int live_neighbors = 8;

        // top
        char topl = row > 0 && col > 0 ? back[row - 1][col - 1] : ' ';
        if (topl == ' ')
          live_neighbors--;

        char top = row > 0 ? back[row - 1][col] : ' ';
	if (top == ' ')
          live_neighbors--;

        char topr = row > 0 && col < (COLS - 1) ? back[row - 1][col + 1] : ' ';
	if (topr == ' ')
          live_neighbors--;

        // middle
        char left = col > 0 ? back[row][col - 1] : ' ';
	if (left == ' ')
          live_neighbors--;
	
        char right = col < (COLS - 1) ? back[row][col + 1] : ' ';
	if (right == ' ')
          live_neighbors--;

        // bottom
        char botl = row < (ROWS - 1) && col > 0 ? back[row + 1][col - 1] : ' ';
	if (botl == ' ')
          live_neighbors--;
       
        char bot = row < (ROWS - 1) ? back[row + 1][col] : ' ';
	if (bot == ' ')
          live_neighbors--;
	
        char botr =
            row < (ROWS - 1) && col < (COLS - 1) ? back[row + 1][col + 1] : ' ';
	if (botr == ' ')
          live_neighbors--;
	
        // handle dead cells
        if (back[row][col] == ' ') {
          if (live_neighbors == 3) {
            back[row][col] = 'O';
          }
          continue;
        }

        // handle alive cells
        if (live_neighbors < 2 || live_neighbors > 3) {
          back[row][col] = ' ';
        }
      }
    }

    for (int row = 0; row < ROWS; row++) {
      for (int col = 0; col < COLS; col++) {
        printf("%c", back[row][col]);
      }
      printf("\n");
    }

    sleep(1);
    clear();
  }
}
