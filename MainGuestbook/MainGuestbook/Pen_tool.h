#pragma once
#include <windows.h> 

class Pen_tool {
public:
	int width; // 펜의 두께
	int style; // 펜의 스타일
	COLORREF color; // 펜의 색상

	Pen_tool();
	Pen_tool(int style, int width, COLORREF color);

	HPEN Pen();
};