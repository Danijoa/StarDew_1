#pragma once
#include "GameNode.h"

class Image;
class HouseScene;
class Player : public GameNode
{
private:
	//
	Image* player;
	FPOINT pos;
	FPOINT imagePos;
	FPOINT playerFuturePos;
	INT_RECTINDEX playerRectIndex;
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

	// ���� ��
	INT_PAIR curImageSize;

	//
	char szText[128] = "";

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void SetFuture(FPOINT playerFuturePos);
	void Move();

	// ��ġ
	inline void SetPlayerImagePos(FPOINT pos) { this->imagePos = pos; }
	inline FPOINT GetPlayerImagePos() { return this->imagePos; }
	inline void SetPlayerPos(FPOINT pos) { this->pos = pos; }
	inline FPOINT GetPlayerPos() { return this->pos; }
	inline RECT GetPlayerRect() { return this->playerRect; }

	// ���� ��Ʈ �ε���
	inline RECT GetPlayerFutureRect() { return this->playerFutureRect; }
	inline INT_RECTINDEX GetPlayerFutureRectIndex() { return this->playerFutureRectIndex; }
	inline INT_RECTINDEX GetPlayerCurrRectIndex() { return this->playerRectIndex; }

	// �� Ÿ�� ����
	inline void SetCurrImageSize(INT_PAIR size) { this->curImageSize = size; }

	// �̵�
	inline void SetCanMove(bool move) { this->canMove = move; }

	// ����
	inline void SetStartFrameIndex(INT_PAIR frameIndex) { this->frameIndex = frameIndex; }

	virtual ~Player() {};
};

