#include "UtilFunc.h"
#include "Replay.h"
#include "Pen_tool.h"
#include "buffer.h"

extern Pen_tool myPen;
extern ReplayBuffer replay_buffer;

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

void GetPaintLine(HDC currentDc, std::vector<Line>* lines, bool replayFlag)
{
	if (replayFlag)
	{

		//버퍼 없는 상태면 버퍼 생성
		if (replay_buffer.bf_dc == nullptr)
		{
			//버퍼를 위한 rect 선언
			RECT br;

			// 받은 핸들 찾고 알아낸 내용을 바탕으로 창 크기 받아오기
			HWND hWnd = WindowFromDC(currentDc);
			GetClientRect(hWnd, &br);
			
			//버퍼 생성
			replay_buffer.CreateBuffer(currentDc, br.right - br.left, br.bottom - br.top);
		}

		// 리플레이 시작시 다시 세팅하는 조건문
		if (tmp_Replay == 0)
		{
			replay_buffer.ClearBuffer();
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

		for (unsigned i = 0; i < replay_limit; i++)
		{

			// line 변수 선언
			const Line& line = (*lines)[i];
			HPEN current_pen = GetCurrentPen(&(*lines)[i].current_pen);
			
			if (replay_buffer.bf_dc == nullptr)
			{
				// 버퍼가 없으면 펜 메모리 해제
				if (current_pen) {
					DeleteObject(current_pen);
				}
				continue;
			}

			// 화면에 직접하지말고 버퍼에 그리게 수정
			HPEN OldPen = (HPEN)SelectObject(replay_buffer.bf_dc, current_pen);

			//lines에서 스프레이 플래그 사용중이라면 진행될 조건문
			if ((*lines)[i].is_spray)
			{
				// 스프레이 점들 다 똑같이 찍히게 고정 
				// 안하면 스프레이가 지글지글거림
				srand(i);

				// 저장 색 반영시킴
				myPen.color = line.color;

				// 스프레이 크기 받아옴
				if (line.width > 0)
				{
					myPen.width = line.width;
				}

				// 스프레이 호출시킴
				myPen.DrawSpray(replay_buffer.bf_dc, (*lines)[i].end.x, (*lines)[i].end.y);

			}
			else
			{
				MoveToEx(replay_buffer.bf_dc, (*lines)[i].start.x, (*lines)[i].start.y, NULL);
				LineTo(replay_buffer.bf_dc, (*lines)[i].end.x, (*lines)[i].end.y);
			}
			SelectObject(replay_buffer.bf_dc, OldPen);
			DeleteObject(current_pen);
		}

		// 버퍼에 그린걸 화면에 그리기
		replay_buffer.DrawBuffer(currentDc);
	}
	else
	{
	for (int i = 0; i < lines->size(); i++)
		{
			const Line& line = (*lines)[i];
			HPEN current_pen = GetCurrentPen(&(*lines)[i].current_pen);
			HPEN OldPen = (HPEN)SelectObject(currentDc, current_pen);

			if ((*lines)[i].is_spray)
			{
				srand(i);
				myPen.color = line.color;
				
				if (line.width > 0)
				{
					myPen.width = line.width;
				}
				
				
				myPen.DrawSpray(currentDc, (*lines)[i].end.x, (*lines)[i].end.y);
			}
			else
			{
				MoveToEx(currentDc, (*lines)[i].start.x, (*lines)[i].start.y, NULL);
				LineTo(currentDc, (*lines)[i].end.x, (*lines)[i].end.y);
			}
			SelectObject(currentDc, OldPen);
			DeleteObject(current_pen);
		}
	}
}