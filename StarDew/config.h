// config.h

#pragma once
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "msimg32.lib")

#include <Windows.h>
#include <string>
#include <ctime>
#include <bitset>
#include <map>
#include <vector>

using namespace std;

#include "KeyManager.h"
#include "ImageManager.h"
#include "TimerManager.h"
#include "SceneManager.h"

// 인게임 윈도우 창
#define WINSIZE_X	1355
#define WINSIZE_Y	725

// 타일찍기 창
#define TILEMAPTOOLSIZE_X	1900
#define TILEMAPTOOLSIZE_Y	900
#define TILESIZE 15
#define TILE_X 50
#define TILE_Y 50

//
#define PI			3.141592f
#define SAFE_DELETE(p) { if (p) { delete p; p = nullptr; } }
#define SAFE_RELEASE(p) { if (p) { p->Release(); delete p; p = nullptr; } }

typedef struct tagFPoint
{
	float x;
	float y;
} FPOINT, * PFPOINT;

extern enum class TileType {};

typedef struct tagTile
{
	RECT rcTile;
	int frameX;
	int frameY;
	TileType currTile;
} TILE_INFO;

typedef struct tagTile
{
	int x;
	int y;
}INT_PAIR;

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
extern POINT g_ptMouse;