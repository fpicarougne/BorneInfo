#ifndef Stream_h
	#define Stream_h

#include <stdio.h>
#include <stdarg.h>
//#include <iostream.h>

#ifdef WIN32
	#ifdef OPENUTILITY_DLL
		#ifdef DLLSTREAM_EXPORTS
		#define STREAM_API __declspec(dllexport)
		#else
		#define STREAM_API __declspec(dllimport)
		#endif
	#else
		#define STREAM_API
	#endif
#else
	#define STREAM_API
#endif

#define STR_END -1
#define TAILLE_DEF 100

namespace OpenUtility
{

class STREAM_API CGalStream
{
public:
	CGalStream();
	~CGalStream();
	operator char*() const;

protected:
	char *Stream;
	unsigned int taille;

public:
	void ReplaceLastCar(char c);
	int FindString(char *Str,int Start=0,int End=STR_END);

public:
	static unsigned int HashStr(char *str) {return(HashStr(const_cast<const char*>(str)));}
	static unsigned int HashStr(const char *str);
	static unsigned int HashStr(char **str) {return(HashStr(const_cast<const char**>(str)));}
	static unsigned int HashStr(const char **str);

protected:
	int NbCharFormat(const char *StrFormat,va_list argList);
	void GalTrimLeft();
	void GalTrimRight();
	inline void GalTrim() {GalTrimLeft();GalTrimRight();}
};

class CBlockStream;

//TODO: Harmoniser les fonctions disponibles en CStream et CBlockStream

class STREAM_API CStream : public CGalStream
{
friend class CBlockStream;

// Attributes
public:

protected:

// Operations
public:
	CStream(bool empty=false);
	CStream(CStream &S);
	CStream(const char *buffer);
	CStream(char car);
	~CStream();
	char* GetStream() const;
	char& operator[](const int indice);
	unsigned int GetSize();
	void AddStream(const char *buffer,int size);
	void AddStream(char car);
	void AddFormatStream(const char *StrFormat,...);
	void AddVFormatStream(const char *StrFormat,va_list argList);
	CStream& operator+(char *buffer);
	CStream& operator+(const char *buffer);
	CStream& operator+(char car);
	CStream& operator+(CStream &S);
	CStream& operator+(CBlockStream &S);
	CStream& operator=(const char car);
	CStream& operator=(char *buffer);
	CStream& operator=(const char *buffer);
	CStream& operator=(CStream &S);
	CStream& operator=(CBlockStream &S);
	CStream& operator+=(const char *buffer);
	CStream& operator+=(char *buffer);
	CStream& operator+=(char car);
	CStream& operator+=(CStream &S);
	CStream& operator+=(CBlockStream &S);
	bool operator==(CStream &S);
	bool operator==(CBlockStream &S);
	bool operator==(char *Str);
	bool operator==(const char *Str);
	bool operator!=(CStream &S);
	bool operator!=(CBlockStream &S);
	bool operator!=(char *Str);
	bool operator!=(const char *Str);
	bool operator<(CStream &S);
	bool operator<(CBlockStream &S);
	bool operator<(const char *Str);
	bool operator<(const char car);
	bool operator<=(CStream &S);
	bool operator<=(CBlockStream &S);
	bool operator<=(const char *Str);
	bool operator<=(const char car);
	bool operator>(CStream &S);
	bool operator>(CBlockStream &S);
	bool operator>(const char *Str);
	bool operator>(const char car);
	bool operator>=(CStream &S);
	bool operator>=(CBlockStream &S);
	bool operator>=(const char *Str);
	bool operator>=(const char car);
	friend CStream operator+(CStream &S1,CStream &S2);
	friend CStream operator+(CStream &S,const char *buffer);
	friend CStream operator+(const char *buffer,CStream &S);
	friend CStream operator+(CStream &S,char car);
	friend CStream operator+(char car,CStream &S);
/*
	friend ostream& operator<<(ostream &ar,CStream &S);
	friend istream& operator>>(istream &ar,CStream &S);
*/
	void Format(const char *StrFormat,...);
	void VFormat(const char *StrFormat,va_list argList);
	void ToLower();
	void ToUpper();
	inline void Trim() {GalTrim();CalcNewSize();}
	inline void TrimLeft() {GalTrimLeft();CalcNewSize();}
	inline void TrimRight() {GalTrimRight();CalcNewSize();}
	void AllocBuffer(unsigned int size);
	void RemoveCar(int nb);

public:
	static unsigned int Hash(CStream *str) {return(Hash(const_cast<const CStream*>(str)));}
	static unsigned int Hash(const CStream *str);

protected:

private:
	void CalcNewSize();
};

/*STREAM_API CStream operator+(CStream &S1,CStream &S2);
STREAM_API CStream operator+(CStream &S,const char *buffer);
STREAM_API CStream operator+(const char *buffer,CStream &S);
STREAM_API CStream operator+(CStream &S,char car);
STREAM_API CStream operator+(char car,CStream &S);
STREAM_API ostream& operator<<(ostream &ar,CStream &S);
STREAM_API istream& operator>>(istream &ar,CStream &S);*/

class STREAM_API CBlockStream : public CGalStream
{
friend class CStream;

// Attributes
public:

protected:

private:
	unsigned int TailleBlock;
	unsigned int IncBlock;

// Operations
public:
	CBlockStream(int TBlock=TAILLE_DEF);
	CBlockStream(CBlockStream &S,int TBlock);
	CBlockStream(CBlockStream &S);
	CBlockStream(CStream &S,int TBlock=TAILLE_DEF);
	CBlockStream(const char *buffer,int TBlock=TAILLE_DEF);
	CBlockStream(const char car,int TBlock=TAILLE_DEF);
	~CBlockStream();
	char* GetStream() const;
	unsigned int GetSize();
	void AddStream(const char *buffer,int size);
	void AddStream(char car);
	void AddFormatStream(const char *StrFormat,...);
	void AddVFormatStream(const char *StrFormat,va_list argList);
	CBlockStream& operator+(const char* buffer);
	CBlockStream& operator+(char* buffer);
	CBlockStream& operator+(char car);
	CBlockStream& operator+(CBlockStream &S);
	CBlockStream& operator+(CStream &S);
	CBlockStream& operator=(const char car);
	CBlockStream& operator=(const char* buffer);
	CBlockStream& operator=(CBlockStream &S);
	CBlockStream& operator=(CStream &S);
	CBlockStream& operator+=(const char* buffer);
	CBlockStream& operator+=(char* buffer);
	CBlockStream& operator+=(char car);
	CBlockStream& operator+=(CBlockStream &S);
	CBlockStream& operator+=(CStream &S);
	bool operator==(CBlockStream &S);
	bool operator==(CStream &S);
	bool operator==(const char *Str);
	bool operator!=(CBlockStream &S);
	bool operator!=(CStream &S);
	bool operator!=(const char *Str);
	bool operator<(CStream &S);
	bool operator<(CBlockStream &S);
	bool operator<(const char *Str);
	bool operator<(const char car);
	bool operator<=(CStream &S);
	bool operator<=(CBlockStream &S);
	bool operator<=(const char *Str);
	bool operator<=(const char car);
	bool operator>(CStream &S);
	bool operator>(CBlockStream &S);
	bool operator>(const char *Str);
	bool operator>(const char car);
	bool operator>=(CStream &S);
	bool operator>=(CBlockStream &S);
	bool operator>=(const char *Str);
	bool operator>=(const char car);
	void Format(const char *StrFormat,...);
	void VFormat(const char *StrFormat,va_list argList);
	void RemoveCar(int nb);
	void ToLower();
	void ToUpper();
	inline void Trim() {GalTrim();CalcNewSize();}
	inline void TrimLeft() {GalTrimLeft();CalcNewSize();}
	inline void TrimRight() {GalTrimRight();CalcNewSize();}
	int ReplaceOne(char *StrFind,char *StrReplace,unsigned int pos);
	void ReplaceFirst(char *StrFind,char *StrReplace);
	void ReplaceAll(char *StrFind,char *StrReplace);

public:
	static unsigned int Hash(CBlockStream *str) {return(Hash(const_cast<const CBlockStream*>(str)));}
	static unsigned int Hash(const CBlockStream *str);

protected:

private:
	void CalcNewSize();
};

CStream operator+(CStream &S1,CStream &S2);
CStream operator+(CStream &S,const char *buffer);
CStream operator+(const char *buffer,CStream &S);
CStream operator+(CStream &S,char car);
CStream operator+(char car,CStream &S);

CStream& operator<<(CStream &stream,char *str);

/*
ostream& operator<<(ostream &ar,CStream &S);
istream& operator>>(istream &ar,CStream &S);
*/

}

#endif
