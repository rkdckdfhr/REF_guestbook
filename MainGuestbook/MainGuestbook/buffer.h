#pragma once
#include <windows.h>


/// 필요한 기능 : 가상 도화지에서 리플레이 돌리기 -> 가상에서 돌린걸 원본으로 보여줘 -> 이걸 반복해 -> 벡터끝나 -> 가상 도화지 해제
class ReplayBuffer
{
public:
	// default -> 객체를 만들고 이후에 값이 들어가는 구조가 될 것 이기 때문에 default 필수
	ReplayBuffer() = default;
		~ReplayBuffer();

		// 비트맵 크기 설정 변수들
		int bf_width = 0;
		int bf_height = 0;

		HDC bf_dc = nullptr;

		// 버퍼 만드는 함수
		void CreateBuffer(HDC hdc,int width,int height);

		// 버퍼 그림을 지우는 함수 invalidaterect 같은 느낌 겹치는 문제 때문에 필요
		void ClearBuffer();

		// 버퍼에서 그리는 함수
		void DrawBuffer(HDC hdc);

		// 버퍼 해제 함수
		void ReleaseBuffer();

		// 그림 그릴 비트맵
		HBITMAP bf_bitmap = nullptr;
		// 기본 비트맵 저장할 변수
		HBITMAP bf_change = nullptr;

		// 비트맵을 써도 문제가 없는 이유 :	CreateCompatibleBitmap 이 API를 사용할 예정인데 이걸 쓰면 hdc 인수를 사용해서
		// 비트 구조가 똑같아짐
};
