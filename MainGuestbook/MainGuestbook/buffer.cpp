#include "buffer.h"


ReplayBuffer::~ReplayBuffer()
{
    ReleaseBuffer();
}



/// 버퍼 생성 함수
void ReplayBuffer::CreateBuffer(HDC hdc, int width, int height)
{
    // 창 크기가 0 이하면 비트맵 만들다가 메모리 용량을 알 수 없게 되버려 에러남
   if (width <= 0 || height <= 0) return;
   
   // 도화지 생성 & 크기 변화가 아무것도 없으면 더 생성하지 않게하는 조건문
   if (bf_dc != nullptr && bf_width == width && bf_height == height) return;

   ReleaseBuffer();
   
   // 변화하면 바로 업데이트
   bf_width = width;
   bf_height = height;

   bf_dc = CreateCompatibleDC(hdc);
   
   // 비트맵 생성 변수
   bf_bitmap = CreateCompatibleBitmap(hdc, width, height);
   // 기본 비트맵 잡고 있을 변수
   bf_change = (HBITMAP)SelectObject(bf_dc, bf_bitmap);

}



// 자원 해제 함수
void ReplayBuffer::ReleaseBuffer()
{
    if (bf_dc != nullptr)
    {
        // 기본 비트맵과 내가 만든 비트맵 다시 교체
        SelectObject(bf_dc, bf_change);
        // 가상 도화지 삭제
        DeleteObject(bf_bitmap);
        // ReleaseDC 과는 역할이 다름 가상 DC는 os가 아닌  내가 만든 자원이라 스스로 파괴 필요
        DeleteDC(bf_dc);

        // 전부 초기화
        bf_dc = nullptr;
        bf_bitmap = nullptr;
        bf_change = nullptr;
        bf_width = 0;
        bf_height = 0;
    }
}



// 버퍼에 그린 그림 옮기는 함수
void ReplayBuffer::DrawBuffer(HDC hdc)
{
    if (bf_dc != nullptr && bf_width > 0 && bf_height > 0)
    {
        // 복사받을 위치, (목적지 시작 좌표),복사할 크기, 복사 대상, (복사 대상 시작 좌표),SRCCOPY : bf_dc를 그대로 덮어씌움
        BitBlt(hdc, 0, 0, bf_width, bf_height, bf_dc, 0, 0, SRCCOPY);
    }
}



// 이전의 잔상을 지우는 함수
void ReplayBuffer::ClearBuffer()
{
    if (bf_dc == nullptr) return;

    // 색칠 범위 계산하는 변수
    RECT rect = { 0, 0, bf_width, bf_height };

    // 어떤 색으로 덮을지 (흰색)
    HBRUSH bf_brush = CreateSolidBrush(RGB(255, 255, 255));

    HBRUSH bf_os_brush = (HBRUSH)SelectObject(bf_dc, bf_brush);

    // 만든 버퍼에 선택한 범위까지 정한 색으로 채움
    FillRect(bf_dc, &rect, bf_brush);

    SelectObject(bf_dc, bf_os_brush);
    //만든 색 바로 삭제
    DeleteObject(bf_brush);

}