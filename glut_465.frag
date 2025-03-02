uniform vec2 iResolution;
uniform float iTime;

//varying vec2 vTexCoord;

// Platimum: 0, Gold: 1, Gold and Blue: 2
#define COLOR 0

// 2D rotation. Always handy. Angle vector, courtesy of Fabrice.
mat2
rot (float th)
{
  vec2 a = sin (vec2 (1.5707963, 0.0) + th);
  return mat2 (a, -a.y, a.x);
}

float
n3D (vec3 p)
{
  const vec3 s = vec3 (7.0, 157.0, 113.0);
  vec3 ip = floor (p); p -= ip;
  vec4 h = vec4 (0., s.yz, s.y + s.z) + dot (ip, s);
  p = p * p * (3.0 - 2.0 * p); //p *= p*p*(p*(p * 6. - 15.) + 10.);
  h = mix (fract (sin (h) * 43758.5453), fract (sin (h + s.x) * 43758.5453), p.x);
  h.xy = mix (h.xz, h.yw, p.y);
  return mix (h.x, h.y, p.z);
}

vec2
hash22 (vec2 p, float w)
{
  // The Voronoi pattern needs to be repeatable. Hence the "mod" line below.
  p = mod (p, w);
  float n = sin (dot (p, vec2 (41.0, 289.0)));
  return fract (vec2 (262144.0, 32768.0) * n) * 0.9 + 0.1;

  // Animated.
  //p = fract(vec2(262144, 32768)*n); 
  //return sin( p*6.2831853 + iTime )*.45 + .5; 
}

float vx;
float
Voronoi (vec2 p, float w)
{
  vec2 g = floor (p), o; p -= g;
  vec3 d = vec3 (1.4142);
  for (int y = -1; y <= 1; y++)
  {
    for (int x = -1; x <= 1; x++)
    {
      o = vec2(x, y);
      o += hash22 (g + o, w) - p;
      d.z = length (o);//(dot(o, o)); 

      // More distance metrics.
      //o = abs(o);
      //d.z = mix(max(abs(o.x)*.866025 + o.y*.5, -o.y), dot(o, o), .2);//
      //d.z = max(abs(o.x)*.866025 - o.y*.5, o.y);
      //d.z = max(abs(o.x) + o.y*.5, -(o.y)*.8660254);
      //d.z = max(o.x, o.y);
      //d.z = (o.x*.7 + o.y*.7);
      d.y = max (d.x, min (d.y, d.z));
      d.x = min (d.x, d.z); 
    }
  }
  d /= 1.4142;
  vx = d.x;
  d = smoothstep (0.0, 1.0, d);
  return max (d.y / 1.333 - d.x, 0.0) * 1.333;
  //return d.y - d.x;
}

float objID;
float
bumpFunction (vec3 p)
{
  vec2 uv = vec2 (atan (p.y, p.x) / 6.2831853, p.z / 8.0);
  float c = Voronoi (uv * 16.0, 16.0);

  objID = 0.0;
  if (c < 0.15)
  {
    c = abs (max (c, 0.01) - 0.3), objID = 1.0;
  }
  return c;
}

vec3
doBumpMap (vec3 p, vec3 n, float bumpfactor, inout float edge)
{
  vec2 e = vec2 (2.5 / iResolution.y, 0.0);
  float f = bumpFunction (p);
  float fx = bumpFunction (p - e.xyy);
  float fy = bumpFunction (p - e.yxy);
  float fz = bumpFunction (p - e.yyx);
  float fx2 = bumpFunction (p + e.xyy);
  float fy2 = bumpFunction (p + e.yxy);
  float fz2 = bumpFunction (p + e.yyx);

  vec3 grad = (vec3 (fx - fx2, fy - fy2, fz - fz2)) / e.x / 2.0;

  edge = abs (fx + fy + fz + fx2 + fy2 + fz2 - 6.0 * f);
  //edge = abs(fx + fx2 - f*2.) + abs(fy + fy2 - f*2.)+ abs(fz + fz2 - f*2.);
  edge = smoothstep (0.0, 1.0, edge / e.x);

  grad -= n * dot (n, grad);

  return normalize (n + grad * bumpfactor);
}


float
bumpFunction2 (vec3 p)
{
  float c = n3D (p * 16.0);
  c = fract (c * 4.0);

  return min (c, c * (1.0 - c) * 4.0);
}

vec3
doBumpMap (vec3 p, vec3 n, float bumpfactor)
{
  vec2 e = vec2 (2.5 / iResolution.y, 0.0);
  float f = bumpFunction2 (p); 
  float fx = bumpFunction2 (p - e.xyy);
  float fy = bumpFunction2 (p - e.yxy);
  float fz = bumpFunction2 (p - e.yyx);

  vec3 grad = (vec3 (fx, fy, fz) - f) / e.x;
  grad -= n * dot (n, grad);

  return normalize (n + grad * bumpfactor);
}

void
main ()
{
  vec3 rd = normalize (vec3 (gl_FragCoord.xy - iResolution.xy * 0.5, iResolution.y));
  rd.xy = rot (iTime * 0.25) * rd.xy;
  rd.xz = rot (iTime * 0.125) * rd.xz;

  vec3 ro = vec3 (0.0, 0.0, iTime);

  vec3 col = vec3 (0.0);

  const float rad = 1.0;

  float sDist = rad / max (length (rd.xy), 0.001);

  vec3 sp = ro + rd * sDist;
  vec3 sn = normalize (vec3 (-sp.xy, 0.0));

  float edge;
  sn = doBumpMap (sp, sn, 0.75, edge);
  float svObjID = objID;
  if (svObjID > 0.5)
    sn = doBumpMap (sp, sn, 0.003);

  vec3 tex = vec3 (1.0) * n3D (fract (sp) * 192.0);
  vec3 objCol = smoothstep (0.1, 0.9, tex) * 0.8 + 1.0;

  if (svObjID < 0.5)
    objCol *= vec3 (0.45, 0.425, 0.5);
  else
    objCol *= vec3 (0.68, 0.64, 0.75);

#if COLOR == 1
  // Gold color scheme.
  if (svObjID > 0.5)
    objCol *= vec3 (1.4, 0.8, 0.3);
  else
    objCol *= vec3 (0.5, 2.0, 5.0).zyx;
#elif COLOR == 2
  if (svObjID > 0.5)
    objCol *= vec3 (1.0, 0.8, 0.2) * 1.35;
  else
    objCol *= vec3 (0.1, 2.5, 5.0);
#endif

  // Lighting.
  vec3 lp = ro + vec3 (0.0, 0.5, 0.0);
  vec3 ld = lp - sp;
  float dist = max (length (ld), 0.001);
  ld /= dist;

  float atten = 1.5 / (1.0 + dist * 0.05 + dist * dist * 0.075);
    
  atten *= bumpFunction (sp) * 0.9 + 0.1;//getGrey(texCol(iChannel0, sp))*.5 + .5;

  float diff = max (dot (ld, sn), 0.0);
  diff = pow (diff, 8.0) * 0.66 + pow (diff, 16.0) * 0.34;

  float spec = pow (max (dot (reflect (ld, sn), rd), 0.0), 8.0);

  vec3 ref = sp + refract (rd, sn, 1.0 / 1.6);
  vec2 tuv = vec2 (atan (ref.y, ref.x) / 6.2832, ref.z / 8.0);
  float c2 = Voronoi (tuv * 4.0 - vec2 (1.0, 0.5) * iTime, 4.0);
  c2 = smoothstep (0.8, 1.0, 1.0 - vx);

  vec3 elec = (objCol * 0.7 + 0.3) * pow (min (vec3 (1.5, 1.0, 1.0) * c2, 1.0), vec3 (1.0,  3.0, 8.0));

  if (svObjID < 0.5)
    objCol += elec * 8.0;
  else
    objCol += elec * 2.0;

  col += (objCol * (diff * vec3 (1.0, 0.97, 0.92) * 2.0 + 0.25) + vec3 (1.0, 0.6, 0.2) * spec * 2.0) * atten;

  ref = reflect (rd, sn);
  float rc = n3D (ref * 2.0);
  col += col * smoothstep (0.3, 1.0, rc) * 4.0 * atten;
  col *= 1.0 - edge * 0.7;

  //col = mix(col, vec3(2, 1.5, 1).zyx, 1. - exp(-.002*sDist*sDist)); // Blue fog.
  col = mix (col, vec3 (0.0), 1.0 - exp (-0.002 * sDist * sDist));

  gl_FragColor = vec4 (sqrt (clamp (col, 0.0, 1.0)), 1.0);
}
