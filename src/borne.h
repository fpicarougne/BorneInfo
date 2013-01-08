//coding: utf-8
#ifndef _borne_h
	#define _borne_h

#define IN 0
#define OUT 1

void PrintLog(const char *name,const char *desc=NULL,...);
void ExitError(const char *str);
void SwitchTTY();
int OpenConsole(const char *name);
int GetConsoleFd(const char* name=NULL);

#endif
