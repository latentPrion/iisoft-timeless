
#include "odbcserver.h"
#include <cstdio>

TCHAR	mbStrTmp[1024];
SQLHSTMT	login0;
int		userId;
int		sessionId;
int		employeeId=0;
int		userSessionType=0;
int		tempSessionType=1;
int		timeRemaining=0;
char		loginQString[1024];
int		loginType=0;

void extract_error(
    char *fn,
    SQLHANDLE handle,
    SQLSMALLINT type)
{
    SQLINTEGER	 i = 0;
    SQLINTEGER	 native;
    SQLCHAR	 state[ 7 ];
    SQLCHAR	 text[256];
    SQLSMALLINT	 len;
    SQLRETURN	 ret;

    tsprintf(mbStrTmp,
            TEXT("\nThe driver reported the following diagnostics whilst running %s\n\n"),
            fn);
	MessageBox(NULL, mbStrTmp, TEXT("odbcserver"), 0);

    do
    {
        ret = SQLGetDiagRec(type, handle, ++i, state, &native, text,
                            sizeof(text), &len );
        if (SQL_SUCCEEDED(ret)) {
            tsprintf(mbStrTmp, TEXT("%s:%ld:%ld:%s\n"), state, i, native, text);
	    MessageBox(NULL, mbStrTmp, TEXT("odbcserver"), 0);
	};
    }
    while( ret == SQL_SUCCESS );
}

static char *convertToUtf8(const TCHAR *str)
{
	char		*ret;
	int		i;

	ret = (char *)malloc(sizeof(char) * 64);
	if (ret == NULL) {
		return NULL;
	};

	for (i=0; str[i] != 0; i++) {
		ret[i] = (char)str[i];
	};

	ret[i] = 0;
	return ret;
}

#define REFRESH_STMT(__stmt)		\
	do \
	{ \
		SQLFreeHandle(SQL_HANDLE_STMT, __stmt); \
		if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_STMT, dbc, &__stmt))) \
		{ \
			MessageBox(NULL, TEXT("Failed to refresh SQL stmt." #__stmt), TEXT("odbcserver"), MB_ICONERROR); \
			return -1; \
		} \
	} while (0);


int odbcFuncs::loginSession(sockMessageS *msg)
{
	SYSTEMTIME	stime;
	char		startTime[26];

	GetSystemTime(&stime);
	sprintf(startTime, "%04d-%02d-%02d %02d:%02d:00",
		stime.wYear, stime.wMonth, stime.wDay,
		stime.wHour, stime.wMinute);

	sprintf(loginQString, "SELECT * FROM session WHERE id=%d", msg->loginSession.sessionId);
	REFRESH_STMT(login0);

	if (!SQL_SUCCEEDED(SQLExecDirect(login0, (SQLCHAR *)loginQString, SQL_NTS)))
	{
		extract_error("Failed to execute query to look up session in session-login.", login0, SQL_HANDLE_STMT);
		return -1;
	};

	if (SQLFetch(login0) == SQL_NO_DATA)
	{
		return MSG_LOGIN_SESSION_INVALID;
	};

	loginType = tempSessionType;
	sessionId = msg->loginSession.sessionId;

	// Set the session ID in the pc record.
	REFRESH_STMT(login0);
	sprintf(loginQString, "UPDATE pc SET lastSessionId=%d, currentSessionId=%d WHERE id=%d",
		sessionId, sessionId, msg->loginSession.pcNo);

	if (!SQL_SUCCEEDED(SQLExecDirect(login0, (SQLCHAR *)loginQString, SQL_NTS)))
	{
		extract_error(TEXT("Failed setting session ID in PC record."), login0, SQL_HANDLE_STMT);
		return -1;
	};

	sprintf(loginQString, "UPDATE session SET startTime='%s' WHERE id=%d", startTime, msg->loginSession.sessionId);
	REFRESH_STMT(login0);

	if (!SQL_SUCCEEDED(SQLExecDirect(login0, (SQLCHAR *)loginQString, SQL_NTS)))
	{
		extract_error("Failed to execute query to set session start time in session-login.", login0, SQL_HANDLE_STMT);
		return -1;
	};

	return MSG_LOGIN_SESSION_SUCCESS;
}

int odbcFuncs::userConfirm(sockMessageS *msg)
{
	char		*username, *password;
	SQLRETURN	sqlret;
	SQLLEN		ind;
	SYSTEMTIME	stime;
	char		startTime[26];

	GetSystemTime(&stime);
	sprintf(startTime, "%04d-%02d-%02d %02d:%02d:00",
		stime.wYear, stime.wMonth, stime.wDay,
		stime.wHour, stime.wMinute);

	username = convertToUtf8(msg->login.username);
	password = convertToUtf8(msg->login.password);
	if (username == NULL || password == NULL) {
		return -1;
	};

	REFRESH_STMT(login0);
	SQLPrepare(
		login0,
		(SQLCHAR *)"SELECT * FROM customer WHERE username = ? AND password = ?",
		strlen("SELECT * FROM customer WHERE username = ? AND password = ?"));

	SQLBindParameter(
		login0,
		1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0,
		username, strlen(username), NULL);

	SQLBindParameter(
		login0,
		2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0,
		password, strlen(username), NULL);

	// Get and store the customer ID.
	if (!SQL_SUCCEEDED(SQLBindCol(login0, 1, SQL_C_SLONG,
		&userId, 0, &ind)))
	{
		MessageBox(NULL, TEXT("Failed to bind ID of customer.%d."), TEXT(""), 0);
		return -1;
	};
	if (!SQL_SUCCEEDED(SQLBindCol(login0, 6, SQL_C_SLONG,
		&timeRemaining, 0, &ind)))
	{
		MessageBox(NULL, TEXT("Failed to bind reminaing time of customer.%d."), TEXT(""), 0);
		return -1;
	};

	if (!SQL_SUCCEEDED(SQLExecute(login0)))
	{
		MessageBox(NULL, TEXT("Failed to execute login select query. Reason %d."), TEXT("odbcserver"), 0);
		free(username);
		free(password);
		return -1;
	};

	free(username);
	free(password);

	loginType = userSessionType;

	sqlret = SQLFetch(login0);
	if (!SQL_SUCCEEDED(sqlret)) {
		return MSG_LOGIN_USER_PASS_INVALID;
	};

	REFRESH_STMT(login0);
	sprintf(loginQString, "INSERT into session (customerId, employeeId, type, startTime, pcId, startTimeSeed, closed) VALUES (?, ?, ?, '%s', %d, '%s', 0)",
		startTime, msg->login.pcNo, startTime);

	SQLPrepare(
		login0,
		(SQLCHAR *)loginQString,
		strlen(loginQString));

	SQLBindParameter(
		login0,
		1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0,
		&userId, 0, NULL);

	SQLBindParameter(
		login0,
		2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0,
		&employeeId, 0, NULL);

	SQLBindParameter(
		login0,
		3, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0,
		&userSessionType, 0, NULL);

	if (!SQL_SUCCEEDED(SQLExecute(login0)))
	{
		extract_error(TEXT("trying to create session query."), login0, SQL_HANDLE_STMT);
		return -1;
	};

	// Get the session ID that was just created.
	REFRESH_STMT(login0);
	sprintf(loginQString, "SELECT * FROM session WHERE customerId=%d AND startTimeSeed='%s'",
		userId, startTime);

	if (!SQL_SUCCEEDED(SQLBindCol(login0, 1, SQL_C_SLONG,
		&sessionId, 0, &ind)))
	{
		extract_error(TEXT("Trying bind session ID column"), login0, SQL_HANDLE_STMT);
	};

	if (!SQL_SUCCEEDED(SQLExecDirect(login0, (SQLCHAR *)loginQString, SQL_NTS)))
	{
		extract_error(TEXT("Trying get session ID."), login0, SQL_HANDLE_STMT);
		return -1;
	};

	SQLFetch(login0);

	// SEt the session ID in the pc record.
	REFRESH_STMT(login0);
	sprintf(loginQString, "UPDATE pc SET lastSessionId=%d, currentSessionId=%d WHERE id=%d",
		sessionId, sessionId, msg->login.pcNo);

	if (!SQL_SUCCEEDED(SQLExecDirect(login0, (SQLCHAR *)loginQString, SQL_NTS)))
	{
		extract_error(TEXT("Failed setting session ID in PC record."), login0, SQL_HANDLE_STMT);
		return -1;
	};

	return MSG_LOGIN_SUCCESS;
}


char	currTimeQString[1024];
SQLHSTMT	currTime0;

int odbcFuncs::getCurrentTime(sockMessageS *msg)
{
	int		closed=0, tmpSessionTimeRemaining;
	SQLINTEGER	ind;

	sprintf(currTimeQString, "SELECT * FROM session WHERE id = %d", sessionId);

	REFRESH_STMT(currTime0);
	if (!SQL_SUCCEEDED(SQLBindCol(currTime0, 12, SQL_C_SLONG, &closed, 0, &ind)))
	{
		extract_error("Failed to bind force close column", currTime0, SQL_HANDLE_STMT);
		return -1;
	};

	if (!SQL_SUCCEEDED(SQLBindCol(currTime0, 13, SQL_C_SLONG, &tmpSessionTimeRemaining, 0, &ind)))
	{
		extract_error("Failed to bind tmpsession reminaing time column", currTime0, SQL_HANDLE_STMT);
		return -1;
	};

	if (!SQL_SUCCEEDED(SQLExecDirect(currTime0, (SQLCHAR *)currTimeQString, SQL_NTS)))
	{
		extract_error("Failed to exec query to get force close check column", currTime0, SQL_HANDLE_STMT);
		return -1;
	};

	// Get the values.
	if (SQLFetch(currTime0) == SQL_NO_DATA)
	{
		MessageBox(NULL, TEXT("When getting session for remining time lookup, got no records returned."),
			TEXT("odbcserver"), 0);

		return -1;
	};

	/*tsprintf(mbStrTmp, TEXT("Returned values: force closed %d, tempsessionremainingtime %d."),
		closed, tmpSessionTimeRemaining);
	MessageBox(NULL, mbStrTmp, TEXT("Values"), 0);*/

	if (closed) {
		return -2;
	};

	if (loginType == tempSessionType) {
		return tmpSessionTimeRemaining;
	};

	if (!closed)
	{
		// Get the amount of time remaining for the user.
		sprintf(currTimeQString, "SELECT * FROM customer WHERE id=%d", userId);
		REFRESH_STMT(currTime0);

		if (!SQL_SUCCEEDED(SQLBindCol(currTime0, 6, SQL_C_SLONG, &timeRemaining, 0, &ind)))
		{
			extract_error("Failed to bind time remaining column in customer", currTime0, SQL_HANDLE_STMT);
			return -1;
		};

		if (!SQL_SUCCEEDED(SQLExecDirect(currTime0, (SQLCHAR *)currTimeQString, SQL_NTS)))
		{
			extract_error("Failed to get the time remaining for customer", currTime0, SQL_HANDLE_STMT);
			return -1;
		};

		SQLFetch(currTime0);
		return timeRemaining;
	};

	return -1;
}


SQLHSTMT	updateTime0;
char		updateTimeQString[1024];

int odbcFuncs::updateTime(sockMessageS *msg)
{
	// Just send an update query.
	if (loginType == userSessionType) {
		sprintf(updateTimeQString, "UPDATE customer SET timeRemaining=%d WHERE id=%d", msg->updateTime.minutes, userId);
	} else {
		sprintf(updateTimeQString, "UPDATE session SET timeRemaining=%d WHERE id=%d", msg->updateTime.minutes, sessionId);
	};

	REFRESH_STMT(updateTime0);
	if (!SQL_SUCCEEDED(SQLExecDirect(updateTime0, (SQLCHAR *)updateTimeQString, SQL_NTS)))
	{
		extract_error(TEXT("Sending query for updating time."), updateTime0, SQL_HANDLE_STMT);
		return -1;
	};

	return 1;
}


SQLHSTMT	logout0;
char		logoutQString[1024];

int odbcFuncs::logout(sockMessageS *msg)
{
	// Set the currentSessionId in the PC table to 0.
	REFRESH_STMT(logout0);
	sprintf(logoutQString, "UPDATE pc SET currentSessionId = 0 WHERE id=%d", msg->logout.pcNo);
	if (!SQL_SUCCEEDED(SQLExecDirect(logout0, (SQLCHAR *)logoutQString, SQL_NTS)))
	{
		extract_error("Sending query for logout to clear currentSessionId", logout0, SQL_HANDLE_STMT);
		return -1;
	};

	return 1;
}
