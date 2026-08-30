#pragma once

#include <windows.h>
#include <stdio.h>
#include <vector>
#include "Replay.h"
#include "Pen_tool.h"

#include "InitUI.h"
/*
HINSTANCE는 "Handle to an INSTance"의 약자로,
윈도우 운영체제가 메모리에 로드된 현재 프로그램(실행 파일)을 식별하기 위해 부여하는 고유 ID (시작 주소)임.
OS는 여러 프로그램이 동시에 돌아가는 환경이므로
"이 윈도우 창이 어떤 프로그램의 것인지", "이 아이콘이 어떤 실행 파일에 들어있는지"를 구분해야 할때 HINSTANCE의 주소로 구분
*/


/**
* @brief 점의 시작과 끝 좌표를 구하기 위한 Win32 제공 구조체
* @brief POINT 내부엔 LONG x, LONG y만 선언 되어있음
*/
struct Line
{
	POINT start;
	POINT end;
};

/*
struct DrawPoint
{
	LONG x;
	LONG y;
} DrawStart, DrawEnd;
*/



class DrwWindow
{
public:
	DrwWindow();
	~DrwWindow();
	

	/// 그리기 기능에 필요한 변수들
	bool is_drawing = false;
	static std::vector<Line> lines;


	static POINT draw_start; /// 그리기 시작 좌표 저장
	static POINT draw_end; /// 그리기 끝 좌표 저장


	bool NewWnd(HINSTANCE hInst, HWND pHwnd);

	static LRESULT CALLBACK DrawWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE hInstance;
};
