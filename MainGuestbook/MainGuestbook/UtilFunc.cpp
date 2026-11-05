#include "UtilFunc.h"
#include "Replay.h"
#include "Pen_tool.h"
#include "buffer.h"

extern Pen_tool myPen;
extern ReplayBuffer replay_buffer;

// 지금까지 그린 선 개수
static unsigned int draw_cnt = 0;
// 리플레이 판별 플래그
static bool replaying_call = false;

/// ExtCreatePen을 담기 위한 CreatePenIndirect() 함수 자체 제작
/// 현재의 펜 정보의 주소를 받아서 ExtCreatePen 함수에 담고
/// 값을 반환하여 사용 (WM_PAINT)
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

// 선 하나를 hdc에 그리는 공통 함수 (리플레이/일반 모드 둘 다 사용)
static void DrawOneLine(HDC hdc, const Line& line, unsigned int index)
{
	if (line.is_spray)
	{
		// 스프레이 점들 다 똑같이 찍히게 시드 고정
		// 안하면 스프레이가 지글지글거림
		srand(index);

		// last 변수 없애고 line에서 직접 떼옴
		myPen.color = line.color;
		if (line.width > 0)
		{
			myPen.width = line.width;
		}

		myPen.DrawSpray(hdc, line.end.x, line.end.y);
	}
	else
	{
		HPEN current_pen = GetCurrentPen(&line.current_pen);
		if (current_pen)
		{
			HPEN OldPen = (HPEN)SelectObject(hdc, current_pen);

			MoveToEx(hdc, line.start.x, line.start.y, NULL);
			LineTo(hdc, line.end.x, line.end.y);

			SelectObject(hdc, OldPen);
			DeleteObject(current_pen);
		}
	}
}

void GetPaintLine(HDC currentDc, std::vector<Line>* lines, bool replayFlag)
{
	// 그릴 대상이 없으면 아무것도 못 함 
	if (currentDc == nullptr) return;

	if (replayFlag)
	{
		// 리플레이 모드인데 시작처리가 안됐을 때
		if (replaying_call == false)
		{
			replay_buffer.ClearBuffer();
			draw_cnt = 0;
			replaying_call = true;
		}

		// 리플레이 line 한계선
		unsigned int replay_limit;

		if (tmp_Replay < lines->size())
		{
			replay_limit = tmp_Replay;
		}
		else
		{
			replay_limit = (unsigned int)lines->size();
		}

		// 재시작 등으로 한계선이 줄어들었으면 처음부터 다시
		if (replay_limit < draw_cnt)
		{
			replay_buffer.ClearBuffer();
			draw_cnt = 0;
		}

		// 색 백업 변수
		COLORREF backup_color = myPen.color;
		int backup_width = myPen.width;

		// 새로 그려진 선만 그림
		for (unsigned int i = draw_cnt; i < replay_limit; i++)
		{
			DrawOneLine(currentDc, (*lines)[i], i);
		}

		// 백업했던 사용자 펜 값 복구
		myPen.color = backup_color;
		myPen.width = backup_width;

		// 그린만큼 기록
		draw_cnt = replay_limit;
	}
	else
	{
		replaying_call = false;   
		draw_cnt = 0;

		COLORREF backup_color = myPen.color;
		int backup_width = myPen.width;

		for (unsigned int i = 0; i < lines->size(); i++)
		{
			DrawOneLine(currentDc, (*lines)[i], i);
		}

		myPen.color = backup_color;
		myPen.width = backup_width;
	}
}