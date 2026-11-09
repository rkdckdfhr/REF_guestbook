#include "Ui_Sub.h"
#include "File_io.h"
#include "UtilFunc.h"

namespace
{
  HWND g_parent_wnd = NULL;
  HWND g_saver_wnd = NULL;            // 지금 떠있는 화면보호기 창 (중복 생성 방지용)
  std::vector<Line> g_play_lines;     // 재생할 그림 (그림 창의 lines와 별개)
  int    g_file_index = 0;            // 다음 파일 번호
  size_t g_shown_count = 0;           // 지금까지 그린 선 개수
  int    g_wait_tick = 0;             // 다 그린 뒤 대기
  POINT  g_first_mouse = { -1, -1 };  // 마우스 기준점

  // 화면보호기 윈도우 프로시저
  LRESULT CALLBACK saver_proc(HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param)
  {
    switch (msg)
    {
    case WM_CREATE:
      ShowCursor(FALSE);            // 마우스 커서 숨김
      SetTimer(h_wnd, 1, 16, NULL); // 60fps 타이머 설정
      return 0;

    case WM_TIMER:
      if (g_shown_count < g_play_lines.size())
      {
        if (g_play_lines.size() < g_shown_count)
        {
          g_shown_count = 0;
        }
        g_shown_count += 2;       // 한 번에 4개씩 그림

      }
      else if (++g_wait_tick > 60)   // 다 그린 뒤 약 1초 대기
      {
        g_wait_tick = 0;
        g_shown_count = 0;
        // 다음 파일 로드 (없으면 창 닫기)
        if (!File_LoadNext(g_file_index, g_play_lines) || g_play_lines.empty())
        {
          DestroyWindow(h_wnd);
          return 0;
        }
      }
      InvalidateRect(h_wnd, NULL, FALSE); // 화면 갱신
      return 0;

    case WM_PAINT:
    {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(h_wnd, &ps);
      RECT rc;
      GetClientRect(h_wnd, &rc);

      // 깜빡임 방지: 메모리 DC에 먼저 그린 뒤 한 번에 복사 (더블 버퍼링)
      HDC mem_dc = CreateCompatibleDC(hdc);
      HBITMAP mem_bmp = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
      HBITMAP old_bmp = (HBITMAP)SelectObject(mem_dc, mem_bmp);
      FillRect(mem_dc, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));

      // 현재 개수(g_shown_count)만큼만 선 그리기
      DrawLineRange(mem_dc, g_play_lines, 0, min(g_shown_count, g_play_lines.size()));

      BitBlt(hdc, 0, 0, rc.right, rc.bottom, mem_dc, 0, 0, SRCCOPY);
      SelectObject(mem_dc, old_bmp);
      DeleteObject(mem_bmp);
      DeleteDC(mem_dc);
      EndPaint(h_wnd, &ps);
      return 0;
    }

    case WM_ERASEBKGND:
      return 1; // 배경 지우기 생략 (깜빡임 방지)

    case WM_MOUSEMOVE:
    {
      // 마우스 5픽셀 이상 이동 시 종료
      POINT p = { (short)LOWORD(l_param), (short)HIWORD(l_param) };
      if (g_first_mouse.x == -1) { g_first_mouse = p; return 0; }
      if (abs(p.x - g_first_mouse.x) > 5 || abs(p.y - g_first_mouse.y) > 5)
        DestroyWindow(h_wnd);
      return 0;
    }

    case WM_LBUTTONDOWN:
    case WM_KEYDOWN:
      DestroyWindow(h_wnd); // 클릭 또는 키 입력 시 종료
      return 0;

    case WM_DESTROY:
      KillTimer(h_wnd, 1);
      ShowCursor(TRUE);                        // 마우스 커서 복원
      g_saver_wnd = NULL;                      // 창 닫힐 때 가드 변수 비워줌
      SetTimer(g_parent_wnd, 1, 7000, NULL);   // 메인 창 타이머(7초) 재예약
      return 0;
    }
    return DefWindowProc(h_wnd, msg, w_param, l_param);
  }
}

// 화면보호기 실행 함수
void Ui_Sub::open(HWND parent_wnd)
{
  // 이미 화면보호기 창이 떠있으면 중복 생성하지 않음 (속도 누적 버그 방지)
  if (g_saver_wnd != NULL)
  {
    return;
  }

  g_parent_wnd = parent_wnd;

  // 보여줄 파일이 없으면 창을 안 띄우고 7초 후 재시도
  if (!File_LoadNext(g_file_index, g_play_lines) || g_play_lines.empty())
  {
    SetTimer(parent_wnd, 1, 7000, NULL);
    return;
  }

  // 윈도우 클래스 등록 (최초 1회)
  static bool is_registered = false;
  HINSTANCE h_inst = GetModuleHandle(NULL);
  if (!is_registered)
  {
    WNDCLASS wc = {};
    wc.lpfnWndProc = saver_proc;
    wc.hInstance = h_inst;
    wc.lpszClassName = L"SaverWindowClass";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);
    is_registered = true;
  }

  // 카운터 초기화
  g_shown_count = 0;
  g_wait_tick = 0;
  g_first_mouse = { -1, -1 };

  // 전체 화면 창 생성
  g_saver_wnd = CreateWindowEx(WS_EX_TOPMOST, L"SaverWindowClass", L"",
    WS_POPUP | WS_VISIBLE,
    0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
    NULL, NULL, h_inst, NULL);
  SetFocus(g_saver_wnd);
}