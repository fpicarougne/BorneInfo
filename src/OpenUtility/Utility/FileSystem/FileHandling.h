#ifndef FileHandling_h
	#define FileHandling_h

#include <stdlib.h>
#include "../Stream/Stream.h"

namespace OpenUtility
{

struct DirListStruct;
typedef DirListStruct* DIRHANDLE;

enum ETypeResult
{
	ONLY_FILE,
	ONLY_DIR,
	FILE_DIR
};

DIRHANDLE InitDirList(const char *baseDir,ETypeResult type=FILE_DIR);
bool GetNextDirFile(DIRHANDLE handle,CStream &str);
void ReleaseDirList(DIRHANDLE handle);
bool isFile(const char *file);
bool isDir(const char *file);
void ParseFileName(const char * const file,CStream &dir,CStream &fileName,CStream &ext);
size_t GetFileSize(const char * const file);

}

#endif
