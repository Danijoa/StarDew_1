#pragma once
#include "GameNode.h"

class Image;
class Player;
class HouseScene : public GameNode
{
private:
	// �Ͽ콺 Ÿ�� �̹���
	Image* houseSample;

	// �Ͽ콺 Ÿ���� ����
	static TILE_INFO houseTileInfo[HOUSE_TILE_X * HOUSE_TILE_Y];

	// Ÿ�� ����
	FPOINT renderCoor;
	INT_PAIR startFrame;

	// �÷��̾�
	Player* player;

	// �� �� ���ΰ� Ȯ��
	Image* sleepBox;
	bool checkSleep;
	bool yesSleep;
	bool noSleep;

	// ��ġ �˷��ִ� ���� ��Ʈ
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

