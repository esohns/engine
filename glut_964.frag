#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;
uniform sampler2D iChannel3;

// 0: No accumulation -- Noisy, sharper picture, but with no blur. 
// 1: Regular accumulation with no reprojection -- A mixture.
// 2: Temporal reprojection. -- Smoother for faster machines.
#define BUFF_ACCUM 2

#define FAR 25.

vec2 seed = vec2(.13, .27);

vec3
rnd23 ()
{
  seed = fract(seed + vec2(.723, .647));
  vec3 p = vec3(dot(seed.xy, vec2(12.989, 78.233)), 
                dot(seed.xy, vec2(41.898, 57.267)),
                dot(seed.xy, vec2(65.746, 83.765)));

  return fract(sin(p)*vec3(43758.5453, 23421.6361, 34266.8747));
}

vec2
hash22 ()
{
  seed = fract(seed + vec2(.723, .647));

  return fract(sin(vec2(dot(seed.xy, vec2(12.989, 78.233)), dot(seed.xy, vec2(41.898, 57.263)))) * vec2(43758.5453, 23421.6361));
}

float hash21 (vec2 p) { return fract(sin(dot(p, vec2(27.619, 57.583)))*43758.5453); }
float hash31 (vec3 p) { return fract(sin(dot(p, vec3(91.537, 151.761, 72.453)))*435758.5453); }

vec3
cosDir (float seed, vec3 n)
{
  vec2 rnd = hash22();
  float u = rnd.x;
  float v = rnd.y;

  float a = 6.2831853*v;
  u = 2.*u - 1.;

  return normalize(n + vec3(sqrt(1. - u*u)*vec2(cos(a), sin(a)), u));
}

mat2 rot2 (float a) { float c = cos(a), s = sin(a); return mat2(c, -s, s, c); }

vec2 s = vec2(1, 1)/2.; 

float
rayLine (vec2 ro, vec2 rd, vec2 p, vec2 n)
{
  return dot(p - ro, n)/dot(rd, n);
}

vec2
gridID (vec2 p)
{
  return floor(p/s) + .5;
}

float
h (vec2 p)
{
  vec2 texSize = vec2(textureSize(iChannel0, 0));
  vec3 tx = texture(iChannel0, p / texSize * 4.).xyz;
  tx *= tx;
  float f = dot(tx, vec3(.299, .587, .114));
  float f2 = f;
  f *= min(p.x*p.x/16., 1.);

  return floor((f*12. + f2*4.)*8.)/8.;
}

vec4
raycast (vec3 ro, vec3 rd)
{
  vec4 res = vec4(FAR, 0, 0, 0);
    
  vec2 n1 = vec2(-1, 0), n2 = vec2(0, -1);

  float d1 = dot(rd.xz, n1);
  float d2 = dot(rd.xz, n2);
  n1 = d1<0.? -n1 : n1;
  n2 = d2<0.? -n2 : n2;

  vec3 pos = ro;

  vec2 ip = gridID(pos.xz);

  float t1 = 1e8, t2 = 1e8, tT = 1e8;

  int hit = 0;

  for(int i = 0; i<64; i++)
  {
    float ma = h(ip*s);

    if(pos.y<ma)
    {
      hit = 1;
      break; 
    } 

    t1 = rayLine(pos.xz, rd.xz, (ip + n1*.5)*s, -n1);
    t2 = rayLine(pos.xz, rd.xz, (ip + n2*.5)*s, -n2);

    vec3 tn = t1<t2? vec3(t1, n1) : vec3(t2, n2);

    tT = (ma - pos.y)/rd.y;
    tT = tT<0. ? 1e8 : tT;

    if(tT<tn.x)
    {
      pos += rd*tT; 
      hit = 2;
      break;
    }

    ip += tn.yz;

    pos += rd*tn.x;
  }

  float fID = tT<t1 && tT<t2? 0. : t1<t2? 1. : 2.;
  if(fID == 1.){ fID = d1<0.? -fID : fID; }
  else if(fID == 2.){ fID = d2<0.? -fID : fID; }

  res.x = length(pos - ro);
  if(hit == 0) res.x = FAR;

  return vec4(res.x, fID, ip);
}

vec3
nr (float fID, vec3 rd)
{
  vec3 n = fID == 0.? vec3(0, 1, 0) : abs(fID) == 1.? vec3(1, 0, 0) : vec3(0, 0, 1);
  n *= fID<-.001? -1. : 1.; 

  return n;
}

mat3
rot (vec3 ang)
{
  vec3 c = cos(ang), s = sin(ang);

  return mat3(c.x*c.z - s.x*s.y*s.z, -s.x*c.y, -c.x*s.z - s.x*s.y*c.z,
              c.x*s.y*s.z + s.x*c.z, c.x*c.y, c.x*s.y*c.z - s.x*s.z,
              c.y*s.z, -s.y, c.y*c.z);    
}

void
main ()
{
#if BUFF_ACCUM == 2
  vec3 resPos = vec3(0);
  float resT = 0.;
#endif

  vec2 uv0 = (gl_FragCoord.xy - iResolution*.5)/iResolution.y;

  seed = uv0 + vec2(fract(iTime/113.671)*.123, fract(iTime/57.913)*.14527);

  vec3 ro = vec3(-s.x/2., 3.5, iTime*.25); 

  vec3 cam = ro;

  float FOV = 1.;

  mat3 mCam = mat3(vec3(1, 0, 0), vec3(0, 1, 0), vec3(0, 0, 1));
  mCam *= rot(vec3(0, 0, cos(iTime/8.*.25)/4.));
  mCam *= rot(vec3(-sin(iTime/4.*.25)/8., 0, 0));
  mCam *= rot(vec3(0, .5, 0));

  vec3 atot = vec3(0);
  const int sampNum = 8;
    
  for(int j = min(0, iFrame); j<sampNum; j++)
  {
    vec2 jit = hash22() - .5;

    vec2 uv = uv0 + jit/iResolution.y;

    vec3 rd = mCam*normalize(vec3(uv, 1./FOV)); 

    float fDist = 6.;
    vec2 jitDOF = hash22()*2. - 1.;
    vec3 vDOF = mCam*vec3(jitDOF, 0.)*.035;
    rd = normalize(rd - vDOF/fDist);
    ro = cam + vDOF;

    vec3 acc = vec3(0);

    vec3 through = vec3(1);

    float t0; 

    for(int i = min(0, iFrame); i<3; i++)
    {
      vec4 res = raycast(ro, rd);

      float t = res.x, d;
      float fID = res.y;
      vec2 id = res.zw;

      t = min(t, FAR);

      if(i == 0) t0 = t;

      vec3 p = ro + rd*t;

#if BUFF_ACCUM == 2
      if(i==0)
      {
        resPos += p/float(sampNum); // Accumulative position.
        resT += t/float(sampNum); // Accumulative distance.
      }
#endif

      if(t<FAR - 1e-6)
      {
        vec3 n = nr(fID, rd);

        vec2 tuv = fID == 0.? p.xz : abs(fID) == 1.? p.zy : p.xy;
        vec3 tx = texture(iChannel1, tuv/3.).xyz; tx *= tx;

        vec3 oCol = .125 + tx*2.5;

        float h0 = h(id*s);

        float minEdge = min(s.x, s.y)/4.;
        float edge = s.y/4.;
        float edge2 = s.x/4.;

        vec2 lc = p.xz - id*s;

        vec2 ap = abs(lc) - s/2.;

        float fEdge = max(ap.x, ap.y);
        fEdge = abs(fEdge);
        fEdge = max(fEdge, -(p.y - h0)) - .015;

        float sEdge = min(ap.x, ap.y);
        sEdge = max(-sEdge, (p.y - h0)) - .015;

        fEdge = min(fEdge, sEdge);

        float sf = .001*(1. + res.x*res.x*.1);

        oCol = mix(oCol, vec3(0), (1. - smoothstep(0., sf, fEdge))*.85);

        float strip = abs(p.y - h0 + 2./8.) - 1./8.;
        oCol = mix(oCol, vec3(0), (1. - smoothstep(0., sf, abs(strip) - .02/2.))*.85);

        vec2 tip = floor(tuv*8.);
        vec2 tup = abs(tuv - (tip + .5)/8.);
        float sq = max(tup.x, tup.y) - .5/8.;

        float rough = .9;

        vec3 emissive = vec3(0);

        if(strip<.0)
        {
          oCol = mix(oCol, vec3(0), (1. - smoothstep(0., sf*8., abs(sq) - .01/2.))*.85);

          vec3 eCol = .5 + .45*cos(6.2831*hash21(tip +.17)/5. + vec3(0, 1.4, 2));

          emissive = oCol*eCol;
          if(hash21(id +.2)<.5) emissive = oCol*eCol.zyx;

          emissive = mix(emissive, emissive.xzy, floor(hash21(tip +.42)*4.999)/4.*.35);

          float blink = smoothstep(.2, .3, sin(6.2831*hash21(tip + .09) + iTime/4.)*.5 + .5);
          emissive *= mix(1., 0., blink);

          emissive *= 8.; 

          rough = hash21(tip + .21)*.75;
        }
        else
          oCol *= (1. + .25*cos(hash21(id + .06)*6.2831/4. + vec3(0, 1, 2)));

        emissive = mix(emissive, vec3(0), (1. - smoothstep(0., sf, fEdge))*.95);

        acc += emissive*through;
        through *= oCol;

        vec3 ref = reflect(rd, n);
        vec3 rrd = cosDir(0., n);

        float rChance = step(0., rough - hash21(uv + vec2(i*277, j*113) + fract(iTime*.97 + .137)));
        rd = (mix(ref, rrd, rChance));

        ro = p + n*.001;
      }
      else
      {
        acc += vec3(.4, .6, 1)*.035;
        break;
      }
    }

    vec3 sky = mix(vec3(1, .7, .5), vec3(.4, .6, 1), uv0.y*2.5 - .15)*1.5;

    acc = mix(acc, sky, smoothstep(.35, .99, t0/FAR));

    atot += acc;
  }

  vec3 col = atot/float(sampNum);

#if BUFF_ACCUM == 2
  float kFocLen = 1./FOV;
  vec3 pos = resPos;
  ivec2 q = ivec2(gl_FragCoord.xy);
  col = clamp(col, 0., 1.);

  mat3x4 oldCam = mat3x4(texelFetch(iChannel3, ivec2(0, 0), 0),
                         texelFetch(iChannel3, ivec2(1, 0), 0),
                         texelFetch(iChannel3, ivec2(2, 0), 0));

  vec4 wpos = vec4(pos, 1.);

  vec3 cpos = wpos*oldCam;

  vec2 npos = (kFocLen*2.)*cpos.xy/cpos.z;

  vec2 spos = .5 + .5*npos*vec2(iResolution.y/iResolution.x, 1);

  vec2 rpos = spos*iResolution;

  vec4 ocolt = textureLod(iChannel3, spos, 0.);

  if(iFrame>0 && resT<FAR && (rpos.y>1.5 || rpos.x>3.5))
    col = mix( ocolt.xyz, col, 1./8.);
    
  gl_FragColor = vec4(col, resT);

  if(q.y == 0 && q.x<3)
  {
    if(q.x == 0)
      gl_FragColor = vec4(mCam[0], -dot(mCam[0], cam));
    else if (q.x == 1)
      gl_FragColor = vec4(mCam[1], -dot(mCam[1], cam));
    else
      gl_FragColor = vec4(mCam[2], -dot(mCam[2], cam));
  }
#elif BUFF_ACCUM == 1
  vec4 preCol = texelFetch(iChannel3, ivec2(fragCoord), 0);
  float blend = (iFrame < 2) ? 1. : 1./4.; 
  gl_FragColor = mix(preCol, vec4(clamp(col, 0., 1.), 1), blend);
#else
  gl_FragColor = vec4(max(col, 0.), 1);
#endif
}
