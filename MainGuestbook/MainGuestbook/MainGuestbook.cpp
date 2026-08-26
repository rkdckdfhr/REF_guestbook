// MainGuestbook.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "MainGuestbook.h"
#include "Pen_tool.h"
#include <vector>


#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MAINGUESTBOOK, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MAINGUESTBOOK));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINGUESTBOOK));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MAINGUESTBOOK);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}
//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   //HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
   //   0, 0, 1280, 640, nullptr, nullptr, hInstance, nullptr);

   HWND hWnd = CreateWindowEx(
       0, szWindowClass, L"방명록프로젝트", WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT, CW_USEDEFAULT,
       900, 700,
       NULL, NULL, hInstance, nullptr);

   
   DrwWindow DW;

   if (!hWnd)
   {
      return FALSE;
   }

   
   if (!DW.NewWnd(hInst, hWnd))
   {
       return false;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

 

   return TRUE;
}



//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//


/// 선 저장용 구조체

POINT DrwStart; /// 그리기 시작 좌표 저장
POINT DrwEnd; /// 그리기 끝 좌표 저장

std::vector<Line> Lines;
std::vector<Line> gLines;

bool isDrawing = false;

testPen tP;
HPEN hP = nullptr;
HPEN Default = nullptr;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        /// @TODO: 만들기
        
    }
    break;

    case WM_LBUTTONDOWN:
    {
        isDrawing = true;
        DrwStart.x = LOWORD(lParam);
        DrwStart.y = HIWORD(lParam);

    }
    break;
    /// 외부 파일에서 Pen 생성 후 메인으로 불러오는 테스트
    //case WM_RBUTTONDOWN:
    //{
    //    HDC hdc = GetDC(hWnd);



    //    hP = tP.hPen();

    //}
    //break;

    case WM_MOUSEMOVE:
    {
        if (isDrawing)
        {
            HDC hdc = GetDC(hWnd);
            /// 외부 파일에서 Pen 생성 후 메인으로 불러오는 테스트
            //Default = (HPEN)SelectObject(hdc, hP);
            /*HPEN hPen = CreatePen(PS_DOT, 2, RGB(255, 0, 0));
            HPEN Default = (HPEN)SelectObject(hdc, hPen);*/

            Pen_tool myPen; //기본 세팅된 펜
            HPEN hPen = myPen.Pen();
            /* 펜 스타일 옵션 줄때 괄호안에 스타일,두께,색상주기
           PS_DASH 파선
		   PS_DASHDOT 점선
		   PS_DASHDOTDOT 점선
           */
            HPEN oldPen = (HPEN)SelectObject(hdc, hPen);

            DrwEnd.x = LOWORD(lParam);
            DrwEnd.y = HIWORD(lParam);

            MoveToEx(hdc, DrwStart.x, DrwStart.y, NULL);
            LineTo(hdc, DrwEnd.x, DrwEnd.y);


            /**
            * @brief Lines 안에 있는 구조체 POINT start, POINT end에 DrwStart, DrwEnd 값을 넣는 방식
            * ex) Lines[0] . start . x == DrwStart . x   Lines[0] . start . y == DrwStart . y
            * Lines[0] . end . x == DrwEnd . x  Lines[0] . end . y == DrwEnd . y
            * 선을 그릴 때마다 좌표값이 저장된다 생각하면 편함
            */

            Lines.push_back({ DrwStart, DrwEnd });

            DrwStart.x = DrwEnd.x;
            DrwStart.y = DrwEnd.y;

            SelectObject(hdc, oldPen);
            DeleteObject(hPen);


           // ReleaseDC(hWnd, hdc);
        }
    }
    break;

    case WM_LBUTTONUP:
    {
        
        isDrawing = false;
        //InvalidateRect(hWnd, NULL, FALSE);
    }
    break;

    /// 외부 파일에서 Pen 생성 후 메인으로 불러오는 테스트
    /*
    case WM_RBUTTONUP:
    {
        DeleteObject(hP);
    }
    break;
    */

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);


            /// @TODO: 끝점과 시작점이 계속 이어짐 해결 필요 , 해결 완
            /// Lines.push_back({ DrwStart, DrwEnd })로 받아온 좌표로 창에 그려 저장
            

            for (int i = 0; i < Lines.size(); i++)
            {
                MoveToEx(hdc, Lines[i].start.x, Lines[i].start.y, NULL);
                LineTo(hdc, Lines[i].end.x, Lines[i].end.y);
            }

            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
