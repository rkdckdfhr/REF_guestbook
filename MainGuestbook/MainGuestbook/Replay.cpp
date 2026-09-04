#include <windows.h>
#include <vector>
#include "DRW_Window.h"
#include "Replay.h"


//리플레이 중 인지 확인하는 변수
bool isReplaying = false;
//리플레이 일시 정지 상태인지 확인 변수 
bool isWaiting = false;
//반복문에서 PAINT에 넘길 때 몇 번째 선인지 기억해주는 변수
unsigned tmp_Replay = 0;

DWORD WINAPI ReplayThreads(LPVOID lpParam)
{
    //스레드 행동 함수 작성

    isReplaying = true;

    HWND hWnd = (HWND)lpParam;

    tmp_Replay = 0;
    //그리는 면 지우고 바로 실행 시킬 함수들
    InvalidateRect(hWnd, NULL, TRUE);

        for (unsigned int i = 0; i < DrwWindow::lines.size(); i++)
        {
            while (isWaiting)
            {
                    //만약 Sleep을 안 주면 cpu 사용량이 너무 많아짐 필수 사용
                    Sleep(5);
            }
           
            tmp_Replay = i + 1;
 
            InvalidateRect(hWnd, NULL, FALSE);
            UpdateWindow(hWnd);

            Sleep(10);
        }

    isReplaying = false;
    isWaiting = false;
    tmp_Replay = 0;

    

    return 0;
}

void ThreadTrigger(HWND hWnd)
{
    if (isReplaying == true) return;

    HANDLE NewThread = CreateThread(NULL, 0, ReplayThreads, (LPVOID)hWnd, 0, NULL);

    if (NewThread != NULL)
    {
        //스레드 할 일이 끝나면 자동 종료
        CloseHandle(NewThread);
    }
}

