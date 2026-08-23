#include "InitUI.h"

/**
 * @brief 프로그램 시작 시 메인 UI 화면을 초기화하고 구성합니다.
 *
 * @param hWnd UI 요소들이 부착될 메인 부모 창의 리모컨(핸들)
 * @param hInst 현재 실행 중인 인스턴스 핸들
 */
void InitUI(HWND hWnd, HINSTANCE  hInst)
{
  HMENU hMenuBar = CreateMenu();


  HMENU hFileMenu = CreatePopupMenu();


  AppendMenu(hFileMenu, MF_STRING, 1001, L"저장하기"); // 1001 사건번호 
  AppendMenu(hFileMenu, MF_STRING, 1002, L"불러오기");
  AppendMenu(hFileMenu, MF_STRING, 1005, L"상욱이");
  AppendMenu(hFileMenu, MF_SEPARATOR, 0, nullptr); // 가로 구분선 긋기
  AppendMenu(hFileMenu, MF_STRING, 1003, L"끝내기");

  // 다 채운 [파일] 드롭다운 메뉴를 맨 위 큰 메뉴바에 이름표 달아서 붙이기
  AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hFileMenu, L"파일");




  // 세팅 메뉴를 위한 코드들
  HMENU hSetting = CreatePopupMenu();
  AppendMenu(hSetting, MF_STRING, 2001, L"1");
  AppendMenu(hSetting, MF_STRING, 2002, L"2");
  AppendMenu(hSetting, MF_STRING, 2003, L"3");


  AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hSetting, L"세팅");


  SetMenu(hWnd, hMenuBar); // 바를 화면인데에 붙히는 함수


  HWND hToolBar = CreateWindowW(L"STATIC", L"",
    WS_CHILD | WS_VISIBLE, // 테두리가 있는 기본 스타일
    0, 0,
    2000, 50,
    hWnd, nullptr, hInst, nullptr);

  // 부모 창(8번째 인자)을 hWnd가 아니라 hToolBar로 지정하면 바 위에 버튼이 찰싹 붙습니다.
  CreateWindowW(L"BUTTON", L"선택", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
    15, 10, 60, 30, // 바(hToolBar) 안에서의 위치
    hWnd, (HMENU)3001, hInst, nullptr);


}
