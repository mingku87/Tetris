#include "GameManager.h"

int main(void)
{
	system("mode con cols=52 lines=30 | title Tetris");

	Title();
	StartGame();

	remove("temp_bgm.wav");
	return 0;
}