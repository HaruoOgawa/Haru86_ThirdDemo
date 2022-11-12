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
uniform float _Scale;
uniform float _rOffRange;
uniform int _UseToTanScale;

layout(location=0)in vec3 vertex;
layout(location=1)in vec3 normal;
layout(location=2)in vec2 texcoord;

layout(location=0) out vec2 out_uv;
layout(location=1) out vec4 out_WorldVertexPos;
layout(location=2) out vec4 out_WorldNormal;
layout(location=3) flat out int  out_gl_InstanceID;
layout(location=4) out vec4 out_tangent;

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

#define rot(a) mat2(cos(a),sin(a),-sin(a),cos(a))
#define pi 3.14159265

float rand(vec2 st){
    return fract(
        sin(dot(st.xy,vec2(12.9898,78.233)))*43758.5453123
    );
}

vec3 RandRotate(vec3 p,float id, inout vec3 normal)
{
	vec3 pos = p;
	vec3 randRot = vec3(
		(rand(vec2(6.3666, 1.010) * id) * 2.0 - 1.0) * pi,
		(rand(vec2(888.88, 4.55) * id) * 2.0 - 1.0) * pi,
		(rand(vec2(0.0025 , 97.52) * id) * 2.0 - 1.0) * pi
	);

	pos.yz*=rot(randRot.x);
	pos.xz*=rot(randRot.y);
	pos.xy*=rot(randRot.z);
	
	normal.yz*=rot(randRot.x);
	normal.xz*=rot(randRot.y);
	normal.xy*=rot(randRot.z);

	return pos;
}

void main()
{
	vec4 pos=vec4(vertex,1.0);
	vec3 oNormal = normal;

	//
	float id=float(gl_InstanceID);
	int NumOfPoints = _TrailNum * _LineSegment;
	vec4 pos0,pos1;

	int randIDA = int(floor(rand(vec2(3.53535, id)) * float(NumOfPoints - 1)));
	STrs trs = out_trs_buffer.trs[randIDA];
	int group = trs.param.x;
	int	 my_segment = trs.param.y;
	int	 now_segment = trs.param.z;
	pos0 = trs.pos;
	
	int randIDB = (my_segment == now_segment)? randIDA : group * _LineSegment + int(mod(float(my_segment) + 1.0, float(_LineSegment)));
	pos1 = out_trs_buffer.trs[randIDB].pos;
	vec3 tangent = normalize(pos1.xyz - pos0.xyz);

	//
	float PosMixVal = rand(vec2(35.3535,1.1111) * id);
	vec4 randPos = vec4(mix(pos0.xyz, pos1.xyz, PosMixVal) ,1.0);

	if(length(pos1.xyz - pos0.xyz) != 0.0 && _rOffRange != 0.0)
	{
		
		float randSign = sign( rand(vec2(6.666,9.999)*id)*2.0-1.0 );
		float rOffVal = _rOffRange * rand(vec2(4.545,1.91919) * id); 
		vec3 randDir = normalize(cross(vec3(0.0, 1.0, 0.0), tangent)) * randSign * rOffVal;
		randPos.xyz += randDir;
	}
	
	vec3 ToTanScale = vec3(1.0);
	if(_UseToTanScale == 1)
	{
		//ToTanScale = vec3(10.0, 1.0 ,1.0);
		//ToTanScale = tangent * 5.0;
	}

	//
	pos.xyz *= _Scale * ToTanScale;
	if(_UseToTanScale == 1)
	{
		
	}
	pos.xyz = RandRotate(pos.xyz, id, oNormal); 
	pos.xyz += randPos.xyz;

	gl_Position=MVPMatrix*pos;
	out_uv=texcoord;
	out_WorldVertexPos=MMatrix * pos;
	out_WorldNormal=MMatrix * vec4(normalize(oNormal), 0.0);
	out_gl_InstanceID = int(gl_InstanceID);
	out_tangent = MMatrix * vec4(tangent, 0.0);
}

)"