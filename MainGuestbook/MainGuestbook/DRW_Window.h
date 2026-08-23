#pragma once

#include <windows.h>
#include <stdio.h>

/*
HINSTANCE는 "Handle to an INSTance"의 약자로,
윈도우 운영체제가 메모리에 로드된 현재 프로그램(실행 파일)을 식별하기 위해 부여하는 고유 ID (시작 주소)임.
OS는 여러 프로그램이 동시에 돌아가는 환경이므로
"이 윈도우 창이 어떤 프로그램의 것인지", "이 아이콘이 어떤 실행 파일에 들어있는지"를 구분해야 할때 HINSTANCE의 주소로 구분
*/


class DrwWindow
{
public:
	DrwWindow();
	~DrwWindow();
	
	bool NewWnd(HINSTANCE hInst, HWND pHwnd);

	static LRESULT CALLBACK TestWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE hInstance;
};
