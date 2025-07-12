#version 330

mat2 rot2 (float a) { float c = cos(a), s = sin(a); return mat2(c, s, -s, c); }

float
hash21 (vec2 f)
{
  uvec2 p = floatBitsToUint(f + 1024.);
  p = 1664525U*(p>>1U^p.yx);
  return float(1103515245U*(p.x^(p.y>>3U)))/float(0xffffffffU);
}

float
sBoxS (vec2 p, vec2 b, float rf)
{
  vec2 d = abs(p) - b + rf;
  return min(max(d.x, d.y), 0.) + length(max(d, 0.)) - rf;
}

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

#define MOBIUS
#define LOG_POLAR
#define SPIRAL

#define PI 3.14159265358979
#define TAU 6.283185307179

#define R vec2(1, 0)

vec2 conj (vec2 a) { return vec2(a.x, -a.y); }
vec2 cmul (vec2 a, vec2 b) { return mat2(a, -a.y, a.x)*b; }
vec2 cinv (vec2 a) { return vec2(a.x, -a.y)/dot(a, a); }
vec2 cdiv (vec2 a, vec2 b) { return cmul(a, cinv(b)); }
vec2 clog (vec2 z) { return vec2(log(length(z)), atan(z.y, z.x)); }

vec2
mobius (vec2 z, vec2 a, vec2 b, vec2 c, vec2 d)
{
  return cdiv(cmul(z, a) + b, cmul(z, c) + d);
}

vec2
transf (vec2 z)
{
  vec2 fz = vec2(0);

  // Two metaball movement time variables.
  float time = iTime/4.;

  float time2 = time + 3.;

  int pNum = 5;
  // Set the particle's position, calculated it's field strength contribution, then
  // add it to the total field strength value.
  for(int i = 0; i<pNum; i++)
  {
    float fi = float(i);
    // Random sinusoidal motion for each particle. Made up on the spot, so could
    // definitely be improved upon.
    float rnd = fi*(PI*2. + .5453); //(hash(i)*.5 + .5)*PI*2.;
    vec2 pos = vec2(sin(time*.97 + rnd*3.11)*.5,
                        cos(time*1.03 + rnd*5.73)*.4);
    pos *= vec2(sin(time*1.05 + fi*(fi + 1.)*PI/9.11),
                    cos(time*.95 + fi*(fi - 1.)*PI/7.73))*1.25;

    if((i%2) == 1) pos *= (cos(time*3. + fi*3.14159/float(pNum))*.5 + .5);

    vec2 pos2 = vec2(sin(time2*.97 + rnd*3.11)*.5,
                     cos(time2*1.03 + rnd*5.73)*.4);
    pos *= vec2(sin(time2*1.05 + fi*(fi + 1.)*PI/9.11),
                cos(time2*.95 + fi*(fi - 1.)*PI/7.73))*1.25;

    if((i%2) == 1) pos2 *= (cos(time2*3. + fi*3.14159/float(pNum))*.5 + .5); // Smoother motion.

#ifdef MOBIUS
    vec2 zi = mobius(z, -R, pos, -R, pos2);
#else
    vec2 zi = z - pos;
#endif

#ifdef LOG_POLAR
    zi = clog(zi);
#endif

#ifdef SPIRAL
    vec2 e = vec2(1, 2./TAU);
    zi = cmul(zi, e);
#endif

    zi = zi/2.;

    fz += zi;
  }

  return fz;
}

vec2 dP;
vec3
distField (vec2 p)
{
  p = transf(p);

  vec2 op = p;

  vec2 sc = vec2(1, 1)/4.;
  sc.y *= 6.2831853/6.;

  float dir = mod(floor(p.x/sc.x), 2.)<.5? -1. : 1.;
  p.y += mod(dir*iTime/4., 6.2831853/6.*1.);

  vec2 ip = floor(p/sc);
  p -= (ip + .5)*sc;

  ip.y = mod(ip.y, 4.);
  ip.x = mod(ip.x, 4.);

  float d = sBoxS(p, sc/2., sc.x*.1);

  dP = p;

  return vec3(d, ip);
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - iResolution*.5)/iResolution.y;

  const float sc = 1.;
  float sf = sc/iResolution.y;

  vec2 p = sc*uv;

  float px = 1e-2/iResolution.y;
  vec2 dtX = (transf(uv + vec2(px, 0)) - transf(uv))/px;
  float dt = length(dtX);

  vec3 d3 = distField(p);

  float d = d3.x;
  vec2 id = d3.yz;

  vec3 d3Y = distField(p + vec2(0, px));
  float dB = d3Y.x;

  vec2 pp = dP;

  float rnd4 = hash21(id + .14);
  float rnd = hash21(floor(id/vec2(1, 4)) + .23) + rnd4*.2 - .05;

  vec3 sCol = .5 + .45*cos(TAU*(id.x + abs(id.y - 2.)/4.)/4. + vec3(0, 1, 2)*1.6);

  float b = (max(dB, -.025) - max(d, -.025))/px;
  b = max(.5 + b, 0.);
  sCol *= .5 + b;

  vec3 tx = texture(iChannel0, dP/dt).xyz; tx *= tx;
  sCol *= tx + .75;

  d /= dt;

  vec3 col = vec3(0);

  float thF = sqrt(450./iResolution.y);
  col = mix(col, sCol, 1. - smoothstep(0., sf, d + .005*thF));

  uv = gl_FragCoord.xy/iResolution;
  col *= pow(16.*uv.x*uv.y*(1. - uv.x)*(1. - uv.y) , 1./32.)*1.05;

  gl_FragColor = vec4(sqrt(max(col, 0.)), 1);
}
