#pragma once

#include <windows.h>
#include <stdio.h>
#include <vector>

bool UiWnd(HINSTANCE hInst, HWND pHwnd);

static LRESULT CALLBACK UiProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);