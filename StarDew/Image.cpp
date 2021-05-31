#include "Image.h"

HRESULT Image::Init(int width, int height)
{
    HDC hdc = GetDC(g_hWnd);

    imageInfo = new IMAGE_INFO();
    imageInfo->resID = 0;
    imageInfo->hMemDC = CreateCompatibleDC(hdc);
    imageInfo->hBitmap = CreateCompatibleBitmap(hdc, width, height);
    imageInfo->hOldBit =
        (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBitmap);
    imageInfo->width = width;
    imageInfo->height = height;
    imageInfo->loadType = IMAGE_LOAD_KIND::EMPTY;

    ReleaseDC(g_hWnd, hdc);

    if (imageInfo->hBitmap == NULL)
    {
        Release();
        return E_FAIL;
    }

    this->isTransparent = FALSE;
    this->transColor = FALSE;

    return S_OK;
}

HRESULT Image::Init(const char* fileName, int width, int height,
    bool isTransparent/* = FALSE*/, COLORREF transColor/* = FALSE*/)
{
    HDC hdc = GetDC(g_hWnd);

    imageInfo = new IMAGE_INFO();
    imageInfo->resID = 0;

    imageInfo->hMemDC = CreateCompatibleDC(hdc);
    imageInfo->hBitmap =
        (HBITMAP)LoadImage(g_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
    imageInfo->hOldBit =
        (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBitmap);

    imageInfo->width = width;
    imageInfo->height = height;
    imageInfo->loadType = IMAGE_LOAD_KIND::FILE;

    // 알파 블랜드 dc 
    imageInfo->hBlendDC = CreateCompatibleDC(hdc);
    imageInfo->hBlendBit = CreateCompatibleBitmap(hdc,
        imageInfo->width*2, imageInfo->height*2);
    imageInfo->hOldBlendBit = (HBITMAP)SelectObject(imageInfo->hBlendDC,
        imageInfo->hBlendBit);

    // 회전 dc 
    imageInfo->hRotDC = CreateCompatibleDC(hdc);
    imageInfo->hRotBit = CreateCompatibleBitmap(hdc, imageInfo->width * 2.5, imageInfo->height * 2.5);
    imageInfo->hOldRotBit = (HBITMAP)SelectObject(imageInfo->hRotDC, imageInfo->hRotBit);

    imageInfo->hResetDC = CreateCompatibleDC(hdc);
    imageInfo->hResetBit = CreateCompatibleBitmap(hdc, imageInfo->width * 2.5, imageInfo->height * 2.5);
    imageInfo->hOldResetBit = (HBITMAP)SelectObject(imageInfo->hResetDC, imageInfo->hResetBit);

    myPoint = new POINT();

    ReleaseDC(g_hWnd, hdc);

    if (imageInfo->hBitmap == NULL)
    {
        Release();
        return E_FAIL;
    }

    this->isTransparent = isTransparent;
    this->transColor = transColor;

    this->blendFunc.AlphaFormat = 0;
    this->blendFunc.BlendFlags = 0;
    this->blendFunc.BlendOp = AC_SRC_OVER;
    this->blendFunc.SourceConstantAlpha = 255;

    return S_OK;
}

HRESULT Image::Init(const char* fileName, int width, int height, int maxFrameX, int maxFrameY, bool isTransparent, COLORREF transColor)
{
    HDC hdc = GetDC(g_hWnd);

    imageInfo = new IMAGE_INFO();
    imageInfo->resID = 0;
    imageInfo->hMemDC = CreateCompatibleDC(hdc);
    imageInfo->hBitmap =
        (HBITMAP)LoadImage(g_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
    imageInfo->hOldBit =
        (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBitmap);
    imageInfo->width = width;
    imageInfo->height = height;
    imageInfo->loadType = IMAGE_LOAD_KIND::FILE;

    imageInfo->maxFrameX = maxFrameX;
    imageInfo->maxFrameY = maxFrameY;
    imageInfo->frameWidth = width / maxFrameX;
    imageInfo->frameHeight = height / maxFrameY;
    imageInfo->currFrameX = 0;
    imageInfo->currFrameY = 0;

    ReleaseDC(g_hWnd, hdc);

    if (imageInfo->hBitmap == NULL)
    {
        Release();
        return E_FAIL;
    }

    this->isTransparent = isTransparent;
    this->transColor = transColor;

    return S_OK;
}

// 정적
void Image::Render(HDC hdc, int destX, int destY, bool isCenterRenderring)
{
    int x = destX;
    int y = destY;
    if (isCenterRenderring)
    {
        x = destX - (imageInfo->width / 2);
        y = destY - (imageInfo->height / 2);
    }

    if (isTransparent)
    {
        // 특정 색상을 빼고 복사하는 함수
        GdiTransparentBlt(
            hdc,
            x, y,
            imageInfo->width, imageInfo->height,
            imageInfo->hMemDC,
            0, 0,
            imageInfo->width, imageInfo->height,
            transColor
        );
    }
    else
    {
        // bitmap 에 있는 이미지 정보를 다른 비트맵에 복사
        BitBlt(
            hdc,                // 복사 목적지 DC
            x, y,               // 복사 시작 위치(윈도우 상)
            imageInfo->width,   // 원본에서 복사될 가로크기
            imageInfo->height,  // 원본에서 복사될 세로크기
            imageInfo->hMemDC,  // 원본 DC
            0, 0,               // 원본에서 복사 시작 위치
            SRCCOPY             // 복사 옵션
        );
    }
}

//
void Image::LightRender(HDC hdc, int destX, int destY)
{
    int x = destX;
    int y = destY;
     
	// bitmap 에 있는 이미지 정보를 다른 비트맵에 복사
	BitBlt(
		hdc,                    // 복사 목적지 DC
		x, y,                   // 복사 시작 위치(윈도우 상)
		imageInfo->width,       // 원본에서 복사될 가로크기
		imageInfo->height,      // 원본에서 복사될 세로크기
		imageInfo->hMemDC,      // 원본 DC
		0, 0,                   // 원본에서 복사 시작 위치
        MERGECOPY               // 복사 옵션 : 소스 비트맵과 대상 화면을 AND 연산한다 (MERGEPAINT : 소스 비트맵과 대상 화면을 OR 연산한다)
	);
}

// 정적
void Image::BarRender(HDC hdc, int destX, int destY, int height)
{
	int x = destX;
	int y = destY;

	// bitmap 에 있는 이미지 정보를 다른 비트맵에 복사
	BitBlt(
		hdc,                                                // 복사 목적지 DC
		x, y + (imageInfo->height - height),                // 복사 시작 위치(윈도우 상)
		imageInfo->width,                                   // 원본에서 복사될 가로크기
		height,                                             // 원본에서 복사될 세로크기
		imageInfo->hMemDC,                                  // 원본 DC
		0, 0,                                               // 원본에서 복사 시작 위치
		SRCCOPY                                             // 복사 옵션
	);
}

// 알파
void Image::AlphaRender(HDC hdc, int destX, int destY, bool isCenterRenderring)
{
    int x = destX;
    int y = destY;
    if (isCenterRenderring)
    {
        x = destX - (imageInfo->width / 2);
        y = destY - (imageInfo->height / 2);
    }

    // 원본DC(hdc)에 그려져 있는 내용(배경)을 (비어져 있는 임시) BlendDC에 복사
    BitBlt(
        imageInfo->hBlendDC,                 // 복사 목적지 DC
        0, 0,                                // 복사 시작 위치
        imageInfo->width,                    // 원본에서 복사될 가로크기
        imageInfo->height,                   // 원본에서 복사될 세로크기
        hdc,                                 // 원본 DC
        x, y,                                // 원본에서 복사 시작 위치
        SRCCOPY                              // 복사 옵션
    );

    // 출력할 이미지 hMemDC의 내용(우주선 이미지)을 지정한 색상(마젠타)을 제외하면서 BlendDC에 복사
    GdiTransparentBlt(
        imageInfo->hBlendDC,
        0, 0,
        imageInfo->width, imageInfo->height,
        imageInfo->hMemDC,
        0, 0,
        imageInfo->width, imageInfo->height,
        transColor
    );

    // BlendDC를 원본DC(hdc)에 그려주는 과정에서 알파블랜드 적용시키기
    AlphaBlend(
        hdc,
        x, y,
        imageInfo->width, imageInfo->height,
        imageInfo->hBlendDC,
        0, 0,
        imageInfo->width, imageInfo->height,
        blendFunc);
}

// 동적
void Image::FrameRender(HDC hdc, int destX, int destY, int currFrameX, int currFrameY, bool isCenterRenderring, int size)
{
    imageInfo->currFrameX = currFrameX;
    imageInfo->currFrameY = currFrameY;

    int x = destX;
    int y = destY;
    if (isCenterRenderring)
    {
        x = destX - (imageInfo->frameWidth / 2);
        y = destY - (imageInfo->frameHeight / 2);
    }

    if (isTransparent)
    {
        // 특정 색상을 빼고 복사하는 함수
        GdiTransparentBlt(
            hdc,                                            // 목적지 DC
            x, y,                                           // 복사 위치
            imageInfo->frameWidth * size,
            imageInfo->frameHeight * size,                  // 복사 크기
            imageInfo->hMemDC,                              // 원본 DC
            imageInfo->frameWidth * imageInfo->currFrameX,  // 복사 X 위치
            imageInfo->frameHeight * imageInfo->currFrameY, // 복사 Y 위치
            imageInfo->frameWidth, imageInfo->frameHeight,  // 복사 크기
            transColor                                      // 제외할 색상
        );
    }
    else
    {
        if (size > 1)
        {
            StretchBlt(hdc,
                x, y,
                imageInfo->frameWidth * size,
                imageInfo->frameHeight * size,
                imageInfo->hMemDC,
                imageInfo->frameWidth * imageInfo->currFrameX,
                imageInfo->frameHeight * imageInfo->currFrameY,
                imageInfo->frameWidth,
                imageInfo->frameHeight,
                SRCCOPY);
        }
        else
        {
            BitBlt(
                hdc,
                x, y,
                imageInfo->frameWidth,
                imageInfo->frameHeight,
                imageInfo->hMemDC,
                imageInfo->frameWidth * imageInfo->currFrameX,
                imageInfo->frameHeight * imageInfo->currFrameY,
                SRCCOPY
            );
        }
    }
}

// 동적
void Image::FrameListRender(HDC hdc, int destX, int destY, int tempHeight, int tempCopyY, int currFrameX, int currFrameY)
{
    imageInfo->currFrameX = currFrameX;
    imageInfo->currFrameY = currFrameY;

    int x = destX;
    int y = destY;

	BitBlt(
		hdc,
		x, y,

		imageInfo->frameWidth,
		tempHeight,

		imageInfo->hMemDC,

		imageInfo->frameWidth * imageInfo->currFrameX,
		imageInfo->frameHeight * imageInfo->currFrameY + tempCopyY,

		SRCCOPY
	);
}

// 회전
void Image::RotateRender(HDC hdc, int destX, int destY, float len, float angle)
{
    len += 0.5f;

    //POINT* myPoint;          //CONST
    //myPoint = new POINT();
    myPoint[0].x = (LONG)(len + cosf((135.0f + angle) * PI / 180.0f) * (len * sqrt(2))); // 왼쪽 상단
    myPoint[0].y = (LONG)(len - sinf((135.0f + angle) * PI / 180.0f) * (len * sqrt(2)));
    myPoint[1].x = (LONG)(len + cosf((45.0f + angle) * PI / 180.0f) * (len * sqrt(2)));  // 오른쪽 상단
    myPoint[1].y = (LONG)(len - sinf((45.0f + angle) * PI / 180.0f) * (len * sqrt(2)));
    myPoint[2].x = (LONG)(len + cosf((225.0f + angle) * PI / 180.0f) * (len * sqrt(2))); // 왼쪽 하단
    myPoint[2].y = (LONG)(len - sinf((225.0f + angle) * PI / 180.0f) * (len * sqrt(2)));
 
    int x = destX;
    int y = destY;

    // 돌리기
    PlgBlt(
        imageInfo->hRotDC,                  // 회전 이미지 저장할 복사 목적지 DC
        myPoint,                            // 변환된 꼭지점
        imageInfo->hMemDC,                  // 화살표 이미지 DC
        0, 0,                               // 원본에서 복사 시작 위치
        imageInfo->width,                   // 복사 가로 크기
        imageInfo->height,                  // 복사 세로 크기
        NULL, NULL, NULL
    );

    // 투명화
    GdiTransparentBlt(
        hdc,                                            // 목적지 DC
        x, y,                                           // 복사 위치
        imageInfo->width * 2.5,
        imageInfo->height * 2.5,                        // 복사 크기
        imageInfo->hRotDC,                              // 원본 DC
        0, 0,
        imageInfo->width, 
        imageInfo->height,
        transColor                                      // 제외할 색상
    );

    //// 지워주기
    //BitBlt(
    //    imageInfo->hRotDC,
    //    0, 0, 
    //    imageInfo->width * 2.5,
    //    imageInfo->height * 2.5,
    //    imageInfo->hResetDC,
    //    x, y, 
    //    SRCCOPY); 

}

void Image::Release()
{
    if (imageInfo)
    {
        SelectObject(imageInfo->hMemDC, imageInfo->hOldBit);
        DeleteObject(imageInfo->hBitmap);
        DeleteDC(imageInfo->hMemDC);

        if (imageInfo->hBlendDC != NULL)
        {
            SelectObject(imageInfo->hBlendDC, imageInfo->hOldBlendBit);
            DeleteObject(imageInfo->hBlendBit);
            DeleteDC(imageInfo->hBlendDC);
        }

        delete imageInfo;
        imageInfo = nullptr;
    }

    delete[] myPoint;
}
