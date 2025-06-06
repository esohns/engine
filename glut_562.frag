uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;

#define EPS 0.01
#define STEPS 64
#define TAU 6.28318530718

#define ROTATION
#define HOMOTOPY

float
mobius (vec3 p, float b)
{
  float x = p.x, y = p.y, z = p.z;
  float xx = x*x, yy = y*y, zz = z*z, y3 = yy*y, x3 = xx*x;
  float xy = xx+yy, b2 = b*2.0, zxy = z*(xx*y*3.0-y3), xyy = x*yy*3.0-x3;
  float k1 = (2.0*zxy+xyy*(xy-zz+1.0))*(b-0.1)-xy*xy*(b2+0.2);
  float k2 = b*xy*0.2+(b2-0.2)*(zxy+xyy)-xy*(b+0.1)*(xy+zz+1.0);
  return k1*k1-xy*k2*k2;
}

vec3
grad (vec3 p, float b)
{
  vec2 q = vec2(0.0, EPS);
  return vec3(mobius(p+q.yxx, b) - mobius(p-q.yxx, b), 
          mobius(p+q.xyx, b) - mobius(p-q.xyx, b),
          mobius(p+q.xxy, b) - mobius(p-q.xxy, b));
}

float
torus (vec3 p)
{
  vec2 t = vec2(1.0, 0.32);
  vec2 q = vec2(length(p.xy)-t.x,p.z);
  return length(q)-t.y;
}

mat3
rotY (float ang)
{
  float c = cos(ang), s = sin(ang);
  return mat3(c, 0.0, s, 0.0, 1.0, 0.0, -s, 0.0, c);
}

mat3
rotX (float ang)
{
  float c = cos(ang), s = sin(ang);
  return mat3(1.0, 0.0, 0.0, 0.0, c, -s, 0.0, s, c);
}

vec3
shade (vec3 p, vec3 rd, float b, mat3 m)
{
  vec3 col = vec3(0.0);
  vec3 n = normalize(-grad(p, b));
  
  vec3  amb = vec3(0.05375, 0.05, 0.06625);
  vec3  dif = vec3(0.18275, 0.17, 0.22525);
  vec3  spe = vec3(0.332741, 0.328634, 0.346435);
  float shin = 39.4;
  
  vec3 l = normalize(m*vec3(1.0));
  vec3 h = normalize(l-rd);
  float lambert = max(0.0, dot(n, l));
  float blinn = lambert > 0.0 ? pow(max(0.0, dot(n, h)), shin) : 0.0;
  col += vec3(3.0, 2.0, 3.0)*(0.4*dif*lambert + 1.4*spe*blinn + 0.1*amb);

  lambert = max(0.0, dot(n, -rd));
  blinn = lambert > 0.0 ? pow(lambert, shin) : 0.0;
  col += vec3(1.0)*(0.4*dif*lambert + 1.4*spe*blinn + 0.1*amb);

  col += 2.25*pow(clamp(1.0+dot(n, rd), 0.0, 1.0), 3.0); 

  return col/(col+1.0);
}

float
animCurve (float t)
{
  t = mod(iTime, 15.0);
  float f1 = smoothstep(5.0, 7.0, t);
  float f2 = 1.0-smoothstep(7.0, 9.0, t);
  return 0.01+0.09*f1*f2;
}

void
main ()
{
  vec2 fc = gl_FragCoord.xy / iResolution.xy;
  vec2 uv = -1.0+2.0*fc;
  uv.x *= iResolution.x/iResolution.y;	

  vec2 mouse = 0.5*TAU*(-1.0+2.0*iMouse.xy/iResolution.xy);

#ifndef HOMOTOPY
  float b = 0.01;
#else
  float b = animCurve(iTime);
#endif

#ifdef ROTATION
  mouse.x += 0.3*iTime;
#endif

  mat3 m = rotY(mouse.x)*rotX(mouse.y);
  vec3 ro = m*vec3(0.0, 0.0, 1.8);
  vec3 rd = m*normalize(vec3(uv, -1.0));

  float d = 10.0, t1 = 0.0;
  vec3 p = ro, col = vec3(1.0);

  for (int i = 0; i < STEPS; ++i)
  {
    if (d < EPS || t1 > 4.0)
      continue;
    d = torus(p);
    t1 += d; p = ro + t1*rd;
  }

  if (d < EPS)
  {
    float t2 = t1; d = mobius(p, b);
    for (int i = 0; i < 2*STEPS; ++i)
    {
      if (d > 0.0)
        continue;
      d = mobius(p, b);
      t2 += 2.0*EPS; p = ro + t2*rd;
    }
    if (d > 0.0)
    {
      for (int i = 0; i < 12; ++i)
      {
        d = 0.5*(t1+t2); p = ro + d*rd;
        if (mobius(p, b) > 0.0)
          t2 = d;
        else
          t1 = d;
      }
      col = shade(ro+d*rd, rd, b, m);
    }
  }

  col = smoothstep(0.0, 1.0, col);
  col *= 0.5 + 0.5*pow(25.0*fc.x*(1.0-fc.x)*fc.y*(1.0-fc.y), 0.45);
  col = pow(col, vec3(1.0/2.2));

  gl_FragColor = vec4(col,1.0);
}
