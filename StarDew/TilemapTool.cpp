#include "TilemapTool.h"
#include "Image.h"
#include "CommonFunction.h"

HRESULT TilemapTool::Init()
{
	// 크기 조정
	SetClientRect(g_hWnd, TILEMAPTOOLSIZE_X, TILEMAPTOOLSIZE_Y);

	/*확인용임*/
	ImageManager::GetSingleton()->AddImage("test", "Image/SpringFarm.bmp", 1280, 1040);
	test = ImageManager::GetSingleton()->FindImage("test");

	return S_OK;
}

void TilemapTool::Release()
{
}

void TilemapTool::Update()
{
}

void TilemapTool::Render(HDC hdc)
{
	PatBlt(hdc, 0, 0, TILEMAPTOOLSIZE_X, TILEMAPTOOLSIZE_Y, WHITENESS);

	if (test)
	{
		test->Render(hdc, 0, 0);
	}
}

void TilemapTool::Save(int sceneNum)
{
}

void TilemapTool::Load(int sceneNum)
{
}
