//======================================================================
// ��� ����
//
#ifndef _WINMAIN_STYLE_4_H_
#define _WINMAIN_STYLE_4_H_
//================================================================
// ������ ���� ��ȯ
#define dim(x) (sizeof(x) / sizeof(x[0])) 

//----------------------------------------------------------------------
// ������ Ÿ�� ����
//
struct decodeUINT {   // �޽����� �Լ��� ���� ��Ű�� ����ü
    UINT Code;
    LRESULT (*Fxn)(HWND, UINT, WPARAM, LPARAM);
}; 

struct decodeCMD {    // �޴� ID�� �Լ��� ���� ��Ű�� ����ü
    UINT Code;
    LRESULT (*Fxn)(HWND, WORD, HWND, WORD);
};

//----------------------------------------------------------------------
// �Լ� ����
//
BOOL InitApplication (HINSTANCE);
BOOL InitInstance (HINSTANCE, LPSTR, int);
int TermInstance (HINSTANCE, int);

// ������ ���ν���
LRESULT CALLBACK MainWndProc (HWND, UINT, WPARAM, LPARAM);

//����� �Լ�
BOOL CALLBACK CallbackEnumWindowProc(HWND hWnd, LPARAM lParam);
HWND GetHwndFromProcessHandle(HANDLE hProcess);
DWORD WINAPI GetProcessIDbyProcessHandle(HANDLE hProcess);

// �޽��� �ڵ鷯
LRESULT DoCreateMain (HWND, UINT, WPARAM, LPARAM);
LRESULT DoPaintMain (HWND, UINT, WPARAM, LPARAM);
LRESULT DoDestroyMain (HWND, UINT, WPARAM, LPARAM);
LRESULT DoLButtonDownMain(HWND, UINT, WPARAM, LPARAM);
LRESULT DoRButtonDownMain(HWND, UINT, WPARAM, LPARAM);


//----------------------------------------------------------------------
// MainWndProc�� ó�� �޽��� �й� ���̺�
const struct decodeUINT MainMessages[] = {
    WM_CREATE, DoCreateMain,
    WM_PAINT, DoPaintMain,
    WM_DESTROY, DoDestroyMain,
	WM_LBUTTONDOWN, DoLButtonDownMain,
	WM_RBUTTONDOWN, DoRButtonDownMain,
};

#endif	/* _WINMAIN_STYLE_4_H_ */