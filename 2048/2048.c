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


GRAPHDATA graphs[19] = {
    {16, 16, (unsigned char*)  // Empty [0]
    "\xFF\xFF\x80\x01\x80\x01\x80\x01\x80\x01\x80\x01\x80\x01\x80\x01"
    "\x80\x01\x80\x01\x80\x01\x80\x01\x80\x01\x80\x01\x80\x01\xFF\xFF"},
    {16, 16, (unsigned char*)  // 2 [1]
    "\xFF\xFF\x80\x01\x80\x01\x80\x01\x81\x81\x82\x41\x80\x41\x80\x81"
    "\x81\x01\x82\x01\x83\xC1\x80\x01\x80\x01\x80\x01\x80\x01\xFF\xFF"},
    {16, 16, (unsigned char*)  // 4 [2]
    "\xFF\xFF\x80\x01\x80\x01\x80\x01\x82\x01\x82\x41\x82\x41\x83\xC1"
    "\x80\x41\x80\x41\x80\x41\x80\x01\x80\x01\x80\x01\x80\x01\xFF\xFF"},
    {16, 16, (unsigned char*)  // 8 [3]
    "\xFF\xFF\x80\x01\x80\x01\x80\x01\x81\x81\x82\x41\x82\x41\x81\x81"
    "\x82\x41\x82\x41\x81\x81\x80\x01\x80\x01\x80\x01\x80\x01\xFF\xFF"},
    {16, 16, (unsigned char*)  // 16 [4]
    "\xFF\xFF\x80\x01\x80\x01\x80\x01\x84\x61\x8C\x91\x84\x81\x84\xE1"
    "\x84\x91\x84\x91\x8E\x61\x80\x01\x80\x01\x80\x01\x80\x01\xFF\xFF"},
    {16, 16, (unsigned char*)  // 32 [5]
    "\xFF\xFF\x80\x01\x80\x01\x80\x01\x8C\x61\x92\x91\x82\x11\x8C\x21"
    "\x82\x41\x92\x81\x8C\xF1\x80\x01\x80\x01\x80\x01\x80\x01\xFF\xFF"},
    {16, 16, (unsigned char*)  // 64 [6]
    "\xFF\xFF\x80\x01\x80\x03\x80\x01\x8C\x81\x92\x93\x90\x91\x9C\xF1"
    "\x92\x13\x92\x11\x8C\x11\x80\x03\x80\x01\x80\x01\xA4\x89\xFF\xFF"},
    {16, 16, (unsigned char*)  // 128 [7]
    "\xFF\xFF\x80\x01\x80\x03\x80\x01\x80\x01\x97\x73\xB1\x51\x97\x71"
    "\x94\x53\x97\x71\x80\x01\x80\x03\x80\x01\x80\x01\xA4\x93\xFF\xFF"},
    {16, 16, (unsigned char*)  // 256 [8]
    "\xFF\xFF\x80\x01\x80\x03\x80\x01\x80\x01\xBB\xBB\x8A\x21\xBB\xB9"
    "\xA0\xAB\xBB\xB9\x80\x01\x80\x03\x80\x01\x80\x01\xA4\x93\xFF\xFF"},
    {16, 16, (unsigned char*)  // 512 [9]
    "\xFF\xFF\x80\x01\x80\x03\x80\x01\x80\x01\xB9\x73\xA3\x11\xB9\x71"
    "\x89\x43\xB9\x71\x80\x01\x80\x03\x80\x01\x80\x01\xA4\x93\xFF\xFF"},
    {16, 16, (unsigned char*)  // 1024 [10]
    "\xFF\xFF\x80\x01\x84\xE3\x8C\xA3\x84\xA3\x84\xA3\x84\xE3\x80\x03"
    "\x8E\xA3\x82\xA3\x8E\xE3\x88\x23\x8E\x23\x80\x03\xBF\xFF\xFF\xFF"},
    {16, 16, (unsigned char*)  // 2048 [11]
    "\xFF\xFF\x80\x01\x8E\xE3\x82\xA3\x8E\xA3\x88\xA3\x8E\xE3\x80\x03"
    "\x8A\xE3\x8A\xA3\x8E\xE3\x82\xA3\x82\xE3\x80\x03\xBF\xFF\xFF\xFF"},
    {16, 16, (unsigned char*)  // 4096 [12]
    "\xFF\xFF\x80\x01\x8A\xE3\x8A\xA3\x8E\xA3\x82\xA3\x82\xE3\x80\x03"
    "\x8E\xE3\x8A\x83\x8E\xE3\x82\xA3\x8E\xE3\x80\x03\xBF\xFF\xFF\xFF"},
    {16, 16, (unsigned char*)  // 8192 [13]
    "\xFF\xFF\x80\x01\x8E\x43\x8A\xC3\x8E\x43\x8A\x43\x8E\x43\x80\x03"
    "\x8E\xE3\x8A\x23\x8E\xE3\x82\x83\x8E\xE3\x80\x03\xBF\xFF\xFF\xFF"},
    {16, 16, (unsigned char*)  // 16384 [14]
    "\xFF\xFF\xFF\xFD\xFB\x1D\xF3\x7D\xFB\x1D\xFB\x5D\xFB\x1D\xFF\xFD"
    "\xC4\x55\xF5\x55\xC4\x45\xF5\x75\xC4\x75\xFF\xFD\x80\x01\xFF\xFF"},
    {16, 16, (unsigned char*)  // 32768 [15]
    "\xFF\xFF\xFF\xFD\xF1\x1D\xFD\xDD\xF1\x1D\xFD\x7D\xF1\x1D\xFF\xFD"
    "\xC4\x45\xD5\xD5\xF4\x45\xF5\x55\xF4\x45\xFF\xFD\x80\x01\xFF\xFF"},
    {16, 16, (unsigned char*)  // 65536 [16]
    "\xFF\xFF\xFF\xFD\xF1\x1D\xF7\x7D\xF1\x1D\xF5\xDD\xF1\x1D\xFF\xFD"
    "\xC4\x45\xDF\x5D\xC4\x45\xF7\x55\xC4\x45\xFF\xFD\x80\x01\xFF\xFF"},
    {16, 16, (unsigned char*)  // 130172 [17]
    "\xFF\xFF\xFF\xFD\xE8\x8D\xCE\xAD\xE8\xAD\xEE\xAD\xE8\x8D\xFF\xFD"
    "\xE8\x8D\xCA\xED\xEA\x8D\xEE\xBD\xEE\x8D\xFF\xFD\x80\x01\xFF\xFF"},
    {24, 24, (unsigned char*)  // Can move UI [18]
    "\x00\x7E\x00\x03\x81\xC0\x04\x18\x20\x08\x3C\x10\x10\x7E\x08\x20"
    "\x18\x04\x40\x18\x02\x40\x18\x02\x40\x18\x02\x88\x18\x11\x98\x00"
    "\x19\xBF\xDB\xFD\xBF\xDB\xFD\x98\x00\x19\x88\x18\x11\x40\x18\x02"
    "\x40\x18\x02\x40\x18\x02\x20\x18\x04\x10\x7E\x08\x08\x3C\x10\x04"
    "\x18\x20\x03\x81\xC0\x00\x7E\x00"}
};

int board[4][4] = {0};
int score = 0;
int moves = 0;

void initBoard() {
    int i; int j;
    score = 0;
    moves = 0;
    for(i = 0; i<4; ++i) {
        for(j = 0; j<4; ++j) {
            board[i][j] = 0;
        }
    }
}

void placeRandom() {
    int empty = 0;
    int i; int j;
    int pos;
    if(isDead()) {
        return;
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            empty += board[i][j] == 0;
        }
    }
    if (empty == 0) {
        return;
    }
    pos = rand() % empty;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (board[i][j] == 0) {
                if (pos == 0) {
                    board[i][j] = 1 + (rand()%10==0);
                    drawBoard();
                    //Bdisp_AreaReverseVRAM(53+15*j, 2+15*i, 68+15*j, 17+15*i);
                    Bdisp_DrawLineVRAM(55+15*j, 4+15*i, 66+15*j, 4+15*i);
                    Bdisp_DrawLineVRAM(55+15*j, 4+15*i, 55+15*j, 15+15*i);
                    Bdisp_DrawLineVRAM(55+15*j, 15+15*i, 66+15*j, 15+15*i);
                    Bdisp_DrawLineVRAM(66+15*j, 4+15*i, 66+15*j, 15+15*i);
                    return;
                }
                pos--;
            }
        }
    }
    return;
}

// Returns false when moving would not change the board
// True is either there is a merge or an empty space 
// OR there is an empty space in front of a non-empty space
// empty spaces BEHIND do not count
int canMove(int dir) {
    int i; int j;
    int isFirstZero = 1;
    switch (dir)
    {
        case 0:
            for(i = 0; i<4; ++i) {
                isFirstZero = 1;
                for(j = 3; j>=0; --j) {
                    if(board[i][j] == 0) {
                        if(isFirstZero) {
                            continue;
                        }
                        return 1;
                    }
                    if(isFirstZero) {
                        isFirstZero = 0;
                    }
                    if(j>=1 && board[i][j] == board[i][j-1]) {
                        return 1;
                    }
                }
            }
            break;
        case 1:
            for(j = 0; j<4; ++j) {
                isFirstZero = 1;
                for(i = 0; i<4; ++i) {
                    if(board[i][j] == 0) {
                        if(isFirstZero) {
                            continue;
                        }
                        return 1;
                    }
                    if(isFirstZero) {
                        isFirstZero = 0;
                    }
                    if(i<3 && board[i][j] == board[i+1][j]) {
                        return 1;
                    }
                }
            }
            break;
        case 2:
            for(i = 0; i<4; ++i) {
                isFirstZero = 1;
                for(j = 0; j<4; ++j) {
                    if(board[i][j] == 0) {
                        if(isFirstZero) {
                            continue;
                        }
                        return 1;
                    }
                    if(isFirstZero) {
                        isFirstZero = 0;
                    }
                    if(j<3 && board[i][j] == board[i][j+1]) {
                        return 1;
                    }
                }
            }
            break;
        case 3:
            for(j = 0; j<4; ++j) {
                isFirstZero = 1;
                for(i = 3; i>=0; --i) {
                    if(board[i][j] == 0) {
                        if(isFirstZero) {
                            continue;
                        }
                        return 1;
                    }
                    if(isFirstZero) {
                        isFirstZero = 0;
                    }
                    if(i>=1 && board[i][j] == board[i-1][j]) {
                        return 1;
                    }
                }
            }
            break;
    }
    return 0;
}

int isDead() {
    int i; int j;
    for(i = 0; i<4; ++i) {
        for(j = 0; j<4; ++j) {
            if(board[i][j] == 0) {
                return 0;
            }
            if(i > 0 && board[i][j] == board[i-1][j]) {
                return 0;
            }
            if(i < 3 && board[i][j] == board[i+1][j]) {
                return 0;
            }
            if(j > 0 && board[i][j] == board[i][j-1]) {
                return 0;
            }
            if(j < 3 && board[i][j] == board[i][j+1]) {
                return 0;
            }
        }
    }
    return 1;
}

// 3: up, 2: right, 1: down, 0: left
void move(int dir) {
    int i; int j; int k;
    int merged[4][4] = {0};
    moves++;
    switch (dir)
    {
    case 0:
        for(i = 0; i<4; ++i) {
            for(j = 0; j<4; ++j) {
                for(k = j; k>0; --k) {
                    if(board[i][k-1] == 0) {
                        board[i][k-1] = board[i][k];
                        board[i][k] = 0;
                    } else if(board[i][k-1] == board[i][k] 
                    && !merged[i][k-1] && !merged[i][k]) {
                        board[i][k-1] += 1;
                        board[i][k] = 0;
                        merged[i][k-1] = 1;
                        score += 1 << board[i][k-1];
                    }
                }
            }
        }
        break;
    case 1:
        for(j = 0; j<4; ++j) {
            for(i = 3; i>=0; --i) {
                for(k = i; k<3; ++k) {
                    if(board[k+1][j] == 0) {
                        board[k+1][j] = board[k][j];
                        board[k][j] = 0;
                    } else if(board[k+1][j] == board[k][j] 
                    && !merged[k+1][j] && !merged[k][j]) {
                        board[k+1][j] += 1;
                        board[k][j] = 0;
                        merged[k+1][j] = 1;
                        score += 1 << board[k+1][j];
                    }
                }
            }
        }
        break;
    case 2:
        for(i = 0; i<4; ++i) {
            for(j = 3; j>=0; --j) {
                for(k = j; k<3; ++k) {
                    if(board[i][k+1] == 0) {
                        board[i][k+1] = board[i][k];
                        board[i][k] = 0;
                    } else if(board[i][k+1] == board[i][k]
                    && !merged[i][k+1] && !merged[i][k]) {
                        board[i][k+1] += 1;
                        board[i][k] = 0;
                        merged[i][k+1] = 1;
                        score += 1 << board[i][k+1];
                    }
                }
            }
        }
        break;
    case 3:
        for(j = 0; j<4; ++j) {
            for(i = 0; i<4; ++i) {
                for(k = i; k>0; --k) {
                    if(board[k-1][j] == 0) {
                        board[k-1][j] = board[k][j];
                        board[k][j] = 0;
                    } else if(board[k-1][j] == board[k][j]
                    && !merged[k-1][j] && !merged[k][j]) {
                        board[k-1][j] += 1;
                        board[k][j] = 0;
                        merged[k-1][j] = 1;
                        score += 1 << board[k-1][j];
                    }
                }
            }
        }
        break;
    default:
        break;
    }
    Bdisp_PutDisp_DD();
}

void atoi(int n, char* str) {
    int i = 0; int j = 0;
    if (n == 0) {
        str[i++] = '0'; // Handle case when n is 0
    }
    while(n > 0) {
        str[i++] = n % 10 + '0';
        n /= 10;
    }
    str[i] = '\0';
    for(j = 0; j < i / 2; ++j) {
        char temp = str[j];
        str[j] = str[i-j-1];
        str[i-j-1] = temp;
    }
}

int drawBoard() {
    int i; int j;
    char* str;
    DISPBOX box;
    DISPGRAPH graph;
    graph.WriteModify = IMB_WRITEMODIFY_NORMAL;
    graph.WriteKind = IMB_WRITEKIND_OVER;
    box.left = 53;
    box.top = 2;
    box.right = 113;
    box.bottom = 62;
    Bdisp_AreaClr_VRAM(&box);
    for(i = 0; i<4; ++i) {
        for(j = 0; j<4; ++j) {
            box.left = 53 + 15*j;
            box.top = 2 + 15*i;
            box.right = 68 + 15*j;
            box.bottom = 17 + 15*i;
            graph.x = box.left;
            graph.y = box.top;
            graph.GraphData = graphs[board[i][j]];
            Bdisp_WriteGraph_VRAM(&graph);
        }
    }
    PrintMini(7, 4, "SCORE", MINI_OVER);
    atoi(score, str);
    PrintMini(7, 10, str, MINI_OVER);

    PrintMini(7, 19, "MOVES", MINI_OVER);
    atoi(moves, str);
    PrintMini(7, 25, str, MINI_OVER);

    box.left = 13;
    box.top = 37;
    box.right = 13+24-1;
    box.bottom = 37+24-1;
    graph.x = box.left;
    graph.y = box.top;
    graph.GraphData = graphs[18];
    Bdisp_WriteGraph_VRAM(&graph);
    if(!canMove(3)) {
        box.left = 22;
        box.top = 39;
        box.right = 28;
        box.bottom = 46;
        Bdisp_AreaClr_VRAM(&box);
    }
    if(!canMove(2)) {
        box.left = 27;
        box.top = 46;
        box.right = 35;
        box.bottom = 51;
        Bdisp_AreaClr_VRAM(&box);
    }
    if(!canMove(1)) {
        box.left = 22;
        box.top = 51;
        box.right = 28;
        box.bottom = 58;
        Bdisp_AreaClr_VRAM(&box);
    }
    if(!canMove(0)) {
        box.left = 15;
        box.top = 46;
        box.right = 23;
        box.bottom = 51;
        Bdisp_AreaClr_VRAM(&box);
    }
    if(isDead()) {
        Bdisp_DrawLineVRAM(13, 37, 36, 60);
        Bdisp_DrawLineVRAM(13, 38, 35, 60);
        Bdisp_DrawLineVRAM(14, 37, 36, 59);
        Bdisp_DrawLineVRAM(13, 60, 36, 37);
        Bdisp_DrawLineVRAM(13, 59, 35, 37);
        Bdisp_DrawLineVRAM(14, 60, 36, 38);
    }
}

int game() {
    unsigned int key;
    Bdisp_AllClr_DDVRAM();
    initBoard();
    placeRandom();
    //placeRandom();
    drawBoard();
    while(1) {
        GetKey(&key);
        switch (key) {
            case KEY_CTRL_UP:
                if(isDead() || !canMove(3)) {
                    continue;
                }
                move(3);
                placeRandom();
                break;
            case KEY_CTRL_RIGHT:
                if(isDead() || !canMove(2)) {
                    continue;
                }
                move(2);
                placeRandom();
                break;
            case KEY_CTRL_DOWN:
                if(isDead() || !canMove(1)) {
                    continue;
                }
                move(1);
                placeRandom();
                break; 
            case KEY_CTRL_LEFT:
                if(isDead() || !canMove(0)) {
                    continue;
                }
                move(0);
                placeRandom();
                break; 
            case KEY_CTRL_EXIT:
                titleScreen();
                return 0;
            case KEY_CTRL_EXE:
                if(isDead()) {
                    titleScreen();
                    return 0;
                }
                break;
        }
    }
}

int backgroundTick() {
    SetTimer(ID_USER_TIMER1, 100, backgroundTick);
    rand();
}

int titleScreen() {
    unsigned int key;

    backgroundTick();
    Bdisp_AllClr_DDVRAM();
    locate(3,3);
    Print("2048");
    locate(3,6);
    Print("EXE to start");

    while(1) {
        GetKey(&key);
        if(key == KEY_CTRL_EXE) {
            break;
        }
    }
    game();
    return 0;
}



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
int AddIn_main(int isAppli, unsigned short OptionNum)
{
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

