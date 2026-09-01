#include "InitUI.h"
//#include <commctrl.h>
//#pragma comment(lib, "comctl32.lib")
#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")










LRESULT INIT_UI::InitColor(WPARAM wParam, LPARAM lParam)
{

  //HDC 그래픽 자원에 주소 *
  HDC hdcStatic = (HDC)wParam;
  // HWND 창 핸들러에 주소 *
  HWND hChildWnd = (HWND)lParam;

  if (hChildWnd == hToolBar)
  {
    hBrush = CreateSolidBrush(RGB(10, 200, 200));
    //  글자 배경을 투명/흰색으로 복구 할 수 있다
    SetBkMode(hdcStatic, TRANSPARENT);

    return (LRESULT)hBrush;
  }
  return 0;
}
INIT_UI::INIT_UI() {};

INIT_UI::~INIT_UI ()
{
  if(hBrush != nullptr)
  {
    DeleteObject(hBrush);
		hBrush = nullptr;
  }
}



/**
* @brief 프로그램 시작 시 메인 UI 화면을 초기화하고 구성합니다.
*
* @param hWnd UI 요소들이 부착될 메인 부모 창의 리모컨(핸들)
* @param hInst 현재 실행 중인 인스턴스 핸들
*/
// InitUI 함수 작성구간
void INIT_UI::InitUI(HWND hWnd, HINSTANCE hInst)
{
  HMENU hMenuBar = CreateMenu();


  HMENU hFileMenu = CreatePopupMenu();


  AppendMenu(hFileMenu, MF_STRING, 1001, L"저장하기");
  AppendMenu(hFileMenu, MF_STRING, 1002, L"불러오기");
  AppendMenu(hFileMenu, MF_STRING, 1005, L"상욱이");
  AppendMenu(hFileMenu, MF_SEPARATOR, 0, nullptr);
  AppendMenu(hFileMenu, MF_STRING, 1003, L"끝내기");

  AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hFileMenu, L"파일");

  // 세팅 메뉴를 위한 코드들
  HMENU hSetting = CreatePopupMenu();

  AppendMenu(hSetting, MF_STRING, 2001, L"1");
  AppendMenu(hSetting, MF_STRING, 2002, L"2");
  AppendMenu(hSetting, MF_STRING, 2003, L"3");

  AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hSetting, L"세팅");

  SetMenu(hWnd, hMenuBar); // 바를 화면인데에 붙히는 함수


  hToolBar = CreateWindowW(L"STATIC", L"",
    WS_CHILD | WS_VISIBLE,
    0, 0,
    2000, 70,
    hWnd, nullptr, hInst, nullptr);

  // 첫번째 버튼!
  HWND Button_Pen = CreateWindowW(L"STATIC", nullptr, WS_CHILD | WS_VISIBLE | SS_ICON | SS_NOTIFY,
    35, 10, 50, 50,
    hWnd, (HMENU)3000, hInst, nullptr);
  HICON hicon_Pen = (HICON)LoadImageW(nullptr, L"images/pen2.ico", IMAGE_ICON, 50, 50, LR_LOADFROMFILE | LR_SHARED);
  SendMessage(Button_Pen, STM_SETIMAGE, IMAGE_ICON, (LPARAM)hicon_Pen);


  HWND Button = CreateWindowW(L"STATIC", nullptr, WS_CHILD | WS_VISIBLE | SS_ICON | SS_NOTIFY ,
    95, 10, 50, 50,
    hWnd, (HMENU)3001, hInst, nullptr);

  // 아이콘 파일명만 주면 바로 그 파일안에서 어려 정보를 불러오는 함수이다 ex 아이콘,비트맵그림,마우스커서그림 
  HICON hicon = (HICON)LoadImageW(nullptr, L"images/spray.ico", IMAGE_ICON, 50, 50, LR_LOADFROMFILE | LR_SHARED);

  SendMessage(Button, STM_SETIMAGE, IMAGE_ICON, (LPARAM)hicon); // hion은 핸들러라 정수타입으로 형변환 

  HWND Button_Eraser = CreateWindowW(L"STATIC", L"선택", WS_CHILD | WS_VISIBLE | SS_ICON | SS_NOTIFY,
      155, 10, 50, 50,
      hWnd, (HMENU)3003, hInst, nullptr);

  HICON hicon_Eraser = (HICON)LoadImageW(nullptr, L"images/ERASER.ico", IMAGE_ICON, 50, 50, LR_LOADFROMFILE | LR_SHARED);
  SendMessage(Button_Eraser, STM_SETIMAGE, IMAGE_ICON, (LPARAM)hicon_Eraser);


  HWND Button_Color = CreateWindowW(L"STATIC", L"선택", WS_CHILD | WS_VISIBLE | SS_ICON | SS_NOTIFY,
    335, 10, 50, 50,
    hWnd, (HMENU)3002, hInst, nullptr);

  HICON hicon_Color = (HICON)LoadImageW(nullptr, L"images/color.ico", IMAGE_ICON, 50, 50, LR_LOADFROMFILE | LR_SHARED);
  SendMessage(Button_Color, STM_SETIMAGE, IMAGE_ICON, (LPARAM)hicon_Color);


  // 저장 버튼
  HWND Button_Save= CreateWindowW(L"Button", nullptr, WS_CHILD | WS_VISIBLE | BS_ICON | BS_PUSHBUTTON,
    1350, 10, 50, 50,
    hWnd, (HMENU)3004, hInst, nullptr);

  HICON hicon_Save = (HICON)LoadImageW(nullptr, L"images/save.ico", IMAGE_ICON, 50, 50, LR_LOADFROMFILE | LR_SHARED);
  SendMessage(Button_Save, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hicon_Save);

  //리플레이 시작버튼
  HWND Button_Play = CreateWindowW(L"Button", nullptr, WS_CHILD | WS_VISIBLE | BS_ICON | BS_PUSHBUTTON,
    1200, 10, 50, 50,
    hWnd, (HMENU)3004, hInst, nullptr);

	HICON hicon_Play = (HICON)LoadImageW(nullptr, L"images/play1.ico", IMAGE_ICON, 50, 50, LR_LOADFROMFILE | LR_SHARED);
  SendMessage(Button_Play, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hicon_Play);

  //리플레이 중지 버튼
  HWND Button_Stop = CreateWindowW(L"Button", nullptr, WS_CHILD | WS_VISIBLE | BS_ICON | BS_PUSHBUTTON,
    1260, 10, 50, 50,
    hWnd, (HMENU)3004, hInst, nullptr);

  HICON hicon_Stop = (HICON)LoadImageW(nullptr, L"images/stop.ico", IMAGE_ICON, 50, 50, LR_LOADFROMFILE | LR_SHARED);
  SendMessage(Button_Stop, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hicon_Stop);

	/// @brief 굵기 조절 바 생성 버튼 입니다.
  hPenBar = CreateWindowW(TRACKBAR_CLASSW, L"",
    WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_VERT, // 세로형 스타일
    10, 150, 40, 300, 
    hWnd, 
    (HMENU)5000, hInst, nullptr);

  SendMessage(hPenBar, TBM_SETRANGE, TRUE, MAKELPARAM(1, 30)); // 스크롤에 최소값과 최대값을 지정해줌 TBM_SETRANGE -> 범위
  SendMessage(hPenBar, TBM_SETPOS, TRUE, 5); // 창이 켜졌을때 5값에 버튼이 위치하게 세팅 TBM_SETPOS -> 위치
}
