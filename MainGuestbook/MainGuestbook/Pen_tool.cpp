#include <windows.h>
#include "Pen_tool.h"
//기본세팅
Pen_tool::Pen_tool() {
	width = 1;
	style = PS_SOLID; //실선
	color = RGB(0, 0, 0);
}
//원하는 값 넣어서 값 세팅
Pen_tool::Pen_tool(int s, int w, COLORREF c) {
	style = s;
	width = w;
	color = c;
}
//진짜 펜 생성
HPEN Pen_tool::Pen() {
	return CreatePen(style, width, color);
}