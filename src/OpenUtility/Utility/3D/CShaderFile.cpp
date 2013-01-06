#ifdef WIN32
	#include <GL/glew.h>
#else
#ifdef GLES2
	#include <GLES2/gl2.h>
#endif
#endif
#include <string.h>
#include "CShaderFile.h"
#include "CShaderProgram.h"

#include <assert.h>
#define check() assert(glGetError()==0)

OpenUtility::CShaderFile::CShaderFile(EShaderType type,const char *name,const char *vertexFile) :
	Name(name),
	ShaderType(type),
	State(EShaderNoSource)
{
	Init();
	if (vertexFile) LoadFile(vertexFile);
}

OpenUtility::CShaderFile::CShaderFile(const OpenUtility::CShaderFile &obj) :
	ShaderType(obj.ShaderType),
	State(EShaderNoSource)
{
	Init();
	InitCopy(obj);
}

OpenUtility::CShaderFile& OpenUtility::CShaderFile::operator=(const OpenUtility::CShaderFile &obj)
{
	if (obj.ShaderType!=ShaderType)
	{
		glDeleteShader(idShader);
		ShaderType=obj.ShaderType;
		Init();
	}
	InitCopy(obj);
	return(*this);
}

OpenUtility::CShaderFile::~CShaderFile()
{
/*	while (ProgramList.GetSize())
		ProgramList[(unsigned int)0]->RemoveShader(this);

	glDeleteShader(idShader);*/
}

void OpenUtility::CShaderFile::Init()
{
	GLenum type;

	switch(ShaderType)
	{
	case EVertexShader:type=GL_VERTEX_SHADER;break;
	case EFragmentShader:type=GL_FRAGMENT_SHADER;break;
#ifndef GLES2
	case EGeometryShader:type=GL_GEOMETRY_SHADER_ARB;break;
#endif
	}
	idShader=glCreateShader(type);
}

void OpenUtility::CShaderFile::InitCopy(const OpenUtility::CShaderFile &obj)
{
	LoadStream(obj.ShaderSource);
}

void OpenUtility::CShaderFile::SetState(OpenUtility::CShaderFile::EShaderState state)
{
	if (state!=State)
	{
		unsigned int i;

		State=state;
		for (i=0;i<ProgramList.GetSize();i++)
			ProgramList[i]->SetHasChanged();
	}
}

bool OpenUtility::CShaderFile::LoadFile(const char *file)
{
	FILE *fp;
	long count;

	if ((fp=fopen(file,"rt"))!=NULL)
	{
		fseek(fp,0,SEEK_END);
		count=ftell(fp);
		rewind(fp);

		if (count>0)
		{
			ShaderSource.AllocBuffer(count+1);
			count=static_cast<long>(fread(ShaderSource,sizeof(char),count,fp));
			ShaderSource[count]='\0';

			SetState(EShaderSourceModified);
		}
		fclose(fp);

		return(CompileShader());
	}
	else
	{
		CurLog.Format("Could not open file '%s'",file);
		ShaderSource="";
		SetState(EShaderNoSource);
		return(false);
	}
}

bool OpenUtility::CShaderFile::LoadStream(const char *src)
{
	if (strlen(src)>0) SetState(EShaderSourceModified);
	else SetState(EShaderNoSource);
	ShaderSource=src;

	return(CompileShader());
}

bool OpenUtility::CShaderFile::CompileShader()
{
	if (State==EShaderSourceModified)
	{
		GLint status;
		char *csrc=ShaderSource;

		glShaderSource(idShader,1,const_cast<const GLchar**>(&csrc),NULL);
check();
		glCompileShader(idShader);
check();
		glGetShaderiv(idShader,GL_COMPILE_STATUS,&status);
check();

		if (status!=0)
		{
			CurLog="";
			SetState(EShaderValid);
		}
		else
		{
			if (!GetOpenglLog())
				CurLog="Compilation error, but connot get log...";
			SetState(EShaderNotValid);
		}
	}
	return((State==EShaderValid || State==EShaderNoSource));
}

bool OpenUtility::CShaderFile::GetOpenglLog()
{
	int infologLength=0;
	int charsWritten=0;

	glGetShaderiv(idShader,GL_INFO_LOG_LENGTH,&infologLength);
check();
	if (infologLength>1)
	{
		CurLog.AllocBuffer(infologLength+1);
		glGetShaderInfoLog(idShader,infologLength,&charsWritten,CurLog);
		CurLog[charsWritten]='\0';
		return(true);
	}
	return(false);
}

void OpenUtility::CShaderFile::AddProgram(OpenUtility::CShaderProgram *program)
{
	unsigned int i;

	// Check if program already in list
	for (i=0;i<ProgramList.GetSize();i++)
		if (ProgramList[i]==program) return;

	ProgramList.Add(program);
}

void OpenUtility::CShaderFile::RemoveProgram(OpenUtility::CShaderProgram *program)
{
	unsigned int i;

	// Check if program already in list
	for (i=0;i<ProgramList.GetSize();i++)
	{
		if (ProgramList[i]==program)
		{
			ProgramList.Remove(i);
			return;
		}
	}
}
