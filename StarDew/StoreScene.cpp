#include "StoreScene.h"
#include "Image.h"
#include "DataManager.h"
#include "Player.h"
#include "InventoryManager.h"
#include "StoreData.h"

TILE_INFO StoreScene::storeTileInfo[STORE_TILE_X * STORE_TILE_Y];

HRESULT StoreScene::Init()
{
    // ���� �̹��� ����
    ImageManager::GetSingleton()->AddImage("�����̹���", "Image/Store.bmp", 1408, 896, STORE_TILE_X, STORE_TILE_Y);
    storeSample = ImageManager::GetSingleton()->FindImage("�����̹���");
    loadStoreScene(3);

    // �÷��̾�
    player = new Player();
    player->Init();
    if (DataManager::GetSingleton()->GetPreScene() == 0)    // �׽�Ʈ
        player->SetPlayerImagePos({ 430, 600 });
    if (DataManager::GetSingleton()->GetPreScene() == 2)    // ���忡�� ��ȯ�� ���
        player->SetPlayerImagePos({ 430, 600 });
    player->SetCurrImageSize({ storeSample->GetWidth(), storeSample->GetHeight() });
     
    // ���� ���� (ȭ��� �������� �̹��� ���� ��ġ)
    renderCoor.x = player->GetPlayerImagePos().x - WINSIZE_X / 2;
    renderCoor.y = player->GetPlayerImagePos().y - WINSIZE_Y / 2;

    // ���� �Ǹ� 
    ImageManager::GetSingleton()->AddImage("��������Ʈ", "Image/StoreList.bmp", 800, 343, true, RGB(255, 255, 255));
    storeList = ImageManager::GetSingleton()->FindImage("��������Ʈ");
    ImageManager::GetSingleton()->AddImage("����Ʈ�ݱ�", "Image/listExit.bmp", 32, 32, true, RGB(255, 255, 255));
    listExit = ImageManager::GetSingleton()->FindImage("����Ʈ�ݱ�");

    // ���� �Ǹ� � 
    ImageManager::GetSingleton()->AddImage("����Ʈ����", "Image/CropList(2).bmp", 768, 702 , 1, 9);
    listBox = ImageManager::GetSingleton()->FindImage("����Ʈ����");

    // �� ����
    ImageManager::GetSingleton()->AddImage("������", "Image/moneyWarn.bmp", 480, 270, true, RGB(255, 255, 255));
    moneyWarn = ImageManager::GetSingleton()->FindImage("������");
    noMoney = false;
    moneyTimeCnt = 0.0f;

    // � ����
    storeData = new StoreData();
    storeData->Init();

    //
    storeDir = 0;
    player->SetStartFrameIndex({0, 2});
    canMove = true;
    listOpen = false;
    mouseIndex = { 0,0 };
    startListIndex = 0;
    listYSize = 78;
    renderListCoor = 0;
    moveSpeed = 18;

    return S_OK;
}

void StoreScene::Release()
{
}

void StoreScene::MoveControl()
{
    if (renderCoor.x < 0)
        renderCoor.x = 0;
    if (renderCoor.x > storeSample->GetWidth() - WINSIZE_X)
        renderCoor.x = storeSample->GetWidth() - WINSIZE_X;
    if (renderCoor.y < 0)
        renderCoor.y = 0;
    if (renderCoor.y > storeSample->GetHeight() - WINSIZE_Y)
        renderCoor.y = storeSample->GetHeight() - WINSIZE_Y;
}

void StoreScene::Move()
{
    if (KeyManager::GetSingleton()->IsStayKeyDown('W'))			// ��
        storeDir = 0;
    else if (KeyManager::GetSingleton()->IsStayKeyDown('S'))	// ��
        storeDir = 1;
    else if (KeyManager::GetSingleton()->IsStayKeyDown('A'))	// ��
        storeDir = 2;
    else if (KeyManager::GetSingleton()->IsStayKeyDown('D'))	// ��
        storeDir = 3;
    playerStoreCollision();
}

void StoreScene::playerStoreCollision()
{
    int left = (player->GetPlayerRect().left + renderCoor.x) / TILESIZE;
    int top = (player->GetPlayerRect().top + renderCoor.y) / TILESIZE;
    int right = (player->GetPlayerRect().right + renderCoor.x) / TILESIZE;
    int bottom = (player->GetPlayerRect().bottom + renderCoor.y) / TILESIZE;

    switch (storeDir)
    {
    case 0:
        if (storeTileInfo[left + top * STORE_TILE_X].tileType == TileType::WALL)	// left,top
            canMove = false;
        if (storeTileInfo[right + top * STORE_TILE_X].tileType == TileType::WALL)	// right,top
            canMove = false;
        if (storeTileInfo[left + top * STORE_TILE_X].tileType == TileType::COUNTER)	// left,top
            canMove = false;
        if (storeTileInfo[right + top * STORE_TILE_X].tileType == TileType::COUNTER)	// right,top
            canMove = false;
        break;
    case 1:
        // ����->���� �� ����
        if (storeTileInfo[left + bottom * STORE_TILE_X].tileType == TileType::STOREDOOR ||
            storeTileInfo[right + bottom * STORE_TILE_X].tileType == TileType::STOREDOOR)
        {
            DataManager::GetSingleton()->SetPreScene(3); // ���� ��(����=3) ����
            SceneManager::GetSingleton()->ChangeScene("�����");
        }
        if (storeTileInfo[left + bottom * STORE_TILE_X].tileType == TileType::WALL)	// left,bottom
            canMove = false;
        if (storeTileInfo[right + bottom * STORE_TILE_X].tileType == TileType::WALL)	// right,bottom
            canMove = false;
        break;
    case 2:
        if (storeTileInfo[left + top * STORE_TILE_X].tileType == TileType::WALL)	// left,top
            canMove = false;
        if (storeTileInfo[left + bottom * STORE_TILE_X].tileType == TileType::WALL)	// left,bottom
            canMove = false;
        if (storeTileInfo[left + top * STORE_TILE_X].tileType == TileType::COUNTER)	// left,top
            canMove = false;
        if (storeTileInfo[left + bottom * STORE_TILE_X].tileType == TileType::COUNTER)	// left,bottom
            canMove = false;
        break;
    case 3:
        if (storeTileInfo[right + top * STORE_TILE_X].tileType == TileType::WALL)	// right,top
            canMove = false;
        if (storeTileInfo[right + bottom * STORE_TILE_X].tileType == TileType::WALL)	// right,bottom
            canMove = false;
        break;
    default:
        break;
    }

    player->SetCanMove(canMove);
}

void StoreScene::Update()
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

    // ���� ����Ʈ Ʋ ���� 
    if (!listOpen && KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
    {
        mouseIndex.x = (int)(g_ptMouse.x + renderCoor.x) / TILESIZE;
        mouseIndex.y = (int)(g_ptMouse.y + renderCoor.y) / TILESIZE;

        if (storeTileInfo[mouseIndex.x + STORE_TILE_X * mouseIndex.y].tileType == TileType::COUNTER)
        {
            listOpen = true;
            InventoryManager::GetSingleton()->SetStoreListOpen(listOpen);
        }

        // �ݱ�
        if (g_ptMouse.x >= 1065 && g_ptMouse.x <= 1085 && g_ptMouse.y >= 105 && g_ptMouse.y <= 125)
        {
            renderListCoor = 0;
            listIndex = 0;
            listOpen = false;
            InventoryManager::GetSingleton()->SetStoreListOpen(listOpen);
        }
    }

    // ����Ʈ ���� ��ũ��
    if (listOpen)
    {
        if (InventoryManager::GetSingleton()->GetScrolledUp())
        {
            if (renderListCoor > 0)
                renderListCoor -= moveSpeed;
            InventoryManager::GetSingleton()->SetScrolledUp(false);
        }

        if (InventoryManager::GetSingleton()->GetScrolledDown())
        {
            if (renderListCoor < listBox->GetHeight() - listYSize * 4 - moveSpeed)
                renderListCoor += moveSpeed;
            InventoryManager::GetSingleton()->SetScrolledDown(false);
        }
    }

    startListIndex = renderListCoor / listYSize;

    // �÷��̾� �̵� ����
    if (listOpen == true)
        player->SetCanMove(false);

    // ����Ʈ ���� ���
    firstBoxHeight = listYSize - (abs)(renderListCoor) % listYSize;
    lastBoxHeight = listYSize - firstBoxHeight;

    // ���콺 ��ġ�� ����Ʈ �ε���
    if (listOpen)
    {
        // ����Ʈ ����
        if (g_ptMouse.x > 300 && g_ptMouse.x < 300 + listBox->GetWidth() - listExit->GetWidth() && g_ptMouse.y > 118 && g_ptMouse.y < 118 + listBox->GetHeight())
        {
            // �ε��� ���
            if (firstBoxHeight != listYSize)
            {
                if (g_ptMouse.y < firstBoxHeight + 118)
                    listIndex = startListIndex;
                else
                    listIndex = startListIndex + (g_ptMouse.y - (firstBoxHeight + 118)) / listYSize + 1;
            }
            else
                listIndex = (g_ptMouse.y - 118) / listYSize;

            // ��ǰ ���� ����
            storeData->SetListIndex(listIndex);

            // ���� �ֱ�
            if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON)
                && (g_ptMouse.x >= 300 && g_ptMouse.x <= 300 + storeList->GetWidth() && g_ptMouse.y >= 118 && g_ptMouse.y <= 118 + storeList->GetHeight()))
            {
                int price = storeData->GetMapCropData()->find(listIndex)->second->price;
                // ���ִ�
                if (price <= InventoryManager::GetSingleton()->GetPlayerMoney())
                {
                    bool alreadyExist = false;
                    
                    // vInven�� ���� ���� Ȯ��
                    string cropName = storeData->GetMapCropData()->find(listIndex)->second->cropName;
                    vector<INVEN_INFO*>* vInven = InventoryManager::GetSingleton()->GetVectorInven();
                    int existIndex;
                    for (int i = 0; i < vInven->size(); i++)
                    {
                        if ((*vInven)[i] != NULL)
                        {
                            test = (*vInven)[i]->productName;
                            if (((*vInven)[i]->productName).compare(cropName) == 0)
                            {
                                existIndex = i;
                                 alreadyExist = true;
                                 break;
                            }
                        }
                    }

                    // vInven�� ����
                    if (alreadyExist == false)
                    {
                        // ���λ��� + ���� ����
                        InventoryManager::GetSingleton()->SetAddInven(new INVEN_INFO(ObjectType::CROPS, 0, listIndex, price, 0, cropName, 1));
                        InventoryManager::GetSingleton()->SetPlayerMoneyDecrease(price);
                    }
                    // vInven�� �ִ�
                    else
                    {
                        // ���� �κ��丮 ���Ϳ� �ִ� �ε��� ��ġ�� �ִ� ��� �� �ÿ��ֱ� + ���� ����
                        (*vInven)[existIndex]->amount += 1;
                        InventoryManager::GetSingleton()->SetPlayerMoneyDecrease(price);
                    }
                }
                // �� ����
                else
                {  
                    noMoney = true;
                }
            }
        }

        // �ݱ�
        if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON)
            && (g_ptMouse.x >= 1065 && g_ptMouse.x <= 1085 && g_ptMouse.y >= 105 && g_ptMouse.y <= 125))
        {
			renderListCoor = 0;
			listIndex = 0;
			listOpen = false;
            InventoryManager::GetSingleton()->SetStoreListOpen(false);
		}
    }

}

void StoreScene::Render(HDC hdc)
{
    // ���� ���
    for (int i = startFrame.y; i <= startFrame.y + WINSIZE_Y / TILESIZE + 1; i++)        // ����
    {
        for (int j = startFrame.x; j <= startFrame.x + WINSIZE_X / TILESIZE + 1; j++)    // ����
        {
            int tempIndex = i * STORE_TILE_X + j;
            int tempPosX = -((int)renderCoor.x % TILESIZE) + TILESIZE * (j - startFrame.x);
            int tempPosY = -((int)renderCoor.y % TILESIZE) + TILESIZE * (i - startFrame.y);

            // ���� ���
            storeSample->FrameRender(hdc,
                tempPosX,
                tempPosY,
                storeTileInfo[tempIndex].frameX,
                storeTileInfo[tempIndex].frameY);

            // ��
            if (storeTileInfo[tempIndex].tileType == TileType::WALL)
            {
                //Rectangle(hdc, tempPosX, tempPosY,
                    //tempPosX + TILESIZE, tempPosY + TILESIZE);
            }
        }
    }

    // �÷��̾�
    player->Render(hdc);

    // ���� ����Ʈ
    if (storeList && listOpen)
    {
        // ���� ����Ʈ Ʋ
        storeList->Render(hdc, 290, 100);

        // ����Ʈ
        for (int i = startListIndex; i < startListIndex + 5; i++)  //����
        {
            // �������� ȭ�鿡 �׷��� y ��ġ (118 + ��)
            int tempPosY;
            if(i == startListIndex)
                tempPosY = 118;
            else
                tempPosY = 118 + firstBoxHeight + listYSize * (i - startListIndex - 1);

            // �� �����ӿ��� ��µ� ���� (������ listYSize, �� ù��° �������̶� �� ������ �����Ӹ� �ٸ�)
            int tempHeight;
            if (i == startListIndex)
                tempHeight = firstBoxHeight;
            else if(i == startListIndex + 5 - 1)
                tempHeight = lastBoxHeight;
            else
                tempHeight = listYSize;

            // �����ӿ��� ����� ���۵� ��ġ (������ 0, �� ù��° �����Ӹ� �ٸ�)
            int tempCopyY;
            if (i == startListIndex)
                tempCopyY = lastBoxHeight;
            else
                tempCopyY = 0;

            // ���� �׸���
            listBox->FrameListRender(hdc,
                300, 
                tempPosY, 
                tempHeight, 
                tempCopyY,
                0, 
                i);

            // ���콺 ��ġ ��ǰ ǥ��
            hpen = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
            hpenOld = (HPEN)::SelectObject(hdc, (HGDIOBJ)hpen);

            if (i == listIndex)
            {
                MoveToEx(hdc, 300, tempPosY, NULL);
                LineTo(hdc, 300 + listBox->GetFrameWidth(), tempPosY);
                LineTo(hdc, 300 + listBox->GetFrameWidth(), tempPosY + tempHeight);
                LineTo(hdc, 300, tempPosY + tempHeight);
                LineTo(hdc, 300, tempPosY);
            }

            hpen = (HPEN)::SelectObject(hdc, hpenOld);
            DeleteObject(hpen);
        }

        // ��ǰ ����
        if (g_ptMouse.x >= 300 && g_ptMouse.x <= 300 + storeList->GetWidth() && g_ptMouse.y >= 118 && g_ptMouse.y <= 118 + storeList->GetHeight())
        {
            storeData->Render(hdc);
        }

        // �ݱ�
        listExit->Render(hdc, 1055, 100);

        // �� ����
        if (noMoney)
        {
            moneyTimeCnt += TimerManager::GetSingleton()->GetElapsedTime();
            moneyWarn->Render(hdc, WINSIZE_X / 2 - moneyWarn->GetWidth() / 2, WINSIZE_Y / 2 - moneyWarn->GetHeight() / 2);
            if (moneyTimeCnt > 0.7f)
            {
                noMoney = false;
                moneyTimeCnt = 0.0f;
            }
        }
    }

    // �κ��丮
    InventoryManager::GetSingleton()->Render(hdc);
}

void StoreScene::loadStoreScene(int sceneNum)
{
    //�� �̹��� load
    string fileName = "Save/saveMapData";
    fileName += to_string(sceneNum) + ".map";
    HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD readBytes;
    if (ReadFile(hFile, storeTileInfo, sizeof(TILE_INFO) * STORE_TILE_X * STORE_TILE_Y, &readBytes, NULL)) {}
    else
        MessageBox(g_hWnd, "�������� �ε� ����", "����", MB_OK);
    CloseHandle(hFile);
}
