#pragma once
#include <windows.h>
#include <vector>
#include "StructData.h"

bool ShowFileDialog(HWND hWnd, wchar_t* filePath, bool isSave); //파일 보여주기
bool File_Save(const wchar_t* path, std::vector<Line>& lines); //파일저장
bool File_Call(const wchar_t* path, std::vector<Line>& lines); // 파일불러오기
bool File_AutoSave(std::vector<Line>& lines); //파일 자동 저장