
#pragma once
#include <windows.h>

class ScreenSaver
{
public:
	void StartScreenSaver(HWND hWnd);        // 기존에 만들려던 함수 그대로
	bool IsMouseMoved(LPARAM lParam);  // 시작할 때 위치에서 실제로 움직였는지

private:
	POINT m_startPos = { -1, -1 };           // 보호기가 시작된 순간의 마우스 위치
};
