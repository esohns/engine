uniform vec2 iResolution;
uniform float iTime;

#define FAR 80.

float objID;
vec3 vObjID;
float gID; 

mat2 rot2 (float a) { float c = cos(a), s = sin(a); return mat2(c, -s, s, c); }

float
tube (vec2 p)
{
  return max(max(p.x, p.y), (p.x + p.y)*.5773);
}

vec4
torTube (vec3 p)
{
  const float rad2 = .065;

  float tb = tube(abs(vec2(length(p.xy) - .5, p.z))) - rad2;

  float innerTb = tb + .015; 

  const float aNum = 24.; 

  float a = atan(p.y, p.x);    
  float ia = floor(a/6.283*aNum) + .5;

  p.xy = rot2(ia*6.283/aNum)*p.xy;

  p.x -= .5;

  p = abs(p);

  float band = 1e5;

  if(mod(ia + 1., 3.)>2.)
  {
    band = max(tube(p.xz) - rad2 - .01, p.y - .04);
    band = max(band, min(band + .005, -p.y + .015));
  }
  else
  {
    p = abs(p - .02);

    tb = max(tb, -min(tube(p.xy) - rad2 + .055, tube(p.yz) - rad2 + .055)); 
  }

  return vec4(tb, band, innerTb, ia);
}

vec4
torTubeTest (vec3 p)
{
  vec2 v = vec2(length(p.xy) - .5, p.z);
  return vec4(p, dot(v, v));
}

float
map (vec3 p)
{
  float rnd = fract(sin(dot(floor(p + vec3(111, 73, 27)), vec3(7.63, 157.31, 113.97)))*43758.5453);

  p = fract(p) - .5;

  if(rnd>.833) p = p.xzy;
  else if(rnd>.666) p = p.yxz;
  else if(rnd>.5) p = p.yzx;
  else if(rnd>.333) p = p.zxy;
  else if(rnd>.166) p = p.zyx;

  vec4 tb1 = torTubeTest(vec3(p.xy + .5, p.z));
  vec4 tb2 = torTubeTest(vec3(p.yz - .5, p.x));
  vec4 tb3 = torTubeTest(vec3(p.xz - vec2(.5, -.5), p.y));

  p = tb1.w<tb2.w && tb1.w<tb3.w ? tb1.xyz : tb2.w<tb3.w ? tb2.xyz : tb3.xyz;

  vec4 tb = torTube(p);

  gID = tb.w;

  vObjID = tb.xyz;

  return min(min(vObjID.x, vObjID.y), vObjID.z);
}

float
lightBlink (vec3 p, float gID)
{
  float rnd = fract(sin(dot(floor(p + vec3(111, 73, 27)), vec3(7.63, 157.31, 113.97)))*43758.5453);
  rnd = fract(rnd + gID*43758.54571);
  return smoothstep(0.33, .66, sin(rnd*6.283 + iTime*3.)*.5 + .5);
}

float
trace (vec3 o, vec3 r)
{
  float t = 0., d;

  for (int i = 0; i<128; i++)
  {
    d = map(o + r*t);

    if(abs(d)<.001*(t*.125 + 1.) || t>FAR)
      break;

    t += d; 
  }

  return min(t, FAR);
}

float
shadow (vec3 ro, vec3 lp, float k, float t)
{
  const int maxIterationsShad = 32; 

  vec3 rd = lp-ro;

  float shade = 1.;
  float dist = .001*(t*.125 + 1.);
  float end = max(length(rd), 0.0001);
  rd /= end;

  for (int i=0; i<maxIterationsShad; i++)
  {
    float h = map(ro + rd*dist);
    shade = min(shade, smoothstep(0.0, 1.0, k*h/dist));
    dist += clamp(h, .01, .2); 

    if (h<0.0 || dist > end)
      break;
  }

  return min(max(shade, 0.) + .0, 1.); 
}

float
cAO (vec3 pos, vec3 nor)
{
  float sca = 1., occ = 0.0;
  for( int i=0; i<5; i++ )
  {
    float hr = 0.01 + float(i)*0.35/4.0;
    float dd = map(nor * hr + pos);
    occ += (hr - dd)*sca;
    sca *= 0.7;
  }

  return clamp( 1.0 - occ, 0.0, 1.0 );
}

vec3
nrm (vec3 p, inout float edge, inout float crv, float t)
{
  vec2 e = vec2(1./mix(400., iResolution.y, .5)*(1. + t*.5), 0);

  float d1 = map(p + e.xyy), d2 = map(p - e.xyy);
  float d3 = map(p + e.yxy), d4 = map(p - e.yxy);
  float d5 = map(p + e.yyx), d6 = map(p - e.yyx);
  float d = map(p)*2.;

  edge = abs(d1 + d2 - d) + abs(d3 + d4 - d) + abs(d5 + d6 - d);
  edge = smoothstep(0., 1., sqrt(edge/e.x*2.));

  e = vec2(.002, 0);
  d1 = map(p + e.xyy), d2 = map(p - e.xyy);
  d3 = map(p + e.yxy), d4 = map(p - e.yxy);
  d5 = map(p + e.yyx), d6 = map(p - e.yyx);

  return normalize(vec3(d1 - d2, d3 - d4, d5 - d6));
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - iResolution.xy*.5)/iResolution.y;

  vec3 o = vec3(0., 0., iTime); 
  vec3 lp = vec3(-1, 3, -.25) + o;

  vec3 r = normalize(vec3(uv, 1));
  r.xz *= rot2(sin(iTime/2.) * 0.4);
  r.xy *= rot2(cos(iTime/2.) * 0.2);

  float t = trace(o, r);

  objID = (vObjID.x<vObjID.y && vObjID.x<vObjID.z) ? 0. : (vObjID.y<vObjID.z) ? 1. : 2.;

  float svGID = gID;

  vec3 sc = vec3(0);

  if(t<FAR)
  {
    vec3 sp = o + r*t;

    float edge = 0., crv = 1.;
    vec3 sn = nrm(sp, edge, crv, t);

    vec3 oCol = mix(vec3(1, .1, .3), vec3(1, .5, .1), dot(sin(sp*8. - cos(sp.yzx*4.)), vec3(.166)) + .5);
    oCol = mix(oCol, oCol.yzx, smoothstep(.3, 1., dot(sin(sp*4. + cos(sp.zxy*4. + iTime)), vec3(.166*.6)) + .3));

    if(objID<.5)
      oCol = mix(oCol, vec3(1), .97);
    else if(objID<1.5)
      oCol = mix(oCol, vec3(1), .05);
    else
    {
      oCol = mix(oCol, vec3(1), .05);

      oCol *= lightBlink(sp, svGID)*7.5 + .5;
    }

    float ao = cAO(sp, sn);
    float sh = shadow(sp + sn*.002, lp, 16., t); 

    vec3 ld = lp - sp;
    float dist = max(length(ld), 0.001);
    ld /= dist;

    float atten = 3.5/(1. + dist*0.05 + dist*dist*0.05);

    float diff = max(dot(ld, sn), 0.);

    sc = oCol*(diff + ao*.35);

    sc *= (1. - edge*.7)*atten*(sh + ao*.3)*ao;
  }

  float fog = 1./(1. + t*.125 + t*t*.05);
  sc = mix(vec3(0), sc, fog);//

  uv = gl_FragCoord.xy/iResolution.xy;
  sc = mix(pow(min(vec3(1.5, 1, 1).zyx*sc, 1.), vec3(1, 3, 16).zyx), sc,
           pow(16.*uv.x*uv.y*(1. - uv.x)*(1. - uv.y) , .125)*.75 + .25);

  gl_FragColor = vec4(sqrt(max(sc, 0.)), 1);
}
