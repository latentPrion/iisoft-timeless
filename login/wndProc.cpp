
#include "timeless.h"

uint8_t		keyboardHookActive=0;
HHOOK		keyboardHookHandle=0;

LRESULT CALLBACK kbdProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	KBDLLHOOKSTRUCT* keyInfo;

	keyInfo = (KBDLLHOOKSTRUCT*)lParam;

	if (nCode < 0 || nCode != HC_ACTION) {
		return CallNextHookEx(keyboardHookHandle, nCode, wParam, lParam);
	};

	switch (wParam)
	{
	case WM_KEYUP:
	case WM_KEYDOWN:
	case WM_SYSKEYUP:
	case WM_SYSKEYDOWN:
		if (keyInfo->vkCode == VK_MENU
			|| keyInfo->vkCode == VK_LWIN
			|| keyInfo->vkCode == VK_RWIN
			|| keyInfo->vkCode == VK_F4
			|| keyInfo->vkCode == VK_TAB)
		{
			// Consume the keypress.
			return 1;
		}
		else {
			return CallNextHookEx(keyboardHookHandle, nCode, wParam, lParam);
		};
	default:
		return CallNextHookEx(keyboardHookHandle, nCode, wParam, lParam);
	};
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		loginScreen.drawElements(hwnd);
		return 0;

	case WM_CLOSE:
		return 0;

	case WM_ACTIVATEAPP:
		if (wParam == TRUE)
		{
			if (keyboardHookActive) { return 0; };
			keyboardHookHandle = SetWindowsHookEx(
				WH_KEYBOARD_LL, &kbdProc,
				GetModuleHandle(NULL),
				0);

			keyboardHookActive = 1;
		}
		else
		{
			if (!keyboardHookActive) { return 0; };
			UnhookWindowsHookEx(keyboardHookHandle);
			keyboardHookActive = 0;
		};

		return 0;

	case WM_COMMAND:
		if (LOWORD(wParam) == 1) {
			messaging::attemptLogin(hwnd);
		};

		return 0;

	case TIMELESS_LOGIN_SUCCESSFUL:
		loginScreen.destroyWindow();
		if (monitor.createWindow(moduleHandle) != 0) {
			PostQuitMessage(EXIT_FAILURE);
			return 0;
		} else {
			monitor.showWindow();

			timerHandle = SetTimer(monitorWindowHandle, 01, 5000, NULL);
			if (timerHandle == 0)
			{
				MessageBox(monitorWindowHandle,
					TEXT("Unable to create timer! Fatal."),
					TEXT("Monitor"),
					MB_ICONERROR);

				PostQuitMessage(EXIT_FAILURE);
				return 0;
			};
		};

		return 0;

	default: return DefWindowProc(hwnd, message, wParam, lParam);
	};
}

LRESULT CALLBACK monitorProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		monitor.drawElements(hwnd);
		return 0;

	case WM_COMMAND:
		if (LOWORD(wParam) == 5)
		{
			// Send logout message.
			messaging::logout(hwnd);
			// Switch to the login screen. No need to update the time.
			monitor.destroyWindow();
			if (loginScreen.createWindow(moduleHandle) != 0)
			{
				PostQuitMessage(EXIT_FAILURE);
				return 0;
			}
			else
			{
				loginScreen.showWindow();
				return 0;
			};
		};

		return 0;

	case WM_CLOSE:
		return 0;

	case WM_ACTIVATEAPP:
		if (wParam == TRUE)
		{
			if (keyboardHookActive) { return 0; };
			keyboardHookHandle = SetWindowsHookEx(
				WH_KEYBOARD_LL, &kbdProc,
				GetModuleHandle(NULL),
				0);

			keyboardHookActive = 1;
		}
		else
		{
			if (!keyboardHookActive) { return 0; };
			UnhookWindowsHookEx(keyboardHookHandle);
			keyboardHookActive = 0;
		};

		return 0;

	case WM_TIMER:
		if (minutes == 0 && seconds <= 5)
		{
			// update the time.
			monitor.updateDisplay();
			// Close the session.
			messaging::logout(hwnd);
			// Close monitor and re-open login screen.
			PostMessage(hwnd, TIMELESS_LOGOUT_MANDATE, TIMELESS_LOGOUT_TYPE_EXPIRED, 0);
			return 0;
		};

		if (seconds == 0)
		{
			minutes--;
			seconds = 55;
			// Update the time.
			messaging::updateTime(hwnd, minutes);
			monitor.updateDisplay();
			return 0;
		}
		else
		{
			seconds -= 5;
			// Check for force closure.
			if (messaging::getRemainingTime(hwnd) == MSG_GET_CURRTIME_FORCE_CLOSED)
			{
				messaging::logout(hwnd);
				PostMessage(hwnd, TIMELESS_LOGOUT_MANDATE, TIMELESS_LOGOUT_TYPE_FORCE_CLOSED, 0);
				return 0;
			};
			// IF Force closure, then:
			// PostMessage(hwnd, TIMELESS_LOGOUT_MANDATE, TIMELESS_LOGOUT_TYPE_FORCE_CLOSED, 0);
			monitor.updateDisplay();
			return 0;
		};

		return 0;

	case TIMELESS_LOGOUT_MANDATE:
		monitor.destroyWindow();
		if (loginScreen.createWindow(moduleHandle) != 0) {
			PostQuitMessage(EXIT_FAILURE);
			return 0;
		}
		else
		{
			loginScreen.showWindow();
			switch (wParam)
			{
			case TIMELESS_LOGOUT_TYPE_EXPIRED:
				MessageBox(
					mainWindowHandle,
					TEXT("Your session has expired!\nPlease see the front desk for more information."),
					TEXT("Monitor"),
					MB_ICONINFORMATION);

				break;
			
			case TIMELESS_LOGOUT_TYPE_FORCE_CLOSED:
				MessageBox(
					mainWindowHandle,
					TEXT("Your session has been terminated by the administrator.\nPlease see the front desk for more information."),
					TEXT("Monitor"),
					MB_ICONINFORMATION);

				break;
			
			default: break;
			};

			return 0;
		};

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	};
}

/*LRESULT CALLBACK timerProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}*/