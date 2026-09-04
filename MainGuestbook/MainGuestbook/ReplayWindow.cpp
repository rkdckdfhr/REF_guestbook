#include "ReplayWindow.h"


bool ReplayWindow::NewReplayWnd(HINSTANCE hInst, HWND dHwnd)
{
	WNDCLASS rewc = {};

	rewc.lpfnWndProc = ReplayProc;
	rewc.hInstance = hInst;
	rewc.lpszClassName = L"ReplayWindowClass";
	rewc.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
	rewc.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClass(&rewc);

	RECT dRect;
	GetClientRect(dHwnd, &dRect);

	HWND hWnd = CreateWindowEx(0,
		rewc.lpszClassName, L"ReplayWindow", WS_CHILD | WS_VISIBLE,
		0, 70,
		dRect.right, dRect.bottom,
		dHwnd, NULL, hInst, this);

	if (!hWnd)
	{
		MessageBox(dHwnd, L"생성 실패", L"실패", MB_OK);
		return false;
	}

	return true;
}


DrwWindow Draw;

LRESULT CALLBACK ReplayWindow::ReplayProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		ThreadTrigger(hWnd);
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

				HPEN current_pen = CreatePenIndirect(&Draw.lines[i].current_pen);
				HPEN OldPen = (HPEN)SelectObject(hdc, current_pen);
				MoveToEx(hdc, Draw.lines[i].start.x, Draw.lines[i].start.y, NULL);
				LineTo(hdc, Draw.lines[i].end.x, Draw.lines[i].end.y);
				SelectObject(hdc, OldPen);
				DeleteObject(current_pen);
			}
		}
		//if(!isReplaying) DestroyWindow(hWnd);
		
		EndPaint(hWnd, &cPs);
	}
		break;
	
	case WM_RBUTTONDOWN:
	{
		/// 우클릭 시 리플레이 되는 창에서 빠져나오고 다시 그리기 창으로 복귀
		/// 를 원하는 중 일단은 됨 ( 09/04 )
		DestroyWindow(hWnd);
	}
	break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}




ReplayWindow::ReplayWindow()
{
}

ReplayWindow::~ReplayWindow()
{
}