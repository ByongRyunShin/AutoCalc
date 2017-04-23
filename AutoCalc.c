//======================================================================
// Win32 API Template - Douglas Boling 코드
//
//======================================================================
#include <windows.h>                 // 윈도우 관련 정의
#include "AutoCalc.h"          // 프로그램 관련 정의
#include <Shellapi.h>
#include <stdio.h>

//----------------------------------------------------------------------
// 전역 데이터
//
static const LPCWSTR szAppName = TEXT("20150680 신병륜");   // 프로그램 이름
BOOL execResult=FALSE;
HWND calhnd = NULL;								//계산기 핸들
HINSTANCE hInst;								// 프로그램 인스턴스 핸들
HWND hwndMain;									// 메인 윈도우 핸들

//======================================================================
// 프로그램 시작점
//
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine, int nCmdShow)
{
    MSG msg;
    int rc = 0;

    // 어플리케이션 초기화
    if( !InitApplication(hInstance) )
		return 0;

    // 인스턴스 초기화
    if( !InitInstance(hInstance, lpCmdLine, nCmdShow) )
		return 0;

    // 메시지 루프
    while (GetMessage (&msg, NULL, 0, 0)) {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }

    // 인스턴스 소거
    return TermInstance (hInstance, msg.wParam);
}

//----------------------------------------------------------------------
// InitApp - 어플리케이션 초기화
//
BOOL InitApplication (HINSTANCE hInstance)
{
    WNDCLASS wc;

    // 전역 변수에 인스턴스 핸들 보관
    hInst = hInstance;

    // 주 윈도우 클래스 등록
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS; // 윈도우 스타일
    wc.lpfnWndProc = MainWndProc;             // 윈도우 프로시저
    wc.cbClsExtra = 0;                        // 추가 클래스 데이터
    wc.cbWndExtra = 0;                        // 추가 윈도우 데이터
    wc.hInstance = hInstance;                 // 소유자 핸들
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);   // 프로그램 아이콘
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);// 기본 커서
    wc.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
    wc.lpszMenuName =  NULL;                  // 메뉴 이름
    wc.lpszClassName = (LPWSTR)szAppName;     // 윈도우 클래스 이름

    if (!RegisterClass(&wc) )
		return FALSE;

    return TRUE;
}

//----------------------------------------------------------------------
// InitInstance - 인스턴스 초기화
//
BOOL InitInstance (HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // 주 윈도우 생성
    hwndMain = CreateWindowEx (WS_EX_APPWINDOW,
						 szAppName,           // 윈도우 클래스
                         szAppName,     // 윈도우 타이틀
                         // 스타일 플래그
                         WS_OVERLAPPEDWINDOW,  //WS_VISIBLE | WS_SYSMENU | WS_CAPTION,
                         CW_USEDEFAULT,       // x 좌표
                         CW_USEDEFAULT,       // y 좌표
                         500,       // 초기 너비
                         200,       // 초기 높이
                         NULL,                // 부모 윈도우 핸들
                         NULL,                // 메뉴 (NULL로 설정)
                         hInstance,           // 응용프로그램 인스턴스
                         NULL);               // 생성 매개변수 포인터

    if (!IsWindow (hwndMain))
		return FALSE;  // 윈도우 생성 실패시 작동 실패

    // 윈도우 표시 및 갱신
    ShowWindow (hwndMain, nCmdShow);
    UpdateWindow (hwndMain);
    return TRUE;
}

//----------------------------------------------------------------------
// TermInstance - 프로그램 소거
//
int TermInstance (HINSTANCE hInstance, int nDefRC)
{
    return nDefRC;
}

//======================================================================
// 주 윈도우를 위한 메시지 처리 핸들러
//
//----------------------------------------------------------------------
// MainWndProc - 주 윈도우의 콜백 함수
//
LRESULT CALLBACK MainWndProc (HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
    int i;
    //
    // 메시지 분배 테이블을 검사하여 해당 메시지를 처리할지 확인한 후
    // 해당 메시지 핸들러를 호출
    //
    for (i = 0; i < dim(MainMessages); i++) {
        if (wMsg == MainMessages[i].Code)
            return (*MainMessages[i].Fxn)(hWnd, wMsg, wParam, lParam);
    }
    return DefWindowProc (hWnd, wMsg, wParam, lParam);
}

//----------------------------------------------------------------------
// DoCreateMain - WM_CREATE 메시지 처리
//
LRESULT DoCreateMain (HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
    return 0;
}

//----------------------------------------------------------------------
// DoPaintMain - WM_PAINT 메시지 처리
//
LRESULT DoPaintMain (HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    RECT rect;
    HDC hdc;
	TCHAR str[100];

    // 클라이언트 영역의 크기 계산
    GetClientRect (hWnd, &rect);
    hdc = BeginPaint (hWnd, &ps);
	wsprintf(str, TEXT("%#08x"), calhnd);
	if (execResult) {
		DrawText (hdc, str, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
    
    EndPaint (hWnd, &ps); 
    return 0;
}

//----------------------------------------------------------------------
// DoDestroyMain - WM_DESTROY 메시지 처리
//
LRESULT DoDestroyMain (HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
    PostQuitMessage (0);
    return 0;
} 
LRESULT DoLButtonDownMain(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	//handle=ShellExecute(hWnd, TEXT("open"), TEXT("calc.exe"), NULL, NULL, SW_SHOWNORMAL);
	SHELLEXECUTEINFO sInfo;
	HANDLE process_handle = NULL;
	TCHAR str[100];

	ZeroMemory(&sInfo, sizeof(SHELLEXECUTEINFO));
	sInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	sInfo.lpFile = TEXT("calc.exe"); //프로세스 경로
	sInfo.lpParameters = NULL; //전달 인자
	sInfo.nShow = SW_SHOWNORMAL; //실행시 최소화
	sInfo.lpVerb = TEXT("runas"); //관리자 권한 실행
	sInfo.fMask = SEE_MASK_NOCLOSEPROCESS; //프로세스 핸들값 사용

	execResult = ShellExecuteExW(&sInfo);

	if (execResult)
	{
		process_handle = sInfo.hProcess;
		calhnd = GetHwndFromProcessHandle(process_handle);
		wsprintf(str, TEXT("%#08x"), calhnd);
		MessageBox(hWnd, str, "hi", NULL);
		InvalidateRect(hWnd, NULL, TRUE);
	}
	return 0;
}

LRESULT DoRButtonDownMain(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

BOOL CALLBACK CallbackEnumWindowProc(HWND hWnd, LPARAM lParam)
{
	HANDLE hTarget = NULL;
	DWORD dwID = 0, dwSrcID = (DWORD)(*(int*)lParam);

	GetWindowThreadProcessId(hWnd, &dwID);

	if (dwID == dwSrcID)
	{
		*((int*)lParam) = (int)hWnd;
		*(((int*)lParam) + 1) = 1;
		return FALSE;
	}

	return TRUE;
}
HWND GetHwndFromProcessHandle(HANDLE hProcess)
{
	if (hProcess == NULL)
		return NULL;

	__int64 lltmp = 0; // 64bit 를 상위 하위 나누어 쓴다
	lltmp = (int)GetProcessIDbyProcessHandle(hProcess); // GetProcessId()

	EnumWindows(CallbackEnumWindowProc, (LPARAM)&lltmp); // lltmp In/Out

	if (((int)(lltmp >> 32)))
		return (HWND)(int)lltmp;

	return NULL;
}
DWORD WINAPI GetProcessIDbyProcessHandle(HANDLE hProcess)
{
	if (hProcess == NULL)    return 0xffffffff;
	PTHREAD_START_ROUTINE lpStartAddress = (PTHREAD_START_ROUTINE)
		GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "GetCurrentProcessId");
	if (lpStartAddress == NULL) return 0xffffffff;

	HANDLE hProcessAccAdj;
	BOOL bRes = DuplicateHandle(GetCurrentProcess(),
		hProcess, GetCurrentProcess(), &hProcessAccAdj,
		PROCESS_QUERY_INFORMATION | PROCESS_CREATE_THREAD |
		PROCESS_VM_OPERATION | PROCESS_VM_WRITE,
		FALSE, 0);
	if (!bRes || hProcessAccAdj == NULL)
	{
		UINT unError = GetLastError();
		return 0xffffffff;
	}
	DWORD dwThreadID;
	HANDLE hRemoteThread = CreateRemoteThread(hProcessAccAdj, NULL,
		0, lpStartAddress, 0, 0, &dwThreadID);
	CloseHandle(hProcessAccAdj);
	if (hRemoteThread == NULL) return 0xffffffff;
	WaitForSingleObject(hRemoteThread, INFINITE);
	DWORD dwExitCode;
	if (GetExitCodeThread(hRemoteThread, &dwExitCode) == 0)    dwExitCode = 0xffffffff;
	CloseHandle(hRemoteThread);
	return dwExitCode;
}