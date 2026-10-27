#include "DRW_Window.h"
#include "framework.h"
#include "Replay.h"
#include "Pen_tool.h"
#include "Resource.h"

#include "InitUI.h"
#include "File_io.h"
#include "UtilFunc.h"
#include "buffer.h"	
#include "Screen.h"	

std::vector<Line> DrwWindow::lines;
POINT DrwWindow::draw_start;
POINT DrwWindow::draw_end;


/*
안될땐 꼭 브레이크 걸고 디버그 실행해서 한줄씩 찾기
*/


bool DrwWindow::NewWnd(HINSTANCE hInst, HWND pHwnd)
{


	WNDCLASS wc = {};
	wc.lpfnWndProc = DrawWndProc;
	wc.hInstance = hInst;
	wc.lpszClassName = L"MainWindowClass";
	wc.hbrBackground = CreateSolidBrush(RGB(230, 230, 230));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClass(&wc);

	/// 부모창의 크기를 구하는 코드 자식 창의 크기를 부모창에 맞추기 위함
	RECT rect;
	GetClientRect(pHwnd, &rect);

	/// rect.right = 부모창의 너비 , rect.bottom = 부모창의 높이

	HWND hWnd = CreateWindowEx(0,
		wc.lpszClassName, L"TestWindow", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0, 70,
		rect.right, rect.bottom,
		pHwnd, NULL, hInst, this);


	// 자식 윈도우 생성이 되는지 유효성 검사
	//if (!hWnd)
	//{
	//	
	//	DWORD err = GetLastError();
	//	wchar_t buf[256];
	//	swprintf_s(buf, L"자식 윈도우 생성 실패, 에러 코드: %lu", err);
	//	MessageBoxW(NULL, buf, L"Error", MB_OK);

	//	return false;
	//}
	//

	//ShowWindow(hWnd, NULL);
	
	return true;
}


bool g_isSaved = false;   // 저장된 그림인지
bool g_isManualReplay = false; ///수동 재생 모드인지 확인하는 전역 변수   
bool g_isScreenSaverOn = false; ///화면 보호기 모드인지 확인하는 전역 변수 
ScreenSaver saver; ///화면 보호기 모드 관련 클래스	

DrwWindow DW;
INIT_UI ui;
Pen_tool myPen; //기본 세팅된 펜
/* 펜 스타일 옵션 줄때 괄호안에 스타일,두께,색상주기
PS_DASH 파선
PS_DASHDOT 점선
PS_DASHDOTDOT 점선
*/

ReplayBuffer replay_buffer;

LRESULT CALLBACK DrwWindow::DrawWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	/// @TODO: 공부해서 채워넣자
	/// 반드시 DefWindowProc로 운영체제에 남은 자원 반환
	/// 이곳에 그리기 기능 넣기
	switch (message)
	{
	case WM_ERASEBKGND:
	{
		// 화면 또 지울 필요 없다는 return 1
		// 깜빡임 문제 해결을 위해 필요
		return 1;
	}
	case WM_REPLAY_DONE:
	{
		// 보호기가 끝까지 돌았고(마우스로 끄지 않았고), 저장된 그림이면 3초 뒤 다시 예약
		if (g_isScreenSaverOn == true && g_isSaved == true)
		{
			SetTimer(hWnd, 1, 5000, NULL);
		}
	}
	break;

	case WM_SIZE:
	{
		RECT rect;

		// 그릴 수 있는 영역 크기와 좌표 알려주는 API
		GetClientRect(hWnd, &rect);

		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		if (width > 0 && height > 0)
		{
			HDC hdc = GetDC(hWnd);

			//위에서 받은 형식 받아서 버퍼 생성 & 재구성
			replay_buffer.CreateBuffer(hdc, width, height);
			ReleaseDC(hWnd, hdc);

			InvalidateRect(hWnd, NULL, FALSE);
		}
	}
	break;


	case WM_CREATE:
	{
		/// TODO: 채워넣기

		/// 버튼 생성

		//GetModuleHandle 하면 현재 창나온 인스턴스 핸들을 가져올 수 있음
		HINSTANCE hInst = GetModuleHandle(NULL);
		// 1번 타이머를 5000밀리초(5초) 간격으로 설정합니다.
		SetTimer(hWnd, 1, 5000, NULL);
	}
	break;

	case WM_TIMER:
	{
		if (wParam == 1)
		{
			// 저장된 그림이 아니면 보호기를 쓰지 않고 타이머도 끔
			if (g_isSaved == false)
			{
				KillTimer(hWnd, 1);
				break;
			}

			// 그리는 중이거나 리플레이 중이면 이번엔 건너뜀 
			if (DW.is_drawing || isReplaying) break;

			KillTimer(hWnd, 1);
			saver.StartScreenSaver(hWnd);
		}
	}
	break;


	case WM_COMMAND:
	{
		if (isReplaying) break;

		/// 버튼 클릭시 이벤트 발생 시키는 switch문
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
			/// 버튼 기능 관련 연결부
		case BUTTON_LOAD:
		{
			if (isReplaying) break;   // 리플레이 중에는 lines를 바꾸면 위험해서 막기

			wchar_t path[MAX_PATH] = L"";
			if (ShowFileDialog(hWnd, path, false))
			{
				if (File_Call(path, lines))
				{
					g_isSaved = true;
					KillTimer(hWnd, 1);                 // 저장 후 대기 중이던 보호기 타이머 취소
					InvalidateRect(hWnd, NULL, TRUE);   // 기존 줄 그대로
					saver.StartScreenSaver(hWnd);       // 바로 한 번 리플레이
				}

			}
		}
			break;
		case BUTTON_PEN:
			MessageBox(hWnd, L"준비 중", L"펜 스타일", MB_OK);
			myPen.style = 7;
			break;
		case BUTTON_TOOL1: // 실선입니다
			MessageBox(hWnd, L"실선", L"펜 스타일", MB_OK);
			myPen.style = 0;
			break;
		case BUTTON_TOOL2:
			MessageBox(hWnd, L"점선", L"펜 스타일", MB_OK);
			myPen.style = 1;
			break;
		case BUTTON_COLOR:
			myPen.SelectColor(hWnd);
			InvalidateRect(ui.Button_ShowColor, NULL, TRUE);
			break;
		case BUTTON_ERASER:
			lines.clear();
			InvalidateRect(hWnd, NULL, TRUE);
					break;
		case BUTTON_SAVE:
		{
			if (MessageBox(hWnd, L"저장하시겠습니까?", L"저장 확인", MB_YESNO) == IDYES)
			{
				wchar_t path[MAX_PATH] = L"";

				if (ShowFileDialog(hWnd, path, true))
				{
					if (File_Check(path) == true)
					{
						MessageBox(hWnd, L"이미 같은 이름의 파일이 있습니다!\n 다른 이름으로 저장해주세요", L"저장 실패", MB_OK);
					}
					else if (File_Save(path, lines))
					{
						MessageBox(hWnd, L"저장 완료", L"저장", MB_OK);
						g_isSaved = true;
						SetTimer(hWnd, 1, 5000, NULL); //저장하고 5초 후에 화면 보호기 모드로 전환	

					}
				}
			}
		}
			break;
		case BUTTON_PLAY:
		{
			MessageBox(hWnd, L"아직 준비 중입니다. 재생", L"재생 버튼", MB_OK);

			g_isManualReplay = true;

			// 재생 중에 화면 보호기가 겹치지 않도록 타이머는 잠시 꺼둡니다.
	

			
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			ThreadTrigger(hWnd);
		}
			break;
		case BUTTON_STOP:
			MessageBox(hWnd, L"아직 준비 중입니다. 정지", L"정지 버튼", MB_OK);
			g_isManualReplay = false;
			g_isScreenSaverOn = false;
			break;

		case BUTTON_NEW:
			lines.clear();
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
	}
	break;

	//case WM_CTLCOLORSTATIC:
	//{
	//	HDC hdcStatic = (HDC)wParam;
	//	HWND hwndStatic = (HWND)lParam;
	//	LRESULT hBrush = ui.InitColor(wParam, lParam);
	//	if (hBrush != 0)
	//	{
	//		return hBrush;
	//	}


	//	if (hwndStatic == ui.Button_ShowColor)
	//	{

	//		if (ui.g_current_color != NULL) {
	//			DeleteObject(ui.g_current_color);
	//		}


	//		ui.g_current_color = CreateSolidBrush(myPen.GetColor());


	//		return (INT_PTR)ui.g_current_color;
	//	}

	//}
	//break;

	/// 펜 두께 슬라이더로 조절
	case WM_VSCROLL:
	{
		// 슬라이더 바 위치로 펜 두께 적용
		// 날 위한 주석 myPan.width 는 int타입이라 형변환
		// myPen.width = 31 - 어쩌구 : 보기 편하려고 슬라이더 위 아래를 바꿈
		myPen.width = 31 - (int)SendMessage(ui.hPenBar, TBM_GETPOS, 0, 0);
	}
	break;

	case WM_LBUTTONDOWN:
	{
		
		if (g_isScreenSaverOn == true)
		{
			g_isScreenSaverOn = false;

			if (g_isSaved == true)
			{
				SetTimer(hWnd, 1, 5000, NULL);   // 5초 뒤 보호기 다시 예약
			}

		}

	
		if (isReplaying) break;

		DW.is_drawing = true;
		/// 마우스가 나가도 마우스 메세지를 현재 창으로 보내주는 함수
		SetCapture(hWnd);

		/// UI 부분에 펜 올라갔을 시에 바닥으로 내리 꽂히는현상 수정 
		/// (short)로 형변환을 시켜 큰 값으로 변경되는 오류 수정
		draw_start.x = (short)LOWORD(lParam);
		draw_start.y = (short)HIWORD(lParam);

	}
	break;

	case WM_MOUSEMOVE:
	{

		
		if (g_isScreenSaverOn == true && saver.IsMouseMoved(lParam) == true)
		{
			g_isScreenSaverOn = false;

			if (g_isSaved == true)
			{
				SetTimer(hWnd, 1, 5000, NULL);   // 5초 뒤 보호기 다시 예약
			}

		}

		if (DW.is_drawing)
		{
			HDC hdc = GetDC(hWnd);

			draw_end.x = (short)LOWORD(lParam);
			draw_end.y = (short)HIWORD(lParam);
			ULONGLONG get_time = GetTickCount64();

			HPEN hPen = myPen.Pen();

			/// LOGPEN = 현재 펜 정보를 담을 수 있는 WIN32 API 제공 구조체
			EXTLOGPEN new_pen;
			
			/// 현재 펜의 오브젝트를 넘겨서 저장
			GetObject(hPen, sizeof(EXTLOGPEN), &new_pen);

			HPEN oldPen = (HPEN)SelectObject(hdc, hPen);
			bool is_spray = (myPen.style == PS_USERSTYLE);
			if (is_spray)
			{
				myPen.DrawSpray(hdc, draw_end.x, draw_end.y);
			}	
			else
			{
				MoveToEx(hdc, draw_start.x, draw_start.y, NULL);
				LineTo(hdc, draw_end.x, draw_end.y);
			}
			if (replay_buffer.bf_dc)
			{
				// 버퍼 쪽에도 그림 그리는 코드
					
				// 버퍼쪽에서 hpen 빌려쓰고
				HPEN oldBufPen = (HPEN)SelectObject(replay_buffer.bf_dc, hPen);

				if (is_spray)
				{
					myPen.DrawSpray(replay_buffer.bf_dc, draw_end.x, draw_end.y);
				}
				else
				{
				MoveToEx(replay_buffer.bf_dc, draw_start.x, draw_start.y, NULL);

				LineTo(replay_buffer.bf_dc, draw_end.x, draw_end.y);
				}

				// 다시 반납
				SelectObject(replay_buffer.bf_dc, oldBufPen);
			}
			lines.push_back({ draw_start, draw_end, new_pen, get_time, is_spray, myPen.color, myPen.width });
			g_isSaved = false;

			draw_start = draw_end;

			SelectObject(hdc, oldPen);

			DeleteObject(hPen);
			
			ReleaseDC(hWnd, hdc);
		}
	}
	break;

	case WM_LBUTTONUP:
	{
		DW.is_drawing = false;
		ReleaseCapture();
	}
	break;
	
	
	case WM_PAINT:
	{
		PAINTSTRUCT cPs;
		HDC hdc = BeginPaint(hWnd, &cPs);


		// 잔상 지우기
		replay_buffer.ClearBuffer();

		// 펜 형식 안 가져오면 색 안써짐
		GetPaintLine(replay_buffer.bf_dc, &lines, isReplaying);

		// 그리기
		replay_buffer.DrawBuffer(hdc);

		/// 벡터의 내용으로 PAINT에 그리는 함수
		/// UtilFunc.cpp 참조
		//if (isReplaying)
		//{
		//	GetPaintLine(hdc, &lines, true);
		//}
		//else
		//{
		//	GetPaintLine(hdc, &lines);
		//}


		EndPaint(hWnd, &cPs);
	}
	break;

	case WM_DESTROY:
	{
		replay_buffer.ReleaseBuffer();
		PostQuitMessage(0);
	}
	break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	
	return DefWindowProc(hWnd, message, wParam, lParam);

}



DrwWindow::DrwWindow()
{
}


DrwWindow::~DrwWindow()
{

}