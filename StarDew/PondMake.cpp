#include "PondMake.h"
#include "Image.h"

HRESULT PondMake::Init()
{
    //ImageManager::GetSingleton()->AddImage("��������", ");
    return S_OK;
}

void PondMake::Release()
{
}

void PondMake::Update()
{
    if (KeyManager::GetSingleton()->IsOnceKeyDown('I'))
    {

    }
}

void PondMake::Render(HDC hdc)
{
}
