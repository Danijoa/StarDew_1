#include "Button.h"
#include "Image.h"

HRESULT Button::Init(const char* fileName, int posX, int posY)
{
    // 기본 상태
    state = ButtonState::NONE;
    
    // 받아온 이름과 동일한 버튼 이미지 불러오기
    buttonImg = ImageManager::GetSingleton()->FindImage(fileName);
    if (buttonImg == nullptr)
    {
        string warningText = string(fileName) + "가 없다!!";
        MessageBox(g_hWnd, warningText.c_str(), "실패", MB_OK);
        return E_FAIL;
    }

    // 그려질 (left, top)위치
    pos.x = posX;
    pos.y = posY;

    // rect 정보
    rc.left = pos.x;
    rc.top = pos.y;
    rc.right = rc.left + buttonImg->GetWidth() / 2;
    rc.bottom = rc.top + buttonImg->GetFrameHeight();

    // 연결될 기능(수행할 함수)
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

            //버튼다운 후 업 하는 순간 수행 할 동작들
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
