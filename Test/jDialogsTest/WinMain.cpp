#include <Windows.h>

#include "..\..\include\jdialogs.h"

namespace jDialogs = perchik71::jDialogs;

HINSTANCE g_hInstance = NULL;
jDialogs::jDialogW* lpDialogs;

#define TITLEAPP L"jDialog Test"
#define CLASSNAME L"jDialogMainWindow"

class MainWindow
{
private:
	HWND m_hWind;
public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	BOOL Initialize(VOID);
	VOID Release(VOID);
public:
	MainWindow(VOID);
	virtual ~MainWindow(VOID);
};

INT_PTR CALLBACK DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{

	}
	return TRUE;

	case WM_CLOSE:
	{
		EndDialog(hWnd, 0);
	}
	return TRUE;

	default:
		return FALSE;
	}
}

LRESULT CALLBACK MainWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		lpDialogs = new jDialogs::jDialogW();
		/*
		lpDialogs->ParseJSON(R"({
			"ExStyle": 0,
			"Style": ["WS_SYSMENU", "WS_THICKFRAME", "WS_MINIMIZEBOX"],
			"Title": "Dialog for testing",
			"ClassName": "",
			"x": 0,
			"y": 0,
			"Width": 520,
			"Height": 365,
			"FontName": "Microsoft Sans Serif",
			"FontSize": 12,
			"FontWeight": 400,
			"FontItalic": 0,
			"Controls": [
			{"Type": "GROUPBOX", "Title": "Hello World", "x": 10, "y": 10, "Width": 400, "Height": 300},
			{"Type": "LTEXT", "Title": "Hello World", "x": 20, "y": 28, "Width": 230, "Height": 12},
			{"Type": "CTEXT", "Title": "Hello World", "x": 20, "y": 40, "Width": 230, "Height": 12},
			{"Type": "RTEXT", "Title": "Hello World", "x": 20, "y": 52, "Width": 230, "Height": 12},
			{"Type": "EDITTEXT", "Title": "Hello World", "Style": ["ES_AUTOHSCROLL"], "x": 20, "y": 64, "Width": 230, "Height": 14},
			{"Type": "DEFPUSHBUTTON", "Title": "Hello World", "x": 20, "y": 80, "Width": 120, "Height": 12},
			{"Type": "LISTBOX", "x": 60, "y": 100, "Width": 120, "Height": 80},
			{"Type": "COMBOBOX", "x": 200, "y": 100, "Width": 120, "Height": 80},
			{"Type": "AUTORADIOBUTTON", "Title": "Hello World", "x": 200, "y": 200, "Width": 120, "Height": 14},
			{"Type": "AUTOCHECKBUTTON", "Title": "Hello World", "x": 200, "y": 220, "Width": 120, "Height": 14},
			{"Type": "PROGRESSBAR", "x": 60, "y": 240, "Width": 120, "Height": 20}
			]
})"); 
*/
		lpDialogs->LoadFromFile(L"test.json");
		lpDialogs->ShowModal(hWnd, DlgProc, 0); 
		lpDialogs->SaveToFile("test.bin");
	}
	return S_OK;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hDC = BeginPaint(hWnd, &ps);
	
		EndPaint(hWnd, &ps);
	}
	return S_OK;

	case WM_CLOSE:
	{
		delete lpDialogs;
		PostQuitMessage(S_OK);
	}
	return S_OK;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

BOOL MainWindow::Initialize(VOID)
{
	WNDCLASSEX wcClass = { 0 };
	wcClass.cbSize = sizeof(WNDCLASSEX);
	wcClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcClass.hCursor = (HCURSOR)LoadCursorW(NULL, IDC_ARROW);
	wcClass.hInstance = g_hInstance;
	wcClass.lpfnWndProc = (WNDPROC)&WndProc;
	wcClass.lpszClassName = CLASSNAME;
	wcClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	
	if (!RegisterClassEx(&wcClass))
		return FALSE;

	m_hWind = CreateWindowEx(0, CLASSNAME, TITLEAPP, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
		CW_USEDEFAULT, NULL, NULL, g_hInstance, (LPVOID)this);
	if (!m_hWind)
		return FALSE;

	ShowWindow(m_hWind, SW_SHOW);
	UpdateWindow(m_hWind);

	return TRUE;
}

VOID MainWindow::Release(VOID)
{
	DestroyWindow(m_hWind);
	UnregisterClass(CLASSNAME, g_hInstance);
}

MainWindow::MainWindow(VOID): m_hWind(NULL)
{}

MainWindow::~MainWindow(VOID)
{
	Release();
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nShowCmd)
{
	if (hPrevInstance)
		return S_FALSE;

	g_hInstance = hInstance;
	MainWindow* Window = new MainWindow();
	if (!Window || !Window->Initialize())
		return S_FALSE;

	MSG msg;
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}
	}

	delete Window;
	return S_OK;
}