#include "GameManager.h"

bool isPlaying = true;
bool isPaused = false;

int frames = 1;
int framesN = 50;
int framesMove = 0;
int framesGround = 0;

int level = 0;
int score = 0;
int bestScore = 0;

char* fileName = "score.txt";
FILE* scoreFile;

void Title()
{
	PlayMusic("GamePlay");

	srand(time(NULL));

	scoreFile = fopen("score.txt", "a");
	fclose(scoreFile);

	bestScore = LoadScore();

	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.bVisible = 0;
	cursorInfo.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

	printXY("T E T R I S", 19, 7);
	printXY("Press Any Key to Start Game", 11, 10);

	while (true)
	{
		if (_kbhit()) {
			char ch = _getch();
			break;
		}
	}
}

int LoadScore()
{
	scoreFile = fopen(fileName, "r"); // 읽기 모드로 열기
	int value = 0;

	if (fscanf(scoreFile, "%d", &value) != 1) {
		fclose(scoreFile);
		SaveScore(0);
		return 0;
	}

	fclose(scoreFile);
	return value; 
}

void SaveScore(int value)
{
	scoreFile = fopen(fileName, "w");

	fprintf(scoreFile, "%d", value);
	fclose(scoreFile);
}

void Initialize()
{
	score = 0;
	
	ClearScreen();
	
	InitBlocks();
	CreateNewBlock();

	DrawMap();
}

void StartGame()
{
	Initialize();

	while (isPlaying == true)
	{
		if (isPaused == true) Pause();

		Update();
	}

	GameOver();

	printXY("                          ", 15, 26);
	printXY("                          ", 15, 27);
	printXY("                          ", 15, 28);

	if (isPlaying == true) StartGame();
	else return;
}

void Update()
{
	if (frames % framesMove == 0) MoveBlock(0, 1);

	InputProcess();

	Draw();
	CheckGround();
	CheckPop();

	Sleep(1000 / framesN);
	frames++;
}

void Pause()
{
	printXY("Paused", 4, 11);

	while (true)
	{
		if (_kbhit()) {
			char ch = _getch();
			if (ch == 27) { 
				isPaused = false;
				break; 
			}
		}
	}

	printXY("      ", 4, 11);
}

void InputProcess()
{
	if (_kbhit() == false) return;

	char ch = _getch();
	switch (ch) {
	case 27:
		isPaused = true;
		break;
	case 'q':
	case 'Q':
		isPlaying = false;
		break;
	case 80: // 아래
		MoveBlock(0, 1);
		break;
	case 75: // 왼쪽
		MoveBlock(-1, 0);
		break;
	case 77: // 오른쪽
		MoveBlock(1, 0);
		break;
	case ' ':
		DropBlock();
		CreateNewBlock();
		break;
	case 'z':
	case 'Z':
		RotateBlock(-1);
		break;
	case 72: // 위
	case 'x':
	case 'X':
		RotateBlock(1);
		break;
	case 'c':
	case 'C':
		SwitchBlock();
		break;
	default:
		break;
	}
}

void UpdateLevel()
{
	level = score / 50 + 1;

	framesMove = framesN - level * 2;
	if (framesMove < 3) framesMove = 3;
}

void GameOver()
{
	PlayMusic("GameOver");

	printXY("Game Over!!", 15, 26);
	printXY("Press Any Key to Restart", 15, 27);
	printXY("Press Q to Quit", 15, 28);

	clock_t startTime = clock();
	int inputIgnored = 1;

	while (1)
	{
		if (inputIgnored && (clock() - startTime) / CLOCKS_PER_SEC > 0.5)
		{
			while (_kbhit())_getch();
			inputIgnored = 0;
		}
		
		if (!inputIgnored && _kbhit()) 
		{
			char ch = _getch();
			switch (ch)
			{
			case 'q':
				isPlaying = false;
				return;
			default:
				isPlaying = true;
				PlayMusic("GamePlay");
				return;
			}
		}
	}
}

void PlayMusicFromResource(int resourceID) {
	char tempPath[MAX_PATH];
	GetTempPathA(MAX_PATH, tempPath);
	strcat_s(tempPath, sizeof(tempPath), "temp_bgm.wav");

	HRSRC res = FindResource(NULL, MAKEINTRESOURCE(resourceID), RT_RCDATA);
	HGLOBAL resHandle = LoadResource(NULL, res);
	DWORD resSize = SizeofResource(NULL, res);
	void* resData = LockResource(resHandle);

	FILE* tempFile = fopen(tempPath, "wb");
	if (tempFile != NULL) {
		fwrite(resData, 1, resSize, tempFile);
		fclose(tempFile);

		sndPlaySoundA(tempPath, SND_ASYNC | SND_LOOP);
	}
}

void PlayMusic(char* name)
{
	if (strcmp(name, "GamePlay") == 0)
		PlayMusicFromResource(IDR_GAMEPLAY);
	else if (strcmp(name, "GameOver") == 0)
		PlayMusicFromResource(IDR_GAMEOVER);
}

BOOL WINAPI ConsoleHandler(DWORD signal) 
{
	if (signal == CTRL_C_EVENT || signal == CTRL_CLOSE_EVENT) exit(0);
	return TRUE;
}