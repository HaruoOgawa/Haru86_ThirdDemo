R"(

#version 330

// ShaderToy --> OpenGL/GLSL
//#define DRAW_ON_SHADERTOY

#ifdef DRAW_ON_SHADERTOY
#define _resolution iResolution
#define gl_FragCoord fragCoord
#define gl_FragColor fragColor
#define _time iTime
#define main() mainImage( out vec4 fragColor, in vec2 fragCoord )
const int _RenderingTarget = 1;
const float _LeaveStartTime = 1.0;
#else
uniform float _time;
uniform vec2 _resolution;
uniform float _RenderingTarget;
uniform vec3 _WorldCameraPos;
uniform vec3 _WorldCameraCenter;

uniform float _LeaveStartTime;

in vec2 uv;
#endif

#define pi 3.14159265
#define rot(a) mat2(cos(a),-sin(a),sin(a),cos(a))

float random(in vec2 st)
{
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

// Get noise
float noise(in vec2 st)
{
    // Splited integer and float values.
    vec2 i = floor(st);
    vec2 f = fract(st);

    float a = random(i + vec2(0.0, 0.0));
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    // -2.0f^3 + 3.0f^2
    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

float fbm(vec2 p)
{
    mat2 m=rot(35.6*pi/180.0);
    float f=0.0,ASum=0.0;
    for(float i=0.0;i<4.0;i++)
    {
        float Att = pow(0.5,i+1.0) ;
        f+=Att*noise(p);p=2.0*m*p;
        ASum+=Att;
    }
    
    return f/ASum;
}

float hash( float n ) { return fract(sin(n)*753.5453123); }
float noise3( in vec3 x )
{
    vec3 p = floor(x);
    vec3 f = fract(x);
    f = f*f*(3.0-2.0*f);
	
    float n = p.x + p.y*157.0 + 113.0*p.z;
    return mix(mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
                   mix( hash(n+157.0), hash(n+158.0),f.x),f.y),
               mix(mix( hash(n+113.0), hash(n+114.0),f.x),
                   mix( hash(n+270.0), hash(n+271.0),f.x),f.y),f.z);
}


float fbm3(vec3 p,float num,float A)
{
    float w=0.0,asum=0.0;
    for(float i=0.0;i<num;i++)
    {
        float Att = pow(0.5,i+1.0) * A;
        w+=Att*noise3(p);
        p=2.0*p;
        asum+=Att;
    }
    
    return w/asum;
}

vec3 star(vec3 rd){return vec3( smoothstep(0.8,0.95,fbm((100.0*rd.xy)/rd.z)) );}

vec3 Draw3DClouds(vec3 ro,vec3 rd)
{
    vec3 col = vec3(0.0),skycol=vec3(0.0);
 
    // ‰_
    vec3 clouds = vec3(0.0);
    float s = 0.25;
    for(int i=0;i<3;i++)
    {
        clouds+=fbm3(rd*5.0+vec3(0.0,0.0,0.0),5.0,1.0);
        s*=1.35;
    }
    
    //
    col = skycol + 0.1*clouds + star(rd);
    return col;
}

void main()
{
if(_RenderingTarget==2) // ZTest
{
    gl_FragColor = vec4(vec3(0.0),1.0);
}
else
{
#ifdef DRAW_ON_SHADERTOY
    vec2 st = (gl_FragCoord.xy*2.0-_resolution.xy)/min(_resolution.x,_resolution.y);
#else
    vec2 st=uv*2.0-1.0;
    st.x*=(_resolution.x/_resolution.y);
#endif
    vec3 col = vec3(0.0),ro=_WorldCameraPos,ta=_WorldCameraCenter,
    cdir=normalize(ta-ro),cside=normalize(cross(vec3(0.0,1.0,0.0),cdir)),cup=normalize(cross(cdir,cside)),
    rd=normalize(st.x*cside+st.y*cup+1.0*cdir);
    col = Draw3DClouds(ro,rd) * 0.25;
    
    gl_FragColor = vec4(col,1.0);
}

}

)"