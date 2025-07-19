#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <stdio.h>
#include <stdbool.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <mmsystem.h>
#include "Block.h"
#include "Canvas.h"
#include "resource.h"

#pragma comment(lib, "winmm.lib")

extern bool isPlaying;
extern bool isPaused;

extern int frames;
extern int framesN;
extern int framesMove;
extern int framesGround;

extern int level;
extern int score;
extern int bestScore;

extern FILE* scoreFile;
extern char* fileName;

extern void Title();
extern void Initialize();
extern void StartGame();
extern void Update();
extern void Pause();
extern void InputProcess();
extern void UpdateLevel();
extern void GameOver();

extern int LoadScore();
extern void SaveScore(int value);

extern void PlayMusicFromResource(int resourceID);
extern void PlayMusic(char* name);

#endif