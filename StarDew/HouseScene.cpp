#include "HouseScene.h"
#include "Image.h"
#include "Player.h"

TILE_INFO HouseScene::houseTileInfo[HOUSE_TILE_X * HOUSE_TILE_Y];

HRESULT HouseScene::Init()
{
    // ũ�� ����
    //SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);

    // �� ���� �̹���
    ImageManager::GetSingleton()->AddImage("���̹���", "Image/House.bmp", 800, 832, HOUSE_TILE_X, HOUSE_TILE_Y);	
    houseSample = ImageManager::GetSingleton()->FindImage("���̹���");

    // �÷��̾�
    player = new Player();
    player->Init();

    return S_OK;
}

void HouseScene::Release()
{
}

void HouseScene::Update()
{
    loadHouseScene(1);
    player->Update();
    player->SetHouseTileInfo(houseTileInfo);
    player->SetSceneNum(1);
}

void HouseScene::Render(HDC hdc)
{
    // �ڿ� ���� �������� ĥ�ϱ�
    PatBlt(hdc, 0, 0, WINSIZE_X, WINSIZE_Y, BLACKNESS);

    // �Ͽ콺 ���� Ÿ�� ���
    for (int i = 0; i < HOUSE_TILE_X * HOUSE_TILE_Y; i++)
    {
        houseSample->FrameRender(hdc,
            houseTileInfo[i].rcTile.left + 300,
            houseTileInfo[i].rcTile.top - 45,
            houseTileInfo[i].frameX,
            houseTileInfo[i].frameY);

        if (houseTileInfo[i].tileType == TileType::WALL)
            Rectangle(hdc, houseTileInfo[i].rcTile.left + 300, houseTileInfo[i].rcTile.top - 45, 
                houseTileInfo[i].rcTile.left + 300 + TILESIZE, houseTileInfo[i].rcTile.top - 45 + TILESIZE);
    }

    // �÷��̾�
    player->Render(hdc);
}

void HouseScene::loadHouseScene(int sceneNum)
{
    //�� �̹��� load
    string fileName = "Save/saveMapData";
    fileName += to_string(sceneNum) + ".map";
    HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD readBytes;
    if (ReadFile(hFile, houseTileInfo, sizeof(TILE_INFO) * HOUSE_TILE_X * HOUSE_TILE_Y, &readBytes, NULL)) {}
    else
        MessageBox(g_hWnd, "�������� �ε� ����", "����", MB_OK);
    CloseHandle(hFile);
}
