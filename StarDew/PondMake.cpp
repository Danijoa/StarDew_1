#include "PondMake.h"
#include "Image.h"

HRESULT PondMake::Init()
{
    //ImageManager::GetSingleton()->AddImage("¿¬¸ø»ùÇÃ", ");
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
