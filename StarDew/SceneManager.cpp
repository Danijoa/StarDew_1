#include "SceneManager.h"
#include "GameNode.h"

// static�� �ҷ��� �� �����ȴ�
// ������ ������ �Ǵ� �������� �Ʒ� ������ �ʿ��ϴϱ� �ʱ�ȭ�� �̷��� �� �ش�
GameNode* SceneManager::currentScene = nullptr; // ���� ��
GameNode* SceneManager::loadingScene = nullptr; // �ε� ��
GameNode* SceneManager::readyScene = nullptr;   // �ε� ����,init ������ ������ ��

// global(����)���� ����� �־� �ϴ� �Լ�
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
    // ��ü ���� ��ü�� �� �� ���
    if (scene == nullptr)
    {
        return nullptr;
    }

    // �����Ϸ��� �����Ͱ� �̹� �����ϴ� ���
    map<string, GameNode*>::iterator it = mSceneDatas.find(key);
    if (it != mSceneDatas.end())
    {
        return it->second;	//�ش� key�� �´� GameNode ��ȯ
    }

    // �����Ϸ��� �����Ͱ� �������� �ʴ� ���
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

    // ��ã�� ���
    if (it == mSceneDatas.end())
    {
        return E_FAIL;	// �����ϰ�, �ϰ͵� ���Ѵ�
    }

    // ã�¾��� ���� �����ְ� �ִ� ���� ���
    if (it->second == currentScene)
    {
        return S_OK;	// �����ϰ�, �ϰ͵� ���Ѵ�
    }

    // ã�¾��� ���� �����ְ� �ִ� ���� �ƴϸ�
    if (SUCCEEDED(it->second->Init()))	// ã�¾� �ʱ�ȭ ����
    {
        if (currentScene)
        {
            currentScene->Release();	// ���� �����ִ� �� ����
        }
        currentScene = it->second;	// ã�¾����� ����

        return S_OK;
    }

    //�ʱ�ȭ ����
    return E_FAIL;
}

HRESULT SceneManager::ChangeScene(string sceneName, string loadingsceneName)
{
    // init �ð��� ���� �ɸ��ٰ� �����Ͽ��� ��
    // �����带 ����Ͽ� ���� �۾��Ͽ�(���������� ���ô� �ƴ�, �����ư��鼭, ������ �и�)
    // ������ �۾��� �Ϸ�Ǵ� �ε����� ���� �����ְ� 
    // �̴��� �۾��Ѵ�

    map<string, GameNode*>::iterator it = mSceneDatas.find(sceneName);
    if (it == mSceneDatas.end())
    {
        return E_FAIL;
    }

    if (it->second == currentScene)
    {
        return S_OK;
    }

    // loading �� ã��
    map<string, GameNode*>::iterator itLoading
        = mLoadingSceneDatas.find(loadingsceneName);

    // ��ã�� ���
    if (itLoading == mLoadingSceneDatas.end())
    {
        return ChangeScene(sceneName);
    }

    // ã�� ���
    // loading ���� ���� �����ش�
    if (SUCCEEDED(itLoading->second->Init()))
    {
        if (currentScene)
        {
            currentScene->Release();
        }
        currentScene = itLoading->second;

        readyScene = it->second;
        loadingScene = itLoading->second;

        //���� ���� �ʱ�ȭ �� �����带 ����
        DWORD loadingThreadId;
        HANDLE hThread;
        hThread = CreateThread(NULL, 0,
            LoadingThread,          // ���� ��ų �Լ� ������
            NULL,                   // ���� ��ų �Լ��� �� �Ű�����
            0,
            &loadingThreadId
        );

        CloseHandle(hThread);
        return S_OK;
    }

    return E_FAIL;
}
