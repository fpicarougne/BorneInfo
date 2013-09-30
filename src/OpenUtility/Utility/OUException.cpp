#include "OUException.h"
#include <stdlib.h>
#ifdef WIN32
    #ifndef __GNUG__
        #include <Windows.h>
        #include <Dbghelp.h>
        #pragma comment(lib,"Dbghelp.lib")
    #endif
#else
	#include <execinfo.h>
	#include <dlfcn.h>
	#include <cxxabi.h>
char **__backtrace_symbols(void *const *buffer, int size);
#endif

OpenUtility::Exception::Exception(const char *str,bool displayStack) :
	DisplayStack(displayStack)
{
	if (str) Detail=str;

	// Print Stack
#ifdef WIN32
    #ifndef __GNUG__
	void *stack[30];
	USHORT nbFrame;
	ULONG64 buffer[(sizeof(SYMBOL_INFO)+MAX_SYM_NAME*sizeof(TCHAR)+sizeof(ULONG64)-1)/sizeof(ULONG64)];
	HANDLE hProcess=GetCurrentProcess();
	PSYMBOL_INFO pSymbol=(PSYMBOL_INFO)buffer;
	IMAGEHLP_LINE64 line;
	DWORD dwDisplacement;

	pSymbol->SizeOfStruct=sizeof(SYMBOL_INFO);
	pSymbol->MaxNameLen=MAX_SYM_NAME;
	line.SizeOfStruct = sizeof(line);

	SymSetOptions(SYMOPT_LOAD_LINES);
	BOOL initRes=SymInitialize(hProcess,NULL,true);

	nbFrame=CaptureStackBackTrace(1,30,stack,NULL);
	for(USHORT i=0;i<nbFrame;i++)
	{
		if (SymFromAddr(hProcess,(DWORD64)(stack[i]),0,pSymbol))
		{
			Stack.AddFormatStream("-> %s",pSymbol->Name);
			if (SymGetLineFromAddr64(hProcess,(DWORD64)(stack[i]),&dwDisplacement,&line))
			{
				Stack.AddFormatStream(" - line %d (%s)",line.LineNumber,line.FileName);
			}
			else Stack+=" - no file information";
			Stack.AddStream('\n');
		}
	}
	#endif
#else
	void *stack[30];
	size_t nbFrame;
	char **retValues;
	Dl_info info;

	nbFrame=backtrace(stack,30);
	if ((retValues=backtrace_symbols(stack,nbFrame))!=NULL)
	{
		for (size_t i=0;i<nbFrame-2;i++)
		{
			Stack+=retValues[i];
			Stack+="\n\t";
			dladdr(stack[i],&info);
			Stack+=info.dli_fname;
			Stack+="\n\t";
char *strdemang;
int status;
strdemang=abi::__cxa_demangle(info.dli_sname,NULL,NULL,&status);
			Stack+=strdemang;
free(strdemang);
			Stack+="\n\t";
			Stack.AddFormatStream("stack: %p\n\t",stack[i]);
			Stack.AddFormatStream("%d",(unsigned int)((unsigned int)stack[i] - (unsigned int)info.dli_saddr));
			Stack+="\n";
		}
		free(retValues);
	}
#endif
	UpdateStr();
}

OpenUtility::Exception::Exception(const OpenUtility::Exception &obj) :
	std::exception(obj),
	DisplayStack(obj.DisplayStack),
	Detail(obj.Detail),
	Error(obj.Error),
	Stack(obj.Stack)
{
}

OpenUtility::Exception& OpenUtility::Exception::SetFileLine(const char *file,long line)
{
	if (line>=0) LineError.Format("File \"%s\" - line %ld",file,line);
	else LineError="";
	UpdateStr();
	return(*this);
}

void OpenUtility::Exception::UpdateStr()
{
	Error="";
	if (LineError.GetSize())
	{
		Error+=LineError;
		Error+="\n\t";
	}
	Error+=Detail;

	if (DisplayStack && Stack.GetSize())
	{
		Error+="\n\n--- Stack ---\n";
		Error+=Stack;
	}
}
