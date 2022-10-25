R"(

#version 330
// ShaderToy --> OpenGL/GLSL Convert Preprocessor /////////////////////////////
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

// Shared Preprocessor ////////////////////////////////////////////////////////
#define pi 3.14159265
#define rot(a) mat2(cos(a),-sin(a),sin(a),cos(a))
#define dmin 0.0003
#define tmax 30.0
#define ldir vec3( -0.48666426339228763, 0.8111071056538127, 0.3244428422615251 )

// Gloabal Valiable
vec3 g_ro;
int CameraIndex;
float ln; // LoopNum
vec2 g_HexDomain;

// Useful Function ////////////////////////////////////////////////////////////

struct mapr // MapResult
{
   float d; // Distance
   bool  hit;
   int   m; // MaterialType
            // 0: Debug
            // 1: Common
            // 2: CyberPunk
};

void compm(inout mapr mr,float d,int mt,bool IsMin) // CompareMap
{
    if(IsMin)
    {
        if(d<mr.d) mr = mapr(d,false,mt);
        if(mr.d<dmin) mr.hit=true; 
    }
    else
    {
        if(d>mr.d) mr = mapr(d,false,mt);
        if(mr.d<dmin) mr.hit=true; 
    }
}

vec3 trs(vec3 p,vec3 s,vec3 r,vec3 t)
{
    p+=t; 
    p.yz*=rot(s.x);p.xz*=rot(s.y);p.xy*=rot(s.z);
    p*=s;
    
    return p;
}

float modc(float a, float b){return a-b*floor(a/b);}

// Noise Function /////////////////////////////////////////////////////////////
float rand(vec2 p)
{
    return fract( sin(dot(p,vec2(12.9898,78.233)))*43758.5453123 );
}

// Distance Function
float cube(vec3 p,vec3 s)
{
    return length(max(vec3(0.0), abs(p)-s));
}

float sdHex(vec3 p,vec2 h)
{
    vec3 k = vec3(-0.8660254,0.57735,0.5);
    p = abs(p);
    p.xz -= 2.0 * min(dot(k.xz,p.xz), 0.0) * k.xz;
    vec2 d = vec2(
      length(p.xz - vec2(clamp(p.x, -k.y * h.x, k.y * h.x), h.x)) *sign(p.z - h.x),
      p.y - h.y
    ); 
    
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0));
}

float deHexTiling(vec3 p, float radius, float scale,float offV,out vec2 HexDomain)
{
    vec2 rep = vec2(2.0 * sqrt(3.0), 2.0) * radius;
    vec2 hexP = mod(p.xz + offV * rep, rep) - rep * 0.5;

    HexDomain = floor((p.xz + rep * offV) / rep);
    return sdHex(vec3(hexP.x,p.y,hexP.y), vec2(scale * radius, 0.1));
}

// Ray Function ///////////////////////////////////////////////////////////////
mapr map(vec3 p)
{
    mapr mr;
    mr.d=1000.0;
    mr.hit=false;
    mr.m=-1;
    
    vec3 off = vec3(0.0,2.0,0.0);
    
    //{compm(mr,length(trs(p,vec3(1.0),vec3(0.0),vec3(0.0)))-0.5,0,true);}
    // downside
    {
        vec2 HexDomain = vec2(0.0);
        float d = deHexTiling(p - off, 1.0, 0.9, 0.0, HexDomain);
        float h = rand(HexDomain) * 0.35; 
        //+ -(max(0.0, distance(vec2(0.0), HexDomain)));
        compm(mr, d /*max(d, p.y + h)*/, 0,true);
    }
    
    {
        vec2 HexDomain = vec2(0.0);
        float d = deHexTiling(p - off, 1.0, 0.9, 0.5, HexDomain);
        float h = rand(HexDomain) * 0.35 ;
        //+ -(max(0.0, distance(vec2(0.0), HexDomain)));
        compm(mr, d /*max(d, p.y + h)*/, 0,true);
    }
    
    // upside
    {
        vec2 HexDomain = vec2(0.0);
        float d = deHexTiling(p + off, 1.0, 0.9, 0.0, HexDomain);
        float h = rand(HexDomain) * 0.35; 
        //+ -(max(0.0, distance(vec2(0.0), HexDomain)));
        compm(mr, d /*max(d, p.y + h)*/, 0,true);
    }
    
    {
        vec2 HexDomain = vec2(0.0);
        float d = deHexTiling(p + off, 1.0, 0.9, 0.5, HexDomain);
        float h = rand(HexDomain) * 0.35 ;
        //+ -(max(0.0, distance(vec2(0.0), HexDomain)));
        compm(mr, d /*max(d, p.y + h)*/, 0,true);
    }
    
    return mr;
}

vec3 gn(vec3 p)
{
    vec2 e=vec2(0.001,0.0);
    return normalize(vec3(
        map(p+e.xyy).d-map(p-e.xyy).d,
        map(p+e.yxy).d-map(p-e.yxy).d,
        map(p+e.yyx).d-map(p-e.yyx).d
    ));
}

// Lighting ///////////////////////////////////////////
vec3 hsv2rgb2(vec3 c, float k) {
    return smoothstep(0. + k, 1. - k,
        .5 + .5 * cos((vec3(c.x, c.x, c.x) + vec3(3., 2., 1.) / 3.) * radians(360.)));
}

// https://github.com/i-saint/RaymarchingOnUnity5/blob/master/Assets/Raymarching/Raymarcher.shader
vec2 DrawPattern(vec2 p)
{
    p=fract(p);
    float r = 0.123;
    float v=0.0,g=0.0;
    r=fract(r*9184.928);
    float cp,d;
    
    d=p.x;
    g+=pow(clamp(1.0-abs(d), 0.0, 1.0), 1000.0);
    d=p.y;
    g+=pow(clamp(1.0-abs(d), 0.0, 1.0), 1000.0);
    d=p.x - 1.0;
    g+=pow(clamp(3.0-abs(d), 0.0, 1.0), 1000.0);
    d=p.y - 1.0;
    g+=pow(clamp(1.0-abs(d), 0.0, 1.0), 10000.0);
    
    const int ITER = 12;
    for(int i=0; i<ITER; i++)
    {
      cp=0.5+(r-0.5)*0.9;
      d=p.x-cp;
      g+=pow(clamp(1.0-abs(d), 0.0, 1.0), 200.0);
      if(d>0.0)
      {
          r=fract(r*4829.013);
          p.x=(p.x-cp)/(1.0-cp);
          v+=1.0;
      }
      else
      {
          r=fract(r*1239.528);
          p.x=p.x/cp;
      }
      p=p.yx;
    }
    
    v/=float(ITER);
    return vec2(g,v);
}

float calcAo(in vec3 p,in vec3 n)
{
    float k=1.0, occ=0.0;
    for(int i=0; i<5; i++)
    {
        float len = 0.15 + float(i) * 0.15;
        float d = map(n*len + p).d;
        occ += (len - d) * k;
        k*=0.5;
    }
    
    return clamp(1.0-occ, 0.0, 1.0);
}

// Main ///////////////////////////////////////////////////////////////////////
void main()
{
if(_RenderingTarget==2) // ZTest
{
    gl_FragColor = vec4(vec3(0.0),1.0);
}
else
{
#ifdef DRAW_ON_SHADERTOY
    vec2 st=(gl_FragCoord.xy*2.-_resolution.xy)/min(_resolution.x,_resolution.y);
#else
    vec2 st=uv*2.0-1.0;
    st.x*=(_resolution.x/_resolution.y);
#endif
    float zfactor=1.0;
    CameraIndex = 0;
    ln = 128.0;
  
    vec3 col = vec3(0.0),ro=vec3(0.0,0.0,1.5),ta=vec3(0.0,0.0,0.0);
    if(CameraIndex == 0)
    {
        float h = 0.5;
        ta = vec3(0.0, h, 0.0);
        ro = vec3(2.0,h,2.0);
    }
    
    vec3 cdir=normalize(ta-ro),cside=normalize(cross(vec3(0.0,1.0,0.0),cdir)),
    cup=normalize(cross(cdir,cside)),
    rd=normalize(st.x*cside+st.y*cup+zfactor*cdir);
    
    //
    g_ro = ro;
    
    float i=0.0,t=0.0,som=0.0,acc=0.0;mapr mr; // SumOfStep
    for(;++i<ln;){mr=map(ro+rd*(t+=mr.d));if(mr.d<dmin||t>tmax)break;acc+=exp(-3.0*mr.d);}
    som=i/float(ln);
    
    if(mr.hit && mr.m == 0)
    {
        vec3 p = ro+rd*t;
        vec3 n = gn(p);
        
        //
        float diff = max(0.0, dot(n,ldir));
        col = vec3(1.0) * diff;
        
        //
        float spec=pow(clamp(dot(reflect(ldir, n), rd) , 0.0, 1.0), 10.0);
        col += vec3(1.0) * spec; 
         
        // 
        float glow=0.0;
        vec2 gp=DrawPattern(p.xz*0.5);
        //glow+=gp.x*max((modc(length(p)-_time*1.5*2.5, 10.0)-9.0)*2.5, 0.0);
        glow+=gp.x;
            
        if(gp.x<1.3){glow=0.0;}
        // ƒŒƒC‚Æ‚’¼‚Å‚ ‚é‚Ù‚ÇŒõ‚ç‚¹‚é
        glow+=max(1.0-abs(dot(rd,n)) - 0.4, 0.0) * 1.0;
        vec3 glowcol=vec3(1.0,1.0,1.0)*glow*0.25;
        
        //col+=glowcol;
        
        //
        float ao = calcAo(p, n);
        col += ao * vec3(1.0) * 0.1;
        
        //col = n;
    }
    
    gl_FragColor = vec4(col,1.0);
}

}

)"