#ifndef Socket_h
	#define Socket_h

#ifdef WIN32
	#include <windows.h>
	typedef int socklen_t;
	#define CLOSE_SOCKET(sd) closesocket(sd)
	#define READ_SOCKET(sd,buf,size) recv(sd,buf,size,0)
	#define WRITE_SOCKET(sd,buf,size) send(sd,buf,size,0)
#else
	#include <sys/socket.h>
	#include <netinet/in.h>
	#define SOCKET int
	#define CLOSE_SOCKET(sd) close(sd)
	#define READ_SOCKET(sd,buf,size) read(sd,buf,size)
	#define WRITE_SOCKET(sd,buf,size) write(sd,buf,size)
	#define SOCKET_ERROR (-1)
	#define INVALID_SOCKET (SOCKET)(~0)
#endif

namespace OpenUtility
{

void InitSocket();
SOCKET OpenClientTcpSocket(char *host,unsigned short port);
SOCKET OpenServerTcpSocket(unsigned short port);
bool WOL(const char *bcastAddr,const char *macAddr="255.255.255.255");

}

#endif
