#pragma once
#include "GameNode.h"

class Image;
class Player;
class StoreData;
class StoreScene : public GameNode
{
private:
	// 상점 타일 이미지
	Image* storeSample;

	// 상점 타일의 정보
	static TILE_INFO storeTileInfo[STORE_TILE_X * STORE_TILE_Y];

	// 타일 렌더
	FPOINT renderCoor;
	INT_PAIR startFrame;

	// 플레이어
	Player* player;

	// 상품 목록
	Image* storeList;
	Image* listExit;
	Image* listBox;
	//Image* cropData;
	StoreData* storeData;

	// 돈 없다
	Image* moneyWarn;
	bool noMoney;
	float moneyTimeCnt;

	// 
	int storeDir;
	bool canMove;
	char szText[128] = "";
	bool listOpen;
	INT_PAIR mouseIndex;
	int startListIndex;
	int renderListCoor;
	int listYSize;
	int moveSpeed;
	HPEN hpen;
	HPEN hpenOld;
	int listIndex;

	int firstBoxHeight;
	int lastBoxHeight;

	string test;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void MoveControl();
	void Move();
	void playerStoreCollision();

	void loadStoreScene(int sceneNum);

	virtual ~StoreScene() {};
};

