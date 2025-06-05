uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

// Arc shape - Circle: 0, Octagon: 1, Hexadecagon: 2
#define SHAPE 0

#define sqrt2 1.414213562373

float
hash12 (vec2 p)
{
	return fract(sin(dot(p ,vec2(12.9898, 78.233)))*43758.5453);
}
 
vec2
hash22 (vec2 p)
{
  return fract(sin(vec2(dot(p,vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3))))*43758.5453);
}

mat2 rot2 (float a){ float c = cos(a), s = sin(a); return mat2(c, -s, s, c); }

void
solveCircle (vec2 a, vec2 b, out vec2 o, out float r)
{
  vec2 m = a + b;
  o = dot(a, a)/dot(m, a)*m;
  r = length(o - a);
}

vec3
doArc (vec3 col, float w, float arc, vec2  p, float aNum, float r)
{
  vec3 tx = texture(iChannel0, p).xyz; tx *= tx;
     
  p = rot2(3.14159/aNum)*p;

  float shade = min(abs(arc)/.1*1.25, 1.);
  shade = clamp(-sin(shade*2.*6.283) + .25, 0., 1.);

  float ang = atan(p.y, p.x); 
  float ia = (floor(ang/6.2831*aNum) + .5)/aNum;
  p = rot2(ia*6.2831)*p;
  p.x -= r;

  p = abs(p);
  float d = max(p.x - .05, p.y + .005);
  //float d = clamp(cos(ang*aNum)*1. + .25, 0., 1.);

  d = max(arc + .045, -d);
  //d = length(p) - .05;

  vec3 arcCol = mix(tx*1.5, vec3(1, .9, .8), .75);

  col = mix(col, vec3(0), (1. - smoothstep(-w*8., w*8., arc - .04))*.5);
  col = mix(col, vec3(0), 1. - smoothstep(-w, w, arc - .015));
  col = mix(col, arcCol*shade*vec3(1.2, .8, .6)*vec3(1.5, .9, .6), 1. - smoothstep(-w, w, arc + .015));

  col = mix(col, vec3(0), (1. - smoothstep(-w*2., w*2., d - .01))*.5);
  col = mix(col, vec3(0), 1. - smoothstep(-w, w, d));
  col = mix(col, arcCol, 1. - smoothstep(-w, w, d + .02));

  return col;
}

float
dist (vec2 p, float sc)
{
#if SHAPE == 0
  return length(p);
#elif SHAPE == 1
  p = abs(p);
  float oct = max((p.y + p.x)/sqrt2, max(p.x, p.y));
  p *= rot2(3.14159/8.);
  float dec = max(oct, max((p.y + p.x)/sqrt2, max(p.x, p.y)));
  return sc<32.?  oct : dec;
#else
  p = abs(p);
  float d = max((p.y + p.x)/sqrt2, max(p.x, p.y));
  p *= rot2(3.14159/8.);
  return max(d, max((p.y + p.x)/sqrt2, max(p.x, p.y)));
#endif
}

vec3
renderArc (vec3 col, vec2 p, vec2 a, vec2 b, float aw, float sc)
{
  float w = 3./iResolution.y;

  vec2 o; float r;
  solveCircle(a, b, o, r);   

  float arc = dist(p - o, sc);
  arc = abs(arc - r) - aw; 

  col = doArc(col, w, arc, p - o, sc, r);

  return col;
}

void
main ()
{
  vec2 uv = gl_FragCoord.xy/min(iResolution.y, 650.);

  vec2 p = uv*4. + vec2(.5, .125)*iTime;   
  p = rot2(3.14159/8.)*p;
  vec2 oP = p;

  float w = 1./iResolution.y;

  const vec2 doBg = vec2(1, 1);

  vec3 tx = texture(iChannel0, oP/4. + .3).xyz; tx *= tx;
  vec3 bg = tx*vec3(2, 1.45, 1);

  vec3 col = bg;

  const float aw = .1;
  const float scL = 80.;
  const float scS = 16.;

  vec2 ip = floor(p);
  p -= ip + .5;

  float dia = abs(p.x) + abs(p.y) - (1. - sqrt2/2.);
  if(dia<.0)
  {
    if(doBg.x>.5)
    {
      vec3 dCol = bg*2.;
      float snD = clamp(-cos(dia*6.2831*16. + 3.14159/2.)*1. + .75, 0., 1.);
      dCol *= vec3(1, .8, .6)*snD;

      col = mix(col, vec3(0), 1. - smoothstep(-w, w, dia - .015));
      col = mix(col, dCol, 1. - smoothstep(-w, w, dia + .015));
    }

    if(hash12(ip + .59)>.5)
      p = p.yx;

    const float k = .5 - sqrt2/4.;

    col = renderArc(col, p, vec2(k, k), vec2(k, -k), aw, scS);
    col = renderArc(col, p, vec2(-k, k), vec2(-k, -k), aw, scS);
  }
  else
  {
    p = oP - .5;
    vec2 ip = floor(p);
    p -= ip + .5;

    float oct = max((abs(p.x) + abs(p.y))/sqrt2, max(abs(p.x), abs(p.y))) - .5;

    vec3 rnd3 = vec3(hash22(ip + vec2(37.73, 132.57)), hash12(ip)); 

    if(doBg.y>.5)
    {
      if(rnd3.x>.5 && rnd3.y>.5)
        col *= 1.25;

      float snD = clamp(cos(oct*6.2831*20. + 3.14159/2.)*2. + 1.5, 0., 1.);

      float octTor = abs(oct + .1666/2. - .015) - .1666/2. + .015;
      col = mix(col, vec3(0), (1. - smoothstep(-w*5., w*5., octTor - .03))*.5);
      col = mix(col, vec3(0), 1. - smoothstep(-w, w, octTor - .015));
      col = mix(col, bg/1.25*(snD*.75 + .25), 1. - smoothstep(-w, w, octTor + .015));

#if SHAPE == 0
      float shp = length(p) - .14;
#else
      //oct = min((abs(p.x) + abs(p.y))/sqrt2, max(abs(p.x), abs(p.y))) - .5;
      float shp = oct + .1666 + .2;//max(abs(p.x), abs(p.y)) - .15;// - (1. - sqrt2/2.);
#endif

      snD = clamp(-sin(shp*6.2831*20. + 3.14159/2.)*1. + .5, 0., 1.); // Concentric rings.

      col = mix(col, vec3(0), 1. - smoothstep(-w, w, shp - .065));
      col = mix(col, mix(bg*1.5, vec3(1, .9, .8), .65), (1. - smoothstep(-w*2., w*2., shp - .03)));
      col = mix(col, vec3(0), 1. - smoothstep(-w*2., w*2., shp - .015));
      col = mix(col, bg/1.*(snD*.95 + .05)*vec3(1.5, 2.125, 2.75), 1. - smoothstep(-w, w, shp + .015));
    }

    float iRnd = floor(rnd3.z*8.);
    p = rot2(3.14159/4.*iRnd)*p;

    vec2 a = vec2(0, .5);

    mat2 r1 = rot2(3.14159/4.), r2 = rot2(3.14159/2.), r3 = rot2(3.14159/4.*3.);

    if(rnd3.x>.5)
    {
      col = renderArc(col, p, a, r3*a, aw, scL);
      col = renderArc(col, p, r1*a, r2*a, aw, scS);
    }
    else
    {
      col = renderArc(col, p, a, r1*a, aw, scS);
      col = renderArc(col, p, r2*a, r3*a, aw, scS);
    }

    a = -a;

    if(rnd3.y>.5)
    {
      col = renderArc(col, p, a, r3*a, aw, scL);
      col = renderArc(col, p, r1*a, r2*a, aw, scS);
    }
    else
    {
      col = renderArc(col, p, a, r1*a, aw, scS);
      col = renderArc(col, p, r2*a, r3*a, aw, scS);
    }
  }

  col = mix(col.xzy, col, pow(uv.y, .25));

  uv = gl_FragCoord.xy/iResolution.xy;
  col *= pow(16.*uv.x*uv.y*(1. - uv.x)*(1. - uv.y) , .0625);

  gl_FragColor = vec4(sqrt(max(col, 0.)), 1);
}
