#include <SFML/Graphics.hpp>
#include <time.h>

using namespace sf;

const int ROWS = 20;
const int COLS = 10;

int field[ROWS][COLS] = {0};

// The a point array will hold the current positios of four blocks that make up a tetromino
// The b point array will be used as a temporary storage when translating or rotating the tetromino
struct Point {
    int x, y;
};
Point a[4], b[4]; 

// Defining the tetrominoes
// Each tetromino is represented by a 4x4 grid, where 1 indicates the presence of a block
int figures[7][4] = {
    1, 3, 5, 7, // I: a straight line block
    2, 4, 5, 7, // Z: a zig-zag shape block
    3, 5, 4, 6, // S: a mirrored zig-zag shape
    3, 5, 4, 7, // T: a T shape block
    2, 3, 5, 7, // L: an L shape block
    3, 5, 7, 6, // J: a mirrored L shape block
    2, 3, 4, 5  // O: a square block
};

