#include "Block.h"

Block blockNow = { -1,-1 };
Block blockHold = { -1,-1 };
Block blockNext[NEXT] = { { -1,-1 }, { -1,-1 }, { -1,-1 } };

Pos blockNowPos = { 3, 0 };

void InitBlockPos()
{
	blockNowPos.x = 3;
	blockNowPos.y = 0;
}

void InitBlocks()
{
	InitBlock(&blockNow, -1, -1);
	InitBlock(&blockHold, -1, -1);
	InitBlock(&blockNext[0], -1, -1);
	InitBlock(&blockNext[1], -1, -1);
	InitBlock(&blockNext[2], -1, -1);
}

Blocks blocks[7][4] =
{
	// бсбсбсбс
	{{{{ 0, 2 }, { 1, 2 }, { 2, 2 }, { 3, 2 }}},
	 {{{ 1, 0 }, { 1, 1 }, { 1, 2 }, { 1, 3 }}},
	 {{{ 0, 2 }, { 1, 2 }, { 2, 2 }, { 3, 2 }}},
	 {{{ 1, 0 }, { 1, 1 }, { 1, 2 }, { 1, 3 }}}},

	// бс
	// бсбс
	//   бс
	{{{{ 1, 1 }, { 1, 2 }, { 2, 2 }, { 2, 3 }}},
	 {{{ 0, 3 }, { 1, 3 }, { 1, 2 }, { 2, 2 }}},
	 {{{ 0, 1 }, { 0, 2 }, { 1, 2 }, { 1, 3 }}},
	 {{{ 0, 2 }, { 1, 2 }, { 1, 1 }, { 2, 1 }}}},

	//   бс
	// бсбс
	// бс
	{{{{ 2, 1 }, { 2, 2 }, { 1, 2 }, { 1, 3 }}},
	 {{{ 0, 2 }, { 1, 2 }, { 1, 3 }, { 2, 3 }}},
	 {{{ 0, 3 }, { 0, 2 }, { 1, 1 }, { 1, 2 }}},
	 {{{ 0, 1 }, { 1, 1}, { 1, 2 }, { 2, 2 }}}},

	// бсбс
	// бсбс
	{{{{ 1, 1 }, { 1, 2 }, { 2, 1 }, { 2, 2 }}},
	 {{{ 1, 1 }, { 1, 2 }, { 2, 1 }, { 2, 2 }}},
	 {{{ 1, 1 }, { 1, 2 }, { 2, 1 }, { 2, 2 }}},
	 {{{ 1, 1 }, { 1, 2 }, { 2, 1 }, { 2, 2 }}}},

	// бсбсбс
	// бс
	{{{{ 0, 3 }, { 0, 2 }, { 1, 2 }, { 2, 2 }}},
	 {{{ 0, 1 }, { 1, 1 }, { 1, 2 }, { 1, 3 }}},
	 {{{ 0, 2 }, { 1, 2 }, { 2, 2 }, { 2, 1 }}},
	 {{{ 1, 1 }, { 1, 2 }, { 1, 3 }, { 2, 3 }}}},

	// бс
	// бсбсбс
	{{{{ 0, 1 }, { 0, 2 }, { 1, 2 }, { 2, 2 }}},
	 {{{ 1, 1 }, { 1, 2 }, { 1, 3 }, { 2, 1 }}},
	 {{{ 0, 2 }, { 1, 2 }, { 2, 2 }, { 2, 3 }}},
	 {{{ 0, 3 }, { 1, 1 }, { 1, 2 }, { 1, 3 }}}},

	//   бс
	// бсбсбс
	{{{{ 0, 2 }, { 1, 2 }, { 1, 1 }, { 2, 2 }}},
	 {{{ 1, 1 }, { 1, 2 }, { 1, 3 }, { 2, 2 }}},
	 {{{ 0, 2 }, { 1, 2 }, { 2, 2 }, { 1, 3 }}},
	 {{{ 0, 2 }, { 1, 1 }, { 1, 2 }, { 1, 3 }}}}
};

void InitBlock(Block* block, int idx, int rot)
{
	block->idx = idx;
	block->rot = rot;
}

void CreateNewBlock()
{
	if (blockNext[0].idx == -1)
	{
		InitBlock(&blockNow, rand() % 7, 0);
		for (int i = 0;i < NEXT;i++) InitBlock(&blockNext[i], rand() % 7, 0);
	}
	else
	{
		for (int i = 0;i < 4;i++)
		{
			Blocks b = blocks[blockNow.idx][blockNow.rot];
			screen[blockNowPos.x + b.pos[i].x][blockNowPos.y + b.pos[i].y] = GROUND;
		}
		GetNextBlock();
	}

	InitBlockPos();
	DrawBlockNext();

	if (CheckCollision(blockNow, blockNowPos) == true) isPlaying = false;
}

bool CheckCollision(Block block, Pos blockPos)
{
	Blocks b = blocks[block.idx][block.rot];

	for (int i = 0;i < 4;i++)
	{
		int x = blockPos.x + b.pos[i].x;
		int y = blockPos.y + b.pos[i].y;
		if (x < 0 || x > WIDTH - 1 || y > HEIGHT - 1 || (screen[x][y] != (int)SPACE)) return true;
	}
	return false;
}

bool CanMove(Block block, Pos pos, int x, int y)
{
	Pos newPos = { pos.x + x, pos.y + y };
	if (CheckCollision(block, newPos) == 1) return false;
	return true;
}

void MoveBlock(int x, int y)
{
	if (CanMove(blockNow, blockNowPos, x, y) == 1) {
		blockNowPos.x += x;
		blockNowPos.y += y;
	}
}

void CheckGround()
{
	if (CanMove(blockNow, blockNowPos, 0, 1) == 1)
	{
		framesGround = 0;
		return;
	}
	
	framesGround++;

	if (framesGround > (float)framesN * 0.4)
	{
		framesGround = 0;
		CreateNewBlock();
	}
}

void RotateBlock(int dir)
{
	int rot = (blockNow.rot + dir + 4) % 4;
	Block block = { blockNow.idx, rot };
	if (CheckCollision(block, blockNowPos) == 1)
	{
		if (CanMove(block, blockNowPos, 1, 0) == 1) blockNowPos.x++;
		else if (CanMove(block, blockNowPos, -1, 0) == 1) blockNowPos.x--;
		else if (CanMove(block, blockNowPos, 0, 1) == 1) blockNowPos.y++;
		else if (CanMove(block, blockNowPos, 0, -1) == 1) blockNowPos.y--;
		else return;
	}
	blockNow.rot = rot;
}

void DropBlock()
{
	blockNowPos = ShadowBlockPos();
}

Pos ShadowBlockPos()
{
	Pos pos = { blockNowPos.x ,blockNowPos.y };
	while (CheckCollision(blockNow, pos) == 0) pos.y++;
	pos.y--;
	return pos;
}

void GetNextBlock()
{
	SwapBlock(&blockNow, &blockNext[0]);
	SwapBlock(&blockNext[0], &blockNext[1]);
	SwapBlock(&blockNext[1], &blockNext[2]);
	InitBlock(&blockNext[2], rand() % 7, 0);

	DrawBlockNext();
}

void SwitchBlock()
{
	SwapBlock(&blockNow, &blockHold);

	if (blockNow.idx == -1) GetNextBlock();
	else
	{
		while (CheckCollision(blockNow, blockNowPos) == 1)
		{
			if (blockNowPos.x < 0) blockNowPos.x++;
			else blockNowPos.x--;
		}
	}

	DrawBlockHold();
}

void SwapBlock(Block* block1, Block* block2)
{
	Block temp = *block1;
	*block1 = *block2;
	*block2 = temp;
}