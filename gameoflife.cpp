#include <cstdlib>
#include <string>
#include <array>
#include <iostream>
#include <ctime>

using namespace std;

const int width  = 50;
const int height = 50;
const string deadCell = "░";
const string liveCell = "\033[1;32m█\033[0m"; // green block;

int applyRules(int cell, int count) {
  // Any live cell with fewer than two live neighbours dies, as if by underpopulation.
  // Any live cell with more than three live neighbours dies, as if by overpopulation.
  if (cell == 1 && (count < 2 || count > 3)) return 0;
  // Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
  if (cell == 0 && count == 3) return 1;
  // Any live cell with two or three live neighbours lives on to the next generation.
  return cell;
}

int countNeighbours(const array<array<int, height>, width> &game, int y, int x) {
  // nw n ne
  // w  .  e
  // sw s se
  int nw, n, ne, w, e, sw, s, se;

  n  = (y > 0)                       ? game[y-1][x  ] : 0;
  nw = (y > 0 && x > 0)              ? game[y-1][x-1] : 0;
  ne = (y > 0 && x < width-1)        ? game[y-1][x+1] : 0;
  e  = (x < width-1)                 ? game[y  ][x+1] : 0;
  w  = (x > 0)                       ? game[y  ][x-1] : 0;
  s  = (y < height-1)                ? game[y+1][x  ] : 0;
  sw = (y < height-1 && x > 0)       ? game[y+1][x-1] : 0;
  se = (y < height-1 && x < width-1) ? game[y+1][x+1] : 0;

  return (nw + n + ne + w + e + sw + s + se);
}

int main(int argc, char const *argv[]) {
  // 2 dim arrays (chords [y][x])
  array<array<int, height>, width> game;
  array<array<int, height>, width> generation = game;

  // make rand() random ¯\_(ツ)_/¯
  srand(time(NULL));

  // seed random value between 0-1
  for(int y = 0; y < height; ++y) {
    for(int x = 0; x < width; ++x) {
      int cell = rand() % 2;
      game[y][x] = cell;
    }
  }

  int tick = 0;
  cout << "Press `\033[1;31mEnter\033[0m` for next generation, or `\033[1;31mctrl + c\033[0m` to exit" << endl;
  while(cin.get() == '\n') {
    system("clear");
    int lifeCells = 0;

    // print game board
    for(int y = 0; y < height; ++y) {
      for(int x = 0; x < width; ++x) {
        if (game[y][x] == 1) {
          ++lifeCells;
          cout << liveCell;
        } else {
          cout << deadCell;
        }
      }
      cout << endl;
    }
    // stats
    cout << "Generation:  " << tick      << endl;
    cout << "Cells alive: " << lifeCells << endl;

    // calculate next generation
    for(int y = 0; y < height; ++y) {
      for(int x = 0; x < width; ++x) {
        generation[y][x] = applyRules(game[y][x], countNeighbours(game, y, x));
      }
    }
    game = generation;
    ++tick;
  }
  return 0;
}
