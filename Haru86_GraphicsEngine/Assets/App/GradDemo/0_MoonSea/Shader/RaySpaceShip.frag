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
const int _IsUseShowing = 0;
#else
uniform float _time;
uniform vec2 _resolution;
uniform float _RenderingTarget;
uniform vec3 _WorldCameraPos;
uniform vec3 _WorldCameraCenter;
uniform int _IsUseShowing;

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
};

void compm(inout mapr mr,float d,int mt,bool IsMin, bool IsSMin) // CompareMap
{
    if(IsMin)
    {
        if(!IsSMin)
        {
            if(d<mr.d) mr = mapr(d,false,mt,0.0,0.0,0.0);
            if(mr.d<dmin) mr.hit=true; 
        }
        else
        {
            if(d<mr.d) mr = mapr(smin(d, mr.d, 1.5),false,mt,0.0,0.0,0.0);
            if(mr.d<dmin) mr.hit=true; 
        }
    }
    else
    {
        if(d>mr.d) mr = mapr(d,false,mt,0.0,0.0,0.0);
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
    
    if(IsRef)
    {
        // ŒŽ
        vec2 moonPos = rd.xy/rd.z+vec2(0.0,0.25);
        float moond=length(moonPos),moonr=0.355;
        vec3 moonCol = vec3(fbm(moonPos*10.0))
            *smoothstep(0.37,0.35,moond);
        moonCol+=vec3(1.0)*pow(moond*2.25,-16.0)
        *( (moond<moonr)? 0.0:0.01 );
        
        col += moonCol*vec3(0.5,0.4,0.15)*2.0;
    }
    
    return col;
}

mapr refmap(vec3 p)
{
    mapr mr = mapr(1000.0,false,-1,0.0,0.0,0.0);
    return mr;
}

vec3 dRefColor(vec3 ro, vec3 rd, vec3 n)
{
    vec3 col = vec3(0.0);
    vec3 dir = reflect(-rd, n);
    
    col = sky(reflect(-rd, n), true);

    return col;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float B(vec3 a)
{
    return max(max(a.x,a.y),a.z);
}

float J(vec3 a)
{
    return max(B(abs(a)),B(vec3(length(a.xy),length(a.yz),length(a.zx)))-.2)-1.0;
}

float K(vec3 a)
{
    a = abs(a);
    return max(a.x*.87+a.y*.5, a.y) - 1.0;
}

float L(vec3 a)
{
    a.z -= 5.0;
    return max(max(abs(length(a)-5.0) - .05, K(a)), a.z);
}

float body(vec3 h)
{
    return min(
          max(L(h*vec3(.075, .5, -.6) + vec3(.6, -.1, 0.0))/.6,1.6-abs(h.x)),
          L(h*vec3(1.0, .35, -.5)+vec3(.45, 0.0, .3))
         );
}

float wind(vec3 h, vec3 f)
{
    return min(
          min(
           max(K(f*1.25-vec3(.2,1.1,-2.5))/1.5,abs(h.y+2.0)-1.75),
           L(vec3(0.0, -.05, 1.5)-f.yzx*.5)*2.0),
          max(J(f*.7+vec3(-1.75, .35, 1.4))/.7,
          -J(f*.8+vec3(-2.0, .4, 2.8))/.8)
         );
}

float engine(vec3 p)
{
    return 1.0;
}

// M : ”òs‘D–{‘Ì
mapr map(vec3 p)
{
    mapr mr = mapr(1000.0,false,-1,0.0,0.0,0.0);

    vec3 pos = (p)*.5;
    pos.xz *= 0.35;
    float d = length(pos)-5.0, dbody=1000.0, dwind=1000.0;
    float ShowingVal = 0.0;
    if(_IsUseShowing == 1)
    {
        ShowingVal = max(0.0,(5.0-_time));
    }
    
    if(d < 1.0){
        float a=(floor(atan(pos.x,pos.y)/3.14159265*1.5)+.5)/3.0*3.14159265*2.0;
        float b=sin(a);
        float c=cos(a);
        vec3 f=vec3(pos.x*b+pos.y*c,abs(pos.x*c-pos.y*b),pos.z) + ShowingVal;
        vec3 h=vec3(abs(pos.x),pos.zy) + ShowingVal; // x‚ÉŠÖ‚µ‚Äfold
        
        dbody = body(h) * 2.0;
        dwind = wind(h, f) * 2.0;
        //d = min(body(h), wind(h, f))*2.0;
        
        compm(mr, dbody, 1, true, false);
        compm(mr, dwind, 2, true, false);
    }
    else
    {
        compm(mr, d, 0, true, false);
    }
    
    return mr;
}

mapr ray(vec3 ro, vec3 rd, bool IsRef)
{
    float t = 0.0, i=0.0;mapr mr;
    for(i=0.0; i<80.0; ++i) {
        if(!IsRef)
        {
            mr = map(ro+rd*t);
        }
        else
        {
            mr = refmap(ro+rd*t);
        }
        
        if(abs(mr.d)<(t*5.0 + 1.0)*.0001 || t>=3000.0) break;
        t = min(t+mr.d, 3000.0);
    }
    mr.i=i;mr.t=t;mr.acc=0.0;
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

/*vec3 dRefColor(vec3 ro, vec3 rd, vec3 n)
{
    vec3 col = vec3(0.0);
    vec3 dir = reflect(-rd, n);
    col = texture(iChannel0, dir).rgb;
    
    return col;
}*/

void main()
{
#ifdef DRAW_ON_SHADERTOY
    vec2 st=(gl_FragCoord.xy*2.-_resolution.xy)/min(_resolution.x,_resolution.y);
    //vec3 ro= 30.0 * vec3(cos(_time),sin(_time),sin(_time)),ta=vec3(0.0,0.0,0.0);
#else
    vec2 st=uv*2.0-1.0;st.x*=(_resolution.x/_resolution.y);
    //vec3 ro= _WorldCameraPos,ta=_WorldCameraCenter;
#endif
    vec3 ro= 30.0 * vec3(cos(_time),sin(_time),sin(_time)),ta=vec3(0.0,0.0,0.0);
    vec3 cdir=normalize(ta-ro),cside=normalize(cross(vec3(0.0,1.0,0.0),cdir)),cup=normalize(cross(cdir,cside)),
    rd=normalize(st.x*cside+st.y*cup+1.0*cdir),col = vec3(0.0);
    
    //col = texture(iChannel0, rd).rgb;
    
    mapr mr= ray(ro, rd, false);
    vec3 p = ro + rd * mr.t;
    bool ISDraw = true;
    if(_IsUseShowing == 1 && p.z < 20.0-_time*10.0){ISDraw = false;}
    float Alpha = 0.0;

    if(mr.hit && ISDraw)
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
             float metallic = 0.75;
             vec3 refcol = dRefColor(ro, rd, n);
             col = mix(col, refcol, metallic);
         }

         Alpha = 1.0;
    }
    gl_FragColor = vec4(col, Alpha);
}

)"