uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;

const float PI  = 3.14159265359;

void
pR (inout vec2 p, float a)
{
  p = cos(a)*p + sin(a)*vec2(p.y, -p.x);
}

float
smin (float a, float b, float k)
{
  float f = clamp(0.5 + 0.5 * ((a - b) / k), 0., 1.);
  return (1. - f) * a + f  * b - f * (1. - f) * k;
}

float
smax (float a, float b, float k)
{
  return -smin(-a, -b, k);
}

float time;

vec4
leaf (vec3 p, vec2 uv)
{
  float thick = clamp(uv.y, .7, 1.);
  thick = 1.;
  float th = thick * .16;
  pR(p.xz, -uv.x);
  float width = mix(.5, .1, min(uv.y, 1.));
  width = .75 / uv.y;
  width *= thick;
  vec3 n = normalize(vec3(1,0,width));
  float d = -dot(p, n);
  d = max(d, dot(p, n * vec3(1,1,-1)));
  float len = mix(PI / 1.2, PI / 2., pow(uv.y/2.9, 2.));
  len = max(len, 0.);
  pR(p.yz, PI / 2. - len);
  d = smax(d, p.y, thick);
  d = smax(d, abs(length(p) - uv.y) - thick * th, th);
  vec2 uuv = vec2(atan(p.y, p.z) / -len, p.x);
  vec3 col = mix(vec3(0), vec3(.5,1,.7) * .05, 1.-smoothstep(.0, .5, uuv.x));
  col += vec3(.06,.0,.03) * max(1. - uv.y / 2., 0.);
  col = mix(col, col * .2, 1.-smoothstep(.0, .2, uuv.x));

  return vec4(d, col);
}

bool lightingPass;

vec4
opU (vec4 a, vec4 b)
{
  return a.x < b.x ? a : b;
}

vec4
bloom (vec3 p)
{
  float bound = length(p - vec3(0,-1.2,0)) - 3.3;
  bound = max(bound, p.y - 1.1);
  if (bound > .01 && !lightingPass)
    return vec4(bound, 0, 0, 0);

  vec2 cc = vec2(5., 8.);
  if (iMouse.z > 0.)
    cc = floor(iMouse.xy / iResolution.xy * 10.);
  float aa = atan(cc.x / cc.y);
  float r = (PI*2.) / sqrt(cc.x*cc.x + cc.y*cc.y);
  mat2 rot = mat2(cos(aa), -sin(aa), sin(aa), cos(aa));

  vec2 offset = vec2(1, 2) * time * r * rot;

  vec2 uv = vec2(atan(p.x, p.z), length(p));

  uv -= offset;

  uv = rot * uv;
  vec2 cell = round(uv / r);

  vec4 d = vec4(1e12, vec3(0));

  d = opU(d, leaf(p, ((cell + vec2(-1, 0)) * rot * r) + offset));
  d = opU(d, leaf(p, ((cell + vec2(0, -1)) * rot * r) + offset));
  d = opU(d, leaf(p, ((cell + vec2(0, 0)) * rot * r) + offset));
  d = opU(d, leaf(p, ((cell + vec2(1, -1)) * rot * r) + offset));
  d = opU(d, leaf(p, ((cell + vec2(1, 0)) * rot * r) + offset));

  d = opU(d, leaf(p, ((cell + vec2(-1, -1)) * rot * r) + offset));
  d = opU(d, leaf(p, ((cell + vec2(-1, 1)) * rot * r) + offset));
  d = opU(d, leaf(p, ((cell + vec2(0, 1)) * rot * r) + offset));
  d = opU(d, leaf(p, ((cell + vec2(1, 1)) * rot * r) + offset));

  return d;
}

vec4
map (vec3 p)
{
  return bloom(p);
}

vec3
calcNormal (vec3 pos)
{
  float eps = .0005;
  vec2 e = vec2(1.0,-1.0) * 0.5773;
  return normalize(e.xyy * map(pos + e.xyy * eps).x + 
                   e.yyx * map(pos + e.yyx * eps).x + 
                   e.yxy * map(pos + e.yxy * eps).x + 
                   e.xxx * map(pos + e.xxx * eps).x);
}

float
softshadow (vec3 ro, vec3 rd, float mint, float tmax)
{
  float res = 1.0;
  float t = mint;
  float ph = 1e10;
    
  for( int i=0; i<64; i++ )
  {
    float h = map( ro + rd*t ).x;
    res = min( res, 10.0*h/t );
    t += h;
    if( res<0.0001 || t>tmax )
      break;
  }
  return clamp( res, 0.0, 1.0 );
}

float
calcAO (vec3 pos, vec3 nor)
{
  float occ = 0.0;
  float sca = 1.0;
  for( int i=0; i<5; i++ )
  {
    float hr = 0.01 + 0.12*float(i)/4.0;
    vec3 aopos =  nor * hr + pos;
    float dd = map( aopos ).x;
    occ += -(dd-hr)*sca;
    sca *= 0.95;
  }
  return clamp( 1.0 - 3.0*occ, 0.0, 1.0 );    
}

mat3
calcLookAtMatrix (vec3 ro, vec3 ta, float roll)
{
  vec3 ww = normalize( ta - ro );
  vec3 uu = normalize( cross(ww,vec3(sin(roll),cos(roll),0.0) ) );
  vec3 vv = normalize( cross(uu,ww));
  return mat3( uu, vv, ww );
}

void
main ()
{
  vec3 col;
  vec3 tot = vec3(0.0);

  float mTime = mod(iTime / 2., 1.);
  time = mTime;

  vec2 o = vec2(0);

#ifdef AA
  for( int m=0; m<AA; m++ )
  for( int n=0; n<AA; n++ )
  {
    o = vec2(float(m),float(n)) / float(AA) - 0.5;
    float d = 0.5*sin(gl_FragCoord.x*147.0)*sin(gl_FragCoord.y*131.0);
    time = mTime - 0.1*(1.0/24.0)*(float(m*AA+n)+d)/float(AA*AA-1);
#endif
  lightingPass = false;

  vec2 p = (-iResolution + 2.0*(gl_FragCoord.xy+o))/iResolution.y;

  vec3 camPos = vec3(0.5, 7.4, -8.7) * .9;
  mat3 camMat = calcLookAtMatrix( camPos, vec3(0,-1.4,0), -0.5);
  vec3 rd = normalize( camMat * vec3(p.xy,2.8) );

  vec3 pos = camPos;
  float rayLength = 0.;
  float dist = 0.;
  bool bg = false;
  vec4 res;

  for (int i = 0; i < 100; i++)
  {
    rayLength += dist;
    pos = camPos + rd * rayLength;
    res = map(pos);
    dist = res.x;

    if (abs(dist) < .001)
      break;

    if (rayLength > 16.)
    {
      bg = true;
      break;
    }
  }

  col = vec3(.19,.19,.22) * 1.9;
  col = vec3(.35,.24,.0) * 1.8;
  col = vec3(.5,1,.7) * .05;
        
  if ( ! bg)
  {
    lightingPass = true;

    vec3 nor = calcNormal(pos);
    float occ = calcAO( pos, nor );
    vec3  lig = normalize( vec3(-.2, 1.5, .3) );
    vec3  lba = normalize( vec3(.5, -1., -.5) );
    vec3  hal = normalize( lig - rd );
    float amb = sqrt(clamp( 0.5+0.5*nor.y, 0.0, 1.0 ));
    float dif = clamp( dot( nor, lig ), 0.0, 1.0 );
    float bac = clamp( dot( nor, lba ), 0.0, 1.0 )*clamp( 1.0-pos.y,0.0,1.0);
    float fre = pow( clamp(1.0+dot(nor,rd),0.0,1.0), 2.0 );

    occ = mix(1., occ, .8);
            
    dif *= softshadow( pos, lig, 0.001, .9 );

    float spe = pow( clamp( dot( nor, hal ), 0.0, 1.0 ),16.0)* dif * (0.04 + 0.96*pow( clamp(1.0+dot(hal,rd),0.0,1.0), 5.0 ));

    vec3 lin = vec3(0.0);
    lin += 2.80*dif*vec3(1.30,1.00,0.70);
    lin += 0.55*amb*vec3(0.40,0.60,1.15)*occ;
    lin += 1.55*bac*vec3(0.25,0.25,0.25)*occ*vec3(2,0,1);
    lin += 0.25*fre*vec3(1.00,1.00,1.00)*occ;

    col = res.yzw;
    col = col*lin;
    col += 5.00*spe*vec3(1.10,0.90,0.70);
  }
  tot += col;
#ifdef AA
  }
  tot /= float(AA*AA);
#endif

  col = tot;
  col *= 1.3;    
  col = pow( col, vec3(0.4545) );

  gl_FragColor = vec4(col,1.0);
}
