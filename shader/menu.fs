precision mediump float;

uniform float u_Alpha;
uniform float u_BorderX;
uniform float u_BorderY;
uniform lowp sampler2D u_texId;

varying vec4 v_Color;
varying vec3 v_Normal;
varying lowp vec2 v_Tex;
varying vec2 v_Id;

void main()
{
	vec4 color=v_Color;
	float alpha=1.0;

	if (gl_FrontFacing)
	{
		if (v_Id.x<u_BorderX) alpha=min(alpha,v_Id.x/u_BorderX);
		if (v_Id.x>(1.0-u_BorderX)) alpha=min(alpha,(1.0-v_Id.x)/u_BorderX);
		if (v_Id.y<u_BorderY) alpha=min(alpha,v_Id.y/u_BorderY);
		if (v_Id.y>(1.0-u_BorderY)) alpha=min(alpha,(1.0-v_Id.y)/u_BorderY);

		if (alpha<0.5) color=vec4(0.0,0.0,0.0,alpha*2.0*alpha*2.0);
		else if (alpha<1.0) color=vec4(mix(color.rgb,vec3(0.0,0.0,0.0),(2.0-alpha*2.0)*(2.0-alpha*2.0)),color.a);
	}
	else
	{
		color=texture2D(u_texId, v_Tex);
//		color=vec4(1.0,v_Tex.x,v_Tex.y,1.0);
	}

	gl_FragColor = vec4(color.rgb,color.a*u_Alpha);
}
