#version 330
precision highp float;

//#include "glut_484_common.glsl"
#define ch0 iChannel0
#define ch1 iChannel1
#define ch2 iChannel2
#define ch3 iChannel3

#define R iResolution.xy

#define LOAD(ch, pos) texelFetch(ch, ivec2(pos), 0)
#define LOAD3D(ch, pos) texelFetch(ch, ivec2(dim2from3(pos)), 0)

#define PI 3.1415926535
#define TWO_PI 6.28318530718

#define light_dir normalize(vec3(0.741,1.000,0.580))

#define initial_particle_density 1u
#define dt 1.0
#define rest_density 0.5
#define gravity 0.01

#define PRESSURE 3.5
#define PRESSURE_RAD 1.0
#define VISCOSITY 0.65
#define SURFACE_TENSION 0.1
#define SURFACE_TENSION_RAD 2.0
#define SPIKE_KERNEL 0.5
#define SPIKE_RAD 0.75

#define force_mouse 0.005
#define force_mouse_rad 40.0
#define force_boundary 5.0
#define boundary_h 5.0
#define max_velocity 1.0
#define cooling 0.0

float sqr(float x)
{
    return x * x;
}

float cub(float x)
{
    return x*x*x;
}

float Pressure(float rho)
{
    return (rho/rest_density - 1.0)/max(rho*rho, 0.001);
}

float Gaussian(float r, float d)
{
    float norm = 1.0/(cub(d)*sqrt(cub(TWO_PI)));
    return norm * exp(-0.5*sqr(r/d));
}

vec3 GaussianGrad(vec3 dx, float d)
{
    float norm = 1.0/(cub(d)*sqrt(cub(TWO_PI)));
    float r = length(dx);
    return - (norm/sqr(d)) * exp(-0.5*sqr(r/d)) * dx;
}

vec4 GaussianGrad2(vec3 dx, float d)
{
    float norm = 1.0/(cub(d)*sqrt(cub(TWO_PI)));
    float r = length(dx);
    return norm * exp(-0.5*sqr(r/d)) * vec4(-dx /sqr(d), 1.0);
}

#define KERNEL_POLY6_NORMALIZATION (315.0 / (64.0 * PI))
#define KERNEL_SPIKE_NORMALIZATION (45 / PI)
#define KERNEL_RADIUS 1.0

float KernelPoly6(float r, float d)
{
    float c = KERNEL_POLY6_NORMALIZATION / cub(cub(d));
    return (r <= d) ? c * cub(sqr(d) - sqr(r)) : 0.0;
}

vec3 KernelPoly6Grad(vec3 dx, float d)
{
    float c = KERNEL_POLY6_NORMALIZATION / cub(cub(d));
    float r = length(dx);
    return (r <= d) ? clamp(-6.0 * c * sqr(sqr(d) - sqr(r)), -3.0, 0.0) * dx : vec3(0.0);
}

#define GD(x, R) Gaussian(length(x),R)
#define GGRAD(x, R) GaussianGrad(x, R)
#define GGRAD2(x,R) GaussianGrad2(x, R)
//#define GD(x, R) 12.0*KernelPoly6(length(x),R*4.0/2.0)
//#define GGRAD(x, R) 12.0*KernelPoly6Grad(x, R*4.0/2.0)
#define GS(x) exp(-dot(x,x))
#define DIR(phi) vec2(cos(TWO_PI*phi),sin(TWO_PI*phi))

#define loop(i,x) for(int i = 0; i < x; i++)
#define range(i,a,b) for(int i = a; i <= b; i++)

//3d slice aspect ratio 
#define ar vec2(1.0,0.5)
vec2 SCALE;
vec3 size3d;

vec2 dim2from3(vec3 p3d)
{
    p3d = clamp(p3d, vec3(0.0), size3d);
    float ny = floor(p3d.z/SCALE.x);
    float nx = floor(p3d.z) - ny*SCALE.x;
    return vec2(nx, ny)*vec2(size3d.xy) + p3d.xy;
}

vec3 dim3from2(vec2 p2d)
{
    return vec3(p2d - size3d.xy*floor(p2d/size3d.xy),(floor(p2d.x/size3d.x) + SCALE.x*floor(p2d.y/size3d.y)));
}


//#define pixel(a, p, s) texture(a, (p+0.5)/vec2(s))

vec4 pixel(sampler2D ch, vec2 pos)
{
    vec2 d = pos - floor(pos);
    vec4 p00 = texelFetch(ch, ivec2(pos), 0);
    vec4 p01 = texelFetch(ch, ivec2(pos) + ivec2(0, 1), 0);
    vec4 p10 = texelFetch(ch, ivec2(pos) + ivec2(1, 0), 0);
    vec4 p11 = texelFetch(ch, ivec2(pos) + ivec2(1, 1), 0);
    return mix(mix(p00, p01, d.y), mix(p10, p11, d.y), d.x);
}

vec4 voxel(sampler2D ch, vec3 p3d)
{
    return pixel(ch, dim2from3(p3d));
}

//trilinear interpolation = linear interp between layers
vec4 trilinear(sampler2D ch, vec3 p3d)
{
    return mix(voxel(ch, vec3(p3d.xy, floor(p3d.z))),voxel(ch, vec3(p3d.xy, ceil(p3d.z))), fract(p3d.z));
}


float sdBox( vec3 p, vec3 b )
{
    vec3 d = abs(p) - b;
    return min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,0.0));
}

vec2 hash21(float p)
{
  vec3 p3 = fract(vec3(p) * vec3(.1031, .1030, .0973));
  p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.xx+p3.yz)*p3.zy);
}

vec2 hash23(vec3 p3)
{
  p3 = fract(p3 * vec3(.1031, .1030, .0973));
    p3 += dot(p3, p3.yzx+33.33);
    return fract((p3.xx+p3.yz)*p3.zy);
}

vec3 udir(vec2 rng)
{
    float phi = 2.*PI*rng.x;
    float ctheta = 2.*rng.y-1.;
    float stheta = sqrt(1.0-ctheta*ctheta);
    return vec3(cos(phi)*stheta, sin(phi)*stheta, ctheta);
}

struct Particle 
{
    uint mass;
    vec3 pos;
    vec3 vel;
    vec3 force;
    float density;
};

//5 bits for shared exponent, 9 bits for each component
uint packvec3(vec3 v)
{
    //get the exponent
    float maxv = max(abs(v.x), max(abs(v.y), abs(v.z)));
    int exp = clamp(int(ceil(log2(maxv))), -15, 15);
    float scale = exp2(-float(exp));
    uvec3 sv = uvec3(round(clamp(v*scale, -1.0, 1.0) * 255.0) + 255.0);
    uint packed = uint(exp + 15) | (sv.x << 5) | (sv.y << 14) | (sv.z << 23);
    return packed;
}

vec3 unpackvec3(uint packed)
{
    int exp = int(packed & 0x1Fu) - 15;
    vec3 sv = vec3((packed >> 5) & 0x1FFu, (packed >> 14) & 0x1FFu, (packed >> 23) & 0x1FFu);
    vec3 v = (sv - 255.0) / 255.0;
    v *= exp2(float(exp));
    return v;
}

uint packmasspos(uint mass, vec3 p0)
{
    uvec3 pos0 = uvec3(clamp(p0, 0.0, 1.0) * 255.0);
    return mass | (pos0.x << 8) | (pos0.y << 16) | (pos0.z << 24);
}

uint packMassPos(uint mass, vec3 pos)
{
    uvec3 pos0 = uvec3(clamp(pos, 0.0, 1.0) * 255.0); // Assuming pos range [0, 1] in a cell
    uint data1 = mass | (pos0.x << 8) | (pos0.y << 16) | (pos0.z << 24);
    return data1;
}

void unpackMassPos(uint packed, out uint mass, out vec3 pos)
{
    mass = packed & 0xFFu;
    uvec3 pos0 = uvec3((packed >> 8) & 0xFFu, (packed >> 16) & 0xFFu, (packed >> 24) & 0xFFu);
    pos = vec3(pos0) / 255.0;
}

vec4 packParticles(Particle p0, Particle p1, vec3 pos)
{
    p0.pos -= pos;
    p1.pos -= pos;

    uvec4 data = uvec4(
        packMassPos(p0.mass, p0.pos),
        packMassPos(p1.mass, p1.pos),
        packvec3(p0.vel),
        packvec3(p1.vel)
    );

    return uintBitsToFloat(data);
}

void unpackParticles(vec4 packed, vec3 pos, out Particle p0, out Particle p1)
{
    uvec4 data = floatBitsToUint(packed);
    
    unpackMassPos(data.x, p0.mass, p0.pos);
    unpackMassPos(data.y, p1.mass, p1.pos);

    p0.pos += pos;
    p1.pos += pos;

    p0.vel = unpackvec3(data.z);
    p1.vel = unpackvec3(data.w);
}

float sdBox( in vec2 p, in vec2 b )
{
    vec2 d = abs(p)-b;
    return length(max(d,0.0)) + min(max(d.x,d.y),0.0);
}

int ClosestCluster(Particle p0, Particle p1, Particle incoming)
{
    //first try to choose the particle with significantly smaller mass
    if(float(p0.mass) < 0.01*float(p1.mass) || float(p1.mass) < 0.01*float(p0.mass))
    {
        return p0.mass < p1.mass ? 0 : 1;
    }

    //otherwise choose the closest one
    float d0 = length(p0.pos - incoming.pos);
    float d1 = length(p1.pos - incoming.pos);
    return d0 < d1 ? 0 : 1;
}

void BlendParticle(inout Particle p, in Particle incoming)
{
    uint newMass = p.mass + incoming.mass;
    vec2 weight = vec2(p.mass, incoming.mass) / float(newMass);
    p.pos = p.pos*weight.x + incoming.pos*weight.y;
    p.vel = p.vel*weight.x + incoming.vel*weight.y;
    p.mass = newMass;
}

void Clusterize(inout Particle p0, inout Particle p1, in Particle incoming, vec3 pos)
{
    //check if the incoming particle is in the cell
    if(!all(equal(pos, floor(incoming.pos))))
    {
        return;
    }

    int closest = ClosestCluster(p0, p1, incoming);
    if(closest == 0)
    {
        BlendParticle(p0, incoming);
    }
    else
    {
        BlendParticle(p1, incoming);
    }
}

void SplitParticle(inout Particle p1, inout Particle p2)
{
    vec3 pos = p1.pos;
    uint newMass = p1.mass;
    p1.mass = newMass/2u;
    p2.mass = newMass - p1.mass;
    vec3 dir = udir(hash23(pos));
    p1.pos = pos - dir*5e-3;
    p2.pos = pos + dir*5e-3;
    p2.vel = p1.vel;
}

void ApplyForce(inout Particle p, in Particle incoming)
{
    if(incoming.mass == 0u) return;

    vec3 dx = incoming.pos - p.pos;
    vec3 dv = incoming.vel - p.vel;
    float d = length(dx);
    vec3 dir = dx / max(d, 1e-3);
    
    float rho0 = p.density;
    float rho1 = incoming.density;
    float mass0 = float(p.mass);
    float mass1 = float(incoming.mass);
  
    vec3 ggrad = GGRAD(dx, PRESSURE_RAD);
    float pressure = 0.5*p.density*(Pressure(p.density) + Pressure(incoming.density));
    vec3 F_SPH = - PRESSURE * pressure * ggrad;
    vec3 F_VISC = VISCOSITY * dot(dir, dv) * ggrad;
    vec3 F_ST = SURFACE_TENSION * GGRAD(dx, SURFACE_TENSION_RAD);
    vec3 F_SPIKE = SPIKE_KERNEL * GD(d, SPIKE_RAD) * dir;
   
    p.force += - (F_ST + F_SPH + F_VISC + F_SPIKE) * mass1;
    
}

float minv(vec3 a)
{
    return min(min(a.x, a.y),a.z);
}

float maxv(vec3 a)
{
    return max(max(a.x, a.y),a.z);
}

float distance2border(vec3 p)
{
    vec3 a = vec3(size3d - 1.) - p;
    return min(minv(p),minv(a)) + 1.;
}

vec4 border_grad(vec3 p)
{
    const float dx = 0.001;
    const vec3 k = vec3(1,-1,0);
  return  (k.xyyx*distance2border(p + k.xyy*dx) +
       k.yyxx*distance2border(p + k.yyx*dx) +
       k.yxyx*distance2border(p + k.yxy*dx) +
       k.xxxx*distance2border(p + k.xxx*dx))/vec4(4.*dx,4.*dx,4.*dx,4.);
}

void IntegrateParticle(inout Particle p, vec3 pos, vec2 iR, vec4 iM, float time)
{
    p.force = p.force;/// max(0.0001, float(p.mass));
    p.force += gravity*vec3(0.4*sin(1.5*time), 0.2*cos(0.75*time), -1.0); //gravity
    

    vec4 border = border_grad(p.pos);
    vec3 bound =1.*normalize(border.xyz)*exp(-0.4*border.w*border.w);
    p.force += force_boundary*bound*dt;
    //p.force += vec2(0.0, 0.0)*GS(distance(p.pos, iR*vec2(0.2,0.5))/force_mouse_rad);
 
    //if(iM.z > 0.)
    //{
    //    vec3 dx = pos - vec3(iM.xy, 0.0);
    //    p.force -= force_mouse*dx*GS(dx/force_mouse_rad);
    //}
        
    p.vel += p.force * dt;

    //velocity limit
    float v = length(p.vel)/max_velocity;
    p.vel /= (v > 1.)?v:1.;
}

void InitGrid(vec2 iR)
{
    SCALE = floor(ar*pow(iR.x*iR.y,0.1666666));
    size3d = vec3(floor(iR.xy/SCALE), SCALE.x*SCALE.y);
}

vec3 hsv2rgb( in vec3 c )
{
    vec3 rgb = clamp( abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0 );

  rgb = rgb*rgb*(3.0-2.0*rgb); // cubic smoothing	

  return c.z * mix( vec3(1.0), rgb, c.y);
}
//#include "glut_484_common.glsl"

uniform vec2 iResolution;
uniform int iFrame;
uniform sampler2D iChannel0;

void
main ()
{
    InitGrid(iResolution.xy);
    vec2 fragCoord = floor(gl_FragCoord.xy);
    vec3 pos = dim3from2(fragCoord);
    
    Particle p0, p1;
    vec4 packed;

/*    if (iFrame >= 10)
    else */
    if (iFrame < 10)
    {
      p0.mass = 0u;
      p1.mass = 0u;
    }
/*    else
    {
      //load the particles
      packed = LOAD3D(ch0, pos);
      unpackParticles(packed, pos, p0, p1);
    }*/

    //advect neighbors and accumulate + clusterize density if they fall into this cell
    range(i, -1, 1) range(j, -1, 1) range(k, -1, 1)
    {
//        if(i == 0 && j == 0 && k == 0) continue;
        vec3 pos1 = pos + vec3(i, j, k);
        if(!all(lessThanEqual(pos1, size3d)) || !all(greaterThanEqual(pos1, vec3(0.0))))
        {
            continue;
        }
        //load the particles 
        Particle p0_, p1_;
        unpackParticles(LOAD3D(ch0, pos1), pos1, p0_, p1_);
        
        if(p0_.mass > 0u)
        {
            p0_.pos += p0_.vel*dt;
            Clusterize(p0, p1, p0_, pos);
        }
   
        if(p1_.mass > 0u)
        {
            p1_.pos += p1_.vel*dt;
            Clusterize(p0, p1, p1_, pos);
        }
    }
    
    if(p1.mass == 0u && p0.mass > 0u)
    {
        SplitParticle(p0, p1);
    }

    if(p0.mass == 0u && p1.mass > 0u)
    {
        SplitParticle(p1, p0);
    }
    
    packed = packParticles(p0, p1, pos);
    gl_FragData[0] = packed;
}
