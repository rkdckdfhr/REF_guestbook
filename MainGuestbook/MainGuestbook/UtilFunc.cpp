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

		// 이전 크기 기억할 변수
		int last_width = -1;
		// 이전 색 기억할 변수 
		COLORREF last_color = (COLORREF)-1;
		

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

			//lines에서 스프레이 플래그 사용중이라면 진행될 조건문
			if ((*lines)[i].is_spray)
			{
				// 스프레이 점들 다 똑같이 찍히게 고정 
				// 안하면 스프레이가 지글지글거림
				srand(i);

				// 이전 색 비교
				if (last_color != line.color)
				{
					// 저장 색 반영시킴
					myPen.color = line.color;
					// 이전 색에 현재 색 집어넣기
					last_color = line.color;
				}
				// 스프레이 크기 받아옴 + 이전 크기 비교
				if (line.width > 0 && last_width != line.width)
				{
					myPen.width = line.width;
					last_width = line.width;
				}

				// 스프레이 호출시킴
				myPen.DrawSpray(replay_buffer.bf_dc, (*lines)[i].end.x, (*lines)[i].end.y);

			}
			else
			{

				HPEN current_pen = GetCurrentPen(&(*lines)[i].current_pen);

				// oldPen쪽에서 버퍼가 안 만들어졌을 경우의 경고 생겨서 버퍼 확인 조건문도 추가
				if (current_pen && replay_buffer.bf_dc != nullptr)
				{
					// 버퍼 나갈때 펜 지우던 코드 삭제
					
					// 스프레이는 선이 필요없으니까 선 그릴때만 펜만듦
					HPEN OldPen = (HPEN)SelectObject(replay_buffer.bf_dc, current_pen);

					MoveToEx(replay_buffer.bf_dc, (*lines)[i].start.x, (*lines)[i].start.y, NULL);
					LineTo(replay_buffer.bf_dc, (*lines)[i].end.x, (*lines)[i].end.y);

					SelectObject(replay_buffer.bf_dc, OldPen);
					DeleteObject(current_pen);
				}
			}
		}

		// 버퍼에 그린걸 화면에 그리기
		replay_buffer.DrawBuffer(currentDc);
	}
	else
	{

		
		int last_width = -1;
		COLORREF last_color = (COLORREF)-1;

	for (int i = 0; i < lines->size(); i++)
		{
			const Line& line = (*lines)[i];

			if ((*lines)[i].is_spray)
			{
				srand(i);
				if (last_color != line.color)
				{
					myPen.color = line.color;
					last_color = line.color;
				}
				
				if (line.width > 0 && last_width != line.width)
				{
					myPen.width = line.width;
					last_width = line.width;
				}
				
				
				myPen.DrawSpray(currentDc, (*lines)[i].end.x, (*lines)[i].end.y);
			}
			else
			{
				HPEN current_pen = GetCurrentPen(&(*lines)[i].current_pen);
				if (current_pen)
				{
					HPEN OldPen = (HPEN)SelectObject(currentDc, current_pen);

					MoveToEx(currentDc, (*lines)[i].start.x, (*lines)[i].start.y, NULL);
					LineTo(currentDc, (*lines)[i].end.x, (*lines)[i].end.y);

					SelectObject(currentDc, OldPen);
					DeleteObject(current_pen);

				}
			}
			
		}
	}
}