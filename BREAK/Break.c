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

////////////////////////////////////////

int isGameActive = 0;

float max(float a, float b) {
    return a > b ? a : b;
}
float min(float a, float b) {
    return a < b ? a : b;
}
int round(float a) {
    return (int)(a + 0.5f);
}
int floor(float a) {
    return (int)a;
}
int ceil(float a) {
    return (int)(a + 1.0f);
}
float fabs(float f) {
    return f < 0 ? -f : f;
}

typedef struct tag_Point {
    float x, y;
} Point;

typedef struct tag_IntPoint {
    int x, y;
} IntPoint;

// gets the X pos of line segment (x, y) -> (x+dx, y+dy) with line y=a
// yes the naming is confusing, but fuck you i guess
float intersectY(float x, float y, float dx, float dy, float a) {
    return x + dx * (a-y) / dy;
}
// gets the Y pos of line segment (x, y) -> (x+dx, y+dy) with line x=a
float intersectX(float x, float y, float dx, float dy, float a) {
    return y + dy * (a-x) / dx;
}

// sqrt of float using Newton-Raphson method
float sqrtf(float x) {
    float guess = x / 2.0f;
    float epsilon = 0.00001f; // Tolerance level
    if (x < 0) {
        return -1; // Return -1 for negative input as an error indicator
    }
    while (fabs(guess * guess - x) >= epsilon) {
        guess = (guess + x / guess) / 2.0f;
    }
    return guess;
}

// distance between points
float dist(Point p, Point q) {
    return sqrtf((p.x - q.x) * (p.x - q.x) + (p.y - q.y) * (p.y - q.y));
}

int rSeed = 10;

float randf() {
    rSeed = rSeed * 1103515245 + 12345;
    return (float)(rSeed & 0x7FFFFFFF) / (float)0x7FFFFFFF;
}

// checks if x is between a and b
int isBetween(float x, float a, float b) {
    if ((x >= a && x <= b) || (x >= b && x <= a)) {
        return 1;
    } else {
        return 0;
    }
}

#define FPS 30
#define OPS_PER_FRAME 6

#define MIN_PX 0.0f
#define MAX_PX 72.0f
#define PADDLE_Y 53.0f

// Nearest multiple of n starting with x
// greater if dir > 0, lesser if dir < 0
int nearestMultiple(int x, int n, int dir) {
    if(dir > 0) {
        return ceil(x/n)*n;
    } else {
        return floor(x/n)*n;
    }
}

int bricks[18][10];  // brick width 4, height 2

// made because ball kept hitting the exact corner of a hole
// hit the brick behind it then deleted the entire wall
int isHittable(int x, int y) {
    const int leftClear = x>0 && bricks[x-1][y]==0;
    const int rightClear = x<17 && bricks[x+1][y]==0;
    const int topClear = y==9 || bricks[x][y+1]==0;
    const int bottomClear = y==0 || bricks[x][y-1]==0;
    return leftClear || rightClear || topClear || bottomClear;
}

// returns index of colliding brick travelling from p to n
// returns (-1, -1) if there is no brick
// puts the point of collision to "Collision"
// puts whether it hit the horizontal (0) or vertical (1) edge of the brick to "side"

IntPoint getCollidingBrick(Point p, Point n, Point* Collision, int* side) {
    // get which brick n is in
    int x = floor(n.x/4);
    int y = floor(n.y/2);
    float dx = n.x - p.x;
    float dy = n.y - p.y;
    int brickTopY = y*2;
    int brickBottomY = y*2+2;
    int brickLeftX = x*4;
    int brickRightX = x*4+4; 
   
    Point xCol ; // collision of left / right side
    Point yCol;  // collision of top / bottom side

    IntPoint returnVal = {-1,-1};

    if(x < 0 || x >= 18 || y < 0 || y >= 10) {
        return returnVal;
    }
    if(!bricks[x][y]) {
        return returnVal;
    }
    returnVal.x = x;
    returnVal.y = y;
    // which side the ball hit?
    if(dx > 0) {  // moving right; check left wall
        xCol.x = brickLeftX;
        xCol.y = intersectX(p.x, p.y, dx, dy, brickLeftX);
    } else {  // moving left; check right wall
        xCol.x = brickRightX;
        xCol.y = intersectX(p.x, p.y, dx, dy, brickRightX);
    }
    if(dy < 0) {  // moving up; check bottom wall
        yCol.y = brickBottomY;
        yCol.x = intersectY(p.x, p.y, dx, dy, brickBottomY);
    } else {  // moving down; check top wall
        yCol.y = brickTopY;
        yCol.x = intersectY(p.x, p.y, dx, dy, brickTopY);
    }
    
    // use the one that's closer to p
    if(dist(p, xCol) < dist(p, yCol)) {
        *Collision = xCol;
        *side = 0;
    } else {
        *Collision = yCol;
        *side = 1;
    }

    // check if the brick is hittable
    // if not, instead hit the one that was supposed to be hit
    if(!isHittable(x, y)) {
        if(*side) {
            if(dy > 0) {
                returnVal.y--;
            } else {
                returnVal.y++;
            }
        } else {
            if(dx > 0) {
                returnVal.x--;
            } else {
                returnVal.x++;
            }
        }
    }


    return returnVal;

}

void drawWalls() {
    int i;
    for(i = 0; i < 5; i++) {
        Bdisp_DrawLineVRAM(i, 4-i, i, 59);  // left wall
        Bdisp_DrawLineVRAM(5, i, 76, i);  // top wall
        Bdisp_DrawLineVRAM(i+77, i, i+77, 59);  // right wall

    }
}


// Reminder: call BEFORE drawing ball
void drawPaddle(float px) {
    // height 58
    const int x = round(px) + 5;
    Bdisp_ClearLineVRAM(5, 5+PADDLE_Y, 76, 5+PADDLE_Y);
    Bdisp_DrawLineVRAM(x-4, 5+PADDLE_Y, x+3, 5+PADDLE_Y);

}

// bx, by is OLD POSITION of ball
void drawBall(Point bPos, Point nbPos) {
    Point dPos;// = { nbPos.x - bPos.x, nbPos.y - bPos.y };
    int x, y, nx, ny;
    dPos.x = nbPos.x - bPos.x;
    dPos.y = nbPos.y - bPos.y;
    bPos.x -= dPos.x*2; // offset so it's more visible
    bPos.y -= dPos.y*2;

    x = round(bPos.x-0.5) + 5;
    y = round(bPos.y-0.5) + 5;
    nx = round(nbPos.x-0.5) + 5;
    ny = round(nbPos.y-0.5) + 5;
    //Bdisp_SetPoint_VRAM(x, y, 0);
    //Bdisp_SetPoint_VRAM(nx, ny, 1);
    Bdisp_DrawLineVRAM(x, y, nx, ny);
}


void drawBricks() {
    int i, j;
    for(i = 0; i < 18; i++) {
        for(j = 0; j < 10; j++) {
            if(bricks[i][j]) {
                Bdisp_DrawLineVRAM(i*4+5, j*2+5, i*4+8, j*2+5);
                Bdisp_DrawLineVRAM(i*4+5, j*2+6, i*4+8, j*2+6);
            }
        }
    }
}

void itoa(int n, char* buffer) {
    int i = 0; 
    int j = 0;
    int isNegative = 0;

    if(n == 0) {
        buffer[0] = '0';
        buffer[1] = 0;
        return;
    }

    if(n < 0) {
        isNegative = 1;
        n = -n;
    }

    while(n > 0) {
        buffer[i++] = '0' + n % 10;
        n /= 10;
    }

    if(isNegative) {
        buffer[i++] = '-';
    }

    buffer[i] = 0;

    for(j = 0; j < i/2; j++) {
        char temp = buffer[j];
        buffer[j] = buffer[i-j-1];
        buffer[i-j-1] = temp;
    }
}


float px;  // paddle width 8
int pSpeedMode;  // ranging from 1 to 5, prevent floating point errors
float pSpeeds[6] = {0, 0.10, 0.15, 0.25, 0.4, 0.6};
float pSpeed;
int bricksHit = 0; // speed increases for 4th 8th 12th hit (3 times)
float bSpeedMultiplier = 1.0f;
int lives;
int score;
int respawnFrames = 0;  // -1 meaning alive

Point bPos;
Point bVel;


void drawUI() {
    // lives
    int i;
    char str[10];
    PrintMini(95, 9, "LIVES", 0);
    itoa(lives, str);
    PrintMini(95, 15, str, 0);
    // score
    PrintMini(95, 26, "SCORE", 0);
    itoa(score, str);
    PrintMini(95, 32, str, 0);
    // speed
    PrintMini(95, 43, "SPEED", 0);
    itoa(pSpeedMode, str);
    PrintMini(95, 49, str, 0);

}


// board size 72x55 (+extends 4 downwards)

void titleScreen();

void frame() {

    Point nbPos; // = { bPos.x + bVel.x, bPos.y + bVel.y };
    static int changeSpeedKeyDown = 0;
    Point collisionPoint;
    IntPoint collisionIndex;
    int collisionSide;
    int i = 0;
    Point bPosGraphic; // = { bPos.x, bPos.y };
    bPosGraphic.x = bPos.x;
    bPosGraphic.y = bPos.y;

    isGameActive = 1;

    Bdisp_AllClr_VRAM();

    if(respawnFrames > 0) {
        // if dead just lock into perpetual "respawn"
        if(lives) --respawnFrames;
    }
    else if(respawnFrames==0) {
        bricksHit = 0;
        bSpeedMultiplier = 1.0f;
        bPos.x = 35.5+randf(); bPos.y = 29.5+randf();
        nbPos = bPos;
        bPosGraphic = bPos;
        bVel.x = -0.1f+randf()*0.2f;  bVel.y = 0.10f;
        respawnFrames = -1;
    }
    for(i = 0; i<OPS_PER_FRAME; ++i) {
        if(KeyDown(KEY_CTRL_MENU) || KeyDown(KEY_CTRL_EXIT) || (KeyDown(KEY_CTRL_EXE) && lives == 0)) {
            Bdisp_AllClr_DDVRAM();
            locate(1,1);
            Print("Press any key");
            KillTimer(ID_USER_TIMER1);
            Bdisp_PutDisp_DD();
            isGameActive = 0;
            return;
        } 
        if(respawnFrames == -1) {
            nbPos.x = bPos.x + bVel.x;
            nbPos.y = bPos.y + bVel.y;
        }

        // change speed
        if(KeyDown(KEY_CHAR_8) && !changeSpeedKeyDown) {
            pSpeedMode = min(5, pSpeedMode + 1);
            pSpeed = pSpeeds[pSpeedMode];
            changeSpeedKeyDown = 1;
        }
        else if(KeyDown(KEY_CHAR_7) && !changeSpeedKeyDown) {
            pSpeedMode = max(1, pSpeedMode - 1);
            pSpeed = pSpeeds[pSpeedMode];
            changeSpeedKeyDown = 1;
        }
        else if(!KeyDown(KEY_CHAR_8) && !KeyDown(KEY_CHAR_7)) {
            changeSpeedKeyDown = 0;
        }

        if(KeyDown(KEY_CTRL_LEFT)) {
            px = max(MIN_PX, px - pSpeed);
        }
        if(KeyDown(KEY_CTRL_RIGHT)) {
            px = min(MAX_PX, px + pSpeed);
        }

        if(respawnFrames == -1) {

            collisionIndex = getCollidingBrick(bPos, nbPos, &collisionPoint, &collisionSide);

            if(nbPos.y <= 0) {  // top wall
                nbPos.x = intersectY(bPos.x, bPos.y, bVel.x, bVel.y, 0);
                nbPos.y = 0;
                bVel.y = -bVel.y;
                bPosGraphic = nbPos;
            }
            else if(nbPos.y >= PADDLE_Y && bPos.y < PADDLE_Y) {  // passes thorugh paddle
                float ix = intersectY(bPos.x, bPos.y, bVel.x, bVel.y, PADDLE_Y);
                
                // divide into 5 segments: -5~-3, -3~-1, -1~1, 1~3, 3~5
                if(isBetween(ix, px-5, px-3)) {  // leftmost
                    bVel.x = -0.18f * bSpeedMultiplier;
                    bVel.y = -0.06f * bSpeedMultiplier;
                } else if(isBetween(ix, px-3, px-1)) {
                    bVel.x = -0.12f * bSpeedMultiplier;
                    bVel.y = -0.12f * bSpeedMultiplier;
                } else if(isBetween(ix, px-1, px+1)) {
                    if(bVel.x == 0) {  // going straight, bounce up
                        if(randf() < 0.5) {
                            bVel.x = -0.08f * bSpeedMultiplier;
                            bVel.y = -0.16f * bSpeedMultiplier;
                        } else {
                            bVel.x = 0.08f * bSpeedMultiplier;
                            bVel.y = -0.16f * bSpeedMultiplier;
                        }
                    }
                    else if(bVel.x > 0) {  // going right, bounce right
                        bVel.x = 0.08f * bSpeedMultiplier;
                        bVel.y = -0.16f * bSpeedMultiplier;
                    } else {  // going left, bounce left
                        bVel.x = -0.08f * bSpeedMultiplier;
                        bVel.y = -0.16f * bSpeedMultiplier;
                    }
                } else if(isBetween(ix, px+1, px+3)) {
                    bVel.x = 0.12f * bSpeedMultiplier;
                    bVel.y = -0.12f * bSpeedMultiplier;
                } else if(isBetween(ix, px+3, px+5)) {
                    bVel.x = 0.18f * bSpeedMultiplier;
                    bVel.y = -0.06f * bSpeedMultiplier;
                }
            }
            else if(nbPos.y >= 58) {  // bottom wall
                nbPos.x = intersectY(bPos.x, bPos.y, bVel.x, bVel.y, 58);
                nbPos.y = 58;
                bVel.x = 0;
                bVel.y = 0;
                bPosGraphic = nbPos;
                respawnFrames = FPS*2;
                lives--;
                break;
            }
            else if(nbPos.x <= 0) { // left wall
                nbPos.x = 0;
                nbPos.y = intersectX(bPos.x, bPos.y, bVel.x, bVel.y, 0);
                bVel.x = -bVel.x;
                bPosGraphic = nbPos;
            }
            else if(nbPos.x >= 72) {  // right wall
                nbPos.x = 72;
                nbPos.y = intersectX(bPos.x, bPos.y, bVel.x, bVel.y, 72);
                bVel.x = -bVel.x;
                bPosGraphic = nbPos;
            }

            if(collisionIndex.x != -1) {  // brick collision
                bricks[collisionIndex.x][collisionIndex.y] = 0;
                if(collisionSide) {
                    bVel.y = -bVel.y;
                } else {
                    bVel.x = -bVel.x;
                }
                nbPos = collisionPoint;
                bPosGraphic = nbPos;
                bricksHit++;
                if(bricksHit == 4 || bricksHit == 8 || bricksHit == 12) {
                    bVel.x *= (bSpeedMultiplier + 0.1f) / bSpeedMultiplier;
                    bSpeedMultiplier += 0.1f;
                }
                if(collisionIndex.y == 4 || collisionIndex.y == 5) {
                    score += 7;
                } else if(collisionIndex.y == 6 || collisionIndex.y == 7) {
                    score += 4;
                } else {
                    score += 1;
                }

                if(collisionIndex.y <= 6) {
                    // max speed when hitting top 3 rows
                    bSpeedMultiplier = 1.3f;
                    bricksHit = 99;
                }
            }

            bPos = nbPos;
        }
    }
    if(respawnFrames==-1) drawBall(bPosGraphic, nbPos);
    drawPaddle(px);
    drawWalls();
    drawBricks();
    drawUI();
    Bdisp_PutDisp_DD();

    SetTimer(ID_USER_TIMER1, 1000/FPS, frame);
}


void titleScreen() {
    unsigned int key;
    int i, j;

    Bdisp_AllClr_DDVRAM();
    locate(3,2);
    Print("BREAKOUT");
    locate(3,4);
    Print("Move: Left/Right");
    locate(3,5);
    Print("Change speed: 7/8");
    locate(3,7);
    Print("EXE to start");

    Bdisp_PutDisp_DD();

    for(i = 0; i < 18; i++) {
        for(j = 4; j < 10; j++) {
            bricks[i][j] = 1;
        }
    }

    while(KeyDown(KEY_CTRL_EXE)) {
        ++rSeed;
    }

    // while(!KeyDown(KEY_CTRL_EXE) && !KeyDown(KEY_CTRL_MENU)) {
    //     ++rSeed;
    // }
    while(1) {
        GetKeyMod(&key);
        if(key == KEY_CTRL_EXE || key == KEY_CTRL_MENU) {
            break;
        }
    }

    px = 36.0f;
    pSpeedMode = 3;
    pSpeed = 0.25f;

    bPos.x = 35.5+randf(); bPos.y = 29.5+randf();
    bVel.x = -0.1+randf()*0.2f;  bVel.y = 0.05f; // DO NOT RAISE OVER 2
    bricksHit = 0; // speed increases for 4th 8th 12th hit (3 times)
    bSpeedMultiplier = 1.0f;
    lives = 5;
    respawnFrames = FPS;
    score = 0;

    frame();

    return;
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

void randomTick() {
    randf();
    SetTimer(ID_USER_TIMER5, 100, randomTick);
}

int AddIn_main(int isAppli, unsigned short OptionNum)
{
    unsigned int key = 0;

    randomTick();
    titleScreen();

    while(1){
        GetKey(&key);
        if(!isGameActive) {
            isGameActive = 1;
            titleScreen();
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

