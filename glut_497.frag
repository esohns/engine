#version 330
precision highp float;

// glut_497_common.glsl
const float dissipation 	= 0.95;

const float ballRadius		= 0.06;
const float fogHeigth		= ballRadius * 4.;
const int	nbSlice			= 24;
const float fogSlice		= fogHeigth / float(nbSlice);
const int	nbSphere 		= 3;
const float shadowDensity 	= 25.;
const float fogDensity 		= 20.;
const float lightHeight     = 1.;

const float tau =  radians(360.);

float hash12(vec2 p)
{
  vec3 p3  = fract(vec3(p.xyx) * .1031);
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.x + p3.y) * p3.z);
}

vec4 hash41(float p)
{
  vec4 p4 = fract(vec4(p) * vec4(.1031, .1030, .0973, .1099));
    p4 += dot(p4, p4.wzxy+33.33);
    return fract((p4.xxyz+p4.yzzw)*p4.zywx);
    
}

vec2 rotate(float angle, float radius)
{
    return vec2(cos(angle),-sin(angle)) * radius;
}

bool floorIntersect(in vec3 ro, in vec3 rd, in float floorHeight, out float t) 
{
    ro.y -= floorHeight;
    if(rd.y < -0.01)
    {
        t = ro.y / - rd.y;
        return true;
    }
    return false;
} 

vec2 sphIntersect( in vec3 ro, in vec3 rd, in vec3 ce, float ra )
{
    vec3 oc = ro - ce;
    float b = dot( oc, rd );
    float c = dot( oc, oc ) - ra*ra;
    float h = b*b - c;
    if( h<0.0 ) return vec2(-1.0); // no intersection
    h = sqrt( h );
    return vec2( -b-h, -b+h );
}

vec2 boxIntersection( in vec3 ro, in vec3 rd, in vec3 rad, in vec3 center,out vec3 oN ) 
{
    ro -= center;
    vec3 m = 1.0/rd;
    vec3 n = m*ro;
    vec3 k = abs(m)*rad;
    vec3 t1 = -n - k;
    vec3 t2 = -n + k;

    float tN = max( max( t1.x, t1.y ), t1.z );
    float tF = min( min( t2.x, t2.y ), t2.z );
  
    if( tN>tF || tF<0.0) return vec2(-1.0); // no intersection
    
    oN = -sign(rd)*step(t1.yzx,t1.xyz)*step(t1.zxy,t1.xyz);

    return vec2( tN, tF );
}

vec2 spherePosition(int id, int frame)
{
    vec4 offset = hash41(float(id)) * tau;
    float fframe = float(frame);
    return vec2(cos(offset.x + fframe * 0.015) + cos(offset.y + fframe * 0.020), cos(offset.z + fframe * 0.017) + cos(offset.w + fframe * 0.022)) * vec2(1., 0.5) * 0.9;
}

float dist2(vec3 v)
{
    return dot(v, v);
}
// glut_497_common.glsl

uniform vec2 iResolution;
uniform int iFrame;
uniform float iTime;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

float noise(vec3 p)
{
  vec3 ip=floor(p);
    p-=ip; 
    vec3 s=vec3(7,157,113);
    vec4 h=vec4(0.,s.yz,s.y+s.z)+dot(ip,s);
    p=p*p*(3.-2.*p); 
    h=mix(fract(sin(h)*43758.5),fract(sin(h+s.x)*43758.5),p.x);
    h.xy=mix(h.xz,h.yw,p.y);
    return mix(h.x,h.y,p.z); 
}

vec2 fbm(vec3 p, int octaveNum)
{
  vec2 acc = vec2(0);	
  float freq = 1.0;
  float amp = 0.5;
    vec3 shift = vec3(100);
  for (int i = 0; i < octaveNum; i++)
  {
    acc += vec2(noise(p), noise(p + vec3(0,0,10))) * amp;
        p = p * 2.0 + shift;
        amp *= 0.5;
  }
  return acc;
}


vec3 sampleMinusGradient(vec2 coord)
{
    vec3	veld	= texture(iChannel1, coord / iResolution.xy).xyz;
    float	left	= texture(iChannel0,(coord + vec2(-1, 0)) / iResolution.xy).x;
    float	right	= texture(iChannel0,(coord + vec2( 1, 0)) / iResolution.xy).x;
    float	bottom	= texture(iChannel0,(coord + vec2( 0,-1)) / iResolution.xy).x;
    float	top 	= texture(iChannel0,(coord + vec2( 0, 1)) / iResolution.xy).x;
    vec2	grad 	= vec2(right - left,top - bottom) * 0.5;
    return	vec3(veld.xy - grad, veld.z);
}

vec3 vignette(vec3 color, vec2 q, float v)
{
    color *= mix(1., pow(16.0 * q.x * q.y * (1.0 - q.x) * (1.0 - q.y), v), 0.02);
    return color;
}

void
main ()
{
    // Euler advection
    vec2	velocity = sampleMinusGradient(gl_FragCoord.xy).xy;
    vec3	veld = sampleMinusGradient(gl_FragCoord.xy - dissipation * velocity).xyz;
    float	density = veld.z;
    velocity = veld.xy;

    vec2	uv = (2. * gl_FragCoord.xy - iResolution.xy) / iResolution.y;
    // Small perturbation
    vec2 detailNoise = fbm(vec3(uv*40., iTime * 0.5 + 30.), 7) - 0.5;
    velocity += detailNoise * 0.2;
    density += length(detailNoise) * 0.01;
    
    // Injection
    vec2 injectionNoise = fbm(vec3(uv *1.5, iTime * 0.1 + 30.), 7) - 0.5;
    velocity += injectionNoise * 0.1;
    density += max((length(injectionNoise) * 0.04), 0.);

    // Inject emiter
    float influenceRadius = ballRadius * 2.;
    for(int i = 0 ; i < nbSphere ; i++)
    {
        vec2 p = spherePosition(i, iFrame);
        float dist = distance(uv, p);
        if(dist < influenceRadius)
        {
            vec2 op = spherePosition(i, iFrame + 1);
            vec2 ballVelocity = p - op; 
            density -= ((influenceRadius - dist) / influenceRadius) * 0.15;
            density = max(0., density);
          velocity -= ballVelocity * 5.;   
        }
        
    }	    
    density = min(1., density);
    density *= 0.99;     // damp
    veld = vec3(velocity, density);
    veld = vignette(veld, gl_FragCoord.xy / iResolution.xy, 1.);
    gl_FragColor = vec4(veld, 1);
}
