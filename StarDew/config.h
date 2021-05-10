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
#define TILESIZE (16*4)
// 집
#define HOUSE_TILE_X (800/16/4)	//50/4
#define HOUSE_TILE_Y (832/16/4)	//52/4
// 마당
#define FARM_TILE_X (1280/16/4)
#define FARM_TILE_Y (1040/16/4)
// 광산
// 마을
// 상점

// 기타
#define PI			3.141592f
#define SAFE_DELETE(p) { if (p) { delete p; p = nullptr; } }
#define SAFE_RELEASE(p) { if (p) { p->Release(); delete p; p = nullptr; } }

typedef struct tagFPoint
{
	float x;
	float y;
} FPOINT, * PFPOINT;

extern enum class TileType {WALL, GROUND, HOUSEDOOR};

typedef struct tagTile
{
	RECT rcTile;
	int frameX;
	int frameY;
	TileType tileType;
} TILE_INFO;

typedef struct myPair
{
	int x;
	int y;
}INT_PAIR;

typedef struct rectIndex
{
	int leftIndex;
	int topIndex;
	int rightIndex;
	int bottomIndex;
}INT_RECTINDEX;

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
extern POINT g_ptMouse;