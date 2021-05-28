#pragma once
#include "GameNode.h"

class Image;
class Player;
class StoreData;
class FarmScene : public GameNode
{
private:
	// ���� Ÿ�� �̹���
	Image* farmSample;

	// ���� Ÿ���� ����
	TILE_INFO* farmTileInfo;

	// Ÿ�� ����
	FPOINT renderCoor;
	INT_PAIR startFrame;

	// �÷��̾�
	Player* player;

	// �� �̹���
	Image* tilesetSample;
	INT_PAIR winClicked;
	INT_PAIR farmClicked;

	//��� �̹���
	Image* growCrop;
	StoreData* storeData;
	map<int, bool> checkDay;

	// ��ġ �˷��ִ� ���� ��Ʈ
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

