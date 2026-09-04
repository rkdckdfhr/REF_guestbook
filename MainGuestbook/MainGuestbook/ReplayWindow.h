#pragma once

#include "MainGuestbook.h"


class ReplayWindow
{
public:
	ReplayWindow();
	~ReplayWindow();

	static LRESULT CALLBACK ReplayProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	bool NewReplayWnd(HINSTANCE hInst, HWND dHwnd);

private:

};

