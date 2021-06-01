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
#define SAMPLE_TILE_X 4
#define SAMPLE_TILE_Y 8
#define TILEMAPTOOLSIZE_X	1900
#define TILEMAPTOOLSIZE_Y	900
#define TILESIZE (32)	//(16*4/2)
#define F_TILESIZE (16*4)

// 집
#define HOUSE_TILE_X (44)		//(1408/16/4*2)
#define HOUSE_TILE_Y (24)		//(768/16/4*2)
// 마당
#define FARM_TILE_X (1280*3/16/4)			//(60*2)				//(1280*3/16/4)
#define FARM_TILE_Y (896*3/16/4)			//(42*2)			//(896*3/16/4)
// 상점
#define STORE_TILE_X (1408/16/4*2)
#define STORE_TILE_Y (896/16/4*2)

// 기타
#define PI			3.141592f
#define SAFE_DELETE(p) { if (p) { delete p; p = nullptr; } }
#define SAFE_RELEASE(p) { if (p) { p->Release(); delete p; p = nullptr; } }

typedef struct tagFPoint
{
	float x;
	float y;
} FPOINT, * PFPOINT;

enum class TileType {
	WALL, GROUND, HOUSEDOOR, CAVE, FLOOR, STOREDOOR, POND, COUNTER, BUSSTOP,
	WOOD, STONE, FIBER,
	DIG, WETDIG,
	CROP
};

enum class SeedType {
	NONE, CROP, 
	PARSNIP, CAULIFLOWER, RHUBARB, BLUEBERRY, CRANBERRIES, FAIRYROSE,
	ANCIENTFRUIT, GREENBEAN, GRAPE
};

typedef struct tagTile
{
	RECT rcTile;
	int frameX;
	int frameY;
	int objFrameX;
	int objFrameY;
	TileType tileType;

	int seedFrameX;
	int seedFrameY;
	SeedType seedType;
	int day;
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

enum class ObjectType { NONE, TOOLS, CROPS, CROPGROWN};

typedef struct myIven
{
	ObjectType objType;
	int frameX;
	int frameY;
	int orderNum;	// 가격
	int salePrice;
	string productName;
	int amount;

	myIven(ObjectType objType, int frameX, int frameY, int orderNum, int salePrice, string productName, int amount)
	{
		this->objType = objType;
		this->frameX = frameX;
		this->frameY = frameY;
		this->orderNum = orderNum;
		this->salePrice = salePrice;
		this->productName = productName;
		this->amount = amount;
	};

} INVEN_INFO;

extern HWND g_hWnd;		
extern HINSTANCE g_hInstance;
extern POINT g_ptMouse;