#ifndef _ODBCSERVER_MAIN_HEADER_H
	#define _ODBCSERVER_MAIN_HEADER_H

#include <Windows.h>
#include <sql.h>
#include <sqlext.h>
#include <cstdint>
#include <cstring>
#pragma comment(lib, "wsock32.lib")

extern int		userId;
extern int		sessionId;
extern int		employeeId;
extern int		userSessionType;
extern int		tempSessionType;
extern int		timeRemaining;

#define tsprintf(buff,str,...)		wsprintf(buff,str,__VA_ARGS__)

void extract_error(
    char *fn,
    SQLHANDLE handle,
    SQLSMALLINT type);

#define MTYPE_IDENTIFY_CLIENT_PC	1
#define MTYPE_LOGIN			2
#define MTYPE_GET_CURRENT_TIME		3
#define MTYPE_UPDATE_TIME		4
#define MTYPE_LOGOUT			5
#define MTYPE_LOGIN_SESSION		6

extern SQLHSTMT		login0, currTime0, updateTime0, logout0;
extern SQLHDBC		dbc;

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

namespace messages
{
	void processLogin(int clientFd, sockMessageS *msg);
	void processGetCurrentTime(int clientFd, sockMessageS *msg);
	void processUpdateTime(int clientFd, sockMessageS *msg);
	void processLogout(int clientFd, sockMessageS *msg);
	void processLoginSession(int clientFd, sockMessageS *msg);
}

namespace odbcFuncs
{
	int userConfirm(sockMessageS *msg);
	int userCreateSession(sockMessageS *msg);
	int user_pcUpdateCurrentSession(sockMessageS *msg);

	int loginSession(sockMessageS *msg);

	int getCurrentTime(sockMessageS *msg);
	int updateTime(sockMessageS *msg);
	int logout(sockMessageS *msg);
}

class socketConnectionC
{
public:
	socketConnectionC(void);
	~socketConnectionC(void);

	int listenOn(uint16_t port);
	int getFd(void) { return sockfd; };

private:
	void returnSafely(int retval);
	int		sockfd;
	sockaddr_in	sock;
};

#endif
