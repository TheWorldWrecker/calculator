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


//****************************************************************************
//  AddIn_main (Sample program main function)
//
//  param   :   isAppli   : 1 = This application is launched by MAIN MENU.
//                        : 0 = This application is launched by a strip in eACT application.
//
//              OptionNum : Strip number (0~3)
//                         (This parameter is only used when isAppli parameter is 0.)
//
//  retval  :   1 = No error / 0 = Error
//
//****************************************************************************

unsigned int rSeed = 12345;
int randInt(int min, int max) {
    int randomNum;
    // LCG parameters from Numerical Recipes
    rSeed = 1664525 * rSeed + 1013904223;
    
    // Generate a random number between min and max (inclusive)
    randomNum = (rSeed % (max - min)) + min;
    return randomNum;
}

void drawUI(int board[30][30], int revealed[30][30]) {
    int i;
    for(i = 0; i<64; ++i) {
        if(i%2) {
            //Bdisp_ClearLineVRAM(38, i, 39, i);
        }
        else {
            Bdisp_DrawLineVRAM(38, i, 39, i);
        }
    }

    Bdisp_SetPoint_VRAM(5, 4, 1);
    Bdisp_SetPoint_VRAM(5, 9, 1);
    Bdisp_SetPoint_VRAM(10, 4, 1);
    Bdisp_SetPoint_VRAM(10, 9, 1);
    Bdisp_DrawLineVRAM(5, 6, 5, 7);
    Bdisp_DrawLineVRAM(10, 6, 10, 7);
    Bdisp_DrawLineVRAM(6, 5, 6, 8);
    Bdisp_DrawLineVRAM(7, 4, 7, 9);
    Bdisp_DrawLineVRAM(8, 4, 8, 9);
    Bdisp_DrawLineVRAM(9, 5, 9, 8);
    Bdisp_SetPoint_VRAM(7, 6, 0);
}

void drawCellOutlines(int x, int y, int camX, int camY,
 int up, int down, int left, int right) {

    const sX = 79+10*(x-camX);
    const sY = 27+10*(y-camY);

    if(up) {
        Bdisp_DrawLineVRAM(sX, sY, sX+9, sY);
    }
    if(down) {
        Bdisp_DrawLineVRAM(sX, sY+9, sX+9, sY+9);
    }
    if(left) {
        Bdisp_DrawLineVRAM(sX, sY, sX, sY+9);
    }
    if(right) {
        Bdisp_DrawLineVRAM(sX+9, sY, sX+9, sY+9);
    }
    
}

// state: 0 = hidden, 1 = revealed, 2 = flagged
void drawCell(int x, int y, int camX, int camY, int num, int state, int dead) {
    const sX = 79+10*(x-camX);
    const sY = 27+10*(y-camY);
    drawCellOutlines(x, y, camX, camY, 1, 1, 1, 1);
    switch(state) {
        case 0:  // Hidden tile, draw checker pattern
            if((!dead) || (dead && num != -1)) {
                Bdisp_DrawLineVRAM(sX+1, sY+2, sX+2, sY+1);
                Bdisp_DrawLineVRAM(sX+1, sY+4, sX+4, sY+1);
                Bdisp_DrawLineVRAM(sX+1, sY+6, sX+6, sY+1);
                Bdisp_DrawLineVRAM(sX+1, sY+8, sX+8, sY+1);
                Bdisp_DrawLineVRAM(sX+3, sY+8, sX+8, sY+3);
                Bdisp_DrawLineVRAM(sX+5, sY+8, sX+8, sY+5);
                Bdisp_DrawLineVRAM(sX+7, sY+8, sX+8, sY+7);
            }
            else if(dead && num == -1) {  // Show mine on unrevealed square
                Bdisp_SetPoint_VRAM(sX+2, sY+2, 1);
                Bdisp_SetPoint_VRAM(sX+2, sY+7, 1);
                Bdisp_SetPoint_VRAM(sX+7, sY+2, 1);
                Bdisp_SetPoint_VRAM(sX+7, sY+7, 1);
                Bdisp_DrawLineVRAM(sX+2, sY+4, sX+2, sY+5);
                Bdisp_DrawLineVRAM(sX+3, sY+3, sX+3, sY+6);
                Bdisp_DrawLineVRAM(sX+4, sY+2, sX+4, sY+7);
                Bdisp_DrawLineVRAM(sX+5, sY+2, sX+5, sY+7);
                Bdisp_DrawLineVRAM(sX+6, sY+3, sX+6, sY+6);
                Bdisp_DrawLineVRAM(sX+7, sY+4, sX+7, sY+5);
                Bdisp_SetPoint_VRAM(sX+4, sY+4, 0);
            }
            break;
        case 1:  // Revealed tile, draw numbers
            switch(num) {
                case 1:
                    //Bdisp_SetPoint_VRAM(sX+4, sY+2, 1);  // This line is cursed
                    Bdisp_DrawLineVRAM(sX+5, sY+2, sX+5, sY+6);
                    Bdisp_DrawLineVRAM(sX+4, sY+7, sX+6, sY+7);
                    Bdisp_DrawLineVRAM(sX+4, sY+2, sX+5, sY+2);
                    break;
                case 2:
                    Bdisp_DrawLineVRAM(sX+3, sY+3, sX+4, sY+2);
                    Bdisp_DrawLineVRAM(sX+5, sY+2, sX+6, sY+3);
                    Bdisp_DrawLineVRAM(sX+6, sY+4, sX+4, sY+6);
                    Bdisp_DrawLineVRAM(sX+3, sY+7, sX+6, sY+7);
                    break;
                case 3:
                    Bdisp_DrawLineVRAM(sX+3, sY+2, sX+5, sY+2);
                    Bdisp_DrawLineVRAM(sX+6, sY+2, sX+6, sY+3);
                    Bdisp_DrawLineVRAM(sX+4, sY+4, sX+5, sY+4);
                    Bdisp_DrawLineVRAM(sX+6, sY+5, sX+6, sY+6);
                    Bdisp_DrawLineVRAM(sX+3, sY+6, sX+4, sY+7);
                    Bdisp_DrawLineVRAM(sX+4, sY+7, sX+5, sY+7);
                    break;
                case 4:
                    Bdisp_DrawLineVRAM(sX+3, sY+2, sX+3, sY+5);
                    Bdisp_DrawLineVRAM(sX+4, sY+5, sX+6, sY+5);
                    Bdisp_DrawLineVRAM(sX+5, sY+3, sX+5, sY+7);
                    break;
                case 5:
                    Bdisp_DrawLineVRAM(sX+3, sY+2, sX+6, sY+2);
                    Bdisp_DrawLineVRAM(sX+3, sY+3, sX+3, sY+4);
                    Bdisp_DrawLineVRAM(sX+4, sY+4, sX+5, sY+4);
                    Bdisp_DrawLineVRAM(sX+6, sY+5, sX+6, sY+6);
                    Bdisp_DrawLineVRAM(sX+4, sY+7, sX+5, sY+7);
                    Bdisp_SetPoint_VRAM(sX+3, sY+6, 1);
                    break;
                case 6:
                    Bdisp_SetPoint_VRAM(sX+6, sY+3, 1);
                    Bdisp_DrawLineVRAM(sX+4, sY+2, sX+5, sY+2);
                    Bdisp_DrawLineVRAM(sX+3, sY+3, sX+3, sY+6);
                    Bdisp_DrawLineVRAM(sX+4, sY+5, sX+5, sY+5);
                    Bdisp_SetPoint_VRAM(sX+6, sY+6, 1);
                    Bdisp_DrawLineVRAM(sX+4, sY+7, sX+5, sY+7);
                    break;
                case 7:
                    Bdisp_DrawLineVRAM(sX+3, sY+2, sX+3, sY+4);
                    Bdisp_DrawLineVRAM(sX+4, sY+2, sX+6, sY+2);
                    Bdisp_DrawLineVRAM(sX+6, sY+2, sX+6, sY+4);
                    Bdisp_DrawLineVRAM(sX+5, sY+5, sX+5, sY+7);
                    break;
                case 8:
                    Bdisp_DrawLineVRAM(sX+4, sY+2, sX+5, sY+2);
                    Bdisp_DrawLineVRAM(sX+4, sY+4, sX+5, sY+4);
                    Bdisp_DrawLineVRAM(sX+4, sY+7, sX+5, sY+7);
                    Bdisp_DrawLineVRAM(sX+3, sY+5, sX+3, sY+6);
                    Bdisp_DrawLineVRAM(sX+6, sY+5, sX+6, sY+6);
                    Bdisp_SetPoint_VRAM(sX+3, sY+3, 1);
                    Bdisp_SetPoint_VRAM(sX+6, sY+3, 1);
                    break;
                case -1:
                    Bdisp_DrawLineVRAM(sX+1, sY+1, sX+8, sY+1);
                    Bdisp_DrawLineVRAM(sX+8, sY+1, sX+8, sY+8);
                    Bdisp_DrawLineVRAM(sX+8, sY+8, sX+1, sY+8);
                    Bdisp_DrawLineVRAM(sX+1, sY+8, sX+1, sY+1);
                    Bdisp_DrawLineVRAM(sX+2, sY+3, sX+3, sY+2);
                    Bdisp_DrawLineVRAM(sX+2, sY+6, sX+3, sY+7);
                    Bdisp_DrawLineVRAM(sX+6, sY+2, sX+7, sY+3);
                    Bdisp_DrawLineVRAM(sX+6, sY+7, sX+7, sY+6);
                    Bdisp_SetPoint_VRAM(sX+4, sY+4, 1);
                    break;
                case 0:
                    break;
            }
            break;
        case 2:  // Flagged tile, draw flag
            Bdisp_DrawLineVRAM(sX+4, sY+2, sX+6, sY+2);
            Bdisp_DrawLineVRAM(sX+3, sY+3, sX+6, sY+3);
            Bdisp_DrawLineVRAM(sX+4, sY+4, sX+6, sY+4);
            Bdisp_DrawLineVRAM(sX+6, sY+5, sX+6, sY+6);
            Bdisp_DrawLineVRAM(sX+3, sY+7, sX+7, sY+7);
            if(dead && num != -1) {  // Show X on flagged square
                Bdisp_DrawLineVRAM(sX+1, sY+1, sX+8, sY+8);
                Bdisp_DrawLineVRAM(sX+1, sY+8, sX+8, sY+1);
            }
            Bdisp_AreaReverseVRAM(sX+1, sY+1, sX+8, sY+8);
            break;
    }
}

int outOfBounds(int x, int y, int width, int height) {
    return x < 0 || x >= width || y < 0 || y >= height;
}

int abs(int x) {
    return x < 0 ? -x : x;
}

void updateBoard(int board[30][30], int revealed[30][30], int camX, int camY,
    int width, int height, int cursorX, int cursorY, int dead) {
    int i; int j;
    Bdisp_AllClr_VRAM();
    for(i = -4; i<5; ++i) {
        for(j = -3; j<4; ++j) {
            if(outOfBounds(camX+i, camY+j, width, height)) {
                if(camX+i == -1 && 0 <= camY+j && camY+j < height)
                    drawCellOutlines(camX+i, camY+j, camX, camY, 0, 0, 0, 1);
                if(camX+i == width && 0 <= camY+j && camY+j < height)
                    drawCellOutlines(camX+i, camY+j, camX, camY, 0, 0, 1, 0);
                if(camY+j == -1 && 0 <= camX+i && camX+i < width)
                    drawCellOutlines(camX+i, camY+j, camX, camY, 0, 1, 0, 0);
                if(camY+j == height && 0 <= camX+i && camX+i < width)
                    drawCellOutlines(camX+i, camY+j, camX, camY, 1, 0, 0, 0);
            }
            else {
                drawCell(camX+i, camY+j, camX, camY,
                 board[camX+i][camY+j], revealed[camX+i][camY+j], dead);
            }
        }
    }
    drawUI(board, revealed);
    if(!dead) {
        Bdisp_AreaReverseVRAM(79+10*(cursorX-camX), 27+10*(cursorY-camY),
        79+10*(cursorX-camX)+9, 27+10*(cursorY-camY)+9);
    }
}

// Return value: 1 if dead, 0 if not
int click(int cx, int cy, int board[30][30],
 int revealed[30][30], int width, int height, int mines, int started) {
    int i; int j; int k; int l;
    if(outOfBounds(cx, cy, width, height)) {
        return 0;
    }
    if(!started) {
        // Place mines, mine is -1
        // Do not place mines within 3x3 square of first click
        for(i = 0; i < mines; i++) {
            int x = rand() % width;
            int y = rand() % height;
            if(board[x][y] == -1 || abs(cx-x) <= 1 || abs(cy-y) <= 1) {
                i--;  // Try again
            }
            else {
                board[x][y] = -1;
            }
        }
        // Write numbers
        for(i = 0; i<width; i++) {
            for(j = 0; j<height; j++) {
                if(board[i][j] != -1) {
                    int count = 0;
                    for(k = i-1; k<=i+1; k++) {
                        for(l = j-1; l<=j+1; l++) {
                            if(k >= 0 && k < width && l >= 0 && l < height) {
                                if(board[k][l] == -1) {
                                    count++;
                                }
                            }
                        }
                    }
                    board[i][j] = count;
                }
            }
        }
    }
    if(revealed[cx][cy] != 0) {  // Opened or flagged
        return 0;
    }
    revealed[cx][cy] = 1;
    if(board[cx][cy] == 0) {
        for(i = -1; i<=1; ++i) {
            for(j = -1; j<=1; ++j) {
                if(revealed[cx+i][cy+j] == 2) {
                    revealed[cx+i][cy+j] = 0;  // Unflag
                }
                click(cx+i, cy+j, board, revealed, width, height, mines, 1);
            }
        }
    }
    return board[cx][cy] == -1;
}

int chord(int cx, int cy, int board[30][30],
 int revealed[30][30], int width, int height, int mines, int started) {
    int i; int j; int k; int l;
    int flags = 0;
    int died = 0;

    for(i = -1; i<=1; ++i) {
        for(j = -1; j<=1; ++j) {
            if(revealed[cx+i][cy+j] == 2) {
                flags++;
            }
        }
    }
    if(flags != board[cx][cy]) {
        return 0;
    }
    for(i = -1; i<=1; ++i) {
        for(j = -1; j<=1; ++j) {
            if(revealed[cx+i][cy+j] == 0) {
                if(click(cx+i,cy+j,board,revealed,width,height,mines,1)) {
                    died = 1;
                }
            }
        }
    }
    return died;
}

// Easy: 9x9, 10
// Medium: 16x16, 40
// Hard: 30x16, 99
int game(int width, int height, int mines) {
    int i; int j; int k; int l;
    const int minCamX = 3; const int minCamY = 2;
    const int maxCamX = width-5;
    const int maxCamY = height-4;
    int camX = width/2; int camY = height/2;  // Position of center
    int cursorX = camX; int cursorY = camY;
    unsigned int key;
    int started = 0;
    int dead = 0;

    int board[30][30];  // No dynamic memory :(   pretend its [width][height]
    int revealed[30][30];  // 0 = hidden, 1 = revealed, 2 = flagged

    // initialize revealed
    for(i = 0; i<width; i++) {
        for(j = 0; j<height; j++) {
                revealed[i][j] = 0;
        }
    }

    updateBoard(board, revealed, camX, camY, width, height,
     cursorX, cursorY, dead);

    while(1){
        GetKey(&key);
        switch(key) {
            case KEY_CTRL_UP:
                if(cursorY > 0) {
                    cursorY--;
                }
                if(camY > minCamY && cursorY < camY) {
                    camY--;
                }
                if(dead) {
                    cursorY = camY;
                }
                break;
            case KEY_CTRL_DOWN:
                if(cursorY < height-1) {
                    cursorY++;
                }
                if(camY <= maxCamY && cursorY > camY) {
                    camY++;
                }
                if(dead) {
                    cursorY = camY;
                }
                break;
            case KEY_CTRL_LEFT:
                if(cursorX > 0) {
                    cursorX--;
                }
                if(camX > minCamX && cursorX < camX) {
                    camX--;
                }
                if(dead) {
                    cursorX = camX;
                }
                break;
            case KEY_CTRL_RIGHT:
                if(cursorX < width-1) {
                    cursorX++;
                }
                if(camX <= maxCamX && cursorX > camX) {
                    camX++;
                }
                if(dead) {
                    cursorX = camX;
                }
                break;
            case KEY_CHAR_7:
                if(!dead) {
                    if(revealed[cursorX][cursorY] == 0) {
                        dead = click(cursorX, cursorY, board, revealed, 
                        width, height, mines, started);
                    }
                    else if(revealed[cursorX][cursorY] == 1) {
                        dead = chord(cursorX, cursorY, board, revealed, 
                        width, height, mines, started);
                    }
                }
                started = 1;
                break;
            case KEY_CHAR_4:
                if(!dead) {
                    if(revealed[cursorX][cursorY] == 0) {
                        revealed[cursorX][cursorY] = 2;
                    }
                    else if(revealed[cursorX][cursorY] == 2) {
                        revealed[cursorX][cursorY] = 0;
                    }
                }
                break;
            case KEY_CTRL_EXIT:
                difficultySelect();
                return 0;
                break;
            case KEY_CTRL_EXE:
                if(dead) {
                    difficultySelect();
                    return 0;
                }
                break;
        }
        updateBoard(board, revealed, camX, camY,
         width, height, cursorX, cursorY, dead);
    }
}

int titleScreen() {
    unsigned int key;

    Bdisp_AllClr_DDVRAM();
    locate(3,2);
    Print("MINESWEEPER");
    locate(3,4);
    Print("Controls");
    locate(3,5);
    Print(" 4: Flag");
    locate(3,6);
    Print(" 7: Open / Chord");
    locate(3,7);
    Print("EXE to continue");

    while(1) {
        GetKey(&key);
        if(key == KEY_CTRL_EXE) {
            break;
        }
    }
    difficultySelect();
}

int difficultySelect() {
    unsigned int key;
    Bdisp_AllClr_DDVRAM();
    locate(3,2);
    Print("SELECT");
    locate(3,4);
    Print("1: Easy");
    locate(3,5);
    Print("2: Medium");
    locate(3,6);
    Print("3: Hard");
    locate(3,7);
    Print("4: Extreme");
    while(1) {
        GetKey(&key);
        switch(key) {
            case KEY_CHAR_1:
                game(9, 9, 10);
                break;
            case KEY_CHAR_2:
                game(16, 16, 40);
                break;
            case KEY_CHAR_3:
                game(30, 16, 99);
                break;
            case KEY_CHAR_4:
                game(30, 30, 200);
                break;
            case KEY_CTRL_EXIT:
                titleScreen();
        }
    }
}

int backgroundTick() {
    ++rSeed;
    SetTimer(ID_USER_TIMER1, 100, backgroundTick);
}

int AddIn_main(int isAppli, unsigned short OptionNum)
{
    //backgroundTick();
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

