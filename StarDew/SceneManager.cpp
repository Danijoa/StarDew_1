#include "SceneManager.h"
#include "GameNode.h"

// static은 불러올 때 생성된다
// 하지만 컴파일 되는 시점에도 아래 내용을 필요하니까 초기화를 이렇게 해 준다
GameNode* SceneManager::currentScene = nullptr; // 현재 씬
GameNode* SceneManager::loadingScene = nullptr; // 로딩 씬
GameNode* SceneManager::readyScene = nullptr;   // 로딩 다음,init 끝나면 보여줄 씬

// global(전역)으로 만들어 둬야 하는 함수
DWORD CALLBACK LoadingThread(LPVOID pvParam)
{
    if (SUCCEEDED(SceneManager::readyScene->Init()))
    {
        SceneManager::currentScene = SceneManager::readyScene;

        SceneManager::loadingScene->Release();
        SceneManager::loadingScene = nullptr;
        SceneManager::readyScene = nullptr;
    }
    return 0;
}

HRESULT SceneManager::Init()
{
    return S_OK;
}

void SceneManager::Release()
{
    map<string, GameNode*>::iterator it;
    for (it = mSceneDatas.begin(); it != mSceneDatas.end(); it++)
    {
        if (it->second)
        {
            SAFE_RELEASE(it->second);
        }
    }
    mSceneDatas.clear();

    ReleaseSingleton();
}

void SceneManager::Update()
{
    if (currentScene)
    {
        currentScene->Update();
    }
}

void SceneManager::Render(HDC hdc)
{
    if (currentScene)
    {
        currentScene->Render(hdc);
    }
}

GameNode* SceneManager::AddScene(string key, GameNode* scene)
{
    // 객체 생성 자체가 안 된 경우
    if (scene == nullptr)
    {
        return nullptr;
    }

    // 삽입하려는 데이터가 이미 존재하는 경우
    map<string, GameNode*>::iterator it = mSceneDatas.find(key);
    if (it != mSceneDatas.end())
    {
        return it->second;	//해당 key에 맞는 GameNode 반환
    }

    // 삽입하려는 데이터가 존재하지 않는 경우
    mSceneDatas.insert(pair<string, GameNode*>(key, scene));

    return scene;
}

GameNode* SceneManager::AddLoadingScene(string key, GameNode* scene)
{
    if (scene == nullptr)
    {
        return nullptr;
    }

    map<string, GameNode*>::iterator it = mLoadingSceneDatas.find(key);
    if (it != mLoadingSceneDatas.end())
    {
        return it->second;
    }

    mLoadingSceneDatas.insert(pair<string, GameNode*>(key, scene));

    return scene;
}

HRESULT SceneManager::ChangeScene(string sceneName)
{
    map<string, GameNode*>::iterator it = mSceneDatas.find(sceneName);

    // 못찾은 경우
    if (it == mSceneDatas.end())
    {
        return E_FAIL;	// 실패하고, 암것도 안한다
    }

    // 찾는씬이 지금 보여주고 있는 씬인 경우
    if (it->second == currentScene)
    {
        return S_OK;	// 성공하고, 암것도 안한다
    }

    // 찾는씬이 지금 보여주고 있는 씬이 아니며
    if (SUCCEEDED(it->second->Init()))	// 찾는씬 초기화 성공
    {
        if (currentScene)
        {
            currentScene->Release();	// 지금 보여주는 씬 해제
        }
        currentScene = it->second;	// 찾는씬으로 변경

        return S_OK;
    }

    //초기화 실패
    return E_FAIL;
}

HRESULT SceneManager::ChangeScene(string sceneName, string loadingsceneName)
{
    // init 시간이 많이 걸린다고 생각하였을 때
    // 스레드를 사용하여 분할 작업하여(내부적으로 동시는 아님, 번갈아가면서, 공간만 분리)
    // 빠르게 작업이 완료되는 로딩씬을 먼저 보여주고 
    // 이닛을 작업한다

    map<string, GameNode*>::iterator it = mSceneDatas.find(sceneName);
    if (it == mSceneDatas.end())
    {
        return E_FAIL;
    }

    if (it->second == currentScene)
    {
        return S_OK;
    }

    // loading 씬 찾기
    map<string, GameNode*>::iterator itLoading
        = mLoadingSceneDatas.find(loadingsceneName);

    // 못찾은 경우
    if (itLoading == mLoadingSceneDatas.end())
    {
        return ChangeScene(sceneName);
    }

    // 찾은 경우
    // loading 씬을 먼저 보여준다
    if (SUCCEEDED(itLoading->second->Init()))
    {
        if (currentScene)
        {
            currentScene->Release();
        }
        currentScene = itLoading->second;

        readyScene = it->second;
        loadingScene = itLoading->second;

        //다음 씬을 초기화 할 쓰레드를 생성
        DWORD loadingThreadId;
        HANDLE hThread;
        hThread = CreateThread(NULL, 0,
            LoadingThread,          // 실행 시킬 함수 포인터
            NULL,                   // 실행 시킬 함수에 들어갈 매개변수
            0,
            &loadingThreadId
        );

        CloseHandle(hThread);
        return S_OK;
    }

    return E_FAIL;
}
