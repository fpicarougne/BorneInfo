#ifndef Memory_h
	#define Memory_h

#include <stdio.h>
#include <iostream>
#include "CodeErreur.h"
#include <execinfo.h>

#define GetCMyException(valeur) CMyException(__FILE__,__LINE__,__FUNCTION__,valeur)
#define GetCMyExceptionObj(obj,valeur) CMyException obj(__FILE__,__LINE__,__FUNCTION__,valeur)

namespace OpenUtility
{

void* MyMalloc(int Size);
void* MyCalloc(size_t num,size_t size);
void* MyRealloc(void *Ptr,int Size);
char* MyStrdup(const char *strSource);
void MyFree(void *Ptr);

class CMyException
{
	// cette classe, de type abstrait, represente une ou plusieurs exceptions

	// ATTRIBUTS:
private:
	
	unsigned int valeur;	// cette variable contient la valeur de
							// l'exception.
	unsigned int type;	// cette variable donne des infos suplémentaires
						// sur le type d'erreur rencontrée.

	/* Etat initial
		valeur=NO_ERREUR
		type=NO_ERREUR */

	const char *File;
	int Line;
	const char *Func;

	// PRIMITIVES:
public:
	
	CMyException(const char *file,int line,const char *func);
	/* Constructeur par defaut de la classe
	E: néant
	necessite: néant
	S: néant
	entraine: l'objet est initialisé */

	CMyException(const char *file,int line,const char *func,unsigned int Nvaleur);
	/* Constructeur de la classe
	E: valeur du champ valeur
	necessite: néant
	S: néant
	entraine: l'objet est initialisé */

	CMyException(const char *file,int line,const char *func,unsigned int Nvaleur,unsigned int Ntype);
	/* Constructeur de la classe
	E: valeur du champ valeur, valeur du champ type
	necessite: néant
	S: néant
	entraine: l'objet est initialisé */

	~CMyException(){}
	/* Destructeur par defaut de la classe
	E: néant
	necessite: néant
	S: néant
	entraine: l'objet est détruit */

	void modifier_valeur(unsigned int valeur);
	/* Cette fonction permet de modifier la valeur de l'exception
	E: la nouvelle valeur
	necessite: néant
	S: néant
	entraine: la valeur de l'exception est retournée */

	int lire_valeur();
	/* Cette fonction permet de consulter la valeur de l'exception
	E: néant
	necessite: néant
	S: la valeur de l'exception
	entraine: la valeur de l'exception est retournée */

	void modifier_type(unsigned int valeur);
	/* Cette fonction permet de modifier la valeur de type
	E: la nouvelle valeur
	necessite: néant
	S: néant
	entraine: la valeur de type est retournée */

	int lire_type();
	/* Cette fonction permet de consulter la valeur de type
	E: néant
	necessite: néant
	S: la valeur de la ligne d'erreur
	entraine: la valeur de type est retournée */

	inline friend std::ostream& operator<<(std::ostream &obj,const CMyException &e)
	{
		obj << "Exception (" << e.valeur << ":" << e.type << ")\n\tFile: " << e.File << "\n\tLine: " << e.Line << "\n\tFunc: " << e.Func << std::endl;
#ifndef WIN32
void *array[10];
size_t size;

// get void*'s for all entries on the stack
size = backtrace(array, 20);

// print out all the frames to stderr
backtrace_symbols_fd(array, size, 2);
#endif

		return(obj);
	}
	//MODULES EXTERNES
};

}

#endif
