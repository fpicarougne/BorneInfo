//coding: utf-8
#ifndef _ScreenContact_h
	#define _ScreenContact_h

#include "Screen.h"

class CScreenContact : public CScreen
{
public:
	CScreenContact();
	CScreenContact(const CScreenContact &obj);
	virtual ~CScreenContact();
	CScreenContact& operator=(const CScreenContact &obj);
};

#endif
