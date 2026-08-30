#include <windows.h>
#include <vector>
#include "DRW_Window.h"
#include "Replay.h"

//좌표 위치 저장된걸 가져오는 역할
extern std::vector<Line> lines;

//리플레이 중 인지 확인하는 변수
bool isReplaying = false;
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

    if (!lines.empty())
    {
        for (unsigned int i = 0; i < lines.size(); i++)
        {
            tmp_Replay = i+1;
            
            InvalidateRect(hWnd, NULL, FALSE);

            Sleep(10);
        }
    }

    isReplaying = false;

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