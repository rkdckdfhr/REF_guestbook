#include "Ui_window.h"
#include "InitUI.h"

bool UiWnd(HINSTANCE hInst, HWND pHwnd)
{
	WNDCLASS uiwc = {};
	uiwc.lpfnWndProc = UiProc;
	uiwc.hInstance = hInst;
	uiwc.lpszClassName = L"UiWindow";
	uiwc.hbrBackground = CreateSolidBrush(RGB(247, 225, 50));
	uiwc.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClass(&uiwc);

	/// 부모창의 크기를 구하는 코드 자식 창의 크기를 부모창에 맞추기 위함
	RECT rect;
	GetClientRect(pHwnd, &rect);

	/// rect.right = 부모창의 너비 , rect.bottom = 부모창의 높이

	HWND hWnd = CreateWindowEx(WS_EX_DLGMODALFRAME | WS_EX_TOPMOST,
		uiwc.lpszClassName, L"UiWindow", WS_CHILD | WS_VISIBLE,
		rect.left, rect.top,
		rect.right, rect.top + 70,
		pHwnd, NULL, hInst, nullptr);
	//CreateWindowEx()
	//CreateWindowExW();

	if (!hWnd) return false;

	return true;
}

INIT_UI ui1;

LRESULT CALLBACK UiProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_CREATE:
	{
		HINSTANCE hInst = GetModuleHandle(NULL);

		ui1.InitUI(hWnd, hInst);

		SetTimer(hWnd, 1, 100, NULL);
	}
	break;

	case WM_TIMER:
	{
		InvalidateRect(hWnd, NULL, TRUE);
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT cPs;
		HDC hdc = BeginPaint(hWnd, &cPs);

		HBRUSH hb, ob;
		hb = CreateSolidBrush(RGB(247, 225, 50));
		ob = (HBRUSH)SelectObject(hdc, hb);
		Rectangle(hdc, 0, 0, 0, 70);
		SelectObject(hdc, ob);
		DeleteObject(hb);
		EndPaint(hWnd, &cPs);
	}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}