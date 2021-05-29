#pragma once
#include "config.h"
#include "Singleton.h"

enum class ToolType {};
enum class CropType {};

class Image;
class SortManager;
class InventoryManager : public Singleton<InventoryManager>
{
private:
	// 폰트
	Image* numFont;
	Image* dayFont;

	// 시계
	Image* clock;
	int dayCnt;
	int timeCnt;
	int money;
	bool checkDayPass;

	// 다음날
	bool sleepDay;
	bool timeoutDay;
	bool energyoutDay;

	// 체력
	Image* energyBar;
	Image* energy;
	int playerEnergy;

	// 정렬
	static SortManager sortMgr;

	// 플레이어 작은 인벤토리
	Image* smallInven;
	RECT smallRect;
	bool smallInvenOpened;
	int sInvenX;	// 인덱스
	string selectedItem;
	int selectedIndex;
	ObjectType selectedObjectType;

	// 플레이어 큰 인벤토리
	Image* bigInven;
	RECT bigRect;
	bool bigInvenOpened;
	INT_PAIR bInven;	// 인덱스
	int tabCnt;

	// 곡물들
	Image* crops;

	// 농사 툴
	Image* tools;

	// 인벤에 띄어줄 이미지 이름, 프레임 위치를 담아야 하는건가..
	vector<INVEN_INFO*> vInven;
	int vIndex;
	int size;
	INT_PAIR downIndex;
	INT_PAIR upIndex;

	// 위치 알려주는 빨간,초록 렉트
	HPEN hpen;
	HPEN hpenOld;

	// 스크롤 사용 여부
	bool scrolledUp;
	bool scrolledDown;
	bool storeOpened;

	/* 확인용 */
	char szText[128] = "";
	float timeCheck;

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	// 스크롤
	inline void SetScrolledUp(bool up) { this->scrolledUp = up; }
	inline bool GetScrolledUp() { return this->scrolledUp; }

	inline void SetStoreListOpen(bool opened) { this->storeOpened = opened; }

	inline void SetScrolledDown(bool down) { this->scrolledDown = down; }
	inline bool GetScrolledDown() { return this->scrolledDown; }

	// 인벤토리에 있는지 여부
	inline vector<INVEN_INFO*>* GetVectorInven() { return &(this->vInven); }

	// 인벤토리 넣기
	inline int GetInvenIndex() { return this->vIndex; }
	inline void SetAddInven(INVEN_INFO* ivData) { this->vInven[vIndex] = ivData; }

	// 큰 인벤토리 생성 여부
	inline bool GetBigOpened() { return this->bigInvenOpened; }

	// 작은 인벤토리 크기
	inline RECT GetSmallInvenRect() { return this->smallRect; }

	// 플레이어가 선택한 아이템
	inline string GetSelectedItem() { return this->selectedItem; }
	inline int GetSelectedIndex() { return this->selectedIndex; }
	inline ObjectType GetSeletecObjectType() { return this->selectedObjectType; }

	// 돈
	inline int GetPlayerMoney() { return this->money; }
	inline void SetPlayerMoneyDecrease(int price) { this->money -= price; }

	// 날짜
	inline int GetDay() { return this->dayCnt; }
	inline void SetDay() { this->dayCnt += 1; }

	inline bool GetDayCheck() { return this->checkDayPass; }
	inline void SetDayCheck(bool dayPass) { this->checkDayPass = dayPass; }

	// 체력
	inline void SetEnergy(int used) { this->playerEnergy -= used; }
	inline void SetReEnergy() { this->playerEnergy = 213; }

	// 다음날
	inline void SetSleepDay(bool check) { this->sleepDay = check; }
	inline bool GetSleepDay() { return this->sleepDay; }

	inline void SetTimeoutDay(bool check) { this->timeoutDay = check; }
	inline bool GetTimeoutDay() { return this->timeoutDay; }

	inline void SetEnergyoutDay(bool check) { this->energyoutDay = check; }
	inline bool GetEnergyoutDay() { return this->energyoutDay; }

};

