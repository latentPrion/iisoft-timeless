
#include "timeless.h"


char		hostname[256];
hostent		*serverDesc;
static TCHAR		mbStrTmp[2024];

connectionC::connectionC(void)
{
	memset(&sock, 0, sizeof(sock));
	sockfd = -1;
	connected = 0;
}

connectionC::~connectionC(void)
{
}

int connectionC::testAndConnect(const char *hostip, uint16_t port)
{
	//int		ret;

	//strcpy(hostname, host);

	/*serverDesc = gethostbyname(hostname);
	if (serverDesc == NULL)
	{
		MessageBox(NULL, TEXT("DNS lookup of host failed."), TEXT("connection"), 0);
		//return -1;
	};*/

	if (connected) { return 0; };

	sock.sin_family = AF_INET;
	sock.sin_port = htons(port);
	sock.sin_addr.s_addr = inet_addr(hostip);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		MessageBox(
			((mainWindowHandle == NULL) ? NULL : mainWindowHandle),
			TEXT("Couldn't allocate socket"), TEXT("connection"),
			MB_ICONERROR);

		return sockfd;
	};

	if (connect(sockfd, (sockaddr *)&sock, sizeof(sock)) == -1)
	{
		MessageBox(((mainWindowHandle == NULL) ? NULL : mainWindowHandle),
			TEXT("Connect() failed."), TEXT("connection"),
			MB_ICONERROR);

		connected = 0;
		return -1;
	}
	else {
		connected = 1;
	};

	return 0;
}

int connectionC::send(sockMessageS *msg)
{
	size_t		ret;

	if (testAndConnect(ipString, 32769) != 0) {
		return -1;
	};

	ret = ::send(sockfd, (const char *)msg, sizeof(*msg), 0);
	if (ret == -1 || ret < sizeof(msg))
	{
		MessageBox(
			mainWindowHandle,
			TEXT("send(): Failed to send message"),
			TEXT("Login screen"),
			MB_ICONERROR);

		return -1;
	};

	return 0;
}

int connectionC::recv(sockMessageS *msg)
{
	int		ret;

	if (testAndConnect(ipString, 32769) != 0) {
		return -1;
	};

	ret = ::recv(sockfd, (char *)msg, sizeof(*msg), 0);
	if (ret == -1 || ret < sizeof(msg))
	{
		MessageBox(
			mainWindowHandle,
			TEXT("recv(): Failed to get response message"),
			TEXT("Login screen"),
			MB_ICONERROR);

		return -1;
	};

	return 0;
}
