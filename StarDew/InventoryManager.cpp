#include "InventoryManager.h"
#include "Image.h"
#include "SortManager.h"

SortManager InventoryManager::sortMgr;

HRESULT InventoryManager::Init()
{
    // �ð�
    ImageManager::GetSingleton()->AddImage("�ð�", "Image/Clock.bmp", 71 * 3, 59 * 3, true, RGB(255, 0, 255));
    clock = ImageManager::GetSingleton()->FindImage("�ð�");
    dayCnt = 1;
    timeCnt = TimerManager::GetSingleton()->GetGameSecond();
    money = 1000;

    // ü��
    ImageManager::GetSingleton()->AddImage("��������", "Image/energyBar.bmp", 45, 268, true, RGB(255, 255, 255));
    energyBar = ImageManager::GetSingleton()->FindImage("��������");
    ImageManager::GetSingleton()->AddImage("������", "Image/energy.bmp", 23, 213, true, RGB(255, 255, 255));
    energy = ImageManager::GetSingleton()->FindImage("������");
    playerEnergy = energy->GetHeight();

    // ���� �κ��丮 �̹���
    ImageManager::GetSingleton()->AddImage("�����κ�", "Image/SmallInven.bmp", 800, 96, true, RGB(0, 0, 0));
    smallInven = ImageManager::GetSingleton()->FindImage("�����κ�");
    smallInvenOpened = true;
    smallRect = { WINSIZE_X / 2 - smallInven->GetWidth() / 2, WINSIZE_Y - 80 ,  WINSIZE_X / 2 + smallInven->GetWidth() / 2 - 10 , WINSIZE_Y };
    selectedIndex = 0;
    selectedObjectType = ObjectType::NONE;
    downIndex.x = 0;
    downIndex.y = 0;
    storeOpened = false;

    // ū �κ��丮 �̹���
    ImageManager::GetSingleton()->AddImage("ū�κ�", "Image/BigInven.bmp", 912, 256, true, RGB(255, 255, 255));
    bigInven = ImageManager::GetSingleton()->FindImage("ū�κ�");
    bigInvenOpened = false;
    //bigRect = { WINSIZE_X / 2 - smallInven->GetWidth() / 2 - 60, 200, WINSIZE_X / 2 - smallInven->GetWidth() / 2 - 60 +  bigInven->GetWidth(), 200 + bigInven->GetHeight() };
    bigRect = { 295, 220, 1080, 420 };

    // �۹���
    ImageManager::GetSingleton()->AddImage("���", "Image/Crops.bmp", 32 * 3, 162 * 3, 2, 9, true, RGB(255, 255, 255));
    crops = ImageManager::GetSingleton()->FindImage("���");

    // ��ⱸ
    ImageManager::GetSingleton()->AddImage("��ⱸ", "Image/tool(2).bmp", 16 * 3, 36 * 3, 1, 2, true, RGB(255, 255, 255));
    tools = ImageManager::GetSingleton()->FindImage("��ⱸ");

    // ����
    ImageManager::GetSingleton()->AddImage("����", "Image/font_num.bmp", 80, 11, 10, 1, true, RGB(255, 0, 255));
    cropNum = ImageManager::GetSingleton()->FindImage("����");

    // �κ��丮 ���� ����   
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

    return S_OK;
}

void InventoryManager::Release()
{
}

void InventoryManager::Update()
{
    // �ð� �帧
    timeCnt = TimerManager::GetSingleton()->GetGameSecond() -timeCheck;

    // ��¥ �帧
    if (KeyManager::GetSingleton()->IsOnceKeyDown('P'))
    {
        dayCnt += 1;
        //timeCnt = 0;
        timeCheck = TimerManager::GetSingleton()->GetGameSecond();
    }

    // ü��
    if (playerEnergy <= 0)
    {
        playerEnergy = 0;
    }

    // ����ִ� �κ��丮 ��ġ ã��
    for (int i = 0; i < 36; i++)
    {
        if (vInven[i] == NULL)
        {
            vIndex = i;
            break;
        }
    }

    // ���� �κ��丮 �ε���
    if (smallInvenOpened && !storeOpened)
    {
        sInvenX = (g_ptMouse.x - 290) / size;

        // ����Ʈ ���� ��ũ��
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

    // ���� �κ��丮 ��ġ ����
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

            // �ٲٱ�
            if (vInven[downIndex.x] != NULL)
            {
                INVEN_INFO* temp;
                temp = vInven[downIndex.x];
                vInven[downIndex.x] = vInven[upIndex.x];
                vInven[upIndex.x] = temp;
            }
        }
    }

    // ū �κ��丮 �ε���
    if (bigInvenOpened)
    {
        bInven.x = (g_ptMouse.x - 320) / size;
        bInven.y = (g_ptMouse.y - 230) / size;

        // ����
        //if (KeyManager::GetSingleton()->IsOnceKeyDown('A'))
        //{
        //    // ����: ?
        //    sortMgr.ChangeSort(new SelectionSort());
        //    sortMgr.DoSort(&vInven);
        //}

        if (KeyManager::GetSingleton()->IsOnceKeyDown('B'))
        {
            // ��: ���� �� (orderNum)
            sortMgr.ChangeSort(new QuickSort());
            sortMgr.DoSort(&vInven);
        }

        if (KeyManager::GetSingleton()->IsOnceKeyDown('C'))
        {
            // ����: �̸� �ձ��� �� (productName)
            sortMgr.ChangeSort(new InsertionSort());
            sortMgr.DoSort(&vInven);
        }
    }

    // ū �κ��丮 ��ġ ����
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

            // �ٲٱ�
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
    // �ð�
    if (clock)
    {
        clock->Render(hdc, WINSIZE_X - clock->GetWidth(), 0);
    }

    // ü��
    if (energyBar)
    {
        energyBar->Render(hdc, WINSIZE_X - energyBar->GetWidth()- 10, WINSIZE_Y - energyBar->GetHeight() - 10);
    }
    if (energy)
    {
        energy->BarRender(hdc, WINSIZE_X - energy->GetWidth() - 10 - 11, WINSIZE_Y - energy->GetHeight() - 10 - 6, playerEnergy);
    }

    // ���� �κ� â ��� (12��)
    if (smallInvenOpened)
    {
        smallInven->Render(hdc, WINSIZE_X / 2 - smallInven->GetWidth() / 2, WINSIZE_Y - smallInven->GetHeight());

       /* hpen = CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
        hpenOld = (HPEN)::SelectObject(hdc, (HGDIOBJ)hpen);

        if (g_ptMouse.y > smallRect.top && g_ptMouse.y < smallRect.bottom
            && g_ptMouse.x > smallRect.left && g_ptMouse.x < smallRect.right)
        {
            MoveToEx(hdc, 290 + size * sInvenX, 645, NULL);
            LineTo(hdc, 290 + size * (sInvenX + 1), 645);
            LineTo(hdc, 290 + size * (sInvenX + 1), 645 + size);
            LineTo(hdc, 290 + size * sInvenX, 645 + size);
            LineTo(hdc, 290 + size * sInvenX, 645);
        }

        hpen = (HPEN)::SelectObject(hdc, hpenOld);
        DeleteObject(hpen);*/

        // ���� �κ� ���� ���
        for (int i = 0; i < 12; i++)
        {
            int objPosX = 300 + 65 * i;
            int objPosY = 650;

            // �׸�
            if (vInven[i] == NULL)
                continue;
            else if (vInven[i]->objType == ObjectType::CROPS || vInven[i]->objType == ObjectType::CROPGROWN)
            {
                crops->FrameRender(hdc, objPosX, objPosY, vInven[i]->frameX, vInven[i]->frameY);
                // ����
                cropNum->FrameRender(hdc, objPosX + 45, objPosY + 45, vInven[i]->amount, 0);
            }
            else if (vInven[i]->objType == ObjectType::TOOLS)
            {
                tools->FrameRender(hdc, objPosX, objPosY, vInven[i]->frameX, vInven[i]->frameY);
            }
        }
    }

    // ������ ��
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

    // ū �κ� â ��� (12 * 3��)
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

        // ū �κ� ���� ���
        for (int i = 0; i < 36; i++)
        {
            int objPosX = 325 + 65 * (i % 12);
            int objPosY = 235 + 65 * (i / 12);

            // �׸�
            if (vInven[i] == NULL)
                continue;
            else if (vInven[i]->objType == ObjectType::CROPS || vInven[i]->objType == ObjectType::CROPGROWN)
            {
                crops->FrameRender(hdc, objPosX, objPosY, vInven[i]->frameX, vInven[i]->frameY);
                // ����
                cropNum->FrameRender(hdc, objPosX + 45, objPosY + 45, vInven[i]->amount, 0);
            }
            else if (vInven[i]->objType == ObjectType::TOOLS)
            {
                tools->FrameRender(hdc, objPosX, objPosY, vInven[i]->frameX, vInven[i]->frameY);
            }
        }
    }

    /*Ȯ�ο�*/
	if (vInven[downIndex.x] == NULL)
	{
		wsprintf(szText, "���� ������: x ");
		TextOut(hdc, 0, 0, szText, strlen(szText));
	}
	else
	{
		wsprintf(szText, "���� ������: %s ", vInven[downIndex.x]->productName.c_str());
		TextOut(hdc, 0, 0, szText, strlen(szText));
	}

    // 
    wsprintf(szText, "Day : %d", dayCnt);
    TextOut(hdc, WINSIZE_X - 100, 20, szText, strlen(szText));

    // 
    wsprintf(szText, "Time : %d", timeCnt);
    TextOut(hdc, WINSIZE_X - 100, 85, szText, strlen(szText));

    // 
    wsprintf(szText, "Money : %d", money);
    TextOut(hdc, WINSIZE_X - 135, 145, szText, strlen(szText));
}
