R"(

#version 410

in vec2 uv;

uniform float _time;
uniform float _deltaTime;
uniform vec2 _resolution;
uniform sampler2D _SrcTexture;
uniform sampler2D _NormalMap;
uniform sampler2D _DepthMapPolygone;
uniform sampler2D _DepthMapRaymarch;
uniform sampler2D _DepthMapMixed;
uniform float _frameResolusion;
uniform vec3 _WorldCameraPos;
uniform vec3 _WorldCameraCenter;

//PostEffectValue///////////////////////////////////

uniform int	  _UseWhite;
uniform float _WhiteVal;

vec3 DrawWhite(in vec3 col)
{
	col+=vec3(1.0)*_WhiteVal;
	return col;
}

void main(){
	vec3 col=vec3(0.0);
	//vec2 st=gl_FragCoord.xy/_resolution.xy;
	vec2 st=uv*_frameResolusion;
	
	col=texture(_SrcTexture,st).rgb;
	
	if(_UseWhite == 1) col = DrawWhite(col);

	gl_FragColor=vec4(col,1.0);
}

)"