#include "InventoryManager.h"
#include "Image.h"
#include "SortManager.h"
#include "DataManager.h"
#include <math.h>

SortManager InventoryManager::sortMgr;

HRESULT InventoryManager::Init()
{
    // 시계
    ImageManager::GetSingleton()->AddImage("시계", "Image/Clock.bmp", 71 * 3, 59 * 3, true, RGB(255, 0, 255));
    clock = ImageManager::GetSingleton()->FindImage("시계");
    dayCnt = 1;
    //timeCnt = TimerManager::GetSingleton()->GetGameSecond();
    money = 1000;
    ImageManager::GetSingleton()->AddImage("시계화살표", "Image/timeArrow.bmp", 23, 47, true, RGB(255, 255, 255));
    timeArrow = ImageManager::GetSingleton()->FindImage("시계화살표");
    angle = 0.0f;

    // 체력
    ImageManager::GetSingleton()->AddImage("에너지바", "Image/energyBar.bmp", 45, 268, true, RGB(255, 255, 255));
    energyBar = ImageManager::GetSingleton()->FindImage("에너지바");
    ImageManager::GetSingleton()->AddImage("에너지", "Image/energy.bmp", 23, 213, true, RGB(255, 255, 255));
    energy = ImageManager::GetSingleton()->FindImage("에너지");
    playerEnergy = energy->GetHeight();

    // 작은 인벤토리 이미지
    ImageManager::GetSingleton()->AddImage("작은인벤", "Image/SmallInven.bmp", 800, 96, true, RGB(0, 0, 0));
    smallInven = ImageManager::GetSingleton()->FindImage("작은인벤");
    smallInvenOpened = true;
    smallRect = { WINSIZE_X / 2 - smallInven->GetWidth() / 2, WINSIZE_Y - 80 ,  WINSIZE_X / 2 + smallInven->GetWidth() / 2 - 10 , WINSIZE_Y };
    selectedIndex = 0;
    selectedObjectType = ObjectType::NONE;
    downIndex.x = 0;
    downIndex.y = 0;
    storeOpened = false;

    // 큰 인벤토리 이미지
    ImageManager::GetSingleton()->AddImage("큰인벤", "Image/BigInven.bmp", 912, 256, true, RGB(255, 255, 255));
    bigInven = ImageManager::GetSingleton()->FindImage("큰인벤");
    bigInvenOpened = false;
    //bigRect = { WINSIZE_X / 2 - smallInven->GetWidth() / 2 - 60, 200, WINSIZE_X / 2 - smallInven->GetWidth() / 2 - 60 +  bigInven->GetWidth(), 200 + bigInven->GetHeight() };
    bigRect = { 295, 220, 1080, 420 };

    // 작물들
    ImageManager::GetSingleton()->AddImage("곡물들", "Image/Crops.bmp", 32 * 3, 162 * 3, 2, 9, true, RGB(255, 255, 255));
    crops = ImageManager::GetSingleton()->FindImage("곡물들");

    // 농기구
    ImageManager::GetSingleton()->AddImage("농기구", "Image/tool(2).bmp", 16 * 3, 36 * 3, 1, 2, true, RGB(255, 255, 255));
    tools = ImageManager::GetSingleton()->FindImage("농기구");

    // 폰트
    ImageManager::GetSingleton()->AddImage("숫자", "Image/font_num.bmp", 80, 11, 10, 1, true, RGB(255, 0, 255));
    numFont = ImageManager::GetSingleton()->FindImage("숫자");
    ImageManager::GetSingleton()->AddImage("데이", "Image/dayFont.bmp", 25, 12, true, RGB(255, 255, 255));
    dayFont = ImageManager::GetSingleton()->FindImage("데이");
    ImageManager::GetSingleton()->AddImage("날짜도트", "Image/timeDot.bmp", 7, 12, true, RGB(255, 255, 255));
    timeDot = ImageManager::GetSingleton()->FindImage("날짜도트");

    // 인벤토리 내용 저장   
    for (int i = 0; i < 36; i++)
        vInven.push_back(NULL);
    vInven[0] = (new INVEN_INFO(ObjectType::TOOLS, 0, 0, 0, "Can", 1));
    vInven[1] = (new INVEN_INFO(ObjectType::TOOLS, 0, 1, 1, "Axe", 1));
    vInven[2] = (new INVEN_INFO(ObjectType::CROPS, 0, 2, 100, "Rhubarb", 1));
    vInven[3] = (new INVEN_INFO(ObjectType::CROPS, 0, 3, 80, "Blueberry", 1));
    vInven[4] = (new INVEN_INFO(ObjectType::CROPS, 0, 4, 240, "Cranberries", 1));
    vInven[5] = (new INVEN_INFO(ObjectType::CROPS, 0, 5, 200, "FairyRose", 1));
    vIndex = 6;

    //
    size = 64;
    tabCnt = 0;
    timeCheck = 0.0f;
    checkDayPass = false;
    sleepDay = false;
    timeoutDay = false;
    energyoutDay = false;
    tempTime = 0.0f;
    timeControl = 0;
    timeHour = 9;
    timeMin = 0;

    return S_OK;
}

void InventoryManager::Release()
{
}

void InventoryManager::Update()
{
    // 시간 흐름
    tempTime += TimerManager::GetSingleton()->GetElapsedTime();
    if (tempTime >= 1.0f) 
    {
        angle += 0.6;
        tempTime = 0.0f;
        timeMin += 3;
    }

    if (timeMin > 60)
    {
        timeHour += 1;
        timeMin = 0;
    }

    if (KeyManager::GetSingleton()->IsStayKeyDown('O'))
    {
        angle += 0.6;
        timeMin += 3;
    }

    if (timeHour >= 24) //다음날
    {
        DataManager::GetSingleton()->SetPreScene(4);                    // 타이틀 씬에서 새로 로드 하는 것 처럼 저장
        InventoryManager::GetSingleton()->SetTimeoutDay(true);            // 에너지 아웃으로 잠들었다
        SceneManager::GetSingleton()->ChangeScene("하우스씬", "로딩씬");
    }

    // 날짜 흐름
    if (KeyManager::GetSingleton()->IsOnceKeyDown('P'))
    {
        checkDayPass = true;
        SetDay();
        SetReEnergy();
        SetReTime();
    }

    /* 일단 강제로 막아두기*/ 
    if (dayCnt >= 999)
        dayCnt = 999;

    // 체력
    if (playerEnergy <= 0)
    {
        playerEnergy = 0;

        DataManager::GetSingleton()->SetPreScene(4);                    // 타이틀 씬에서 새로 로드 하는 것 처럼 저장
        InventoryManager::GetSingleton()->SetEnergyoutDay(true);            // 에너지 아웃으로 잠들었다
        SceneManager::GetSingleton()->ChangeScene("하우스씬", "로딩씬");
    }

    // 비어있는 인벤토리 위치 찾기
    for (int i = 0; i < 36; i++)
    {
        if (vInven[i] == NULL)
        {
            vIndex = i;
            break;
        }
    }

    // 작은 인벤토리 인덱스
    if (smallInvenOpened && !storeOpened)
    {
        sInvenX = (g_ptMouse.x - 290) / size;

        // 리스트 내용 스크롤
        if (InventoryManager::GetSingleton()->GetScrolledUp())
        {
            if(downIndex.x >= 0 + 1)
                downIndex.x -= 1;
            if (vInven[downIndex.x] != NULL)
            {
                selectedItem = vInven[downIndex.x]->productName;
                selectedIndex = downIndex.x;
                selectedObjectType = vInven[downIndex.x]->objType;
            }
            InventoryManager::GetSingleton()->SetScrolledUp(false);
        }

        if (InventoryManager::GetSingleton()->GetScrolledDown())
        {
            if (downIndex.x < 12 - 1)
                downIndex.x += 1;
            if (vInven[downIndex.x] != NULL)
            {
                selectedItem = vInven[downIndex.x]->productName;
                selectedIndex = downIndex.x;
                selectedObjectType = vInven[downIndex.x]->objType;
            }
            InventoryManager::GetSingleton()->SetScrolledDown(false);
        }
    }

    // 작은 인벤토리 위치 변경
    if (smallInvenOpened
        && (g_ptMouse.x >= smallRect.left && g_ptMouse.x <= smallRect.right && g_ptMouse.y >= smallRect.top && g_ptMouse.y <= smallRect.bottom))
    {
        if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
        {
            downIndex.x = sInvenX;
            if (vInven[downIndex.x] != NULL)
            {
                selectedItem = vInven[downIndex.x]->productName;
                selectedIndex = downIndex.x;
                selectedObjectType = vInven[downIndex.x]->objType;
            }
        }

        if (KeyManager::GetSingleton()->IsOnceKeyUp(VK_LBUTTON))
        {
            upIndex.x = sInvenX;

            // 바꾸기
            if (vInven[downIndex.x] != NULL)
            {
                INVEN_INFO* temp;
                temp = vInven[downIndex.x];
                vInven[downIndex.x] = vInven[upIndex.x];
                vInven[upIndex.x] = temp;
            }
        }
    }

    // 큰 인벤토리 인덱스
    if (bigInvenOpened)
    {
        bInven.x = (g_ptMouse.x - 320) / size;
        bInven.y = (g_ptMouse.y - 230) / size;

        // 정렬
        //if (KeyManager::GetSingleton()->IsOnceKeyDown('A'))
        //{
        //    // 선택: ?
        //    sortMgr.ChangeSort(new SelectionSort());
        //    sortMgr.DoSort(&vInven);
        //}

        if (KeyManager::GetSingleton()->IsOnceKeyDown('B'))
        {
            // 퀵: 가격 순 (orderNum)
            sortMgr.ChangeSort(new QuickSort());
            sortMgr.DoSort(&vInven);
        }

        if (KeyManager::GetSingleton()->IsOnceKeyDown('C'))
        {
            // 삽입: 이름 앞글자 순 (productName)
            sortMgr.ChangeSort(new InsertionSort());
            sortMgr.DoSort(&vInven);
        }
    }

    // 큰 인벤토리 위치 변경
    if (bigInvenOpened
        && (g_ptMouse.x >= bigRect.left && g_ptMouse.x <= bigRect.right && g_ptMouse.y >= bigRect.top && g_ptMouse.y <= bigRect.bottom))
    {
        if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
        {
            downIndex.x = bInven.x;
            downIndex.y = bInven.y;
        }

        if (KeyManager::GetSingleton()->IsOnceKeyUp(VK_LBUTTON))
        {
            upIndex.x = bInven.x;
            upIndex.y = bInven.y;

            // 바꾸기
            if (vInven[downIndex.x + 12 * downIndex.y] != NULL)
            {
                INVEN_INFO* temp;
                temp = vInven[downIndex.x + 12 * downIndex.y];
                vInven[downIndex.x + 12 * downIndex.y] = vInven[upIndex.x + 12 * upIndex.y];
                vInven[upIndex.x + 12 * upIndex.y] = temp;
            }
        }
    }

    if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_TAB) && !storeOpened)
    {
        tabCnt += 1;
        if (tabCnt % 2 == 1)
        {
            smallInvenOpened = false;
            bigInvenOpened = true;
        }
        else
        {
            smallInvenOpened = true;
            bigInvenOpened = false;
            tabCnt = 0;
        }
    }
}

void InventoryManager::Render(HDC hdc)
{
    // 시계
    if (clock)
        clock->Render(hdc, WINSIZE_X - clock->GetWidth(), 0);

    // 초시계 화살표
    timeArrow->RotateRender(hdc, 600, 200,  25, angle);

    // day 날짜
    int dayHun = dayCnt / 100;
    int dayTen = (dayCnt % 100) / 10;
    int dayOne = dayCnt % 10;
    if (dayFont)
        dayFont->Render(hdc, WINSIZE_X - 70 - 30, 22);
	// 100의 자리
	if (dayHun > 0)
		numFont->FrameRender(hdc, WINSIZE_X - 70, 22, dayHun, 0);
	// 10의 자리
	if (dayTen > 0 || dayHun > 0)
		numFont->FrameRender(hdc, WINSIZE_X - 70 + 10, 22, dayTen, 0);
	// 1의 자리
	numFont->FrameRender(hdc, WINSIZE_X - 70 + 20, 22, dayOne, 0);

    // time 시간
    numFont->FrameRender(hdc, WINSIZE_X - 100 - 20, 85, timeHour / 10, 0);
    numFont->FrameRender(hdc, WINSIZE_X - 100 - 10, 85, timeHour % 10, 0);
    timeDot->Render(hdc, WINSIZE_X - 100, 85);
    numFont->FrameRender(hdc, WINSIZE_X - 100 + 10, 85, timeMin / 10, 0);
    numFont->FrameRender(hdc, WINSIZE_X - 100 + 20, 85, timeMin % 10, 0);

    // money 돈
    if (money <= 0)
    {
        numFont->FrameRender(hdc, WINSIZE_X - 33, 150, 0, 0);
    }
    else
    {
        int moneySize = 0;
        int tempMoney = money;
        while (tempMoney > 0)
        {
            tempMoney = tempMoney / 10;
            moneySize++;
        }

        for (int i = 1; i < moneySize + 1; i++)
        {
            int a = pow(10, i);
            int b = pow(10, (i - 1));
            int curX = (money % a) / b;
            numFont->FrameRender(hdc, WINSIZE_X - 33 - (18 * (i - 1)), 150, curX, 0);
        }
    }

    // 체력
    if (energyBar)
        energyBar->Render(hdc, WINSIZE_X - energyBar->GetWidth()- 10, WINSIZE_Y - energyBar->GetHeight() - 10);
    if (energy)
        energy->BarRender(hdc, WINSIZE_X - energy->GetWidth() - 10 - 11, WINSIZE_Y - energy->GetHeight() - 10 - 6, playerEnergy);

    // 작은 인벤 창 출력 (12개)
    if (smallInvenOpened)
    {
        smallInven->Render(hdc, WINSIZE_X / 2 - smallInven->GetWidth() / 2, WINSIZE_Y - smallInven->GetHeight());

        // 작은 인벤 내용 출력
        for (int i = 0; i < 12; i++)
        {
            int objPosX = 300 + 65 * i;
            int objPosY = 650;

            // 그림
            if (vInven[i] == NULL)
                continue;
            else if (vInven[i]->objType == ObjectType::CROPS || vInven[i]->objType == ObjectType::CROPGROWN)
            {
                crops->FrameRender(hdc, objPosX, objPosY, vInven[i]->frameX, vInven[i]->frameY);
                // 숫자
                numFont->FrameRender(hdc, objPosX + 45, objPosY + 45, (vInven[i]->amount % 10), 0);
                if(vInven[i]->amount >= 10)
                    numFont->FrameRender(hdc, objPosX + 45 - 10, objPosY + 45, (vInven[i]->amount / 10), 0);
            }
            else if (vInven[i]->objType == ObjectType::TOOLS)
            {
                tools->FrameRender(hdc, objPosX, objPosY, vInven[i]->frameX, vInven[i]->frameY);
            }
        }
    }

    // 선택한 값
    if (!bigInvenOpened)
    {
        hpen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
        hpenOld = (HPEN)::SelectObject(hdc, (HGDIOBJ)hpen);

        MoveToEx(hdc, 290 + size * downIndex.x, 645, NULL);
        LineTo(hdc, 290 + size * (downIndex.x + 1), 645);
        LineTo(hdc, 290 + size * (downIndex.x + 1), 645 + size);
        LineTo(hdc, 290 + size * downIndex.x, 645 + size);
        LineTo(hdc, 290 + size * downIndex.x, 645);

        hpen = (HPEN)::SelectObject(hdc, hpenOld);
        DeleteObject(hpen);
    }

    // 큰 인벤 창 출력 (12 * 3개)
    if (bigInvenOpened)
    {
        bigInven->Render(hdc, WINSIZE_X / 2 - smallInven->GetWidth() / 2 - 60, 200);

        hpen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
        hpenOld = (HPEN)::SelectObject(hdc, (HGDIOBJ)hpen);

        if (g_ptMouse.y > bigRect.top && g_ptMouse.y < bigRect.bottom
            && g_ptMouse.x < bigRect.right && g_ptMouse.x > bigRect.left)
        {
            MoveToEx(hdc, 320 + size * bInven.x, 230 + size * bInven.y, NULL);
            LineTo(hdc, 320 + size * (bInven.x + 1), 230 + size * bInven.y);
            LineTo(hdc, 320 + size * (bInven.x + 1), 230 + size * (bInven.y + 1));
            LineTo(hdc, 320 + size * bInven.x, 230 + size * (bInven.y + 1));
            LineTo(hdc, 320 + size * bInven.x, 230 + size * bInven.y);
        }

        hpen = (HPEN)::SelectObject(hdc, hpenOld);
        DeleteObject(hpen);

        // 큰 인벤 내용 출력
        for (int i = 0; i < 36; i++)
        {
            int objPosX = 325 + 65 * (i % 12);
            int objPosY = 235 + 65 * (i / 12);

            // 그림
            if (vInven[i] == NULL)
                continue;
            else if (vInven[i]->objType == ObjectType::CROPS || vInven[i]->objType == ObjectType::CROPGROWN)
            {
                crops->FrameRender(hdc, objPosX, objPosY, vInven[i]->frameX, vInven[i]->frameY);
                // 숫자
                numFont->FrameRender(hdc, objPosX + 45, objPosY + 45, (vInven[i]->amount % 10), 0);
                if (vInven[i]->amount >= 10)
                    numFont->FrameRender(hdc, objPosX + 45 - 10, objPosY + 45, (vInven[i]->amount / 10), 0);
            }
            else if (vInven[i]->objType == ObjectType::TOOLS)
            {
                tools->FrameRender(hdc, objPosX, objPosY, vInven[i]->frameX, vInven[i]->frameY);
            }
        }
    }

    /*확인용*/
	if (vInven[downIndex.x] == NULL)
	{
		wsprintf(szText, "선택 아이템: x ");
		TextOut(hdc, 0, 30, szText, strlen(szText));
	}
	else
	{
		wsprintf(szText, "선택 아이템: %s ", vInven[downIndex.x]->productName.c_str());
		TextOut(hdc, 0, 30, szText, strlen(szText));
	}
}
