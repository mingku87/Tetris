#ifndef BLOCK_H
#define BLOCK_H
#define NEXT 3

#include "GameManager.h"

typedef struct {
	int x;
	int y;
}Pos;

typedef struct {
	Pos pos[4];
}Blocks;

typedef struct {
	int idx;
	int rot;
}Block;

extern Block blockNow;
extern Block blockHold;
extern Block blockNext[NEXT];

extern Blocks blocks[7][4];

extern Pos blockNowPos;
extern void InitBlockPos();
extern void InitBlocks();
extern void InitBlock(Block* block, int idx, int rot);
extern void CreateNewBlock();

extern bool CheckCollision(Block block, Pos pos);
extern bool CanMove(Block block, Pos pos, int x, int y);
extern void MoveBlock(int x, int y);
extern void CheckGround();
extern void RotateBlock(int dir);
extern void DropBlock();
extern Pos ShadowBlockPos();

extern void GetNextBlock();
extern void SwitchBlock();
extern void SwapBlock(Block* block1, Block* block2);

#endif