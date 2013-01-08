#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>
#include "Stream.h"
#include "../Memory.h"

OpenUtility::CBlockStream::CBlockStream(int TBlock)
{
	if (TBlock<2) throw(ERR_ARGUMENT);

	Stream=(char*)MyMalloc(TBlock*sizeof(char));
	taille=0;
	Stream[0]='\0';
	TailleBlock=TBlock;
	IncBlock=TBlock;
}

OpenUtility::CBlockStream::CBlockStream(OpenUtility::CBlockStream &S,int TBlock)
{
	char *temp=S.GetStream();

	if (TBlock<2) throw(ERR_ARGUMENT);

	taille=S.GetSize();
	TailleBlock=(((taille+1)/TBlock)+1)*TBlock;
	Stream=(char*)MyMalloc(TailleBlock*sizeof(char));

	memcpy(Stream,temp,sizeof(char)*taille);
	Stream[taille]='\0';
	IncBlock=TBlock;
}

OpenUtility::CBlockStream::CBlockStream(OpenUtility::CBlockStream &S)
{
	taille=S.taille;
	IncBlock=S.IncBlock;
	TailleBlock=S.TailleBlock;
	Stream=(char*)MyMalloc(TailleBlock*sizeof(char));

	memcpy(Stream,S.Stream,sizeof(char)*taille);
	Stream[taille]='\0';
}

OpenUtility::CBlockStream::CBlockStream(CStream &S,int TBlock)
{
	char *temp=S.GetStream();

	if (TBlock<2) throw(ERR_ARGUMENT);

	taille=S.GetSize();
	TailleBlock=(((taille+1)/TBlock)+1)*TBlock;
	Stream=(char*)MyMalloc(TailleBlock*sizeof(char));

	memcpy(Stream,temp,sizeof(char)*taille);
	Stream[taille]='\0';
	IncBlock=TBlock;
}

OpenUtility::CBlockStream::CBlockStream(const char *buffer,int TBlock)
{
	if (TBlock<2) throw(ERR_ARGUMENT);

	if (buffer==NULL) taille=0;
	else taille=(unsigned int)strlen(buffer);
	TailleBlock=(((taille+1)/TBlock)+1)*TBlock;
	Stream=(char*)MyMalloc(TailleBlock*sizeof(char));

	if (buffer) memcpy(Stream,buffer,sizeof(char)*taille);
	Stream[taille]='\0';
	IncBlock=TBlock;
}

OpenUtility::CBlockStream::CBlockStream(const char car,int TBlock)
{
	if (TBlock<2) throw(ERR_ARGUMENT);

	taille=1;
	TailleBlock=(((taille+1)/TBlock)+1)*TBlock;
	Stream=(char*)MyMalloc(TailleBlock*sizeof(char));

	Stream[0]=car;
	Stream[taille]='\0';
	IncBlock=TBlock;
}

OpenUtility::CBlockStream::~CBlockStream()
{
	MyFree(Stream);
}

unsigned int OpenUtility::CBlockStream::Hash(const OpenUtility::CBlockStream *str)
{
	return(HashStr(str->GetStream()));
}

char* OpenUtility::CBlockStream::GetStream() const
{
	return(Stream);
}

unsigned int OpenUtility::CBlockStream::GetSize()
{
	return(taille);
}

void OpenUtility::CBlockStream::AddStream(const char *buffer,int size)
{
	if (taille+size+1>TailleBlock)
	{
		TailleBlock=(((taille+size+1)/IncBlock)+1)*IncBlock;
		Stream=(char*)MyRealloc(Stream,TailleBlock*sizeof(char));
	}
	memcpy(&Stream[taille],buffer,sizeof(char)*size);
	Stream[taille+size]='\0';
	taille+=size;
}

void OpenUtility::CBlockStream::AddStream(char car)
{
	if (taille+2>TailleBlock)
	{
		TailleBlock+=IncBlock;
		Stream=(char*)MyRealloc(Stream,TailleBlock*sizeof(char));
	}
	Stream[taille]=car;
	Stream[taille+1]='\0';
	taille++;
}

OpenUtility::CBlockStream& OpenUtility::CBlockStream::operator+(const char* buffer)
{
	this->AddStream(buffer,(unsigned int)strlen(buffer));

	return(*this);
}

OpenUtility::CBlockStream& OpenUtility::CBlockStream::operator+(char* buffer)
{
	this->AddStream(buffer,(unsigned int)strlen(buffer));

	return(*this);
}

OpenUtility::CBlockStream& OpenUtility::CBlockStream::operator+(char car)
{
	this->AddStream(car);

	return(*this);
}

OpenUtility::CBlockStream& OpenUtility::CBlockStream::operator+(OpenUtility::CBlockStream &S)
{
	this->AddStream(S.GetStream(),S.GetSize());

	return(*this);
}

OpenUtility::CBlockStream& OpenUtility::CBlockStream::operator+(CStream &S)
{
	this->AddStream(S.GetStream(),S.GetSize());

	return(*this);
}

OpenUtility::CBlockStream& OpenUtility::CBlockStream::operator=(const char car)
{
	unsigned int newTailleBlock;

	if ((newTailleBlock=((1/IncBlock)+1)*IncBlock)!=TailleBlock)
	{
		TailleBlock=newTailleBlock;
		Stream=(char*)MyRealloc(Stream,TailleBlock*sizeof(char));
	}

	Stream[0]=car;
	Stream[1]='\0';
	taille=1;

	return(*this);
}

OpenUtility::CBlockStream& OpenUtility::CBlockStream::operator=(const char* buffer)
{
	int i=(int)strlen(buffer);
	unsigned int newTailleBlock;

	if ((newTailleBlock=(((i+1)/IncBlock)+1)*IncBlock)!=TailleBlock)
	{
		TailleBlock=newTailleBlock;
		Stream=(char*)MyRealloc(Stream,TailleBlock*sizeof(char));
	}

	if (i) memcpy(Stream,buffer,sizeof(char)*i);
	Stream[i]='\0';
	taille=i;

	return(*this);
}

OpenUtility::CBlockStream& OpenUtility::CBlockStream::operator=(OpenUtility::CBlockStream &S)
{
	unsigned int newTailleBlock;

	if (this==&S) return(*this);

	if ((newTailleBlock=(((S.GetSize()+1)/IncBlock)+1)*IncBlock)!=TailleBlock)
	{
		TailleBlock=newTailleBlock;
		Stream=(char*)MyRealloc(Stream,TailleBlock*sizeof(char));
	}

	if (S.GetSize()) memcpy(Stream,S.GetStream(),sizeof(char)*S.GetSize());
	Stream[S.GetSize()]='\0';
	taille=S.GetSize();

	return(*this);
}

OpenUtility::CBlockStream& OpenUtility::CBlockStream::operator=(CStream &S)
{
	unsigned int newTailleBlock;

	if ((newTailleBlock=(((S.GetSize()+1)/IncBlock)+1)*IncBlock)!=TailleBlock)
	{
		TailleBlock=newTailleBlock;
		Stream=(char*)MyRealloc(Stream,TailleBlock*sizeof(char));
	}

	if (S.GetSize()) memcpy(Stream,S.GetStream(),sizeof(char)*S.GetSize());
	Stream[S.GetSize()]='\0';
	taille=S.GetSize();

	return(*this);
}

OpenUtility::CBlockStream& OpenUtility::CBlockStream::operator+=(const char* buffer)
{
	return(operator +(buffer));
}

OpenUtility::CBlockStream& OpenUtility::CBlockStream::operator+=(char* buffer)
{
	return(operator +(buffer));
}

OpenUtility::CBlockStream& OpenUtility::CBlockStream::operator+=(char car)
{
	return(operator +(car));
}

OpenUtility::CBlockStream& OpenUtility::CBlockStream::operator+=(OpenUtility::CBlockStream &S)
{
	return(operator +(S));
}

OpenUtility::CBlockStream& OpenUtility::CBlockStream::operator+=(CStream &S)
{
	return(operator +(S));
}

bool OpenUtility::CBlockStream::operator==(OpenUtility::CBlockStream &S)
{
	unsigned int i=0;
	char *temp;

	if (S.taille!=taille) return(false);
	temp=S.GetStream();
	while (i<taille)
	{
		if (temp[i]!=Stream[i]) return(false);
		i++;
	}
	return(true);
}

bool OpenUtility::CBlockStream::operator==(CStream &S)
{
	unsigned int i=0;
	char *temp;

	if (taille!=S.GetSize()) return(false);
	temp=S.GetStream();
	while (i<taille)
	{
		if (temp[i]!=Stream[i]) return(false);
		i++;
	}
	return(true);
}

bool OpenUtility::CBlockStream::operator==(const char *Str)
{
	unsigned int i=0;

	if (strlen(Str)!=taille) return(false);
	while (i<taille)
	{
		if (Str[i]!=Stream[i]) return(false);
		i++;
	}
	return(true);
}

bool OpenUtility::CBlockStream::operator!=(OpenUtility::CBlockStream &S)
{
	return(!operator==(S));
}

bool OpenUtility::CBlockStream::operator!=(CStream &S)
{
	return(!operator==(S));
}

bool OpenUtility::CBlockStream::operator!=(const char *Str)
{
	return(!operator==(Str));
}

bool OpenUtility::CBlockStream::operator<(CStream &S)
{
	return(strcmp(Stream,S.Stream)<0);
}

bool OpenUtility::CBlockStream::operator<(OpenUtility::CBlockStream &S)
{
	return(strcmp(Stream,S.Stream)<0);
}

bool OpenUtility::CBlockStream::operator<(const char *Str)
{
	return(strcmp(Stream,Str)<0);
}

bool OpenUtility::CBlockStream::operator<(const char car)
{
	return(Stream[0]<car);
}

bool OpenUtility::CBlockStream::operator<=(CStream &S)
{
	return(strcmp(Stream,S.Stream)<=0);
}

bool OpenUtility::CBlockStream::operator<=(OpenUtility::CBlockStream &S)
{
	return(strcmp(Stream,S.Stream)<=0);
}

bool OpenUtility::CBlockStream::operator<=(const char *Str)
{
	return(strcmp(Stream,Str)<=0);
}

bool OpenUtility::CBlockStream::operator<=(const char car)
{
	return((Stream[0]<car) || ((taille==1) && (Stream[0]==car)));
}

bool OpenUtility::CBlockStream::operator>(CStream &S)
{
	return(strcmp(Stream,S.Stream)>0);
}

bool OpenUtility::CBlockStream::operator>(OpenUtility::CBlockStream &S)
{
	return(strcmp(Stream,S.Stream)>0);
}

bool OpenUtility::CBlockStream::operator>(const char *Str)
{
	return(strcmp(Stream,Str)>0);
}

bool OpenUtility::CBlockStream::operator>(const char car)
{
	return(Stream[0]>car);
}

bool OpenUtility::CBlockStream::operator>=(CStream &S)
{
	return(strcmp(Stream,S.Stream)>=0);
}

bool OpenUtility::CBlockStream::operator>=(OpenUtility::CBlockStream &S)
{
	return(strcmp(Stream,S.Stream)>=0);
}

bool OpenUtility::CBlockStream::operator>=(const char *Str)
{
	return(strcmp(Stream,Str)>=0);
}

bool OpenUtility::CBlockStream::operator>=(const char car)
{
	return((Stream[0]>car) || ((taille==1) && (Stream[0]==car)));
}

void OpenUtility::CBlockStream::Format(const char *StrFormat,...)
{
	va_list marker;
	int RealLen;

	va_start(marker,StrFormat);	// Initialise la variable arguments

	taille=NbCharFormat(StrFormat,marker);
	TailleBlock=(((taille+1)/IncBlock)+1)*IncBlock;
	Stream=(char*)MyRealloc(Stream,TailleBlock*sizeof(char));

	if ((RealLen=vsprintf(Stream,StrFormat,marker))>(int)taille)
	{
		GetCMyExceptionObj(E,ERR_PRGM);
		throw(E);
	}
	taille=RealLen;

	va_end(marker);	// Libère la variable arguments
}

void OpenUtility::CBlockStream::VFormat(const char *StrFormat,va_list argList)
{
	int RealLen;

	taille=NbCharFormat(StrFormat,argList);
	TailleBlock=(((taille+1)/IncBlock)+1)*IncBlock;
	Stream=(char*)MyRealloc(Stream,TailleBlock*sizeof(char));

	if ((RealLen=vsprintf(Stream,StrFormat,argList))>(int)taille)
	{
		GetCMyExceptionObj(E,ERR_PRGM);
		throw(E);
	}
	taille=RealLen;
}

void OpenUtility::CBlockStream::AddFormatStream(const char *StrFormat,...)
{
	va_list marker;
	int RealLen,tailletemp;

	va_start(marker,StrFormat);	// Initialise la variable arguments

	tailletemp=NbCharFormat(StrFormat,marker);
	TailleBlock=(((taille+tailletemp+1)/IncBlock)+1)*IncBlock;
	Stream=(char*)MyRealloc(Stream,TailleBlock*sizeof(char));

	if ((RealLen=vsprintf(&Stream[taille],StrFormat,marker))>(int)tailletemp)
	{
		GetCMyExceptionObj(E,ERR_PRGM);
		throw(E);
	}
	taille+=RealLen;

	va_end(marker);	// Libère la variable arguments
}

void OpenUtility::CBlockStream::AddVFormatStream(const char *StrFormat,va_list argList)
{
	int RealLen,tailletemp;

	tailletemp=NbCharFormat(StrFormat,argList);
	TailleBlock=(((taille+tailletemp+1)/IncBlock)+1)*IncBlock;
	Stream=(char*)MyRealloc(Stream,TailleBlock*sizeof(char));

	if ((RealLen=vsprintf(&Stream[taille],StrFormat,argList))>(int)tailletemp)
	{
		GetCMyExceptionObj(E,ERR_PRGM);
		throw(E);
	}
	taille+=RealLen;
}

void OpenUtility::CBlockStream::ToLower()
{
	for (unsigned int i=0;i<taille;i++) Stream[i]=tolower(Stream[i]);
}

void OpenUtility::CBlockStream::ToUpper()
{
	for (unsigned int i=0;i<taille;i++) Stream[i]=toupper(Stream[i]);
}

void OpenUtility::CBlockStream::RemoveCar(int nb)
{
	if (nb>(int)taille) taille=0;
	else taille-=nb;
	if (TailleBlock-IncBlock>taille)
	{
		TailleBlock=IncBlock*(((taille+1)/IncBlock)+1);
		Stream=(char*)MyRealloc(Stream,TailleBlock*sizeof(char));
	}
	Stream[taille]='\0';
}

void OpenUtility::CBlockStream::CalcNewSize()
{
	unsigned int newTailleBlock;

	if ((newTailleBlock=(((taille+1)/IncBlock)+1)*IncBlock)!=TailleBlock)
	{
		TailleBlock=newTailleBlock;
		Stream=(char*)MyRealloc(Stream,TailleBlock*sizeof(char));
	}
}

int OpenUtility::CBlockStream::ReplaceOne(char *StrFind,char *StrReplace,unsigned int pos)
{
	int i,j,k,l;
	unsigned int SauvTailleBlock,SauvTaille;

	if ((i=FindString(StrFind,pos))!=-1)
	{
		j=(int)strlen(StrFind);
		k=(int)strlen(StrReplace);
		if (j!=k)
		{
			SauvTaille=taille;
			taille+=k-j;
			SauvTailleBlock=TailleBlock;
			TailleBlock=(((taille+1)/IncBlock)+1)*IncBlock;
			if (SauvTailleBlock!=TailleBlock) Stream=(char*)MyRealloc(Stream,TailleBlock*sizeof(char));
			memmove(&Stream[i+k],&Stream[i+j],SauvTaille-i-j+1);
		}
		for(l=0;l<k;l++) Stream[i+l]=StrReplace[l];
	}
	return(i);
}

void OpenUtility::CBlockStream::ReplaceFirst(char *StrFind,char *StrReplace)
{
	ReplaceOne(StrFind,StrReplace,0);
}

void OpenUtility::CBlockStream::ReplaceAll(char *StrFind,char *StrReplace)
{
	int i,j;

	i=0;
	j=(int)strlen(StrReplace);
	try
	{
		while ((i=ReplaceOne(StrFind,StrReplace,i))!=-1) i+=j;
	}
	catch (CMyException E)
	{
	}
}
