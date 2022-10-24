R"(


#version 330
#extension GL_ARB_separate_shader_objects : enable

layout(triangles) in;
layout(triangle_strip,max_vertices=8) out;

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

void main()
{
	gl_Position = MVPMatrix * gl_in[0].gl_Position;
	out_uv = in_uv[0];
	out_WorldVertexPos = MMatrix * in_WorldVertexPos[0];
	out_WorldNormal = MMatrix * in_WorldNormal[0];
	EmitVertex();

	gl_Position = MVPMatrix * gl_in[1].gl_Position;
	out_uv = in_uv[1];
	out_WorldVertexPos = MMatrix * in_WorldVertexPos[1];
	out_WorldNormal = MMatrix * in_WorldNormal[1];
	EmitVertex();

	gl_Position = MVPMatrix * gl_in[2].gl_Position;
	out_uv = in_uv[2];
	out_WorldVertexPos = MMatrix * in_WorldVertexPos[2];
	out_WorldNormal = MMatrix * in_WorldNormal[2];
	EmitVertex();

	EndPrimitive();
}

)"