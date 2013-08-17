#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>
#include "Stream.h"
#include "../Memory.h"
#include "../OUException.h"

OpenUtility::CStream::CStream(bool empty)
{
	if (empty) Stream=NULL;
	else
	{
		Stream=(char*)MyMalloc(sizeof(char));
		Stream[0]='\0';
	}
	taille=0;
}

OpenUtility::CStream::CStream(const OpenUtility::CStream &S)
{
	char *temp=S.GetStream();

	taille=S.GetSize();
	Stream=(char*)MyMalloc((taille+1)*sizeof(char));

	memcpy(Stream,temp,sizeof(char)*taille);
	Stream[taille]='\0';
}

OpenUtility::CStream::CStream(char car)
{
	taille=1;
	Stream=(char*)MyMalloc(2*sizeof(char));

	Stream[0]=car;
	Stream[taille]='\0';
}

OpenUtility::CStream::CStream(const char *buffer)
{
	if (buffer==NULL) taille=0;
	else taille=(unsigned int)strlen(buffer);
	Stream=(char*)MyMalloc((taille+1)*sizeof(char));

	if (buffer) memcpy(Stream,buffer,sizeof(char)*taille);
	Stream[taille]='\0';
}

unsigned int OpenUtility::CStream::Hash(const OpenUtility::CStream *str)
{
	return(HashStr(str->GetStream()));
}

OpenUtility::CStream::~CStream()
{
	MyFree(Stream);
}

char& OpenUtility::CStream::operator[](const int i)
{
	if ((i<-((int)taille)) || (i>((int)taille))) THROW(Exception,"Out of bounds error");

	// Attention l'indice peut �tre n�gatif, 
	// l'indexation se fait alors � partir de la fin de la chaine
	if (i>=0) return Stream[i];
	else return Stream[taille+i];
}

void OpenUtility::CStream::AddStream(const char *buffer,const int size)
{
	Stream=(char*)MyRealloc(Stream,(taille+size+1)*sizeof(char));
	memcpy(&Stream[taille],buffer,sizeof(char)*size);
	Stream[taille+size]='\0';
	taille+=size;
}

void OpenUtility::CStream::AddStream(const char car)
{
	Stream=(char*)MyRealloc(Stream,(taille+2)*sizeof(char));
	Stream[taille]=car;
	Stream[taille+1]='\0';
	taille++;
}

OpenUtility::CStream OpenUtility::CStream::operator+(const char *buffer) const
{
	CStream str(*this);
	str.AddStream(buffer,(unsigned int)strlen(buffer));
	return(str);
}

OpenUtility::CStream OpenUtility::CStream::operator+(const char car) const
{
	CStream str(*this);
	str.AddStream(car);
	return(str);
}

OpenUtility::CStream OpenUtility::CStream::operator+(const CGalStream &S) const
{
	CStream str(*this);
	str.AddStream(S.GetStream(),S.GetSize());
	return(str);
}

OpenUtility::CStream& OpenUtility::CStream::operator=(const char car)
{
	this->taille=0;
	this->AddStream(car);

	return(*this);
}

OpenUtility::CStream& OpenUtility::CStream::operator=(const char *buffer)
{
	this->taille=0;
	this->AddStream(buffer,(unsigned int)strlen(buffer));

	return(*this);
}

OpenUtility::CStream& OpenUtility::CStream::operator=(const OpenUtility::CStream &S)
{
	if (this==&S) return(*this);

	this->taille=0;
	this->AddStream(S.GetStream(),S.GetSize());

	return(*this);
}

OpenUtility::CStream& OpenUtility::CStream::operator=(const CBlockStream &S)
{
	this->taille=0;
	this->AddStream(S.Stream,S.taille);

	return(*this);
}

OpenUtility::CStream& OpenUtility::CStream::operator+=(const char *buffer)
{
	this->AddStream(buffer,(unsigned int)strlen(buffer));
	return(*this);
}

OpenUtility::CStream& OpenUtility::CStream::operator+=(const char car)
{
	this->AddStream(car);
	return(*this);
}

OpenUtility::CStream& OpenUtility::CStream::operator+=(const CGalStream &S)
{
	this->AddStream(S.GetStream(),S.GetSize());
	return(*this);
}

bool OpenUtility::CStream::operator==(const CGalStream &S) const
{
	unsigned int i=0;
	char *temp;

	if (S.GetSize()!=taille) return(false);
	temp=S.GetStream();
	while (i<taille)
	{
		if (temp[i]!=Stream[i]) return(false);
		i++;
	}
	return(true);
}

bool OpenUtility::CStream::operator==(const char *Str) const
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

bool OpenUtility::CStream::operator!=(const CGalStream &S) const
{
	return(!operator==(S));
}

bool OpenUtility::CStream::operator!=(const char *Str) const
{
	return(!operator==(Str));
}

bool OpenUtility::CStream::operator<(const CGalStream &S) const
{
	return(strcmp(Stream,S.GetStream())<0);
}

bool OpenUtility::CStream::operator<(const char *Str) const
{
	return(strcmp(Stream,Str)<0);
}

bool OpenUtility::CStream::operator<(const char car) const
{
	return(Stream[0]<car);
}

bool OpenUtility::CStream::operator<=(const CGalStream &S) const
{
	return(strcmp(Stream,S.GetStream())<=0);
}

bool OpenUtility::CStream::operator<=(const char *Str) const
{
	return(strcmp(Stream,Str)<=0);
}

bool OpenUtility::CStream::operator<=(const char car) const
{
	return((Stream[0]<car) || ((taille==1) && (Stream[0]==car)));
}

bool OpenUtility::CStream::operator>(const CGalStream &S) const
{
	return(strcmp(Stream,S.GetStream())>0);
}

bool OpenUtility::CStream::operator>(const char *Str) const
{
	return(strcmp(Stream,Str)>0);
}

bool OpenUtility::CStream::operator>(const char car) const
{
	return(Stream[0]>car);
}

bool OpenUtility::CStream::operator>=(const CGalStream &S) const
{
	return(strcmp(Stream,S.GetStream())>=0);
}

bool OpenUtility::CStream::operator>=(const char *Str) const
{
	return(strcmp(Stream,Str)>=0);
}

bool OpenUtility::CStream::operator>=(const char car) const
{
	return((Stream[0]>car) || ((taille==1) && (Stream[0]==car)));
}

OpenUtility::CStream OpenUtility::operator+(OpenUtility::CStream &S1,OpenUtility::CStream &S2)
{
	OpenUtility::CStream res(S1);

	res+=S2;
	return res;
}

OpenUtility::CStream OpenUtility::operator+(OpenUtility::CStream &S,const char *buffer)
{
	OpenUtility::CStream res(S);

	res+=buffer;
	return res;
}

OpenUtility::CStream OpenUtility::operator+(const char *buffer,OpenUtility::CStream &S)
{
	OpenUtility::CStream res(buffer);

	res+=S;
	return res;
}

OpenUtility::CStream OpenUtility::operator+(OpenUtility::CStream &S,char car)
{
	OpenUtility::CStream res(S);

	res.AddStream(car);
	return res;
}

OpenUtility::CStream OpenUtility::operator+(char car,OpenUtility::CStream &S)
{
	OpenUtility::CStream res(car);

	res+=S;
	return res;
}

/*
ostream& operator<<(ostream &ar,OpenUtility::CStream &S)
{
	ar.write(S.Stream,S.taille);
	return ar;
}

istream& operator>>(istream &ar,OpenUtility::CStream &S)
{
	char c;

	// Vide la chaine
	S.Stream="";

	// Lit les espaces avant le d�but du flot d'entr�e
	do ar.get(c);
	while ((c==' ') || (c=='\t') || (c=='\n'));

	// Lit et place dans la chaine tous les caract�res du flot d'entr�e
	// jusqu'� un signe de s�paration
	while ((c!=' ') && (c!='\t') && (c!='\n'))
	{
		S.AddStream(c);
		ar.get(c);
	}
	return ar;
}
*/

OpenUtility::CStream& operator<<(OpenUtility::CStream &stream,char *str)
{
	stream+=str;
	return(stream);
}

void OpenUtility::CStream::Format(const char *StrFormat,...)
{
	va_list marker;
	int RealLen;

	va_start(marker,StrFormat);	// Initialise la variable arguments

	taille=NbCharFormat(StrFormat,marker);
	Stream=(char*)MyRealloc(Stream,(taille+1)*sizeof(char));

	if ((RealLen=vsprintf(Stream,StrFormat,marker))>(int)taille) THROW(Exception,"Implementation problem, contact OpenUtility developpers");
	taille=RealLen;

	va_end(marker);	// Lib�re la variable arguments
}

void OpenUtility::CStream::VFormat(const char *StrFormat,va_list argList)
{
	int RealLen;

	taille=NbCharFormat(StrFormat,argList);
	Stream=(char*)MyRealloc(Stream,(taille+1)*sizeof(char));

	if ((RealLen=vsprintf(Stream,StrFormat,argList))>(int)taille) THROW(Exception,"Implementation problem, contact OpenUtility developpers");
	taille=RealLen;
}

void OpenUtility::CStream::AddFormatStream(const char *StrFormat,...)
{
	va_list marker;
	int RealLen,tailletemp;

	va_start(marker,StrFormat);	// Initialise la variable arguments

	tailletemp=NbCharFormat(StrFormat,marker);
	Stream=(char*)MyRealloc(Stream,(taille+tailletemp+1)*sizeof(char));

	if ((RealLen=vsprintf(&Stream[taille],StrFormat,marker))>(int)tailletemp) THROW(Exception,"Implementation problem, contact OpenUtility developpers");
	taille+=RealLen;

	va_end(marker);	// Lib�re la variable arguments
}

void OpenUtility::CStream::AddVFormatStream(const char *StrFormat,va_list argList)
{
	int RealLen,tailletemp;

	tailletemp=NbCharFormat(StrFormat,argList);
	Stream=(char*)MyRealloc(Stream,(taille+tailletemp+1)*sizeof(char));

	if ((RealLen=vsprintf(&Stream[taille],StrFormat,argList))>(int)tailletemp) THROW(Exception,"Implementation problem, contact OpenUtility developpers");
	taille+=RealLen;
}

void OpenUtility::CStream::ToLower()
{
	for (unsigned int i=0;i<taille;i++) Stream[i]=tolower(Stream[i]);
}

void OpenUtility::CStream::ToUpper()
{
	for (unsigned int i=0;i<taille;i++) Stream[i]=toupper(Stream[i]);
}

void OpenUtility::CStream::RemoveCar(int nb)
{
	if (nb>(int)taille) taille=0;
	else taille-=nb;
	Stream=(char*)MyRealloc(Stream,(taille+1)*sizeof(char));
	Stream[taille]='\0';
}

void OpenUtility::CStream::CalcNewSize()
{
	Stream=(char*)MyRealloc(Stream,(taille+1)*sizeof(char));
}

void OpenUtility::CStream::AllocBuffer(unsigned int size)
{
	taille=size;
	CalcNewSize();
	Stream[taille]='\0';
}
