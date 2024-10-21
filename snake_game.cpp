#include <iostream>
#include <vector>
#include <ncurses.h>
#include <unistd.h>
#include <ctime>

using namespace std;

const int width = 20;
const int height = 20;

struct Snake {
    int x, y;
};

struct Food {
    int x, y;
};

int main() {
    srand(time(0));

    vector<Snake> snake = {{width / 2, height / 2}};
    int snakeLength = 1;

    Food food = {rand() % width, rand() % height};

    int score = 0;
    char direction = 'd';
    bool gameOver = false;


    // Ncurses initialization
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);

    while (!gameOver) {
        // Input (using ncurses)
        int input = getch();
        switch (input) {
            case KEY_UP:    if (direction != 's') direction = 'w'; break;
            case KEY_LEFT:  if (direction != 'd') direction = 'a'; break;
            case KEY_DOWN:  if (direction != 'w') direction = 's'; break;
            case KEY_RIGHT: if (direction != 'a') direction = 'd'; break;
            case 'x':       gameOver = true; break; 
        }

        int newX = snake[0].x;
        int newY = snake[0].y;

        switch (direction) {
            case 'w': newY--; break;
            case 'a': newX--; break;
            case 's': newY++; break;
            case 'd': newX++; break;
        }

        if (newX < 0 || newX >= width || newY < 0 || newY >= height) {
            gameOver = true;
        }
        for (int i = 1; i < snakeLength; i++) {
            if (newX == snake[i].x && newY == snake[i].y) {
                gameOver = true;
            }
        }

        snake.insert(snake.begin(), {newX, newY});

        if (newX == food.x && newY == food.y) {
            score++;
            food = {rand() % width, rand() % height};
            snakeLength++;
        } else {
            snake.pop_back();
        }


        // Clear and draw (using ncurses)
        clear();  // Ncurses clear screen

        // Draw borders
        for (int i = 0; i < width + 2; i++) {
            mvprintw(0, i, "#");                 // Top border
            mvprintw(height + 1, i, "#");      // Bottom border
        }
        for (int i = 0; i < height + 2; i++) {
            mvprintw(i, 0, "#");                 // Left border
            mvprintw(i, width + 1, "#");      // Right border
        }


        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                bool isSnake = false;
                for (const auto& segment : snake) {
                    if (segment.x == j && segment.y == i) {
                        mvprintw(i + 1, j + 1, "O");
                        isSnake = true;
                        break;
                    }
                }
                if (!isSnake) {
                    if (food.x == j && food.y == i) {
                        mvprintw(i + 1, j + 1, "F");
                    } else {
                        mvprintw(i + 1, j + 1, " ");
                    }
                }
            }
        }



        mvprintw(height + 2, 0, "Score: %d", score);  // Display score
        refresh();                                   // Refresh ncurses display



        usleep(100000); 
    }

    endwin();
    cout << "Game Over!" << endl;

    return 0;
}
