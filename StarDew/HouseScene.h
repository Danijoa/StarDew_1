#pragma once
#include "GameNode.h"

class Image;
class Player;
class HouseScene : public GameNode
{
private:
	// 하우스 타일 이미지
	Image* houseSample;

	// 하우스 타일의 정보
	static TILE_INFO houseTileInfo[HOUSE_TILE_X * HOUSE_TILE_Y];

	// 타일 렌더
	FPOINT renderCoor;
	INT_PAIR startFrame;

	// 플레이어
	Player* player;

	// 잠 들 것인가 확인
	Image* sleepBox;
	bool checkSleep;
	bool yesSleep;
	bool noSleep;

	// 위치 알려주는 빨간 렉트
	HPEN hpen;
	HPEN hpenOld;

	// 
	int houseDir;
	bool canMove;
	char szText[128] = "";

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	inline bool GetCanMove() { return this->canMove; }

	void MoveControl();
	void Move();
	void playerHouseCollision();

	void loadHouseScene(int sceneNum);

	virtual ~HouseScene() {};
};

