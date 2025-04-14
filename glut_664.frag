uniform vec2 iResolution;
uniform float iTime;

#define TRANSPARENT
#define RANDOM_VARIATION

float
n2D (vec2 p)
{
  vec2 i = floor(p); p -= i; p *= p*(3. - p*2.);  
  return dot(mat2(fract(sin(vec4(0, 1, 113, 114) + dot(i, vec2(1, 113)))*43758.5453)) * vec2(1. - p.y, p.y), vec2(1. - p.x, p.x) );
}

float fbm (vec2 p) { return n2D(p)*.533 + n2D(p*2.)*.267 + n2D(p*4.)*.133 + n2D(p*8.)*.067; }

vec3
getLight (vec3 p, vec3 n, vec3 lp)
{
  vec3 ld = lp - p;
  float lDist = length(ld);
  ld /= lDist;
  float diff = max(dot(ld, n), 0.);
  float atten = 1.5/(1. + lDist*lDist);
  vec3 light = vec3(1)*(diff + .5)*atten;

  return light;
}

const float num = 21.;

void
main ()
{
  vec4 O;
  vec2 U = gl_FragCoord.xy;

  vec2 R = iResolution.xy;

  float yRes = min(R.y, 800.);

  U = (2.*U - R)/yRes;
  //U *= 1. + (dot(U, U) - .5)*.025;

  vec3 lp = vec3(0, 0, 1); // vec3(.3*cos(t), .2*sin(t), 1).
  vec3 n = vec3(0, 0, -1);

  vec3 bg = vec3(.5, .45, .4);
#ifdef TRANSPARENT
  bg /= 2.;
#endif

  vec3 light = getLight( vec3(U, 2.6), n,lp);
  bg *= light;

  float ns = fbm(U*5. + 17.3);
  ns = mix(ns, sin(ns*32. - cos(ns*34.)), .125);
  bg *= max(ns, 0.)*.4 + .8;

  float pat = clamp(sin((U.x - U.y)*min(R.y, 800.)/1.5)*1. + .5, 0., 1.);
  float rnd = fract(sin(dot(U, vec2(11.27, 112.43)))*43758.5453);
  if(rnd>.5) pat *= .6; 
  else pat *= 1.4;
  bg *= pat*.3 + .75;  

  vec3 col = bg;

  vec2 b = vec2(iResolution.x/iResolution.y, 1) - .1;
  vec2 q = abs(U);
  float bord = max(q.x - b.x, q.y - b.y);
  bord = max(bord, -(bord + .11));
  bord = max(bord, -min(q.x - b.x + .22, q.y - b.y + .22));
  //bord = max(bord, -(bord + .02));

  float falloff = 1./min(R.y, 800.);
  col = mix(col, vec3(0), (1. - smoothstep(0., falloff*12., bord ))*.35);
  col = mix(col, vec3(0), (1. - smoothstep(0., falloff, bord))*.7);
  col = mix(col, bg*2.2, (1. - smoothstep(0., falloff, bord + .01)));
  col = mix(col, vec3(0), (1. - smoothstep(0., falloff, bord + .035)));
  col = mix(col, bg*1.2, (1. - smoothstep(0., falloff, bord + .044)));   

  vec4 c[9];

  vec4 p;

  float t = iTime/4.;
  float a0 = atan(U.y, U.x) - t;

  for(int i=0; i<3; i++)
  {
    float a = a0;

    for(int j=0; j<3; j++)
    {
      float ia = mod(floor(a*num/6.283) + float(i - 1), num*3.);

      p.w = ia; 
      ia = (ia/num + .5/num)*6.283;

      float off = ia*5./3. + iTime;

      p.xy = (.55 - .25*sin(off))*vec2(cos(ia + t), sin(ia + t));

      p.z = 2. + cos(off - 3.14159/5.)*.35;

      c[i*3 + j] = p;

      a += 6.283; 
    }
  }

  for(int i=0; i<9; i++)
    for(int j=i + 1; j<9; j++)
    {
      if(c[i].z<c[j].z)
      {
        vec4 temp = c[i]; c[i] = c[j]; c[j] = temp;
      }
    }

  for(int i=0; i<3; i++)
    for (int j=0; j<3; j++)
    {
      p = c[i*3 + j];

      float sz = .175/p.z;
      float d = length(U - p.xy) - sz;

#ifdef RANDOM_VARIATION   
      float rnd = fract(sin(p.w + 37.)*43758.5453);
      if(rnd>.5) d = (abs(d + sz*.375) - sz*.375); 

#ifdef TRANSPARENT
      vec3 pCol = mod(p.w, 3.)==0.?  vec3(1.5, .9, .3) : vec3(1.5, .24, .52);
#else
      vec3 pCol = mod(p.w, 3.)==0.? vec3(.35) : vec3(1, .22, .45);
#endif
#else
      vec3 pCol = vec3(1, .75, .25);
#endif

      light = getLight(p.xyz, n, lp);

      falloff = .0005/sz;

#ifdef TRANSPARENT
      pCol = (col + .1)*pCol*light*3.;
      //pCol = mix(pCol*light, (col + .1)*pCol*light*3., smoothstep(-.1, .1, sin(iTime/4.)));            
#else
      pCol *= light;
#endif

      col = mix(col, vec3(0), (1. - smoothstep(0., falloff*10., d - .0035))*.5);
      col = mix(col, vec3(0), 1. - smoothstep(0., falloff, d));
      col = mix(col, pCol, 1. - smoothstep(0., falloff, d + .01));
      col = mix(col, vec3(0), 1. - smoothstep(0., falloff, d + sz - sz*.4));
      col = mix(col, vec3(light), 1. - smoothstep(0., falloff, d + sz - sz*.4 + .01));
      col = mix(col, vec3(0), 1. - smoothstep(0., falloff, d + sz - sz*.2 + .01));
    }

  col = mix(col.xzy, col, U.y*.3 + .65);

#ifndef RANDOM_VARIATION
#ifdef TRANSPARENT
  col = col.zyx;
#endif 
#endif 

  O = vec4(sqrt(max(col, 0.)), 1);

  gl_FragColor = O;
}
