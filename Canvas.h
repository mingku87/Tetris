#ifndef CANVAS_H
#define CANVAS_H
#define WIDTH 10
#define HEIGHT 21

#include "GameManager.h"

enum DrawType
{
	SPACE,
	BLOCK,
	SHADOW,
	GROUND,	
};

extern int screen[WIDTH][HEIGHT];

extern void gotoXY(int x, int y);
extern void printXY(char* str, int x, int y);

extern void ClearScreen();
extern void DrawMap();
extern void UpdateScore();

extern void Draw();
extern void DrawTile(int x, int y, int blockType);
extern void DrawBlockNow();
extern void DrawBlockHold();
extern void DrawBlockNext();
extern void CheckPop();
extern void DeleteLine(int line);

#endif