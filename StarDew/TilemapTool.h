#pragma once
#include "GameNode.h"

#define SAMPLE_TILE_X 4
#define SAMPLE_TILE_Y 4

class Image;
class TilemapTool : public GameNode
{
private:
	// ���� Ÿ���� ����
	static TILE_INFO tileInfo[TILE_X * TILE_Y];
	static TileType tileTypeInfo[TILE_X * TILE_Y];

	// ���� Ÿ�� ����
	Image* test;
	TILE_INFO sampleTileInfo[SAMPLE_TILE_X * SAMPLE_TILE_Y];

	// ���� Ÿ�� ����
	INT_PAIR ptSelectedFrame;
	HBRUSH hSelectedBrush;
	HBRUSH hOldSelectedBrush;

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

