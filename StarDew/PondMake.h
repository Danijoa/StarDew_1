#pragma once
#include "GameNode.h"
#include <queue>

class PondMake : public GameNode
{
private:
	Image* pondSample;

	// �� ����
	int xSize;	
	int ySize;	
	int* map = (int*)malloc(sizeof(int) * xSize * ySize);

	// �÷����� ������
	int startX;
	int startY;

	// �湮(���� ��ġ) üũ
	bool* visited;

	//�� �� �� ��
	int dirY[4];
	int dirX[4];

	// ť
	queue<INT_PAIR> myQ;

	/* Ȯ�ο� */
	char szText[128] = "";

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void ReFloodFill(int splitDir, int point, int increaseDir,				// ���� ��� ����
		int* map, int startX, int startY, int endX, int endY, int xSize);	// ���� �� ������ (���� �� x yũ��, ��ü �� xũ��)
	void BFS(int startX, int startY, int xSize, int ySize, int* map, bool* visited);

	virtual ~PondMake() {};
};

