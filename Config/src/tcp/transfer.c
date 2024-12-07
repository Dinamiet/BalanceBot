#include "private.h"
#include "tcp.h"

size_t TCP_Read(void* data, const size_t size)
{
	ssize_t received = recv(tcpSocket, data, size, 0);
	if (received < 0)
		return 0;

	return received;
}

size_t TCP_Write(const void* data, const size_t size)
{
	ssize_t sent = send(tcpSocket, data, size, 0);
	if (sent < 0)
		return 0;

	return sent;
}
