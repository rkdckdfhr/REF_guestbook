#pragma once
#include <windows.h>
#include "InitUI.h"

// UI 윈도우 클래스 등록 및 생성 함수
void RegisterUiWindowClass(HINSTANCE hInstance);
HWND CreateUiWindow(HWND hParent, HINSTANCE hInstance, int x, int y, int width, int height);

// UI 윈도우 프로시저
LRESULT CALLBACK UiProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);