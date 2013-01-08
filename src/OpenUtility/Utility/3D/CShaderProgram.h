#ifndef _CShaderProgram_h
	#define _CShaderProgram_h

#include "CShaderFile.h"
#include "../../Template/CMap.h"
#include "../../Template/CVector.h"
#include "../OUException.h"

namespace OpenUtility
{

class CShaderProgram
{
public:
	class Exception : public OpenUtility::Exception
	{
		friend class CShaderProgram;

	private:
		enum EError
		{
			EErrNotValid,
			EErrIdNotFound
		};
	
	private:
		Exception(EError err,const char *detail=NULL);

	public:
		Exception(const Exception &obj);
		~Exception() throw() {}

	protected:
		void UpdateStr();

	private:
		EError ErrType;
		CStream Detail;
	};

private:
	enum EProgramState
	{
		EProgramNotValid,
		EProgramValid,
		EProgramModified
	};

	enum EShaderVariableFamily
	{
		EVaraibleAttribute,
		EVariableUniform
	};

	struct SShaderVariable
	{
		SShaderVariable(const char *name,EShaderVariableFamily family,GLenum type,GLint size,GLint id) : Name(name),Family(family),Type(type),Size(size),Id(id) {}
		CStream Name;
		EShaderVariableFamily Family;
		GLenum Type;
		GLint Size;
		GLint Id;
	};

public:
	CShaderProgram();
	CShaderProgram(const CShaderProgram &obj);
	~CShaderProgram();
	CShaderProgram& operator=(const CShaderProgram &obj);
	void AddShaderFile(CShaderFile *shader);
	void RemoveShader(CShaderFile *shader);
	bool LinkProgram();
	bool UseProgram();
	static void UnUseProgram();
	inline const CStream& GetLog() {return(CurLog);}
	GLint GetVariableId(const char *str);
	inline GLint operator[](const char *str) {return(GetVariableId(str));}
	inline GLuint GetId() {return(idProgram);}
	inline void SetHasChanged() {State=EProgramModified;}

private:
	void Init();
	void InitCopy(const CShaderProgram &obj);
	bool GetOpenglLog();
	bool IsValidProgramState();

private:
	EProgramState State;
	CStream CurLog;
	GLuint idProgram;
	CMap<const char*,CGalStream> MapVariable;
	CVector<SShaderVariable> VectVariable;
	CListe<CShaderFile> ShaderList;
};

}

#endif
