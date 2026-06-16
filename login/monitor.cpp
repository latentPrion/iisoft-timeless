
#include "timeless.h"


HWND		monitorWindowHandle=0;
WNDCLASS	monitorWindowClass;

HWND		logoutButtonHandle;
HWND		counterTextHandle;
UINT_PTR	timerHandle;

int		minutes, seconds;

static TCHAR	mbStrTmp[1024];

monitorC::monitorC(void)
{
	status = STATUS_DESTROYED;
	windowClassCreated = 0;
}

monitorC::~monitorC(void)
{
	destroyWindow();
}

int monitorC::createWindow(HINSTANCE hModule)
{
	if (!windowClassCreated)
	{
		/* Initialize and register the window class.
		 * Want to create a window that is maximized, Z-ORDER at top level,
		 * and not minimizable.
		 **/
		monitorWindowClass.style = CS_HREDRAW | CS_VREDRAW;
		monitorWindowClass.lpfnWndProc = &monitorProc;
		monitorWindowClass.hInstance = hModule;
		monitorWindowClass.hIcon = NULL;
		monitorWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		monitorWindowClass.hbrBackground = (HBRUSH) COLOR_WINDOW;
		monitorWindowClass.lpszMenuName = NULL;
		monitorWindowClass.lpszClassName = TEXT("monitorWindowClass");

		if (!RegisterClass(&monitorWindowClass))
		{
			MessageBox(
				NULL, TEXT("Monitor: Failed to register window class."),
				TEXT("Monitor"), MB_ICONINFORMATION);

				return -1;
		};

		windowClassCreated = 1;
	};

	monitorWindowHandle = CreateWindow(
		TEXT("monitorWindowClass"),
		TEXT("Timeless Internet Cafe Time Management System"),
		0,
		0, 0,
		400,
		56,
		NULL,
		NULL,
		hModule,
		NULL);

	if (!monitorWindowHandle)
	{
		MessageBox(
			NULL,
			TEXT("Monitor: CreateWindow failed."),
			TEXT("Monitor"),
			MB_ICONERROR);

		return -1;
	};

	if (!SetWindowPos(
		monitorWindowHandle,
		HWND_TOPMOST,
		0, 0,
		0, 0,
		SWP_NOMOVE | SWP_NOSIZE))
	{
		MessageBox(
			monitorWindowHandle,
			TEXT("Monitor: Failed resize and Z-ordering."),
			TEXT("Monitor"),
			MB_ICONERROR);
		
		return -1;
	};

	status = STATUS_CREATED;
	return 0;
}

int monitorC::showWindow(void)
{
	if (status != STATUS_CREATED)
	{
		MessageBox(
			monitorWindowHandle,
			TEXT("Monitor: Attempt to ShowWindow when window is not in status STATUS_CREATED."),
			TEXT("Monitor"),
			MB_ICONERROR);

		return -1;
	};

	status = STATUS_DISPLAYED;
	return ShowWindow(monitorWindowHandle, SW_SHOWNORMAL);
}

void monitorC::destroyWindow(void)
{
	if (status != STATUS_DESTROYED)
	{
		status = STATUS_DESTROYED;
		DestroyWindow(monitorWindowHandle);
	};

	if (keyboardHookActive)
	{
		UnhookWindowsHookEx(keyboardHookHandle);
		keyboardHookActive = 0;
	};

}

int monitorC::drawElements(HWND hwnd)
{
	tsprintf(mbStrTmp, TEXT("Time remaining: %dmin : %dsec."), minutes, seconds);

	counterTextHandle = CreateWindow(
		TEXT("STATIC"), mbStrTmp,
		WS_VISIBLE | WS_CHILD,
		10, 5, 260, 15,
		hwnd,
		NULL,
		NULL,
		NULL);

	if (!counterTextHandle)
	{
		MessageBox(monitorWindowHandle, TEXT("Failed to create element."), TEXT("Monitor"), 0);
		return -1;
	};

	logoutButtonHandle = CreateWindow(
		TEXT("BUTTON"), TEXT("Log out"),
		WS_VISIBLE | WS_TABSTOP | WS_CHILD,
		300, 1, 80, 25,
		hwnd, (HMENU)5,
		NULL, NULL);

	if (!logoutButtonHandle)
	{
		MessageBox(monitorWindowHandle, TEXT("Failed to create element."), TEXT("Monitor"), 0);
		return -1;
	};

	return 0;
}

void monitorC::updateDisplay(void)
{
	DestroyWindow(counterTextHandle);

	tsprintf(mbStrTmp, TEXT("Time remaining: %dmin : %dsec."), minutes, seconds);
	counterTextHandle = CreateWindow(
		TEXT("STATIC"), mbStrTmp,
		WS_VISIBLE | WS_CHILD,
		10, 5, 260, 15,
		monitorWindowHandle,
		NULL, NULL, NULL);
}
