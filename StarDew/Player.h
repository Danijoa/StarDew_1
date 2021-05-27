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
	
	// 움직임
	int playerDir;	// 상하좌우
	bool canMove;

	// 미래
	INT_RECTINDEX playerFutureRectIndex;
	RECT playerFutureRect;

	// 현재 씬
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

	// 위치
	inline void SetPlayerImagePos(FPOINT pos) { this->imagePos = pos; }
	inline FPOINT GetPlayerImagePos() { return this->imagePos; }
	inline void SetPlayerPos(FPOINT pos) { this->pos = pos; }
	inline FPOINT GetPlayerPos() { return this->pos; }
	inline RECT GetPlayerRect() { return this->playerRect; }

	// 예측 렉트 인덱스
	inline RECT GetPlayerFutureRect() { return this->playerFutureRect; }
	inline INT_RECTINDEX GetPlayerFutureRectIndex() { return this->playerFutureRectIndex; }
	inline INT_RECTINDEX GetPlayerCurrRectIndex() { return this->playerRectIndex; }

	// 맵 타일 정보
	inline void SetCurrImageSize(INT_PAIR size) { this->curImageSize = size; }

	// 이동
	inline void SetCanMove(bool move) { this->canMove = move; }

	// 방향
	inline void SetStartFrameIndex(INT_PAIR frameIndex) { this->frameIndex = frameIndex; }

	virtual ~Player() {};
};

