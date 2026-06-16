
#include "odbcserver.h"


#define ODBCSERVER_MAX_NCLIENTS		32

struct clientInfoS
{
	sockaddr_in	sock;
	int		sockLength;
};

clientInfoS	clients[ODBCSERVER_MAX_NCLIENTS];

static int findEmptyClientSlot(void)
{
	for (int i=0; i<ODBCSERVER_MAX_NCLIENTS; i++)
	{
		if (clients[i].sock.sin_addr.s_addr == 0) {
			return i;
		};
	};
	return -1;
}

void socketConnectionC::returnSafely(int retval)
{
	memset(clients, 0, sizeof(clients));
	if (sockfd != -1) {
		// Close socket here.
	};
}

socketConnectionC::socketConnectionC(void)
{
	sockfd = -1;
	ZeroMemory(&sock, sizeof(sock));
}

socketConnectionC::~socketConnectionC(void)
{
}

int socketConnectionC::listenOn(uint16_t port)
{
	memset(&sock, 0, sizeof(sock));

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		MessageBox(NULL, TEXT("Failed to allocate a sockfd"), TEXT("socket"), 0);
	};

	sock.sin_family = AF_INET;
	sock.sin_port = htons(port);
	sock.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockfd, (sockaddr *)&sock, sizeof(sock)) == -1)
	{
		MessageBox(NULL, TEXT("Failed to bind to stack and port"), TEXT("socket"), 0);
		returnSafely(-1);
	};

	if (listen(sockfd, 128) == -1)
	{
		MessageBox(NULL, TEXT("Failed to allocate listen queue"), TEXT("socket"), 0);
		returnSafely(-1);
	};

	return 0;
}
