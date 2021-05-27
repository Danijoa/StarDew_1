//#include "config.h"
#include "DataManager.h"
#include "Image.h"

TILE_INFO DataManager::farmTileInfo[FARM_TILE_X * FARM_TILE_Y];

HRESULT DataManager::Init()
{
    // 0:시작 1:집 2:마당 3:상점
    preScene = 0;

    // 농장 타일 로드
    loadFarmScene(2);

    return S_OK;
}

void DataManager::Release()
{
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

