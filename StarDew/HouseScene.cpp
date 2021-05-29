#include "HouseScene.h"
#include "Image.h"
#include "DataManager.h"
#include "InventoryManager.h"
#include "Player.h"

TILE_INFO HouseScene::houseTileInfo[HOUSE_TILE_X * HOUSE_TILE_Y];

HRESULT HouseScene::Init()
{
    if (DataManager::GetSingleton()->GetPreScene() == 4)
    {
        Sleep(1500);    // 로딩 대기

        // 잠들었는 방법 갱신
        InventoryManager::GetSingleton()->SetSleepDay(false);   
        InventoryManager::GetSingleton()->SetTimeoutDay(false);
        InventoryManager::GetSingleton()->SetEnergyoutDay(false);

        InventoryManager::GetSingleton()->SetDay();             // day 갱신
        InventoryManager::GetSingleton()->SetDayCheck(true);    // day 갱신 알려주기
        InventoryManager::GetSingleton()->SetReEnergy();        // 체력 갱신
    }

    // 크기 조정
    //SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);

    // 집 샘플 이미지
    ImageManager::GetSingleton()->AddImage("집이미지", "Image/House.bmp", 1408, 768, HOUSE_TILE_X, HOUSE_TILE_Y);
    houseSample = ImageManager::GetSingleton()->FindImage("집이미지");

    loadHouseScene(1);

    // 플레이어
    player = new Player();
    player->Init();
    if (DataManager::GetSingleton()->GetPreScene() == 0 || DataManager::GetSingleton()->GetPreScene() == 4)    // 타이틀에서 전환된 장면 or 잠들고 일어난 경우
        player->SetPlayerImagePos({ 320, 310 });
    if (DataManager::GetSingleton()->GetPreScene() == 2)    // 마당에서 전환된 장면
        player->SetPlayerImagePos({ 423, 550 });
    player->SetCurrImageSize({ houseSample->GetWidth(), houseSample->GetHeight() });

    // 농장 렌더 (화면상에 보여지는 이미지 시작 위치)
    renderCoor.x = player->GetPlayerImagePos().x - WINSIZE_X / 2;
    renderCoor.y = player->GetPlayerImagePos().y - WINSIZE_Y / 2;

    // 잠 확인
    ImageManager::GetSingleton()->AddImage("잠박스", "Image/sleepBox.bmp", 1277, 361, true, RGB(255, 255, 255));
    sleepBox = ImageManager::GetSingleton()->FindImage("잠박스");
    checkSleep = false;
    yesSleep = false;
    noSleep = false;

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
    if (KeyManager::GetSingleton()->IsStayKeyDown('W'))			// 상
    {
        houseDir = 0;
        playerHouseCollision();
    }
    else if (KeyManager::GetSingleton()->IsStayKeyDown('S'))	// 하
    {
        houseDir = 1;
        playerHouseCollision();
    }
    else if (KeyManager::GetSingleton()->IsStayKeyDown('A'))	// 좌
    {
        houseDir = 2;
        playerHouseCollision();
    }
    else if (KeyManager::GetSingleton()->IsStayKeyDown('D'))	// 우
    {
        houseDir = 3;
        playerHouseCollision();
    }
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
        // 집->마당 씬 변경
        if (houseTileInfo[left + bottom * HOUSE_TILE_X].tileType == TileType::HOUSEDOOR ||
            houseTileInfo[right + bottom * HOUSE_TILE_X].tileType == TileType::HOUSEDOOR)
        {
            DataManager::GetSingleton()->SetPreScene(1); // 현재 씬(집=1) 저장
            SceneManager::GetSingleton()->ChangeScene("농장씬");
        }
        if (houseTileInfo[left + bottom * HOUSE_TILE_X].tileType == TileType::WALL) 	// left,bottom
            canMove = false;
        if (houseTileInfo[right + bottom * HOUSE_TILE_X].tileType == TileType::WALL)	// right,bottom
            canMove = false;
        break;
    case 2:
        // 잠들자
        if (player->GetPlayerRect().left > 320 && player->GetPlayerRect().left < 400
            && player->GetPlayerRect().top > 330 && player->GetPlayerRect().top < 400)
        {
            checkSleep = true;
        }
        if (houseTileInfo[left + top * HOUSE_TILE_X].tileType == TileType::WALL)	// left,top
            canMove = false;
        if (houseTileInfo[left + bottom * HOUSE_TILE_X].tileType == TileType::WALL)	// left,bottom
            canMove = false;
        break;
    case 3:
        if (houseTileInfo[right + top * HOUSE_TILE_X].tileType == TileType::WALL)	    // right,top
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

    // 잠 들 것인가
    if (checkSleep)
    {
        // 잘 거임
        if (g_ptMouse.x > 80 && g_ptMouse.x < 1265 && g_ptMouse.y > 320 && g_ptMouse.y < 405)
        {
            if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
            {
                yesSleep = true;
                noSleep = false;

                // 다음 날
                checkSleep = false;
		        DataManager::GetSingleton()->SetPreScene(4);                    // 타이틀 씬에서 새로 로드 하는 것 처럼 저장
                player->SetCanMove(true);
                InventoryManager::GetSingleton()->SetSleepDay(true);            // 정상적으로 잠들었다
                SceneManager::GetSingleton()->ChangeScene("하우스씬", "로딩씬");
                return;
            }
        }

        // 인 잘 거임
        if (g_ptMouse.x > 80 && g_ptMouse.x < 1265 && g_ptMouse.y > 405 && g_ptMouse.y < 490)
        {
            if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
            {
                yesSleep = false;
                noSleep = true;

                // 다시 활동
                checkSleep = false;
                player->SetCanMove(true);
            }
        }
    }
}

void HouseScene::Render(HDC hdc)
{
    // 뒤에 검은 바탕으로 칠하기
    PatBlt(hdc, 0, 0, WINSIZE_X, WINSIZE_Y, BLACKNESS);

    // 하우스 메인 타일 출력
    for (int i = startFrame.y; i <= startFrame.y + HOUSE_TILE_Y; i++)        // 세로
    {
        for (int j = startFrame.x; j <= startFrame.x + HOUSE_TILE_X; j++)    // 가로
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

    // 플레이어
    player->Render(hdc);

    if (checkSleep)
    {
        sleepBox->Render(hdc, WINSIZE_X / 2 - sleepBox->GetWidth() / 2, WINSIZE_Y / 2 - sleepBox->GetHeight() / 2);
        player->SetCanMove(false);

        hpen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
        hpenOld = (HPEN)::SelectObject(hdc, (HGDIOBJ)hpen);


        if (g_ptMouse.x > 80 && g_ptMouse.x < 1265 && g_ptMouse.y > 320 && g_ptMouse.y < 405)
        {
            MoveToEx(hdc, 80, 320, NULL);
            LineTo(hdc, 1265, 320);
            LineTo(hdc, 1265, 405);
            LineTo(hdc, 80, 405);
            LineTo(hdc, 80, 320);
        }

        if (g_ptMouse.x > 80 && g_ptMouse.x < 1265 && g_ptMouse.y>405 && g_ptMouse.y < 490)
        {
            MoveToEx(hdc, 80, 405, NULL);
            LineTo(hdc, 1265, 405);
            LineTo(hdc, 1265, 490);
            LineTo(hdc, 80, 490);
            LineTo(hdc, 80, 405);
        }

        hpen = (HPEN)::SelectObject(hdc, hpenOld);
        DeleteObject(hpen);
    }

    // 인벤토리
    InventoryManager::GetSingleton()->Render(hdc);
}

void HouseScene::loadHouseScene(int sceneNum)
{
    //맵 이미지 load
    string fileName = "Save/saveMapData";
    fileName += to_string(sceneNum) + ".map";
    HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD readBytes;
    if (ReadFile(hFile, houseTileInfo, sizeof(TILE_INFO) * HOUSE_TILE_X * HOUSE_TILE_Y, &readBytes, NULL)) {}
    else
        MessageBox(g_hWnd, "저장파일 로드 실패", "실패", MB_OK);
    CloseHandle(hFile);
}
