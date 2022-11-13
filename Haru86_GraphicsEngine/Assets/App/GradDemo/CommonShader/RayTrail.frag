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
int _MapIndex;
#else
uniform float _time;
uniform vec2 _resolution;
uniform float _RenderingTarget;
uniform vec3 _WorldCameraPos;
uniform vec3 _WorldCameraCenter;

uniform sampler2D _BufferA;

in vec2 uv;
#endif

void main()
{
    vec2 st=uv*2.0-1.0;
    //st.x*=(_resolution.x/_resolution.y);
    vec2 tst = uv; // 0 -1
   // tst.x*=(_resolution.x/_resolution.y);

    float t = _time*2.;

    vec3 c = vec3(.2/length(vec2(cos(t*.7),sin(t))-st*1.5)); // 球移動
    
    vec3 fb = texture(_BufferA, tst).xyz; // ここで自己参照している？
    
    c = mix(c,fb,0.981); // 今の状態と前の状態をブレンド
    // たぶんRenderBufferはglClearせずに段々薄くする感じでトレイルにしている？
    // 上記の式から察するにマイフレーム各に0.9倍していって薄くしている
    
    gl_FragColor = vec4(c,1.0);
}

)"