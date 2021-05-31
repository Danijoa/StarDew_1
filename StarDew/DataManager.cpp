//#include "config.h"
#include "DataManager.h"
#include "Image.h"
#include "InventoryManager.h"

TILE_INFO DataManager::farmTileInfo[FARM_TILE_X * FARM_TILE_Y];

HRESULT DataManager::Init()
{
    // 0:시작 1:집 2:마당 3:상점 4: 로딩
    preScene = 0;

    // 농장 타일 로드
    loadFarmScene(2);

    return S_OK;
}

void DataManager::Release()
{
}

void DataManager::testDry()
{
	for (int i = 0; i <= FARM_TILE_Y; i++)        // 세로
	{
		for (int j = 0; j <= FARM_TILE_X; j++)    // 가로
		{
			int tempIndex = i * FARM_TILE_X + j;

			// 물 준 곳
			if (farmTileInfo[tempIndex].tileType == TileType::WETDIG)
			{
				// 건조
				farmTileInfo[tempIndex].tileType = TileType::DIG;

				// 혼자
				if (farmTileInfo[tempIndex].objFrameX == 0 &&
					farmTileInfo[tempIndex].objFrameY == 5)
				{
					farmTileInfo[tempIndex].objFrameX = 0;
					farmTileInfo[tempIndex].objFrameY = 2;
				}

				// 상
				if (farmTileInfo[tempIndex].objFrameX == 2 &&
					farmTileInfo[tempIndex].objFrameY == 6)
				{
					farmTileInfo[tempIndex].objFrameX = 2;
					farmTileInfo[tempIndex].objFrameY = 3;
				}

				// 하
				if (farmTileInfo[tempIndex].objFrameX == 0 &&
					farmTileInfo[tempIndex].objFrameY == 6)
				{
					farmTileInfo[tempIndex].objFrameX = 0;
					farmTileInfo[tempIndex].objFrameY = 3;
				}

				// 좌
				if (farmTileInfo[tempIndex].objFrameX == 3 &&
					farmTileInfo[tempIndex].objFrameY == 6)
				{
					farmTileInfo[tempIndex].objFrameX = 3;
					farmTileInfo[tempIndex].objFrameY = 3;
				}

				// 우
				if (farmTileInfo[tempIndex].objFrameX == 1 &&
					farmTileInfo[tempIndex].objFrameY == 6)
				{
					farmTileInfo[tempIndex].objFrameX = 1;
					farmTileInfo[tempIndex].objFrameY = 3;
				}

				// 상 하
				if (farmTileInfo[tempIndex].objFrameX == 1 &&
					farmTileInfo[tempIndex].objFrameY == 5)
				{
					farmTileInfo[tempIndex].objFrameX = 1;
					farmTileInfo[tempIndex].objFrameY = 2;
				}

				// 좌 우
				if (farmTileInfo[tempIndex].objFrameX == 2 &&
					farmTileInfo[tempIndex].objFrameY == 5)
				{
					farmTileInfo[tempIndex].objFrameX = 2;
					farmTileInfo[tempIndex].objFrameY = 2;
				}

				// 상 우
				if (farmTileInfo[tempIndex].objFrameX == 1 &&
					farmTileInfo[tempIndex].objFrameY == 7)
				{
					farmTileInfo[tempIndex].objFrameX = 1;
					farmTileInfo[tempIndex].objFrameY = 4;
				}

				// 상 좌
				if (farmTileInfo[tempIndex].objFrameX == 2 &&
					farmTileInfo[tempIndex].objFrameY == 7)
				{
					farmTileInfo[tempIndex].objFrameX = 2;
					farmTileInfo[tempIndex].objFrameY = 4;
				}

				// 하 우
				if (farmTileInfo[tempIndex].objFrameX == 0 &&
					farmTileInfo[tempIndex].objFrameY == 7)
				{
					farmTileInfo[tempIndex].objFrameX = 0;
					farmTileInfo[tempIndex].objFrameY = 4;
				}

				// 하 좌
				if (farmTileInfo[tempIndex].objFrameX == 3 &&
					farmTileInfo[tempIndex].objFrameY == 7)
				{
					farmTileInfo[tempIndex].objFrameX = 3;
					farmTileInfo[tempIndex].objFrameY = 4;
				}

				// 상 우 하
				if (farmTileInfo[tempIndex].objFrameX == 0 &&
					farmTileInfo[tempIndex].objFrameY == 0)
				{
					farmTileInfo[tempIndex].objFrameX = 2;
					farmTileInfo[tempIndex].objFrameY = 0;
				}

				// 상 우 좌
				if (farmTileInfo[tempIndex].objFrameX == 2 &&
					farmTileInfo[tempIndex].objFrameY == 1)
				{
					farmTileInfo[tempIndex].objFrameX = 3;
					farmTileInfo[tempIndex].objFrameY = 2;
				}

				// 상 하 좌
				if (farmTileInfo[tempIndex].objFrameX == 1 &&
					farmTileInfo[tempIndex].objFrameY == 1)
				{
					farmTileInfo[tempIndex].objFrameX = 3;
					farmTileInfo[tempIndex].objFrameY = 1;
				}

				// 하 우 좌
				if (farmTileInfo[tempIndex].objFrameX == 1 &&
					farmTileInfo[tempIndex].objFrameY == 0)
				{
					farmTileInfo[tempIndex].objFrameX = 3;
					farmTileInfo[tempIndex].objFrameY = 0;
				}

				// 상 하 좌 우
				if (farmTileInfo[tempIndex].objFrameX == 0 &&
					farmTileInfo[tempIndex].objFrameY == 1)
				{
					farmTileInfo[tempIndex].objFrameX = 3;
					farmTileInfo[tempIndex].objFrameY = 5;
				}
			}
		}
	}
}

void DataManager::Update()
{
	if (InventoryManager::GetSingleton()->GetDayCheck() == true)
	{
		// 물 뿌린 곳만 자라 날 수 있도록
		for (int i = 0; i <= FARM_TILE_Y; i++)        // 세로
		{
			for (int j = 0; j <= FARM_TILE_X; j++)    // 가로
			{
				int tempIndex = i * FARM_TILE_X + j;

				// 곡식
				if (farmTileInfo[tempIndex].seedType == SeedType::CROP)
				{
					// 물 줬니..?
					if (farmTileInfo[tempIndex].tileType != TileType::WETDIG)
					{
						farmTileInfo[tempIndex].day += 1;
					}
				}
			}
		}

		// 땅 마르기
		testDry();

		// day 갱신 확인 완료
		InventoryManager::GetSingleton()->SetDayCheck(false);
	}
}

void DataManager::loadFarmScene(int sceneNum)
{
    //맵 이미지 load
    string fileName = "Save/saveMapData";
    fileName += to_string(sceneNum) + ".map";
    HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD readBytes;
    if (ReadFile(hFile, farmTileInfo, sizeof(TILE_INFO) * FARM_TILE_X * FARM_TILE_Y, &readBytes, NULL)) {}
    else
        MessageBox(g_hWnd, "저장파일 로드 실패", "실패", MB_OK);
    CloseHandle(hFile);
}

