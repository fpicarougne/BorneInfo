precision mediump float;

//uniform vec3 u_LightDir;
//uniform vec3 u_LightColor;
uniform sampler2D u_texId;
uniform vec4 u_GlobalColor;

varying vec3 v_Normals;
varying vec4 v_Color;
varying vec2 v_texCoords;

void main()
{
//	vec3 LNorm = normalize(u_LightDir);
//	vec3 normal = normalize(u_Normals);
//	float intensity = max(dot(LNorm, normal),0.0);
	vec4 texColor = texture2D(u_texId, v_texCoords);
//	vec3 calcColor = vec3(0.2,0.2,0.2) + u_LightColor * intensity;
//	gl_FragColor = vec4(texColor.rgb * calcColor, texColor.a);
//	gl_FragColor = v_Color;
//	if (texColor.r<0.01) discard;
	gl_FragColor = texColor*u_GlobalColor;
}
