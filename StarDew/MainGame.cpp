#include "MainGame.h"
#include "Image.h"
#include "TilemapTool.h"
#include "StartScene.h"
#include "HouseScene.h"
#include "FarmScene.h"

HRESULT MainGame::Init()
{
	hdc = GetDC(g_hWnd);

	KeyManager::GetSingleton()->Init();
	ImageManager::GetSingleton()->Init();
	SceneManager::GetSingleton()->Init();

	// ¹é¹öÆÛ ÀÌ¹ÌÁö
	int maxWidth, maxHeight;
	maxWidth = max(WINSIZE_X, TILEMAPTOOLSIZE_X);
	maxHeight = max(WINSIZE_Y, TILEMAPTOOLSIZE_Y);
	backBuffer = new Image();
	backBuffer->Init(maxWidth, maxHeight);

	// Å°¿Í ¾À ÀúÀå
	SceneManager::GetSingleton()->AddScene("Å¸ÀÏ¸ÊÅø", new TilemapTool());
	SceneManager::GetSingleton()->AddScene("½ÃÀÛ¾À", new StartScene());
	SceneManager::GetSingleton()->AddScene("ÇÏ¿ì½º¾À", new HouseScene());
	SceneManager::GetSingleton()->AddScene("³óÀå¾À", new FarmScene());

	// ½ÃÀÛ ¾À -> ÀÎ°ÔÀÓÀº x¹è·Î Å°¿ö¼­ Ãâ·ÂÇÏÀÚ
	//SceneManager::GetSingleton()->ChangeScene("Å¸ÀÏ¸ÊÅø");
	SceneManager::GetSingleton()->ChangeScene("½ÃÀÛ¾À");

	return S_OK;
}

void MainGame::Release()
{
	KeyManager::GetSingleton()->Release();
	ImageManager::GetSingleton()->Release();
	SceneManager::GetSingleton()->Release();

	SAFE_RELEASE(backBuffer);

	ReleaseDC(g_hWnd, hdc);
}

void MainGame::Update()
{
	SceneManager::GetSingleton()->Update();
}

void MainGame::Render()
{
	HDC hBackDC = backBuffer->GetMemDC();

	SceneManager::GetSingleton()->Render(hBackDC);

	// ¸¶¿ì½º ÁÂÇ¥
	wsprintf(szText, "X : %d, Y : %d", g_ptMouse.x, g_ptMouse.y);
	TextOut(hBackDC, 200, 20, szText, strlen(szText));

	// FPS
	TimerManager::GetSingleton()->Render(hBackDC);

	backBuffer->Render(hdc);
}

LRESULT MainGame::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_CREATE:
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_MOUSEMOVE:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

MainGame::MainGame()
{
}

MainGame::~MainGame()
{
}
