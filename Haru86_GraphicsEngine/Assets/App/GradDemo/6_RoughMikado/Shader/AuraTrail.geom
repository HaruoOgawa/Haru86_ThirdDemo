R"(

#extension GL_ARB_separate_shader_objects : enable

layout(points) in;
layout(triangle_strip,max_vertices=6) out;

layout(location=0) in vec2 in_uv[];
layout(location=1) in vec4 in_WorldVertexPos[];
layout(location=2) in vec4 in_WorldNormal[];
layout(location=3) flat in int  in_gl_InstanceID[];
layout(location=4) in vec4 in_tangent[];

layout(location=0) out vec2 out_uv;
layout(location=1) out vec4 out_WorldVertexPos;
layout(location=2) out vec4 out_WorldNormal;
layout(location=3) flat out int out_gl_InstanceID;

#define pi 3.14159265

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 PMatrix;
uniform float _time;
uniform float _deltaTime;
uniform vec3 _CameraPos;
uniform int _TrailNum;
uniform int _LineSegment;

void main()
{

	vec4 pos = gl_in[0].gl_Position;
	vec4 tangent = in_tangent[0];
	vec4 bionormal = vec4(normalize(cross(tangent.xyz, vec3(0.0,1.0,0.0))) ,0.0);
	vec4 normal = vec4(normalize(cross(tangent.xyz, bionormal.xyz)) ,1.0);
	float Scale = 1.0, a = pi/4.0, h = pi/2.0;

	vec4 pos0 = Scale * bionormal * cos(a)		     + Scale * tangent*sin(a);
	vec4 pos1 = Scale * bionormal * cos(a + h * 1.0) + Scale * tangent*sin(a + h * 1.0);
	vec4 pos2 = Scale * bionormal * cos(a + h * 2.0) + Scale * tangent*sin(a + h * 2.0);
	vec4 pos3 = Scale * bionormal * cos(a + h * 3.0) + Scale * tangent*sin(a + h * 3.0);

	gl_Position = PMatrix * VMatrix * pos0;
	out_uv = vec2(1.0, 1.0);
	out_WorldVertexPos = pos0;
	out_WorldNormal = normal;
	out_gl_InstanceID = in_gl_InstanceID[0];
	EmitVertex();
	
	gl_Position = PMatrix * VMatrix * pos1;
	out_uv = vec2(0.0, 1.0);
	out_WorldVertexPos = pos1;
	out_WorldNormal = normal;
	out_gl_InstanceID = in_gl_InstanceID[0];
	EmitVertex();
	
	gl_Position = PMatrix * VMatrix * pos2;
	out_uv = vec2(0.0, 0.0);
	out_WorldVertexPos = pos2;
	out_WorldNormal = normal;
	out_gl_InstanceID = in_gl_InstanceID[0];
	EmitVertex();
	
	gl_Position = PMatrix * VMatrix * pos3;
	out_uv = vec2(1.0, 0.0);
	out_WorldVertexPos = pos3;
	out_WorldNormal = normal;
	out_gl_InstanceID = in_gl_InstanceID[0];
	EmitVertex();

	EndPrimitive();
}

)"