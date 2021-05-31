#include "StoreData.h"
#include "Image.h"

HRESULT StoreData::Init()
{
	// 곡물 정보
	ImageManager::GetSingleton()->AddImage("상품정보", "Image/cropData.bmp", 1620, 169, 9, 1, true, RGB(0, 0, 0));
	cropData = ImageManager::GetSingleton()->FindImage("상품정보");

	// 정보 셋팅
	mCropData.insert(pair<int, CROP_DATA*>(0, new CROP_DATA(0, "Parsnip", 20)));
	mCropData.insert(pair<int, CROP_DATA*>(1, new CROP_DATA(1, "Cauliflower", 80)));
	mCropData.insert(pair<int, CROP_DATA*>(2, new CROP_DATA(2, "Rhubarb", 100)));
	mCropData.insert(pair<int, CROP_DATA*>(3, new CROP_DATA(3, "Blueberry", 80)));
	mCropData.insert(pair<int, CROP_DATA*>(4, new CROP_DATA(4, "Cranberries", 240)));
	mCropData.insert(pair<int, CROP_DATA*>(5, new CROP_DATA(5, "FairyRose", 200)));
	mCropData.insert(pair<int, CROP_DATA*>(6, new CROP_DATA(6, "AncientFruit", 550)));
	mCropData.insert(pair<int, CROP_DATA*>(7, new CROP_DATA(7, "GreenBean", 60)));
	mCropData.insert(pair<int, CROP_DATA*>(8, new CROP_DATA(8, "Grape", 60)));

	// 인덱스
	listIndex = 0;

	return S_OK;
}

void StoreData::Release()
{
}

void StoreData::Update()
{
}

void StoreData::Render(HDC hdc)
{
	// 어떤 곡식의 정보를 렌더 할 것인가
	if(cropData)
		cropData->FrameRender(hdc, g_ptMouse.x, g_ptMouse.y, listIndex, 0);

	// 인덱스
	//wsprintf(szText, "listIndex: %d", listIndex);
	//TextOut(hdc, 0, 135, szText, strlen(szText));
}
