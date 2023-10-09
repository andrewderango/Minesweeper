#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define SIZE 10
#define BOMBS 15

void createBoard(char board[SIZE][SIZE]) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            board[row][col] = '-';
        }
    }
}

void placeBombs(char board[SIZE][SIZE]) {
    int bombArr[BOMBS];

    // Initialize bombArr with invalid values
    for (int i = 0; i < BOMBS; i++) {
        bombArr[i] = -1;
    }

    for (int bombsPlaced = 0; bombsPlaced < BOMBS; bombsPlaced++) {
        int num = rand() % (int)(pow(SIZE, 2));

        int duplicateBomb = 0;
        for (int j = 0; j < (int)pow(SIZE, 2); j++) {
            if (bombArr[j] == num) {
                duplicateBomb = 1;
                break;
            }
        }

        if (duplicateBomb) {
            bombsPlaced--;
        }
        else {
            bombArr[bombsPlaced] = num;
        }
    }

    for (int l = 0; l < BOMBS; l++) {
        // printf("%d: ", bombArr[l]);
        // printf("%d %d\n", bombArr[l]/SIZE, bombArr[l]%SIZE);
        board[bombArr[l]/SIZE][bombArr[l]%SIZE] = 'X';
    }
}

void printBoard(char board[SIZE][SIZE], int showBoard) {
    printf("  ");
    for (int head = 0; head < SIZE; head++) {
        printf("%d ", head);
    }
    printf("\n");
    for (int row = 0; row < SIZE; row++) {
        printf("%d ", row);
        for (int col = 0; col < SIZE; col++) {
            if (!showBoard && board[row][col] == 'X') {
                printf("- ");
            }
            else {
                printf("%c ", board[row][col]);
            }
        }
        printf("\n");
    }
}

void getUserInput(char board[SIZE][SIZE], int *x, int *y) {
    long double inputX, inputY;
    int validInput = 0;

    while (!validInput) {
        printf("Enter coordinates (x y): ");
        if (scanf("%Lf %Lf", &inputX, &inputY) == 2) {
            int inputXint = (int)inputX;
            int inputYint = (int)inputY;
            if (inputXint >= 0 && inputXint < SIZE && inputYint >= 0 && inputYint < SIZE && (board[inputYint][inputXint] == '-' || board[inputYint][inputXint] == 'X')) {
                validInput = 1;
                *x = inputXint;
                *y = inputYint;
            } 
            else {
                printf("Invalid input. Please try again.\n");
            }
        } else {
            printf("Invalid input format. Please enter two numbers separated by a space (x y).\n");
            while (getchar() != '\n'); // Clear input buffer
        }
    }
}

int countAdjacentBombs(char board[SIZE][SIZE], int x, int y, int visited[SIZE][SIZE]) {
    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE || visited[y][x]) {
        return 0;
    }
    visited[y][x] = 1;

    int adjBombs = 0;

    if (board[y][x] == 'X') {
        return 1; // user loses
    }
    else {
        if (board[y-1][x-1]=='X' && y!=0 && x!=0) {
            adjBombs++;
        }
        if (board[y-1][x]=='X' && y!=0) {
            adjBombs++;
        }
        if (board[y-1][x+1]=='X' && y!=0 && x!=SIZE-1) {
            adjBombs++;
        }
        if (board[y][x-1]=='X' && x!=0) {
            adjBombs++;
        }
        if (board[y][x+1]=='X' && x!=SIZE-1) {
            adjBombs++;
        }
        if (board[y+1][x-1]=='X' && y!=SIZE-1 && x!=0) {
            adjBombs++;
        }
        if (board[y+1][x]=='X' && y!=SIZE-1) {
            adjBombs++;
        }
        if (board[y+1][x+1]=='X' && y!=SIZE-1 && x!=SIZE-1) {
            adjBombs++;
        }
    }
    board[y][x] = '0' + adjBombs;

    if (adjBombs == 0) {
        countAdjacentBombs(board, x-1, y-1, visited);
        countAdjacentBombs(board, x-1, y, visited);
        countAdjacentBombs(board, x-1, y+1, visited);
        countAdjacentBombs(board, x, y-1, visited);
        countAdjacentBombs(board, x, y, visited);
        countAdjacentBombs(board, x, y+1, visited);
        countAdjacentBombs(board, x+1, y-1, visited);
        countAdjacentBombs(board, x+1, y, visited);
        countAdjacentBombs(board, x+1, y+1, visited);
    }
    return 0; // game continues
}

int main() {
    char board[SIZE][SIZE];
    int gameOver = 0; // boolean checking if the game is over
    int remainingCells = SIZE * SIZE - BOMBS;
    int visited[SIZE][SIZE] = {0};
    srand(time(NULL));

    printf("\nWelcome to Minesweeper!\nEnter the coordinates (x y) to reveal a cell.\n");
    createBoard(board);
    placeBombs(board);
    printBoard(board, 1); // change to 0

    int x, y;
    while (remainingCells>0 && !gameOver) {
        getUserInput(board, &x, &y);
        gameOver = countAdjacentBombs(board, x, y, visited);
        if (!gameOver) {
            remainingCells--;
            printBoard(board, 0);
        }
    }

    if (gameOver) {
        printf("Game Over! You hit a bomb. Here is a view of the final board: \n");
        printBoard(board, 1);
    }
    else {
        printf("You win! Congratulations.\n");
    }

    return 0;
}