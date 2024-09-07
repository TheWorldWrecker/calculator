/*****************************************************************/
/*                                                               */
/*   CASIO fx-9860G SDK Library                                  */
/*                                                               */
/*   File name : [ProjectName].c                                 */
/*                                                               */
/*   Copyright (c) 2006 CASIO COMPUTER CO., LTD.                 */
/*                                                               */
/*****************************************************************/

#include "fxlib.h"
#include "timer.h"

#define WIDTH (17)  // 25
#define HEIGHT (15)  // 21
#define SNAKE (1)
#define FOOD (-1)
#define EMPTY (0)
#define SPEED_LO (0)
#define SPEED_MD (1)
#define SPEED_HI (2)
#define UP (0)
#define RIGHT (1)
#define DOWN (2)
#define LEFT (3)

GRAPHDATA filledGraph = {4, 4, (unsigned char*) "\xF0\xF0\xF0\xF0"};

int board[WIDTH][HEIGHT];
int snakePos[WIDTH*HEIGHT][2];
int snakeLen = 3;
int headX = WIDTH / 3;
int headY = HEIGHT / 2;
int dir = RIGHT;
int speed = SPEED_MD;
int dead = 0;
unsigned int rSeed = 0;

void titleScreen() {
    unsigned int key;
    Bdisp_AllClr_VRAM();
    locate(3, 2);
    Print("SNAKE");
    locate(3, 4);
    Print("Select speed");
    locate(3, 5);
    Print("1: Low");
    locate(3, 6);
    Print("2: Medium");
    locate(3, 7);
    Print("3: High");
    while(1) {
        GetKey(&key);
        switch(key) {
            case KEY_CHAR_1:
                speed = SPEED_LO;
                initGame();
                break;
            case KEY_CHAR_2:
                speed = SPEED_MD;
                initGame();
                break;
            case KEY_CHAR_3:
                speed = SPEED_HI;
                initGame();
                break;
        }
    }
}

int initGame() {
    int i, j;
    int key;
    srand(rSeed);
    for (i = 0; i < WIDTH; i++) {
        for (j = 0; j < HEIGHT; j++) {
            board[i][j] = EMPTY;
        }
    }
    for(i = 0; i < WIDTH*HEIGHT; ++i) {
        snakePos[i][0] = 0;
        snakePos[i][1] = 0;
    }
    headX = WIDTH / 3;
    headY = HEIGHT / 2;
    dir = RIGHT;
    board[headX][headY] = SNAKE;
    board[headX-1][headY] = SNAKE;
    board[headX-2][headY] = SNAKE;
    snakeLen = 3;
    snakePos[0][0] = headX;
    snakePos[0][1] = headY;
    snakePos[1][0] = headX-1;
    snakePos[1][1] = headY;
    snakePos[2][0] = headX-2;
    snakePos[2][1] = headY;

    dead = 0;
    board[WIDTH*2/3][HEIGHT/2] = FOOD;
    drawScreen();
    while(1) {
        GetKey(&key);
        switch (key) {
            case KEY_CTRL_UP:
                dir = UP;
                game();
                break;
            case KEY_CTRL_DOWN:
                dir = DOWN;
                game();
                break;
            // case KEY_CTRL_LEFT:
            //     dir = LEFT;
            //     break;
            case KEY_CTRL_RIGHT:
                dir = RIGHT;
                game();
                break;
            case KEY_CTRL_EXIT:
                KillTimer(ID_USER_TIMER1);
                titleScreen();
                return;
            case KEY_CTRL_EXE:
                if(dead) {
                    KillTimer(ID_USER_TIMER1);
                    titleScreen();
                    return;
                }
        }
    }
}

int placeFood() {
    int emptyCells[WIDTH*HEIGHT][2];
    int emptyCount = 0;
    int x, y;
    int i, j;
    int randIndex;
    for(i = 0; i < WIDTH; ++i) {
        for(j = 0; j < HEIGHT; ++j) {
            if(board[i][j] == EMPTY) {
                emptyCells[emptyCount][0] = i;
                emptyCells[emptyCount][1] = j;
                emptyCount++;
            }
        }
    }
    if(emptyCount == 0) {
        dead = 1;
        return;
    }
    randIndex = rand() % emptyCount;
    x = emptyCells[randIndex][0];
    y = emptyCells[randIndex][1];
    board[x][y] = FOOD;
}

int snakeCellGraph(int x, int y) {
    int i = 15;
    if(board[x][y] == SNAKE) {
        if(y>0 && board[x][y-1] == SNAKE) {
            i -= 1;
        }
        if(x<WIDTH-1 && board[x+1][y] == SNAKE) {
            i -= 2;
        }
        if(y<HEIGHT-1 && board[x][y+1] == SNAKE) {
            i -= 4;
        }
        if(x>0 && board[x-1][y] == SNAKE) {
            i -= 8;
        }
        return i;
    }
    else if(board[x][y] == FOOD) {
        return 16;
    }
    return 0;
}

int game() {
    unsigned int key;
    frame();
    while(1) {
        GetKey(&key);
        switch (key) {
            case KEY_CTRL_UP:
                if(dir == LEFT || dir == RIGHT) {
                    dir = UP;
                    frame();
                }
                break;
            case KEY_CTRL_DOWN:
                if(dir == LEFT || dir == RIGHT) {
                    dir = DOWN;
                    frame();
                }
                break;
            case KEY_CTRL_LEFT:
                if(dir == UP || dir == DOWN) {
                    dir = LEFT;
                    frame();
                }
                break;
            case KEY_CTRL_RIGHT:
                if(dir == UP || dir == DOWN) {
                    dir = RIGHT;
                    frame();
                }
                break;
            case KEY_CTRL_EXIT:
                KillTimer(ID_USER_TIMER1);
                titleScreen();
                return;
            case KEY_CTRL_EXE:
                if(dead) {
                    KillTimer(ID_USER_TIMER1);
                    titleScreen();
                    return;
                }
        }
    }
}

void drawSnake() {
    DISPGRAPH graph;
    int i; int j;
    graph.WriteModify = IMB_WRITEMODIFY_NORMAL;
    graph.WriteKind = IMB_WRITEKIND_OVER;
    for(i = 0; i < WIDTH; ++i) {
        for(j = 0; j < HEIGHT; ++j) {
            if(board[i][j] == EMPTY) {
                continue;
            }
            graph.x = i*4 +45;
            graph.y = j*4 +2;
            graph.GraphData = filledGraph;
            Bdisp_WriteGraph_VRAM(&graph);
        }
    }
    // Hollow out snake
    for(i = 0; i < snakeLen-1; ++i) {
        Bdisp_ClearLineVRAM(
            snakePos[i][0]*4 + 46, snakePos[i][1]*4 + 3,
            snakePos[i+1][0]*4 + 46, snakePos[i+1][1]*4 + 3);
        Bdisp_ClearLineVRAM(
            snakePos[i][0]*4 + 46, snakePos[i][1]*4 + 4,
            snakePos[i+1][0]*4 + 46, snakePos[i+1][1]*4 + 4);
        Bdisp_ClearLineVRAM(
            snakePos[i][0]*4 + 47, snakePos[i][1]*4 + 3,
            snakePos[i+1][0]*4 + 47, snakePos[i+1][1]*4 + 3);
        Bdisp_ClearLineVRAM(
            snakePos[i][0]*4 + 47, snakePos[i][1]*4 + 4,
            snakePos[i+1][0]*4 + 47, snakePos[i+1][1]*4 + 4);

    }
}

void itoa(int n, char *str) {
    int i = 0, j;
    if (n == 0) {
        str[i++] = '0'; // Handle 0 explicitly
    } else {
        while(n > 0) {
            str[i++] = n % 10 + '0';
            n /= 10;
        }
    }
    str[i] = '\0'; // Null-terminate the string
    // Reverse the string
    for(j = 0; j < i/2; ++j) {
        char tmp = str[j];
        str[j] = str[i-j-1];
        str[i-j-1] = tmp;
    }
}

int drawScreen() {
    char str[10];
    int i;
    Bdisp_AllClr_VRAM();
    Bdisp_DrawLineVRAM(43, 0, 43, 63);
    Bdisp_DrawLineVRAM(43, 63, 114, 63);
    Bdisp_DrawLineVRAM(114, 63, 114, 0);
    Bdisp_DrawLineVRAM(114, 0, 43, 0);
    Bdisp_DrawLineVRAM(44, 1, 44, 62);
    Bdisp_DrawLineVRAM(44, 62, 113, 62);
    Bdisp_DrawLineVRAM(113, 62, 113, 1);
    Bdisp_DrawLineVRAM(113, 1, 44, 1);
    drawSnake();

    PrintMini(14, 14, "LEN", 0);
    itoa(snakeLen, str);
    PrintMini(14, 20, str, 0);
    PrintMini(14, 40, "SPD", 0);
    switch (speed)
    {
        case SPEED_LO:
            PrintMini(14, 46, "LO", 0);
            break;
        case SPEED_MD:
            PrintMini(14, 46, "MD", 0);
            break;
        case SPEED_HI:
            PrintMini(14, 46, "HI", 0);
            break;
        default:
            break;
    }

    Bdisp_PutDisp_DD();
}

int frame() {
    const nextFrameTime = speed == SPEED_HI ? 140
     : speed == SPEED_MD ? 210 : 280;
    int i, j;
    const int dx = dir == RIGHT ? 1 : dir == LEFT ? -1 : 0;
    const int dy = dir == DOWN ? 1 : dir == UP ? -1 : 0;
    const tailX = snakePos[snakeLen-1][0];
    const tailY = snakePos[snakeLen-1][1];
    const int newX = headX + dx;
    const int newY = headY + dy;
    int ateFood;
    KillTimer(ID_USER_TIMER1);
    if(dead) return;
    if(Bdisp_GetPoint_VRAM(0,0) == 1) {  // If at calculator menu
        if(!dead) {
            SetTimer(ID_USER_TIMER1, nextFrameTime, frame);
        }
        return;
    }
    if(newX < 0 || newX >= WIDTH || newY < 0 || newY >= HEIGHT) {
        dead = 1;
        return;
    }
    if(board[newX][newY] == SNAKE && !(newX == tailX && newY == tailY)) {
        dead = 1;
        return;
    }
    ateFood = board[newX][newY] == FOOD;
    for(i = snakeLen-1; i > 0; --i) {
        snakePos[i][0] = snakePos[i-1][0];
        snakePos[i][1] = snakePos[i-1][1];
    }
    if(ateFood) {
        snakePos[snakeLen][0] = tailX;
        snakePos[snakeLen][1] = tailY;
        snakeLen++;
        board[tailX][tailY] = SNAKE;
        placeFood();
    } else {
        board[tailX][tailY] = EMPTY;
    }
    board[newX][newY] = SNAKE;
    snakePos[0][0] = newX;
    snakePos[0][1] = newY;
    headX = newX;
    headY = newY;

    drawScreen();

    if(!dead) {
        SetTimer(ID_USER_TIMER1, nextFrameTime, frame);
    }

    Bdisp_PutDisp_DD();
}

int backgroundTick() {
    SetTimer(ID_USER_TIMER2, 100, backgroundTick);
    ++rSeed;
}

int AddIn_main(int isAppli, unsigned short OptionNum)
{

    backgroundTick();
    titleScreen();

    return 1;
}




//****************************************************************************
//**************                                              ****************
//**************                 Notice!                      ****************
//**************                                              ****************
//**************  Please do not change the following source.  ****************
//**************                                              ****************
//****************************************************************************


#pragma section _BR_Size
unsigned long BR_Size;
#pragma section


#pragma section _TOP

//****************************************************************************
//  InitializeSystem
//
//  param   :   isAppli   : 1 = Application / 0 = eActivity
//              OptionNum : Option Number (only eActivity)
//
//  retval  :   1 = No error / 0 = Error
//
//****************************************************************************
int InitializeSystem(int isAppli, unsigned short OptionNum)
{
    return INIT_ADDIN_APPLICATION(isAppli, OptionNum);
}

#pragma section

