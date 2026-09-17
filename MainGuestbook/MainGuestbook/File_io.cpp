#include "File_io.h"
#include <commdlg.h>
#include <stdio.h>
#pragma comment(lib, "comdlg32.lib")// 윈도우 기본 파일 대화상자 기능을 쓰기 위해 시스템 라이브러리 연결

// 실행 파일(.exe) 위치를 기준으로 자동저장 폴더 경로를 만드는 함수
// 컴퓨터, 폴더 위치가 달라져도 항상 exe 옆에 폴더를 만들도록 함
void Folder_Road(wchar_t* outFolder, size_t bufferSize)
{
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH); // 지금 실행 중인 exe의 전체 경로를 실시간으로 알아냄

    wchar_t* lastSlash = wcsrchr(exePath, L'\\'); // 경로에서 마지막 \ 위치 찾기
    if (lastSlash) *lastSlash = L'\0';            // 그 자리를 문자열 끝으로 잘라서 파일명(.exe) 제거, 폴더 경로만 남김

    swprintf_s(outFolder, bufferSize, L"%s\\저장 파일", exePath); 
}

bool ShowFileDialog(HWND hWnd, wchar_t* filePath, bool isSave)
{
    OPENFILENAMEW ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;       // 부모 윈도우 설정
    ofn.lpstrFile = filePath;   // 사용자가 선택한 파일 경로를 담아올 빈 문자열 
    ofn.nMaxFile = MAX_PATH;    // 경로 최대 길이 제한
    ofn.lpstrFilter = L"Drawing Files\0*.drw\0"; // 사용자에게 보여줄 확장자 종류
    ofn.nFilterIndex = 1;       // 필터 목록 중 1번째를 기본으로 사용
    ofn.lpstrDefExt = L"drw";   // 기본 확장자 설정

    wchar_t initDir[MAX_PATH];
    Folder_Road(initDir, MAX_PATH);   // 다이얼로그가 자동저장 폴더에서 열리도록
    ofn.lpstrInitialDir = initDir;

    if (isSave)
        return GetSaveFileNameW(&ofn);  //저장
    else
        return GetOpenFileNameW(&ofn); //열기
}

bool File_Save(const wchar_t* path, std::vector<Line>& lines)
{
    FILE* fp = nullptr;
    _wfopen_s(&fp, path, L"wb");
    if (!fp) return false;

    int count = (int)lines.size();
    fwrite(&count, sizeof(int), 1, fp); //불러올때 몇개인지 알기위해 데이터 개수 맨앞에 저장
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
    lines.resize(count); //읽어올 데이터 개수만큼 벡터 공간 미리 확보
    fread(lines.data(), sizeof(Line), count, fp); // 확보해둔 공간에 파일 안의 데이터를 한 번에 채워 넣기

    fclose(fp);
    return true;
}

bool File_Check(const wchar_t* path) //중복 파일 검사
{
    DWORD path_Check = GetFileAttributesW(path); // 
    return (path_Check != INVALID_FILE_ATTRIBUTES); //에러 찾아주는 상수
}

bool File_AutoSave(std::vector<Line>& lines)
{
    wchar_t folder[MAX_PATH];
    Folder_Road(folder, MAX_PATH);

    CreateDirectoryW(folder, NULL);// 자동 저장용 폴더 생성 

    wchar_t filename[MAX_PATH];
    int num = 1;

    do
    {
        swprintf_s(filename, L"%s\\file%d.drw", folder, num);//폴더 주소 번호를 합쳐서 파일 이름 문자열 
        num++;
    } while (File_Check(filename));

    return File_Save(filename, lines);
}