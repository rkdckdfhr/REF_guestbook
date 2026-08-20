#pragma once
#include <Windows.h>


/**
* @brief 점의 시작과 끝 좌표를 구하기 위한 Win32 제공 구조체
* @brief POINT 내부엔 LONG x, LONG y만 선언 되어있음
*/
struct Line
{
	POINT start;
	POINT end;
};



class MainTool
{
public:
	MainTool();
	~MainTool();

private:

};
