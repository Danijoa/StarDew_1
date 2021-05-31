//#include "config.h"
#include "DataManager.h"
#include "Image.h"
#include "InventoryManager.h"

TILE_INFO DataManager::farmTileInfo[FARM_TILE_X * FARM_TILE_Y];

HRESULT DataManager::Init()
{
    // 0:���� 1:�� 2:���� 3:���� 4: �ε�
    preScene = 0;

    // ���� Ÿ�� �ε�
    loadFarmScene(2);

    return S_OK;
}

void DataManager::Release()
{
}

void DataManager::testDry()
{
	for (int i = 0; i <= FARM_TILE_Y; i++)        // ����
	{
		for (int j = 0; j <= FARM_TILE_X; j++)    // ����
		{
			int tempIndex = i * FARM_TILE_X + j;

			// �� �� ��
			if (farmTileInfo[tempIndex].tileType == TileType::WETDIG)
			{
				// ����
				farmTileInfo[tempIndex].tileType = TileType::DIG;

				// ȥ��
				if (farmTileInfo[tempIndex].objFrameX == 0 &&
					farmTileInfo[tempIndex].objFrameY == 5)
				{
					farmTileInfo[tempIndex].objFrameX = 0;
					farmTileInfo[tempIndex].objFrameY = 2;
				}

				// ��
				if (farmTileInfo[tempIndex].objFrameX == 2 &&
					farmTileInfo[tempIndex].objFrameY == 6)
				{
					farmTileInfo[tempIndex].objFrameX = 2;
					farmTileInfo[tempIndex].objFrameY = 3;
				}

				// ��
				if (farmTileInfo[tempIndex].objFrameX == 0 &&
					farmTileInfo[tempIndex].objFrameY == 6)
				{
					farmTileInfo[tempIndex].objFrameX = 0;
					farmTileInfo[tempIndex].objFrameY = 3;
				}

				// ��
				if (farmTileInfo[tempIndex].objFrameX == 3 &&
					farmTileInfo[tempIndex].objFrameY == 6)
				{
					farmTileInfo[tempIndex].objFrameX = 3;
					farmTileInfo[tempIndex].objFrameY = 3;
				}

				// ��
				if (farmTileInfo[tempIndex].objFrameX == 1 &&
					farmTileInfo[tempIndex].objFrameY == 6)
				{
					farmTileInfo[tempIndex].objFrameX = 1;
					farmTileInfo[tempIndex].objFrameY = 3;
				}

				// �� ��
				if (farmTileInfo[tempIndex].objFrameX == 1 &&
					farmTileInfo[tempIndex].objFrameY == 5)
				{
					farmTileInfo[tempIndex].objFrameX = 1;
					farmTileInfo[tempIndex].objFrameY = 2;
				}

				// �� ��
				if (farmTileInfo[tempIndex].objFrameX == 2 &&
					farmTileInfo[tempIndex].objFrameY == 5)
				{
					farmTileInfo[tempIndex].objFrameX = 2;
					farmTileInfo[tempIndex].objFrameY = 2;
				}

				// �� ��
				if (farmTileInfo[tempIndex].objFrameX == 1 &&
					farmTileInfo[tempIndex].objFrameY == 7)
				{
					farmTileInfo[tempIndex].objFrameX = 1;
					farmTileInfo[tempIndex].objFrameY = 4;
				}

				// �� ��
				if (farmTileInfo[tempIndex].objFrameX == 2 &&
					farmTileInfo[tempIndex].objFrameY == 7)
				{
					farmTileInfo[tempIndex].objFrameX = 2;
					farmTileInfo[tempIndex].objFrameY = 4;
				}

				// �� ��
				if (farmTileInfo[tempIndex].objFrameX == 0 &&
					farmTileInfo[tempIndex].objFrameY == 7)
				{
					farmTileInfo[tempIndex].objFrameX = 0;
					farmTileInfo[tempIndex].objFrameY = 4;
				}

				// �� ��
				if (farmTileInfo[tempIndex].objFrameX == 3 &&
					farmTileInfo[tempIndex].objFrameY == 7)
				{
					farmTileInfo[tempIndex].objFrameX = 3;
					farmTileInfo[tempIndex].objFrameY = 4;
				}

				// �� �� ��
				if (farmTileInfo[tempIndex].objFrameX == 0 &&
					farmTileInfo[tempIndex].objFrameY == 0)
				{
					farmTileInfo[tempIndex].objFrameX = 2;
					farmTileInfo[tempIndex].objFrameY = 0;
				}

				// �� �� ��
				if (farmTileInfo[tempIndex].objFrameX == 2 &&
					farmTileInfo[tempIndex].objFrameY == 1)
				{
					farmTileInfo[tempIndex].objFrameX = 3;
					farmTileInfo[tempIndex].objFrameY = 2;
				}

				// �� �� ��
				if (farmTileInfo[tempIndex].objFrameX == 1 &&
					farmTileInfo[tempIndex].objFrameY == 1)
				{
					farmTileInfo[tempIndex].objFrameX = 3;
					farmTileInfo[tempIndex].objFrameY = 1;
				}

				// �� �� ��
				if (farmTileInfo[tempIndex].objFrameX == 1 &&
					farmTileInfo[tempIndex].objFrameY == 0)
				{
					farmTileInfo[tempIndex].objFrameX = 3;
					farmTileInfo[tempIndex].objFrameY = 0;
				}

				// �� �� �� ��
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
		// �� �Ѹ� ���� �ڶ� �� �� �ֵ���
		for (int i = 0; i <= FARM_TILE_Y; i++)        // ����
		{
			for (int j = 0; j <= FARM_TILE_X; j++)    // ����
			{
				int tempIndex = i * FARM_TILE_X + j;

				// ���
				if (farmTileInfo[tempIndex].seedType == SeedType::CROP)
				{
					// �� ���..?
					if (farmTileInfo[tempIndex].tileType != TileType::WETDIG)
					{
						farmTileInfo[tempIndex].day += 1;
					}
				}
			}
		}

		// �� ������
		testDry();

		// day ���� Ȯ�� �Ϸ�
		InventoryManager::GetSingleton()->SetDayCheck(false);
	}
}

void DataManager::loadFarmScene(int sceneNum)
{
    //�� �̹��� load
    string fileName = "Save/saveMapData";
    fileName += to_string(sceneNum) + ".map";
    HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD readBytes;
    if (ReadFile(hFile, farmTileInfo, sizeof(TILE_INFO) * FARM_TILE_X * FARM_TILE_Y, &readBytes, NULL)) {}
    else
        MessageBox(g_hWnd, "�������� �ε� ����", "����", MB_OK);
    CloseHandle(hFile);
}

