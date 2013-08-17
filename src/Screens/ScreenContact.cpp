//coding: utf-8
#include "ScreenContact.h"

CScreenContact::CScreenContact() :
	CScreen("Contact")
{
}

CScreenContact::CScreenContact(const CScreenContact &obj) :
	CScreen(obj)
{
}

CScreenContact::~CScreenContact()
{
}

CScreenContact& CScreenContact::operator=(const CScreenContact &obj)
{
	CScreen::operator=(obj);
	return(*this);
}
