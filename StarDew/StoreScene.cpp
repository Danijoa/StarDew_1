#include "StoreScene.h"
#include "Image.h"
#include "DataManager.h"
#include "Player.h"
#include "InventoryManager.h"
#include "StoreData.h"

TILE_INFO StoreScene::storeTileInfo[STORE_TILE_X * STORE_TILE_Y];

HRESULT StoreScene::Init()
{
    // 상점 이미지 샘플
    ImageManager::GetSingleton()->AddImage("상점이미지", "Image/Store.bmp", 1408, 896, STORE_TILE_X, STORE_TILE_Y);
    storeSample = ImageManager::GetSingleton()->FindImage("상점이미지");
    loadStoreScene(3);

    // 플레이어
    player = new Player();
    player->Init();
    if (DataManager::GetSingleton()->GetPreScene() == 0)    // 테스트
        player->SetPlayerImagePos({ 430, 600 });
    if (DataManager::GetSingleton()->GetPreScene() == 2)    // 농장에서 전환된 장면
        player->SetPlayerImagePos({ 430, 600 });
    player->SetCurrImageSize({ storeSample->GetWidth(), storeSample->GetHeight() });
     
    // 상점 렌더 (화면상에 보여지는 이미지 시작 위치)
    renderCoor.x = player->GetPlayerImagePos().x - WINSIZE_X / 2;
    renderCoor.y = player->GetPlayerImagePos().y - WINSIZE_Y / 2;

    // 상점 판매 
    ImageManager::GetSingleton()->AddImage("상점리스트", "Image/StoreList.bmp", 800, 343, true, RGB(255, 255, 255));
    storeList = ImageManager::GetSingleton()->FindImage("상점리스트");
    ImageManager::GetSingleton()->AddImage("리스트닫기", "Image/listExit.bmp", 32, 32, true, RGB(255, 255, 255));
    listExit = ImageManager::GetSingleton()->FindImage("리스트닫기");

    // 상점 판매 곡물 
    ImageManager::GetSingleton()->AddImage("리스트상자", "Image/CropList(2).bmp", 768, 702 , 1, 9);
    listBox = ImageManager::GetSingleton()->FindImage("리스트상자");

    // 돈 없어
    ImageManager::GetSingleton()->AddImage("돈없어", "Image/moneyWarn.bmp", 480, 270, true, RGB(255, 255, 255));
    moneyWarn = ImageManager::GetSingleton()->FindImage("돈없어");
    noMoney = false;
    moneyTimeCnt = 0.0f;

    // 곡물 정보
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
    if (KeyManager::GetSingleton()->IsStayKeyDown('W'))			// 상
        storeDir = 0;
    else if (KeyManager::GetSingleton()->IsStayKeyDown('S'))	// 하
        storeDir = 1;
    else if (KeyManager::GetSingleton()->IsStayKeyDown('A'))	// 좌
        storeDir = 2;
    else if (KeyManager::GetSingleton()->IsStayKeyDown('D'))	// 우
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
        // 상점->마당 씬 변경
        if (storeTileInfo[left + bottom * STORE_TILE_X].tileType == TileType::STOREDOOR ||
            storeTileInfo[right + bottom * STORE_TILE_X].tileType == TileType::STOREDOOR)
        {
            DataManager::GetSingleton()->SetPreScene(3); // 현재 씬(상점=3) 저장
            SceneManager::GetSingleton()->ChangeScene("농장씬");
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

    // 화면 이동 + 플레이어 충돌 체크
    canMove = true;
    Move();

    // 렌더 시작 위치
    renderCoor.x = player->GetPlayerImagePos().x - WINSIZE_X / 2;
    renderCoor.y = player->GetPlayerImagePos().y - WINSIZE_Y / 2;

    // 화면 이동 제어
    MoveControl();

    // 렌더 시작 인덱스
    startFrame.x = renderCoor.x / TILESIZE;
    startFrame.y = renderCoor.y / TILESIZE;

    // 상점 리스트 틀 띄우기 
    if (!listOpen && KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
    {
        mouseIndex.x = (int)(g_ptMouse.x + renderCoor.x) / TILESIZE;
        mouseIndex.y = (int)(g_ptMouse.y + renderCoor.y) / TILESIZE;

        if (storeTileInfo[mouseIndex.x + STORE_TILE_X * mouseIndex.y].tileType == TileType::COUNTER)
        {
            listOpen = true;
            InventoryManager::GetSingleton()->SetStoreListOpen(listOpen);
        }

        // 닫기
        if (g_ptMouse.x >= 1065 && g_ptMouse.x <= 1085 && g_ptMouse.y >= 105 && g_ptMouse.y <= 125)
        {
            renderListCoor = 0;
            listIndex = 0;
            listOpen = false;
            InventoryManager::GetSingleton()->SetStoreListOpen(listOpen);
        }
    }

    // 리스트 내용 스크롤
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

    // 플레이어 이동 제어
    if (listOpen == true)
        player->SetCanMove(false);

    // 리스트 높이 계산
    firstBoxHeight = listYSize - (abs)(renderListCoor) % listYSize;
    lastBoxHeight = listYSize - firstBoxHeight;

    // 마우스 위치의 리스트 인덱스
    if (listOpen)
    {
        // 리스트 선택
        if (g_ptMouse.x > 300 && g_ptMouse.x < 300 + listBox->GetWidth() - listExit->GetWidth() && g_ptMouse.y > 118 && g_ptMouse.y < 118 + listBox->GetHeight())
        {
            // 인덱스 계산
            if (firstBoxHeight != listYSize)
            {
                if (g_ptMouse.y < firstBoxHeight + 118)
                    listIndex = startListIndex;
                else
                    listIndex = startListIndex + (g_ptMouse.y - (firstBoxHeight + 118)) / listYSize + 1;
            }
            else
                listIndex = (g_ptMouse.y - 118) / listYSize;

            // 상품 정보 띄우기
            storeData->SetListIndex(listIndex);

            // 선택 넣기
            if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON)
                && (g_ptMouse.x >= 300 && g_ptMouse.x <= 300 + storeList->GetWidth() && g_ptMouse.y >= 118 && g_ptMouse.y <= 118 + storeList->GetHeight()))
            {
                int price = storeData->GetMapCropData()->find(listIndex)->second->price;
                // 돈있다
                if (price <= InventoryManager::GetSingleton()->GetPlayerMoney())
                {
                    bool alreadyExist = false;
                    
                    // vInven에 존재 여부 확인
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

                    // vInven에 없다
                    if (alreadyExist == false)
                    {
                        // 새로생성 + 가격 차감
                        InventoryManager::GetSingleton()->SetAddInven(new INVEN_INFO(ObjectType::CROPS, 0, listIndex, price, 0, cropName, 1));
                        InventoryManager::GetSingleton()->SetPlayerMoneyDecrease(price);
                    }
                    // vInven에 있다
                    else
                    {
                        // 기존 인벤토리 벡터에 있던 인덱스 위치에 있는 곡식 양 늘여주기 + 가격 차감
                        (*vInven)[existIndex]->amount += 1;
                        InventoryManager::GetSingleton()->SetPlayerMoneyDecrease(price);
                    }
                }
                // 돈 없다
                else
                {  
                    noMoney = true;
                }
            }
        }

        // 닫기
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
    // 상점 배경
    for (int i = startFrame.y; i <= startFrame.y + WINSIZE_Y / TILESIZE + 1; i++)        // 세로
    {
        for (int j = startFrame.x; j <= startFrame.x + WINSIZE_X / TILESIZE + 1; j++)    // 가로
        {
            int tempIndex = i * STORE_TILE_X + j;
            int tempPosX = -((int)renderCoor.x % TILESIZE) + TILESIZE * (j - startFrame.x);
            int tempPosY = -((int)renderCoor.y % TILESIZE) + TILESIZE * (i - startFrame.y);

            // 상점 배경
            storeSample->FrameRender(hdc,
                tempPosX,
                tempPosY,
                storeTileInfo[tempIndex].frameX,
                storeTileInfo[tempIndex].frameY);

            // 벽
            if (storeTileInfo[tempIndex].tileType == TileType::WALL)
            {
                //Rectangle(hdc, tempPosX, tempPosY,
                    //tempPosX + TILESIZE, tempPosY + TILESIZE);
            }
        }
    }

    // 플레이어
    player->Render(hdc);

    // 상점 리스트
    if (storeList && listOpen)
    {
        // 상점 리스트 틀
        storeList->Render(hdc, 290, 100);

        // 리스트
        for (int i = startListIndex; i < startListIndex + 5; i++)  //세로
        {
            // 프레임이 화면에 그려질 y 위치 (118 + α)
            int tempPosY;
            if(i == startListIndex)
                tempPosY = 118;
            else
                tempPosY = 118 + firstBoxHeight + listYSize * (i - startListIndex - 1);

            // 한 프레임에서 출력될 높이 (보통은 listYSize, 맨 첫번째 프레임이랑 맨 마지막 프레임만 다름)
            int tempHeight;
            if (i == startListIndex)
                tempHeight = firstBoxHeight;
            else if(i == startListIndex + 5 - 1)
                tempHeight = lastBoxHeight;
            else
                tempHeight = listYSize;

            // 프레임에서 출력이 시작될 위치 (보통은 0, 맨 첫번째 프레임만 다른)
            int tempCopyY;
            if (i == startListIndex)
                tempCopyY = lastBoxHeight;
            else
                tempCopyY = 0;

            // 이제 그리자
            listBox->FrameListRender(hdc,
                300, 
                tempPosY, 
                tempHeight, 
                tempCopyY,
                0, 
                i);

            // 마우스 위치 상품 표시
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

        // 상품 정보
        if (g_ptMouse.x >= 300 && g_ptMouse.x <= 300 + storeList->GetWidth() && g_ptMouse.y >= 118 && g_ptMouse.y <= 118 + storeList->GetHeight())
        {
            storeData->Render(hdc);
        }

        // 닫기
        listExit->Render(hdc, 1055, 100);

        // 돈 없어
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

    // 인벤토리
    InventoryManager::GetSingleton()->Render(hdc);
}

void StoreScene::loadStoreScene(int sceneNum)
{
    //맵 이미지 load
    string fileName = "Save/saveMapData";
    fileName += to_string(sceneNum) + ".map";
    HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD readBytes;
    if (ReadFile(hFile, storeTileInfo, sizeof(TILE_INFO) * STORE_TILE_X * STORE_TILE_Y, &readBytes, NULL)) {}
    else
        MessageBox(g_hWnd, "저장파일 로드 실패", "실패", MB_OK);
    CloseHandle(hFile);
}
