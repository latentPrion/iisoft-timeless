#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include "timeless.h"

#define _UNICODE
#define TL_MAINWINDOW_TITLE	"Timeless Internet Cafe Time Manager"

HINSTANCE	moduleHandle;

loginScreenC	loginScreen;
monitorC	monitor;
connectionC	connection;
WSADATA		wsaData;

char		ipString[16];
uint32_t	sessionId;

int WINAPI WinMain(
	HINSTANCE hModule,
	HINSTANCE hPrevInstance,
	PSTR szCmdLine,
	int iCmdShow
	)
{
	// Message object from the WIN32 API.
	MSG		msg;

	// Cast to void, means they are not used.
	(void)	hPrevInstance;
	(void)	szCmdLine;
	(void)	iCmdShow;

	moduleHandle = hModule;
	strcpy(ipString, "127.0.0.1");

	// Zero out the memory.
	ZeroMemory(&mainWindowClass, sizeof(mainWindowClass));
	ZeroMemory(&monitorWindowClass, sizeof(monitorWindowHandle));
	ZeroMemory(&wsaData, sizeof(wsaData));

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		MessageBox(NULL, TEXT("Failed to request WSA subsystem"), TEXT("Login Screen"), 0);
	};

	connection.testAndConnect(ipString, 32769);

	if (loginScreen.createWindow(hModule) == 0) {
		loginScreen.showWindow();
	}
	else {
		PostQuitMessage(EXIT_FAILURE);
	};

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	};

	return msg.wParam;
}
