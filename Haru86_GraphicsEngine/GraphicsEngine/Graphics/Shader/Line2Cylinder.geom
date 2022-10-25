R"(

#version 330
#extension GL_ARB_separate_shader_objects : enable

layout(lines) in;
layout(triangle_strip,max_vertices=24) out;

layout(location=0) in vec2 in_uv[];
layout(location=1) in vec4 in_WorldVertexPos[];
layout(location=2) in vec4 in_WorldNormal[];

layout(location=0) out vec2 out_uv;
layout(location=1) out vec4 out_WorldVertexPos;
layout(location=2) out vec4 out_WorldNormal;

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 PMatrix;
uniform float _time;
uniform float _deltaTime;
uniform vec3 _CameraPos;

uniform float _Radius;
uniform float _CircleSegment; // ‚Ğ‚Æ‚Ü‚¸‰~‚Ì•ªŠ„”‚Ì‚±‚ê‚¾‚¯‚Å‚¢‚¢‚©‚ÈH
uniform float _TangentSegment; // tangent•ûŒü‚Ì•ªŠ„”

#define pi 3.14159265

void main()
{
	vec4 pos0 = gl_in[0].gl_Position;
	vec4 pos1 = gl_in[1].gl_Position;
	vec3 TangentVector = pos1.xyz - pos0.xyz;
	vec3 tangent = normalize(TangentVector);
	float TangentLength = length(TangentVector);

	vec3 normal = normalize(cross(vec3(0.0, 1.0, 0.0), tangent));
	vec3 bionormal = normalize(cross(tangent, normal));

	for(float n=0.0; n<_CircleSegment; n++)
	{
		float a0 = (2.0*pi/_CircleSegment) * n;
		float a1 = (2.0*pi/_CircleSegment) * (n+1.0);
		
		vec4 n0 = vec4(normalize( cos(a0)*bionormal + sin(a0)*normal), 0.0);
		vec4 n1 = vec4(normalize( cos(a1)*bionormal + sin(a1)*normal), 0.0);

		vec4 p0 = _Radius * n0 + pos0;
		vec4 p1 = _Radius * n1 + pos0;
		vec4 p2 = _Radius * n0 + pos1;
		vec4 p3 = _Radius * n1 + pos1;
		
		gl_Position = MVPMatrix * p0;
		out_uv = in_uv[0];
		out_WorldVertexPos = MMatrix * p0;
		out_WorldNormal = MMatrix * n0;
		EmitVertex();
	
		gl_Position = MVPMatrix * p1;
		out_uv = in_uv[1];
		out_WorldVertexPos = MMatrix * p1;
		out_WorldNormal = MMatrix * n1;
		EmitVertex();
		
		gl_Position = MVPMatrix * p2;
		out_uv = in_uv[0];
		out_WorldVertexPos = MMatrix * p2;
		out_WorldNormal = MMatrix * n0;
		EmitVertex();
	
		gl_Position = MVPMatrix * p3;
		out_uv = in_uv[1];
		out_WorldVertexPos = MMatrix * p3;
		out_WorldNormal = MMatrix * n1;
		EmitVertex();

		EndPrimitive();
	}

	////////////////////////
	/*gl_Position = MVPMatrix * pos0;
	out_uv = in_uv[0];
	out_WorldVertexPos = MMatrix * pos0;
	out_WorldNormal = MMatrix * in_WorldNormal[0];
	EmitVertex();
	
	gl_Position = MVPMatrix * pos1;
	out_uv = in_uv[1];
	out_WorldVertexPos = MMatrix * pos1;
	out_WorldNormal = MMatrix * in_WorldNormal[1];
	EmitVertex();

	EndPrimitive();*/
}

)"