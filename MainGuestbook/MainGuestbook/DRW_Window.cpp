#include "DRW_Window.h"

/*
안될땐 꼭 브레이크 걸고 디버그 실행해서 한줄씩 찾기
*/

bool DrwWindow::NewWnd(HINSTANCE hInst, HWND pHwnd)
{
	hInstance = hInst;

	//WCHAR ClassName = L"TestClass";

	WNDCLASS wc = {};
	wc.lpfnWndProc = TestWndProc;
	wc.hInstance = hInst;
	wc.lpszClassName = L"MainWindowClass";
	wc.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClass(&wc);

	
	HWND hWnd = CreateWindowEx(0,
		wc.lpszClassName, L"TestWindow", WS_CHILD | WS_VISIBLE,
		0, 0,
		900, 700,
		pHwnd, NULL, hInst, this);


	/*
	if (!hWnd)
	{
		
		DWORD err = GetLastError();
		wchar_t buf[256];
		swprintf_s(buf, L"자식 윈도우 생성 실패, 에러 코드: %lu", err);
		MessageBoxW(NULL, buf, L"Error", MB_OK);

		return false;
	}
	*/

	//ShowWindow(hWnd, NULL);
	
	return true;
}

LRESULT CALLBACK DrwWindow::TestWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	/// @TODO: 공부해서 채워넣자
	/// 반드시 DefWindowProc로 운영체제에 남은 자원 반환
	/// 이곳에 그리기 기능 넣기
	
	
	
	return DefWindowProc(hWnd, message, wParam, lParam);
}


DrwWindow::DrwWindow()
{

}


DrwWindow::~DrwWindow()
{

}