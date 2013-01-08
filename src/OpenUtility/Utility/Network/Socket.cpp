#include "Socket.h"
#include "../Memory.h"
#ifndef WIN32
	#include <unistd.h>
	#include <netdb.h>
	#include <string.h>
	#include <arpa/inet.h>
#endif

void OpenUtility::InitSocket()
{
#ifdef WIN32
	static bool isInit=false;

	if (!isInit)
	{
		isInit=true;

		WSADATA wsaData;
		// Initialisation des sockets
		WORD wVersionRequested=MAKEWORD(1,1);
		int nResult=WSAStartup(wVersionRequested,&wsaData);
		if (nResult!=0)
		{
			OpenUtility::CMyException E(ERR_BAD_INIT);
			throw(E);
		}
		else
		{
			if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
			{
				WSACleanup();
				OpenUtility::CMyException E(ERR_BAD_INIT);
				throw(E);
			}
		}
	}
#endif
}

SOCKET OpenUtility::OpenClientTcpSocket(char *host,unsigned short port)
{
	struct sockaddr_in name;
	struct hostent *hptr;
	SOCKET sd;

	OpenUtility::InitSocket();

	memset(&name,0,sizeof(name));
	name.sin_family=AF_INET;
	name.sin_port=htons(port);

	if ((hptr=gethostbyname(host))==NULL)
	{
		return(0);
	}

	/* copy the IP address of host into name */
	//bcopy(hptr->h_addr,&name.sin_addr.s_addr,hptr->h_length);
	memcpy(&name.sin_addr.s_addr,hptr->h_addr,hptr->h_length);

	if ((sd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))<0)
	{
		return(0);
	}

	if (connect(sd,(sockaddr*)(&name),sizeof(name))<0)
	{
		CLOSE_SOCKET(sd);
		return(0);
	}

	return sd;
}

SOCKET OpenUtility::OpenServerTcpSocket(unsigned short port)
{
	SOCKET sd;	// file descriptor of the socket
	struct sockaddr_in addr;	// address (local port)

	OpenUtility::InitSocket();

	// set the address to a local port (variable `port')
	memset(&addr,0,sizeof(addr));
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr=INADDR_ANY;
	addr.sin_port=htons(port);

	// create the socket
	if ((sd=socket(PF_INET,SOCK_STREAM,0))!=INVALID_SOCKET)
	{
		// bind the socket to the local port
		if (!bind(sd,(struct sockaddr*)&addr,sizeof(addr)))
		{
			// listen to the socket
			if (!listen(sd,SOMAXCONN)) return(sd);
		}

		// closes the server socket
		CLOSE_SOCKET(sd);
	}
	return(0);
}

bool OpenUtility::WOL(const char *macAddr,const char *bcastAddr)
{
	SOCKET sd;	// file descriptor of the socket
	struct sockaddr_in name;
	char optval;
	struct hostent *hptr;
	const int len=17*6;
	unsigned char buf[len];
	unsigned char mac[6];
	int i;

	OpenUtility::InitSocket();

	for (i=0;*macAddr!='\0' && i<6*3;macAddr++)
	{
		if (i%3==0) mac[i/3]=0;
		if (i%3==2)
		{
			if ((*macAddr!=':') && (*macAddr!='-')) return(false);
		}
		else
		{
			if ((*macAddr>='0') && (*macAddr<='9')) mac[i/3]|=(*macAddr-'0')<<((1-(i%3))*4);
			else if ((*macAddr>='a') && (*macAddr<='f')) mac[i/3]|=(*macAddr-'a'+10)<<((1-(i%3))*4);
			else if ((*macAddr>='A') && (*macAddr<='F')) mac[i/3]|=(*macAddr-'A'+10)<<((1-(i%3))*4);
			else return(false);
		}
		i++;
	}
	if (*macAddr!='\0') return(false);

	for (i=0;i<6;i++) buf[i]=0xff;
	for (i=0;i<16;i++) memcpy(buf+6+i*6,mac,6);

	memset(&name,0,sizeof(name));
	name.sin_family=AF_INET;
	name.sin_port=htons(9);
	if ((hptr=gethostbyname(bcastAddr))!=NULL)
	{
		memcpy(&name.sin_addr.s_addr,hptr->h_addr,hptr->h_length);
		name.sin_family=hptr->h_addrtype;
	}
	else if ((name.sin_addr.s_addr=inet_addr(bcastAddr))!=INADDR_NONE)
	{
		name.sin_family=AF_INET;
	}
	else return(false);

	if ((sd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP))<0)
	{
		return(false);
	}

	optval=1;
	if (setsockopt(sd,SOL_SOCKET,SO_BROADCAST,&optval,sizeof(optval))<0)
	{
		return(false);
	}

	if (sendto(sd,(char*)buf,len,0,(sockaddr*)&name,sizeof(name))!=len)
	{
		return(false);
	}

	return(true);
}
