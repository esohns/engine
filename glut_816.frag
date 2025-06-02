uniform vec2 iResolution;
uniform float iTime;

#define OFFSET_ROW

mat2 rot2 (float a) { float c = cos(a), s = sin(a); return mat2(c, -s, s, c); }

float hash21 (vec2 p) { return fract(sin(dot(p, vec2(27.619, 57.583)))*43758.5453); }

vec2 cellID;

vec2
hash22B (vec2 p)
{
  float n = sin(dot(p, vec2(41, 289)));
  return fract(vec2(262144, 32768)*n)*2. - 1.; 
}

float
n2D3G (vec2 p)
{
  vec2 i = floor(p); p -= i;

  vec4 v;
  v.x = dot(hash22B(i), p);
  v.y = dot(hash22B(i + vec2(1, 0)), p - vec2(1, 0));
  v.z = dot(hash22B(i + vec2(0, 1)), p - vec2(0, 1));
  v.w = dot(hash22B(i + 1.), p - 1.);

  p = p*p*(3. - 2.*p);

  return mix(mix(v.x, v.y, p.x), mix(v.z, v.w, p.x), p.y);
}

float
fBm (vec2 p)
{
  return n2D3G(p)*.66 + n2D3G(p*2.)*.34;
}

float
bMap (vec2 p)
{
  p *= rot2(-3.14159/5.);

#ifdef OFFSET_ROW
  if(mod(floor(p.y), 2.)<.5)
    p.x += .5;
#endif

  vec2 ip = floor(p);
  p -= ip + .5;

  cellID = ip;

  float ang = -3.14159*3./5. + (fBm(ip/8. + iTime/3.))*6.2831*2.;
  vec2 offs = vec2(cos(ang), sin(ang))*.35;

  if(p.x<offs.x)
    p.x = 1. - (p.x + .5)/abs(offs.x  + .5);
  else
  p.x = (p.x - offs.x)/(.5 - offs.x);

  if(p.y<offs.y)
    p.y = 1. - (p.y + .5)/abs(offs.y + .5);
  else
    p.y = (p.y - offs.y)/(.5 - offs.y);

  return 1. - max(p.x, p.y);
}

vec3
doBumpMap (vec2 p, vec3 n, float bumpfactor, inout float edge)
{
  vec2 e = vec2(.025, 0);

  float f = bMap(p);
  float fx = bMap(p - e.xy);
  float fy = bMap(p - e.yx);
  float fx2 = bMap(p + e.xy);
  float fy2 = bMap(p + e.yx);

  vec3 grad = (vec3(fx - fx2, fy - fx2, 0))/e.x/2.;   

  edge = length(vec2(fx, fy) + vec2(fx2, fy2) - f*2.);

  edge = smoothstep(0., 1., edge/e.x);

  grad -= n*dot(n, grad);          

  return normalize( n + grad*bumpfactor );
}

float
doHatch (vec2 p, float res)
{
  p *= res/16.;

  float hatch = clamp(sin((p.x - p.y)*3.14159*200.)*2. + .5, 0., 1.);

  float hRnd = hash21(floor(p*6.) + .73);
  if(hRnd>.66)
    hatch = hRnd;

  return hatch;
}

void
main ()
{
//  float iRes = min(iResolution.y, 800.);
  float iRes = iResolution.y;
  vec2 uv = (gl_FragCoord.xy - iResolution.xy*.5)/iRes;

  vec3 rd = normalize(vec3(uv, .5));

  const float gSc = 10.;
  vec2 p = uv*gSc + vec2(0, iTime/2.);
  vec2 oP = p;

  float m = bMap(p);

  vec2 svID = cellID;

  vec3 n = vec3(0, 0, -1);

  float edge = 0., bumpFactor = .25;
  n = doBumpMap(p, n, bumpFactor, edge);

  vec3 lp =  vec3(-0. + sin(iTime)*.3, .0 + cos(iTime*1.3)*.3, -1) - vec3(uv, 0);

  float lDist = max(length(lp), .001);
  vec3 ld = lp/lDist;

  float diff = max(dot(n, ld), 0.);
  diff = pow(diff, 4.);
  float spec = pow(max(dot(reflect(-ld, n), -rd), 0.), 16.);
  float fre = min(pow(max(1. + dot(n, rd), 0.), 4.), 3.);

  vec3 col = vec3(.15)*(diff + .251 + spec*vec3(1, .7, .3)*9. + fre*vec3(.1, .3, 1)*12.);

  float rf = smoothstep(0., .35, bMap(reflect(rd, n).xy*2.)*fBm(reflect(rd, n).xy*3.) + .1);
  col += col*col*rf*rf*vec3(1, .1, .1)*15.;

  //float rnd = hash21(svID);
  //float rnd2 = hash21(svID + .7);
  //rnd = sin(rnd*6.2831 + iTime*1.);
  //col *= mix(vec3(1), (.5 + .4*cos(6.2831*rnd2 + vec3(0, 1, 2)))*6., smoothstep(.96, .99, rnd));

  float shade = m*.83 + .17;
  col *= shade;

  col *= 1. - edge*.8;

  float hatch = doHatch(oP/gSc, iRes);
  col *= hatch*.5 + .7;

  gl_FragColor = vec4(sqrt(max(col, 0.)), 1.);
}
