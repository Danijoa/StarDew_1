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

    // ���� ���� dc 
    imageInfo->hBlendDC = CreateCompatibleDC(hdc);
    imageInfo->hBlendBit = CreateCompatibleBitmap(hdc,
        imageInfo->width*2, imageInfo->height*2);
    imageInfo->hOldBlendBit = (HBITMAP)SelectObject(imageInfo->hBlendDC,
        imageInfo->hBlendBit);

    // ȸ�� dc 
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

// ����
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
        // Ư�� ������ ���� �����ϴ� �Լ�
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
        // bitmap �� �ִ� �̹��� ������ �ٸ� ��Ʈ�ʿ� ����
        BitBlt(
            hdc,                // ���� ������ DC
            x, y,               // ���� ���� ��ġ(������ ��)
            imageInfo->width,   // �������� ����� ����ũ��
            imageInfo->height,  // �������� ����� ����ũ��
            imageInfo->hMemDC,  // ���� DC
            0, 0,               // �������� ���� ���� ��ġ
            SRCCOPY             // ���� �ɼ�
        );
    }
}

//
void Image::LightRender(HDC hdc, int destX, int destY)
{
    int x = destX;
    int y = destY;
     
	// bitmap �� �ִ� �̹��� ������ �ٸ� ��Ʈ�ʿ� ����
	BitBlt(
		hdc,                    // ���� ������ DC
		x, y,                   // ���� ���� ��ġ(������ ��)
		imageInfo->width,       // �������� ����� ����ũ��
		imageInfo->height,      // �������� ����� ����ũ��
		imageInfo->hMemDC,      // ���� DC
		0, 0,                   // �������� ���� ���� ��ġ
        MERGECOPY               // ���� �ɼ� : �ҽ� ��Ʈ�ʰ� ��� ȭ���� AND �����Ѵ� (MERGEPAINT : �ҽ� ��Ʈ�ʰ� ��� ȭ���� OR �����Ѵ�)
	);
}

// ����
void Image::BarRender(HDC hdc, int destX, int destY, int height)
{
	int x = destX;
	int y = destY;

	// bitmap �� �ִ� �̹��� ������ �ٸ� ��Ʈ�ʿ� ����
	BitBlt(
		hdc,                                                // ���� ������ DC
		x, y + (imageInfo->height - height),                // ���� ���� ��ġ(������ ��)
		imageInfo->width,                                   // �������� ����� ����ũ��
		height,                                             // �������� ����� ����ũ��
		imageInfo->hMemDC,                                  // ���� DC
		0, 0,                                               // �������� ���� ���� ��ġ
		SRCCOPY                                             // ���� �ɼ�
	);
}

// ����
void Image::AlphaRender(HDC hdc, int destX, int destY, bool isCenterRenderring)
{
    int x = destX;
    int y = destY;
    if (isCenterRenderring)
    {
        x = destX - (imageInfo->width / 2);
        y = destY - (imageInfo->height / 2);
    }

    // ����DC(hdc)�� �׷��� �ִ� ����(���)�� (����� �ִ� �ӽ�) BlendDC�� ����
    BitBlt(
        imageInfo->hBlendDC,                 // ���� ������ DC
        0, 0,                                // ���� ���� ��ġ
        imageInfo->width,                    // �������� ����� ����ũ��
        imageInfo->height,                   // �������� ����� ����ũ��
        hdc,                                 // ���� DC
        x, y,                                // �������� ���� ���� ��ġ
        SRCCOPY                              // ���� �ɼ�
    );

    // ����� �̹��� hMemDC�� ����(���ּ� �̹���)�� ������ ����(����Ÿ)�� �����ϸ鼭 BlendDC�� ����
    GdiTransparentBlt(
        imageInfo->hBlendDC,
        0, 0,
        imageInfo->width, imageInfo->height,
        imageInfo->hMemDC,
        0, 0,
        imageInfo->width, imageInfo->height,
        transColor
    );

    // BlendDC�� ����DC(hdc)�� �׷��ִ� �������� ���ĺ��� �����Ű��
    AlphaBlend(
        hdc,
        x, y,
        imageInfo->width, imageInfo->height,
        imageInfo->hBlendDC,
        0, 0,
        imageInfo->width, imageInfo->height,
        blendFunc);
}

// ����
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
        // Ư�� ������ ���� �����ϴ� �Լ�
        GdiTransparentBlt(
            hdc,                                            // ������ DC
            x, y,                                           // ���� ��ġ
            imageInfo->frameWidth * size,
            imageInfo->frameHeight * size,                  // ���� ũ��
            imageInfo->hMemDC,                              // ���� DC
            imageInfo->frameWidth * imageInfo->currFrameX,  // ���� X ��ġ
            imageInfo->frameHeight * imageInfo->currFrameY, // ���� Y ��ġ
            imageInfo->frameWidth, imageInfo->frameHeight,  // ���� ũ��
            transColor                                      // ������ ����
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

// ����
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

// ȸ��
void Image::RotateRender(HDC hdc, int destX, int destY, float len, float angle)
{
    len += 0.5f;

    //POINT* myPoint;          //CONST
    //myPoint = new POINT();
    myPoint[0].x = (LONG)(len + cosf((135.0f + angle) * PI / 180.0f) * (len * sqrt(2))); // ���� ���
    myPoint[0].y = (LONG)(len - sinf((135.0f + angle) * PI / 180.0f) * (len * sqrt(2)));
    myPoint[1].x = (LONG)(len + cosf((45.0f + angle) * PI / 180.0f) * (len * sqrt(2)));  // ������ ���
    myPoint[1].y = (LONG)(len - sinf((45.0f + angle) * PI / 180.0f) * (len * sqrt(2)));
    myPoint[2].x = (LONG)(len + cosf((225.0f + angle) * PI / 180.0f) * (len * sqrt(2))); // ���� �ϴ�
    myPoint[2].y = (LONG)(len - sinf((225.0f + angle) * PI / 180.0f) * (len * sqrt(2)));
 
    int x = destX;
    int y = destY;

    // ������
    PlgBlt(
        imageInfo->hRotDC,                  // ȸ�� �̹��� ������ ���� ������ DC
        myPoint,                            // ��ȯ�� ������
        imageInfo->hMemDC,                  // ȭ��ǥ �̹��� DC
        0, 0,                               // �������� ���� ���� ��ġ
        imageInfo->width,                   // ���� ���� ũ��
        imageInfo->height,                  // ���� ���� ũ��
        NULL, NULL, NULL
    );

    // ����ȭ
    GdiTransparentBlt(
        hdc,                                            // ������ DC
        x, y,                                           // ���� ��ġ
        imageInfo->width * 2.5,
        imageInfo->height * 2.5,                        // ���� ũ��
        imageInfo->hRotDC,                              // ���� DC
        0, 0,
        imageInfo->width, 
        imageInfo->height,
        transColor                                      // ������ ����
    );

    //// �����ֱ�
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
