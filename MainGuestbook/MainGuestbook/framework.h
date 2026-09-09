// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

/// 헤더 파일 모음입니다.

/// 그리기, 리플레이 창 관련 헤더 파일
#include "DRW_Window.h"
#include "ReplayWindow.h"

/// 파일 관련 헤더 파일
#include "File_io.h"

/// 메뉴 관련 헤더 파일
#include "InitUI.h"

/// 그리기 관련 헤더 파일
#include "Pen_tool.h"
