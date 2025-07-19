//#define _CRT_SECURE_NO_WARNINGS

#include "Canvas.h"

int screen[WIDTH][HEIGHT] = { (int)SPACE };

Pos blockNowScreenPos = { 16, 3 };
Pos blockHoldScreenPos = { 3, 5 };
Pos blockNextScreenPos[] = { { 41, 5 }, { 41, 12 }, { 41, 19 } };

void DrawMap()
{
	UpdateScore();

	gotoXY(0, 2);
	printf("   ________     ____________________     ________ \n");
	printf("  |  Hold  |   |                    |   |  Next  |\n");
	printf("  |--------|   |                    |   |--------|\n");
	printf("  |        |   |                    |   |        |\n");
	printf("  |        |   |                    |   |        |\n");
	printf("  |        |   |                    |   |        |\n");
	printf("  |        |   |                    |   |        |\n");
	printf("  |________|   |                    |   |________|\n");
	printf("               |                    |             \n");
	printf("               |                    |    ________ \n");
	printf("               |                    |   |        |\n");
	printf("               |                    |   |        |\n");
	printf("               |                    |   |        |\n");
	printf("               |                    |   |        |\n");
	printf("               |                    |   |________|\n");
	printf("               |                    |             \n");
	printf("               |                    |    ________ \n");
	printf("               |                    |   |        |\n");
	printf("               |                    |   |        |\n");
	printf("               |                    |   |        |\n");
	printf("               |                    |   |        |\n");
	printf("               |                    |   |________|\n");
	printf("                --------------------");

	DrawBlockHold();
	DrawBlockNext();
}

void ClearScreen()
{
	for (int i = 0;i < WIDTH;i++)
		for (int j = 0;j < HEIGHT;j++)
			screen[i][j] = SPACE;
}

void ClearCanvas(Pos pos, int width, int height)
{
	for (int i = 0;i < width;i++)
		for (int j = 0;j < height;j++)
			printXY("  ", pos.x + 2 * i, pos.y + j);
}

void gotoXY(int x, int y)
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void printXY(char* str, int x, int y)
{
	gotoXY(x, y);
	printf("%s", str);
}

void Draw()
{
	int width = blockNowScreenPos.x, height = blockNowScreenPos.y;

	for (int h = 0;h < HEIGHT;h++) {
		for (int w = 0;w < WIDTH;w++) {
			DrawTile(width + 2 * w, height + h, screen[w][h]);
		}
	}

	DrawBlockNow();
}

void DrawTile(int x, int y, int drawType)
{
	switch (drawType)
	{
	case SPACE:
		printXY("  ", x, y);
		break;
	case BLOCK:
		printXY("бс", x, y);
		break;
	case SHADOW:
		printXY("бр", x, y);
		break;
	case GROUND:
		printXY("в╔", x, y);
		break;
	default:
		break;
	}
}

void DrawBlock(Block block, Pos pos, int drawType)
{
	for (int i = 0;i < 4;i++)
	{
		Blocks b = blocks[block.idx][block.rot];
		DrawTile(pos.x + b.pos[i].x * 2, pos.y + b.pos[i].y, drawType);
	}
}

void DrawBlockNow()
{
	int width = blockNowScreenPos.x, height = blockNowScreenPos.y;

	Pos posShadow = { width + ShadowBlockPos().x * 2, height + ShadowBlockPos().y };
	Pos posNow = { width + blockNowPos.x * 2, height + blockNowPos.y };

	DrawBlock(blockNow, posShadow, SHADOW);
	DrawBlock(blockNow, posNow, BLOCK);
}

void DrawBlockHold()
{
	if (blockHold.idx == -1) return;
	
	ClearCanvas(blockHoldScreenPos, 4, 4);
	DrawBlock(blockHold, blockHoldScreenPos, BLOCK);
}

void DrawBlockNext()
{
	for (int i = 0;i < NEXT;i++)
	{
		if (blockNext[i].idx == -1) return;

		ClearCanvas(blockNextScreenPos[i], 4, 4);
		DrawBlock(blockNext[i], blockNextScreenPos[i], BLOCK);
	}
}

void CheckPop()
{
	for (int h = 0;h < HEIGHT;h++) {
		int isPop = 1;
		for (int w = 0; w < WIDTH; w++) {
			if (screen[w][h] != GROUND) isPop = 0;
		}
		if (isPop == 1) DeleteLine(h);
	}
}

void DeleteLine(int line)
{
	for (int h = line; h > 0; h--)
		for (int w = 0; w < WIDTH; w++)
			screen[w][h] = screen[w][h - 1];

	for (int w = 0; w < WIDTH; w++)
		screen[w][0] = SPACE;

	score += 10;
	UpdateScore();
}

void UpdateScore()
{
	gotoXY(0, 0);
	if (score > bestScore) {
		bestScore = score;
		SaveScore(bestScore);
	}

	UpdateLevel();

	printf("\n     Lv.%-3d          Score: %04d        Best: %04d\n", level, score, bestScore);
}