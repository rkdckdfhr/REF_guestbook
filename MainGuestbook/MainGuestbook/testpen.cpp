#include "testpen.h"

/// 펜 변경 테스트 클래스 


testPen::testPen()
{
}

testPen::~testPen()
{
}


HPEN testPen::hPen()
{

	HPEN hP = CreatePen(PS_DOT, 2, RGB(255, 0, 0));

	return hP;
}