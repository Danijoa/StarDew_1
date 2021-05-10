#pragma once
#include "GameNode.h"

class Image;
class Player : public GameNode
{
private:
	// �⺻ 
	Image* player;
	FPOINT pos;
	FPOINT playerFuturePos;
	RECT playerRect;
	INT_PAIR playerIndex;
	float moveSpeed;
	INT_PAIR frameIndex;
	
	// ������
	int playerDir;	// �����¿�
	bool canMove;

	// �̷�
	INT_RECTINDEX playerFutureRectIndex;
	RECT playerFutureRect;

	// �� Ÿ�� ����
	TILE_INFO* houseTileInfo;

	// ���� ��
	int sceneNum;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void SetFuture(FPOINT playerFuturePos);
	void Move();

	// �Ͽ콺 �� Ÿ�� ����
	inline void SetHouseTileInfo(TILE_INFO* houseTileInfo) { this->houseTileInfo = houseTileInfo; }

	// �� ��ȣ
	inline void SetSceneNum(int sceneNum) { this->sceneNum = sceneNum; }

	virtual ~Player() {};
};

