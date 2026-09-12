#include "UtilFunc.h"


HPEN GetCurrentPen(const EXTLOGPEN* ExtLogPen)
{
	if (!ExtLogPen) return NULL;


	LOGBRUSH eLb;
	eLb.lbStyle = ExtLogPen->elpBrushStyle;
	eLb.lbColor = ExtLogPen->elpColor;
	eLb.lbHatch = ExtLogPen->elpHatch;

	return ExtCreatePen(ExtLogPen->elpPenStyle, ExtLogPen->elpWidth,
		&eLb, ExtLogPen->elpNumEntries, NULL);
}

void GetPaintLine(HDC currentDc, std::vector<Line>* lines)
{
	for (int i = 0; i < lines->size(); i++)
	{
		HPEN current_pen = GetCurrentPen(&(*lines)[i].current_pen);
		HPEN OldPen = (HPEN)SelectObject(currentDc, current_pen);
		MoveToEx(currentDc, (*lines)[i].start.x, (*lines)[i].start.y, NULL);
		LineTo(currentDc, (*lines)[i].end.x, (*lines)[i].end.y);
		SelectObject(currentDc, OldPen);
		DeleteObject(current_pen);
	}
}