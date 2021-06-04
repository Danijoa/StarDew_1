#include "PondMake.h"
#include "DataManager.h"
#include "Image.h"
#include<cstdlib>
#include<ctime>
#define MAX FARM_TILE_X

HRESULT PondMake::Init()
{
	// 타일 정보 가져오기
	farmTileInfo = DataManager::GetSingleton()->GetFarmScene();

	// 플레이어 위치 초시화 해두기
	curPlayerRectPos = { 0,0 };

	// 타일 렌더 위치 초기화
	renderCoor = { 0,0 };
	startFrame = { 0,0 };

    // 연못 샘플 
	ImageManager::GetSingleton()->AddImage("연못샘플", "Image/testPond.bmp", 64 * 6, 64, 6, 1);	// 깊이 표현은 6개로
	pondSample = ImageManager::GetSingleton()->FindImage("연못샘플");

	ImageManager::GetSingleton()->AddImage("연못테두리샘플", "Image/testPondEdge.bmp", 64 * 12, 64, 12, 1);	// 8+4개 경우의 수
	pondEdgeSample = ImageManager::GetSingleton()->FindImage("연못테두리샘플");

    // 맵 생성
	xSize = 14;										//가로
	ySize = 14;										//세로
    map = (int*)malloc(sizeof(int) * xSize * ySize);

    // 맵 초기화
    for (int i = 0; i < ySize; i++)
    {
        for (int j = 0; j < xSize; j++)
        {
            map[j + (xSize * i)] = 0;
        }
    }

	// 시작점
	startX = xSize / 2;
	startY = ySize / 2;

	// 방문 생성 및 초기화
	visited = (int*)malloc(sizeof(int) * (xSize + 2) * (ySize + 2));
	for (int i = 0; i < ySize + 2; i++)
	{
		for (int j = 0; j < xSize + 2; j++)
		{
			visited[j + ((xSize + 2) * i)] = 0;
		}
	}

	// bfs 방향
	dirY[0] = { -1 }, dirY[1] = { 1 }, dirY[2] = { 0 }, dirY[3] = { 0 };	// 상 하 - -
	dirX[0] = { 0 }, dirX[1] = { 0 }, dirX[2] = { -1 }, dirX[3] = { 1 };	// - - 좌 우

	// 재귀 횟수
	recursionCnt = 0;

	// 맵 출력 확인
	checkCnt = 0;
	checkPondMap = false;
	
    return S_OK;
}

void PondMake::Release()
{
}

void PondMake::BFS(int startX, int startY, int xSize, int ySize, int* map, int* visited)
{
	// 깊이표현
	int depth = 32;	//pow(2,6)

	// 시작점 큐에 넣어주고 방문 확인
	int startIndex = startX + xSize * startY;
	myQ.push({ startX , startY, depth });
	int startIndexV = (startX + 1) + (xSize + 2) * (startY + 1);
	visited[startIndexV] = depth;	//depth

	// 시작전 값
	int mapNum = map[startIndex];

	// 큐 안에 있는 값 4방 확인하면서 같은 값 있으면 체크
	while (!myQ.empty())
	{
		// 큐에 있는 값 꺼내고 삭제
		int curX = myQ.front().x;
		int curY = myQ.front().y;
		int curDepth = myQ.front().depth;
		int curIndex = curX + xSize * curY;
		int curNum = map[curIndex];
		myQ.pop();

		// 부모 깊이 / 2
		depth = curDepth / 2;

		// 4방 확인
		int nextIndex;
		for (int i = 0; i < 4; i++)
		{
			int nextX = curX + dirX[i];
			int nextY = curY + dirY[i];
			int nextIndex = nextX + xSize * nextY; 
			int nextIndexV = (nextX + 1) + (xSize + 2) * (nextY + 1);

			// 범위 벗어나면 스킵
			if (nextX <= 0 || nextX >= xSize - 1 || nextY <= 0 || nextY >= ySize - 1)
				continue;

			/* 다 시 !!!!!!!!!!!!!!!!!!!! */
			// 방문 안했고 &&
			if (visited[nextIndexV] == 0 && abs(map[startIndex] - map[nextIndex]) < depth)
			{
				// 큐에 넣어주고 방문 체크
				myQ.push({ nextX, nextY, depth });
				visited[nextIndexV] = depth;
			}

			/*
			// 방문한적 없고 && (비교하는 값)이랑 같으면
			if (mapNum == curNum)
			{
				if (visited[nextIndex] == false &&
					(curNum == map[nextIndex] || (curNum - 1) == map[nextIndex] || (curNum + 1) == map[nextIndex]))
				{
					// 큐에 넣어주고 방문 체크
					myQ.push({ nextX, nextY, depth });
					visited[nextIndex] = depth;
				}
			}
			else if ((mapNum - 1 == curNum) || (mapNum + 1 == curNum))
			{
				if (visited[nextIndex] == false && (curNum == map[nextIndex]))
				{
					// 큐에 넣어주고 방문 체크
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
	// 재귀 할 수 있는지 먼저 확인하자
	bool left = true;
	bool right = true;
	bool top = true;
	bool bottom = true;

	// 크기 2이하로 작아지면 재귀 멈추자
	if (splitDir == 0)	// 가로
	{
		if (point - startY <= 1)
			top = false;
		if (endY - point <= 1)
			bottom = false;
	}
	else if (splitDir == 1)	//세로
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

	// 분할 시작
	if (splitDir == 0)	// 가로로 자르자
	{
		if (increaseDir == 0 && top == true)	// 윗 덩어리 증가	
		{
			for (int i = startY; i < point; i++)
			{
				for (int j = startX; j < endX; j++)
				{
					map[j + (xSize * i)] += 1;
				}
			}
		}
		else if (increaseDir == 1 && bottom == true)	// 아래 덩어리 증가
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
	else if (splitDir == 1)	// 세로로 자르자
	{
		if (increaseDir == 2 && left == true)	// 왼쪽 덩어리 증가
		{
			for (int i = startY; i < endY; i++)
			{
				for (int j = startX; j < point; j++)
				{
					map[j + (xSize * i)] += 1;
				}
			}
		}
		else if (increaseDir == 3 && right == true)	// 오른쪽 덩어리 증가
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

	// 나누어진 두 덩어리 다시 재귀
	int reSplitDir, rePoint, reIncreaseDir;
	if (splitDir == 0)
	{
		// 가로로 잘라진 경우
		for (int i = 0; i < 2; i++)
		{
			reSplitDir = (rand() % 2);

			if (reSplitDir == 0)	// 가로
				reIncreaseDir = (rand() % 2);
			else if (reSplitDir == 1)	// 세로
				reIncreaseDir = (rand() % ((3 - 2) + 1)) + 2;

			if (i == 0 && top == true)
			{
				if (reSplitDir == 0)	// 가로
					rePoint = (rand() % (((point - 1) - startY) + 1)) + startY; //startY ~ (point-1)
				else if (reSplitDir == 1)	// 세로
					rePoint = (rand() % (((endX - 1) - startX) + 1)) + startX; // startX ~ (endX-1)

				ReFloodFill(reSplitDir, rePoint, reIncreaseDir, map, startX, startY, endX, point, xSize);	// 윗부분
			}
			else if (i == 1 && bottom == true)
			{
				if (reSplitDir == 0)	// 가로
					rePoint = (rand() % (((endY - 1) - point) + 1)) + point; // point ~ (endY-1)
				else if (reSplitDir == 1)	// 세로
					rePoint = (rand() % (((endX - 1) - startX) + 1)) + startX; // startX ~ (endX-1)

				ReFloodFill(reSplitDir, rePoint, reIncreaseDir, map, startX, point, endX, endY, xSize);		//아래 부분
			}
		}
	}
	else if (splitDir == 1)
	{
		// 세로로 잘라진 경우
		for (int i = 0; i < 2; i++)
		{
			reSplitDir = (rand() % 2);

			if (reSplitDir == 0)	// 가로
				reIncreaseDir = (rand() % 2);
			else if (reSplitDir == 1)	// 세로
				reIncreaseDir = (rand() % ((3 - 2) + 1)) + 2;

			if (i == 0 && left == true)
			{
				if (reSplitDir == 0)	// 가로
					rePoint = (rand() % (((endY - 1) - startY) + 1)) + startY; // startY ~ (endY-1)
				else if (reSplitDir == 1)	// 세로
					rePoint = (rand() % (((point - 1) - startX) + 1)) + startX; // startX ~ (point-1)

				ReFloodFill(reSplitDir, rePoint, reIncreaseDir, map, startX, startY, point, endY, xSize);	// 왼쪽 부분
			}
			else if (i == 1 && right == true)
			{
				if (reSplitDir == 0)	// 가로
					rePoint = (rand() % (((endY - 1) - startY) + 1)) + startY; // startY ~ (endY-1)
				else if (reSplitDir == 1)	// 세로
					rePoint = (rand() % (((endX - 1) - point) + 1)) + point; // point ~ (endX-1)

				ReFloodFill(reSplitDir, rePoint, reIncreaseDir, map, point, startY, endX, endY, xSize);		// 오른쪽 부분
			}
		}
	}
}

void PondMake::SetPondEdgeTile(int* visited, INT_PAIR pondStartPoint, INT_PAIR curPlayerIndex)
{
	for (int i = 0; i < ySize + 2; i++)			// 세로
	{
		for (int j = 0; j < xSize + 2; j++)		// 가로
		{
			// 연못이면 패스
			if (visited[j + ((xSize + 2) * i)] != 0)
				continue;

			// (j, i)의 4방 확인
			int check = 0;
			for (int k = 0; k < 4; k++)
			{
				int checkX = j + dirX[k];
				int checkY = i + dirY[k];

				// 범위 벗어나면 확인 안한다
				if (checkX < 0 || checkX >= xSize + 2 || checkY < 0 || checkY >= ySize + 2)
					continue;

				if (visited[checkX + ((xSize + 2) * checkY)] != 0)
					check += pow(10, k);							// 상:1 하:10 좌:100 우:1000
			}

			// farm 이미지 인덱스 상에서 연못 테두리 그려질 인덱스
			INT_PAIR pondEdgeMakeIndex = { curPlayerIndex.x + (j - pondStartPoint.x + 1), curPlayerIndex.y + (i - pondStartPoint.y + 1) };

			// 현재 확인 하는 위치(j, i)의 상하좌우 모두 pond면 패스
			// 상
			if (check == 1)
			{
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].tileType = TileType::PONDEDGE;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameX = 1;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameY = 0;
			}
			// 하
			else if (check == 10)
			{
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].tileType = TileType::PONDEDGE;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameX = 0;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameY = 0;
			}
			// 좌
			else if (check == 100)
			{
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].tileType = TileType::PONDEDGE;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameX = 3;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameY = 0;
			}
			// 우
			else if (check == 1000)
			{
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].tileType = TileType::PONDEDGE;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameX = 2;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameY = 0;
			}
			// 상좌
			else if (check == 101)
			{
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].tileType = TileType::PONDEDGE;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameX = 6;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameY = 0;
			}
			// 상우
			else if (check == 1001)
			{
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].tileType = TileType::PONDEDGE;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameX = 7;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameY = 0;
			}
			// 하좌
			else if (check == 110)
			{
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].tileType = TileType::PONDEDGE;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameX = 4;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameY = 0;
			}
			// 하우
			else if (check == 1010)
			{
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].tileType = TileType::PONDEDGE;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameX = 5;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameY = 0;
			}
			// 상좌우
			else if (check == 1101)
			{
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].tileType = TileType::PONDEDGE;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameX = 8;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameY = 0;
			}
			// 하좌우
			else if (check == 1110)
			{
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].tileType = TileType::PONDEDGE;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameX = 9;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameY = 0;
			}
			// 상하좌
			else if (check == 111)
			{
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].tileType = TileType::PONDEDGE;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameX = 10;
				farmTileInfo[pondEdgeMakeIndex.x + (FARM_TILE_X * pondEdgeMakeIndex.y)].objFrameY = 0;
			}
			// 상하우
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
	// 연못 만들어지즌 사각형 시작점 구하기
	INT_PAIR pondStartPoint = { MAX , MAX };
	for (int i = 0; i < ySize + 2; i++)			// 세로
	{
		for (int j = 0; j < xSize + 2; j++)		// 가로
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

	// farm 이미지 인덱스 상에서 플레이어 인덱스
	INT_PAIR curPlayerIndex = { (curPlayerRectPos.first + renderCoor.x) / F_TILESIZE, (curPlayerRectPos.second + renderCoor.y) / F_TILESIZE };

	// 정보 저장
	for (int i = 0; i < ySize; i++)			// 세로
	{
		for (int j = 0; j < xSize; j++)		// 가로
		{
			// farm 이미지 인덱스 상에서 연못 그려질 인덱스
			INT_PAIR pondMakeIndex = { curPlayerIndex.x + (j - pondStartPoint.x + 1), curPlayerIndex.y + (i - pondStartPoint.y + 1) };

			// 깊이가 0 이면, 연못이 아님
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

	// 연못 테두리 
	SetPondEdgeTile(visited, pondStartPoint, curPlayerIndex);
}

void PondMake::Update()
{
	// map + visited + 횟수 재초기화 
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

    // 재귀
    srand((unsigned int)time(NULL));
    int splitDir = 0, point = 0, increaseDir = 0;
    if (KeyManager::GetSingleton()->IsOnceKeyDown('I'))
    {
		recursionCnt += 1;

        splitDir = (rand() % 2);	// (0: →) (1: ↓)

        if (splitDir == 0)	// 가로로 자르기
        {
            point = (rand() % ySize); // 0 ~ (ySize-1)
            increaseDir = (rand() % 2);	// (0: ↑) (1: ↓)
        }
        else if (splitDir == 1)	// 세로로 자르기
        {
            point = (rand() % xSize); // 0 ~ (xSize-1)
            increaseDir = (rand() % ((3 - 2) + 1)) + 2;	// (2: ←) (3: →)
        }

        // 제일 첫 분할은 맵 전체 크기를 대상으로 진행된다
        ReFloodFill(splitDir, point, increaseDir, map, 0, 0, xSize, ySize, xSize);
    }

    // 플러드필
    if (KeyManager::GetSingleton()->IsOnceKeyDown('U'))
    {
		// 방문 초기화
		for (int i = 0; i < ySize + 2; i++)
		{
			for (int j = 0; j < xSize + 2; j++)
			{
				visited[j + ((xSize + 2) * i)] = 0;
			}
		}

		// 일단 bfs
		BFS(startX, startY, xSize, ySize, map, visited);
    }

	// 연못 넣어주기
	if (KeyManager::GetSingleton()->IsOnceKeyDown('Y'))
	{
		SetPondTile(visited);
	}

	// 연못 맵 확인용
	if (KeyManager::GetSingleton()->IsOnceKeyDown('K'))
	{
		checkCnt += 1;
		if(checkCnt %2 == 1)	// 홀수면 출력
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
		// 맵 출력
		for (int i = 0; i < ySize; i++)
		{
			for (int j = 0; j < xSize; j++)
			{
				wsprintf(szText, "%d", map[j + (xSize * i)]);
				TextOut(hdc, 200 + j * 20, i * 20 + 40, szText, strlen(szText));
			}
		}

		// 연못 위치 출력
		for (int i = 0; i < ySize + 2; i++)
		{
			for (int j = 0; j < xSize + 2; j++)
			{
				wsprintf(szText, "%d", visited[j + ((xSize + 2) * i)]);
				TextOut(hdc, 700 + j * 20, i * 20 + 20, szText, strlen(szText));
			}
		}
	}

	// 연못 그리자
	for (int i = startFrame.y; i <= startFrame.y + WINSIZE_Y / F_TILESIZE + 1; i++)        // 세로
	{
		for (int j = startFrame.x; j <= startFrame.x + WINSIZE_X / F_TILESIZE + 1; j++)    // 가로
		{
			int tempIndex = i * FARM_TILE_X + j;
			int tempPosX = -((int)renderCoor.x % F_TILESIZE) + F_TILESIZE * (j - startFrame.x);
			int tempPosY = -((int)renderCoor.y % F_TILESIZE) + F_TILESIZE * (i - startFrame.y);
			
			// 연못
			if (farmTileInfo[tempIndex].tileType == TileType::PONDMADE)
			{
				pondSample->FrameRender(hdc,
					tempPosX,
					tempPosY,
					farmTileInfo[tempIndex].objFrameX,
					farmTileInfo[tempIndex].objFrameY);
			}

			// 테두리
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
