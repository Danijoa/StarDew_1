#pragma once
#include "GameNode.h"

class Image;
class Player;
class StoreData;
class FarmScene : public GameNode
{
private:
	// 농장 타일 이미지
	Image* farmSample;

	// 농장 타일의 정보
	TILE_INFO* farmTileInfo;

	// 타일 렌더
	FPOINT renderCoor;
	INT_PAIR startFrame;

	// 플레이어
	Player* player;

	// 밭 이미지
	Image* tilesetSample;
	INT_PAIR winClicked;
	INT_PAIR farmClicked;

	//곡식 이미지
	Image* growCrop;
	StoreData* storeData;
	map<int, bool> checkDay;

	// 위치 알려주는 검정 렉트
	HPEN hpen;
	HPEN hpenOld;

	// 
	int farmDir;
	bool canMove;
	char szText[128] = "";

public:
	virtual HRESULT Init();
	virtual void Release(); 
	virtual void Update();
	virtual void Render(HDC hdc);

	void testDigUp();
	void testWater();
	void testGrow();
	void testHarvest();
	void testDry();

	void MoveControl();
	void Move();
	void playerFarmCollision();

	virtual ~FarmScene() {};
};

