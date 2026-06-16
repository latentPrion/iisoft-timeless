
#include "timeless.h"

HWND		mainWindowHandle=0;
WNDCLASS	mainWindowClass;

HWND		usernameBoxHandle;
HWND		passwordBoxHandle;
HWND		submitButtonHandle;
HWND		connIpBoxHandle;
HWND		sessionBoxHandle;


loginScreenC::loginScreenC(void)
{
	status = STATUS_DESTROYED;
	windowClassCreated = 0;
}

loginScreenC::~loginScreenC(void)
{
	if (status != STATUS_DESTROYED) {
		destroyWindow();
	};
}

int loginScreenC::createWindow(HINSTANCE hModule)
{
	if (!windowClassCreated)
	{
		/* Initialize and register the window class.
		 * Want to create a window that is maximized, Z-ORDER at top level,
		 * and not minimizable.
		 **/
		mainWindowClass.style = CS_HREDRAW | CS_VREDRAW;
		mainWindowClass.lpfnWndProc = &WndProc;
		mainWindowClass.hInstance = hModule;
		mainWindowClass.hIcon = NULL;
		mainWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		mainWindowClass.hbrBackground = (HBRUSH) COLOR_WINDOW;
		mainWindowClass.lpszMenuName = NULL;
		mainWindowClass.lpszClassName = TEXT("mainWindowClass");

		if (!RegisterClass(&mainWindowClass))
		{
			MessageBox(
				NULL, TEXT("Login Screen: Failed to register window class."),
				TEXT("Login Screen"), MB_ICONINFORMATION);

				return -1;
		};

		windowClassCreated = 1;
	};

	mainWindowHandle = CreateWindow(
		TEXT("mainWindowClass"),
		TEXT("Timeless Internet Cafe Time Management System"),
		WS_BORDER |WS_MAXIMIZE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hModule,
		NULL);

	if (!mainWindowHandle)
	{
		MessageBox(
			NULL,
			TEXT("Login Screen: CreateWindow failed."),
			TEXT("Login Screen"),
			MB_ICONERROR);

		return -1;
	};

	if (!SetWindowPos(
		mainWindowHandle,
		HWND_TOPMOST,
		0, 0,
		//GetSystemMetrics(SM_CXSCREEN),
		//GetSystemMetrics(SM_CYSCREEN),
		0, 0,
		SWP_NOMOVE | SWP_NOSIZE))
	{
		MessageBox(
			mainWindowHandle,
			TEXT("Login Screen: Failed resize and Z-ordering."),
			TEXT("Login Screen"),
			MB_ICONERROR);

		return -1;
	};

	status = STATUS_CREATED;
	return 0;
}

void loginScreenC::destroyWindow(void)
{
	if (status != STATUS_DESTROYED)
	{
		DestroyWindow(mainWindowHandle);
		status = STATUS_DESTROYED;
	};

	if (keyboardHookActive)
	{
		UnhookWindowsHookEx(keyboardHookHandle);
		keyboardHookActive = 0;
	};
}

int loginScreenC::showWindow(void)
{
	if (status != STATUS_CREATED)
	{
		MessageBox(
			NULL,
			TEXT("Login Screen: Attempt to ShowWindow when window is not in status STATUS_CREATED."),
			TEXT("Login Screen"),
			MB_ICONERROR);

		return -1;
	};

	status = STATUS_DISPLAYED;
	return ShowWindow(mainWindowHandle, SW_MAXIMIZE);
}

int loginScreenC::drawElements(HWND hwnd)
{
	usernameBoxHandle = CreateWindow(
		TEXT("EDIT"), TEXT(""),
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER,
		680, 200, 200, 20,
		hwnd,
		(HMENU)2,
		NULL,
		NULL);

	if (!usernameBoxHandle)
	{
		MessageBox(mainWindowHandle, TEXT("Failed."), TEXT("GG"), 0);
		return -1;
	};

	passwordBoxHandle = CreateWindow(
		TEXT("EDIT"), TEXT(""),
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD,
		680, 240, 200, 20,
		hwnd,
		(HMENU)3,
		NULL,
		NULL);

	if (!passwordBoxHandle)
	{
		MessageBox(mainWindowHandle, TEXT("Failed."), TEXT("GG"), 0);
		return -1;
	};

	submitButtonHandle = CreateWindow(
		TEXT("BUTTON"), TEXT("Log in"),
		WS_VISIBLE | WS_TABSTOP | WS_CHILD,
		740, 270, 80, 25,
		hwnd, (HMENU)1,
		NULL, NULL);

	if (!submitButtonHandle)
	{
		MessageBox(mainWindowHandle, TEXT("Failed."), TEXT("GG"), 0);
		return -1;
	};

	connIpBoxHandle = CreateWindow(
		TEXT("EDIT"), TEXT(""),
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER,
		680, 20, 200, 20,
		hwnd,
		(HMENU)4,
		NULL,
		NULL);

	if (!connIpBoxHandle)
	{
		MessageBox(mainWindowHandle, TEXT("Failed."), TEXT("GG"), 0);
		return -1;
	};

	sessionBoxHandle = CreateWindow(
		TEXT("EDIT"), TEXT(""),
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER,
		680, 120, 200, 20,
		hwnd,
		(HMENU)7,
		NULL,
		NULL);

	if (!sessionBoxHandle)
	{
		MessageBox(mainWindowHandle, TEXT("Failed."), TEXT("GG"), 0);
		return -1;
	};

	CreateWindow(
		TEXT("STATIC"), TEXT("IP Address:"),
		WS_VISIBLE | WS_CHILD,
		600, 20, 80, 15,
		hwnd, NULL, NULL, NULL);

	CreateWindow(
		TEXT("STATIC"), TEXT("Session ID:"),
		WS_VISIBLE | WS_CHILD,
		600, 120, 80, 15,
		hwnd, NULL, NULL, NULL);

	CreateWindow(
		TEXT("STATIC"), TEXT("Username:"),
		WS_VISIBLE | WS_CHILD,
		600, 200, 80, 15,
		hwnd, NULL, NULL, NULL);

	CreateWindow(
		TEXT("STATIC"), TEXT("Password:"),
		WS_VISIBLE | WS_CHILD,
		600, 240, 80, 15,
		hwnd, NULL, NULL, NULL);

	return 0;
}
