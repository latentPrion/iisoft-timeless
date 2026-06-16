#ifndef _TIMELESS_MAIN_HEADER_H
	#define _TIMELESS_MAIN_HEADER_H

	#include <windows.h>
	#include <cstdint>
	#include <cstring>
	#pragma comment(lib, "wsock32.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK monitorProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK kbdProc(int nCode, WPARAM wParam, LPARAM lParam);
//LRESULT CALLBACK timerProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

extern HINSTANCE	moduleHandle;
extern HWND		mainWindowHandle;
extern HWND		monitorWindowHandle;
extern WNDCLASS		mainWindowClass, monitorWindowClass;
extern uint8_t		activeApp;
extern HHOOK		keyboardHookHandle;
extern uint8_t		keyboardHookActive;

extern HWND		usernameBoxHandle;
extern HWND		passwordBoxHandle;
extern HWND		submitButtonHandle;
extern HWND		connIpBoxHandle;
extern HWND		sessionBoxHandle;
extern HWND		counterTextHandle;
extern UINT_PTR		timerHandle;

extern int		minutes, seconds;

extern uint32_t		sessionId;

#define tsprintf(buff,str,...)		wsprintf(buff,str,__VA_ARGS__)

#define TIMELESS_LOGIN_SUCCESSFUL	(WM_APP+1)
#define TIMELESS_LOGOUT_MANDATE		(WM_APP+2)

#define TIMELESS_LOGOUT_TYPE_EXPIRED		0
#define TIMELESS_LOGOUT_TYPE_MANUAL		1
#define TIMELESS_LOGOUT_TYPE_FORCE_CLOSED	2

#define MTYPE_IDENTIFY_CLIENT_PC	1
#define MTYPE_LOGIN			2
#define MTYPE_GET_CURRENT_TIME		3
#define MTYPE_UPDATE_TIME		4
#define MTYPE_LOGOUT			5
#define MTYPE_LOGIN_SESSION		6

extern char	ipString[];

class loginScreenC
{
public:
	loginScreenC(void);
	~loginScreenC(void);

public:
	int createWindow(HINSTANCE hModule);
	void destroyWindow(void);
	int showWindow(void);

	// To be handled inside of WM_CREATE.creas
	int drawElements(HWND hwnd);

	int isDisplayed(void)
	{
		return (status == STATUS_DISPLAYED);
	}

public:
	enum statusE
	{
		STATUS_DESTROYED=0,
		STATUS_CREATED,
		STATUS_DISPLAYED
	};

private:
	statusE		status;
	uint8_t		windowClassCreated;
};

extern loginScreenC	loginScreen;

class monitorC
{
public:
	monitorC(void);
	~monitorC(void);

public:
	int createWindow(HINSTANCE hModule);
	void destroyWindow(void);
	int showWindow(void);

	// To be handled inside of WM_CREATE.
	int drawElements(HWND hwnd);
	void updateDisplay(void);

	int isDisplayed(void)
	{
		return (status == STATUS_DISPLAYED);
	}

private:
	enum statusE
	{
		STATUS_DESTROYED=0,
		STATUS_CREATED,
		STATUS_DISPLAYED
	};

	statusE		status;
	uint8_t		windowClassCreated;
};

struct sockMessageS;

class connectionC
{
public:
	connectionC(void);
	~connectionC(void);

	int testAndConnect(const char *hostname, uint16_t port);
	int send(sockMessageS *msg);
	int recv(sockMessageS *msg);

private:
	int		sockfd;
	sockaddr_in	sock;
	int		connected;
};

extern monitorC		monitor;

namespace messaging
{
	int attemptLogin(HWND hwnd);
	int getRemainingTime(HWND hwnd);
	void logout(HWND hwnd);
	int updateTime(HWND hwnd, uint32_t minutes);
}

struct sockMessageS
{
	// message type.
	uint32_t	mtype;
	union
	{
		// mtype = 1: identify client PC message.
		struct
		{
			uint32_t	pcNo;
			int32_t		ret;
		} identifyClientPc;
		// mtype = 2: login message.
		struct
		{
			#define MSG_LOGIN_SUCCESS		1
			#define MSG_LOGIN_USER_PASS_INVALID	2
			uint32_t	pcNo;
			int32_t		ret;
			// Deprecated and moved into MTYPE_LOGIN_SESSION message.
			uint32_t	sessionId;
			TCHAR		username[64];
			TCHAR		password[64];
		} login;
		// mtype = 3: get current time message
		struct
		{
			#define MSG_GET_CURRTIME_SERVER_ERROR	(-1)
			#define MSG_GET_CURRTIME_FORCE_CLOSED	(-2)
			uint32_t	pcNo;
			int32_t		ret;
		} getCurrentTime;
		// mtype = 4: update time message.
		struct
		{
			uint32_t	pcNo;
			uint32_t	minutes;
			int32_t		ret;
		} updateTime;
		// mtype = 5: logout message.
		struct
		{
			#define MSG_LOGOUT_SUCCESS		1;
			#define MSG_LOGOUT_WAS_NOT_LOGGED_IN	2
			uint32_t	pcNo;
			int32_t		ret;
		} logout;

		// mtype = 6: login session mesage.
		struct
		{
			#define MSG_LOGIN_SESSION_SUCCESS		(1)
			#define MSG_LOGIN_SESSION_INVALID		(2)
			uint32_t	pcNo;
			int32_t		ret;
			int32_t		sessionId;
		} loginSession;
	};

};

extern class connectionC	connection;


#endif
