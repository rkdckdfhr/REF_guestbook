#include "InitUi.h"
#include "resource.h"

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
    hBrush = CreateSolidBrush(RGB(240, 240, 240));
    //  글자 배경을 투명/흰색으로 복구 할 수 있다
    SetBkMode(hdcStatic, TRANSPARENT);

    return (LRESULT)hBrush;
  }
  return 0;
}
INIT_UI::INIT_UI() {};

INIT_UI::~INIT_UI()
{
  if (hBrush != nullptr)
  {
    DeleteObject(hBrush);
    hBrush = nullptr;
  }

  if (g_current_color != NULL) {
    DeleteObject(g_current_color);
  }
}
///@brief UI 요소들을 숨기거나 표시하는 함수입니다.
void INIT_UI::ToggleUI(bool show) {


  if (show)
  {
    show = SW_SHOW; // 키는 옵션
  }
  else
  {
    show = SW_HIDE; // 끄는 옵션
  }

  ShowWindow(hToolBar, show);

  ShowWindow(hPenBar, show);

  ShowWindow(Button_Pen, show);

  ShowWindow(Button, show);

  ShowWindow(Button_Eraser, show);

  ShowWindow(Button_Color, show);

  ShowWindow(Button_ShowColor, show);

  ShowWindow(Button_Save, show);

  ShowWindow(Button_Play, show);

  ShowWindow(Button_Stop, show);
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
  InitCommonControls();
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
    2000, 90,
    hWnd, nullptr, hInst, nullptr);


  // 툴바와 도화지를 구분하는 실선
  HWND hSeparator = CreateWindowW(L"STATIC", L"",
    WS_CHILD | WS_VISIBLE | SS_BLACKRECT,
    0, 84,        // y=89 (툴바 높이 90 바로 아래)
    2000, 90,      // 너비는 툴바와 같게, 두께는 2px
    hWnd, nullptr, hInst, nullptr);

  // 불러오기 버튼
  Button_Load = CreateWindowW(L"STATIC", nullptr, WS_CHILD | WS_VISIBLE | SS_ICON | SS_NOTIFY,
    1110, 15, 50, 55,
    hWnd, (HMENU)3000, hInst, nullptr);
  HICON hicon_Load = ((HICON)LoadImageW(hInst, MAKEINTRESOURCEW(IDI_LOAD), IMAGE_ICON, 50, 50, 0));
  SendMessage(Button_Load, STM_SETIMAGE, IMAGE_ICON, (LPARAM)hicon_Load);


  Button = CreateWindowW(L"STATIC", nullptr, WS_CHILD | WS_VISIBLE | SS_ICON | SS_NOTIFY,
    95, 15, 50, 55,
    hWnd, (HMENU)3001, hInst, nullptr);

  // 아이콘 파일명만 주면 바로 그 파일안에서 어려 정보를 불러오는 함수이다 ex 아이콘,비트맵그림,마우스커서그림 
  HICON hicon = (HICON)LoadImageW(hInst, MAKEINTRESOURCEW(IDI_SPRAY), IMAGE_ICON, 50, 50, 0);

  SendMessage(Button, STM_SETIMAGE, IMAGE_ICON, (LPARAM)hicon); // hion은 핸들러라 정수타입으로 형변환 

  Button_Eraser = CreateWindowW(L"STATIC", L"선택", WS_CHILD | WS_VISIBLE | SS_ICON | SS_NOTIFY,
    155, 15, 50, 55,
    hWnd, (HMENU)3003, hInst, nullptr);

  HICON hicon_Eraser = (HICON)LoadImageW(hInst, MAKEINTRESOURCEW(IDI_ERASER), IMAGE_ICON, 50, 50, 0);
  SendMessage(Button_Eraser, STM_SETIMAGE, IMAGE_ICON, (LPARAM)hicon_Eraser);

  Button_Tool1 = CreateWindowW(L"STATIC", nullptr, WS_CHILD | WS_VISIBLE | SS_ICON | SS_NOTIFY,
    215, 15, 50, 55,
    hWnd, (HMENU)3009, hInst, nullptr);

  HICON Button_Tool1_icon = (HICON)LoadImageW(hInst, MAKEINTRESOURCEW(IDI_PEN), IMAGE_ICON, 50, 50, 0);
  SendMessage(Button_Tool1, STM_SETIMAGE, IMAGE_ICON, (LPARAM)Button_Tool1_icon);

  Button_Tool2 = CreateWindowW(L"STATIC", nullptr, WS_CHILD | WS_VISIBLE | SS_ICON | SS_NOTIFY,
    275, 15, 50, 55,
    hWnd, (HMENU)3010, hInst, nullptr);

  HICON Button_Tool2_icon = (HICON)LoadImageW(hInst, MAKEINTRESOURCEW(IDI_PENTOOL), IMAGE_ICON, 50, 50, 0);
  SendMessage(Button_Tool2, STM_SETIMAGE, IMAGE_ICON, (LPARAM)Button_Tool2_icon);



  // 컬러버튼
  Button_Color = CreateWindowW(L"STATIC", L"선택", WS_CHILD | WS_VISIBLE | SS_ICON | SS_NOTIFY,
    495, 15, 50, 55,
    hWnd, (HMENU)3002, hInst, nullptr);


  HICON hicon_Color = (HICON)LoadImageW(hInst, MAKEINTRESOURCEW(IDI_COLOR), IMAGE_ICON, 50, 50, 0);
  SendMessage(Button_Color, STM_SETIMAGE, IMAGE_ICON, (LPARAM)hicon_Color);

  //이버튼은 현재 내 붓 색깔을 보여주는 버튼이다
  Button_ShowColor = CreateWindowW(L"STATIC", L"선택", WS_CHILD | WS_VISIBLE | SS_ICON | SS_NOTIFY,
    555, 15, 50, 50,
    hWnd, (HMENU)3007, hInst, nullptr);

  // 저장 버튼
  Button_Save = CreateWindowW(L"STATIC", nullptr, WS_CHILD | WS_VISIBLE | SS_ICON | SS_NOTIFY,
    1050, 15, 50, 55,
    hWnd, (HMENU)3004, hInst, nullptr);

  HICON hicon_Save = (HICON)LoadImageW(hInst, MAKEINTRESOURCEW(IDI_SAVE), IMAGE_ICON, 50, 50, 0);
  SendMessage(Button_Save, STM_SETIMAGE, IMAGE_ICON, (LPARAM)hicon_Save);

  //새로 만들기 버튼
  button_new = CreateWindowW(L"STATIC", nullptr, WS_CHILD | WS_VISIBLE | SS_ICON | SS_NOTIFY,
    35, 15, 50, 55,
    hWnd, (HMENU)3008, hInst, nullptr);

  HICON hicon_new = (HICON)LoadImageW(hInst, MAKEINTRESOURCEW(IDI_FILE), IMAGE_ICON, 50, 50, 0);
  SendMessage(button_new, STM_SETIMAGE, IMAGE_ICON, (LPARAM)hicon_new);

  //리플레이 시작버튼 
  Button_Play = CreateWindowW(L"STATIC", nullptr, WS_CHILD | WS_VISIBLE | SS_ICON | SS_NOTIFY,
    750, 15, 50, 55,
    hWnd, (HMENU)3005, hInst, nullptr);

  HICON hicon_Play = (HICON)LoadImageW(hInst, MAKEINTRESOURCEW(IDI_PLAY), IMAGE_ICON, 50, 50, 0);
  SendMessage(Button_Play, STM_SETIMAGE, IMAGE_ICON, (LPARAM)hicon_Play);

  //리플레이 중지 버튼
  Button_Stop = CreateWindowW(L"STATIC", nullptr, WS_CHILD | WS_VISIBLE | SS_ICON | SS_NOTIFY,
    810, 15, 50, 55,
    hWnd, (HMENU)3006, hInst, nullptr);

  HICON hicon_Stop = (HICON)LoadImageW(hInst, MAKEINTRESOURCEW(IDI_STOP), IMAGE_ICON, 50, 50, 0);
  SendMessage(Button_Stop, STM_SETIMAGE, IMAGE_ICON, (LPARAM)hicon_Stop);


  HWND hDrawWnd = FindWindowEx(GetParent(hWnd), NULL, L"MainWindowClass", NULL);

  /// @brief 굵기 조절 바 생성 버튼 입니다.
  hPenBar = CreateWindowW(TRACKBAR_CLASSW, L"",
    WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_VERT, // 세로형 스타일
    10, 140, 40, 320,
    hDrawWnd,
    (HMENU)5000, hInst, nullptr);

  SendMessage(hPenBar, TBM_SETRANGE, TRUE, MAKELPARAM(1, 30)); // 스크롤에 최소값과 최대값을 지정해줌 TBM_SETRANGE -> 범위
  SendMessage(hPenBar, TBM_SETPOS, TRUE, 26); // 창이 켜졌을때 5값에 버튼이 위치하게 세팅 TBM_SETPOS -> 위치
}