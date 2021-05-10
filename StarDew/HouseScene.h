#pragma once
#include "GameNode.h"

class Image;
class Player;
class CollisionCheck;
class HouseScene : public GameNode
{
private:
	// 하우스 타일 이미지
	Image* houseSample;

	// 하우스 타일의 정보
	static TILE_INFO houseTileInfo[HOUSE_TILE_X * HOUSE_TILE_Y];

	// 플레이어
	Player* player;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void loadHouseScene(int sceneNum);

	virtual ~HouseScene() {};
};

