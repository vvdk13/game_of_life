#include <stdio.h>
#include <unistd.h>

#define FIELD_X 80
#define FIELD_Y 25

void load(char field[FIELD_Y][FIELD_X]);
void render_screen(char field[FIELD_Y][FIELD_X]);
void get_old_state(char field[FIELD_Y][FIELD_X], int data[FIELD_Y][FIELD_X]);
int count_neighbors(char field[FIELD_Y][FIELD_X], int y, int x);
void new_state(int data[FIELD_Y][FIELD_X], char field[FIELD_Y][FIELD_X]);

int main() {

  char field[FIELD_Y][FIELD_X];
  int sleep, data[FIELD_Y][FIELD_X];
  load(field);
  freopen("/dev/tty", "r", stdin);
  printf("Введите скорость игры в 0,1секундах : ");
  scanf("%d", &sleep);
  sleep *= 100000;

  while (1) {

    printf("\033[H\033[J");
    render_screen(field);
    get_old_state(field, data);
    new_state(data, field);

    usleep(sleep);
  }

  return 0;
}

void load(char field[FIELD_Y][FIELD_X]) {
  for (int i = 0; i < FIELD_Y; i++) {
    for (int j = 0; j < FIELD_X; j++) {
      char c;
      scanf(" %c", &c);
      if (c == '-') {
        field[i][j] = ' ';
      } else {
        field[i][j] = c;
      }
    }
  }
}
void render_screen(char field[FIELD_Y][FIELD_X]) {
  for (int y = 0; y < FIELD_Y; y++) {
    for (int x = 0; x < FIELD_X; x++) {
      printf("%c", field[y][x]);
    }
    printf("\n");
  }
}

void get_old_state(char field[FIELD_Y][FIELD_X], int data[FIELD_Y][FIELD_X]) {
  for (int y = 0; y < FIELD_Y; y++) {
    for (int x = 0; x < FIELD_X; x++) {
      data[y][x] = count_neighbors(field, y, x);
    }
  }
}

int count_neighbors(char field[FIELD_Y][FIELD_X], int y, int x) {
  int count = 0;

  for (int dy = -1; dy <= 1; dy++) {
    for (int dx = -1; dx <= 1; dx++) {
      if (dy == 0 && dx == 0)
        continue;
      int new_y = y + dy;
      int new_x = x + dx;

      if (new_y < 0) {
        new_y += FIELD_Y;
      } else if (new_y >= FIELD_Y) {
        new_y -= FIELD_Y;
      }

      if (new_x < 0) {
        new_x += FIELD_X;
      } else if (new_x >= FIELD_X) {
        new_x -= FIELD_X;
      }

      if ((new_y >= 0) && (new_y < FIELD_Y) && (new_x >= 0) &&
          (new_x < FIELD_X)) {
        if (field[new_y][new_x] == '*') {
          count++;
        }
      }
    }
  }

  return count;
}

void new_state(int data[FIELD_Y][FIELD_X], char field[FIELD_Y][FIELD_X]) {
  for (int y = 0; y < FIELD_Y; y++) {
    for (int x = 0; x < FIELD_X; x++) {
      if (field[y][x] == ' ' && (data[y][x] == 3)) {
        field[y][x] = '*';
      } else if (field[y][x] == '*' && (data[y][x] == 2 || data[y][x] == 3)) {
        field[y][x] = '*';
      } else if (field[y][x] == '*' && (data[y][x] < 2 || data[y][x] > 3)) {
        field[y][x] = ' ';
      }
    }
  }
}
