#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

#define FAR 40.

float hash (float n) { return fract(cos(n)*45758.5453); }

vec3
tex3D (sampler2D tex, vec3 p, vec3 n)
{
  n = max(n*n, 0.001);
  n /= (n.x + n.y + n.z );
  return (texture(tex, p.yz)*n.x + texture(tex, p.zx)*n.y + texture(tex, p.xy)*n.z).xyz;
}

float
sminP (float a, float b, float smoothing)
{
  float h = clamp( 0.5+0.5*(b-a)/smoothing, 0.0, 1.0 );
  return mix( b, a, h ) - smoothing*h*(1.0-h);
}

float
map (vec3 q)
{
  vec3 p = abs(fract(q/3.)*3. - 1.5);
  float d = min(max(p.x, p.y), min(max(p.y, p.z), max(p.x, p.z))) - 1. + .05;

  p =  abs(fract(q) - .5);
  d = max(d, min(max(p.x, p.y), min(max(p.y, p.z), max(p.x, p.z))) - 1./3. + .05);

  p =  abs(fract(q*2.)*.5 - .25);
  d = max(d, min(max(p.x, p.y), min(max(p.y, p.z), max(p.x, p.z))) - .5/3. - .015);

  p =  abs(fract(q*3./.5)*.5/3. - .5/6.);
  return max(d, min(max(p.x, p.y), min(max(p.y, p.z), max(p.x, p.z))) - 1./18. - .015);
}

float
trace (vec3 ro, vec3 rd)
{
  float t = 0., d;
  for(int i=0; i< 64; i++)
  {
    d = map(ro + rd*t);
    if (d <.0025*t || t>FAR) break;
    t += d;
  }
  return t;
}

float
refTrace (vec3 ro, vec3 rd)
{
  float t = 0., d;
  for(int i=0; i< 16; i++)
  {
    d = map(ro + rd*t);
    if (d <.0025*t || t>FAR) break;
    t += d;
  }
  return t;
}

vec3
normal (vec3 p)
{
  vec2 e = vec2(0.005, -0.005);
  return normalize(e.xyy * map(p + e.xyy) + e.yyx * map(p + e.yyx) + e.yxy * map(p + e.yxy) + e.xxx * map(p + e.xxx));
}

float
calculateAO (vec3 p, vec3 n)
{
  float ao = 0.0, l;
  const float nbIte = 6.0;
  const float falloff = 1.;

  const float maxDist = 1.;
  for(float i=1.; i<nbIte+.5; i++)
  {
    l = (i + hash(i))*.5/nbIte*maxDist;
    ao += (l - map( p + n*l ))/ pow(1. + l, falloff);
  }

  return clamp( 1.-ao/nbIte, 0., 1.);
}

float
softShadow (vec3 ro, vec3 lp, float k)
{
  const int maxIterationsShad = 16;

  vec3 rd = (lp-ro);

  float shade = 1.0;
  float dist = 0.05;
  float end = max(length(rd), 0.001);
  float stepDist = end/float(maxIterationsShad);

  rd /= end;

  for (int i=0; i<maxIterationsShad; i++)
  {
    float h = map(ro + rd*dist);
    shade = min(shade, smoothstep(0.0, 1.0, k*h/dist));
    dist += clamp(h, 0.02, 0.25);

    if (h<0.001 || dist > end) break;
  }

  return min(max(shade, 0.) + 0.5, 1.0);
}

void
main ()
{
  vec3 rd = (vec3(2.*gl_FragCoord.xy - iResolution, iResolution.y));
  rd = normalize(vec3(rd.xy, sqrt(max(rd.z*rd.z - dot(rd.xy, rd.xy)*.2, 0.))));

  vec2 m = sin(vec2(0, 1.57079632) + iTime/4.);
  rd.xy = mat2(m.y, -m.x, m)*rd.xy;
  rd.xz = mat2(m.y, -m.x, m)*rd.xz;

  vec3 ro = vec3(0.0, 0.0, iTime);
  vec3 lp = ro  + vec3(0.0, 1.0, 0.0);

  vec3 col = vec3(0);

  float t = trace(ro, rd);

  if(t<FAR)
  {
    vec3 sp = ro + rd*t;
    vec3 sn = normal(sp);
    vec3 ref = reflect(rd, sn);

    const float ts = 2.;
    vec3 oCol = tex3D(iChannel0, sp*ts, sn);

    vec3 q = abs(mod(sp, 3.) - 1.5);
    if (max(max(q.x, q.y), q.z) < 1.063) oCol = oCol*vec3(.7, .85, 1.);

    oCol = smoothstep(0.0, 1.0, oCol);

    float sh = softShadow(sp, lp, 16.);
    float ao = calculateAO(sp, sn);

    vec3 ld = lp - sp;
    float lDist = max(length(ld), 0.001);
    ld /= lDist;

    float diff = max(dot(ld, sn), 0.);
    float spec = pow(max(dot(reflect(-ld, sn), -rd), 0.), 12.);

    float atten = 1.0 / (1.0 + lDist*0.25 + lDist*lDist*.1);

    diff += max(dot(-rd, sn), 0.)*.45;
    spec += pow(max(dot(reflect(rd, sn), -rd), 0.), 12.)*.45;

    float rt = refTrace(sp + ref*0.1, ref);
    vec3 rsp = sp + ref*rt;
    vec3 rsn = normal(rsp);

    vec3 rCol = tex3D(iChannel0, rsp*ts, rsn);
    q = abs(mod(rsp, 3.) - 1.5);
    if (max(max(q.x, q.y), q.z)<1.063) rCol = rCol*vec3(.7, .85, 1.);

    rCol = sqrt(rCol);
    float rDiff = max(dot(rsn, normalize(lp-rsp)), 0.);
    rDiff += max(dot(rsn, normalize(-rd-rsp)), 0.)*.45;

    float rlDist = length(lp - rsp);
    float rAtten = 1./(1.0 + rlDist*0.25 + rlDist*rlDist*.1);
    rCol = min(rCol, 1.)*(rDiff + vec3(.5, .6, .7))*rAtten;

    col = oCol*(diff + vec3(.5, .6, .7)) + vec3(.5, .7, 1)*spec*2. + rCol*0.25;

    col = min(col*atten*sh*ao, 1.);
  }

  col = mix(col, vec3(.55, .75, 1.), smoothstep(0., FAR - 15., t));

  gl_FragColor = vec4(col, 1.0);
}
