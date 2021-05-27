#pragma once
#include "GameNode.h"

class Image;
class Player;
class StoreData;
class StoreScene : public GameNode
{
private:
	// ���� Ÿ�� �̹���
	Image* storeSample;

	// ���� Ÿ���� ����
	static TILE_INFO storeTileInfo[STORE_TILE_X * STORE_TILE_Y];

	// Ÿ�� ����
	FPOINT renderCoor;
	INT_PAIR startFrame;

	// �÷��̾�
	Player* player;

	// ��ǰ ���
	Image* storeList;
	Image* listExit;
	Image* listBox;
	//Image* cropData;
	StoreData* storeData;

	// �� ����
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

