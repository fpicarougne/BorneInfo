/******************************************************************************
***						TITRE: Classe pour les exceptions					***
*******************************************************************************
*** Auteurs: PICAROUGNE, MOULY ************************ Version : 2.0 *********
*** Date: 10/04/00 ************************************************************
*******************************************************************************
*** Lecteurs: PICAROUGNE, MOULY *********************** Date: 10/04/00 ********
*******************************************************************************
***						CORPS DE LA CLASSE CMyException						***
******************************************************************************/

/* Classe CMyException
	Documentation
		Variables:	valeur, entier contenant la valeur de l'exception
					type, entier contenant le type de l'exception
		Fct abstraites: néant
		Structure: néant
		Methode: néant

Modules internes: */
#include "Memory.h"

/*Corps*/

/*-----------------------------------------------------------------------------
--- NOM: CMyException														---
-------------------------------------------------------------------------------
--- Constructeur par defaut de la classe CMyException ; permet				---
--- d'initialiser un objet													---
-------------------------------------------------------------------------------
--- Entree: néant															---
--- Sortie: néant															---
-----------------------------------------------------------------------------*/
OpenUtility::CMyException::CMyException(const char *file,int line,const char *func) :
	File(file),Line(line),Func(Func)
{
	valeur=NO_ERREUR;
	type=NOT_DEFINE;
}

/*-----------------------------------------------------------------------------
--- NOM: CMyException														---
-------------------------------------------------------------------------------
--- Constructeur de la classe CMyException ; permet							---
--- d'initialiser un objet													---
-------------------------------------------------------------------------------
--- Entree: valeur du champ valeur											---
--- Sortie: néant															---
-----------------------------------------------------------------------------*/
OpenUtility::CMyException::CMyException(const char *file,int line,const char *func,unsigned int Nvaleur) :
	File(file),Line(line),Func(Func)
{
	valeur=Nvaleur;
	type=NOT_DEFINE;
}

/*-----------------------------------------------------------------------------
--- NOM: CMyException														---
-------------------------------------------------------------------------------
--- Constructeur de la classe CMyException ; permet							---
--- d'initialiser un objet													---
-------------------------------------------------------------------------------
--- Entree: valeur du champ valeur, valeur du champ type					---
--- Sortie: néant															---
-----------------------------------------------------------------------------*/
OpenUtility::CMyException::CMyException(const char *file,int line,const char *func,unsigned int Nvaleur,unsigned int Ntype) :
	File(file),Line(line),Func(Func)
{
	valeur=Nvaleur;
	type=Ntype;
}

/*-----------------------------------------------------------------------------
--- NOM: modifier_valeur													---
-------------------------------------------------------------------------------
--- Cette fonction permet de modifier la valeur de l'exception				---
-------------------------------------------------------------------------------
--- Entree: la nouvelle valeur												---
--- Sortie: néant															---
-----------------------------------------------------------------------------*/
void OpenUtility::CMyException::modifier_valeur(unsigned int nvaleur)
{
 valeur = nvaleur;
}

/*-----------------------------------------------------------------------------
--- NOM: lire_valeur														---
-------------------------------------------------------------------------------
--- Cette fonction permet de consulter la valeur de l'exception				---
-------------------------------------------------------------------------------
--- Entree: néant															---
--- Sortie: la nouvelle valeur												---
-----------------------------------------------------------------------------*/
int OpenUtility::CMyException::lire_valeur()
{
 return(valeur);
}

/*-----------------------------------------------------------------------------
--- NOM: modifier_type														---
-------------------------------------------------------------------------------
--- Cette fonction permet de modifier la valeur de type						---
-------------------------------------------------------------------------------
--- Entree: la nouvelle valeur												---
--- Sortie: néant															---
-----------------------------------------------------------------------------*/
void OpenUtility::CMyException::modifier_type(unsigned int nvaleur)
{
 type = nvaleur;
}

/*-----------------------------------------------------------------------------
--- NOM: lire_type															---
-------------------------------------------------------------------------------
--- Cette fonction permet de consulter la valeur de type					---
-------------------------------------------------------------------------------
--- Entree: néant															---
--- Sortie: la nouvelle valeur												---
-----------------------------------------------------------------------------*/
int OpenUtility::CMyException::lire_type()
{
 return(type);
}
