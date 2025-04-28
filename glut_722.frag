#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;
uniform sampler2D iChannel2;
uniform sampler2D iChannel3;

#define GSCALE vec2(1./2.)
// Circles: 0, Square: 1, Rounded Square: 2.
#define SHAPE 2
//#define LIGHT_SIDES
//#define GRAYSCALE

#define FAR 20.

float objID;

mat2
rot2 (float a)
{
  float c = cos(a), s = sin(a);
  return mat2(c, -s, s, c);
}

float
hash21 (vec2 p)
{
  return fract(sin(dot(p, vec2(27.609, 57.583))) * 43758.5453);
}

vec3
hash23T (vec2 p)
{
  return textureLod(iChannel2, p, 0.).xyz;
}

vec3
tex3D (sampler2D t, vec3 p, vec3 n)
{
  n = max(abs(n) - .2, .001);
  n /= dot(n, vec3(1));

  vec3 tx = texture(t, p.yz).xyz;
  vec3 ty = texture(t, p.zx).xyz;
  vec3 tz = texture(t, p.xy).xyz;

  return (tx * tx * n.x + ty * ty * n.y + tz * tz * n.z);
}

vec3
getTex (sampler2D ch, vec2 p)
{
  vec3 tx = texture(ch, fract(p)).xyz;
  return tx * tx;
}

float
hm (vec2 p)
{
  return dot(getTex(iChannel1, p / 4.), vec3(.299, .587, .114));
}

float
opExtrusion (float sdf, float pz, float h)
{
  vec2 w = vec2(sdf, abs(pz) - h);
  return min(max(w.x, w.y), 0.) + length(max(w, 0.));
}

float
sBoxS (vec2 p, vec2 b, float sf)
{
  vec2 d = abs(p) - b + sf;
  return min(max(d.x, d.y), 0.) + length(max(d, 0.)) - sf;
}

vec2 oID;

vec4
blocks (vec3 q3)
{
  vec2 oDim = GSCALE;
  vec2 dim = GSCALE;

  float d = 1e5;

  float rndTh[4] = float[4](.333, .5, 1.01, 1.01);
  vec2 id = vec2(0);
  float boxID = 0.;
  oID = vec2(0);

  for (int k = 0; k < 3; k++)
  {
    for (int j = 0; j <= 1; j++)
    {
      for (int i = 0; i <= 1; i++)
      {
        vec2 ip0 = q3.xy / dim + vec2(i, j) - .5;

        float rndIJ[4];
        rndIJ[0] = hash21(floor(ip0));
        if (rndIJ[0] >= rndTh[k])
          continue;

        rndIJ[1] = hash21(floor(ip0 / 2.));
        if (k == 1 && rndIJ[1] < rndTh[0])
          continue;

        rndIJ[2] = hash21(floor(ip0 / 4.));
        if (k == 2 && (rndIJ[1] < rndTh[1] || rndIJ[2] < rndTh[0]))
          continue;

        vec2 p = q3.xy;
        vec2 ip = floor(ip0) + .5;
        p -= ip * dim;

        vec2 idi = ip * dim;

        float h = hm(idi);

        h *= .2;

        vec2 off = vec2(0);

#if SHAPE == 0
        float di2D = length(p - off) - dim.x/2.;
#elif SHAPE == 1
        float di2D = sBoxS(p, dim/2. - .01*oDim.x, .0);
#else
        float di2D = sBoxS(p, dim / 2. - .01 * oDim.x, .3 * (dim.x));
#endif

        vec3 r3 = hash23T(idi * 7.27183 + .36);
        float rnd = r3.x;
        if (rnd < .5)
          di2D = abs(di2D + .04 * oDim.x) - .04 * oDim.x;

        float di = opExtrusion(di2D, (q3.z + h - 32.125), h + 32.125);

        di += di2D / 8.;

        vec2 toID = vec2(0);

        if (k < 2 && rnd < .5)
        {
          float bh = sin(iTime * (r3.y + 5.) / 4. + dot(idi, vec2(27.163, 113.457)) + r3.z * 6.2831) * (dim.x + 1.);
          float qz = q3.z + h - 1. + bh;
          float ball = length(vec3(p, qz)) - .3 * dim.x * (r3.y * .5 + .5);

          toID.x = di < ball ? 0. : 1.;
          toID.y = bh;
          di = min(di, ball);
        }

        if (di < d)
        {
          d = di;
          id = idi;

          boxID = di2D;
          oID = toID;
        }
      }
    }

    dim /= 2.;
  }

  return vec4(d, id, boxID);
}

vec3 gID;

float
map (vec3 p)
{
  float fl = -p.z + 64.251;

  vec4 d4 = blocks(p);
  gID = d4.yzw;
 
  objID = fl < d4.x ? 1. : 0.;

  return min(fl, d4.x);
}

float
trace (vec3 ro, vec3 rd)
{
  float t = 0., d;

  for (int i = min(iFrame, 0); i < 96; i++)
  {
    d = map(ro + rd * t);
    if (abs(d) < .001 || t > FAR)
      break;

    t += d * .7;
  }

  return min(t, FAR);
}

vec3
getNormal (vec3 p, float t)
{
  const vec2 e = vec2(.001, 0);
    
  float sgn = 1.;
  float mp[6];
  vec3[3] e6 = vec3[3](e.xyy, e.yxy, e.yyx);
  for (int i = min(iFrame, 0); i < 6; i++)
  {
    mp[i] = map(p + sgn * e6[i / 2]);
    sgn = -sgn;
    if (sgn > 2.)
      break;
  }

  return normalize(vec3(mp[0] - mp[1], mp[2] - mp[3], mp[4] - mp[5]));
}

float
softShadow (vec3 ro, vec3 lp, vec3 n, float k)
{
  const int maxIterationsShad = 32;

  ro += n * .0015;
  vec3 rd = lp - ro;

  float shade = 1.;
  float t = 0.;
  float end = max(length(rd), 0.0001);
  rd /= end;

  for (int i = min(iFrame, 0); i < maxIterationsShad; i++)
  {
    float d = map(ro + rd * t) * .7;
    shade = min(shade, k * d / t);

    t += clamp(d, .01, .2);

    if (d < 0. || t > end)
      break;
  }

  return max(shade, 0.);
}

float
calcAO (vec3 p, vec3 n)
{
  float sca = 2., occ = 0.;
  for (int i = min(iFrame, 0); i < 5; i++)
  {
    float hr = float(i + 1) * .15 / 5.;
    float d = map(p + n * hr);
    occ += (hr - d) * sca;
    sca *= .7;

    if (sca > 1e5)
      break;
  }

  return clamp(1. - occ, 0., 1.);
}

float
n3D (vec3 p)
{
  const vec3 s = vec3(7, 157, 113);
  vec3 ip = floor(p);
  p -= ip;
  vec4 h = vec4(0., s.yz, s.y + s.z) + dot(ip, s);
  p = p * p * (3. - 2. * p); //p *= p*p*(p*(p * 6. - 15.) + 10.);
  h = mix(fract(sin(mod(h, 6.2831)) * 43758.5453), fract(sin(mod(h + s.x, 6.2831)) * 43758.5453), p.x);
  h.xy = mix(h.xz, h.yw, p.y);
  return mix(h.x, h.y, p.z); // Range: [0, 1].
}

vec3
envMap (vec3 p)
{
  p *= 4.;
  p.y += iTime;

  float n3D2 = n3D(p * 2.);

  float c = n3D(p) * .57 + n3D2 * .28 + n3D(p * 4.) * .15;
  c = smoothstep(.45, 1., c);

  p = vec3(c * c * c, c * c, c);

  return mix(p.zxy, p, n3D2);
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - iResolution.xy * .5) / iResolution.y;

  vec3 ro = vec3(0., iTime / 2., -1.8);
  vec3 lk = ro + vec3(0, 0, .25);
 
  vec3 lp = ro + vec3(-.5, 4, .9);

  float FOV = 1.;
  vec3 fwd = normalize(lk - ro);
  vec3 rgt = normalize(vec3(fwd.z, 0., -fwd.x));
  vec3 up = cross(fwd, rgt);

  vec3 rd = normalize(uv.x * rgt + uv.y * up + fwd / FOV);

  //rd.xy *= rot2( sin(iTime)/32. );
  rd.xy *= rot2(3.14159 / 5.);
  rd.yz *= rot2(-3.14159 / 5.);

  float t = trace(ro, rd);

  float gDist = t;

  vec3 svGID = gID;
  vec2 svoID = oID;
  float svObjID = objID;

  vec3 col = vec3(0);

  if (t < FAR)
  {
    vec3 sp = ro + rd * t;
    vec3 sn = getNormal(sp, t);
    vec3 ld = lp - sp;
    float lDist = max(length(ld), .001);
    ld /= lDist;

    float sh = softShadow(sp, lp, sn, 12.);
    float ao = calcAO(sp, sn);

    float atten = 1. / (1. + lDist * .05);

    float diff = max(dot(sn, ld), 0.);

    float spec = pow(max(dot(reflect(ld, sn), rd), 0.), 16.);

    float fre = pow(clamp(1. - abs(dot(sn, rd)) * .5, 0., 1.), 2.);

    float Schlick = pow(1. - max(dot(rd, normalize(rd + ld)), 0.), 5.);
    float freS = mix(.15, 1., Schlick);

    vec3 texCol;

    if (svObjID < .5)
    {
      vec3 tx = getTex(iChannel3, svGID.xy / 8. + .12);

      vec3 txP = sp;

      if (svoID.x == 1.)
      {
        txP += vec3(0, 0, svoID.y);

        tx = mix(tx, min(1. / vec3(dot(tx, vec3(.299, .587, .114))), 1.), .2);
      }

      tx = min(tx * 1.5, 1.);

      vec3 tx2 = tex3D(iChannel3, txP / 2. + svoID.x / 2. * 0., sn);
      tx2 = smoothstep(0., .5, tx2);

      texCol = mix(tx, tx * tx2 * 4., 1.);

      if (svoID.x == 0.)
      {
        float ht = hm(svGID.xy) * .2;
        float face = svGID.z;

        float face2 = face;
        face = max(abs(face), abs(sp.z + ht * 2.)) - .02;

        float lSc = 30.;
        float pat = (abs(fract((sp.x - sp.y) * lSc - .5) - .5) * 2. - .5) / lSc;

        vec3 tCol = texCol / 4.;

        vec3 faceCol = mix(texCol, vec3(0), (1. - smoothstep(0., .01, pat)) * .25);
        float ew = .03;
        tCol = mix(tCol, tx2 * vec3(1.3, .9, .3) * 4., (1. - smoothstep(0., .002, face2 + .01)));
        tCol = mix(tCol, texCol / 4., (1. - smoothstep(0., .002, face2 + ew)));
        tCol = mix(tCol, faceCol, (1. - smoothstep(0., .002, face2 + ew + .01)));

#ifdef LIGHT_SIDES
        texCol = tx2*vec3(1.3, .9, .3)*3.;
#endif

        texCol = mix(texCol, tCol, (1. - smoothstep(0., .002, sp.z + ht * 2.)));
      }

#ifdef GRAYSCALE
      texCol = vec3(dot(texCol, vec3(.299, .587, .114)));    
#endif
      texCol = mix(texCol, vec3(0), smoothstep(0., 1., sp.z));
    }
    else
      texCol = vec3(0);

    col = texCol * (diff * sh + vec3(.04, .08, .12) + vec3(.2, .4, 1) * fre * sh * 0. + vec3(1, .7, .4) * spec * freS * sh * 8.);

    vec3 cTex = envMap(reflect(rd, sn));
    col += col * cTex * 15.;

    col *= ao * atten;
  }

  gl_FragColor = vec4(max(col, 0.), gDist);
}
