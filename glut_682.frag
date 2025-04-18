uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

#define FOV_MORPH 1

float
box (vec3 p)
{
  return length(max(abs(p)-vec3(.5),0.0)) - 0.15;
}

vec3
rot (vec3 p, float f)
{
  float s = sin(f);
  float c = cos(f);
  p.xy *= mat2(c, -s, s, c);
  p.yz *= mat2(c, -s, s, c);
  return p;
}

vec3
trans (vec3 p, out float rotout)
{
  p.zx += iTime*8.0;

  vec3 b = vec3(4.);
  vec3 rep = floor(p/b);

  p = mod(p,b)-0.5*b;
  
  rotout = iTime*1.88 + (rep.x+rep.z+rep.y)*0.3;
  p = rot(p, rotout);
  return p;	
}

float
scene (vec3 p)
{
  float dummy;
  return box(trans(p,dummy));
}

vec3
normal (vec3 p, float d)
{
  vec3 e = vec3(0.04,.0,.0);
  return normalize(vec3(
    scene(p+e.xyy)-d,
    scene(p+e.yxy)-d,
    scene(p+e.yyx)-d));
}

void
main ()
{
  vec2 xy = gl_FragCoord.xy / iResolution.xy - vec2(0.5,0.5);
  xy.y *= -iResolution.y / iResolution.x;

  float time = iTime*0.5;
  vec3 ro = 1.5*normalize(vec3(cos(time),cos(time)*1.2,sin(time)));
  vec3 eyed = normalize(vec3(0.0) - ro);
  vec3 ud = normalize(cross(vec3(0.0,1.0,0.0), eyed));
  vec3 vd = normalize(cross(eyed,ud));

#if FOV_MORPH
  float fov = 3.14 * 0.8 + sin(time*1.334)*1.45;
#else
  float fov = 3.14 * 0.7;
#endif

  float f = fov * length(xy);
  vec3 rd = normalize(normalize(xy.x*ud + xy.y*vd) + (1.0/tan(f))*eyed);

  vec3 p = ro + rd;

  float dall,d;
  for(int i = 0; i < 64; i++)
  {
    d = scene(p);
    if(d < 0.06)
      break;
    p += d*rd;
    dall += d;
  }

  vec3 bg = normalize(p).zzz + 0.1;

  if(d < 0.06)
  {
    vec3 n = normal(p,d); 
    vec3 col = vec3(dot(vec3(0.0,0.0,1.0), n));
    float objrot;
    vec3 objp = trans(p,objrot);
    vec3 objn = abs(rot(n,objrot));
    
    vec2 uv = 
      (objn.y > 0.707) ? vec2(objp.zx) : 
      (objn.x > 0.707) ? vec2(objp.zy) :
                         vec2(objp.xy) ;
    vec3 tex = texture(iChannel0, uv).rgb;
    vec3 hl = smoothstep(0.6, 1.0, col);
    col *= clamp(tex.xyz+0.3, 0.0, 1.0);

    col = col + hl*.4;
    float fog = clamp(dall/mix(90.0,40.0,((rd.z+1.0)*0.5)), 0.0, 1.0);

    gl_FragColor = vec4(mix(col, bg, fog),1.0);
  }
  else
    gl_FragColor = vec4(bg, 1.0);
}
