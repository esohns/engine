uniform vec2 iResolution;
uniform float iTime;

//#define SHOW_GRID
#define sstep(sf, d) (1. - smoothstep(0., sf, d))

mat2 rot2 (float a) { float c = cos(a), s = sin(a); return mat2(c, -s, s, c); }

float
hash21 (vec2 p)
{
  float n = dot(p, vec2(127.183, 157.927));
  return fract(sin(n)*43758.5453);
}

float
n2D (vec2 p)
{
  vec2 i = floor(p);
  p -= i;
  p *= p*(3. - p*2.);

  return dot(mat2(fract(sin(vec4(0, 1, 113, 114) + dot(i, vec2(1, 113)))*43758.5453)) * vec2(1. - p.y, p.y), vec2(1. - p.x, p.x));
}

float fbm (vec2 p) { return n2D(p)*.533 + n2D(p*2.)*.267 + n2D(p*4.)*.133 + n2D(p*8.)*.067; }

float
TilePattern (vec2 p)
{
  vec2 ip = floor(p);
  p -= ip + .5;

  float rnd = hash21(ip);
  float rnd2 = hash21(ip + 27.93);

  float iRnd = floor(rnd*4.);
  p = rot2(iRnd*3.14159/2.)*p;

  float d = 1e5, d1 = 1e5, d2 = 1e5, d3 = 1e5, l;

  l = length(p - vec2(-.5, .5));
  d1 = abs(l - .25);
  d2 = abs(l - .5);
  d3 = abs(l - .75);
  if(rnd2>.33)
    d3 = abs(length(p - vec2(.125, .5)) - .125);

  d = min(min(d1, d2), d3);

  d1 = 1e5;
  d2 = abs(length(p - vec2(.5, .125)) - .125);
  d3 = abs(length(p - vec2(.5, -.5)) - .25);
  d = min(d, min(d1, min(d2, d3))); 

  l = length(p + .5);
  d = max(d, -(l - .75));

  d1 = abs(l - .5);
  d1 = min(d1, abs(d1 - .25));
  d = min(d, d1);

  d -= .0625;

  return d; 
}

float
sFract (float x, float sf)
{
  x = fract(x);
  return min(x, (1. - x)*x*sf);
}

vec3
GrungeTex (vec2 p)
{
  float c = n2D(p*3.)*.57 + n2D(p*7.)*.28 + n2D(p*15.)*.15;

  vec3 col = mix(vec3(.25, .1, .02), vec3(.35, .5, .65), c);
  col *= n2D(p*vec2(150., 350.))*.5 + .5; 

  col = mix(col, col*vec3(.75, .95, 1.2), sFract(c*4., 12.));
  col = mix(col, col*vec3(1.2, 1, .8)*.8, sFract(c*5. + .35, 12.)*.5);

  c = n2D(p*8. + .5)*.7 + n2D(p*18. + .5)*.3;
  c = c*.7 + sFract(c*5., 16.)*.3;
  col = mix(col*.6, col*1.4, c);

  return clamp(col, 0., 1.);
}

void
main ()
{
  float res = min(iResolution.y, 750.);
  vec2 uv = (gl_FragCoord.xy - iResolution.xy*.5)/res;

  vec2 p = uv*4. + vec2(1, 0)*iTime;

  vec2 eps = vec2(4, 6)/iResolution.y;
  float d = TilePattern(p); // Initial field value.
  float d2 = TilePattern(p + eps); // Slight sample distance, for highlighting,.
  float dS = TilePattern(p + eps*3.); // Larger distance, for shadows.

  float b = smoothstep(0., 15./450., d - .015);
  float b2 = smoothstep(0., 15./450., d2 - .015);

  float bump = max(b2 - b, 0.)/length(eps);
  float bump2 = max(b - b2, 0.)/length(eps);

  float sf = 5./iResolution.y;

  vec3 tx = GrungeTex(p/4. + .5);
  tx = smoothstep(0., .5, tx);

  vec3 bg = tx*vec3(.85, .68, .51);

  vec3 col = bg;

#ifdef SHOW_GRID
  vec2 q = abs(fract(p) - .5);
  float gw = .0275;
  float grid = (max(q.x, q.y) - .5 + gw);
  col = mix(col, vec3(0), (smoothstep(0., sf*4., grid - gw + gw*2.))*.75);
  col = mix(col, bg*2., (smoothstep(0., sf, grid - gw + gw/2.)));
#endif

  col = mix(col, vec3(0), sstep(sf*4., dS - .02)*.75); // Shadow.

  col = mix(col, vec3(0), sstep(sf*8., d)*.35);

  col = mix(col, vec3(0), sstep(sf, d));

  vec3 pCol = vec3(2.5, .75, .25)*tx;

  col = mix(col, pCol, sstep(sf, d + .025));

  col = mix(col, col.xzy, smoothstep(.3, 1., fbm(p*.85))*.7);

  col = col + (vec3(1, .2, .1)*(bump*.01 + bump*bump*.003));
  col = col + col*(vec3(1, .2, .1).zyx*(bump2*.01 + bump2*bump2*.003));   

  //col = tx;

  gl_FragColor = vec4(sqrt(max(col, 0.)), 1);
}
