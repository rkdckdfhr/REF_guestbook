#pragma once

#include "framework.h"
#include "StructData.h"

/// 벡터의 내용을 가져와 화면에 그려주는 함수
/// 매개변수 :
///			 현재 디바이스 정보, 그려질 벡터의 주소
void GetPaintLine(HDC, std::vector<Line>*);

/// EXTCreatePen 함수에 대한 정보를 담고 장착? 시켜주기 위한 함수
/// 매개변수로 현재 팬의 로그를 받음
HPEN GetCurrentPen(const EXTLOGPEN* ExtLogPen);