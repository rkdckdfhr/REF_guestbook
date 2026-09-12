#pragma once
#include <windows.h>
#include <vector>
#include "StructData.h"

bool ShowFileDialog(HWND hWnd, wchar_t* filePath, bool isSave);
bool File_Save(const wchar_t* path, std::vector<Line>& lines);
bool File_Call(const wchar_t* path, std::vector<Line>& lines);