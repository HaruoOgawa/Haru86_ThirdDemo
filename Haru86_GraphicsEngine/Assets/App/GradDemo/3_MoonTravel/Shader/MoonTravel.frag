R"(

// ShaderToy --> OpenGL/GLSL
/*#define _resolution iResolution
#define gl_FragCoord fragCoord
#define gl_FragColor fragColor
#define _time iTime
#define main() mainImage( out vec4 fragColor, in vec2 fragCoord )
const int _RenderingTarget = 1;*/

#version 330
uniform float _time;
uniform vec2 _resolution;
uniform float _RenderingTarget;
uniform vec3 _WorldCameraPos;
uniform vec3 _WorldCameraCenter;

in vec2 uv;

struct mapr // MapResult
{
   float d; // Distance
   int   m; // MaterialType
            // 0: Moon
};

void compm(inout mapr mr,float d,int mt,bool IsMin) // CompareMap
{
    if(IsMin)
    {
        if(d<mr.d) mr = mapr(d,mt);
    }
    else
    {
        if(d>mr.d) mr = mapr(d,mt);
    }
}

//////////////////////////

#define pi 3.14159265
#define rot(a) mat2(cos(a),-sin(a),sin(a),cos(a))

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

float fbm(vec2 p)
{
    mat2 m=rot(35.6*pi/180.0);
    float f=0.0,ASum=0.0;
    for(float i=0.0;i<4.0;i++)
    {
        float Att = pow(0.5,i+1.0);
        f+=Att*noise(p);p=2.0*m*p;
        ASum+=Att;
    }
    
    return f/ASum;
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

vec3 trs(vec3 p,vec3 s,vec3 r,vec3 t)
{
    p+=t; 
    p.yz*=rot(s.x);p.xz*=rot(s.y);p.xy*=rot(s.z);
    p*=s;
    
    return p;
}

//////////////////////////
// https://qiita.com/edo_m18/items/a575606a60b21f0d2c57
// https://www.shadertoy.com/view/Ms2SD1

#define SEA_FREQ 0.16
#define SEA_HEIGHT 0.6
#define SEA_CHOPPY 4.8
#define ITER_GEOMETRY 3
#define ITER_FRAGMENT 3
#define SEA_SPEED 0.8
#define SEA_TIME (1.0+_time*SEA_SPEED)
#define NUM_STEPS 8
#define EPSILON_NRM (0.1 / _resolution.x)
#define SEA_BASE vec3(0.1, 0.19, 0.22)
#define SEA_WATER_COLOR vec3(0.8,0.9,0.6)

float sea_octave(vec2 uv,float choppy)
{
    uv+=noise(uv);
    vec2 wv=1.0-abs(sin(uv));
    vec2 swv=abs(cos(uv));
    wv=mix(wv,swv,wv);
    
    return pow(1.0-pow(wv.x*wv.y,0.65),choppy);
}

float seamap(vec3 p)
{
    float freq = SEA_FREQ;
    float amp = SEA_HEIGHT;
    float choppy = SEA_CHOPPY;
    
    vec2 uv = p.xz;
    
    float d,h=0.0;
    
    mat2 octave_m = mat2(1.6,1.2,-1.2,1.6);
    
    // FBM
    for(int i=0;i<ITER_GEOMETRY;i++)
    {
        d=sea_octave((uv)*freq,choppy);
        d+=sea_octave((uv)*freq,choppy);
        h+=d*amp;
        
        uv*=octave_m;
        freq*=2.0;
        amp*=0.2;
        
        choppy=mix(choppy,1.0,0.2);
    }
    
    return p.y-h;
}

float seamap_detailed(vec3 p)
{
    float freq = SEA_FREQ;
    float amp = SEA_HEIGHT;
    float choppy = SEA_CHOPPY;
    
    vec2 uv = p.xz;
    
    float d,h=0.0;
    
    mat2 octave_m = mat2(1.6,1.2,-1.2,1.6);
    
    // FBM
    for(int i=0;i<ITER_FRAGMENT;i++)
    {
        d=sea_octave((uv)*freq,choppy);
        d+=sea_octave((uv)*freq,choppy);
        h+=d*amp;
        
        uv*=octave_m;
        freq*=2.0;
        amp*=0.2;
        
        choppy=mix(choppy,1.0,0.2);
    }
    
    return p.y-h;
}

float CreateSeaHightMap(vec3 ro,vec3 rd,out vec3 p,inout bool IsMoon)
{
    float tm=0.0;
    float tx=1000.0;
    float hx=seamap(ro+rd*tx);
    
    //
    if(hx>0.0)
    {
        p = vec3(0.0);
        IsMoon=false;
        return tx;
    }
    
    float hm=seamap(ro+rd*tm);
    float tmid=0.0;
    
    for(int i=0;i<NUM_STEPS;i++)
    {
        float f = hm/(hm-hx);
        tmid = mix(tm,tx,f);
        p=ro+rd*tmid;
        
        float hmid=seamap(p);
        
        if(hmid<0.0)
        {
            tx=tmid;
            hx=hmid;
        }
        else
        {
            tm=tmid;
            hm=hmid;
        }
    }
    
    IsMoon=true;
    return tmid;
}

mapr map(vec3 p)
{
    mapr mr;
    mr.d=1000.0;
    mr.m=-1;
    
    compm(mr,length( trs(p,vec3(1.0),vec3(0.0),vec3(0.0,-2.0,0.0)) )-1.25,0,true);
    
    return mr;
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

#define dmin 0.001
#define ln 64.0
#define ldir normalize(vec3(1.0,1.0,-1.0))

vec3 gn_sea(vec3 p,float eps)
{
    vec3 n;
    n.y=seamap_detailed(p);
    n.x=seamap_detailed(vec3(p.x+eps, p.y, p.z))-n.y;
    n.z=seamap_detailed(vec3(p.x, p.y, p.z+eps))-n.y;
    n.y=eps;
    
    return normalize(n);
}

vec3 gn(vec3 p)
{
    vec2 e = vec2(dmin,0.0);
    return normalize(vec3(
        map(p+e.xyy).d-map(p-e.xyy).d,
        map(p+e.yxy).d-map(p-e.yxy).d,
        map(p+e.yyx).d-map(p-e.yyx).d
    ));
}

void main()
{
if(_RenderingTarget==2) // ZTest
{
    gl_FragColor = vec4(vec3(0.0),1.0);
}
else
{
    //vec2 st = (gl_FragCoord.xy*2.0-_resolution.xy)/min(_resolution.x,_resolution.y);
    vec2 st=uv*2.0-1.0;
    st.x*=(_resolution.x/_resolution.y);

    vec3 col = vec3(0.0),ro=vec3(0.0,vec2(2.0)),
    ta=vec3(0.0,1.0+(1.0),0.0),
    cdir=normalize(ta-ro),cside=normalize(cross(vec3(0.0,1.0,0.0),cdir)),cup=normalize(cross(cdir,cside)),
    rd=normalize(st.x*cside+st.y*cup+1.0*cdir);
    float i=0.0,t=0.0,acc=0.0; mapr mr;
    
    for(;++i<ln;){mr=map(ro+rd*(t+=mr.d));if(mr.d<dmin)break;acc+=exp(-3.0*mr.d);}

    // ”wŒi‚Ì‰F’ˆ
    vec3 sro = ro;
    sro.z-=_time*0.01;
    col = StarSpace(sro,rd);
  
    // ’n‹…‚Ì•`‰æ
    { 
        if(mr.m==0)
        {
            vec3 p = ro+rd*t;
            //p.xz*=rot(_time*0.5);
            vec3 n = gn(p);
            vec3 pn = n*0.5+0.5;
            
            // landBlend
            const mat3 m = mat3( 0.00,  0.80,  0.60,
                    -0.80,  0.36, -0.48,
                    -0.60, -0.48,  0.64 );
            vec3 q = p;
            q = m *q *2.01;
            
            // ‘å—¤
            {
                float f = fbm3(2.0*q+vec3(160.0,40.0,120.0),6.0,1.0);
                f=pow(f,1.5);
                float landBlend = min(4.0*max(f-0.3,0.0),1.0);
                vec3 landCol = vec3(1.0,1.3,0.5);
                float continent = fbm3(5.0*q+vec3(120.0),3.0,1.0);
                vec3 oceanCol = vec3(0.0,0.031,0.11);
                col = mix(oceanCol,continent*landCol,landBlend);
            }
            
            // ‰_
            {
                float f = fbm3(7.5*q+vec3(500)+_time,6.0,1.0);
                f=pow(f,1.5);
                float cloudBlend = min(4.0*max(f-0.3,0.0),1.0);
                vec3 cloudCol = vec3(1.0);
                float cloud = fbm3(7.5*q+vec3(500.0)+_time,3.0,1.0);
                vec3 oceanCol = vec3(0.0,0.031,0.11);
                col = mix(col,cloud*cloudCol,cloudBlend);
            }

            //float diffuse = max(0.1,dot(n,ldir));
            //col*=diffuse;
        }
    }
    
    // ŒŽ‚Ì•\–Ê‚ð•`‰æ‚·‚é
    {
        vec3 moonro = ro;
        moonro.z-=_time*0.5;
        
        vec3 p;
        
        bool IsMoon=false;
        CreateSeaHightMap(moonro,rd,p,IsMoon);
        vec3 dist = p-moonro;
        float seat = length(dist);
        vec3 n = gn_sea(p,dot(dist,dist)*EPSILON_NRM);

        // ŒŽ‚Ì•\–Ê
        if(IsMoon)
        {
            col = vec3(1.0)*max(0.0,dot(n,ldir));
        }
    }
    
    gl_FragColor = vec4(col,1.0);
}

}

)"