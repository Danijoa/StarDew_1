// CommonFunction.h

#pragma once
#include "config.h"

// �� �� ������ ������ ���ϴ� �Լ�
inline float GetAngle(FPOINT pos, FPOINT targetPos)
{
	float angle = 0.0f;
	float x = targetPos.x - pos.x;
	float y = targetPos.y - pos.y;

	angle = atan2(-y, x);

	return angle;
}

// ������ ũ�� ����
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