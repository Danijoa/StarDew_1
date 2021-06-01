#pragma once
#include "GameNode.h"
#include <queue>

class PondMake : public GameNode
{
private:
	Image* pondSample;

	// 맵 생성
	int xSize;	
	int ySize;	
	int* map = (int*)malloc(sizeof(int) * xSize * ySize);

	// 플러드필 시작점
	int startX;
	int startY;

	// 방문(연못 위치) 체크
	bool* visited;

	//상 하 좌 우
	int dirY[4];
	int dirX[4];

	// 큐
	queue<INT_PAIR> myQ;

	/* 확인용 */
	char szText[128] = "";

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void ReFloodFill(int splitDir, int point, int increaseDir,				// 분할 방법 정보
		int* map, int startX, int startY, int endX, int endY, int xSize);	// 현재 맵 데이터 (현재 맵 x y크기, 전체 맵 x크기)
	void BFS(int startX, int startY, int xSize, int ySize, int* map, bool* visited);

	virtual ~PondMake() {};
};

