#include "HouseScene.h"
#include "Image.h"
#include "DataManager.h"
#include "InventoryManager.h"
#include "Player.h"

TILE_INFO HouseScene::houseTileInfo[HOUSE_TILE_X * HOUSE_TILE_Y];

HRESULT HouseScene::Init()
{
    // ũ�� ����
    //SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);

    // �� ���� �̹���
    ImageManager::GetSingleton()->AddImage("���̹���", "Image/House.bmp", 1408, 768, HOUSE_TILE_X, HOUSE_TILE_Y);
    houseSample = ImageManager::GetSingleton()->FindImage("���̹���");

    loadHouseScene(1);

    // �÷��̾�
    player = new Player();
    player->Init();
    if (DataManager::GetSingleton()->GetPreScene() == 0)    // Ÿ��Ʋ���� ��ȯ�� ��� or �׽�Ʈ
        player->SetPlayerImagePos({ 300, 350 });
    if (DataManager::GetSingleton()->GetPreScene() == 2)    // ���翡�� ��ȯ�� ���
        player->SetPlayerImagePos({ 423, 550 });
    player->SetCurrImageSize({ houseSample->GetWidth(), houseSample->GetHeight() });

    // ���� ���� (ȭ��� �������� �̹��� ���� ��ġ)
    renderCoor.x = player->GetPlayerImagePos().x - WINSIZE_X / 2;
    renderCoor.y = player->GetPlayerImagePos().y - WINSIZE_Y / 2;

    // �� Ȯ��
    ImageManager::GetSingleton()->AddImage("��ڽ�", "Image/sleepBox.bmp", 184, 44, true, RGB(255, 255, 255));
    sleepBox = ImageManager::GetSingleton()->FindImage("��ڽ�");
    checkSleep = false;

    //
    houseDir = 1;
    canMove = true;

    return S_OK;
}

void HouseScene::Release()
{
}

void HouseScene::MoveControl()
{
    if (renderCoor.x < 0)
        renderCoor.x = 0;
    if (renderCoor.x > houseSample->GetWidth() - WINSIZE_X)
        renderCoor.x = houseSample->GetWidth() - WINSIZE_X;
    if (renderCoor.y < 0)
        renderCoor.y = 0;
    if (renderCoor.y > houseSample->GetHeight() - WINSIZE_Y)
        renderCoor.y = houseSample->GetHeight() - WINSIZE_Y;
}

void HouseScene::Move()
{
    if (KeyManager::GetSingleton()->IsStayKeyDown('W'))			// ��
        houseDir = 0;
    else if (KeyManager::GetSingleton()->IsStayKeyDown('S'))	// ��
        houseDir = 1;
    else if (KeyManager::GetSingleton()->IsStayKeyDown('A'))	// ��
        houseDir = 2;
    else if (KeyManager::GetSingleton()->IsStayKeyDown('D'))	// ��
        houseDir = 3;
    playerHouseCollision();
}

void HouseScene::playerHouseCollision()
{
    int left = (player->GetPlayerFutureRect().left + renderCoor.x) / TILESIZE;
    int top = (player->GetPlayerFutureRect().top + renderCoor.y) / TILESIZE;
    int right = (player->GetPlayerFutureRect().right + renderCoor.x) / TILESIZE;
    int bottom = (player->GetPlayerFutureRect().bottom + renderCoor.y) / TILESIZE;

    switch (houseDir)
    {
    case 0:
        if (houseTileInfo[left + top * HOUSE_TILE_X].tileType == TileType::WALL)	// left,top
            canMove = false;
        if (houseTileInfo[right + top * HOUSE_TILE_X].tileType == TileType::WALL)	// right,top
            canMove = false;
        break;
    case 1:
        // ��->���� �� ����
        if (houseTileInfo[left + bottom * HOUSE_TILE_X].tileType == TileType::HOUSEDOOR ||
            houseTileInfo[right + bottom * HOUSE_TILE_X].tileType == TileType::HOUSEDOOR)
        {
            DataManager::GetSingleton()->SetPreScene(1); // ���� ��(��=1) ����
            SceneManager::GetSingleton()->ChangeScene("�����");
        }
        if (houseTileInfo[left + bottom * HOUSE_TILE_X].tileType == TileType::WALL)	// left,bottom
            canMove = false;
        if (houseTileInfo[right + bottom * HOUSE_TILE_X].tileType == TileType::WALL)	// right,bottom
            canMove = false;
        break;
    case 2:
        // �����
        if (player->GetPlayerRect().left > 320 && player->GetPlayerRect().left < 400
            && player->GetPlayerRect().top > 330 && player->GetPlayerRect().top < 380)
        {
            checkSleep = true;
        }
        if (houseTileInfo[left + top * HOUSE_TILE_X].tileType == TileType::WALL)	// left,top
            canMove = false;
        if (houseTileInfo[left + bottom * HOUSE_TILE_X].tileType == TileType::WALL)	// left,bottom
            canMove = false;
        break;
    case 3:
        if (houseTileInfo[right + top * HOUSE_TILE_X].tileType == TileType::WALL)	// right,top
            canMove = false;
        if (houseTileInfo[right + bottom * HOUSE_TILE_X].tileType == TileType::WALL)	// right,bottom
            canMove = false;
        break;
    default:
        break;
    }

    player->SetCanMove(canMove);
}

void HouseScene::Update()
{
    player->Update();

    // ȭ�� �̵� + �÷��̾� �浹 üũ
    canMove = true;
    Move();

    // ���� ���� ��ġ
    renderCoor.x = player->GetPlayerImagePos().x - WINSIZE_X / 2;
    renderCoor.y = player->GetPlayerImagePos().y - WINSIZE_Y / 2;

    // ȭ�� �̵� ����
    MoveControl();

    // ���� ���� �ε���
    startFrame.x = renderCoor.x / TILESIZE;
    startFrame.y = renderCoor.y / TILESIZE;
}

void HouseScene::Render(HDC hdc)
{
    // �ڿ� ���� �������� ĥ�ϱ�
    PatBlt(hdc, 0, 0, WINSIZE_X, WINSIZE_Y, BLACKNESS);

    // �Ͽ콺 ���� Ÿ�� ���
    for (int i = startFrame.y; i <= startFrame.y + HOUSE_TILE_Y; i++)        // ����
    {
        for (int j = startFrame.x; j <= startFrame.x + HOUSE_TILE_X; j++)    // ����
        {
            int tempIndex = i * HOUSE_TILE_X + j;
            int tempPosX = -((int)renderCoor.x % TILESIZE) + TILESIZE * (j - startFrame.x);
            int tempPosY = -((int)renderCoor.y % TILESIZE) + TILESIZE * (i - startFrame.y);

            houseSample->FrameRender(hdc,
                tempPosX,
                tempPosY,
                houseTileInfo[tempIndex].frameX,
                houseTileInfo[tempIndex].frameY);

            if (houseTileInfo[tempIndex].tileType == TileType::WALL)
            {
                //Rectangle(hdc, tempPosX, tempPosY,
                    //tempPosX + TILESIZE, tempPosY + TILESIZE);
            }

        }
    }

    // �÷��̾�
    player->Render(hdc);

    if (checkSleep)
    {
        sleepBox->Render(hdc, WINSIZE_X / 2 - sleepBox->GetWidth() / 2, WINSIZE_Y - 100);
        player->SetCanMove(false);

        /*yes ������*/
        //player->SetCanMove(true);
        //InventoryManager::GetSingleton()->SetDay();         // day ����
        //InventoryManager::GetSingleton()->SetReEnergy();    // ü�� ����
        //DataManager::GetSingleton()->SetPreScene(0);        // Ÿ��Ʋ ������ ���� �ε� �ϴ� �� ó�� ����
        //SceneManager::GetSingleton()->ChangeScene("�Ͽ콺��");

        /*no ������*/
        //player->SetCanMove(true);
    }
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