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
	// ���� Ÿ���� ����
	TILE_INFO* farmTileInfo;

	// �÷��̾� ���� ��ġ
	pair<LONG, LONG> curPlayerRectPos;

	// Ÿ�� ���� ��ġ
	FPOINT renderCoor;
	INT_PAIR startFrame;

	// ���� ���� �̹���
	Image* pondSample;
	Image* pondEdgeSample;

	// �� ����
	int xSize;	
	int ySize;	
	int* map;

	// �÷����� ������
	int startX;
	int startY;

	// �湮(���� ��ġ) üũ
	int* visited;

	//�� �� �� ��
	int dirY[4];
	int dirX[4];

	// ť
	queue<MyPond> myQ;

	// ��� Ƚ��
	int recursionCnt;

	/* Ȯ�ο� */
	char szText[128] = "";
	int checkCnt;
	bool checkPondMap;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void ReFloodFill(int splitDir, int point, int increaseDir,				// ���� ��� ����
		int* map, int startX, int startY, int endX, int endY, int xSize);	// ���� �� ������ (���� �� x yũ��, ��ü �� xũ��)
	void BFS(int startX, int startY, int xSize, int ySize, int* map, int* visited);
	
	void SetPondTile(int* visited);
	void SetPondEdgeTile(int* visited, INT_PAIR pondStartPoint, INT_PAIR curPlayerIndex);

	inline void SetCurPlayerRectPos(pair<LONG, LONG> curPlayerRectPos) { this->curPlayerRectPos = curPlayerRectPos; }
	inline void SetRenderCoor(FPOINT renderCoor) { this->renderCoor = renderCoor; }
	inline void SetStartFrame(INT_PAIR startFrame) { this->startFrame = startFrame; }

	virtual ~PondMake() {};
};

