#pragma once
#include "GameNode.h"
#include <queue>

typedef struct MyPond
{
	int x;
	int y;
	int depth;
};

class PondMake : public GameNode
{
private:
	// 농장 타일의 정보
	TILE_INFO* farmTileInfo;

	// 플레이어 현재 위치
	pair<LONG, LONG> curPlayerRectPos;

	// 타일 렌더 위치
	FPOINT renderCoor;
	INT_PAIR startFrame;

	// 연못 샘플 이미지
	Image* pondSample;
	Image* pondEdgeSample;

	// 맵 생성
	int xSize;	
	int ySize;	
	int* map;

	// 플러드필 시작점
	int startX;
	int startY;

	// 방문(연못 위치) 체크
	int* visited;

	//상 하 좌 우
	int dirY[4];
	int dirX[4];

	// 큐
	queue<MyPond> myQ;

	// 재귀 횟수
	int recursionCnt;

	/* 확인용 */
	char szText[128] = "";
	int checkCnt;
	bool checkPondMap;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void ReFloodFill(int splitDir, int point, int increaseDir,				// 분할 방법 정보
		int* map, int startX, int startY, int endX, int endY, int xSize);	// 현재 맵 데이터 (현재 맵 x y크기, 전체 맵 x크기)
	void BFS(int startX, int startY, int xSize, int ySize, int* map, int* visited);
	
	void SetPondTile(int* visited);
	void SetPondEdgeTile(int* visited, INT_PAIR pondStartPoint, INT_PAIR curPlayerIndex);

	inline void SetCurPlayerRectPos(pair<LONG, LONG> curPlayerRectPos) { this->curPlayerRectPos = curPlayerRectPos; }
	inline void SetRenderCoor(FPOINT renderCoor) { this->renderCoor = renderCoor; }
	inline void SetStartFrame(INT_PAIR startFrame) { this->startFrame = startFrame; }

	virtual ~PondMake() {};
};

