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
#define _BufferA iChannel0
const int _RenderingTarget = 1;
const float _LeaveStartTime = 1.0;
const int _IsUseShowing = 0;
const float _ShowingFinTime = 30.0;
const float _ShowDuration = 14.0;

const float _MoveStartTime = 3.0;
const float _MoveTimeDuration = 10.0;
const int _TRSIndex = 0; 
const float _GoMoonTime = 3.0;
int _RefMapIndex = 0;
#else
uniform float _time;
uniform vec2 _resolution;
uniform float _RenderingTarget;
uniform vec3 _WorldCameraPos;
uniform vec3 _WorldCameraCenter;
uniform int _IsUseShowing;
uniform float _ShowingFinTime;
uniform float _ShowDuration;

uniform float _MoveStartTime;
uniform float _MoveTimeDuration;

uniform float _MoveH;

uniform int _TRSIndex;
uniform float _GoMoonTime;
uniform int _RefMapIndex ; // 0 ~ -1
uniform sampler2D _BufferA;

in vec2 uv;
#endif

// Shared Preprocessor ////////////////////////////////////////////////////////
#define pi 3.14159265
#define rot(a) mat2(cos(a),-sin(a),sin(a),cos(a))
#define dmin 1.0
#define tmax 3000.0
#define ldir normalize(vec3(1.0,1.0,-1.0))

// Gloabal Valiable
vec3 g_ro;
int CameraIndex;
float ln; // LoopNum
vec3 g_ShipPos;

// Useful Function ////////////////////////////////////////////////////////////

float smin(float d1,float d2,float v)
{
    float h=exp(-v*d1)+exp(-v*d2);
    return -log(h)/v;
}

struct mapr // MapResult
{
   float d; // Distance
   bool  hit;
   int   m; // MaterialType
   float t;     
   float i;
   float acc;
   vec3 jetCol;
};

void compm(inout mapr mr,float d,int mt,bool IsMin, bool IsSMin) // CompareMap
{
    if(IsMin)
    {
        if(!IsSMin)
        {
            if(d<mr.d) mr = mapr(d,false,mt,0.0,0.0,0.0,vec3(0.0));
            if(mr.d<dmin) mr.hit=true; 
        }
        else
        {
            if(d<mr.d) mr = mapr(smin(d, mr.d, 1.5),false,mt,0.0,0.0,0.0,vec3(0.0));
            if(mr.d<dmin) mr.hit=true; 
        }
    }
    else
    {
        if(d>mr.d) mr = mapr(d,false,mt,0.0,0.0,0.0,vec3(0.0));
        if(mr.d<dmin) mr.hit=true; 
    }
}

vec3 trs(vec3 p,vec3 s,vec3 r,vec3 t)
{
    p+=t;
    p.yz*=rot(r.x);p.xz*=rot(r.y);p.xy*=rot(r.z);
    p*=s;
    return p;
}

////////////////////////////////////////////////////
// Get random value
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

// https://www.shadertoy.com/view/MlfXWH
#define EPS (2.0/_resolution.x)
vec3 sky(vec3 rd,bool IsRef)
{
    //
    vec3 col = vec3(0.0),skycol=mix(vec3(0.0,0.038,0.038),vec3(0.0,0.04,0.15),(rd.y*0.5+0.5));
 
    // ¯
    vec3 star = vec3( smoothstep(0.8,0.95,fbm((100.0*rd.xy)/rd.z)) ); 
    
    // ‰_
    vec3 clouds = vec3(0.0);
    float s = 0.25;
    for(int i=0;i<3;i++)
    {
        clouds+=fbm(rd.xz/(rd.y)-s+vec2(0.0,_time*0.5));
        s*=1.35;
    }
    
    //
    col = skycol + 0.25*clouds*max(0.0,rd.y);
    col += star*max(0.0,rd.y)*2.0; 

    return col;
}

// https://www.shadertoy.com/view/XlfGRj
#define tile   0.850
vec3 StarSpace(vec3 ro,vec3 rd)
{
    vec3 col=vec3(0.);
    float t=0.1,fade=1.;
    ro.x+=1.0;
    //ro.z=mod(ro.z+_time*0.1,6.0)-3.0;
    
    for(int m=0;m<20;m++)
    {
        vec3 p = ro+rd*t; float r=0.,SumD=0.;
        p = abs(vec3(tile)-mod(p,vec3(tile*2.))); // tiling fold
        // IFS
        for(int n=0;n<17;n++)
        {
            float k=2.0;
            p=abs(p)/dot(p,p)*k-0.5*k;
            SumD+=abs(length(p)-r);
            r=length(p);
        }
        SumD*=SumD*SumD;
        col+=vec3(t,t*t,t*t*t*t)*SumD*0.0015*fade;
        fade*=0.730;
        t+=0.1;
    }
    
    return col*0.01;
}


mapr refmap(vec3 p)
{
    mapr mr = mapr(1000.0,false,-1,0.0,0.0,0.0,vec3(0.0));
    return mr;
}

vec3 dRefColor(vec3 ro, vec3 rd, vec3 n)
{
    vec3 col = vec3(0.0);
    vec3 dir = reflect(-rd, n);
    
    if(_RefMapIndex == 0) col = sky(reflect(-rd, n), true);
    else if(_RefMapIndex == 1) col = StarSpace(reflect(-rd, n), rd);

    return col;
}

// https://www.shadertoy.com/view/4tscR8
//spaceship distance field is the min() of many sub-distance fields
float mav(vec2 a){return max(a.y,a.x);}
float mav(vec3 a){return max(a.z,mav(a.xy));}
//sub of H and I
vec3 F(vec3 a, float b){float c=sin(b),d=cos(b);return mat3(d,-c,0,c,d,0,0,0,1)*a;}
//sub of T,used once
vec3 H(vec3 a){a=F(a,(floor(atan(a.y,a.x)*1.5/acos(-1.))*2.+1.)*acos(-1.)/3.);
 return vec3(a.x,abs(a.y),a.z);}
//
//sub of S and T
float R(vec3 a){vec3 b=abs(a);return max(b.y,dot(vec3(.87,.5, 0), b))- 1.;}
//sub of T, used twice
float S(vec3 a){return max(max(abs(length(a-vec3(0,0,5.))-5.)-.05,R(a)),a.z-2.);}
//sub of T, used twice
float Q(vec3 a){return max(abs(length(a*vec3(1,1,.3))-.325)-.025,-a.z);}
//sub of T,used twice
float P(vec3 a){vec3 b=abs(a);
 return max(mav(b),max(max(length(b.xy),length(b.yz)),length(b.zx))-.2)-1.;}
//t is most scene specific
//for scene5 it is the distance field of chasing spaceships
float T(vec3 a){
 vec3 b=a*20.0,c=H(b*2.+vec3(0,0,2))-vec3(1.4,0,0),d=b;
 d.y=abs(d.y);
 return 
      min(
          min(
              min(max(R(d*4.-vec3(2,5,0))*.25,abs(d.z)-1.),S(d.yzx*vec3(1,.5,.5)*1.5 + vec3(.3,0,0))/1.5),
              max(min(.1-abs(d.x),-d.z),S(vec3(0, 0, 1) - d.xzy * vec3(1, .5, .5)))),
          min(
              min(max(P(c),-P(c * 1.2 + vec3(0,0, 1.5)) / 1.2),Q(c + vec3(0, 0, 1.5))),
              Q(vec3(abs(c.xy), c.z) - vec3(.5,.5,-1.5)))*.5)*.05;
}

// M : ”òs‘D–{‘Ì
mapr map(vec3 p)
{
    mapr mr = mapr(1000.0,false,-1,0.0,0.0,0.0,vec3(0.0));
    
    if(_TRSIndex == 0)
    {
        //
        float rate = clamp((_time-_MoveStartTime)/_MoveTimeDuration, 0.0, 1.0);
        p = mix(
            trs(p, vec3(1.0), vec3(0.0, 0.0, pi*0.5), vec3(0.0)),
            trs(p,vec3(1.0), vec3(6.21*0.25, sin(_time)*0.15, sin(_time)*0.15+pi*0.5), vec3(0.0, -0.1, 0.0)),
            rate
        );
    }
    else if(_TRSIndex == 1)
    {
        vec3 mdir = vec3(0.0);
        if(_time>= _GoMoonTime) mdir.xz += (exp(_time-_GoMoonTime) - exp(0.001))*10.0;
        vec3 mdir_n = normalize(mdir); 
        float axz = 0.0;
         axz = mix(0.0, atan(-1.0, 1.0), clamp((_time-_GoMoonTime), 0.0, 1.0));
        p = trs(p,vec3(1.0), vec3(11.7*0.25,11.7*0.25 +pi+axz,pi*1.1 + sin(_time)*0.15+pi*0.5), mdir);
    }
    else
    {
        p = trs(p, vec3(1.0), vec3(0.0, sin(_time)*0.15, sin(_time)*0.15+pi*0.5),vec3(0.0));
    }
    
    compm(mr, T(p), 0, true, false);
      
    return mr;
}

mapr ray(vec3 ro, vec3 rd, bool IsRef)
{
    float t=0.0,i=0.0,acc=0.0;mapr mr;vec3 jetCol=vec3(0.0);
    for(i=0.0; i<80.0; ++i) {
        vec3 p = ro+rd*t;
        if(!IsRef)
        {
            mr = map(p);
        }
        else
        {
            mr = refmap(p);
        }
        
        if(abs(mr.d)<(t*5.0 + 1.0)*.0001 || t>=3000.0) break;
        acc += exp(-3.0 * mr.d);
        t = min(t+mr.d, 3000.0);
        
        vec3 jDir = g_ShipPos - p;
        jetCol += exp(-1.0*length(jDir)) * vec3(0.33,0.33,0.88);
    }
    mr.i=i;mr.t=t;mr.acc=acc;mr.jetCol=jetCol;
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

void main()
{
#ifdef DRAW_ON_SHADERTOY
    vec2 uv = gl_FragCoord.xy/_resolution.xy;
    vec2 st=(gl_FragCoord.xy*2.-_resolution.xy)/min(_resolution.x,_resolution.y);
    vec3 ro= 0.25 * vec3(0.0,0.0,1.0),ta=vec3(0.0,0.0,0.0);
    if(_time>=_MoveStartTime) _RefMapIndex = 1;
    //vec3 ro= 0.25 * vec3(cos(_time),0.0,sin(_time)),ta=vec3(0.0,0.0,0.0);
    //vec3 ro= 30.0 * vec3(cos(_time),0.0,sin(_time)),ta=vec3(0.0,0.0,0.0);
    
#else
    vec2 st=uv*2.0-1.0;st.x*=(_resolution.x/_resolution.y);
    vec3 ro= _WorldCameraPos,ta=_WorldCameraCenter;
    if(_MoveH > 0.0) { ro.y -= _MoveH; ta.y -= _MoveH; }
   ro *= 0.1;
#endif
    //vec3 ro= 30.0 * vec3(cos(_time),0.0,sin(_time)),ta=vec3(0.0,0.0,0.0);
    vec3 cdir=normalize(ta-ro),cside=normalize(cross(vec3(0.0,1.0,0.0),cdir)),cup=normalize(cross(cdir,cside)),
    rd=normalize(st.x*cside+st.y*cup+1.0*cdir),col = vec3(0.0);
    g_ShipPos = vec3(0.0);
    //col = texture(iChannel0, rd).rgb;
    
    float ShowingTime = max(0.0, _time - (_ShowingFinTime-_ShowDuration));
    
    mapr mr= ray(ro, rd, false);
    vec3 p = ro + rd * mr.t;
    bool ISDraw = true;
    if(_IsUseShowing == 1 && p.z < 10.0*2.0-ShowingTime*10.0){ISDraw = false;}
    float Alpha = 0.0;

    if(mr.hit && ISDraw && mr.m == 0)
    {
         //vec3 BaseCol = (mr.m == 2)? vec3(0.1) : vec3(0.1); 
         vec3 BaseCol = vec3(1.0);
         
         vec3 n = gn(p);
         vec3 nabs = abs(n);
         float diff = max(0.0, dot(nabs, ldir));
         float spec = pow(max(0.0, dot(reflect(-ldir, nabs), -rd)), 16.0);
         float ambient = 0.25;
         col = BaseCol * (diff + ambient) + vec3(1.0) * spec;
         
         {
             ro = p;
             rd = reflect(rd, n);
             mapr ref_mr = ray(ro ,rd, true);
             float metallic = (_RefMapIndex == 0)? 0.5 : 0.5;
             vec3 refcol = dRefColor(ro, rd, n);
             col = mix(col, refcol, metallic);
         }
         col += mr.jetCol*0.05;
         Alpha = 1.0;
         
        if(_IsUseShowing == 1 && p.z > 10.0*2.0-ShowingTime*10.0 && p.z < (10.0*2.0+2.0)-ShowingTime*10.0)
        {
           col = vec3(0.0,0.0,1.0)*mr.acc;
        }
    }
    else if(ShowingTime > 0.0 && ShowingTime < _ShowDuration && _IsUseShowing == 1)
    {
        // 5sŠÔŒõ‚é
        float power = clamp(sin( ((ShowingTime)/_ShowDuration)*pi ) ,0.0, 1.0);
        col = vec3(0.0,0.0,1.0) * mr.acc * 0.1 * power;
        
        Alpha = clamp(col.b, 0.0, 1.0);
    }
    
    /*if(_RefMapIndex == 1) 
    {
        vec4 BufCol = texture(_BufferA,uv);
        Alpha = BufCol.a;
        //Alpha = (BufCol.b < 0.25)? 0.0: 1.0;
        col += BufCol.rgb;
    }*/
    
     //vec3 BufCol = texture(_BufferA,uv).rgb;
     //col = BufCol; Alpha = 1.0;

    gl_FragColor = vec4(col, Alpha);
}




)"