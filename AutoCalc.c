//======================================================================
// Win32 API Template - Douglas Boling �ڵ�
//
//======================================================================
#include <windows.h>                 // ������ ���� ����
#include "AutoCalc.h"          // ���α׷� ���� ����
#include <Shellapi.h>
#include <stdio.h>

//----------------------------------------------------------------------
// ���� ������
//
static const LPCWSTR szAppName = TEXT("20150680 �ź���");   // ���α׷� �̸�
BOOL execResult=FALSE;
HWND calhnd = NULL;								//���� �ڵ�
HINSTANCE hInst;								// ���α׷� �ν��Ͻ� �ڵ�
HWND hwndMain;									// ���� ������ �ڵ�

//======================================================================
// ���α׷� ������
//
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine, int nCmdShow)
{
    MSG msg;
    int rc = 0;

    // ���ø����̼� �ʱ�ȭ
    if( !InitApplication(hInstance) )
		return 0;

    // �ν��Ͻ� �ʱ�ȭ
    if( !InitInstance(hInstance, lpCmdLine, nCmdShow) )
		return 0;

    // �޽��� ����
    while (GetMessage (&msg, NULL, 0, 0)) {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }

    // �ν��Ͻ� �Ұ�
    return TermInstance (hInstance, msg.wParam);
}

//----------------------------------------------------------------------
// InitApp - ���ø����̼� �ʱ�ȭ
//
BOOL InitApplication (HINSTANCE hInstance)
{
    WNDCLASS wc;

    // ���� ������ �ν��Ͻ� �ڵ� ����
    hInst = hInstance;

    // �� ������ Ŭ���� ���
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS; // ������ ��Ÿ��
    wc.lpfnWndProc = MainWndProc;             // ������ ���ν���
    wc.cbClsExtra = 0;                        // �߰� Ŭ���� ������
    wc.cbWndExtra = 0;                        // �߰� ������ ������
    wc.hInstance = hInstance;                 // ������ �ڵ�
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);   // ���α׷� ������
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);// �⺻ Ŀ��
    wc.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
    wc.lpszMenuName =  NULL;                  // �޴� �̸�
    wc.lpszClassName = (LPWSTR)szAppName;     // ������ Ŭ���� �̸�

    if (!RegisterClass(&wc) )
		return FALSE;

    return TRUE;
}

//----------------------------------------------------------------------
// InitInstance - �ν��Ͻ� �ʱ�ȭ
//
BOOL InitInstance (HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // �� ������ ����
    hwndMain = CreateWindowEx (WS_EX_APPWINDOW,
						 szAppName,           // ������ Ŭ����
                         szAppName,     // ������ Ÿ��Ʋ
                         // ��Ÿ�� �÷���
                         WS_OVERLAPPEDWINDOW,  //WS_VISIBLE | WS_SYSMENU | WS_CAPTION,
                         CW_USEDEFAULT,       // x ��ǥ
                         CW_USEDEFAULT,       // y ��ǥ
                         500,       // �ʱ� �ʺ�
                         200,       // �ʱ� ����
                         NULL,                // �θ� ������ �ڵ�
                         NULL,                // �޴� (NULL�� ����)
                         hInstance,           // �������α׷� �ν��Ͻ�
                         NULL);               // ���� �Ű����� ������

    if (!IsWindow (hwndMain))
		return FALSE;  // ������ ���� ���н� �۵� ����

    // ������ ǥ�� �� ����
    ShowWindow (hwndMain, nCmdShow);
    UpdateWindow (hwndMain);
    return TRUE;
}

//----------------------------------------------------------------------
// TermInstance - ���α׷� �Ұ�
//
int TermInstance (HINSTANCE hInstance, int nDefRC)
{
    return nDefRC;
}

//======================================================================
// �� �����츦 ���� �޽��� ó�� �ڵ鷯
//
//----------------------------------------------------------------------
// MainWndProc - �� �������� �ݹ� �Լ�
//
LRESULT CALLBACK MainWndProc (HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
    int i;
    //
    // �޽��� �й� ���̺��� �˻��Ͽ� �ش� �޽����� ó������ Ȯ���� ��
    // �ش� �޽��� �ڵ鷯�� ȣ��
    //
    for (i = 0; i < dim(MainMessages); i++) {
        if (wMsg == MainMessages[i].Code)
            return (*MainMessages[i].Fxn)(hWnd, wMsg, wParam, lParam);
    }
    return DefWindowProc (hWnd, wMsg, wParam, lParam);
}

//----------------------------------------------------------------------
// DoCreateMain - WM_CREATE �޽��� ó��
//
LRESULT DoCreateMain (HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
    return 0;
}

//----------------------------------------------------------------------
// DoPaintMain - WM_PAINT �޽��� ó��
//
LRESULT DoPaintMain (HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    RECT rect;
    HDC hdc;
	TCHAR str[100];

    // Ŭ���̾�Ʈ ������ ũ�� ���
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
// DoDestroyMain - WM_DESTROY �޽��� ó��
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
	sInfo.lpFile = TEXT("calc.exe"); //���μ��� ���
	sInfo.lpParameters = NULL; //���� ����
	sInfo.nShow = SW_SHOWNORMAL; //����� �ּ�ȭ
	sInfo.lpVerb = TEXT("runas"); //������ ���� ����
	sInfo.fMask = SEE_MASK_NOCLOSEPROCESS; //���μ��� �ڵ鰪 ���

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

	__int64 lltmp = 0; // 64bit �� ���� ���� ������ ����
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