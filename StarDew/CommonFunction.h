// CommonFunction.h

#pragma once
#include "config.h"

// 두 점 사이의 각도를 구하는 함수
inline float GetAngle(FPOINT pos, FPOINT targetPos)
{
	float angle = 0.0f;
	float x = targetPos.x - pos.x;
	float y = targetPos.y - pos.y;

	angle = atan2(-y, x);

	return angle;
}

// 윈도우 크기 조정
inline void SetClientRect(HWND hWnd, int width, int height)
{
	RECT crt;
	DWORD style, exStyle;

	SetRect(&crt, 0, 0, width, height);
	style = GetWindowLong(hWnd, GWL_STYLE);
	exStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
	AdjustWindowRectEx(&crt, style, GetMenu(hWnd) != NULL, exStyle);
	if (style & WS_VSCROLL) crt.right += GetSystemMetrics(SM_CXVSCROLL);
	if (style & WS_HSCROLL) crt.bottom += GetSystemMetrics(SM_CYHSCROLL);
	SetWindowPos(hWnd, NULL, 0, 0, crt.right - crt.left, crt.bottom - crt.top,
		SWP_NOMOVE | SWP_NOZORDER);
}