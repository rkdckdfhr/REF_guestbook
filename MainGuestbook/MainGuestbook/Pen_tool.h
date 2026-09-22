#pragma once
#include <windows.h> 
#include <commctrl.h>	// 슬라이더 바 읽기 위해

class Pen_tool {
public:
	int width; // 펜의 두께
	int style; // 펜의 스타일
	COLORREF color; // 펜의 색상

	Pen_tool();
	void Pen_tool2(int style, int width);

	/// 색상 선택 창을 띄우고 내부 color 변수를 직접 변경하는 함수
	bool SelectColor(HWND hWnd);

	/// 마우스 위치 주변으로 점을 뿌리는 스프레이 함수
	void DrawSpray(HDC hdc, int x, int y);

	HPEN Pen();
};