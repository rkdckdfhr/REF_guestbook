#include "InitUI.h"








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
  HWND Button = CreateWindowW(L"STATIC", nullptr, WS_CHILD | WS_VISIBLE | SS_ICON | SS_NOTIFY ,
    15, 10, 50, 50,
    hWnd, (HMENU)3001, hInst, nullptr);

  // 아이콘 파일명만 주면 바로 그 파일안에서 어려 정보를 불러오는 함수이다 ex 아이콘,비트맵그림,마우스커서그림 
  HICON hicon = (HICON)LoadImageW(nullptr, L"images/spray.ico", IMAGE_ICON, 50, 50, LR_LOADFROMFILE | LR_SHARED);

  SendMessage(Button, STM_SETIMAGE, IMAGE_ICON, (LPARAM)hicon); // hion은 핸들러라 정수타입으로 형변환 

  HWND Button_color = CreateWindowW(L"STATIC", L"선택", WS_CHILD | WS_VISIBLE | SS_ICON | SS_NOTIFY,
    75, 10, 50, 50,
    hWnd, (HMENU)3002, hInst, nullptr);

  HWND Button_eraser = CreateWindowW(L"STATIC", L"선택", WS_CHILD | WS_VISIBLE | SS_ICON | SS_NOTIFY,
      135, 10, 50, 50,
      hWnd, (HMENU)3003, hInst, nullptr);

  HICON hicon_eraser = (HICON)LoadImageW(nullptr, L"images/ERASER.ico", IMAGE_ICON, 50, 50, LR_LOADFROMFILE | LR_SHARED);
  SendMessage(Button_eraser, STM_SETIMAGE, IMAGE_ICON, (LPARAM)hicon_eraser);
}
