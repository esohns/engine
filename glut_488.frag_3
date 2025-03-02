#version 130

// glut_488_common.glsl
#define pixel(a, p) texture(a, p/vec2(textureSize(a,0)))
#define texel(a, p) texelFetch(a, ivec2(p-0.5),0)
#define s2d iResolution.xy
#define ch0 iChannel0
#define ch1 iChannel1
#define ch2 iChannel2
#define ch3 iChannel3

#define dt 0.5


const float scale = .6;
//fluid 1 density 
const float a1 = 0.8;
//fluid 2 density
const float a2 = 1.;

//set to 0 if you want a gas-like behaviour
//fluid 1 fluid-like/gas-like regulator 
const float b1 = 0.99;
//fluid 2 fluid-like/gas-like regulator 
const float b2 = 0.99;

//interaction energy cost
const float ie = 1.5;

//initial conditions for amplitudes
const float amp = 0.8;
const vec4 fluid1_Q = amp*vec4(a1,0,0,0);
const vec4 fluid1_Qv = amp*vec4(0,a1,0,0); //minus for antifluid
const vec4 fluid2_Q = amp*vec4(0,0,a2,0);
const vec4 fluid2_Qv = amp*vec4(0,0,0,a2);
const float mouser = 25.;
const float initr = 150.;


float sq(float x){ return x*x; }
float cb(float x){ return x*x*x; }
float sq(vec2 x){ return dot(x,x); }

const float pressure = 0.001;

//wave potential
float P(vec4 Q, vec2 p)
{
    //fluid 1 amplitude
    float fd1 = 2.*tanh(0.5*length(Q.xy));
    //fluid 2 amplitude
    float fd2 = 2.*tanh(0.5*length(Q.zw));
    
    //liquifier term 1
    float liq1 = 1. - b1*exp(-3.*sq(fd1-a1));      
    //liquifier term 2
    float liq2 = 1. - b2*exp(-3.*sq(fd2-a2));     
    float grav = 0.0005*p.y;
        
    return scale*(cb(fd1)*liq1 + cb(fd2)*liq2+ ie*sq(fd1)*sq(fd2))+ (sq(fd1)+sq(fd2))*grav;
}

//force
#define d 0.001
vec4 F(vec4 Q, vec2 p)
{
    vec3 dx = 0.5*vec3(-d,0.,d);
    return vec4(P(Q + dx.zyyy, p) - P(Q + dx.xyyy, p),
                P(Q + dx.yzyy, p) - P(Q + dx.yxyy, p),
                P(Q + dx.yyzy, p) - P(Q + dx.yyxy, p),
                P(Q + dx.yyyz, p) - P(Q + dx.yyyx, p))/d;
}

//Laplacian operator
vec4 Laplace(sampler2D ch, vec2 p)
{
    vec3 dx = vec3(-1,0.,1);
    return texel(ch, p+dx.xy)+texel(ch, p+dx.yx)+texel(ch, p+dx.zy)+texel(ch, p+dx.yz)-4.*texel(ch, p);
}

vec2 Grad(sampler2D ch, vec2 p)
{
    vec3 dx = vec3(-1,0.,1);
    return vec2(length(texel(ch, p+dx.zy)),length(texel(ch, p+dx.yz))) - length(texel(ch, p));
}
// glut_488_common.glsl

uniform vec2 iResolution;
uniform int iFrame;
uniform vec4 iMouse;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

void mouse(inout vec4 Q, inout vec4 Qv, vec2 p)
{
    if(iMouse.z > 0.)
    {
        float f1 = exp(-sq((p-iMouse.xy)/mouser));
        Q += 0.3*fluid1_Q*f1;
        Qv += 0.3*fluid1_Qv*f1;
    }
}

void
main ()
{
    //get old value
    gl_FragColor = texel(ch0, gl_FragCoord.xy);
    vec4 Qv = texel(ch1, gl_FragCoord.xy);
    
    gl_FragColor += Qv*dt + 0.0005*Laplace(ch0, gl_FragCoord.xy);
   
    mouse(gl_FragColor,Qv,gl_FragCoord.xy);
    
    if(iFrame < 2)
    {
        float f1 = step(sq((gl_FragCoord.xy-s2d*0.3)/initr),1.);
        float f2 = step(sq((gl_FragCoord.xy-s2d*0.7)/initr),1.);
        gl_FragColor = fluid1_Q*f1*(1.-f2) + fluid2_Q*f2*(1.-f1);
    }
}
