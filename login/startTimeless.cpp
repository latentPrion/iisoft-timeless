#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	STARTUPINFOA		si;
	_PROCESS_INFORMATION	pi;

	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);

	printf("Hello.\n");

	if (!CreateProcess(
		"timeless.exe", NULL,
		NULL, NULL,
		TRUE,
		1,
		NULL,
		NULL,
		&si,
		&pi))
	{
		printf("Failed to create process. Last error is %d.\n", GetLastError());
		exit(EXIT_FAILURE);
	};

	printf("New clone process created, ID is %d.\n", pi.dwProcessId);
	Sleep(50000);
}

