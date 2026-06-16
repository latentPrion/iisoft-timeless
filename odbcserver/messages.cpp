
#include "odbcserver.h"

static TCHAR		mbStrTmp[1024];
void messages::processLogin(int clientFd, sockMessageS *msg)
{
	int		ret;

	/*tsprintf(mbStrTmp, TEXT("Got login message: mtype %d, ret as received %d, pcNo %d, sessionId as received %d."),
		msg->mtype, msg->login.ret, msg->login.pcNo, msg->login.sessionId);

	MessageBox(
		NULL, mbStrTmp, TEXT("odbcserver"), 0);*/

	ret = odbcFuncs::userConfirm(msg);
	msg->login.ret = ((ret == MSG_LOGIN_SUCCESS)
		? MSG_LOGIN_SUCCESS : MSG_LOGIN_USER_PASS_INVALID);

	ret = send(clientFd, (char *)msg, sizeof(*msg), 0);
	if (ret == -1 || ret < sizeof(*msg))
	{
		MessageBox(NULL,
			TEXT("Failed to send login response."),
			TEXT("odbcserver"),
			MB_ICONERROR);
	};
}

void messages::processGetCurrentTime(int clientFd, sockMessageS *msg)
{
	int		ret;

	/*tsprintf(mbStrTmp, TEXT("Got getcurrenttime message: mtype %d, ret as received %d."),
		msg->mtype, msg->getCurrentTime.ret);

	MessageBox(
		NULL, mbStrTmp, TEXT("odbcserver"), 0); */

	ret = odbcFuncs::getCurrentTime(msg);
	if (ret == -1) { msg->getCurrentTime.ret = MSG_GET_CURRTIME_SERVER_ERROR; };
	if (ret == -2) { msg->getCurrentTime.ret = MSG_GET_CURRTIME_FORCE_CLOSED; };
	if (ret >= 0) { msg->getCurrentTime.ret =  ret; };

	ret = send(clientFd, (char *)msg, sizeof(*msg), 0);
	if (ret == -1 || ret < sizeof(*msg))
	{
		MessageBox(NULL, TEXT("Failed to send get current time response message."),
			TEXT("odbcserver"), MB_ICONERROR);
	};
}

void messages::processUpdateTime(int clientFd, sockMessageS *msg)
{
	int		ret;

	/*tsprintf(mbStrTmp, TEXT("Got updatetime message: mtype %d, pcNo %d, minutes %d."),
		msg->mtype, msg->updateTime.pcNo, msg->updateTime.minutes);

	MessageBox(
		NULL, mbStrTmp, TEXT("odbcserver"), 0); */

	ret = odbcFuncs::updateTime(msg);
	if (ret == -1) {
		msg->updateTime.ret = 1;
	} else {
		msg->updateTime.ret = ret;
	};

	ret = send(clientFd, (char *)msg, sizeof(*msg), 0);
	if (ret == -1 || ret < sizeof(*msg))
	{
		MessageBox(
			NULL, TEXT("Failed to send update time response message."),
			TEXT("odbcserver"),
			MB_ICONERROR);
	};
}

void messages::processLogout(int clientFd, sockMessageS *msg)
{
	int		ret;

	/*tsprintf(mbStrTmp, TEXT("Got logout message: mtype %d, ret as received %d."),
		msg->mtype, msg->logout.ret);

	MessageBox(
		NULL, mbStrTmp, TEXT("odbcserver"), 0);*/

	odbcFuncs::logout(msg);
	msg->logout.ret = 1;
	ret = send(clientFd, (char *)msg, sizeof(*msg), 0);
	if (ret == -1 || ret < sizeof(*msg))
	{
		MessageBox(
			NULL, TEXT("Failed to send logout message response."),
			TEXT("odbcserver"),
			MB_ICONERROR);
	};
}

void messages::processLoginSession(int clientFd, sockMessageS *msg)
{
	int		ret;

	/*tsprintf(mbStrTmp, TEXT("Got loginSession message: mtype %d, sessionId %d."),
		msg->mtype, msg->loginSession.sessionId);

	MessageBox(
		NULL, mbStrTmp, TEXT("odbcserver"), 0);*/

	ret = odbcFuncs::loginSession(msg);
	msg->loginSession.ret = ((ret == MSG_LOGIN_SUCCESS)
		? MSG_LOGIN_SUCCESS : MSG_LOGIN_USER_PASS_INVALID);

	ret = send(clientFd, (char *)msg, sizeof(*msg), 0);
	if (ret == -1 || ret < sizeof(*msg))
	{
		MessageBox(
			NULL, TEXT("Failed to send login-session response."),
			TEXT("odbcserver"),
			MB_ICONERROR);
	};
}
