/*****************************************************************/
/*                                                               */
/*   CASIO fx-9860G SDK Library                                  */
/*                                                               */
/*   File name : [ProjectName].c                                 */
/*                                                               */
/*   Copyright (c) 2006 CASIO COMPUTER CO., LTD.                 */
/*                                                               */
/*****************************************************************/

// thanks random internet user

#define __KEYBIOS_H__

#include "fxlib.h"
#include "timer.h"

#define KEY_CHAR_0 71
#define KEY_CHAR_1 72
#define KEY_CHAR_2 62
#define KEY_CHAR_3 52
#define KEY_CHAR_4 73
#define KEY_CHAR_5 63
#define KEY_CHAR_6 53
#define KEY_CHAR_7 74
#define KEY_CHAR_8 64
#define KEY_CHAR_9 54
#define KEY_CHAR_DP 61
#define KEY_CHAR_EXP 51
#define KEY_CHAR_PMINUS 41
#define KEY_CHAR_PLUS 42
#define KEY_CHAR_MINUS 32
#define KEY_CHAR_MULT 43
#define KEY_CHAR_DIV 33
#define KEY_CHAR_FRAC 75
#define KEY_CHAR_LPAR 55
#define KEY_CHAR_RPAR 45
#define KEY_CHAR_COMMA 35
#define KEY_CHAR_STORE 25
#define KEY_CHAR_LOG 66
#define KEY_CHAR_LN 56
#define KEY_CHAR_SIN 46
#define KEY_CHAR_COS 36
#define KEY_CHAR_TAN 26
#define KEY_CHAR_SQUARE 67
#define KEY_CHAR_POW 57
#define KEY_CTRL_EXE 31
#define KEY_CTRL_DEL 44
#define KEY_CTRL_AC 32
#define KEY_CTRL_FD 65
#define KEY_CTRL_EXIT 47
#define KEY_CTRL_SHIFT 78
#define KEY_CTRL_ALPHA 77
#define KEY_CTRL_OPTN 68
#define KEY_CTRL_VARS 58
#define KEY_CTRL_UP 28
#define KEY_CTRL_DOWN 37
#define KEY_CTRL_LEFT 38
#define KEY_CTRL_RIGHT 27
#define KEY_CTRL_F1 79
#define KEY_CTRL_F2 69
#define KEY_CTRL_F3 59
#define KEY_CTRL_F4 49
#define KEY_CTRL_F5 39
#define KEY_CTRL_F6 29
#define KEY_CTRL_MENU 48
#ifndef OS2Change
#define OS2Change
#ifndef OS2Change_GetOS2
#define OS2Change_GetOS2
typedef int(*sc_i2cp2sip)(char*, char*, short int*, short int*);
const unsigned int sc0015[] = { 0xD201D002, 0x422B0009, 0x80010070, 0x0015 };
#define GlibGetOSVersionInfo (*(sc_i2cp2sip)sc0015)
int OSVersionAsInt(void)
{
char mainversion;
char minorversion;
short release;
short build;
GlibGetOSVersionInfo( &mainversion, &minorversion, &release, &build );
return ( ( mainversion << 24 ) & 0xFF000000 ) | ( ( minorversion << 16 ) & 0x00FF0000 ) | ( release & 0x0000FFFF );
}
#define isOS2 (OSVersionAsInt() >= 0x02020000)
#define OS2(x,y) ((OSVersionAsInt() >= 0x02020000)?y:x)
#endif
#ifndef OS2Change_Keyboard
#define OS2Change_Keyboard
void delay(void)
{
char i;
for (i=0; i<5; i++){};
}
unsigned char CheckKeyRow(unsigned char code)
{
unsigned char result=0;
short*PORTB_CTRL=(void*)0xA4000102;
short*PORTM_CTRL=(void*)0xA4000118;
char*PORTB=(void*)0xA4000122;
char*PORTM=(void*)0xA4000138;
char*PORTA=(void*)0xA4000120;
short smask;
char cmask;
unsigned char column, row;
column = code>>4;
row = code &0x0F;
smask = 0x0003 << (( row %8)*2);
cmask = ~( 1 << ( row %8) );
if(row <8)
{
  *PORTB_CTRL = 0xAAAA ^ smask;
  *PORTM_CTRL = (*PORTM_CTRL & 0xFF00 ) | 0x00AA;
  delay();
  *PORTB = cmask;
  *PORTM = (*PORTM & 0xF0 ) | 0x0F;
}
else
{
  *PORTB_CTRL = 0xAAAA;
  *PORTM_CTRL = ((*PORTM_CTRL & 0xFF00 ) | 0x00AA)  ^ smask;
  delay();
  *PORTB = 0xFF;
  *PORTM = (*PORTM & 0xF0 ) | cmask;
}

delay();
result = (~(*PORTA))>>column & 1;
delay();
*PORTB_CTRL = 0xAAAA;
*PORTM_CTRL = (*PORTM_CTRL & 0xFF00 ) | 0x00AA;
delay();
*PORTB_CTRL = 0x5555;
*PORTM_CTRL = (*PORTM_CTRL & 0xFF00 ) | 0x0055;
delay();

return result;
}

unsigned char KeyDown(unsigned char keycode)
{
unsigned short key[8];
const unsigned short* keyboardregister = (unsigned short*)0xA44B0000;
if(isOS2)
{
  unsigned char row = keycode%10;
  memcpy(key, keyboardregister, sizeof(unsigned short) << 3);
 
  return (0 != (key[row >> 1] & 1 << keycode / 10 - 1 + ((row & 1) << 3)));
}
else
{
  return CheckKeyRow((keycode % 10) + ((keycode / 10 - 1) << 4));
}
}
unsigned char GetKeyMod(unsigned int *key)
{
unsigned char x, ret;

ret = GetKey(key);

for(x = 0; x < 80; x++)
{
  if(KeyDown(x))
  {
   *key = x;
   break;
  }
}
return ret;
}
#endif
#endif

//****************************************************************************
//  AddIn_main (Sample program main function)
//
//  param   :   isAppli   : 1 = This application is launched by MAIN MENU.
//                        : 0 = This application is launched by a strip in
//                              eACT application.
//
//              OptionNum : Strip number (0~3)
//                         (This parameter is only used when isAppli parameter is 0.)
//
//  retval  :   1 = No error / 0 = Error
//
//****************************************************************************

// 7 pieces, each with 4 rotation states, each occupying a 4*4 space
// Looks not like the SRS table but trust me it is
const unsigned int rotationTable[7][4][4][4] = {
    {  // I piece
        { // State 0
        {0,0,1,0},
        {0,0,1,0},
        {0,0,1,0},
        {0,0,1,0}
        },
        { // State 1
        {0,0,0,0},
        {0,0,0,0},
        {1,1,1,1},
        {0,0,0,0}
        },
        { // State 2
        {0,1,0,0},
        {0,1,0,0},
        {0,1,0,0},
        {0,1,0,0}
        },
        { // State 3
        {0,0,0,0},
        {1,1,1,1},
        {0,0,0,0},
        {0,0,0,0}
        }
    },    
    {  // J piece
        { // State 0
        {0,0,1,1},
        {0,0,1,0},
        {0,0,1,0},
        {0,0,0,0}
        },
        { // State 1
        {0,0,0,0},
        {0,1,1,1},
        {0,0,0,1},
        {0,0,0,0}
        },
        { // State 2
        {0,0,1,0},
        {0,0,1,0},
        {0,1,1,0},
        {0,0,0,0}
        },
        { // State 3
        {0,1,0,0},
        {0,1,1,1},
        {0,0,0,0},
        {0,0,0,0}
        }
    },    
    {  // L piece
        { // State 0
        {0,0,1,0},
        {0,0,1,0},
        {0,0,1,1},
        {0,0,0,0}
        },
        { // State 1
        {0,0,0,0},
        {0,1,1,1},
        {0,1,0,0},
        {0,0,0,0}
        },
        { // State 2
        {0,1,1,0},
        {0,0,1,0},
        {0,0,1,0},
        {0,0,0,0}
        },
        { // State 3
        {0,0,0,1},
        {0,1,1,1},
        {0,0,0,0},
        {0,0,0,0}
        }
    },    
    {  // O piece
        { // State 0
        {0,0,0,0},
        {0,0,1,1},
        {0,0,1,1},
        {0,0,0,0}
        },
        { // State 1
        {0,0,0,0},
        {0,0,1,1},
        {0,0,1,1},
        {0,0,0,0}
        },
        { // State 2
        {0,0,0,0},
        {0,0,1,1},
        {0,0,1,1},
        {0,0,0,0}
        },
        { // State 3
        {0,0,0,0},
        {0,0,1,1},
        {0,0,1,1},
        {0,0,0,0}
        }
    },    
    {  // S piece
        { // State 0
        {0,0,1,0},
        {0,0,1,1},
        {0,0,0,1},
        {0,0,0,0}
        },
        { // State 1
        {0,0,0,0},
        {0,0,1,1},
        {0,1,1,0},
        {0,0,0,0}
        },
        { // State 2
        {0,1,0,0},
        {0,1,1,0},
        {0,0,1,0},
        {0,0,0,0}
        },
        { // State 3
        {0,0,1,1},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
        }
    },    
    {  // T piece
        { // State 0
        {0,0,1,0},
        {0,0,1,1},
        {0,0,1,0},
        {0,0,0,0}
        },
        { // State 1
        {0,0,0,0},
        {0,1,1,1},
        {0,0,1,0},
        {0,0,0,0}
        },
        { // State 2
        {0,0,1,0},
        {0,1,1,0},
        {0,0,1,0},
        {0,0,0,0}
        },
        { // State 3
        {0,0,1,0},
        {0,1,1,1},
        {0,0,0,0},
        {0,0,0,0}
        }
    },    
    {  // Z piece
        { // State 0
        {0,0,0,1},
        {0,0,1,1},
        {0,0,1,0},
        {0,0,0,0}
        },
        { // State 1
        {0,0,0,0},
        {0,1,1,0},
        {0,0,1,1},
        {0,0,0,0}
        },
        { // State 2
        {0,0,1,0},
        {0,1,1,0},
        {0,1,0,0},
        {0,0,0,0}
        },
        { // State 3
        {0,1,1,0},
        {0,0,1,1},
        {0,0,0,0},
        {0,0,0,0}
        }
    }
};

// I / other pieces (2), with 4*4 possible rotations, each with four kicks
// A "kick" looks like [x][y], where x and y are offset positions
// Not including basic rotation ( (0,0) kick )
const int kickTable[2][4][4][5][2] = {
    {  // I piece
        {  // From state 0
            { { 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0} },  // 0 -> 0
            { {-2, 0},{ 1, 0},{-2,-1},{ 1, 2},{ 0, 0} },  // 0 -> 1
            { {-1, 0},{-2, 0},{ 1, 0},{ 2, 0},{ 0, 1} },  // 0 -> 2
            { {-1, 0},{ 2, 0},{-1, 2},{ 2,-1},{ 0, 0} }   // 0 -> 3
        },
        {  // From state 1
            { { 2, 0},{-1, 0},{ 2, 1},{-1,-2},{ 0, 0} },  // 1 -> 0
            { { 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0} },  // 1 -> 1
            { {-1, 0},{ 2, 0},{-1, 2},{ 2,-1},{ 0, 0} },  // 1 -> 2
            { { 0, 1},{ 0, 2},{ 0,-1},{ 0,-2},{-1, 0} }   // 1 -> 3
        },
        {  // From state 2
            { { 1, 0},{ 2, 0},{-1, 0},{-2, 0},{ 0,-1} },  // 2 -> 0
            { { 1, 0},{-2, 0},{ 1,-2},{-2, 1},{ 0, 0} },  // 2 -> 1
            { { 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0} },  // 2 -> 2
            { { 2, 0},{-1, 0},{ 2, 1},{-1,-2},{ 0, 0} }   // 2 -> 3
        },
        {  // From state 3
            { { 1, 0},{-2, 0},{ 1,-2},{-2, 1},{ 0, 0} },  // 3 -> 0
            { { 0, 1},{ 0, 2},{ 0,-1},{ 0,-2},{ 1, 0} },  // 3 -> 1
            { {-2, 0},{ 1, 0},{-2,-1},{ 1, 2},{ 0, 0} },  // 3 -> 2
            { { 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0} }   // 3 -> 3
        }
    }, 
    {  // Other pieces
        {  // From state 0
            { { 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0} },  // 0 -> 0
            { {-1, 0},{-1, 1},{ 0,-2},{-1,-2},{ 0, 0} },  // 0 -> 1
            { { 0, 1},{ 1, 1},{-1, 1},{ 1, 0},{-1, 0} },  // 0 -> 2
            { { 1, 0},{ 1, 1},{ 0,-2},{ 1,-2},{ 0, 0} }   // 0 -> 3
        },
        {  // From state 1
            { { 1, 0},{ 1,-1},{ 0, 2},{ 1, 2},{ 0, 0} },  // 1 -> 0
            { { 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0} },  // 1 -> 1
            { { 1, 0},{ 1,-1},{ 0, 2},{ 1, 2},{ 0, 0} },  // 1 -> 2
            { { 1, 0},{ 1, 2},{ 1, 1},{ 0, 2},{ 0, 1} }   // 1 -> 3
        },
        {  // From state 2
            { { 0,-1},{-1,-1},{ 1,-1},{-1, 0},{ 1, 0} },  // 2 -> 0
            { {-1, 0},{-1, 1},{ 0,-2},{-1,-2},{ 0, 0} },  // 2 -> 1
            { { 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0} },  // 2 -> 2
            { { 1, 0},{ 1, 1},{ 0,-2},{ 1,-2},{ 0, 0} }   // 2 -> 3
        },
        {  // From state 3
            { {-1, 0},{-1,-1},{ 0, 2},{-1, 2},{ 0, 0} },  // 3 -> 0
            { {-1, 0},{-1, 2},{-1, 1},{ 0, 2},{ 0, 1} },  // 3 -> 1
            { {-1, 0},{-1,-1},{ 0, 2},{-1, 2},{ 0, 0} },  // 3 -> 2
            { { 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0} }   // 3 -> 3
        }
    }
};

#define FPS 20

#define I_PIECE 0
#define J_PIECE 1
#define L_PIECE 2
#define O_PIECE 3
#define S_PIECE 4
#define T_PIECE 5
#define Z_PIECE 6

#define ENDLESS_MODE 1
#define SPRINT_MODE 2
#define ULTRA_MODE 3

#define SPAWN_X 3
#define SPAWN_Y 18

#define ROTATE_CW_KEY KEY_CHAR_8
#define ROTATE_ACW_KEY KEY_CHAR_7
#define ROTATE_180_KEY KEY_CHAR_4
#define HOLD_KEY KEY_CHAR_9
#define SOFT_DROP_KEY KEY_CTRL_DOWN
#define HARD_DROP_KEY KEY_CHAR_5

#define FRAME_TIMER_ID ID_USER_TIMER1
#define CLEARTEXT_TIMER_ID ID_USER_TIMER2

#define NEXT_QUEUE_SIZE 5
#define CLEARTEXT_DURATION 1000  // ms

#define DAS 3  // frames
#define SDF 10.0f
#define LOCK_DELAY 10  // frames
#define FORCE_LOCK_MOVES 15
#define LINES_PER_LEVEL 10
#define MAX_LEVEL 15

int ax = SPAWN_X;
int ay = SPAWN_Y;
int aPiece = 0;
int aState = 0;
int holdPiece = -1;
int usedHold = 0;

int moveInputFrames = 0;
int currentMoveDir = 0;  // -1, 0, 1 each mean left, none, right

int holdingRotateCW = 0;
int holdingRotateACW = 0;
int holdingRotate180 = 0;
int holdingHold = 0;
int holdingHardDrop = 0;
int rotatedBefore = 0;
int isB2B = 0;
int combo = 0;

float gravityCounter = 0.0f;
int lockDelayCounter = 0;
int forceLockCounter = 0;

int score = 0;
int linesTotal = 0;
int framesPassed = 1;
int level = 1;

int board[10][40] = { 0 };
int nextQueue[NEXT_QUEUE_SIZE] = { 0 };
int bag[7] = { I_PIECE, J_PIECE, L_PIECE, O_PIECE, S_PIECE, T_PIECE, Z_PIECE };
int bagSize = 7;

int isGameActive = 1;
int isAtMenu = 0;

int gameMode = 0;

// powf() doesn't work
float powFI(float f, unsigned int i) {
    float r = 1.0f;
    int j = 0;
    for(; j<i; ++j) r *= f;
    return r;
}

unsigned int rSeed = 12345;
int randInt(int min, int max) {
    int randomNum;
    // LCG parameters from Numerical Recipes
    rSeed = 1664525 * rSeed + 1013904223;
    
    // Generate a random number between min and max (inclusive)
    randomNum = (rSeed % (max - min + 1)) + min;
    return randomNum;
}

// Concatenates two strings
void strcat(char* dest, const char* src) {
    while(*dest) ++dest;
    while(*src) {
        *dest = *src;
        ++dest;
        ++src;
    }
    *dest = '\0';
}

// With 7-bag system
int getPiece() {

    int randomIndex;
    int i = 0;
    int temp;

    if (bagSize == 0) {
        // Refill bag
        bagSize = 7;
        for (i = 0; i < bagSize; i++) {
            bag[i] = i;
        }
    }

    // Swaps a random index with last index (bagSize-1)
    // "Used" pieces are shoved to the end of the bag

    randomIndex = randInt(0, bagSize - 1);

    temp = bag[randomIndex];
    bag[randomIndex] = bag[bagSize - 1];
    bag[bagSize - 1] = temp;

    bagSize--;

    return bag[bagSize];
}

int getScore(int level, int lines, int tSpin, int isB2B, int combo, int isPC) {
    int score = 0;
    switch(tSpin) {
        case 0:  // No T spin
            switch(lines) {
                case 1:
                    score += 100;
                    break;
                case 2:
                    score += 300;
                    break;
                case 3:
                    score += 500;
                    break;
                case 4:
                    score += isB2B ? 1200 : 800;
                    break;
            }
            break;
        case 1:  // Mini T spin
            switch(lines) {
                case 0:
                    score += 100;
                    break;
                case 1:
                    score += isB2B ? 300 : 200;
                    break;
                case 2:
                    score += isB2B ? 600 : 400;
                    break;
            }
            break;
        case 2:  // T spin
            switch(lines) {
                case 0:
                    score += 400;
                    break;
                case 1:
                    score += isB2B ? 1200 : 800;
                    break;
                case 2:
                    score += isB2B ? 1800 : 1200;
                    break;
                case 3:
                    score += isB2B ? 2400 : 1600;
                    break;
            }
            break;
    }
    // Combo
    if(combo > 0) {
        score += combo*50;
    }
    // Perfect clear
    if(isPC) {
        switch(lines) {
            case 1:
                score += 800;
                break;
            case 2:
                score += 1200;
                break;
            case 3:
                score += 1800;
                break;
            case 4:
                score += isB2B ? 2000 : 3200;
                break;
        }
    }

    score *= level;
    return score;
}

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

void ftoa(char* str, float f) {
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    int m = 0;
    int n = 0;
    int isNegative = 0;
    int intPart = (int)f;
    int fracPart = 0;
    float tempF = 0.0f;
    char tempC;
    if(f < 0.0f) {
        isNegative = 1;
        f = -f;
        intPart = -intPart;
    }
    fracPart = (f - (float)intPart) * 1000;
    itoa(str, intPart);
    while(str[i]) ++i;
    str[i++] = '.';
    itoa(str+i, fracPart);

}

// Only visual, does not change board data
void drawCell(int x, int y) {
    const int blX = 49+3*x;  // meaning "X of bottom left corner"
    const int blY = 62-3*y;
    Bdisp_DrawLineVRAM(blX,blY,blX,blY-2);
    Bdisp_DrawLineVRAM(blX+1,blY,blX+1,blY-2);
    Bdisp_DrawLineVRAM(blX+2,blY,blX+2,blY-2);
}

void drawGhostCell(int x, int y) {
    const int pX = 49+3*x +1;
    const int pY = 62-3*y -1;
    Bdisp_SetPoint_VRAM(pX, pY, 1);
}

// Only visual, does not change board data
void clearCell(int x, int y) {
    const int blX = 49+3*x;
    const int blY = 62-3*y;
    Bdisp_ClearLineVRAM(blX,blY,blX,blY-2);
    Bdisp_ClearLineVRAM(blX+1,blY,blX+1,blY-2);
    Bdisp_ClearLineVRAM(blX+2,blY,blX+2,blY-2);
}

void drawPiece(int x, int y, int pieceType, int state) {
    int i = 0;
    int j = 0;
    for(i = 0; i<4; ++i) {
        for(j = 0; j<4; ++j) {
            if(rotationTable[pieceType][state][i][j]) {
                drawCell(x+i, y+j);
            }
        }
    }
}

void drawNextQueue(int* queue) {
    int i = 0;
    // Clear
    for(i = 81; i<96; ++i) {
        Bdisp_ClearLineVRAM(i, 8, i, 51);
    }
    // Draw
    for(i = 0; i<NEXT_QUEUE_SIZE; ++i) {
        drawPiece(11, 14-3*i, queue[i], 0);
    }
}

void drawHold(int piece) {
    int i = 0;
    // Clear
    for(i = 32; i<47; ++i) {
        Bdisp_ClearLineVRAM(i, 8, i, 14);
    }
    // Draw
    if(piece != -1) drawPiece(-5, 14, piece, 0);
}

void drawLockDelay(int frames) {
    if(!frames) {
        Bdisp_ClearLineVRAM(46, 61-2*LOCK_DELAY, 46, 61);
    }
    else {
        Bdisp_DrawLineVRAM(46, 61, 46, 61-2*LOCK_DELAY);
        Bdisp_ClearLineVRAM(46, 61-2*LOCK_DELAY, 46, 61-2*LOCK_DELAY+2*frames);
    }
}

void drawScore(int score) {
    int i;
    char str[20];
    for (i = 20; i <= 24; i++) {
        Bdisp_ClearLineVRAM(3, i, 47, i);
    }
    itoa(str, score);
    PrintMini(3, 20, str, 0);
}

void drawLinesTotal(int lines, int objective) {
    int i;
    char strA[20];
    char strB[20];
    for (i = 34; i <= 38; i++) {
        Bdisp_ClearLineVRAM(3, i, 47, i);
    }
    itoa(strA, lines);
    if(objective) {
        itoa(strB, objective);
        strcat(strA, " / ");
        strcat(strA, strB);
    }
    PrintMini(3, 34, strA, 0);
}

void drawTime(int framesPassed) {
    int i;
    char strA[20];
    char strB[10];
    int minutes = (framesPassed / FPS) / 60;
    int seconds = (framesPassed / FPS) % 60;
    int deciSeconds = (framesPassed/2) % 10;
    for (i = 48; i <= 52; i++) {
        Bdisp_ClearLineVRAM(3, i, 45, i);
    }
    itoa(strA, minutes);
    strcat(strA, ":");
    if(seconds < 10) strcat(strA, "0");
    itoa(strB, seconds);
    strcat(strA, strB);
    strcat(strA, ".");
    itoa(strB, deciSeconds);
    strcat(strA, strB);
    PrintMini(3, 48, strA, 0);
}

void drawLevel(int level) {
    int i;
    char strA[20] = "LEVEL ";
    char strB[10];
    for (i = 56; i <= 60; i++) {
        Bdisp_ClearLineVRAM(3, i, 47, i);
    }
    itoa(strB, level);
    strcat(strA, strB);
    PrintMini(3, 56, strA, 0);
}

void clearLineClearText() {
    int i;
    for (i = 18; i <= 53; i++) {
        Bdisp_ClearLineVRAM(99, i, 127, i);
    }
}

void drawLineClearText(int lines, int tSpin, int isB2B, int combo, int isPC) {
    char strA[20] = "+";
    char strB[20];
    KillTimer(CLEARTEXT_TIMER_ID);
    switch(lines) {
        case 1:
            PrintMini(99, 30, "SINGLE", 0);
            break;
        case 2:
            PrintMini(99, 30, "DOUBLE", 0);
            break;
        case 3:
            PrintMini(99, 30, "TRIPLE", 0);
            break;
        case 4:
            PrintMini(99, 30, "QUAD", 0);
            break;
        default:
            break;
    }
    switch(tSpin) {
        case 1:
            PrintMini(99, 24, "MTSPIN", 0);
            break;
        case 2:
            PrintMini(99, 24, "TSPIN", 0);
            break;
        default:
            break;
    }
    if(isB2B && lines) {
        PrintMini(99, 18, "B2B", 0);
    }
    if(combo > 0 && lines) {
        char str[10];
        itoa(str, combo);
        strcat(str, " CMB");
        PrintMini(99, 36, str, 0);
    }
    if(isPC && lines) {
        PrintMini(99, 42, "PF.CLR", 0);
    }
    if(gameMode != SPRINT_MODE) {
        itoa(strB, getScore(level, lines, tSpin, isB2B, combo, isPC));
        strcat(strA, strB);
        PrintMini(99, 48, strA, 0);
    }
    SetTimer(CLEARTEXT_TIMER_ID, CLEARTEXT_DURATION, clearLineClearText);
}

// "int y" is for the actual piece, not the ghost
void drawGhost(int x, int y, int pieceType, int state) {
    int newY = getGhostY(x, y, pieceType, state);
    int i = 0;
    int j = 0;
    for(i = 0; i<4; ++i) {
        for(j = 0; j<4; ++j) {
            if(rotationTable[pieceType][state][i][j]) {
                drawGhostCell(x+i, newY+j);
            }
        }
    }
}

// "int y" is for the actual piece, not the ghost
void clearGhost(int x, int y, int pieceType, int state) {
    int newY = getGhostY(x, y, pieceType, state);
    clearPiece(x, newY, pieceType, state);
}

int clearPiece(int x, int y, int pieceType, int state) {
    int i = 0;
    int j = 0;
    for(i = 0; i<4; ++i) {
        for(j = 0; j<4; ++j) {
            if(rotationTable[pieceType][state][i][j]) {
                clearCell(x+i, y+j);
            }
        }
    }
}

// Only visual, does not change board data
void movePiece(int x, int y, int dx, int dy, int pieceType, int state) {
    clearPiece(x, y, pieceType, state);
    drawPiece(x+dx, y+dy, pieceType, state);
}

// Only visual, does not change board data
void rotatePiece(int x, int y, int newX, int newY,
    int pieceType, int state, int newState) {

    clearPiece(x, y, pieceType, state);
    drawPiece(newX, newY, pieceType, newState);
}

int getCell(int x, int y) {
     return x<0 || x>9 || y<0 || y>39 || board[x][y];
}

int getGhostY(int x, int y, int pieceType, int state) {
    int newY = y;
    while(pieceFits(x, newY, pieceType, state)) {
        --newY;
    }
    return newY+1;
}

int pieceFits(int x, int y, int pieceType, int state) {
    int i = 0;
    int j = 0;
    for(i = 0; i<4; ++i) {
        for(j = 0; j<4; ++j) {
            if(rotationTable[pieceType][state][i][j]) {
                if(getCell(x+i, y+j)) return 0;
            }
        }
    }
    return 1;
}

int checkPerfectClear() {
    int i = 0;
    int j = 0;
    for(i = 0; i<10; ++i) {
        for(j = 0; j<40; ++j) {
            if(board[i][j]) return 0;
        }
    }
    return 1;
}

// Only modifies board data, not visuals
void lockPiece(int x, int y, int pieceType, int state) {
    int i = 0;
    int j = 0;
    for(i = 0; i<4; ++i) {
        for(j = 0; j<4; ++j) {
            if(rotationTable[pieceType][state][i][j]) {
                board[x+i][y+j] = 1;
            }
        }
    }
}

// Returns kick used, -1 if none or failed
int attemptRotation(int x, int y, int pieceType, int state, int newState) {
    int i = 0;
    int isNotIPiece = pieceType != I_PIECE;

    if(pieceFits(ax, ay, aPiece, newState)) {
        clearGhost(ax, ay, aPiece, aState);
        rotatePiece(ax, ay, ax, ay, aPiece, aState, newState);
        drawGhost(ax, ay, aPiece, newState);
        aState = newState;
        if(forceLockCounter < FORCE_LOCK_MOVES) {
            lockDelayCounter = 0;
            ++forceLockCounter;
        }
        rotatedBefore = 1;
        return -1;
    }
    // SRS kick
    for(i = 0; i<5; ++i) {
        if(pieceFits(
            ax+kickTable[isNotIPiece][aState][newState][i][0],
            ay+kickTable[isNotIPiece][aState][newState][i][1],
            aPiece, newState) ) {
            
            clearGhost(ax, ay, aPiece, aState);
            rotatePiece(ax, ay,
                ax+kickTable[isNotIPiece][aState][newState][i][0],
                ay+kickTable[isNotIPiece][aState][newState][i][1],
                aPiece, aState, newState);
            ax += kickTable[isNotIPiece][aState][newState][i][0];
            ay += kickTable[isNotIPiece][aState][newState][i][1];
            aState = newState;
            drawGhost(ax, ay, aPiece, aState);
            if(forceLockCounter < FORCE_LOCK_MOVES) {
                lockDelayCounter = 0;
                ++forceLockCounter;
            }
            rotatedBefore = 1;
            return i;
        }
    }
    return -1;
}

// Clears lines, redraws board
// Return line clears
int updateBoard() {
    int i = 0;
    int j = 0;
    int k = 0;
    int rowFull = 0;
    int lines = 0;
    
    // Clear rows loop
    for(j = 0; j<40; ++j) {
        rowFull = 1;
        for(i = 0; i<10; ++i) {
            // Not using getCell() because unneeded out of bounds check
            if(!board[i][j]) {
                rowFull = 0;
                break;
            }
        }
        if(rowFull) {
            ++lines;
            // Shift rows down
            for(k = j; k<39; ++k) {
                for(i = 0; i<10; ++i) {
                    board[i][k] = board[i][k+1];
                }
            }
            // Empty top row
            for(k = 0; k<10; ++k) {
                board[k][39] = 0;
            }
            --j;
        }
    }
    
    // Redraw board loop
    for(i = 0; i<10; ++i) {
        for(j = 0; j<40; ++j) {
            if(board[i][j]) {
                drawCell(i,j);
            }
            else {
                clearCell(i,j);
            }
        }
    }
    return lines;
}

// Main game logic
// 20 FPS as of now
// Controls:
//   Left, Right: Movement
//   Down: Soft drop
//   9: Hold
//   7, 8: Rotate ACW/CW
//   5: Hard drop
//   4: Rotate 180
int frame() {
    int key = 0;
    int usedTSpinKick = 0;
    if(KeyDown(KEY_CTRL_EXIT) || KeyDown(KEY_CTRL_MENU)) {
        KillTimer(FRAME_TIMER_ID);
        if(KeyDown(KEY_CTRL_MENU)) {
            Bdisp_AllClr_DDVRAM();
            isGameActive = 0;
            PopUpWin(3);
            locate(8,3);
            Print("STOPPED");
            locate(5,5);
            Print("Press any key");
            Bdisp_PutDisp_DD();
        }
        else {
            mainMenu(1);
        }
        return 1;
    }
    isGameActive = 1;
    
    SetTimer(ID_USER_TIMER1, 1000/FPS, frame);
    
    // Hold
    if(KeyDown(HOLD_KEY) && !holdingHold && !usedHold) {
        clearPiece(ax, ay, aPiece, aState);
        clearGhost(ax, ay, aPiece, aState);
        ax = SPAWN_X;
        ay = SPAWN_Y;
        aState = 0;
        gravityCounter = 0;
        lockDelayCounter = 0;
        forceLockCounter = 0;
        rotatedBefore = 0;
        if(holdPiece == -1) {
            int i = 0;
            holdPiece = aPiece;
            aPiece = nextQueue[0];
            // Shift queue
            for(i = 0; i<NEXT_QUEUE_SIZE-1; ++i) {
                nextQueue[i] = nextQueue[i+1];
            }
            nextQueue[NEXT_QUEUE_SIZE-1] = getPiece();
            drawNextQueue(nextQueue);
        }
        else {
            int temp = aPiece;
            aPiece = holdPiece;
            holdPiece = temp;
        }
        usedHold = 1;
        drawHold(holdPiece);
        drawPiece(ax, ay, aPiece, aState);
        drawGhost(ax, ay, aPiece, aState);
        holdingHold = 1;
    }
    else {
        holdingHold = 0;
    }

    if(KeyDown(KEY_CTRL_LEFT)) {
        switch(currentMoveDir) {
            case -1:
                ++moveInputFrames;
                break;
            default:
                moveInputFrames = 1;
                currentMoveDir = -1;
        }
    }
    else if(KeyDown(KEY_CTRL_RIGHT)) {
        switch(currentMoveDir) {
            case 1:
                ++moveInputFrames;
                break;
            default:
                moveInputFrames = 1;
                currentMoveDir = 1;
        }
    }
    else {
        moveInputFrames = 0;
        currentMoveDir = 0;
    }
    if( (moveInputFrames == 1 || moveInputFrames > DAS+1)
        && pieceFits(ax+currentMoveDir, ay, aPiece, aState) ) {
        
        clearGhost(ax, ay, aPiece, aState);
        movePiece(ax, ay, currentMoveDir, 0, aPiece, aState);
        ax += currentMoveDir;
        drawGhost(ax, ay, aPiece, aState);
        if(forceLockCounter < FORCE_LOCK_MOVES) {
            lockDelayCounter = 0;
            if(moveInputFrames == 1) {
                ++forceLockCounter;
            }
        }
        rotatedBefore = 0;
    }

    if(KeyDown(ROTATE_CW_KEY)) {
        if(!holdingRotateCW) {
            int newState = (aState+1)%4;
            holdingRotateCW = 1;
            if(attemptRotation(ax, ay, aPiece, aState, newState) == 3 &&
                (newState == 1 || newState == 3)) {  // TST kick or FIN kick

                usedTSpinKick = 1;
            };
        }
    }
    else {
        holdingRotateCW = 0;
    }
    if(KeyDown(ROTATE_ACW_KEY)) {
        if(!holdingRotateACW) {
            int newState = (aState+3)%4;
            holdingRotateACW = 1;
            if(attemptRotation(ax, ay, aPiece, aState, newState) == 3 &&
                (newState == 1 || newState == 3)) {  // TST kick or FIN kick

                usedTSpinKick = 1;
            };
        }
    }
    else {
        holdingRotateACW = 0;
    }
    if(KeyDown(ROTATE_180_KEY)) {
        if(!holdingRotate180) {
            int newState = (aState+2)%4;
            holdingRotate180 = 1;
            attemptRotation(ax, ay, aPiece, aState, newState);
        }
    }
    else {
        holdingRotate180 = 0;
    }

    // Hard drop
    if(KeyDown(HARD_DROP_KEY) && !holdingHardDrop) {
        gravityCounter = 1.0f;
        lockDelayCounter = LOCK_DELAY;
        holdingHardDrop = 1;
        while(pieceFits(ax,ay-1,aPiece,aState)) {
            movePiece(ax, ay, 0, -1, aPiece, aState);
            --ay;
            score += 2;
            rotatedBefore = 0;
        }
        if(gameMode != SPRINT_MODE) drawScore(score);
    }
    else if(!KeyDown(HARD_DROP_KEY)){
        holdingHardDrop = 0;
    }

    gravityCounter += ( 1.0f / ((float)FPS) /
        powFI( (0.8f-((level-1)*0.007f)),(level-1) ) )
        * (KeyDown(SOFT_DROP_KEY)?SDF:1.0f);

    if(!pieceFits(ax,ay-1,aPiece,aState)) {
        if(lockDelayCounter < LOCK_DELAY) {
            ++lockDelayCounter;
            gravityCounter = 1.0f;
        }
        else {
            // Lock piece
            int i = 0;
            int lines = 0;
            int tSpin = 0;  // 0: no, 1: mini, 2: regular
            int isPerfectClear = 0;
            usedHold = 0;
            lockDelayCounter = 0;
            forceLockCounter = 0;

            // T-spin check
            if(aPiece == T_PIECE && rotatedBefore) {
                int NE = getCell(ax+1 +1, ay+1 +2);
                int NW = getCell(ax-1 +1, ay+1 +2);  //  NW ■ NE
                int SE = getCell(ax+1 +1, ay-1 +2);  //  ■  ■  ■
                int SW = getCell(ax-1 +1, ay-1 +2);  //  SW ■ SE
                if(NE+NW+SE+SW >= 3) {
                    if(usedTSpinKick) {
                        tSpin = 2;
                    }
                    else {
                        switch(aState) {
                            case 0:
                                tSpin = (NW&&NE)+1;
                                break;
                            case 1:
                                tSpin = (NE&&SE)+1;
                                break;
                            case 2:
                                tSpin = (SE&&SW)+1;
                                break;
                            case 3:
                                tSpin = (SW&&NW)+1;
                                break;
                        }
                    }
                }
            }

            // Spawn new piece
            lockPiece(ax, ay, aPiece, aState);
            ax = SPAWN_X;
            ay = SPAWN_Y;
            aPiece = nextQueue[0];
            // Shift queue
            for(i = 0; i<NEXT_QUEUE_SIZE-1; ++i) {
                nextQueue[i] = nextQueue[i+1];
            }
            nextQueue[NEXT_QUEUE_SIZE-1] = getPiece();
            drawNextQueue(nextQueue);

            aState = 0;
            lines = updateBoard();
            isPerfectClear = lines && checkPerfectClear();
            if(lines || tSpin) {
                if(!tSpin && lines < 4 && lines > 0) {
                    isB2B = 0;
                }
                drawLineClearText(lines, tSpin, 
                    lines?isB2B:0, lines?combo:0, lines?isPerfectClear:0);
                score += getScore(level, lines, tSpin, 
                    lines?isB2B:0, lines?combo:0, lines?isPerfectClear:0);
                if(gameMode != SPRINT_MODE) drawScore(score);
            }
            if(lines) {
                // B2B check
                if(tSpin || lines == 4) {
                    isB2B = 1;
                }
                else {
                    isB2B = 0;
                }
                ++combo;
                linesTotal += lines;
                if(gameMode == SPRINT_MODE) drawLinesTotal(linesTotal, 40);
                // Level up
                if(gameMode==ENDLESS_MODE && level*LINES_PER_LEVEL<=linesTotal
                    && level<MAX_LEVEL) {
                    ++level;
                    drawLevel(level);
                }
            }
            else {
                combo = 0;
            }
            drawPiece(ax, ay, aPiece, aState);
            drawGhost(ax, ay, aPiece, aState);
            if(!pieceFits(ax,ay,aPiece,aState)) {
                char str[20];
                // Top out
                PopUpWin(3);
                locate(8,3);
                Print("TOP OUT");
                locate(6,4);
                if(gameMode==ENDLESS_MODE){
                    Print("Score: ");
                    itoa(str, score);
                    PrintMini(70, 25, str, 0);
                }
                locate(7,5);
                Print("Press EXE");
                endGame();
                return 1;
            }
            gravityCounter = 0;
        }
    }
    else {  // Can go down
        // Apply gravity
        while(gravityCounter >= 1.0f && pieceFits(ax,ay-1,aPiece,aState)) {
            movePiece(ax, ay, 0, -1, aPiece, aState);
            --ay;
            gravityCounter -= 1.0f;
            lockDelayCounter = 0;
            rotatedBefore = 0;
            if(KeyDown(SOFT_DROP_KEY)) {
                score += 1;
                if(gameMode != SPRINT_MODE) drawScore(score);
            }
        }
    }
    drawLockDelay(lockDelayCounter);
    ++framesPassed;
    if(gameMode != ENDLESS_MODE)drawTime(framesPassed);
    // Check if objective reached
    if(gameMode == SPRINT_MODE && linesTotal >= 40) {
        char strA[20];
        char strB[10];
        int minutes = (framesPassed / FPS) / 60;
        int seconds = (framesPassed / FPS) % 60;
        int deciSeconds = (framesPassed/2) % 10;
        itoa(strA, minutes);
        strcat(strA, ":");
        if(seconds < 10) strcat(strA, "0");
        itoa(strB, seconds);
        strcat(strA, strB);
        strcat(strA, ".");
        itoa(strB, deciSeconds);
        strcat(strA, strB);

        PopUpWin(3);
        locate(8,3);
        Print("COMPLETE");
        locate(5,4);
        Print("Time: ");
        locate(11,4);
        Print(strA);
        locate(7,5);
        Print("Press EXE");
        endGame();
        return 1;
    }
    else if(gameMode == ULTRA_MODE && framesPassed >= 120*FPS) {
        char strA[20];
        PopUpWin(3);
        locate(8,3);
        Print("COMPLETE");
        locate(6,4);
        Print("Score: ");
        itoa(strA, score);
        PrintMini(70, 25, strA, 0);
        locate(7,5);
        Print("Press EXE");
        endGame();
        return 1;
    }

    Bdisp_PutDisp_DD();
    return 1;
}

int mainMenu(int updateScreen) {
    int key = 0;

    if(isAtMenu) return 1;
    isAtMenu = 1;

    Bdisp_AllClr_DDVRAM();
    locate(3,2);
    Print("TETRIS");
    locate(3,5);
    Print("1: Endless");
    locate(3,6);
    Print("2: 40L Sprint");
    locate(3,7);
    Print("3: 2min Ultra");
    if(updateScreen) Bdisp_PutDisp_DD();

    while(1) {
        GetKeyMod(&key);
        if(key == KEY_CHAR_1) {
            isAtMenu = 0;
            startGame(ENDLESS_MODE);
            return 1;
        }
        else if(key == KEY_CHAR_2) {
            isAtMenu = 0;
            startGame(SPRINT_MODE);
            return 1;
        }
        else if(key == KEY_CHAR_3) {
            isAtMenu = 0;
            startGame(ULTRA_MODE);
            return 1;
        }
        else if(key == KEY_CTRL_MENU) {
            isGameActive = 0;
            isAtMenu = 0;
            return 1;
        }
    }
    isAtMenu = 0;
    return 1;
}

int endGame() {
    int menuPressed = 0;

    Bdisp_PutDisp_DD();
    KillTimer(ID_USER_TIMER1);

    while(KeyDown(KEY_CTRL_MENU)) {
        ++rSeed;  // Wait for menu release
    }

    while(!KeyDown(KEY_CTRL_EXE) && !KeyDown(KEY_CTRL_MENU)) {
        ++rSeed; // At this point this is just "do nothing"
        menuPressed = KeyDown(KEY_CTRL_MENU);
    }
    mainMenu(!menuPressed);
    return 1;
}

// Idk why but compiler is unhappy with void return
int startGame(int mode) {

    int i = 0;
    int j = 0;
    int menuPressed = 0;
    char testStr[80];

    Bdisp_AllClr_DDVRAM();

    // Wait till EXE is released
    while(KeyDown(KEY_CTRL_EXE)) {
        ++rSeed;
    }

    locate(3,2);
    Print("Hold: 9");
    locate(3,3);
    Print("Soft/Hard drop:");
    locate(3,4);
    Print("  Down/5");
    locate(3,5);
    Print("Rotate 180/CW/ACW:");
    locate(3,6);
    Print("  4/7/8");
    locate(3,7);
    Print("EXE to start");

    Bdisp_PutDisp_DD();

    //char str[800];
    //int k = 0;
    
    
    //GetKey(&k);
    //k = 3;
    //sprintf(str,"%d", k);
    //locate(1,4);
    //Print((unsigned char*)str);

    ax = SPAWN_X;
    ay = SPAWN_Y;
    //aPiece = getPiece();
    aState = 0;
    holdPiece = -1;
    usedHold = 0;

    moveInputFrames = 0;
    currentMoveDir = 0;  // -1, 0, 1 each mean left, none, right

    holdingRotateCW = 0;
    holdingRotateACW = 0;
    holdingRotate180 = 0;
    holdingHold = 0;
    holdingHardDrop = 0;
    rotatedBefore = 0;
    isB2B = 0;
    combo = 0;

    gravityCounter = 0.0f;
    lockDelayCounter = 0;
    forceLockCounter = 0;
    score = 0;
    linesTotal = 0;
    framesPassed = 0;
    level = (mode==ENDLESS_MODE)?1:5;

    isGameActive = 1;
    gameMode = mode;

    // Clear board
    for(i=0; i<10; ++i) {
        for(j=0; j<40; ++j) {
            board[i][j] = 0;
        }
    }
    do {
        ++rSeed;
        menuPressed = KeyDown(KEY_CTRL_MENU);
    } while(!KeyDown(KEY_CTRL_EXE) && !menuPressed);
    
    if(menuPressed) {
        isGameActive = 0;
        mainMenu(0);
        return 1;
    }
    
    // Reset bag
    for(i=0; i<7; ++i) {
        bag[i] = i;
    }
    bagSize = 7;
    aPiece = getPiece();

    // Make new queue
    for(i=0; i<NEXT_QUEUE_SIZE; ++i) {
        nextQueue[i] = getPiece();
    }

    Bdisp_AllClr_DDVRAM();

    // draw the walls of the board
    Bdisp_DrawLineVRAM(48, 0,  48, 63);
    Bdisp_DrawLineVRAM(79, 0,  79, 63);
    Bdisp_DrawLineVRAM(48, 63, 79, 63);

    // Hold UI
    PrintMini(32, 1, "HOLD", 0);
    Bdisp_DrawLineVRAM(31, 7, 47, 7);
    Bdisp_DrawLineVRAM(31, 16, 47, 16);
    Bdisp_DrawLineVRAM(31, 8, 31, 15);
    drawHold(-1);
    
    // Next UI
    PrintMini(81,1,"NEXT",0);
    Bdisp_DrawLineVRAM(80, 7, 96,7 );
    Bdisp_DrawLineVRAM(96, 8, 96, 52);
    Bdisp_DrawLineVRAM(80, 52, 96, 52);
    drawNextQueue(nextQueue);

    switch(mode) {
        case ENDLESS_MODE:
            PrintMini(3, 14, "SCORE", 0);
            drawScore(0);
            drawLevel(1);
            break;
        case SPRINT_MODE:
            PrintMini(3, 28, "LINES", 0);
            drawLinesTotal(0, 40);
            PrintMini(3, 42, "TIME", 0);
            drawTime(0);
            break;
        case ULTRA_MODE:
            PrintMini(3, 14, "SCORE", 0);
            drawScore(0);
            PrintMini(3, 42, "TIME", 0);
            drawTime(0);
            break;
    }

    updateBoard();
    drawPiece(SPAWN_X, SPAWN_Y, aPiece, aState);
    drawGhost(SPAWN_X, SPAWN_Y, aPiece, aState);
    Bdisp_PutDisp_DD();
    frame();
    
    return 1;
}

int AddIn_main(int isAppli, unsigned short OptionNum)
{
    unsigned int key = 0;

    mainMenu(1);

    while(1){
        GetKeyMod(&key);
        if(!isGameActive) {
            isGameActive = 1;
            mainMenu(1);
        }
    }

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

