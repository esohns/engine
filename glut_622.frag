#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

#define USE_COLORS 3
#define WAVING 1
#define ROTATE 1
#define SHADOWS 1
#define AO 1
#define ANIMATE 1
#define ANIM_SPEED .25

float
hash12 (vec2 p)
{
#if WAVING == 1
   float v = sin(p.x*.3)*sin(p.y*.3+iTime)*.5 + .5;
   return v*v;
#else
   vec3 p3  = fract(vec3(p.xyx) * vec3(.1031, .1030, .0973));
   p3 += dot(p3, p3.yzx + 19.19);
   return fract((p3.x + p3.y) * p3.z);
#endif
}

vec2
hash22 (vec2 p)
{
#if ANIMATE == 0
   return texture( iChannel0, (p+0.5)/256.0, -100.0 ).xy;
#elif 1
   vec3 p3 = fract(vec3(p.xyx) * vec3(.1031, .1030, .0973));
   p3 += dot(p3, p3.yzx+19.19);
   vec2 v = fract(vec2((p3.x + p3.y)*p3.z, (p3.x+p3.z)*p3.y));
   return sin(iTime*ANIM_SPEED + v*6.283185)*.45 + .5;
#else
   vec2 v = texture( iChannel0, (p+0.5)/256.0, -100.0 ).xy;
   return sin(iTime*ANIM_SPEED + 6.283185*v)*.45 + .5;
#endif
}

const float eps = .0001;
const float max_dist = 9999.;

float
find_neighbour (vec2 n, vec2 f, vec2 dir, vec4 mc, out vec4 kc, out vec2 kdr)
{
  float kd = max_dist;
  for( int j=-2; j<=2; j++ )
  for( int i=-2; i<=2; i++ )
  {
    vec2 g = mc.zw + vec2(float(i),float(j));
    vec2 o = hash22( n + g );
    vec2 r = g + o - f;

    vec2 dr = r - mc.xy;
    if ( dot(dr,dr) > eps )
    {
      float d = dot(dir,dr);
      if ( d > eps )
      {
        d = dot(.5*(mc.xy + r),dr)/d;
        if (d < kd)
        {
            kd = d;
            kdr = dr;
            kc = vec4(r, g);
        }
      }
    }
  }

  return kd;
}

vec4
voronoi_column_trace (vec3 ray_pos, vec3 ray_dir, float max_h, out vec4 hit_pos, out vec3 hit_norm, out vec3 hit_dh)
{
  const int iter = 32;
   
  vec2 p = ray_pos.xy;
  float s = 1./length(ray_dir.xy);
  vec2 dir = ray_dir.xy*s;
  vec2 n = floor(p);
  vec2 f = fract(p);
   
  vec4 mc;
  float md;

  md = 8.;

  for( int j=-1; j<=1; j++ )
  for( int i=-1; i<=1; i++ )
  {
    vec2 g = vec2(float(i),float(j));
    vec2 o = hash22( n + g );
    vec2 r = g + o - f;
    float d = dot(r,r);

    if( d<md )
    {
      md = d;
      mc = vec4(r, g);
    }
  }

  vec2 mdr = vec2(0,1);
  float dh = 0.;
  float prev_h = 0.;
  float h = 0.;

  md = eps;

  for( int k=0; k<iter; ++k )
  {
    h = hash12( mc.zw + n )*max_h;
    if (ray_dir.z >= 0.)
    {
      dh = ray_pos.z + ray_dir.z*md;
      if (dh < h || dh > max_h)
        break;
    }

    vec4 kc;
    vec2 kdr;
    float kd = find_neighbour(n, f, dir, mc, kc, kdr)*s;

    if (ray_dir.z < 0.)
    {
      dh = ray_pos.z + ray_dir.z*kd;
      if (dh < h)
        break;
    }

    mc = kc;
    md = kd;
    mdr = kdr;
    prev_h = h;
  }

  if (dh >= h)
  {
    hit_pos = vec4(ray_pos + ray_dir*max_dist,max_dist);
    hit_norm = vec3(0,0,1);
    hit_dh = vec3(1,1,h);
    return vec4(0);
  }

  float d;
  if (ray_dir.z >= 0.)
  {
    d = md;
    hit_norm = vec3(-normalize(mdr),0);
    hit_dh = vec3(vec2(ray_pos.z + ray_dir.z*d - prev_h, h-prev_h)/max_h,h);
  }
  else
  {
    d = (h - ray_pos.z)/ray_dir.z;
    if (md > d)
    {
      d = md;
      hit_norm = vec3(-normalize(mdr),0);
      hit_dh = vec3(vec2(ray_pos.z + ray_dir.z*d - prev_h, h-prev_h)/max_h,h);
    }
    else
    {
      hit_norm = vec3(0,0,1);
      hit_dh = vec3(1,1,h);
    }
  }
  hit_pos = vec4(ray_pos + ray_dir*d, d);
  return mc + vec4(p, n);
} 

vec4
voronoi_column_trace_shadow (vec4 mc, vec3 ray_pos, vec3 ray_dir, float max_h, out vec4 hit_pos, out vec3 hit_norm)
{
  const int iter = 8;

  vec2 p = ray_pos.xy;
  float s = 1./length(ray_dir.xy);
  vec2 dir = ray_dir.xy*s;
  vec2 n = floor(p);
  vec2 f = fract(p);

  mc -= vec4(p, n);

  float md;

  vec2 mdr = vec2(0,1);
  float dh = 0.;
  float h = 0.;

  md = eps;

  for( int k=0; k<iter; ++k )
  {
    vec4 kc;
    vec2 kdr;
    float kd = find_neighbour(n, f, dir, mc, kc, kdr)*s;

    mc = kc;
    md = kd;
    mdr = kdr;

    h = hash12( mc.zw + n )*max_h;
    dh = ray_pos.z + ray_dir.z*md;
    if (dh > max_h || dh < h) break;
  }

  if (dh >= h)
  {
    hit_pos = vec4(ray_pos + ray_dir*max_dist,max_dist);
    hit_norm = vec3(0,0,1);
    return vec4(0);
  }

  float d = md;
  hit_norm = vec3(-normalize(mdr),0);
  hit_pos = vec4(ray_pos + ray_dir*d, d);
  return mc + vec4(p, n);
}

float
voronoi_column_ao (vec2 x, vec4 mc)
{
  vec2 n = floor(x);
  vec2 f = fract(x);

  vec2 mr = mc.xy - x;
  vec2 mg = mc.zw - n;

  float mh = hash12( n + mg );

  mg = step(.5,f) - 1.;

  float mao = 0.;
  for( int j=-1; j<=2; j++ )
  for( int i=-1; i<=2; i++ )
  {
    vec2 g = mg + vec2(float(i),float(j));
    vec2 o = hash22( n + g );
    vec2 r = g + o - f;

    if( dot(mr-r,mr-r)>eps )
    {
      float d = dot( 0.5*(mr+r), normalize(r-mr) );
      float h = hash12( n + g );
      float ao = clamp((h - mh)*2.,0.,.5)*max(0., 1. - d*4.);
      mao = max(mao, ao);
    }
  }

  return mao;
}

const float fog_density = .04;
const float fog_start = 16.;
const float cam_dist = 13.5;
const float ground = -.2;
#if WAVING == 1
const float max_h = 3.;
#else
const float max_h = 2.;
#endif

vec4
trace (vec3 ray_start, vec3 ray_dir, vec3 light_dir, out vec4 norm_h, out vec4 ret_mc, out vec2 shadow_ao)
{
  vec3 p = ray_start;
  p.y += ground;

  norm_h = vec4(0,1,0,0);
  ret_mc = vec4(9999.,0,0,0);
  shadow_ao = vec2(0);

  if (p.y > max_h && ray_dir.y > -.0001)
    return vec4(0.);

  float dist = p.y - max_h;
  if (dist > .001)
  {
    dist /= -ray_dir.y;
    p += ray_dir*dist;
  }
  else
  {
    dist = 0.;
  }

  vec4 hit;
  vec3 hit_dh;
  ret_mc = voronoi_column_trace(p.xzy, ray_dir.xzy, max_h, hit, norm_h.xyz, hit_dh);
#if AO == 1
  if (hit_dh.x < 1.)
  {
    shadow_ao.y = max(0., 1. - hit_dh.x*8.)*.5*hit_dh.y;
  }
  else
  {
    shadow_ao.y = voronoi_column_ao(hit.xy, ret_mc);
  }
#endif

#if SHADOWS == 1
  if (hit.w < 48.-dist)
  {
    vec3 sh_dir = light_dir.xzy;
    vec4 sh_hit;
    vec3 sh_norm;
    if (dot(sh_dir,norm_h.xyz) < .001)
    {
      shadow_ao.x = 1.;
    }
    else
    {
      vec4 smc = voronoi_column_trace_shadow(ret_mc, hit.xyz, sh_dir, max_h, sh_hit, sh_norm);
      shadow_ao.x = 1.-step(max_dist-1.,sh_hit.w);
    }
  }
#endif

  hit = hit.xzyw;
  hit.w += dist;
  hit.y -= ground + hit_dh.z;
  norm_h = vec4(norm_h.xzy, hit_dh.z/max_h);
  return hit;
}

vec3
shade (vec3 ray_start, vec3 ray_dir, vec4 norm_h, vec4 mc, vec2 shadow_ao, vec3 light_dir, vec3 fog_color, vec4 hit)
{
  vec3 norm = norm_h.xyz;
  float diffuse = pow(max(0.05, dot(norm, light_dir)*(1.-shadow_ao.x*.9)),.3);
  diffuse *= 1. - shadow_ao.y*.7*(1.-shadow_ao.x*.6);
  float spec = max(0.0,dot(reflect(light_dir,norm),normalize(ray_dir)));
  spec = pow(spec, 32.0)*.5*(1.-shadow_ao.x);

#if USE_COLORS == 0
  vec3 base_color = vec3(.6);
#else
#if USE_COLORS == 3
#if WAVING == 1
  float sh = sqrt(max(0.,norm_h.w))*.8+.2;
#else
  float sh = norm_h.w*.8+.2;
#endif
  vec3 base_color = vec3(exp(pow(sh-.75,2.)*-10.),
                         exp(pow(sh-.50,2.)*-20.),
                         exp(pow(sh-.25,2.)*-10.));
#elif USE_COLORS == 1
  float sh = mc.z;
  sh = (abs(mod(sh+6.,12.)-6.)+2.5)*(1./9.);
  vec3 base_color = vec3(exp(pow(sh-.75,2.)*-10.),
                         exp(pow(sh-.50,2.)*-20.),
                         exp(pow(sh-.25,2.)*-10.));
#else
  vec3 base_color = texture(iChannel1, mc.zw*.1, -100.).xyz;
#endif
  vec3 an = abs(norm);
  base_color = mix(base_color, texture(iChannel1, ((an.y>an.x&&an.y>an.z)?hit.xz:(an.x>an.z)?hit.yz:hit.xy)*.25).xyz, .2);
#endif

  if (hit.w < max_dist-1.)
  {
    base_color *= 1.-dot(mc.xy-hit.xz,mc.xy-hit.xz)*.25;
  }
  vec3 color = mix(vec3(0.),vec3(1.),diffuse)*base_color + spec*vec3(1.,1.,.9);

  float fog_dist = max(0.,hit.w - fog_start);
  float fog = 1.0 - 1.0/exp(fog_dist*fog_density);
  color = mix(color, fog_color, fog);

  return color;
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - iResolution.xy*0.5) / iResolution.y;

  vec3 light_dir = normalize(vec3(.25, .7, .25));

  float ms = 2.5/iResolution.y;
  float ang, si, co;
  ang = (iMouse.z > 0.0) ? (iMouse.x - iResolution.x*.5) * -ms :
#if ROTATE == 1
    -iTime*.125 + .7;
#else
    .7;
#endif
  si = sin(ang); co = cos(ang);
  mat3 cam_mat = mat3(
    co, 0., si,
    0., 1., 0.,
    -si, 0., co);
  ang = (iMouse.z > 0.0) ? (iMouse.y - iResolution.y) * -ms - .5 :
#if ROTATE == 1
    cos(-iTime*.5)*.2 + .8;
#else
    .8;
#endif
  ang = max(0.,ang);
  si = sin(ang); co = cos(ang);
  cam_mat = cam_mat * mat3(
    1., 0., 0.,
    0., co, si,
    0.,-si, co);

  vec3 pos = cam_mat*vec3(0., 0., -cam_dist);
  vec3 dir = normalize(cam_mat*vec3(uv, 1.));

  vec3 color;
  vec3 fog_color = vec3(min(1.,.4+max(-.1,dir.y*.8)));
  vec4 norm_h;
  vec4 mc;
  vec2 shadow_ao;
  vec4 hit = trace(pos, dir, light_dir, norm_h, mc, shadow_ao);
  if (hit.w == 0.)
    color = fog_color;
  else
    color = shade(pos, dir, norm_h, mc, shadow_ao, light_dir, fog_color, hit);

  color = pow(color,vec3(.6));

  gl_FragColor = vec4(color, 1.);
}
