#pragma once

#include "framework.h"
#include "StructData.h"

void GetPaintLine(HDC, std::vector<Line>*);

HPEN GetCurrentPen(const EXTLOGPEN* ExtLogPen);