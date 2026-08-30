#pragma once
#include <windows.h> 

class Pen_tool {
public:
	int width; // 펜의 두께
	int style; // 펜의 스타일
	COLORREF color; // 펜의 색상

	Pen_tool();
	void Pen_tool2(int style, int width);

	/// 색상 선택 창을 띄우고 내부 color 변수를 직접 변경하는 함수
	bool SelectColor(HWND hWnd);

	HPEN Pen();
};