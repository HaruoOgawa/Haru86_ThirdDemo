R"(

#version 330

in vec2 uv;
in vec3 WorldVertexPos;
in vec3 WorldNormal;

uniform sampler2D _MainTex;
uniform vec4 _Color;

void main(){
	vec4 col=vec4(0.0);
	vec4 TexCol = texture(_MainTex,vec2(uv.x,1.0-uv.y));
	col = _Color * TexCol.r;
	gl_FragColor=col;
}

)"