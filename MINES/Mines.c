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

unsigned int rSeed = 0;

int time = 0;  // in 0.1 secs
int width;
int height;
int totalMines;
int minCamX; int minCamY;
int maxCamX;
int maxCamY;
int camX; int camY;  // Position of center
int cursorX; int cursorY;
unsigned int key;
int started = 0;
int dead = 0;
int won = 0;
int flags = 0;
int cellsOpened = 0;

int board[30][30];  // No dynamic memory :(   pretend its [width][height]
int revealed[30][30];  // 0 = hidden, 1 = revealed, 2 = flagged



// Wall of hex lmao
GRAPHDATA graphs[16] = {
    {10, 10, (unsigned char*)  // 0
    "\xFF\xC0\x80\x40\x80\x40\x80\x40\x80\x40\x80\x40\x80\x40\x80\x40"
    "\x80\x40\xFF\xC0"},
    {10, 10, (unsigned char*)  // 1
    "\xFF\xC0\x80\x40\x8C\x40\x84\x40\x84\x40\x84\x40\x84\x40\x8E\x40"
    "\x80\x40\xFF\xC0"},
    {10, 10, (unsigned char*)  // 2
    "\xFF\xC0\x80\x40\x8C\x40\x92\x40\x82\x40\x84\x40\x88\x40\x9E\x40"
    "\x80\x40\xFF\xC0"},
    {10, 10, (unsigned char*)  // 3
    "\xFF\xC0\x80\x40\x9E\x40\x82\x40\x8C\x40\x82\x40\x92\x40\x8C\x40"
    "\x80\x40\xFF\xC0"},
    {10, 10, (unsigned char*)  // 4
    "\xFF\xC0\x80\x40\x90\x40\x94\x40\x94\x40\x9E\x40\x84\x40\x84\x40"
    "\x80\x40\xFF\xC0"},
    {10, 10, (unsigned char*)  // 5
    "\xFF\xC0\x80\x40\x9E\x40\x90\x40\x9C\x40\x82\x40\x92\x40\x8C\x40"
    "\x80\x40\xFF\xC0"},
    {10, 10, (unsigned char*)  // 6
    "\xFF\xC0\x80\x40\x8C\x40\x92\x40\x90\x40\x9C\x40\x92\x40\x8C\x40"
    "\x80\x40\xFF\xC0"},
    {10, 10, (unsigned char*)  // 7
    "\xFF\xC0\x80\x40\x9E\x40\x92\x40\x92\x40\x84\x40\x84\x40\x84\x40"
    "\x80\x40\xFF\xC0"},
    {10, 10, (unsigned char*)  // 8
    "\xFF\xC0\x80\x40\x8C\x40\x92\x40\x8C\x40\x92\x40\x92\x40\x8C\x40"
    "\x80\x40\xFF\xC0"},
    {10, 10, (unsigned char*)  // Mine (9)
    "\xFF\xC0\x80\x40\xAD\x40\x9E\x40\xB7\x40\xBF\x40\x9E\x40\xAD\x40"
    "\x80\x40\xFF\xC0"},
    {10, 10, (unsigned char*)  // Flag (10)
    "\xFF\xC0\xFF\xC0\xF1\xC0\xE1\xC0\xF1\xC0\xFD\xC0\xFD\xC0\xE0\xC0"
    "\xFF\xC0\xFF\xC0"},
    {10, 10, (unsigned char*)   // Wrong flag (11)
    "\xFF\xC0\xBF\x40\xD0\xC0\xE1\xC0\xF1\xC0\xF1\xC0\xED\xC0\xC0\xC0"
    "\xBF\x40\xFF\xC0"},
    {10, 10, (unsigned char*)   // Checkerboard (12)
    "\xFF\xC0\xAA\xC0\xD5\x40\xAA\xC0\xD5\x40\xAA\xC0\xD5\x40\xAA\xC0"
    "\xD5\x40\xFF\xC0"},
    {17, 11, (unsigned char*)  // Mine counter outline (13)
    "\x7F\xFF\x00\xE0\x03\x80\xC0\x01\x80\x80\x00\x80\x80\x00\x80\x80"
    "\x00\x80\x80\x00\x80\x80\x00\x80\xC0\x01\x80\xE0\x03\x80\x7F\xFF\x00"},
    {5, 8, (unsigned char*)  // Timer (14)
    "\xF8\x88\xF8\x70\x20\x50\xF8\xF8"},
    {6, 6, (unsigned char*)  // Mine no outline (15)
    "\xB4\x78\xDC\xFC\x78\xB4"}
};

void itoa(char* str, int n) {
    int i = 0;
    int j = 0;
    char temp;
    char isNegative = 0;
    if(n < 0) {
        isNegative = 1;
        n = -n;
    }
    do {
        str[i++] = n%10 + '0';
        n /= 10;
    } while(n > 0);
    if(isNegative) {
        str[i++] = '-';
    }
    str[i] = '\0';
    for(j = 0; j < i/2; ++j) {
        temp = str[j];
        str[j] = str[i-j-1];
        str[i-j-1] = temp;
    }
}

void itoa_two_digits(char* str, int n) {
    int i = 0;
    int j, k;
    char isNegative = 0;
    if(n < 0) {
        isNegative = 1;
        n = -n;
    }
    // Convert number to string in reverse order
    do {
        str[i++] = n % 10 + '0';
        n /= 10;
    } while(n > 0);
    // Add leading zero if necessary to ensure two digits
    if(i == 1) {
        str[i++] = '0'; // Add leading zero for numbers less than 10
    }
    if(isNegative) {
        str[i++] = '-'; // This will not be used for fixed two digits positive numbers
    }
    str[i] = '\0';
    // Reverse the string to correct order
    for(j = 0, k = i - 1; j < k; j++, k--) {
        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;
    }
}

void drawUI(int board[30][30], int revealed[30][30], int minesLeft,
 int width, int height, int cx, int cy) {
    int i; int j;
    DISPBOX box;
    DISPGRAPH graph;
    char str[20];
    const int mcDigits = (minesLeft >= 0)
        ? 1 + (minesLeft >= 10) + (minesLeft >= 100)
        : 2 + (minesLeft <= -10) + (minesLeft <= -100) ;
    const int mineCountX = 15+(4-mcDigits)*2;
    const int mmX = 18-(width-1)/2;
    const int mmY = 42-(height-1)/2;
    graph.WriteModify = IMB_WRITEMODIFY_NORMAL;
    graph.WriteKind = IMB_WRITEKIND_OVER;

    box.left = 0;
    box.top = 0;
    box.right = 39;
    box.bottom = 63;
    Bdisp_AreaClr_VRAM(&box);

    for(i = 0; i<64; ++i) {
        if(!(i%2)) {
            Bdisp_DrawLineVRAM(38, i, 39, i);
        }
    }

    // Erases random black line at the left
    // I think it's caused when I draw the graph slightly OOB to the right
    // it loops around to the left
    // Too lazy to fix it so I just erase it and pretend it's not there
    Bdisp_ClearLineVRAM(0, 0, 0, 63);

    // Draw mine icon
    box.left = 5;
    box.top = 4;
    box.right = 14;
    box.bottom = 13;
    graph.x = 5;
    graph.y = 4;
    graph.GraphData = graphs[15];
    Bdisp_WriteGraph_VRAM(&graph);

    // Draw mine counter
    box.left = 14;
    box.top = 2;
    box.right = 30;
    box.bottom = 13;
    graph.x = 14;
    graph.y = 2;
    graph.GraphData = graphs[13];
    Bdisp_WriteGraph_VRAM(&graph);

    // Draw timer icon
    box.left = 3;
    box.top = 14;
    box.right = 9;
    box.bottom = 20;
    graph.x = 3;
    graph.y = 14;
    graph.GraphData = graphs[14];
    Bdisp_WriteGraph_VRAM(&graph);

    // Draw mine counter number
    itoa(str, minesLeft);
    PrintMini(mineCountX, 5, str, 0);

    // Draw timer
    if(!started) {
        time = 0;
    }
    itoa_two_digits(str, (time/600)%100);
    PrintMini(11, 16, str, 0);
    Bdisp_SetPoint_VRAM(19, 17, 1);
    Bdisp_SetPoint_VRAM(19, 19, 1);
    itoa_two_digits(str, (time/10)%60);
    PrintMini(21, 16, str, 0);
    Bdisp_SetPoint_VRAM(29, 20, 1);
    itoa(str, time%10);
    PrintMini(31, 16, str, 0);

    // Draw minimap
    // Outline
    Bdisp_DrawLineVRAM(mmX-1, mmY-1, mmX+width, mmY-1);
    Bdisp_DrawLineVRAM(mmX+width, mmY-1, mmX+width, mmY+height);
    Bdisp_DrawLineVRAM(mmX+width, mmY+height, mmX-1, mmY+height);
    Bdisp_DrawLineVRAM(mmX-1, mmY+height, mmX-1, mmY-1);
    // Crosshair
    Bdisp_AreaReverseVRAM(mmX+cx, mmY-1, mmX+cx, mmY-3);
    Bdisp_AreaReverseVRAM(mmX+cx, mmY+height, mmX+cx, mmY+height+2);
    Bdisp_AreaReverseVRAM(mmX-1, mmY+cy, mmX-3, mmY+cy);
    Bdisp_AreaReverseVRAM(mmX+width, mmY+cy, mmX+width+2, mmY+cy);
    // Contents
    for(i = 0; i<width; ++i) {
        for(j = 0; j<height; ++j) {
            Bdisp_SetPoint_VRAM(i+mmX, j+mmY, !((revealed[i][j])||(won)));
        }
    }
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
    DISPBOX box;
    DISPGRAPH graph;
    box.left = sX;
    box.top = sY;
    box.right = sX + 9;
    box.bottom = sY + 9;
    graph.x = sX;
    graph.y = sY;
    graph.WriteModify = IMB_WRITEMODIFY_NORMAL;
    graph.WriteKind = IMB_WRITEKIND_OVER;

    switch(state) {
        case 0:  // Hidden tile, draw checker pattern
            if((!dead) || (dead && num != -1)) {
                graph.GraphData = graphs[12];
            }
            else if(dead && num == -1 && !won) {  // Show mine on unrevealed
                graph.GraphData = graphs[9];
            }
            else if(dead && num == -1 && won) {  // Auto place flags
                graph.GraphData = graphs[10];
            }
            break;
        case 1:  // Revealed tile, draw numbers
            switch(num) {
                case 0:
                    graph.GraphData = graphs[0];
                    break;
                case 1:
                    graph.GraphData = graphs[1];
                    break;
                case 2:
                    graph.GraphData = graphs[2];
                    break;
                case 3:
                    graph.GraphData = graphs[3];
                    break;
                case 4:
                    graph.GraphData = graphs[4];
                    break;
                case 5:
                    graph.GraphData = graphs[5];
                    break;
                case 6:
                    graph.GraphData = graphs[6];
                    break;
                case 7:
                    graph.GraphData = graphs[7];
                    break;
                case 8:
                    graph.GraphData = graphs[8];
                    break;
                case -1:
                    graph.GraphData = graphs[9];
                    break;
            }
            break;
        case 2:  // Flagged tile, draw flag
            graph.GraphData = graphs[ (dead && num != -1)? 11 : 10 ];
            break;
    }
    Bdisp_WriteGraph_VRAM(&graph);
    //Bdisp_PutDisp_DD();
    //Sleep(100);
}

int outOfBounds(int x, int y, int width, int height) {
    return x < 0 || x >= width || y < 0 || y >= height;
}

int abs(int x) {
    return x < 0 ? -x : x;
}
void updateBoard(int board[30][30], int revealed[30][30], int camX, int camY,
    int width, int height, int cursorX, int cursorY, int dead, int minesLeft) {
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
    drawUI(board, revealed, minesLeft, width, height, cursorX, cursorY);
    Bdisp_AreaReverseVRAM(79+10*(cursorX-camX), 27+10*(cursorY-camY),
    79+10*(cursorX-camX)+9, 27+10*(cursorY-camY)+9);
}

// Return value: 1 if dead, 0 if not
int click(int cx, int cy, int board[30][30], int revealed[30][30],
 int width, int height, int mines, int started, int* pFlags) {
    int i; int j; int k; int l;
    if(outOfBounds(cx, cy, width, height)) {
        return 0;
    }
    if(!started) {
        time = 0;
        // Place mines, mine is -1
        // Do not place mines within 3x3 square of first click
        srand(rSeed);
        for(i = 0; i < mines;) {
            int x = rand() % width;
            int y = rand() % height;
            if(board[x][y] == -1 || (abs(cx-x) <= 1 && abs(cy-y) <= 1)) {
                continue;  // Skip to the next iteration without incrementing i
            }
            else {
                board[x][y] = -1;
                i++;  // Increment i only if a mine is successfully placed
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
    ++cellsOpened;
    if(board[cx][cy] == 0) {
        for(i = -1; i<=1; ++i) {
            for(j = -1; j<=1; ++j) {
                if(revealed[cx+i][cy+j] == 2) {
                    revealed[cx+i][cy+j] = 0;  // Unflag
                    (*pFlags)--;
                }
                click(cx+i,cy+j,board,revealed,width,height,mines,1,pFlags);
            }
        }
    }
    return board[cx][cy] == -1;
}

int chord(int cx, int cy, int board[30][30], int revealed[30][30],
 int width, int height, int mines, int started) {
    int i; int j; int k; int l;
    int flags = 0;
    int died = 0;

    for(i = -1; i<=1; ++i) {
        for(j = -1; j<=1; ++j) {
            if(outOfBounds(cx+i, cy+j, width, height)) {
                continue;
            }
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
            if(outOfBounds(cx+i, cy+j, width, height)) {
                continue;
            }
            if(revealed[cx+i][cy+j] == 0) {
                if(click(cx+i,cy+j,board,revealed,width,height,mines,1,&flags)){
                    died = 1;
                }
            }
        }
    }
    return died;
}


int game() {
    int i; int j; int k; int l;
    minCamX = 3; minCamY = 2;
    maxCamX = width-5; maxCamY = height-4;
    camX = width/2; camY = height/2;
    cursorX = camX; cursorY = camY;
    started = 0;
    dead = 0;
    won = 0;
    flags = 0;
    cellsOpened = 0;

    // initialize board / revealed arrays
    for(i = 0; i<width; i++) {
        for(j = 0; j<height; j++) {
            board[i][j] = 0;
            revealed[i][j] = 0;
        }
    }

    updateBoard(board, revealed, camX, camY, width, height,
     cursorX, cursorY, dead, totalMines-flags);

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
                break;
            case KEY_CTRL_DOWN:
                if(cursorY < height-1) {
                    cursorY++;
                }
                if(camY <= maxCamY && cursorY > camY) {
                    camY++;
                }
                break;
            case KEY_CTRL_LEFT:
                if(cursorX > 0) {
                    cursorX--;
                }
                if(camX > minCamX && cursorX < camX) {
                    camX--;
                }
                break;
            case KEY_CTRL_RIGHT:
                if(cursorX < width-1) {
                    cursorX++;
                }
                if(camX <= maxCamX && cursorX > camX) {
                    camX++;
                }
                break;
            case KEY_CHAR_7:  // Click
                if(!dead) {
                    if(revealed[cursorX][cursorY] == 0) {
                        dead = click(cursorX, cursorY, board, revealed, 
                        width, height, totalMines, started, &flags);
                        if(dead) {
                            updateBoard(board, revealed, camX, camY,
                            width, height, cursorX, cursorY, dead, totalMines-flags);
                        }
                    }
                    else if(revealed[cursorX][cursorY] == 1) {
                        dead = chord(cursorX, cursorY, board, revealed, 
                        width, height, totalMines, started);
                        if(dead) {
                            updateBoard(board, revealed, camX, camY,
                            width, height, cursorX, cursorY, dead, totalMines-flags);
                        }
                    }
                }
                started = 1;
                break;
            case KEY_CHAR_8:  // Flag
                if(!dead) {
                    if(revealed[cursorX][cursorY] == 0) {
                        revealed[cursorX][cursorY] = 2;
                        flags++;
                    }
                    else if(revealed[cursorX][cursorY] == 2) {
                        revealed[cursorX][cursorY] = 0;
                        flags--;
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
        if(cellsOpened >= width*height-totalMines && !dead) {
            dead = 1;  // Victory!
            won = 1;
            flags = totalMines;
        }
        updateBoard(board, revealed, camX, camY,
            width, height, cursorX, cursorY, dead, totalMines-flags);
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
    Print(" 7: Open / Chord");
    locate(3,6);
    Print(" 8: Flag");
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
    started = 0;
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
                width = 9;
                height = 9;
                totalMines = 10;
                game();
                break;
            case KEY_CHAR_2:
                width = 16;
                height = 16;
                totalMines = 40;
                game();
                break;
            case KEY_CHAR_3:
                width = 30;
                height = 16;
                totalMines = 99;
                game();
                break;
            case KEY_CHAR_4:
                width = 30;
                height = 30;
                totalMines = 200;
                game();
                break;
            case KEY_CTRL_EXIT:
                titleScreen();
        }
    }
}

int backgroundTick() {
    if(Bdisp_GetPoint_VRAM(0,0)) {
        return;
    }
    SetTimer(ID_USER_TIMER1, 100, backgroundTick);
    if(!started)
        ++rSeed;
    if(started && !dead) {
        drawUI(board,revealed,totalMines-flags,width,height,cursorX,cursorY);
        ++time;
        Bdisp_PutDisp_DD();
    }
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

