#include "UiWindow.h"
#include "Pen_tool.h"

extern INIT_UI ui; // DRW_Window.cpp에 전역 선언된 ui 객체 사용
extern Pen_tool myPen;

void RegisterUiWindowClass(HINSTANCE hInstance) {
    WNDCLASS wc = { 0 };
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = UiProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(RGB(240, 240, 240)); // 팔레트 톤 배경
    wc.lpszClassName = TEXT("UiWindowClass");
    RegisterClass(&wc);
}

HWND CreateUiWindow(HWND hParent, HINSTANCE hInstance, int x, int y, int width, int height) {
    return CreateWindowEx(0, TEXT("UiWindowClass"), NULL,
        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
        x, y, width, height, hParent, (HMENU)2000, hInstance, NULL);
}

LRESULT CALLBACK UiProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        // 이 윈도우(UI 영역)에 버튼들을 붙임
        ui.InitUI(hWnd, ((LPCREATESTRUCT)lParam)->hInstance);
        break;

    case WM_COMMAND:
    case WM_VSCROLL:
        // 발생한 이벤트를 부모(MainGuestbook)로 올려보내 도화지까지 전달되게 함
        SendMessage(GetParent(hWnd), message, wParam, lParam);
        break;

    case WM_CTLCOLORSTATIC:
    {
        HDC hdcStatic = (HDC)wParam;
        HWND hwndStatic = (HWND)lParam;
        LRESULT hBrush = ui.InitColor(wParam, lParam);
        if (hBrush != 0)
        {
            return hBrush;
        }


        if (hwndStatic == ui.Button_ShowColor)
        {

            if (ui.g_current_color != NULL) {
                DeleteObject(ui.g_current_color);
            }


            ui.g_current_color = CreateSolidBrush(myPen.GetColor());


            return (INT_PTR)ui.g_current_color;
        }


    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}