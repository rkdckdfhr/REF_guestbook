#include <windows.h>
#include "Pen_tool.h"
#include <commdlg.h> //공통 대화 상자 헤더파일
//기본세팅
Pen_tool::Pen_tool() {
	width = 5;
	style = PS_SOLID; //실선
	color = RGB(0, 0, 0);
}
//원하는 값 넣어서 값 세팅
void Pen_tool::Pen_tool2(int s, int w) {
	style = s;
	width = w;
}

//진짜 펜 생성
HPEN Pen_tool::Pen() {
	// 브러시 구조체
	LOGBRUSH lb;
	lb.lbStyle = BS_SOLID;				// 단색 채우기 모드
	lb.lbColor = color;					// 선택한 펜 색상 반영
	lb.lbHatch = 0;						// 빗금 채우기 모드. 안 쓰는 멤버 변수, 쓰레기값 방지. 안정성.
	
	// ExtCreatePen : 굵게해도 펜 형태 유지를 위한 함수
	// PS_GEOMETRIC : ExtCreatePen 안에 있는 펜 형태 유지 상수
								//펜 스타일, 펜 두께, 색상채우기주소 X, X
	return ExtCreatePen(PS_GEOMETRIC | style, width, &lb, 0, NULL);
}

// TODO : 브러쉬 넣기

/// 생상 선택 상자 구현
bool Pen_tool::SelectColor(HWND hWnd) {
	CHOOSECOLOR cc;									// cc : 생상 설정 창 띄우기 위한 변수 이름
	static COLORREF acrCustClr[16];					// 색상 선택 후 유지

	ZeroMemory(&cc, sizeof(cc));					// 구조체 초기화
	cc.lStructSize = sizeof(cc);					// 윈도우 API 버전을 확인하기 위한 구조체 크기 설정 (필수 규칙)
	cc.hwndOwner = hWnd;							// 색상 창이 메인 윈도우(hWnd) 위쪽에 뜨도록 부모 창 지정
	cc.lpCustColors = (LPDWORD)acrCustClr;			// 색상 16개 저장소 연결
	cc.rgbResult = color;							//현재 팬 색상을 기본값으로 설정
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;			// 창 전체 펼치기(FULLOPEN) + 현재 펜 색상 위치에 커서 표시(RGBINIT) 옵션 적용

	///색상 창 색 변경
	if (ChooseColor(&cc) == true) {
		color = cc.rgbResult; //선택한 색 반영
		return true; //변경 성공
	}
	return false; //변경 취소
}
