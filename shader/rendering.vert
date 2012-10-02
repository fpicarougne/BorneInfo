attribute vec4 vPos;
attribute vec3 vNorm;
attribute vec2 vTexCoord;

uniform mat4 u_MVPmatrix;
uniform mat4 u_Nmatrix;

varying vec4 v_Color;
varying vec3 v_Normals;
varying vec2 v_texCoords;

void main()
{
	if (abs(vNorm.x)>0.9) v_Color = vec4(1.0,0.0,0.0,1.0);
	if (abs(vNorm.y)>0.9) v_Color = vec4(0.0,1.0,0.0,1.0);
	if (abs(vNorm.z)>0.9) v_Color = vec4(0.0,0.0,1.0,1.0);
	v_Normals = mat3(u_Nmatrix) * vNorm;
	v_texCoords = vTexCoord;
	gl_Position = u_MVPmatrix * vPos;
}
