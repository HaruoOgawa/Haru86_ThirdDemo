R"(
#extension GL_ARB_separate_shader_objects : enable

layout(points) in;
layout(line_strip,max_vertices=2) out;

layout(location=0) in vec2 in_uv[];
layout(location=1) in vec4 in_WorldVertexPos[];
layout(location=2) in vec4 in_WorldNormal[];
layout(location=3) flat in int  in_gl_InstanceID[];

layout(location=0) out vec2 out_uv;
layout(location=1) out vec4 out_WorldVertexPos;
layout(location=2) out vec4 out_WorldNormal;
layout(location=3) flat out int out_gl_InstanceID;

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 PMatrix;
uniform float _time;
uniform float _deltaTime;
uniform vec3 _CameraPos;
uniform int _TrailNum;
uniform int _LineSegment;

struct STrs
{
	vec4 pos;
	vec4 rot;
	vec4 scl;
	vec4 tan;
	ivec4 param; // ivec4(group, my_segment, now_segment, 0)
};

layout(std430, binding = SegmentBufferBinding) buffer in_trs_buffer
{
	STrs trs[];
} out_trs_buffer;

void main()
{
	vec4 pos0,pos1;
	int id = in_gl_InstanceID[0];
	STrs trs = out_trs_buffer.trs[id];
	int group = trs.param.x;
	int	 my_segment = trs.param.y;
	int	 now_segment = trs.param.z;
	pos0 = trs.pos;

	if(my_segment == now_segment) // last check
	{
		pos1 = out_trs_buffer.trs[id].pos;
	}
	else
	{
		int nextID = group * _LineSegment + int(mod(float(my_segment) + 1.0, float(_LineSegment)));
		pos1 = out_trs_buffer.trs[nextID].pos;
	}
	
	gl_Position = MVPMatrix * pos0;
	out_uv = in_uv[0];
	out_WorldVertexPos = MMatrix * pos0;
	out_WorldNormal = MMatrix * in_WorldNormal[0];
	out_gl_InstanceID = in_gl_InstanceID[0];
	EmitVertex();
	
	gl_Position = MVPMatrix * pos1;
	out_uv = in_uv[0];
	out_WorldVertexPos = MMatrix * pos1;
	out_WorldNormal = MMatrix * in_WorldNormal[0];
	out_gl_InstanceID = in_gl_InstanceID[0];
	EmitVertex();

	EndPrimitive();
}

)"