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
	wc.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClass(&wc);

	
	HWND hWnd = CreateWindowEx(0,
		wc.lpszClassName, L"TestWindow", WS_CHILD | WS_VISIBLE,
		0, 0,
		300, 200,
		pHwnd, NULL, hInst, this);


	
	if (!hWnd)
	{
		
		DWORD err = GetLastError();
		wchar_t buf[256];
		swprintf_s(buf, L"자식 윈도우 생성 실패, 에러 코드: %lu", err);
		MessageBoxW(NULL, buf, L"Error", MB_OK);

		return false;
	}
	

	//ShowWindow(hWnd, NULL);
	
	return true;
}

DrwWindow DW;

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

	case WM_COMMAND:
	{

	}
	break;
	
	case WM_PAINT:
	{
		PAINTSTRUCT cPs;
		HDC hdc = BeginPaint(hWnd, &cPs);

		for (int i = 0; i < lines.size(); i++)
		{
			MoveToEx(hdc, lines[i].start.x, lines[i].start.y, NULL);
			LineTo(hdc, lines[i].end.x, lines[i].end.y);
		}

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