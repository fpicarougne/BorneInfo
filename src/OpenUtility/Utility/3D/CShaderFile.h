#ifndef _CShaderFile_h
	#define _CShaderFile_h

#include "../Stream/Stream.h"
#include "../../Template/CListe.h"
#ifdef GLES2
	#include <GLES2/gl2.h>
#else
	#include <GL/gl.h>
#endif

namespace OpenUtility
{

class CShaderProgram;

class CShaderFile
{
	friend class CShaderProgram;

public:
	enum EShaderType
	{
		EVertexShader,
		EFragmentShader,
#ifndef GLES2
		EGeometryShader
#endif
	};

	enum EShaderState
	{
		EShaderNoSource,
		EShaderSourceModified,
		EShaderValid,
		EShaderNotValid
	};

public:
	CShaderFile(EShaderType type,const char *name,const char *vertexFile=NULL);
	CShaderFile(const CShaderFile &obj);
	~CShaderFile();
	CShaderFile& operator=(const CShaderFile &obj);
	inline const char* GetName() {return(Name.GetStream());}
	bool LoadFile(const char *file);
	bool LoadStream(const char *src);
	inline const CStream& GetLog() {return(CurLog);}
	inline GLuint GetId() {return(idShader);}
	inline EShaderState GetState() {return(State);}
	bool CompileShader();

private:
	void Init();
	void InitCopy(const CShaderFile &obj);
	bool GetOpenglLog();
	void SetState(EShaderState state);
	void AddProgram(CShaderProgram *program);
	void RemoveProgram(CShaderProgram *program);

private:
	CStream Name;
	EShaderType ShaderType;
	EShaderState State;
	CStream ShaderSource;
	GLuint idShader;
	CStream CurLog;
	CListe<CShaderProgram> ProgramList;
};

}

#endif
