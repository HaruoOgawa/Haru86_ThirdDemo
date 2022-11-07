R"(
#extension GL_ARB_separate_shader_objects : enable

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 PMatrix;
uniform float _time;
uniform float _deltaTime;
uniform vec3 _CameraPos;
uniform int _IsMulMatOnVert;

uniform int _TrailNum;
uniform int _LineSegment;

layout(location=0)in vec3 vertex;
layout(location=1)in vec3 normal;
layout(location=2)in vec2 texcoord;
//layout(location=1)in vec4 tangent;

layout(location=0) out vec2 out_uv;
layout(location=1) out vec4 out_WorldVertexPos;
layout(location=2) out vec4 out_WorldNormal;

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

float rand(vec2 st){
    return fract(
        sin(dot(st.xy,vec2(12.9898,78.233)))*43758.5453123
    );
}

void main()
{
	vec4 pos=vec4(vertex,1.0);

	//
	int id=gl_InstanceID;
	int randID = int(floor(rand(vec2(3.53535, id))*16.0));
	vec4 randPos = out_trs_buffer.trs[randID].pos;
	pos.xyz += randPos.xyz;

	gl_Position=MVPMatrix*pos;
	out_uv=vec2(0.0);
	out_WorldVertexPos=MMatrix * pos;
	out_WorldNormal=MMatrix * vec4(normalize(normal), 0.0);
}

)"