R"(

#version 430
#extension GL_ARB_separate_shader_objects : enable

layout(location=0) in vec2 in_uv;
layout(location=1) in vec4 in_WorldVertexPos;
layout(location=2) in vec4 in_WorldNormal;
layout(location=3) flat in int  in_gl_InstanceID;

out vec4 out_Color;

////////////////////////
float random (in vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898,78.233)))* 43758.5453123);
}

float noise (in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f*f*(3.0-2.0*f);
    return mix(a, b, u.x) +
            (c - a)* u.y * (1.0 - u.x) +
            (d - b) * u.x * u.y;
}

#define OCTAVES 6
float fbm (in vec2 st) {
    float value = 0.0;
    float amplitude = .5;
    float frequency = 0.;
  
    for (int i = 0; i < OCTAVES; i++) {
        value += amplitude * noise(st);
        st *= 2.;
        amplitude *= .5;
    }
    return value;
}

void main(){
	vec2 uv = in_uv;
    
    float id = float(in_gl_InstanceID) * 10.0;
    float n = fbm(uv*10.0 + id);
    vec4 col = vec4(n);
    
    vec2 st = uv *2.0 -1.0;
    col *= smoothstep(0.0, 1.0, exp(-(4.5 + 6.0*(n*2.0-1.0)) * length(st)) );
    col.a = (col.a >= 0.01)? min(1.0, col.a*10.0) : col.a;
    //col = vec4(vec3(0.0), 1.0);

	out_Color=col;
}

)"