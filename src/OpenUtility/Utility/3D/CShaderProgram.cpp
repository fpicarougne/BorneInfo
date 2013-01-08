#ifdef WIN32
	#include <GL/glew.h>
#else
	#ifdef GLES2
		#include <GLES2/gl2.h>
	#endif
#endif
#include "CShaderProgram.h"

OpenUtility::CShaderProgram::Exception::Exception(OpenUtility::CShaderProgram::Exception::EError err,const char *detail) :
	ErrType(err),
	Detail(detail)
{
	UpdateStr();
}

OpenUtility::CShaderProgram::Exception::Exception(const OpenUtility::CShaderProgram::Exception &obj) :
	OpenUtility::Exception(obj),
	ErrType(obj.ErrType),
	Detail(obj.Detail)
{
}

void OpenUtility::CShaderProgram::Exception::UpdateStr()
{
	if (Error.GetSize()) Error.AddStream('\n');
	switch(ErrType)
	{
	case EErrNotValid:
		Error+="Shader program not valid";
		break;

	case EErrIdNotFound:
		Error+="Shader variable id not found";
		if (Detail.GetSize()) Error.AddFormatStream(" (%s)",Detail.GetStream());
		break;
	}
}

//****************************************************************************************

OpenUtility::CShaderProgram::CShaderProgram() :
	State(EProgramNotValid),
	MapVariable(CGalStream::HashStr,strcmp,30,50)
{
	Init();
}

OpenUtility::CShaderProgram::CShaderProgram(const OpenUtility::CShaderProgram &obj) :
	State(EProgramNotValid),
	MapVariable(CGalStream::HashStr,strcmp,30,50)
{
	Init();
	InitCopy(obj);
}

OpenUtility::CShaderProgram& OpenUtility::CShaderProgram::operator=(const OpenUtility::CShaderProgram &obj)
{
	InitCopy(obj);
	return(*this);
}

OpenUtility::CShaderProgram::~CShaderProgram()
{
/*	unsigned int i;

	VectVariable.DeleteAll();
	glDeleteProgram(idProgram);

	for (i=0;i<ShaderList.GetSize();i++)
		ShaderList[i]->RemoveProgram(this);*/
}

void OpenUtility::CShaderProgram::Init()
{
	idProgram=glCreateProgram();
}

void OpenUtility::CShaderProgram::InitCopy(const OpenUtility::CShaderProgram &obj)
{
	VectVariable.DeleteAll();
	MapVariable.RemoveAll();
	if (obj.State==EProgramValid) LinkProgram();
}

void OpenUtility::CShaderProgram::AddShaderFile(OpenUtility::CShaderFile *shader)
{
	unsigned int i;

	// Check if Shader already in list
	for (i=0;i<ShaderList.GetSize();i++)
		if (ShaderList[i]==shader) return;

	ShaderList.Add(shader);
	shader->AddProgram(this);
}

void OpenUtility::CShaderProgram::RemoveShader(OpenUtility::CShaderFile *shader)
{
	unsigned int i;

	// Check if Shader already in list
	for (i=0;i<ShaderList.GetSize();i++)
	{
		if (ShaderList[i]==shader)
		{
			ShaderList.Remove(i);
			shader->RemoveProgram(this);
			return;
		}
	}
}

bool OpenUtility::CShaderProgram::LinkProgram()
{
	GLint status;
	GLint nb;
	GLsizei count;
	unsigned int i;
	int j;
	CShaderFile *shader;
	bool found,error;
	CShaderFile::EShaderState state;
	GLuint *idShaders;

	glGetProgramiv(idProgram,GL_ATTACHED_SHADERS,&nb);
	idShaders=new GLuint[nb];
	glGetAttachedShaders(idProgram,nb,&count,idShaders);

	CurLog="";
	error=false;
	for (i=0;i<ShaderList.GetSize() && !error;i++)
	{
		shader=ShaderList[i];
		switch (state=shader->GetState())
		{
		case CShaderFile::EShaderNotValid:
			State=EProgramNotValid;
			CurLog.AddFormatStream("Shader %s(%d) is not valid\n",shader->GetName(),shader->GetId());
			error=true;
			break;

		case CShaderFile::EShaderSourceModified:
			if (!shader->CompileShader())
			{
				State=EProgramNotValid;
				CurLog.AddFormatStream("Shader %s(%d) is not valid\n",shader->GetName(),shader->GetId());
				error=true;
			}
			break;

		default:
			break;
		}

		for (j=0,found=false;j<count && !found;j++)
		{
			if (idShaders[j]==shader->GetId())
			{
				idShaders[j]=0;
				found=true;
			}
		}
		if (found)
		{
			if (state==CShaderFile::EShaderNoSource)
				glDetachShader(idProgram,shader->GetId());
		}
		else
		{
			if (state==CShaderFile::EShaderValid)
				glAttachShader(idProgram,shader->GetId());
		}
	}
	for (j=0;j<count;j++)
	{
		if (idShaders[j]!=0)
			glDetachShader(idProgram,idShaders[j]);
	}

	delete[] idShaders;

	if (!error)
	{
		glLinkProgram(idProgram);
		glValidateProgram(idProgram);
		glGetProgramiv(idProgram,GL_LINK_STATUS,&status);

		if (status==0)
		{
			GetOpenglLog();
			State=EProgramNotValid;
		}
		else
		{
			State=EProgramValid;
			VectVariable.DeleteAll();
			MapVariable.RemoveAll();

			// Parse attribute and uniform variables
			GLsizei maxLength;
			SShaderVariable *shaderVariable;
			GLchar *varName;

			glGetProgramiv(idProgram,GL_ACTIVE_ATTRIBUTES,&nb);
			if (nb)
			{
				glGetProgramiv(idProgram,GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,&maxLength);

				GLint varSize;
				GLenum varType;

				varName=new GLchar[maxLength];
				for (j=0;j<nb;j++)
				{
					glGetActiveAttrib(idProgram,j,maxLength,NULL,&varSize,&varType,varName);
					shaderVariable=VectVariable.Add(new SShaderVariable(varName,EVariableUniform,varType,varSize,glGetAttribLocation(idProgram,varName)));
					MapVariable[shaderVariable->Name]=shaderVariable;
					//GL_FLOAT, GL_FLOAT_VEC2, GL_FLOAT_VEC3, GL_FLOAT_VEC4, GL_FLOAT_MAT2, GL_FLOAT_MAT3, GL_FLOAT_MAT4, GL_FLOAT_MAT2x3, GL_FLOAT_MAT2x4, GL_FLOAT_MAT3x2, GL_FLOAT_MAT3x4, GL_FLOAT_MAT4x2, or GL_FLOAT_MAT4x3
				}
				delete[] varName;
			}

			glGetProgramiv(idProgram,GL_ACTIVE_UNIFORMS,&nb);
			if (nb)
			{
				glGetProgramiv(idProgram,GL_ACTIVE_UNIFORM_MAX_LENGTH,&maxLength);

				GLint varSize;
				GLenum varType;

				varName=new GLchar[maxLength];
				for (j=0;j<nb;j++)
				{
					glGetActiveUniform(idProgram,j,maxLength,NULL,&varSize,&varType,varName);
					shaderVariable=VectVariable.Add(new SShaderVariable(varName,EVariableUniform,varType,varSize,glGetUniformLocation(idProgram,varName)));
					MapVariable[shaderVariable->Name]=shaderVariable;
				}
				delete[] varName;
			}
		}
	}

	return(State==EProgramValid);
}

bool OpenUtility::CShaderProgram::GetOpenglLog()
{
	int infologLength=0;
	int charsWritten=0;

	glGetProgramiv(idProgram,GL_INFO_LOG_LENGTH,&infologLength);
	if (infologLength>1)
	{
		CurLog.AllocBuffer(infologLength+1);
		glGetProgramInfoLog(idProgram,infologLength,&charsWritten,CurLog);
		CurLog[charsWritten]='\0';
		return(true);
	}
	return(false);
}

bool OpenUtility::CShaderProgram::IsValidProgramState()
{
	if (State==EProgramNotValid) return(false);
	if (State==EProgramModified) LinkProgram();
	return(State==EProgramValid);
}

bool OpenUtility::CShaderProgram::UseProgram()
{
	if (!IsValidProgramState()) return(false);
	glUseProgram(idProgram);
	return(true);
}

void OpenUtility::CShaderProgram::UnUseProgram()
{
	glUseProgram(0);
}

GLint OpenUtility::CShaderProgram::GetVariableId(const char *str)
{
	if (IsValidProgramState())
	{
		SShaderVariable *ShaderVariable;

		if (!MapVariable.Lookup(str,(void*&)ShaderVariable)) THROW(Exception,Exception::EErrIdNotFound,str);
		return(ShaderVariable->Id);
	}
	THROW(Exception,Exception::EErrNotValid);
	return(0);
}
