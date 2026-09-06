#include "File_io.h"
#include <commdlg.h>
#pragma comment(lib, "comdlg32.lib")

bool ShowFileDialog(HWND hWnd, wchar_t* filePath, bool isSave)
{
    OPENFILENAMEW ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = filePath;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"Drawing Files\0*.drw\0";
    ofn.lpstrDefExt = L"drw";

    if (isSave)
        return GetSaveFileNameW(&ofn);
    else
        return GetOpenFileNameW(&ofn);
}

bool File_Save(const wchar_t* path, std::vector<Line>& lines)
{
    FILE* fp = nullptr;
    _wfopen_s(&fp, path, L"wb");
    if (!fp) return false;

    int count = (int)lines.size();
    fwrite(&count, sizeof(int), 1, fp);
    fwrite(lines.data(), sizeof(Line), count, fp);

    fclose(fp);
    return true;
}

bool File_Call(const wchar_t* path, std::vector<Line>& lines)
{
    FILE* fp = nullptr;
    _wfopen_s(&fp, path, L"rb");
    if (!fp) return false;

    int count = 0;
    fread(&count, sizeof(int), 1, fp);

    lines.clear();
    lines.resize(count);
    fread(lines.data(), sizeof(Line), count, fp);

    fclose(fp);
    return true;
}