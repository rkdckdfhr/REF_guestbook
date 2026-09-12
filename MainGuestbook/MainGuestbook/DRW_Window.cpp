#include "DRW_Window.h"
#include "framework.h"
#include "Replay.h"
#include "Pen_tool.h"
#include "Resource.h"
#include "ReplayWindow.h"

#include "InitUI.h"
#include "File_io.h"
#include "UtilFunc.h"

std::vector<Line> DrwWindow::lines;
POINT DrwWindow::draw_start;
POINT DrwWindow::draw_end;


/*
안될땐 꼭 브레이크 걸고 디버그 실행해서 한줄씩 찾기
*/


bool DrwWindow::NewWnd(HINSTANCE hInst, HWND pHwnd)
{


	WNDCLASS wc = {};
	wc.lpfnWndProc = DrawWndProc;
	wc.hInstance = hInst;
	wc.lpszClassName = L"MainWindowClass";
	wc.hbrBackground = CreateSolidBrush(RGB(230, 230, 230));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClass(&wc);

	/// 부모창의 크기를 구하는 코드 자식 창의 크기를 부모창에 맞추기 위함
	RECT rect;
	GetClientRect(pHwnd, &rect);

	/// rect.right = 부모창의 너비 , rect.bottom = 부모창의 높이

	HWND hWnd = CreateWindowEx(0,
		wc.lpszClassName, L"TestWindow", WS_CHILD | WS_VISIBLE,
		0, 70,
		rect.right, rect.bottom,
		pHwnd, NULL, hInst, this);


	// 자식 윈도우 생성이 되는지 유효성 검사
	//if (!hWnd)
	//{
	//	
	//	DWORD err = GetLastError();
	//	wchar_t buf[256];
	//	swprintf_s(buf, L"자식 윈도우 생성 실패, 에러 코드: %lu", err);
	//	MessageBoxW(NULL, buf, L"Error", MB_OK);

	//	return false;
	//}
	//

	//ShowWindow(hWnd, NULL);
	
	return true;
}

DrwWindow DW;
INIT_UI ui;
Pen_tool myPen; //기본 세팅된 펜
/* 펜 스타일 옵션 줄때 괄호안에 스타일,두께,색상주기
PS_DASH 파선
PS_DASHDOT 점선
PS_DASHDOTDOT 점선
*/

unsigned int a = 1;

LRESULT CALLBACK DrwWindow::DrawWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	/// @TODO: 공부해서 채워넣자
	/// 반드시 DefWindowProc로 운영체제에 남은 자원 반환
	/// 이곳에 그리기 기능 넣기
	switch (message)
	{
	case WM_CREATE:
	{
		/// TODO: 채워넣기

		/// 버튼 생성

		//GetModuleHandle 하면 현재 창나온 인스턴스 핸들을 가져올 수 있음
		HINSTANCE hInst = GetModuleHandle(NULL);
		// 인스터스명 그냥 귀찮아서 ui로 변수 명만듬
		ui.InitUI(hWnd, hInst);
	}
	break;

	case WM_COMMAND:
	{

		/// 버튼 클릭시 이벤트 발생 시키는 switch문
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case BUTTON_PEN:
			MessageBox(hWnd, L"준비 중", L"펜 스타일", MB_OK);
			break;
		case BUTTON_COLOR:
			myPen.SelectColor(hWnd);
			break;
		case BUTTON_ERASER:
			lines.clear();
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case BUTTON_SAVE:
		{
			wchar_t path[MAX_PATH] = L"";
			if (ShowFileDialog(hWnd, path, true))
				File_Save(path, lines);
		}
			break;
		case BUTTON_PLAY:
		{
			MessageBox(hWnd, L"아직 준비 중입니다. 재생", L"재생 버튼", MB_OK);
			ReplayWindow RW;
			/// 현재 윈도우의 핸들 받아와서 넘기기 리플레이 윈도우의 부모 핸들로 넘김
			HINSTANCE DrawHinst = GetModuleHandle(NULL);
			RW.NewReplayWnd(DrawHinst, hWnd);
		}
			break;
		case BUTTON_STOP:
			MessageBox(hWnd, L"아직 준비 중입니다. 정지", L"정지 버튼", MB_OK);
			break;

		case 1002:
		{
			wchar_t path[MAX_PATH] = L"";
			if (ShowFileDialog(hWnd, path, false))
			{
				File_Call(path, lines);
				InvalidateRect(hWnd, NULL, TRUE);
			}
		}
		break;
		}
	}
	break;

	case WM_CTLCOLORSTATIC:
	{

		LRESULT hBrush = ui.InitColor(wParam, lParam);


		if (hBrush != 0)
		{
			return hBrush;
		}
	}
	break;

	/// 펜 두께 슬라이더로 조절
	case WM_VSCROLL:
	{
		// 슬라이더 바 위치로 펜 두께 적용
		// 날 위한 주석 myPan.width 는 int타입이라 형변환
		// myPen.width = 31 - 어쩌구 : 보기 편하려고 슬라이더 위 아래를 바꿈
		myPen.width = 31 - (int)SendMessage(ui.hPenBar, TBM_GETPOS, 0, 0);
	}
	break;

	case WM_LBUTTONDOWN:
	{
		DW.is_drawing = true;
		draw_start.x = LOWORD(lParam);
		draw_start.y = HIWORD(lParam);

	}
	break;

	case WM_MOUSEMOVE:
	{
		if (draw_start.y < 70) DW.is_drawing = false;

		if (DW.is_drawing)
		{
			HDC hdc = GetDC(hWnd);

			draw_end.x = LOWORD(lParam);
			draw_end.y = HIWORD(lParam);
			ULONGLONG get_time = GetTickCount64();

			HPEN hPen = myPen.Pen();

			/// LOGPEN = 현재 펜 정보를 담을 수 있는 WIN32 API 제공 구조체
			EXTLOGPEN new_pen;
			
			/// 현재 펜의 오브젝트를 넘겨서 저장
			GetObject(hPen, sizeof(EXTLOGPEN), &new_pen);

			HPEN Default = (HPEN)SelectObject(hdc, hPen);
			/* 펜 스타일 옵션 줄때 괄호안에 스타일,두께,색상주기
			PS_DASH 파선
			PS_DASHDOT 점선
			PS_DASHDOTDOT 점선
			*/
			HPEN oldPen = (HPEN)SelectObject(hdc, hPen);
			MoveToEx(hdc, draw_start.x, draw_start.y, NULL);
			LineTo(hdc, draw_end.x, draw_end.y);
			lines.push_back({ draw_start, draw_end, new_pen, get_time });

			draw_start = draw_end;

			SelectObject(hdc, oldPen);

			DeleteObject(hPen);
			
			ReleaseDC(hWnd, hdc);
		}
	}
	break;

	case WM_LBUTTONUP:
	{
		DW.is_drawing = false;
	}
	break;

	case WM_SIZE:
	{

	}
	break;
	
	
	case WM_PAINT:
	{
		PAINTSTRUCT cPs;
		HDC hdc = BeginPaint(hWnd, &cPs);

		/// 벡터의 내용으로 PAINT에 그리는 함수
		/// UtilFunc.cpp 참조
		GetPaintLine(hdc, &lines);


		EndPaint(hWnd, &cPs);
	}
	break;


	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	
	return DefWindowProc(hWnd, message, wParam, lParam);

}



DrwWindow::DrwWindow()
{
}


DrwWindow::~DrwWindow()
{

}