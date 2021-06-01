#include "PondMake.h"
#include "Image.h"
#include<cstdlib>
#include<ctime>

HRESULT PondMake::Init()
{
    // ���� ����
	ImageManager::GetSingleton()->AddImage("��������", "Image/testPond.bmp", 128, 128, 1, 1);
	pondSample = ImageManager::GetSingleton()->FindImage("��������");

    // �� ����
    //xSize = FARM_TILE_X;										//����
    //ySize = FARM_TILE_Y;										//����
	xSize = 10;										//����
	ySize = 10;										//����
    map = (int*)malloc(sizeof(int) * xSize * ySize);

    // �� �ʱ�ȭ
    for (int i = 0; i < ySize; i++)
    {
        for (int j = 0; j < xSize; j++)
        {
            map[j + (xSize * i)] = 0;
        }
    }

	// ������
	startX = xSize / 2;
	startY = ySize / 2;

	// �湮 ���� �� �ʱ�ȭ
	visited = (bool*)malloc(sizeof(bool) * xSize * ySize);
	for (int i = 0; i < ySize; i++)
	{
		for (int j = 0; j < xSize; j++)
		{
			visited[j + (xSize * i)] = false;
		}
	}

	// bfs ����
	dirY[0] = { -1 }, dirY[1] = { 1 }, dirY[2] = { 0 }, dirY[3] = { 0 };	// �� �� - -
	dirX[0] = { 0 }, dirX[1] = { 0 }, dirX[2] = { -1 }, dirX[3] = { 1 };	// - - �� ��
	
    return S_OK;
}

void PondMake::Release()
{
}

void PondMake::BFS(int startX, int startY, int xSize, int ySize, int* map, bool* visited)
{
	// ������ ť�� �־��ְ� �湮 Ȯ��
	int startIndex = startX + xSize * startY;
	myQ.push({ startX , startY });
	visited[startIndex] = true;

	// ������ ��
	int mapNum = map[startIndex];

	// ť �ȿ� �ִ� �� 4�� Ȯ���ϸ鼭 ���� �� ������ üũ
	while (!myQ.empty())
	{
		// ť�� �ִ� �� ������ ����
		int curX = myQ.front().x;
		int curY = myQ.front().y;
		int curIndex = curX + xSize * curY;
		int curNum = map[curIndex];
		myQ.pop();

		// 4�� Ȯ��
		int nextIndex;
		for (int i = 0; i < 4; i++)
		{
			int nextX = curX + dirX[i];
			int nextY = curY + dirY[i];
			int nextIndex = nextX + xSize * nextY;

			// ���� ����� ��ŵ
			if (nextX < 0 || nextX >= xSize || nextY < 0 || nextY >= ySize)
				continue;

			// �湮���� ���� && ���� ���̶� ������
			if (mapNum == curNum)
			{
				if (visited[nextIndex] == false && (curNum == map[nextIndex] || (curNum - 1) == map[nextIndex] || (curNum - 2) == map[nextIndex]))
				{
					// ť�� �־��ְ� �湮 üũ
					myQ.push({ nextX, nextY });
					visited[nextIndex] = true;
				}
			}
			else if ((mapNum - 1 == curNum) || (mapNum - 2 == curNum))
			{
				if (visited[nextIndex] == false && (curNum == map[nextIndex]))
				{
					// ť�� �־��ְ� �湮 üũ
					myQ.push({ nextX, nextY });
					visited[nextIndex] = true;
				}
			}
		}
	}
}

void PondMake::ReFloodFill(int splitDir, int point, int increaseDir, int* map, int startX, int startY, int endX, int endY, int xSize)
{
	// ũ�� 2���Ϸ� �۾����� ��� ������
	if (splitDir == 0)	// ����
	{
		if ((point - startY) <= 3 || (endY - point) <= 3)
			return;
	}
	else if (splitDir == 1)	//����
	{
		if ((point - startX) <= 3 || (endX - point) <= 3)
			return;
	}

	// ���� ����
	if (splitDir == 0)	// ���η� �ڸ���
	{
		if (increaseDir == 0)	// �� ��� ����	
		{
			for (int i = startY; i < point; i++)
			{
				for (int j = startX; j < endX; j++)
				{
					map[j + (xSize * i)] += 1;
				}
			}
		}
		else if (increaseDir == 1)	// �Ʒ� ��� ����
		{
			for (int i = point; i < endY; i++)
			{
				for (int j = startX; j < endX; j++)
				{
					map[j + (xSize * i)] += 1;
				}
			}
		}
	}
	else if (splitDir == 1)	// ���η� �ڸ���
	{
		if (increaseDir == 2)	// ���� ��� ����
		{
			for (int i = startY; i < endY; i++)
			{
				for (int j = startX; j < point; j++)
				{
					map[j + (xSize * i)] += 1;
				}
			}
		}
		else if (increaseDir == 3)	// ������ ��� ����
		{
			for (int i = startY; i < endY; i++)
			{
				for (int j = point; j < endX; j++)
				{
					map[j + (xSize * i)] += 1;
				}
			}
		}
	}

	// �������� �� ��� �ٽ� ���
	int reSplitDir, rePoint, reIncreaseDir;
	if (splitDir == 0)
	{
		// ���η� �߶��� ���
		for (int i = 0; i < 2; i++)
		{
			reSplitDir = (rand() % 2);

			if (reSplitDir == 0)	// ����
				reIncreaseDir = (rand() % 2);
			else if (reSplitDir == 1)	// ����
				reIncreaseDir = (rand() % ((3 - 2) + 1)) + 2;

			if (i == 0)
			{
				if (reSplitDir == 0)	// ����
					rePoint = (rand() % (((point - 1) - startY) + 1)) + startY; //startY ~ (point-1)
				else if (reSplitDir == 1)	// ����
					rePoint = (rand() % (((endX - 1) - startX) + 1)) + startX; // startX ~ (endX-1)

				ReFloodFill(reSplitDir, rePoint, reIncreaseDir, map, startX, startY, endX, point, xSize);	// ���κ�
			}
			else if (i == 1)
			{
				if (reSplitDir == 0)	// ����
					rePoint = (rand() % (((endY - 1) - point) + 1)) + point; // point ~ (endY-1)
				else if (reSplitDir == 1)	// ����
					rePoint = (rand() % (((endX - 1) - startX) + 1)) + startX; // startX ~ (endX-1)

				ReFloodFill(reSplitDir, rePoint, reIncreaseDir, map, startX, point, endX, endY, xSize);		//�Ʒ� �κ�
			}
		}
	}
	else if (splitDir == 1)
	{
		// ���η� �߶��� ���
		for (int i = 0; i < 2; i++)
		{
			reSplitDir = (rand() % 2);

			if (reSplitDir == 0)	// ����
				reIncreaseDir = (rand() % 2);
			else if (reSplitDir == 1)	// ����
				reIncreaseDir = (rand() % ((3 - 2) + 1)) + 2;

			if (i == 0)
			{
				if (reSplitDir == 0)	// ����
					rePoint = (rand() % (((endY - 1) - startY) + 1)) + startY; // startY ~ (endY-1)
				else if (reSplitDir == 1)	// ����
					rePoint = (rand() % (((point - 1) - startX) + 1)) + startX; // startX ~ (point-1)

				ReFloodFill(reSplitDir, rePoint, reIncreaseDir, map, startX, startY, point, endY, xSize);	// ���� �κ�
			}
			else if (i == 1)
			{
				if (reSplitDir == 0)	// ����
					rePoint = (rand() % (((endY - 1) - startY) + 1)) + startY; // startY ~ (endY-1)
				else if (reSplitDir == 1)	// ����
					rePoint = (rand() % (((endX - 1) - point) + 1)) + point; // point ~ (endX-1)

				ReFloodFill(reSplitDir, rePoint, reIncreaseDir, map, point, startY, endX, endY, xSize);		// ������ �κ�
			}
		}
	}
}

void PondMake::Update()
{
    // ���
    srand((unsigned int)time(NULL));
    int splitDir = 0, point = 0, increaseDir = 0;
    if (KeyManager::GetSingleton()->IsOnceKeyDown('I'))
    {
        splitDir = (rand() % 2);	// (0: ��) (1: ��)

        if (splitDir == 0)	// ���η� �ڸ���
        {
            point = (rand() % ySize); // 0 ~ (ySize-1)
            increaseDir = (rand() % 2);	// (0: ��) (1: ��)
        }
        else if (splitDir == 1)	// ���η� �ڸ���
        {
            point = (rand() % xSize); // 0 ~ (xSize-1)
            increaseDir = (rand() % ((3 - 2) + 1)) + 2;	// (2: ��) (3: ��)
        }

        // ���� ù ������ �� ��ü ũ�⸦ ������� ����ȴ�
        ReFloodFill(splitDir, point, increaseDir, map, 0, 0, xSize, ySize, xSize);
    }

    // �÷�����
    if (KeyManager::GetSingleton()->IsOnceKeyDown('U'))
    {
		// �ʱ�ȭ
		for (int i = 0; i < ySize; i++)
		{
			for (int j = 0; j < xSize; j++)
			{
				visited[j + (xSize * i)] = false;
			}
		}

		// �ϴ� bfs
		BFS(startX, startY, xSize, ySize, map, visited);
    }
}

void PondMake::Render(HDC hdc)
{
	// �� ���
	for (int i = 0; i < ySize; i++)
	{
		for (int j = 0; j < xSize; j++)
		{
			wsprintf(szText, "%d", map[j + (xSize * i)]);
			TextOut(hdc, 200 + j * 20, i * 20, szText, strlen(szText));
		}
	}

	// ���� ��ġ ���
	for (int i = 0; i < ySize; i++)
	{
		for (int j = 0; j < xSize; j++)
		{
			wsprintf(szText, "%d", visited[j + (xSize * i)]);
			TextOut(hdc, 700 + j * 20, i * 20, szText, strlen(szText));
		}
	}

	// ���� �׸���
	for (int i = 0; i < ySize; i++)			// ����
	{
		for (int j = 0; j < xSize; j++)    // ����
		{
			int tempIndex = i * xSize + j;
			int tempPosX = j * F_TILESIZE;
			int tempPosY = i * F_TILESIZE;
			
			// ���� ���
			if (visited[tempIndex] == true)
			{
				pondSample->FrameRender(hdc,
					tempPosX,
					tempPosY,
					0,
					0);
			}
		}
	}
}
