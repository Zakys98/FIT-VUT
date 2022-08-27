#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

// Global variable
HINSTANCE hInst;
UINT  MessageCount = 0;
UINT  Count = 0;
int posX = 340;
int posY = 333;
COLORREF color = RGB(255, 255, 0);

// Function prototypes.
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
void paintObject(HWND hWnd, HDC hDC, PAINTSTRUCT ps, int posX, int posY, POINT cursorPosition);
void paintObject2(HWND hWnd, HDC hDC, PAINTSTRUCT ps, int posX, int posY, POINT cursorPosition);
void paintPosition(HWND hWnd, HDC hDC, PAINTSTRUCT ps);

// Application entry point. This is the same as main() in standart C.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	BOOL bRet;
	WNDCLASS wcx;          // register class
	HWND hWnd;

	hInst = hInstance;     // Save the application-instance handle.
		// Fill in the window class structure with parameters that describe the main window.

	wcx.style = CS_HREDRAW | CS_VREDRAW;              // redraw if size changes
	wcx.lpfnWndProc = (WNDPROC)MainWndProc;          // points to window procedure
	wcx.cbClsExtra = 0;                               // no extra class memory
	wcx.cbWndExtra = 0;                               // no extra window memory
	wcx.hInstance = hInstance;                        // handle to instance
	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);      // predefined app. icon
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);        // predefined arrow
	wcx.hbrBackground = GetStockObject(WHITE_BRUSH);  // white background brush
	wcx.lpszMenuName = (LPCSTR)"MainMenu";          // name of menu resource
	wcx.lpszClassName = (LPCSTR)"MainWClass";        // name of window class

	// Register the window class.

	if (!RegisterClass(&wcx)) return FALSE;

	// create window of registered class

	hWnd = CreateWindow(
		"MainWClass",        // name of window class
		"ITU",               // title-bar string
		WS_OVERLAPPEDWINDOW, // top-level window
		200,                  // default horizontal position
		25,                 // default vertical position
		1000,                // default width
		700,                 // default height
		(HWND)NULL,         // no owner window
		(HMENU)NULL,        // use class menu
		hInstance,           // handle to application instance
		(LPVOID)NULL);      // no window-creation data
	if (!hWnd) return FALSE;

	// Show the window and send a WM_PAINT message to the window procedure.
	// Record the current cursor position.

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// loop of message processing
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			// handle the error and possibly exit
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}


LRESULT CALLBACK MainWndProc(
	HWND hWnd,        // handle to window
	UINT uMsg,        // message identifier
	WPARAM wParam,    // first message parameter
	LPARAM lParam)    // second message parameter
{
	HDC         hDC;
	PAINTSTRUCT ps;
	POINT cursorPosition;

	// init cursor position 
	GetCursorPos(&cursorPosition);
	ScreenToClient(hWnd, &cursorPosition);

	switch (uMsg)
	{
	case WM_CREATE:
		break;

		// character input 
	case WM_CHAR:
		switch (wParam) {
		case 0x08:  // backspace
		case 0x0A:  // linefeed
		case 0x1B:  // escape
			break;

		case 0x09:  // tab
			break;

		default:
			break;
		}
		break;

		// key input
	case WM_KEYDOWN:
		switch (wParam) {
			// update posX and posY in order to move object
		case VK_LEFT: // left arrow
			posX -= 5; 
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_RIGHT: // right arrow
			posX += 5;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_UP: // up arrow
			posY -= 5;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_DOWN: // down arrow
			posY += 5;
			InvalidateRect(hWnd, NULL, TRUE);
			break;

			// react on the other pressed keys 
		case VK_SPACE: // space
			color = RGB(0, 0, 255);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_BACK: // backspace
			break;
		case VK_TAB: // tab
			break;
			// more virtual codes can be found here: https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
		}
		break;

		// get cursor position 
	case WM_MOUSEMOVE:
		posX = cursorPosition.x - 100;
		posY = cursorPosition.y - 100;
		InvalidateRect(hWnd, NULL, TRUE);
		break;

		// react on mouse clicks
	case WM_LBUTTONDOWN:
		color = RGB(0, 255, 0);
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONUP:
		break;
	case WM_RBUTTONDOWN:
		color = RGB(255, 0, 0);
		InvalidateRect(hWnd, NULL, TRUE);
		break;

		// paint objects
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		paintObject(hWnd, hDC, ps, posX, posY, cursorPosition);
		paintPosition(hWnd, hDC, ps);
		EndPaint(hWnd, &ps);
		DeleteDC(hDC);
		break;

		//
		// Process other messages.
		//
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}


void paintObject(HWND hWnd, HDC hDC, PAINTSTRUCT ps, int posX, int posY, POINT cursorPosition)
{
	// Paint rectangles, ellipses, polygons, lines etc.
	SelectObject(hDC, GetStockObject(DC_BRUSH)); 
	
	SetDCBrushColor(hDC, color);
	Rectangle(hDC, posX - 10, posY + 180, posX + 10, posY + 80);
	Rectangle(hDC, posX + 15, posY + 180, posX + 35, posY + 80);
	Rectangle(hDC, posX + 190, posY + 180, posX + 210, posY + 80);
	Rectangle(hDC, posX + 165, posY + 180, posX + 185, posY + 80);
	Rectangle(hDC, posX + 165, posY - 150, posX + 200, posY + 10);
	Rectangle(hDC, posX, posY, posX + 200, posY + 100);

	POINT vertices[] = { {posX + 160, posY - 153}, {posX + 210, posY - 153}, {posX + 235, posY - 105} };
	Polygon(hDC, vertices, 3);
	
	SetDCBrushColor(hDC, RGB(0, 0, 0));
	Ellipse(hDC, posX + 20, posY + 20, posX + 40, posY + 40);
	Ellipse(hDC, posX + 50, posY + 50, posX + 70, posY + 70);
	Ellipse(hDC, posX + 120, posY + 75, posX + 140, posY + 95);
	Ellipse(hDC, posX + 70, posY + 70, posX + 90, posY + 90);
	Ellipse(hDC, posX + 120, posY + 10, posX + 140, posY + 30);
	Ellipse(hDC, posX + 150, posY + 30, posX + 170, posY + 50);

	Ellipse(hDC, posX + 180, posY - 140, posX + 190, posY - 150);
	Ellipse(hDC, posX + 200, posY - 140, posX + 210, posY - 150);

	HPEN pen = CreatePen(PS_SOLID, 10, RGB(0, 0, 0));
	SelectObject(hDC, pen);
	MoveToEx(hDC, posX, posY + 20, NULL);
	LineTo(hDC, posX - 60, posY + 60);
	DeleteObject(pen);

	pen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
	SelectObject(hDC, pen);
	MoveToEx(hDC, posX + 180, posY - 153, NULL);
	LineTo(hDC, posX + 165, posY - 193);
	MoveToEx(hDC, posX + 195, posY - 153, NULL);
	LineTo(hDC, posX + 203, posY - 193);
	DeleteObject(pen);

	return;
}

void paintPosition(HWND hWnd, HDC hDC, PAINTSTRUCT ps)
{
	char        text[40];          // buffer to store an output text
	HFONT       font;              // new large font
	HFONT       oldFont;           // saves the previous font

	font = CreateFont(25, 0, 0, 0, 0, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, 0);
	oldFont = (HFONT)SelectObject(hDC, font);
	sprintf(text, "Position -- x:%d, y:%d", posX, posY);
	TextOut(hDC, 50, 600, text, (int)strlen(text));
	SelectObject(hDC, oldFont);
	DeleteObject(font);
}
