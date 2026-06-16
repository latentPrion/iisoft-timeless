
#include "timeless.h"
#include <cstdlib>


static TCHAR		mbStrTmp[1024];
int userLogin(HWND hwnd);
int sessionLogin(HWND hwnd);

static int sessionIsValid(TCHAR *session)
{
	for (int i=0; session[i] != NULL; i++)
	{
		if (session[i] < '0' || session[i] > '9')
		{
			MessageBox(
				mainWindowHandle, TEXT("Session ID can only have digits!"),
				TEXT("Login Screen"), MB_ICONINFORMATION);

			return 0;
		};
	};
	return 1;
}

static int usernameIsValid(TCHAR *username)
{
	int invalid = 0;

	for (int i=0; username[i] != NULL; i++)
	{
		if (username[i] < '0') { invalid = 1; };
		if (username[i] > '9' && username[i] < 'A') { invalid = 1; };
		if (username[i] > 'Z' && username[i] < 'a') { invalid = 1; };
		if (username[i] > 'z') { invalid = 1; };
		if (username[i] > 127) { invalid = 1; };
		if (username[i] == '_') { invalid = 0; };

		if (invalid)
		{
			MessageBox(
				mainWindowHandle,
				TEXT("Invalid username! Valid characters are:\n\t0-9, A-Z, a-z, and underscore."),
				TEXT("Login Screen"), MB_ICONERROR);

			return 0;
		};
	};

	return 1;
}

static int passwordIsValid(TCHAR *password)
{
	int	invalid = 0;

	for (int i=0; password[i] != NULL; i++)
	{
		if (password[i] < '0') { invalid = 1; };
		if (password[i] == '\'') { invalid = 1; };
		if (password[i] == '"') { invalid = 1; };
		if (password[i] == '?') { invalid = 1; };
		if (password[i] == '\\') { invalid = 1; };
		if (password[i] > 127) { invalid = 1; };

		if (invalid)
		{
			MessageBox(
				mainWindowHandle,
				TEXT("Invalid password! These characters are not allowed:\n\t ', \", ?, \\, <space>, and control characters."),
				TEXT("Login Screen"), MB_ICONERROR);

			return 0;
		};
	};

	return 1;
}

static TCHAR	wipString[16];

static int convertIpToUtf16(char *ip, TCHAR **out)
{
	int		i;

	*out = (TCHAR *)malloc(sizeof(TCHAR) * 16);
	if (*out == NULL) { return -1; };

	for (i=0; ip[i] != NULL; i++)
	{
		(*out)[i] = (TCHAR) ip[i];
	};
	(*out)[i] = NULL;
	return 0;
}

static int convertIpToUtf8(TCHAR *ip, char **out)
{
	int		i;

	*out = (char *)malloc(sizeof(char) * 16);
	if (*out == NULL) { return -1; };

	for (i=0; ip[i] != NULL; i++)
	{
		(*out)[i] = (char) ip[i];
	};
	(*out)[i] = NULL;
	return 0;
}

static int ipIsValid(const char *ip)
{
	int		periods=0;

	for (int i=0; ip[i] != NULL; i++)
	{
		if ((ip[i] < '0' || ip[i] > '9') && ip[i] != '.') {
			return 0;
		};

		if (ip[i] == '.') { periods++; };
	};

	if (periods == 3) {
		return 1;
	} else {
		return 0;
	};
}


static int successfulLoginProc(HWND hwnd, sockMessageS *msg)
{
	int			remainingTime;

	remainingTime = messaging::getRemainingTime(hwnd);
	if (remainingTime == 0)
	{
		MessageBox(
			mainWindowHandle,
			TEXT("Your account or session has no time remaining.\nPlease visit the front desk for more information."),
			TEXT("Login screen"),
			MB_ICONINFORMATION);

		return -1;
	};

	if (remainingTime == MSG_GET_CURRTIME_SERVER_ERROR) { return -1; };
	// Handle case for force closure.
	if (remainingTime == MSG_GET_CURRTIME_FORCE_CLOSED)
	{
		MessageBox(
			mainWindowHandle,
			TEXT("Your account or session has been forcibly terminated by the administrator.\nPlease see the help desk for more information"),
			TEXT("Login screen"),
			MB_ICONWARNING);

		return -1;
	};

	// Else, valid login, log them in and post the login message.
	sessionId = msg->login.sessionId;
	minutes = remainingTime - 1;
	seconds = 60;
	messaging::updateTime(hwnd, minutes);
	PostMessage(hwnd, TIMELESS_LOGIN_SUCCESSFUL, 0, 0);
	return 0;
}


TCHAR		sessiontmp[64];

int messaging::attemptLogin(HWND hwnd)
{

	GetWindowText(GetDlgItem(hwnd, 7), sessiontmp, 64);
	if (sessiontmp[0] != NULL) {
		return sessionLogin(hwnd);
	} else {
		return userLogin(hwnd);
	};
}

int sessionLogin(HWND hwnd)
{
	sockMessageS		msg;
	char			*utf8sessiontmp;

	GetWindowText(GetDlgItem(hwnd, 7), sessiontmp, 64);
	if (!sessionIsValid(sessiontmp)) {
		return -1;
	};

	// Convert the string into an integer value.
	if (convertIpToUtf8(sessiontmp, &utf8sessiontmp) != 0)
	{
		MessageBox(mainWindowHandle, TEXT("Failed to do utf16->utf8 conversion on session ID."),
			TEXT("Login Screen"), 0);

		return -1;
	};

	msg.mtype = MTYPE_LOGIN_SESSION;
	msg.loginSession.sessionId = atoi(utf8sessiontmp);
	msg.loginSession.pcNo = 1;
	free(utf8sessiontmp);

	if (connection.send(&msg) != 0) {
		return -1;
	};

	if (connection.recv(&msg) != 0) {
		return -1;
	};

	switch (msg.loginSession.ret)
	{
	case MSG_LOGIN_SESSION_SUCCESS:
		return successfulLoginProc(hwnd, &msg);

	case MSG_LOGIN_SESSION_INVALID:
		MessageBox(
			mainWindowHandle,
			TEXT("The session ID you entered is invalid, or does not exist."),
			TEXT("Login Screen"),
			MB_ICONERROR);

		return -1;

	default:
		tsprintf(mbStrTmp, TEXT("Invalid login return value %d in message."), msg.loginSession.ret);
		MessageBox(
			mainWindowHandle, mbStrTmp,
			TEXT("Login screen"), MB_ICONERROR);

		return -1;
	}

	return -1;
}

int userLogin(HWND hwnd)
{
	char			*tmpIpString;
	TCHAR			*tcip;
	sockMessageS		msg;

	msg.mtype = MTYPE_LOGIN;
	msg.login.pcNo = 1;
	GetWindowText(GetDlgItem(hwnd, 2), msg.login.username, 64);
	GetWindowText(GetDlgItem(hwnd, 3), msg.login.password, 64);
	GetWindowText(GetDlgItem(hwnd, 4), wipString, 15);

	if (msg.login.username[0] == NULL || msg.login.password[0] == NULL)
	{
		MessageBox(
			mainWindowHandle, TEXT("Both the username and password fields are required."),
			TEXT("Login screen"), MB_ICONERROR);

		return -1;
	};

	if (wipString[0] != NULL)
	{
		if (convertIpToUtf8(wipString, &tmpIpString) == 0)
		{
			if (ipIsValid(tmpIpString))
			{
				strcpy(ipString, tmpIpString);
			//	convertIpToUtf16(ipString, &tcip);
			//	tsprintf(mbStrTmp, TEXT("wipString is %s.\nipString, stored globally is %s"), wipString, tcip);
			//	MessageBox(mainWindowHandle, mbStrTmp, TEXT("debug"), 0);
			} else
			{
				MessageBox(mainWindowHandle, TEXT("Invalid IP address!\n\tFormat NNN.NNN.NNN.NNN"), TEXT("Login screen"), MB_ICONERROR);
				return -1;
			};
		}
		else {
			MessageBox(mainWindowHandle, TEXT("Failed to do a utf16->utf8 IP conversion."), TEXT("Login Screen"), MB_ICONERROR);
			return -1;
		};
	};

	if (!usernameIsValid(msg.login.username) || !passwordIsValid(msg.login.password)) {
		return -1;
	}

	if (connection.send(&msg) != 0) {
		return -1;
	};

	if (connection.recv(&msg) != 0) {
		return -1;
	};

	// Examine the message to see if the login was successful.
	switch (msg.login.ret)
	{
	case MSG_LOGIN_SUCCESS:
		return successfulLoginProc(hwnd, &msg);

	case MSG_LOGIN_USER_PASS_INVALID:
		MessageBox(mainWindowHandle,
			TEXT("Login failed!\nInvalid username or password!"),
			TEXT("Login screen"),
			MB_ICONERROR);

		return -1;

	default:
		tsprintf(mbStrTmp, TEXT("Invalid login return value %d in message."), msg.login.ret);
		MessageBox(
			mainWindowHandle, mbStrTmp,
			TEXT("Login screen"), MB_ICONERROR);

		return -1;
	};

	return -1;
}

int messaging::getRemainingTime(HWND hwnd)
{
	sockMessageS		msg;
	int			ret;

	msg.mtype = MTYPE_GET_CURRENT_TIME;
	msg.getCurrentTime.pcNo = 1;
	
	if (connection.send(&msg) != 0) {
		return -1;
	};

	if (connection.recv(&msg) != 0) {
		return -1;
	};

	if (msg.getCurrentTime.ret == MSG_GET_CURRTIME_SERVER_ERROR)
	{
		MessageBox(mainWindowHandle,
			TEXT("getCurrentTime: Unknown error occured at server end."),
			TEXT("Login Screen"), 0);

		return msg.getCurrentTime.ret;
	};

	return msg.getCurrentTime.ret;
}

void messaging::logout(HWND)
{
	sockMessageS		msg;

	msg.mtype = MTYPE_LOGOUT;
	msg.logout.pcNo = 1;

	if (connection.send(&msg) != 0) {
		return;
	};

	if (connection.recv(&msg) != 0) {
		return;
	};
}

int messaging::updateTime(HWND hwnd, uint32_t minutes)
{
	sockMessageS		msg;

	msg.mtype = MTYPE_UPDATE_TIME;
	msg.updateTime.pcNo = 1;
	msg.updateTime.minutes = minutes;

	if (connection.send(&msg) != 0) {
		return -1;
	};

	if (connection.recv(&msg) != 0) {
		return -1;
	};

	/* We should try to do something if the update failed, but
	 * I can't think of anything useful to show/print/alert with.
	 **/
	return 0;
}
