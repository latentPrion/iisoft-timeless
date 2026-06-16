#include <Windows.h>
//#include <sql.h>
//#include <sqlext.h>

#include "odbcserver.h"

#include <cstdio>
#include <cstdint>
#include <cerrno>

#define _UNICODE

uint8_t		envDone=0, dbcDone=0, driverDone=0;
SQLHENV		env;
SQLHDBC		dbc;
SQLCHAR		returnedConnString[1024];
SQLSMALLINT	returnedConnStringLength;
socketConnectionC	socketConnection;
uint8_t		recvBuff[4096];
TCHAR		*strTmp;
static TCHAR		mbStrTmp[1024];

static inline void exitSafely(int ret)
{
	if (driverDone) { SQLDisconnect(dbc); };
	if (dbcDone) { SQLFreeHandle(SQL_HANDLE_DBC, dbc); };
	if (driverDone) { SQLFreeHandle(SQL_HANDLE_ENV, env); };
	PostQuitMessage(ret);
}

void processMessage(uint8_t *recvBuff, int clientFd, sockaddr_in *sock);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	SQLRETURN		ret;
	WSADATA			wsaData;
	sockaddr_in		tmpSock;
	int			tmpSockSize;
	int			tmpSockFd;

	ZeroMemory(&env, sizeof(env));
	ZeroMemory(&dbc, sizeof(dbc));
	ZeroMemory(&wsaData, sizeof(wsaData));
	ZeroMemory(&tmpSock, sizeof(tmpSock));

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		MessageBox(NULL, TEXT("Failed to startup WSA socket subsystem"), TEXT("odbcserver"), 0);
	};

	printf("Hello.");
	socketConnection.listenOn(32769);

	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env) != SQL_SUCCESS) {
		MessageBox(NULL, TEXT("Failed to alloc env handle"), TEXT("odbcserver"), 0);
		exitSafely(EXIT_FAILURE);
	};
	envDone = 1;

	if (SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0) != SQL_SUCCESS) {
		MessageBox(NULL, TEXT("Failed to set odbc3"), TEXT("odbcserver"), 0);
		exitSafely(EXIT_FAILURE);
	};

	if (SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc) != SQL_SUCCESS) {
		MessageBox(NULL, TEXT("Failed to alloc dbc handle"), TEXT("odbcserver"), 0);
		exitSafely(EXIT_FAILURE);
	};
	dbcDone = 1;

	ret = SQLDriverConnect(
		dbc, NULL, (SQLCHAR *)"DSN=tmsdb;", SQL_NTS,
		returnedConnString, sizeof(returnedConnString), &returnedConnStringLength,
		SQL_DRIVER_COMPLETE);

	if (!SQL_SUCCEEDED(ret)) {
		MessageBox(NULL, TEXT("Failed to connect to DSN"), TEXT("odbcserver"), 0);
		exitSafely(EXIT_FAILURE);
	};
	driverDone = 1;

	if (SQLAllocHandle(SQL_HANDLE_STMT, dbc, &login0) != SQL_SUCCESS) {
		MessageBox(NULL, TEXT("Failed to alloc login0 stmt handle"), TEXT("odbcserver"), 0);
		exitSafely(EXIT_FAILURE);
	};

	if (SQLAllocHandle(SQL_HANDLE_STMT, dbc, &currTime0) != SQL_SUCCESS) {
		MessageBox(NULL, TEXT("Failed to alloc currTime0 stmt handle"), TEXT("odbcserver"), 0);
		exitSafely(EXIT_FAILURE);
	};

	if (SQLAllocHandle(SQL_HANDLE_STMT, dbc, &updateTime0) != SQL_SUCCESS) {
		MessageBox(NULL, TEXT("Failed to alloc updateTime0 stmt handle"), TEXT("odbcserver"), 0);
		exitSafely(EXIT_FAILURE);
	};

	if (SQLAllocHandle(SQL_HANDLE_STMT, dbc, &logout0) != SQL_SUCCESS) {
		MessageBox(NULL, TEXT("Failed to alloc updateTime0 stmt handle"), TEXT("odbcserver"), 0);
		exitSafely(EXIT_FAILURE);
	};

	int buffLen = 0;
	for (;;)
	{
		tmpSockFd = accept(socketConnection.getFd(), (sockaddr *)&tmpSock, &tmpSockSize);
		if (tmpSockFd == -1)
		{
			MessageBox(
				NULL, TEXT("Accept() loop: Got bad connection (-1) retval. Skipping."),
				TEXT("odbcserver"), 0);

			continue;
		};

		for (;;)
		{
			buffLen = recv(tmpSockFd, (char *)recvBuff, sizeof(sockMessageS), 0);
			if (buffLen == 0)
			{
				break;
				// Orderly shutdown message.
			};

			if ((buffLen == -1 || buffLen < sizeof(sockMessageS)))
			{
				tsprintf(mbStrTmp, TEXT("Recv: Failed a recv from client, ret was %d, errno is %d\n\nstrerror:\n%s."),
					buffLen, errno, _wcserror(errno));
				
				MessageBox(NULL, mbStrTmp, TEXT("odbcserver"), 0);
				continue;
			};

			processMessage(recvBuff, tmpSockFd, &tmpSock);
		};
	};

	MessageBox(
		NULL,
		TEXT("Exited accept() loop."),
		TEXT("odbcserver"),
		MB_ICONINFORMATION);

	exitSafely(0);
}

inline void processMessage(uint8_t *recvBuff, int clientFd, sockaddr_in *sock)
{
	sockMessageS		*buff;

	buff = (sockMessageS *)recvBuff;

	switch (buff->mtype)
	{
	case MTYPE_LOGIN:
		messages::processLogin(clientFd, buff);
		return;

	case MTYPE_LOGIN_SESSION:
		messages::processLoginSession(clientFd, buff);
		return;

	case MTYPE_GET_CURRENT_TIME:
		messages::processGetCurrentTime(clientFd, buff);
		return;

	case MTYPE_UPDATE_TIME:
		messages::processUpdateTime(clientFd, buff);
		return;

	case MTYPE_LOGOUT:
		messages::processLogout(clientFd, buff);
		return;

	default:
		tsprintf(mbStrTmp, TEXT("Unknown message type %d from client 0x%x."),
			buff->mtype, sock->sin_addr.s_addr);
	};
}
