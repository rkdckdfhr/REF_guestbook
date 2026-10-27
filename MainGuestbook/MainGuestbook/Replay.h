#pragma once
#include <windows.h>

extern bool isReplaying;

extern bool isWaiting;

extern unsigned tmp_Replay;

void ThreadTrigger(HWND hWnd);

const UINT WM_REPLAY_DONE = WM_APP + 1;