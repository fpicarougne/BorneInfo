//coding: utf-8
#include "MultiThread.h"
#include <stdio.h>
#ifndef WIN32
	#include <sys/time.h>

	struct SemStruct
	{
		SemStruct(int initial,int maximum) : max(maximum),cur(initial),nbBlock(0) {pthread_cond_init(&cond,NULL);pthread_mutex_init(&mut,NULL);}
		~SemStruct() {pthread_cond_destroy(&cond);pthread_mutex_destroy(&mut);}
		pthread_mutex_t mut;
		pthread_cond_t cond;
		int max;
		int cur;
		int nbBlock;
	};
#endif

MUTEX OpenUtility::InitMutex()
{
#ifdef WIN32
	#error not done
#else
	MUTEX m=PTHREAD_MUTEX_INITIALIZER;
	return(m);
#endif
}

void OpenUtility::MutexLock(MUTEX &m)
{
#ifdef WIN32
	#error not done
#else
	pthread_mutex_lock(&m);
#endif
}

void OpenUtility::MutexUnlock(MUTEX &m)
{
#ifdef WIN32
	#error not done
#else
	pthread_mutex_unlock(&m);
#endif
}

bool OpenUtility::DestroyMutex(MUTEX &m)
{
#ifdef WIN32
	#error not done
#else
	return(pthread_mutex_destroy(&m)!=-1);
#endif
}

SEMAPHORE OpenUtility::InitSem(int initial,int maximum)
{
#ifdef WIN32
	return(CreateSemaphore(NULL,initial,maximum,NULL));
#else
	return(new SemStruct(initial,maximum));
#endif
}

int OpenUtility::WaitSem(SEMAPHORE &sem,int msTimeout)
{
#ifdef WIN32
	return(WaitForSingleObject(sem,msTimeout));
#else
	int retcode=0;
	struct timeval now;
	struct timespec timeout;
	SemStruct *sema=(SemStruct*)sem;
//	long t;

	gettimeofday(&now,NULL);
	timeout.tv_sec=now.tv_sec+msTimeout/1000;
	timeout.tv_nsec=now.tv_usec*1000+(msTimeout%1000)*1000000;
	if (timeout.tv_nsec>1000000000) {timeout.tv_nsec%=1000000000;timeout.tv_sec++;}
//	t=now.tv_usec*1000+(msTimeout%1000)*1000000;
//	printf("t=%ld\ntimeout : %d'%ld\n",t,timeout.tv_sec,timeout.tv_nsec);
//	printf("now     : %d'%ld\n",now.tv_sec,now.tv_usec);
	pthread_mutex_lock(&sema->mut);
	while ((!sema->cur) && (retcode!=ETIMEDOUT))
	{
		sema->nbBlock++;
		if (msTimeout==int(INFINITE)) retcode=pthread_cond_wait(&sema->cond,&sema->mut);
		else retcode=pthread_cond_timedwait(&sema->cond,&sema->mut,&timeout);
		sema->nbBlock--;
	}
	gettimeofday(&now,NULL);
//	printf("now     : %d'%ld\n\n",now.tv_sec,now.tv_usec);
	if (retcode!=ETIMEDOUT) sema->cur--;
	pthread_mutex_unlock(&sema->mut);

	return(retcode);
#endif
}

bool OpenUtility::SignalSem(SEMAPHORE &sem,int nb)
{
#ifdef WIN32
	return(ReleaseSemaphore(sem,nb,NULL)!=0);
#else
	SemStruct *sema=(SemStruct*)sem;

	pthread_mutex_lock(&sema->mut);
	if (sema->cur<sema->max) sema->cur++;
	pthread_cond_signal(&sema->cond);
	pthread_mutex_unlock(&sema->mut);

	return(true);
#endif
}

bool OpenUtility::DestroySem(SEMAPHORE &sem)
{
#ifdef WIN32
	return(CloseHandle(sem)!=0);
#else
	delete (SemStruct*)sem;
	return(true);
#endif
	sem=0;
}

bool OpenUtility::CreateThread(THREAD_CALLBACK call_back,void *param,THREAD_ID *thread_id)
{
	if (thread_id)
	{
#ifdef WIN32
		*thread_id=::CreateThread(NULL,0,call_back,(void*)param,0,NULL);
		return((*thread_id)!=NULL);
#else
		return(pthread_create(thread_id,NULL,call_back,param)==0);
#endif
	}
	else
	{
#ifdef WIN32
		return(::CreateThread(NULL,0,call_back,(void*)param,0,NULL)!=NULL);
#else
		THREAD_ID id;
		return(pthread_create(&id,NULL,call_back,param)==0);
#endif
	}
}

bool OpenUtility::WaitJoinThread(THREAD_ID thread_id)
{
#ifdef WIN32
	return(WaitForSingleObject(thread_id,INFINITE)==WAIT_OBJECT_0);
#else
	return(pthread_join(thread_id,NULL)>=0);
#endif
}

void OpenUtility::SleepThread(unsigned int msec)
{
#ifdef WIN32
	Sleep(msec);
#else
	usleep(msec*1000);
#endif
}
