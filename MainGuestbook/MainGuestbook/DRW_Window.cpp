#include "DRW_Window.h"


std::vector<Line> DrwWindow::lines;
POINT DrwWindow::draw_start;
POINT DrwWindow::draw_end;


/*
안될땐 꼭 브레이크 걸고 디버그 실행해서 한줄씩 찾기
*/

bool DrwWindow::NewWnd(HINSTANCE hInst, HWND pHwnd)
{
	hInstance = hInst;

	//WCHAR ClassName = L"TestClass";

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
		0, 0,
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
Pen_tool pt;

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
		//GetModuleHandle 하면 현재 창나온 인스턴스 핸들을 가져올 수 있음
		HINSTANCE hInst = GetModuleHandle(NULL);
		// 인스터스명 그냥 귀찮아서 ui로 변수 명만듬
		ui.InitUI(hWnd, hInst);
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

	case WM_LBUTTONDOWN:
	{
		DW.is_drawing = true;
		draw_start.x = LOWORD(lParam);
		draw_start.y = HIWORD(lParam);
	}
	break;

	case WM_MOUSEMOVE:
	{
		if (DW.is_drawing)
		{
			HDC hdc = GetDC(hWnd);

			draw_end.x = LOWORD(lParam);
			draw_end.y = HIWORD(lParam);

			MoveToEx(hdc, draw_start.x, draw_start.y, NULL);
			LineTo(hdc, draw_end.x, draw_end.y);

			DW.lines.push_back({ draw_start, draw_end });

			draw_start = draw_end;

			ReleaseDC(hWnd, hdc);
		}
	}
	break;

	case WM_LBUTTONUP:
	{
		DW.is_drawing = false;
	}
	break;

	case WM_RBUTTONUP:
	{
		//HWND cHwnd = GetWindow(hWnd, GW_CHILD);
		//ThreadTrigger(cHwnd);
		HPEN hp = pt.Pen();
		
		pt.SelectColor(hWnd);

	}
	break;

	case WM_COMMAND:
	{

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

		if (isReplaying)
		{
			for (unsigned i = 0; i < tmp_Replay; i++)
			{
				MoveToEx(hdc, lines[i].start.x, lines[i].start.y, NULL);
				LineTo(hdc, lines[i].end.x, lines[i].end.y);
			}
		}
		else
		{
			for (int i = 0; i < lines.size(); i++)
			{
				MoveToEx(hdc, lines[i].start.x, lines[i].start.y, NULL);
				LineTo(hdc, lines[i].end.x, lines[i].end.y);
			}
		}

		//for (int i = 0; i < lines.size(); i++)
		//{
		//	MoveToEx(hdc, lines[i].start.x, lines[i].start.y, NULL);
		//	LineTo(hdc, lines[i].end.x, lines[i].end.y);
		//}

		EndPaint(hWnd, &cPs);
	}
	break;


	}
	
	
	return DefWindowProc(hWnd, message, wParam, lParam);
}


DrwWindow::DrwWindow()
{
}


DrwWindow::~DrwWindow()
{

}