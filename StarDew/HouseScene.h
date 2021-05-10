#pragma once
#include "GameNode.h"

class Image;
class Player;
class CollisionCheck;
class HouseScene : public GameNode
{
private:
	// �Ͽ콺 Ÿ�� �̹���
	Image* houseSample;

	// �Ͽ콺 Ÿ���� ����
	static TILE_INFO houseTileInfo[HOUSE_TILE_X * HOUSE_TILE_Y];

	// �÷��̾�
	Player* player;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void loadHouseScene(int sceneNum);

	virtual ~HouseScene() {};
};

