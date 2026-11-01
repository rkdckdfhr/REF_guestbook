#pragma once

#include <windows.h>

/**
* @brief 점의 시작과 끝 좌표를 구하기 위한 Win32 제공 구조체
* @brief POINT 내부엔 LONG x, LONG y만 선언 되어있음
*/
struct Line
{
	POINT start;
	POINT end;
	EXTLOGPEN current_pen;
	ULONGLONG timestamp;

	//2026-10-23 이상욱 스프레이 변수 선언
	bool is_spray;

	//2026-10-24 이상욱 스프레이 색 버그 해결 선언
	COLORREF color;

	//2026-10-25 이상욱 스프레이 크기 버그 해결 선언
	int width;

	//2026-10-30 박종연 리플레이 시 스프레이 점 위치 동일 복원용 시드 선언
	unsigned int seed;
};