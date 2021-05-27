//#include "config.h"
#include "DataManager.h"
#include "Image.h"

TILE_INFO DataManager::farmTileInfo[FARM_TILE_X * FARM_TILE_Y];

HRESULT DataManager::Init()
{
    // 0:���� 1:�� 2:���� 3:����
    preScene = 0;

    // ���� Ÿ�� �ε�
    loadFarmScene(2);

    return S_OK;
}

void DataManager::Release()
{
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

