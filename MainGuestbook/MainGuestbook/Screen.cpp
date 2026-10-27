#include "framework.h"     // windows.h + vector 가 이미 들어 있음
#include "Screen.h"
#include "Replay.h"
#include "DRW_Window.h"


extern bool g_isScreenSaverOn;
extern bool g_isManualReplay;

void ScreenSaver::StartScreenSaver(HWND hWnd)
{
	if (DrwWindow::lines.empty()) return;   // 그릴 게 없으면 시작 안 함

	GetCursorPos(&m_startPos);
	ScreenToClient(hWnd, &m_startPos);

	g_isScreenSaverOn = true;
	g_isManualReplay = false;
	ThreadTrigger(hWnd);
}


bool ScreenSaver::IsMouseMoved(LPARAM lParam)  
{
	int x = LOWORD(lParam);   // 현재 마우스 x
	int y = HIWORD(lParam);   // 현재 마우스 y

	// 보호기 시작할 때 위치와 똑같으면 안 움직인 것
	if (x == m_startPos.x && y == m_startPos.y)
	{
		return false;
	}

	// 위치가 다르면 움직인 것
	return true;
}