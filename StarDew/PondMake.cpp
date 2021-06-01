#include "PondMake.h"
#include "Image.h"
#include<cstdlib>
#include<ctime>

HRESULT PondMake::Init()
{
    // 연못 샘플
	ImageManager::GetSingleton()->AddImage("연못샘플", "Image/testPond.bmp", 128, 128, 1, 1);
	pondSample = ImageManager::GetSingleton()->FindImage("연못샘플");

    // 맵 생성
    //xSize = FARM_TILE_X;										//가로
    //ySize = FARM_TILE_Y;										//세로
	xSize = 10;										//가로
	ySize = 10;										//세로
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
	visited = (bool*)malloc(sizeof(bool) * xSize * ySize);
	for (int i = 0; i < ySize; i++)
	{
		for (int j = 0; j < xSize; j++)
		{
			visited[j + (xSize * i)] = false;
		}
	}

	// bfs 방향
	dirY[0] = { -1 }, dirY[1] = { 1 }, dirY[2] = { 0 }, dirY[3] = { 0 };	// 상 하 - -
	dirX[0] = { 0 }, dirX[1] = { 0 }, dirX[2] = { -1 }, dirX[3] = { 1 };	// - - 좌 우
	
    return S_OK;
}

void PondMake::Release()
{
}

void PondMake::BFS(int startX, int startY, int xSize, int ySize, int* map, bool* visited)
{
	// 시작점 큐에 넣어주고 방문 확인
	int startIndex = startX + xSize * startY;
	myQ.push({ startX , startY });
	visited[startIndex] = true;

	// 시작전 값
	int mapNum = map[startIndex];

	// 큐 안에 있는 값 4방 확인하면서 같은 값 있으면 체크
	while (!myQ.empty())
	{
		// 큐에 있는 값 꺼내고 삭제
		int curX = myQ.front().x;
		int curY = myQ.front().y;
		int curIndex = curX + xSize * curY;
		int curNum = map[curIndex];
		myQ.pop();

		// 4방 확인
		int nextIndex;
		for (int i = 0; i < 4; i++)
		{
			int nextX = curX + dirX[i];
			int nextY = curY + dirY[i];
			int nextIndex = nextX + xSize * nextY;

			// 범위 벗어나면 스킵
			if (nextX < 0 || nextX >= xSize || nextY < 0 || nextY >= ySize)
				continue;

			// 방문한적 없고 && 현재 값이랑 같으면
			if (mapNum == curNum)
			{
				if (visited[nextIndex] == false && (curNum == map[nextIndex] || (curNum - 1) == map[nextIndex] || (curNum - 2) == map[nextIndex]))
				{
					// 큐에 넣어주고 방문 체크
					myQ.push({ nextX, nextY });
					visited[nextIndex] = true;
				}
			}
			else if ((mapNum - 1 == curNum) || (mapNum - 2 == curNum))
			{
				if (visited[nextIndex] == false && (curNum == map[nextIndex]))
				{
					// 큐에 넣어주고 방문 체크
					myQ.push({ nextX, nextY });
					visited[nextIndex] = true;
				}
			}
		}
	}
}

void PondMake::ReFloodFill(int splitDir, int point, int increaseDir, int* map, int startX, int startY, int endX, int endY, int xSize)
{
	// 크기 2이하로 작아지면 재귀 멈추자
	if (splitDir == 0)	// 가로
	{
		if ((point - startY) <= 3 || (endY - point) <= 3)
			return;
	}
	else if (splitDir == 1)	//세로
	{
		if ((point - startX) <= 3 || (endX - point) <= 3)
			return;
	}

	// 분할 시작
	if (splitDir == 0)	// 가로로 자르자
	{
		if (increaseDir == 0)	// 윗 덩어리 증가	
		{
			for (int i = startY; i < point; i++)
			{
				for (int j = startX; j < endX; j++)
				{
					map[j + (xSize * i)] += 1;
				}
			}
		}
		else if (increaseDir == 1)	// 아래 덩어리 증가
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
		if (increaseDir == 2)	// 왼쪽 덩어리 증가
		{
			for (int i = startY; i < endY; i++)
			{
				for (int j = startX; j < point; j++)
				{
					map[j + (xSize * i)] += 1;
				}
			}
		}
		else if (increaseDir == 3)	// 오른쪽 덩어리 증가
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

			if (i == 0)
			{
				if (reSplitDir == 0)	// 가로
					rePoint = (rand() % (((point - 1) - startY) + 1)) + startY; //startY ~ (point-1)
				else if (reSplitDir == 1)	// 세로
					rePoint = (rand() % (((endX - 1) - startX) + 1)) + startX; // startX ~ (endX-1)

				ReFloodFill(reSplitDir, rePoint, reIncreaseDir, map, startX, startY, endX, point, xSize);	// 윗부분
			}
			else if (i == 1)
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

			if (i == 0)
			{
				if (reSplitDir == 0)	// 가로
					rePoint = (rand() % (((endY - 1) - startY) + 1)) + startY; // startY ~ (endY-1)
				else if (reSplitDir == 1)	// 세로
					rePoint = (rand() % (((point - 1) - startX) + 1)) + startX; // startX ~ (point-1)

				ReFloodFill(reSplitDir, rePoint, reIncreaseDir, map, startX, startY, point, endY, xSize);	// 왼쪽 부분
			}
			else if (i == 1)
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

void PondMake::Update()
{
    // 재귀
    srand((unsigned int)time(NULL));
    int splitDir = 0, point = 0, increaseDir = 0;
    if (KeyManager::GetSingleton()->IsOnceKeyDown('I'))
    {
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
		// 초기화
		for (int i = 0; i < ySize; i++)
		{
			for (int j = 0; j < xSize; j++)
			{
				visited[j + (xSize * i)] = false;
			}
		}

		// 일단 bfs
		BFS(startX, startY, xSize, ySize, map, visited);
    }
}

void PondMake::Render(HDC hdc)
{
	// 맵 출력
	for (int i = 0; i < ySize; i++)
	{
		for (int j = 0; j < xSize; j++)
		{
			wsprintf(szText, "%d", map[j + (xSize * i)]);
			TextOut(hdc, 200 + j * 20, i * 20, szText, strlen(szText));
		}
	}

	// 연못 위치 출력
	for (int i = 0; i < ySize; i++)
	{
		for (int j = 0; j < xSize; j++)
		{
			wsprintf(szText, "%d", visited[j + (xSize * i)]);
			TextOut(hdc, 700 + j * 20, i * 20, szText, strlen(szText));
		}
	}

	// 연못 그리자
	for (int i = 0; i < ySize; i++)			// 세로
	{
		for (int j = 0; j < xSize; j++)    // 가로
		{
			int tempIndex = i * xSize + j;
			int tempPosX = j * F_TILESIZE;
			int tempPosY = i * F_TILESIZE;
			
			// 농장 배경
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
