#pragma once
#include "GameNode.h"

class Image;
class Player : public GameNode
{
private:
	// 기본 
	Image* player;
	FPOINT pos;
	FPOINT playerFuturePos;
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

	// 집 타일 정보
	TILE_INFO* houseTileInfo;

	// 현재 씬
	int sceneNum;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void SetFuture(FPOINT playerFuturePos);
	void Move();

	// 하우스 맵 타일 정보
	inline void SetHouseTileInfo(TILE_INFO* houseTileInfo) { this->houseTileInfo = houseTileInfo; }

	// 신 번호
	inline void SetSceneNum(int sceneNum) { this->sceneNum = sceneNum; }

	virtual ~Player() {};
};

