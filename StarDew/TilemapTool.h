#pragma once
#include "GameNode.h"

class Image;
class TilemapTool : public GameNode
{
private:
	// ���� Ÿ���� ����
	static TILE_INFO houseTileInfo[HOUSE_TILE_X * HOUSE_TILE_Y];
	static TILE_INFO farmTileInfo[FARM_TILE_X * FARM_TILE_Y];
	static TILE_INFO storeTileInfo[STORE_TILE_X * STORE_TILE_Y];
	static TILE_INFO tilesetTileInfo[SAMPLE_TILE_X * SAMPLE_TILE_Y];

	// ���� Ÿ�� ����
	Image* sample;
	TILE_INFO sampleTileInfo[SAMPLE_TILE_X * SAMPLE_TILE_Y];

	// ���� Ÿ�� ����
	INT_PAIR ptSelectedFrame;
	INT_PAIR ptSelectedFrameShow;
	HBRUSH hSelectedBrush;
	HBRUSH hOldSelectedBrush;

	//
	RECT rcMain;
	char szText[128] = "";

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	// ���� ����
	static void Save(int sceneNum);
	static void Load(int sceneNum);

	virtual ~TilemapTool() {};
};