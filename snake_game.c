#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define WIDTH 20
#define HEIGHT 20

int gameOver;
int score;
int highScore;
int x, y; // Snake head position
int fruitX, fruitY; // Food position
int tailX[100], tailY[100]; // Snake tail positions
int nTail; // Length of the tail
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum eDirection dir;
int speed = 100; // Speed of the snake (in milliseconds)
int paused = 0; // Pause state

// Function prototypes
void Setup();
void Draw();
void Input();
void Logic();
void ShowMenu();
void SetDifficulty();
void SaveHighScore();
void LoadHighScore();
void Color(int color);

void Setup() {
    gameOver = 0;
    dir = STOP;
    x = WIDTH / 2;
    y = HEIGHT / 2;
    fruitX = rand() % WIDTH;
    fruitY = rand() % HEIGHT;
    score = 0;
    nTail = 0;
    LoadHighScore();
}

void Draw() {
    system("cls"); // Clear the console
    Color(7); // Default color (white)

    for (int i = 0; i < WIDTH + 2; i++)
        printf("#");
    printf("\n");

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0)
                printf("#");
            if (i == y && j == x) {
                Color(10); // Green for snake head
                printf("O");
                Color(7); // Reset to default
            } else if (i == fruitY && j == fruitX) {
                Color(12); // Red for food
                printf("F");
                Color(7); // Reset to default
            } else {
                int printTail = 0;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        Color(2); // Dark green for tail
                        printf("o");
                        Color(7); // Reset to default
                        printTail = 1;
                    }
                }
                if (!printTail)
                    printf(" ");
            }
            if (j == WIDTH - 1)
                printf("#");
        }
        printf("\n");
    }

    for (int i = 0; i < WIDTH + 2; i++)
        printf("#");
    printf("\n");

    printf("Score: %d\n", score);
    printf("High Score: %d\n", highScore);
    if (paused) {
        Color(14); // Yellow for pause message
        printf("Game Paused! Press 'P' to Resume.\n");
        Color(7); // Reset to default
    }
}

void Input() {
    if (_kbhit()) {
        char key = _getch();
        switch (key) {
            case 'a':
                if (dir != RIGHT) dir = LEFT;
                break;
            case 'd':
                if (dir != LEFT) dir = RIGHT;
                break;
            case 'w':
                if (dir != DOWN) dir = UP;
                break;
            case 's':
                if (dir != UP) dir = DOWN;
                break;
            case 'p':
                paused = !paused;
                break;
            case 'x':
                gameOver = 1;
                break;
        }
    }
}

void Logic() {
    if (paused) return;

    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }

    if (x >= WIDTH || x < 0 || y >= HEIGHT || y < 0)
        gameOver = 1;

    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y)
            gameOver = 1;
    }

    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % WIDTH;
        fruitY = rand() % HEIGHT;
        nTail++;
        if (score > highScore) {
            highScore = score;
            SaveHighScore();
        }
    }
}

void ShowMenu() {
    int choice;
    do {
        system("cls");
        Color(14); // Yellow for menu
        printf("=== SNAKE GAME ===\n");
        Color(7); // Reset to default
        printf("1. Start Game\n");
        printf("2. Set Difficulty\n");
        printf("3. View High Score\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                Setup();
                while (!gameOver) {
                    Draw();
                    Input();
                    Logic();
                    Sleep(speed); // Control speed
                }
                printf("Game Over! Your score: %d\n", score);
                printf("Press any key to continue...");
                _getch();
                break;
            case 2:
                SetDifficulty();
                break;
            case 3:
                system("cls");
                printf("High Score: %d\n", highScore);
                printf("Press any key to continue...");
                _getch();
                break;
            case 4:
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
                Sleep(1000);
        }
    } while (choice != 4);
}

void SetDifficulty() {
    int choice;
    system("cls");
    printf("=== DIFFICULTY LEVEL ===\n");
    printf("1. Easy (Slow)\n");
    printf("2. Medium (Normal)\n");
    printf("3. Hard (Fast)\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            speed = 200;
            break;
        case 2:
            speed = 100;
            break;
        case 3:
            speed = 50;
            break;
        default:
            printf("Invalid choice! Setting to Medium.\n");
            speed = 100;
            Sleep(1000);
    }
}

void SaveHighScore() {
    FILE *file = fopen("highscore.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d", highScore);
        fclose(file);
    }
}

void LoadHighScore() {
    FILE *file = fopen("highscore.txt", "r");
    if (file != NULL) {
        fscanf(file, "%d", &highScore);
        fclose(file);
    } else {
        highScore = 0;
    }
}

void Color(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

int main() {
    srand(time(0));
    ShowMenu();
    return 0;
}
