//coding: utf-8
#ifndef MultiThread_h
	#define MultiThread_h

#ifndef INFINITE
	#define INFINITE 0xFFFFFFFF
#endif

#ifdef WIN32
	#include <windows.h>
	#include <process.h>
	#define SEMAPHORE HANDLE
	#define THREAD_ID HANDLE
	#define THREAD_CALLBACK LPTHREAD_START_ROUTINE
	#define TH_CALL_BACK_PROTO(Name,arg) static DWORD __stdcall Name(LPVOID arg)
	#define TH_CALL_BACK(Class,Name,arg) DWORD Class::Name(LPVOID arg)
	#define TH_CALL_BACK_FN(Name,arg) DWORD Name(LPVOID arg)
#else
	#include <pthread.h>
	#include <unistd.h>
	#include <sys/types.h>
	#include <errno.h>
	#define MUTEX pthread_mutex_t
	#define SEMAPHORE void*
	#define THREAD_ID pthread_t
	typedef void* (*start_routine)(void *);
	#define THREAD_CALLBACK start_routine
	#define WAIT_TIMEOUT ETIMEDOUT
	#define TH_CALL_BACK_PROTO(Name,arg) static void* Name(void *arg)
	#define TH_CALL_BACK(Class,Name,arg) void* Class::Name(void *arg)
	#define TH_CALL_BACK_FN(Name,arg) void* Name(void *arg)
#endif

namespace OpenUtility
{

MUTEX InitMutex();
void MutexLock(MUTEX &m);
void MutexUnlock(MUTEX &m);
bool DestroyMutex(MUTEX &m);
SEMAPHORE InitSem(int initial,int maximum);
int WaitSem(SEMAPHORE &sem,int msTimeout=INFINITE);
bool SignalSem(SEMAPHORE &sem,int nb=1);
bool DestroySem(SEMAPHORE &sem);
bool CreateThread(THREAD_CALLBACK call_back,void *param,THREAD_ID *thread_id=NULL);
bool WaitJoinThread(THREAD_ID thread_id);
void SleepThread(unsigned int msec);

}

#endif
