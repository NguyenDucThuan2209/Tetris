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

// Check if the tetromino can be placed in the field
bool checkValidPosition() {
    for (int i = 0; i < 4; i++)
    {
        int row = a[i].y;
        int column = a[i].x;
        
        // Check if the block is within the field boundaries
        if (column < 0 || column >= COLS || row >= ROWS)
            return false;

        // Check if the block is colliding with existing blocks in the field
        if (field[row][column] != 0)
            return false;
    }

    return true;
}

int main()
{
    srand(time(0)); // Seed the random number generator

    // Create a window with 320x480 resolution
    RenderWindow window(VideoMode(320, 480), "Tetris");

    // Load textures from images folder
    Texture t1, t2, t3;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/background.png");
    t3.loadFromFile("images/frame.png");

    // Create sprites for the tetromino, background, and frame
    Sprite s(t1), background(t2), frame(t3);

    // Declare clock to keep track of elapsed time betwene frames
    Clock clock;

    // Initialize variables for handling movement and timing
    int dx = 0;
    int colorNum = 1;
    bool isRotate = false;
    float timer = 0, delay = 0.3;

    while (window.isOpen())
    {
        // Update timer with the elapsed time since last frame
        float deltaTime = clock.getElapsedTime().asSeconds();
        clock.restart();

        timer += deltaTime;

        // Handle events
        Event event;
        while (window.pollEvent(event))
        {
            // Close the window if user clicks the close button
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Up)
                {
                    isRotate = true;
                }
                else if (event.key.code == Keyboard::Left)
                {
                    dx = -1;
                }
                else if (event.key.code == Keyboard::Right)
                {
                    dx = 1;
                }
            }
        }

        // If the down arrow is held, decrease the delay time to speed up the fall of the tetromino
        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            delay = 0.05;
        }
        
        // Move the tetromino horizontally
        for (int i = 0; i < 4; i++)
        {
            b[i] = a[i];
            a[i].x += dx;
        }

        // If the tetromino is not in a valid position, revert to the previous position
        if (!checkValidPosition())
        {
            for (int i = 0; i < 4; i++)
            {
                a[i] = b[i];
            }
        }

        // Rotate the tetromino
        if (isRotate)
        {
            // Use the second block as the pivot for rotation
            Point pivotBlock = a[1]; 

            // Rotate 4 blocks around the pivot block
            for (int i = 0; i < 4; i++)
            {
                int xDistanceFromPivot = a[i].x - pivotBlock.x;
                int yDistanceFromPivot = a[i].y - pivotBlock.y;

                a[i].x = pivotBlock.x - yDistanceFromPivot;
                a[i].y = pivotBlock.y + xDistanceFromPivot;
            }
        }

        // Move the tetromino down
        if (timer > delay)
        {
            // Move each block in the tetromino down by one row
            for (int i = 0; i < 4; i++)
            {
                b[i] = a[i];
                a[i].y += 1;
            }

            // If the tetromino is not in a valid position, revert to the previous position
            if (!checkValidPosition())
            {
                // Save the tetromino's position in the field
                for (int i = 0; i < 4; i++)
                {
                    int row = b[i].y;
                    int column = b[i].x;
                    field[row][column] = colorNum;
                }

                // Generate a new tetromino's color
                colorNum = rand() % 7 + 1;

                // Generate a new tetromino by randomly selecting one of the 7 tetromino shapes
                int n = rand() % 7;
                for (int i = 0; i < 4; i++)
                {
                    // Set the tetromino's coordinate using the encoded figure data

                    // The x-coordinate will be the remainder of the division by 2
                    a[i].x = figures[n][i] % 2;

                    // The y-coordinate will be the integer division by 2
                    a[i].y = figures[n][i] / 2;
                }
            }

            timer = 0;
        }

        // Check and clear the completed lines
        int  k = ROWS - 1;
        for (int i = ROWS - 1; i >= 0; i--)
        {
            // Count the number of filled blocks in the current row
            int count = 0;
            for (int j = 0; j < COLS; j++)
            {
                if (field[i][j] != 0)
                {
                    count++;
                }

                field[k][j] = field[i][j];
            }

            // If the row is not filled fully, move it to the next row
            if (count < COLS)
            {
                k--;
            }
        }

        // Reset variables for handling movement and timing
        dx = 0;
        delay = 0.3;
        isRotate = false;

        // Draw the current state onto the game window
        window.clear(Color::White);
        window.draw(background);

        // Draw all blocks from the game field
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                if (field[i][j] == 0)
                    continue;

                s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
                s.setPosition(j * 18, i * 18);
                s.move(28, 31); // offset
                window.draw(s);
            }
        }

        // Draw the current tetromino
        for (int i = 0; i < 4; i++)
        {
            s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
            s.setPosition(a[i].x * 18, a[i].y * 18);
            s.move(28, 31); // offset
            window.draw(s);
        }

        window.draw(frame);
        window.display();
    }

    return 0;
}