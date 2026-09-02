
#pragma once
#include <Windows.h>


class INIT_UI
{
public:
  HWND hToolBar = nullptr;
  HBRUSH hBrush = nullptr;

	HWND hScrollText = nullptr; ///< @brief 스크롤 텍스트 박스 핸들
  /// <summary>
  /// 굵기 바 핸들
  /// </summary>
  HWND hPenBar = nullptr;

  INIT_UI();
  ~INIT_UI();

  // 함수 선언(이름표)만 남깁니다.
  void InitUI(HWND hWnd, HINSTANCE hInst);
  LRESULT InitColor(WPARAM wParam, LPARAM lParam);
};