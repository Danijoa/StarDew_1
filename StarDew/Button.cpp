#include "Button.h"
#include "Image.h"

HRESULT Button::Init(const char* fileName, int posX, int posY)
{
    // �⺻ ����
    state = ButtonState::NONE;
    
    // �޾ƿ� �̸��� ������ ��ư �̹��� �ҷ�����
    buttonImg = ImageManager::GetSingleton()->FindImage(fileName);
    if (buttonImg == nullptr)
    {
        string warningText = string(fileName) + "�� ����!!";
        MessageBox(g_hWnd, warningText.c_str(), "����", MB_OK);
        return E_FAIL;
    }

    // �׷��� (left, top)��ġ
    pos.x = posX;
    pos.y = posY;

    // rect ����
    rc.left = pos.x;
    rc.top = pos.y;
    rc.right = rc.left + buttonImg->GetWidth() / 2;
    rc.bottom = rc.top + buttonImg->GetFrameHeight();

    // ����� ���(������ �Լ�)
    func = nullptr;

    return S_OK;
}

HRESULT Button::Init()
{
    return E_NOTIMPL;
}

void Button::Release()
{
}

void Button::Update()
{
    if (PtInRect(&rc, g_ptMouse))
    {
        state = ButtonState::STAY;
        
        if (KeyManager::GetSingleton()->IsOnceKeyUp(VK_LBUTTON))
        {
            state = ButtonState::UP;

            //��ư�ٿ� �� �� �ϴ� ���� ���� �� ���۵�
            if (func)
                func();
        }
    }
    else
    {
        state = ButtonState::NONE;
    }
}

void Button::Render(HDC hdc)
{
    switch (state)
    {
    case ButtonState::NONE:
        buttonImg->FrameRender(hdc, pos.x, pos.y, 0, 0);
        break;
    case ButtonState::STAY:
        buttonImg->FrameRender(hdc, pos.x, pos.y, 1, 0);
        //break;
    case ButtonState::DOWN:
        break;
    case ButtonState::UP:
        buttonImg->FrameRender(hdc, pos.x, pos.y, 1, 0);
        break;
    }
}
