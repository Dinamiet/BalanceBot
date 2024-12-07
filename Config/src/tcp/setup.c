#include "private.h"
#include "tcp.h"

int tcpSocket;

void TCP_Setup(const char* host)
{
	tcpSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (tcpSocket < 0)
	{
		printf("Error creating socket\n");
		exit(1);
	}

	struct sockaddr_in serverAddres;

	serverAddres.sin_family = AF_INET;
	serverAddres.sin_port   = htons(PORT);

	struct hostent* serverIP = gethostbyname(host);
	if (serverIP == NULL)
	{
		serverAddres.sin_addr.s_addr = inet_addr(host);
	}
	else
		memcpy(&serverAddres.sin_addr, serverIP->h_addr_list[0], serverIP->h_length);

	if (connect(tcpSocket, (struct sockaddr*)&serverAddres, sizeof(serverAddres)) < 0)
	{
		printf("Connection to %s failed\n", host);
		exit(2);
	}

	printf("Connected to %s on port %d\n", host, PORT);
}
