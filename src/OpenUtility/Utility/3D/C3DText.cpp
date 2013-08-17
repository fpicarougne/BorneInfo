#include "C3DText.h"

OpenUtility::C3DText::SShaders *OpenUtility::C3DText::DefaultShader=NULL;
unsigned int OpenUtility::C3DText::DefShaderInUse=0;

static const char FontVertex[]="\
attribute vec4 vPos;\n\
attribute vec2 vTexCoord;\n\
\n\
uniform mat4 u_MVPmatrix;\n\
\n\
varying vec2 v_texCoords;\n\
\n\
void main()\n\
{\n\
	v_texCoords = vTexCoord;\n\
	gl_Position = u_MVPmatrix * vPos;\n\
}\n\
";

static const char FontFragment[]="\
precision mediump float;\n\
\n\
uniform sampler2D u_texId;\n\
uniform vec4 vColor;\n\
\n\
varying vec2 v_texCoords;\n\
\n\
void main()\n\
{\n\
	vec4 texColor = texture2D(u_texId, v_texCoords);\n\
	gl_FragColor = vec4(vColor.rgb, texColor.r*vColor.a);\n\
}\n\
";

OpenUtility::C3DText::C3DText(const OpenUtility::CFontLoader *loader,double lineHeight,bool useDefaultShader) :
	CFontLoader::IFontEngine(loader),
	IsDefaultShaderInUse(false),
	DefColor(1.0,1.0,1.0,1.0),
	ReqLineHeight(lineHeight),
	VertexTab(NULL),
	ElementTab(NULL)
{
	UseDefaultShader(useDefaultShader);
	CommonInit();
}

OpenUtility::C3DText::C3DText(const CFontLoader *loader,const double r,const double g,const double b,const double a,double lineHeight) :
	CFontLoader::IFontEngine(loader),
	IsDefaultShaderInUse(false),
	DefColor(r,g,b,a),
	ReqLineHeight(lineHeight),
	VertexTab(NULL),
	ElementTab(NULL)
{
	UseDefaultShader(true);
	CommonInit();
}

OpenUtility::C3DText::C3DText(const C3DText &obj) :
	CFontLoader::IFontEngine(obj),
	ReqLineHeight(obj.ReqLineHeight),
	VertexTab(NULL),
	ElementTab(NULL)
{
	UseDefaultShader(obj.IsDefaultShaderInUse);
	CommonInit();
	SetText(obj.Text.GetStream(),obj.CurrentHAlign,obj.CurrentVAlign);
}

void OpenUtility::C3DText::CommonInit()
{
	glGenBuffers(1,&VBObuffer);
	glGenBuffers(1,&VBIBuffer);
	CurrentHAlign=EHAlignLeft;
	CurrentVAlign=EVAlignBaseligne;
}

OpenUtility::C3DText::~C3DText()
{
	UseDefaultShader(false);
	glDeleteBuffers(1,&VBObuffer);
	glDeleteBuffers(1,&VBIBuffer);
	delete[] VertexTab;
	delete[] ElementTab;
}

OpenUtility::C3DText& OpenUtility::C3DText::operator=(const C3DText &obj)
{
//TODO:
	return(*this);
}

void OpenUtility::C3DText::UseDefaultShader(bool useDefShader)
{
	if (useDefShader)
	{
		if (!DefaultShader)
		{
			DefaultShader=new SShaders;
			if (!DefaultShader->ShaderVertex.LoadStream(FontVertex))
				std::cout << "-----------------------------------\nCTaskBar: Erreur vertex shader :\n" << DefaultShader->ShaderVertex.GetLog() << std::endl << "--------------------------" << std::endl;
			if (!DefaultShader->ShaderFragment.LoadStream(FontFragment))
				std::cout << "-----------------------------------\nCTaskBar: Erreur fragment shader :\n" << DefaultShader->ShaderFragment.GetLog() << std::endl << "--------------------------" << std::endl;
			if (!DefaultShader->RenderingShader.LinkProgram())
				std::cout << "-----------------------------------\nCTaskBar: Erreur shader program :\n" << DefaultShader->RenderingShader.GetLog() << std::endl << "--------------------------" << std::endl;
		}
		if (!IsDefaultShaderInUse) DefShaderInUse++;
	}
	else
	{
		if (IsDefaultShaderInUse)
		{
			DefShaderInUse--;
			if (DefShaderInUse==0)
			{
				delete DefaultShader;
				DefaultShader=NULL;
			}
		}
	}
	IsDefaultShaderInUse=useDefShader;
}

void OpenUtility::C3DText::SetText(const char *text,EHAlign hAlign,EVAlign vAlign)
{
	CurrentHAlign=hAlign;
	CurrentVAlign=vAlign;
	UpdateText(text);
}

void OpenUtility::C3DText::UpdateText(const char *text)
{
	unsigned int i;
	int currentX,currentY;
	double size=FontLoader->GetSize()/ReqLineHeight;
	const CTexture* texture=FontLoader->GetFontTexture();

	Text=text;
	delete[] VertexTab;
	delete[] ElementTab;
	VertexTab=new SVertex[Text.GetSize()*4];
	ElementTab=new GLubyte[Text.GetSize()*6];

	TotalX=0;
	MinY=0;
	MaxY=0;
	for (i=0;i<Text.GetSize();i++)
	{
		const CFontLoader::SFontChar *fontData=FontLoader->GetCharData(text[i]);
		TotalX+=fontData->Advance;
		if (MinY>(currentY=fontData->BearingY-fontData->texH)) MinY=currentY;
		if (MaxY<(currentY=fontData->BearingY)) MaxY=currentY;
	}

	currentX=0;
	for (i=0;i<Text.GetSize();i++)
	{
		const CFontLoader::SFontChar *fontData=FontLoader->GetCharData(text[i]);
		double x,y;

		switch(CurrentHAlign)
		{
		case EHAlignLeft:x=currentX;break;
		case EHAlignCenter:x=currentX-TotalX/2.0;break;
		case EHAlignRight:x=currentX-TotalX;break;
		default:x=0;
		}
		x=(x+fontData->BearingX)/size;
		currentX+=fontData->Advance;

		switch(CurrentVAlign)
		{
		case EVAlignBottom:y=-MinY;break;
		case EVAlignBaseligne:y=0;break;
		case EVAlignMiddle:y=-((MaxY-MinY)/2.0+MinY);break;
		case EVAlignTop:y=-MaxY;break;
		default:y=0;
		}
		y=(y-fontData->texH+fontData->BearingY)/size;

		SetVertex(VertexTab[i*4],x,y,
								fontData->texX/double(texture->GetWT()),(fontData->texY+fontData->texH)/double(texture->GetHT()));
		SetVertex(VertexTab[i*4+1],x+fontData->texW/size,y,
								(fontData->texX+fontData->texW)/double(texture->GetWT()),(fontData->texY+fontData->texH)/double(texture->GetHT()));
		SetVertex(VertexTab[i*4+2],x,y+fontData->texH/size,
								fontData->texX/double(texture->GetWT()),fontData->texY/double(texture->GetHT()));
		SetVertex(VertexTab[i*4+3],x+fontData->texW/size,y+fontData->texH/size,
								(fontData->texX+fontData->texW)/double(texture->GetWT()),fontData->texY/double(texture->GetHT()));

		ElementTab[i*6]=i*4;
		ElementTab[i*6+1]=ElementTab[i*6+4]=i*4+1;
		ElementTab[i*6+2]=ElementTab[i*6+3]=i*4+2;
		ElementTab[i*6+5]=i*4+3;
	}

	glBindBuffer(GL_ARRAY_BUFFER,VBObuffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(SVertex)*4*Text.GetSize(),VertexTab,GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,VBIBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLubyte)*6*Text.GetSize(),ElementTab, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

void OpenUtility::C3DText::SetAlignement(EHAlign hAlign,EVAlign vAlign)
{
	if (VertexTab)
	{
		bool isUpdated=false;

		if (CurrentHAlign!=hAlign)
		{
			double x1,x2;

			switch(hAlign)
			{
			case EHAlignLeft:x1=0;break;
			case EHAlignCenter:x1=-TotalX/2.0;break;
			case EHAlignRight:x1=-TotalX;break;
			default:x1=0;
			}

			switch(CurrentHAlign)
			{
			case EHAlignLeft:x2=0;break;
			case EHAlignCenter:x2=-TotalX/2.0;break;
			case EHAlignRight:x2=-TotalX;break;
			default:x2=0;
			}
			x1=(x1-x2)/FontLoader->GetSize();

			for (unsigned int i=0;i<Text.GetSize()*4;i++)
				VertexTab[i].position[0]+=x1;

			isUpdated=true;
		}

		if (CurrentVAlign!=vAlign)
		{
			double y1,y2;

			switch(vAlign)
			{
			case EVAlignBottom:y1=-MinY;break;
			case EVAlignBaseligne:y1=0;break;
			case EVAlignMiddle:y1=-((MaxY-MinY)/2.0+MinY);break;
			case EVAlignTop:y1=-MaxY;break;
			default:y1=0;
			}

			switch(CurrentVAlign)
			{
			case EVAlignBottom:y2=-MinY;break;
			case EVAlignBaseligne:y2=0;break;
			case EVAlignMiddle:y2=-((MaxY-MinY)/2.0+MinY);break;
			case EVAlignTop:y2=-MaxY;break;
			default:y2=0;
			}
			y1=(y1-y2)/FontLoader->GetSize();

			for (unsigned int i=0;i<Text.GetSize()*4;i++)
				VertexTab[i].position[1]+=y1;

			isUpdated=true;
		}

		if (isUpdated)
		{
			glBindBuffer(GL_ARRAY_BUFFER,VBObuffer);
			glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(SVertex)*4*Text.GetSize(),VertexTab);
			glBindBuffer(GL_ARRAY_BUFFER,0);
		}
	}

	CurrentHAlign=hAlign;
	CurrentVAlign=vAlign;
}

void OpenUtility::C3DText::AttachAttribToData(GLuint vPos,GLuint vTex)
{
	if (!IsDefaultShaderInUse) DefaultAttachAttribToData(vPos,0,vTex);
}

void OpenUtility::C3DText::AttachAttribToData(GLuint vPos,GLuint vNorm,GLuint vTex)
{
	if (!IsDefaultShaderInUse) DefaultAttachAttribToData(vPos,vNorm,vTex);
}

void OpenUtility::C3DText::DefaultAttachAttribToData(GLuint vPos,GLuint vNorm,GLuint vTex)
{
	glBindBuffer(GL_ARRAY_BUFFER,VBObuffer);
	glVertexAttribPointer(vPos,3,GL_FLOAT,GL_FALSE,sizeof(SVertex),(void*)offsetof(SVertex,position));
	glVertexAttribPointer(vTex,2,GL_FLOAT,GL_FALSE,sizeof(SVertex),(void*)offsetof(SVertex,texcoord));
	glEnableVertexAttribArray(vPos);
	glEnableVertexAttribArray(vTex);
	if (vNorm)
	{
		glVertexAttribPointer(vNorm,3,GL_FLOAT,GL_FALSE,sizeof(SVertex),(void*)offsetof(SVertex,normal));
		glEnableVertexAttribArray(vNorm);
	}
	else 
	glBindTexture(GL_TEXTURE_2D,FontLoader->GetFontTexture()->GetId());
}

void OpenUtility::C3DText::Draw()
{
	if (IsDefaultShaderInUse && DefaultShader)
	{
		DefaultShader->RenderingShader.UseProgram();
		glUniform4f(DefaultShader->RenderingShader["vColor"],DefColor.r,DefColor.g,DefColor.b,DefColor.a);
		glUniformMatrix4fv(DefaultShader->RenderingShader["u_MVPmatrix"],1,GL_FALSE,DefMVPmatrix.GetMatrix());
		DefaultAttachAttribToData(DefaultShader->RenderingShader["vPos"],0,DefaultShader->RenderingShader["vTexCoord"]);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,VBIBuffer);
	glDrawElements(GL_TRIANGLES,6*Text.GetSize(),GL_UNSIGNED_BYTE,(GLvoid*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

void OpenUtility::C3DText::SetVertex(SVertex &vertex,double posX,double posY,double texX,double texY)
{
	vertex.position[0]=posX;
	vertex.position[1]=posY;
	vertex.position[2]=0;
	vertex.normal[0]=0;
	vertex.normal[1]=0;
	vertex.normal[2]=1.0;
	vertex.texcoord[0]=texX;
	vertex.texcoord[1]=texY;
}