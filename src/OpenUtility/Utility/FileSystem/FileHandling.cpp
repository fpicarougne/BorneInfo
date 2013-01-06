#include "FileHandling.h"
#ifdef WIN32
	#include <windows.h>
#else
	#include <dirent.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <unistd.h>
#endif
#include <string.h>

namespace OpenUtility
{

struct DirListStruct
{
#ifdef WIN32
	HANDLE hFile;
	WIN32_FIND_DATAA finddata;
	bool firstElmt;
#else
	struct dirent **namelist;
	int cur;
	int n;
#endif
	CStream curDir;
	ETypeResult whatFind;
};

}

OpenUtility::DIRHANDLE OpenUtility::InitDirList(const char *baseDir,OpenUtility::ETypeResult type)
{
	DirListStruct *dirList;

	dirList=new DirListStruct;
	dirList->whatFind=type;
	dirList->curDir=baseDir;
	if (dirList->curDir.GetSize() && ((dirList->curDir.GetStream()[dirList->curDir.GetSize()-1]!='/') || (dirList->curDir.GetStream()[dirList->curDir.GetSize()-1]!='\\')))
		dirList->curDir.AddStream('/');

#ifdef WIN32
	dirList->firstElmt=true;
	dirList->curDir+="*.*";
	if ((dirList->hFile=FindFirstFileA(dirList->curDir.GetStream(),&dirList->finddata))!=INVALID_HANDLE_VALUE) return(dirList);
#else
	if ((dirList->n=scandir(baseDir,&dirList->namelist,0,alphasort))>=0)
	{
		dirList->cur=0;
		return(dirList);
	}
#endif
	else
	{
		delete dirList;
		return(NULL);
	}
}

bool OpenUtility::GetNextDirFile(OpenUtility::DIRHANDLE dirList,CStream &str)
{
	bool find=false;
	bool fini=false;

#ifdef WIN32
	if (dirList->firstElmt)
	{
		dirList->firstElmt=false;
		find=true;
	}
	else find=(FindNextFileA(dirList->hFile,&dirList->finddata)!=0);
	while(find && !fini)
	{
		if ((fini=(dirList->whatFind==FILE_DIR)) ||
			(fini=((dirList->whatFind==ONLY_FILE ?	!(dirList->finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) :
												dirList->finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=0)))
			str=dirList->finddata.cFileName;
		else
			find=(FindNextFileA(dirList->hFile,&dirList->finddata)!=0);
	}
#else
	CStream curFile;
	while (!fini && (find=(dirList->cur<dirList->n)))
	{
		struct stat buf;

		curFile=dirList->curDir;
		curFile+=dirList->namelist[dirList->cur]->d_name;
		if (stat(curFile.GetStream(),&buf)) find=false;
		else
		{
			if ((fini=(dirList->whatFind==FILE_DIR)) ||
				(fini=(dirList->whatFind==ONLY_FILE ?	!S_ISDIR(buf.st_mode) : S_ISDIR(buf.st_mode))))
				str=dirList->namelist[dirList->cur]->d_name;
		}
		dirList->cur++;
	}
#endif

	return(find);
}

void OpenUtility::ReleaseDirList(OpenUtility::DIRHANDLE dirList)
{
#ifdef WIN32
	FindClose(dirList->hFile);
#else
	for (int i=0;i<dirList->n;i++) free(dirList->namelist[i]);
	free(dirList->namelist);
#endif

	delete dirList;
}

bool OpenUtility::isFile(const char *file)
{
#ifdef WIN32
	DWORD attr;

	if ((attr=GetFileAttributesA(file))==INVALID_FILE_ATTRIBUTES) return(false);
	return((attr & ~FILE_ATTRIBUTE_DIRECTORY)!=0);
#else
	struct stat buf;
	if (stat(file,&buf)) return(false);
	return(!S_ISDIR(buf.st_mode));
#endif
}

bool OpenUtility::isDir(const char *file)
{
#ifdef WIN32
	return((GetFileAttributesA(file) & FILE_ATTRIBUTE_DIRECTORY)!=0);
#else
	struct stat buf;
	if (stat(file,&buf)) return(false);
	return(S_ISDIR(buf.st_mode));
#endif
}

void OpenUtility::ParseFileName(const char * const file,CStream &dir,CStream &fileName,CStream &ext)
{
	int len=strlen(file);
	bool dot=false;

	if (len==0)
	{
		ext="";
		fileName="";
		dir="";
	}
	else
	{
		char *str;
		int i;

		str=const_cast<char*>(&file[len-1]);
		i=0;
		while ((str!=file) && (*str!='\\') && (*str!='/'))
		{
			if ((!dot) && (*str=='.'))
			{
				dot=true;
				ext=str+1;
				i=0;
			}
			else i++;
			str--;
		}
		if ((*str!='\\') && (*str!='/'))
		{
			fileName.AddStream(str,i+1);
			dir="";
		}
		else
		{
			fileName.AddStream(str+1,i);
			dir.AddStream(file,len-fileName.GetSize()-ext.GetSize()+(dot ? -1 : 0));
		}
	}
}

size_t OpenUtility::GetFileSize(const char * const file)
{
#ifdef WIN32
	hFile=CreateFile(file,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	size_t size=GetFileSize(hFile,NULL);
	CloseHandle(hFile);
	return(size);
#else
	struct stat st;
	stat(file,&st);
	return(st.st_size);
#endif
}
