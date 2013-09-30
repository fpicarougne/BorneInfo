attribute vec4 vPos;
attribute vec3 vNorm;
attribute vec4 vColor;
attribute vec2 vTex;
attribute vec2 vId;

uniform mat4 u_MVPmatrix;
uniform mat4 u_Nmatrix;

varying vec4 v_Color;
varying vec3 v_Normal;
varying vec2 v_Tex;
varying vec2 v_Id;

void main()
{
	v_Color = vColor;
	v_Tex = vTex;
	v_Id = vId;
	v_Normal = mat3(u_Nmatrix) * vNorm;
	gl_Position = u_MVPmatrix * vPos;
}
