#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>
#include "Stream.h"
#include "../Memory.h"

#ifndef max
	#define max(a,b)	((a>b) ? a : b)
#endif
#ifndef min
	#define min(a,b)	((a<b) ? a : b)
#endif

OpenUtility::CGalStream::CGalStream()
{
}

OpenUtility::CGalStream::~CGalStream()
{
}

OpenUtility::CGalStream::operator char* () const
{
	return(Stream);
}

unsigned int OpenUtility::CGalStream::HashStr(const char **key)
{
	return(HashStr(*key));
}

unsigned int OpenUtility::CGalStream::HashStr(const char *key)
{
	unsigned int nHash=0;

	while (*key) nHash=(nHash<<5)+nHash+*key++;

	return nHash;
}

void OpenUtility::CGalStream::ReplaceLastCar(char c)
{
	if (taille>0) Stream[taille-1]=c;
}

void OpenUtility::CGalStream::GalTrimLeft()
{
	unsigned int i=0;

	if (taille>0)
	{
		while ((i<=taille) && (Stream[i]==' ')) i++;
		if (i)
		{
			taille-=i;
			memmove(Stream,&(Stream[i]),taille+1);
		}
	}
}

void OpenUtility::CGalStream::GalTrimRight()
{
	unsigned int i=0;

	if (taille>0)
	{
		while ((i<=taille) && (Stream[taille-i-1]==' ')) i++;
		if (i)
		{
			taille-=i;
			Stream[taille]='\0';
		}
	}
}

int OpenUtility::CGalStream::FindString(char *Str,int Start,int End)
{
	int IndStream,IndStr;
	bool fini;
	int i;

	if (End==STR_END) End=taille;

	if ((Str==NULL) || (End>(int)taille) || (Start>End) || (Start<0))
	{
		GetCMyExceptionObj(E,ERR_ARGUMENT);
		throw(E);
	}

	IndStream=Start;
	IndStr=0;

	while ((IndStream<End) && (Str[IndStr]!='\0'))
	{
		if (tolower(Str[IndStr])==tolower(Stream[IndStream]))
			IndStr++;
		else
		{
			fini=false;
			while ((IndStr>0) && (!fini))
			{
				i=0;
				while ((!fini) && (i<IndStr))
				{
					if (Str[IndStr-i-1]!=Stream[IndStream-i]) fini=true;
					else i++;
				}
				fini=!fini;
				if (!fini) IndStr--;
			}
		}
		IndStream++;
	}

	if (Str[IndStr]=='\0') return(IndStream-(int)strlen(Str));
	else return(-1);
}

int OpenUtility::CGalStream::NbCharFormat(const char *StrFormat,va_list argList)
{
	int nbChar;

	nbChar=0;
	for(char *PtrStr=const_cast<char*>(StrFormat);*PtrStr!='\0';PtrStr++)
	{
		// handle '%' character, but watch out for '%%'
		if ((*PtrStr!='%') || (*(++PtrStr)=='%'))
		{
			nbChar++;
			continue;
		}

		int nItemLen=0;

		// handle '%' character with format
		int nWidth=0;
		for (;*PtrStr!='\0';PtrStr++)
		{
			// check for valid flags
			if (*PtrStr=='#') nbChar+=2;	// for '0x'
			else if (*PtrStr=='*') nWidth=va_arg(argList,int);
				else if ((*PtrStr=='-') || (*PtrStr=='+') || (*PtrStr=='0') || (*PtrStr==' '));
					else break;	// hit non-flag character
		}
		// get width and skip it
		if (nWidth==0)
		{
			// width indicated by
			nWidth=atoi(PtrStr);
			for (;(*PtrStr!='\0') && (isdigit(*PtrStr));PtrStr++);
		}
		if (nWidth<0)
		{
			GetCMyExceptionObj(E,ERR_PRGM);
			throw(E);
		}

		int nPrecision=0;
		if (*PtrStr=='.')
		{
			// skip past '.' separator (width.precision)
			PtrStr++;

			// get precision and skip it
			if (*PtrStr=='*')
			{
				nPrecision=va_arg(argList,int);
				PtrStr++;
			}
			else
			{
				nPrecision=atoi(PtrStr);
				for (;(*PtrStr!='\0') && (isdigit(*PtrStr));PtrStr++);
			}
			if (nPrecision<0)
			{
				GetCMyExceptionObj(E,ERR_PRGM);
				throw(E);
			}
		}

		// should be on type modifier or specifier
		if (strncmp(PtrStr,"I64",3)==0)
		{
			PtrStr+=3;
		}
		else
		{
			switch (*PtrStr)
			{
			// modifiers that affect size
			case 'h':
				PtrStr++;
				break;
			case 'l':
				PtrStr++;
				break;

			// modifiers that do not affect size
			case 'F':
			case 'N':
			case 'L':
				PtrStr++;
				break;
			}
		}

		// now should be on specifier
		switch (*PtrStr)
		{
		// single characters
		case 'c':
		case 'C':
			nItemLen=2;
			va_arg(argList,int);

		// strings
		case 's':
		case 'S':
			{
				char *PtrStrNextArg = va_arg(argList,char*);
				if (PtrStrNextArg==NULL) nItemLen=6;	// "(null)"
				else
				{
					nItemLen=(int)strlen(PtrStrNextArg);
					nItemLen=max(1,nItemLen);
				}
			}
			break;
		}

		// adjust nItemLen for strings
		if (nItemLen!=0)
		{
			if (nPrecision!=0) nItemLen=min(nItemLen,nPrecision);
			nItemLen=max(nItemLen,nWidth);
		}
		else
		{
			switch (*PtrStr)
			{
			// integers
			case 'd':
			case 'i':
			case 'u':
			case 'x':
			case 'X':
			case 'o':
				va_arg(argList,int);
				nItemLen=32;
				nItemLen=max(nItemLen,nWidth+nPrecision);
				break;

			case 'e':
			case 'g':
			case 'G':
				va_arg(argList,double);
				nItemLen=128;
				nItemLen=max(nItemLen,nWidth+nPrecision);
				break;

			case 'f':
				{
					double f;
					char *StrTemp;

					// 312 == strlen("-1+(309 zeroes).")
					// 309 zeroes == max precision of a double
					// 6 == adjustment in case precision is not specified,
					//   which means that the precision defaults to 6
					StrTemp=(char*)MyMalloc(max(nWidth,312+nPrecision+6)*sizeof(char));

					f=va_arg(argList,double);
					sprintf(StrTemp,"%*.*f",nWidth,nPrecision+6,f);
					nItemLen=(int)strlen(StrTemp);

					MyFree(StrTemp);
				}
				break;

			case 'p':
				va_arg(argList,void*);
				nItemLen=32;
				nItemLen=max(nItemLen,nWidth+nPrecision);
				break;

			// no output
			case 'n':
				va_arg(argList,int*);
				break;

			default:	// unknown formatting option
				GetCMyExceptionObj(E,ERR_PRGM);
				throw(E);
			}
		}

		// adjust nMaxLen for output nItemLen
		nbChar+=nItemLen;
	}

	va_end(argList);

	return(nbChar);
}
