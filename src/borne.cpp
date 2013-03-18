//coding: utf-8
#include <bcm_host.h>
#include <linux/limits.h>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdarg>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/vt.h>
#include <sys/wait.h>
#include <signal.h>
#include <limits.h>
#include <termios.h>
#include "borne.h"
#include "Client.h"

#define FILE_LOG "borne.log"

struct SParam
{
	char name[];
	int nbParam;
};

struct SEnv
{
	SEnv() : termModified(false),debug(false),restartOnError(true) {}
	bool termModified;
	struct termios terminal;
	bool debug;
	bool restartOnError;
	inline friend std::ostream& operator<<(std::ostream &s,const SEnv &obj)
	{
		s << "Paramètres d'environnement :" << std::endl;
		s << "\tDebug: " << (obj.debug ? "true" : "false") << std::endl;
		s << "\tRestart on error: " << (obj.restartOnError ? "true" : "false") << std::endl;
		return(s);
	}
};

void InitEnv(int argc,char *argv[]);
void ClearTerminal();
void UninitEnv();
void StartClient();
void sigint_handler(int sig);

SEnv sEnv;

int main(int argc,char *argv[])
{
	pid_t pid;
	int fd[2],status;
	int nb=0;
	char c;

	InitEnv(argc,argv);

	if (sEnv.debug) StartClient();
	else
	{
		SwitchTTY();
		ClearTerminal();
		signal(SIGINT,sigint_handler);

		do
		{
			if (pipe(fd)<0) ExitError("Error creating pipe control");
			if ((pid=fork())<0) ExitError("Error creating child process");

			if (pid>0)
			{
				close(fd[OUT]);
				if (read(fd[IN],&c,1)!=1) PrintLog("error","\tErreur de l'application");
				else if (c=='q') PrintLog("stop","\tDemande d'arrêt de l'application");
				else PrintLog("stop-restart","\tPorcessus terminé avec un mauvais code (%d : '%c')",c,c);
				nb++;
				wait(&status);
			}
		}
		while ((pid>0) && (c!='q') && sEnv.restartOnError);

		if (pid==0)
		{
			signal(SIGINT,SIG_DFL);
			close(fd[IN]);
			PrintLog("start");
			StartClient();
			c='q';
			write(fd[OUT],&c,1);
			return(0);
		}

		UninitEnv();
	}
	
	return(0);
}

void sigint_handler(int sig)
{
	UninitEnv();
	signal(SIGINT,SIG_DFL);
	kill(getpid(),SIGINT);
}

void InitEnv(int argc,char *argv[])
{
	int size;
	char path[PATH_MAX+1];
	std::ostringstream str;
	unsigned char state;
	char c;
	int i,j;

	for (j=1,state=0;j<argc;j++)
	{
		c=' ';
		i=-1;
		do
		{
			switch(state)
			{
			case 0:	// looking for '-'
				if (c=='-') state=1;
				break;

			case 1:
				if (c=='-') state=10;
				else if (c==' ') state=0;
				else if (c=='s') sEnv.restartOnError=false;
				else if (c=='d') sEnv.debug=true;
				break;

			case 10:
				break;
			}
			i++;
		} while ((c=argv[j][i])!='\0');
	}

	str << "/proc/" << getpid() << "/exe";
	if ((size=readlink(str.str().c_str(),path,PATH_MAX))<0) ExitError("Not able to get the path of the exe");
	while ((size>0) && (path[size-1]!='/')) size--;
	if (size==0) ExitError("Not able to get the path of the exe");
	path[size]='\0';
	chdir(path);

	PrintLog("global start","\texe path : %s",path);
}

void ClearTerminal()
{
	struct termios tp;

	std::cout << "\e[2J" << "\e[1;1H" << "\e[?16c" << std::flush;

	if (tcgetattr(STDIN_FILENO,&tp)!=-1)
	{
		sEnv.terminal=tp;
		tp.c_lflag&=~ECHO;
		if (tcsetattr(STDIN_FILENO,TCSAFLUSH,&tp)!=-1) sEnv.termModified=true;
	}
}

void UninitEnv()
{
	std::cout << "\e[?c" << std::flush;

	if (sEnv.termModified)
	{
		fflush(stdin);
		tcflush(STDIN_FILENO,TCIOFLUSH);
		tcsetattr(STDIN_FILENO,TCSANOW,&sEnv.terminal);
		sEnv.termModified=false;
	}
}

void ExitError(const char *str)
{
	std::cerr << str << std::endl;
	UninitEnv();
	exit(-1);
}

void PrintLog(const char *name,const char *desc,...)
{
	FILE *fp;
	time_t aTime;
	struct tm *newtime;
	va_list marker;

	if ((fp=fopen(FILE_LOG,"a"))!=NULL)
	{
		time(&aTime);
		newtime=localtime(&aTime);

		std::fprintf(fp,"%d/%02d/%02d %02d:%02d:%02d - [",newtime->tm_year+1900,newtime->tm_mon,newtime->tm_mday,newtime->tm_hour,newtime->tm_min,newtime->tm_sec);
		for (int i=0;name[i]!='\0';i++) std::fputc(toupper(name[i]),fp);
		std::fputs("]\n",fp);
		if (desc && (desc[0]!='\0'))
		{
			va_start(marker,desc);	// Initialise la variable arguments
			std::vfprintf(fp,desc,marker);
			va_end(marker);	// Libère la variable arguments
			std::fputc('\n',fp);
		}
		std::fputc('\n',fp);
		fclose(fp);
	}
}

void SwitchTTY()
{
	if (geteuid()==0)
	{
		int fdtty;
		if ((fdtty=GetConsoleFd("/dev/tty1"))<0) return;
		if (ioctl(fdtty,VT_ACTIVATE,1)<0) return;
		if (ioctl(fdtty,VT_WAITACTIVE,1)<0) return;
		close(0);
		dup(fdtty);
		close(1);
		dup(fdtty);
		close(fdtty);

		uid_t uid;
		if ((uid=getuid())!=0)
		{
			setegid(getgid());
			seteuid(uid);
		}
	}
}

int OpenConsole(const char *name)
{
	int fd;

	if (((fd=open(name,O_RDWR))<0) && (errno == EACCES))
		if (((fd=open(name,O_RDONLY))<0) && (errno == EACCES))
			fd = open(name,O_WRONLY);
	if (fd<0) return -1;
	return fd;
}

int GetConsoleFd(const char* name)
{
	int fd;

	if (name) return OpenConsole(name);
	if ((fd=OpenConsole("/dev/tty"))>0) return fd;
	if ((fd=OpenConsole("/dev/tty0"))>0) return fd;
	if ((fd=OpenConsole("/dev/console"))>0) return fd;
	return -1;
}

void StartClient()
{
	bcm_host_init();

	Client client;
	client.Start();
}
