#pragma once
#include <windows.h>

extern bool isReplaying;

extern bool isWaiting;

extern bool restart_replay;

extern bool exit_replay;

extern unsigned tmp_Replay;



void ThreadTrigger(HWND hWnd);

const UINT WM_REPLAY_DONE = WM_APP + 1;