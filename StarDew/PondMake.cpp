#include "PondMake.h"
#include "DataManager.h"
#include "Image.h"
#include<cstdlib>
#include<ctime>
#define MAX FARM_TILE_X

HRESULT PondMake::Init()
{
	// Ÿ�� ���� ��������
	farmTileInfo = DataManager::GetSingleton()->GetFarmScene();

	// �÷��̾� ��ġ �ʽ�ȭ �صα�
	curPlayerRectPos = { 0,0 };

	// Ÿ�� ���� ��ġ �ʱ�ȭ
	renderCoor = { 0,0 };
	startFrame = { 0,0 };

    // ���� ���� 
	ImageManager::GetSingleton()->AddImage("��������", "Image/testPond.bmp", 64 * 6, 64, 6, 1);	// ���� ǥ���� 6����
	pondSample = ImageManager::GetSingleton()->FindImage("��������");

	ImageManager::GetSingleton()->AddImage("�����׵θ�����", "Image/testPondEdge.bmp", 64 * 12, 64, 12, 1);	// 8+4�� ����� ��
	pondEdgeSample = ImageManager::GetSingleton()->FindImage("�����׵θ�����");

    // �� ����
	xSize = 14;										//����
	ySize = 14;										//����
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
	visited = (int*)malloc(sizeof(int) * (xSize + 2) * (ySize + 2));
	for (int i = 0; i < ySize + 2; i++)
	{
		for (int j = 0; j < xSize + 2; j++)
		{
			visited[j + ((xSize + 2) * i)] = 0;
		}
	}

	// bfs ����
	dirY[0] = { -1 }, dirY[1] = { 1 }, dirY[2] = { 0 }, dirY[3] = { 0 };	// �� �� - -
	dirX[0] = { 0 }, dirX[1] = { 0 }, dirX[2] = { -1 }, dirX[3] = { 1 };	// - - �� ��

	// ��� Ƚ��
	recursionCnt = 0;

	// �� ��� Ȯ��
	checkCnt = 0;
	checkPondMap = false;
	
    return S_OK;
}

void PondMake::Release()
{
}

void PondMake::BFS(int startX, int startY, int xSize, int ySize, int* map, int* visited)
{
	// ����ǥ��
	int depth = 32;	//pow(2,6)

	// ������ ť�� �־��ְ� �湮 Ȯ��
	int startIndex = startX + xSize * startY;
	myQ.push({ startX , startY, depth });
	int startIndexV = (startX + 1) + (xSize + 2) * (startY + 1);
	visited[startIndexV] = depth;	//depth

	// ������ ��
	int mapNum = map[startIndex];

	// ť �ȿ� �ִ� �� 4�� Ȯ���ϸ鼭 ���� �� ������ üũ
	while (!myQ.empty())
	{
		// ť�� �ִ� �� ������ ����
		int curX = myQ.front().x;
		int curY = myQ.front().y;
		int curDepth = myQ.front().depth;
		int curIndex = curX + xSize * curY;
		int curNum = map[curIndex];
		myQ.pop();

		// �θ� ���� / 2
		depth = curDepth / 2;

		// 4�� Ȯ��
		int nextIndex;
		for (int i = 0; i < 4; i++)
		{
			int nextX = curX + dirX[i];
			int nextY = curY + dirY[i];
			int nextIndex = nextX + xSize * nextY; 
			int nextIndexV = (nextX + 1) + (xSize + 2) * (nextY + 1);

			// ���� ����� ��ŵ
			if (nextX <= 0 || nextX >= xSize - 1 || nextY <= 0 || nextY >= ySize - 1)
				continue;

			/* �� �� !!!!!!!!!!!!!!!!!!!! */
			// �湮 ���߰� &&
			if (visited[nextIndexV] == 0 && abs(map[startIndex] - map[nextIndex]) < depth)
			{
				// ť�� �־��ְ� �湮 üũ
				myQ.push({ nextX, nextY, depth });
				visited[nextIndexV] = depth;
			}

			/*
			// �湮���� ���� && (���ϴ� ��)�̶� ������
			if (mapNum == curNum)
			{
				if (visited[nextIndex] == false &&
					(curNum == map[nextIndex] || (curNum - 1) == map[nextIndex] || (curNum + 1) == map[nextIndex]))
				{
					// ť�� �־��ְ� �湮 üũ
					myQ.push({ nextX, nextY, depth });
					visited[nextIndex] = depth;
				}
			}
			else if ((mapNum - 1 == curNum) || (mapNum + 1 == curNum))
			{
				if (visited[nextIndex] == false && (curNum == map[nextIndex]))
				{
					// ť�� �־��ְ� �湮 üũ
					myQ.push({ nextX, nextY, depth });
					visited[nextIndex] = depth;
				}
			}
			*/
		}
	}
}

void PondMake::ReFloodFill(int splitDir, int point, int increaseDir, int* map, int startX, int startY, int endX, int endY, int xSize)
{
	// ��� �� �� �ִ��� ���� Ȯ������
	bool left = true;
	bool right = true;
	bool top = true;
	bool bottom = true;

	// ũ�� 2���Ϸ� �۾����� ��� ������
	if (splitDir == 0)	// ����
	{
		if (point - startY <= 1)
			top = false;
		if (endY - point <= 1)
			bottom = false;
	}
	else if (splitDir == 1)	//����
	{
		if (point - startX <= 1)
			left = false;
		if (endX - point <= 1)
			right = false;
	}

	if (top == false && bottom == false)
		return;
	if (left == false && right == false)
		return;

	// ���� ����
	if (splitDir == 0)	// ���η� �ڸ���
	{
		if (increaseDir == 0 && top == true)	// �� ��� ����	
		{
			for (int i = startY; i < point; i++)
			{
				for (int j = startX; j < endX; j++)
				{
					map[j + (xSize * i)] += 1;
				}
			}
		}
		else if (increaseDir == 1 && bottom == true)	// �Ʒ� ��� ����
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
		if (increaseDir == 2 && left == true)	// ���� ��� ����
		{
			for (int i = startY; i < endY; i++)
			{
				for (int j = startX; j < point; j++)
				{
					map[j + (xSize * i)] += 1;
				}
			}
		}
		else if (increaseDir == 3 && right == true)	// ������ ��� ����
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

			if (i == 0 && top == true)
			{
				if (reSplitDir == 0)	// ����
					rePoint = (rand() % (((point - 1) - startY) + 1)) + startY; //startY ~ (point-1)
				else if (reSplitDir == 1)	// ����
					rePoint = (rand() % (((endX - 1) - startX) + 1)) + startX; // startX ~ (endX-1)

				ReFloodFill(reSplitDir, rePoint, reIncreaseDir, map, startX, startY, endX, point, xSize);	// ���κ�
			}
			else if (i == 1 && bottom == true)
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

			if (i == 0 && left == true)
			{
				if (reSplitDir == 0)	// ����
					rePoint = (rand() % (((endY - 1) - startY) + 1)) + startY; // startY ~ (endY-1)
				else if (reSplitDir == 1)	// ����
					rePoint = (rand() % (((point - 1) - startX) + 1)) + startX; // startX ~ (point-1)

				ReFloodFill(reSplitDir, rePoint, reIncreaseDir, map, startX, startY, point, endY, xSize);	// ���� �κ�
			}
			else if (i == 1 && right == true)
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

void PondMake::SetPondEdgeTile(int* visited, INT_PAIR pondStartPoint, INT_PAIR curPlayerIndex)
{
	for (int i = 0; i < ySize + 2; i++)			// ����
	{
		for (int j = 0; j < xSize + 2; j++)		// ����
		{
			// �����̸� �н�
			if (visited[j + ((xSize + 2) * i)] != 0)
				continue;

			// (j, i)�� 4�� Ȯ��
			int check = 0;
			for (int k = 0; k < 4; k++)
			{
				int checkX = j + dirX[k];
				int checkY = i + dirY[k];

				// ���� ����� Ȯ�� ���Ѵ�
				if (checkX < 0 || checkX >= xSize + 2 || checkY < 0 || checkY >= ySize + 2)
					continue;

				if (visited[checkX + ((xSize + 2) * checkY)] != 0)
					check += pow(10, k);							// ��:1 ��:10 ��:100 ��:1000
			}

			// farm �̹��� �ε��� �󿡼� ���� �׵θ� �׷��� �ε���
			INT_PAIR pondEdgeMakeIndex = { curPlayerIndex.x + (j - pondStartPoint.x + 1), curPlayerIndex.y + (i - pondStartPoint.y + 1) };

			// ���� Ȯ�� �ϴ� ��ġ(j, i)�� �����¿� ��� pond�� �н�
			// ��
			if (check == 1)
			{
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].tileType = TileType::PONDEDGE;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameX = 1;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameY = 0;
			}
			// ��
			else if (check == 10)
			{
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].tileType = TileType::PONDEDGE;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameX = 0;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameY = 0;
			}
			// ��
			else if (check == 100)
			{
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].tileType = TileType::PONDEDGE;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameX = 3;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameY = 0;
			}
			// ��
			else if (check == 1000)
			{
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].tileType = TileType::PONDEDGE;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameX = 2;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameY = 0;
			}
			// ����
			else if (check == 101)
			{
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].tileType = TileType::PONDEDGE;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameX = 6;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameY = 0;
			}
			// ���
			else if (check == 1001)
			{
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].tileType = TileType::PONDEDGE;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameX = 7;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameY = 0;
			}
			// ����
			else if (check == 110)
			{
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].tileType = TileType::PONDEDGE;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameX = 4;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameY = 0;
			}
			// �Ͽ�
			else if (check == 1010)
			{
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].tileType = TileType::PONDEDGE;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameX = 5;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameY = 0;
			}
			// ���¿�
			else if (check == 1101)
			{
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].tileType = TileType::PONDEDGE;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameX = 8;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameY = 0;
			}
			// ���¿�
			else if (check == 1110)
			{
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].tileType = TileType::PONDEDGE;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameX = 9;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameY = 0;
			}
			// ������
			else if (check == 111)
			{
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].tileType = TileType::PONDEDGE;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameX = 10;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameY = 0;
			}
			// ���Ͽ�
			else if (check == 1011)
			{
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].tileType = TileType::PONDEDGE;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameX = 11;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameY = 0;
			}


		}
	}
}


void PondMake::SetPondTile(int* visited)
{
	// ���� ��������� �簢�� ������ ���ϱ�
	INT_PAIR pondStartPoint = { MAX , MAX };
	for (int i = 0; i < ySize + 2; i++)			// ����
	{
		for (int j = 0; j < xSize + 2; j++)		// ����
		{
			if (visited[j + ((xSize + 2) * i)] != 0)
			{
				if (pondStartPoint.x > j)
					pondStartPoint.x = j;
				if (pondStartPoint.y > i)
					pondStartPoint.y = i;
			}
		}
	}

	// farm �̹��� �ε��� �󿡼� �÷��̾� �ε���
	INT_PAIR curPlayerIndex = { (curPlayerRectPos.first + renderCoor.x) / F_TILESIZE, (curPlayerRectPos.second + renderCoor.y) / F_TILESIZE };

	// ���� ����
	for (int i = 0; i < ySize; i++)			// ����
	{
		for (int j = 0; j < xSize; j++)		// ����
		{
			// farm �̹��� �ε��� �󿡼� ���� �׷��� �ε���
			INT_PAIR pondMakeIndex = { curPlayerIndex.x + (j - pondStartPoint.x + 1), curPlayerIndex.y + (i - pondStartPoint.y + 1) };

			// ���̰� 0 �̸�, ������ �ƴ�
			int frameCheck = 0;
			for (int k = 32; k>=1; k /= 2)
			{
				if (visited[j + ((xSize + 2) * i)] == k)
				{
					farmTileInfo[pondMakeIndex.x + (FARM_TILE_X * pondMakeIndex.y)].tileType = TileType::PONDMADE;
					farmTileInfo[pondMakeIndex.x + (FARM_TILE_X * pondMakeIndex.y)].objFrameX = frameCheck;
					farmTileInfo[pondMakeIndex.x + (FARM_TILE_X * pondMakeIndex.y)].objFrameY = 0;
				} 
				frameCheck++;
			}
		}
	}

	// ���� �׵θ� 
	SetPondEdgeTile(visited, pondStartPoint, curPlayerIndex);
}

void PondMake::Update()
{
	// map + visited + Ƚ�� ���ʱ�ȭ 
	if (KeyManager::GetSingleton()->IsOnceKeyDown('L'))
	{
		recursionCnt = 0;

		for (int i = 0; i < ySize; i++)
		{
			for (int j = 0; j < xSize; j++)
			{
				map[j + (xSize * i)] = 0;
			}
		}

		for (int i = 0; i < ySize; i++)
		{
			for (int j = 0; j < xSize; j++)
			{
				visited[j + (xSize * i)] = 0;
			}
		}
	}

    // ���
    srand((unsigned int)time(NULL));
    int splitDir = 0, point = 0, increaseDir = 0;
    if (KeyManager::GetSingleton()->IsOnceKeyDown('I'))
    {
		recursionCnt += 1;

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
		// �湮 �ʱ�ȭ
		for (int i = 0; i < ySize + 2; i++)
		{
			for (int j = 0; j < xSize + 2; j++)
			{
				visited[j + ((xSize + 2) * i)] = 0;
			}
		}

		// �ϴ� bfs
		BFS(startX, startY, xSize, ySize, map, visited);
    }

	// ���� �־��ֱ�
	if (KeyManager::GetSingleton()->IsOnceKeyDown('Y'))
	{
		SetPondTile(visited);
	}

	// ���� �� Ȯ�ο�
	if (KeyManager::GetSingleton()->IsOnceKeyDown('K'))
	{
		checkCnt += 1;
		if(checkCnt %2 == 1)	// Ȧ���� ���
			checkPondMap = true;
		else
		{
			checkPondMap = false;
			checkCnt = 0;
		}
	}
}

void PondMake::Render(HDC hdc)
{
	if (checkPondMap == true)
	{
		// �� ���
		for (int i = 0; i < ySize; i++)
		{
			for (int j = 0; j < xSize; j++)
			{
				wsprintf(szText, "%d", map[j + (xSize * i)]);
				TextOut(hdc, 200 + j * 20, i * 20 + 40, szText, strlen(szText));
			}
		}

		// ���� ��ġ ���
		for (int i = 0; i < ySize + 2; i++)
		{
			for (int j = 0; j < xSize + 2; j++)
			{
				wsprintf(szText, "%d", visited[j + ((xSize + 2) * i)]);
				TextOut(hdc, 700 + j * 20, i * 20 + 20, szText, strlen(szText));
			}
		}
	}

	// ���� �׸���
	for (int i = startFrame.y; i <= startFrame.y + WINSIZE_Y / F_TILESIZE + 1; i++)        // ����
	{
		for (int j = startFrame.x; j <= startFrame.x + WINSIZE_X / F_TILESIZE + 1; j++)    // ����
		{
			int tempIndex = i * FARM_TILE_X + j;
			int tempPosX = -((int)renderCoor.x % F_TILESIZE) + F_TILESIZE * (j - startFrame.x);
			int tempPosY = -((int)renderCoor.y % F_TILESIZE) + F_TILESIZE * (i - startFrame.y);
			
			// ����
			if (farmTileInfo[tempIndex].tileType == TileType::PONDMADE)
			{
				pondSample->FrameRender(hdc,
					tempPosX,
					tempPosY,
					farmTileInfo[tempIndex].objFrameX,
					farmTileInfo[tempIndex].objFrameY);
			}

			// �׵θ�
			if (farmTileInfo[tempIndex].tileType == TileType::PONDEDGE)
			{
				pondEdgeSample->FrameRender(hdc,
					tempPosX,
					tempPosY,
					farmTileInfo[tempIndex].objFrameX,
					farmTileInfo[tempIndex].objFrameY);
			}
		}
	}
}
