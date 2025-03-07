uniform vec2 iResolution;
uniform float iTime;

//#define INTERLACING
//#define MAZE_ONLY

const vec2 s = vec2(1, 1.7320508);

float hash21 (vec2 p) { return fract(sin(dot(p, vec2(141.173, 289.927)))*43758.5453); }

mat2 r2(in float a){ float c = cos(a), s = sin(a); return mat2(c, -s, s, c); }

float
hex (vec2 p)
{
  p = abs(p);
    
  //return max(p.x*.5 + p.y*.866025, p.x); 
  return max(dot(p, s*.5), p.x);
}

vec4
getHex (vec2 p)
{
  vec4 hC = floor(vec4(p, p - vec2(.5, 1))/s.xyxy) + .5;

  vec4 h = vec4(p - hC.xy*s, p - (hC.zw + .5)*s);

  return dot(h.xy, h.xy)<dot(h.zw, h.zw) ? vec4(h.xy, hC.xy) : vec4(h.zw, hC.zw + vec2(.5, 1));
}

float
dots (vec2 p)
{
  p = abs(fract(p) - .5);
  return length(p); // Circles.
  //return (p.x + p.y)/1.5 + .035; // Diamonds.
  //return max(p.x, p.y) + .03; // Squares.
  //return max(p.x*.866025 + p.y*.5, p.y) + .01; // Hexagons.
  //return min((p.x + p.y)*.7071, max(p.x, p.y)) + .08; // Stars.
}

float
dfPol (vec2 p)
{
  return length(p); // Circular arc
  //return hex(p); // Hexagon
  //return max(hex(p), hex(r2(3.14159/6.)*p)); // Dodecahedron
  //return max(abs(p.x)*.866025 - p.y, p.y); // Triangle
}

float
df (vec2 p, float dir)
{
  vec2 uv = vec2(p.x + .1, p.y);

  return min(dots(uv*4.5), dots(uv*4.5 + .5)) - .3;
}

float
getPolarCoord (vec2 q, float dir)
{
  q = r2(iTime*dir)*q;
    
  const float aNum = 1.;
  float a = atan(q.y, q.x);
   
  return mod(a/3.14159, 2./aNum) - 1./aNum;
}

void
main ()
{
  float res = clamp(iResolution.y, 300., 600.); 

  vec2 u = (gl_FragCoord.xy - iResolution.xy*.5)/res;

  vec2 sc = u*4. + s.yx*iTime/12.;

  vec4 h = getHex(sc); // + s.yx*iTime/2.

  vec4 h2 = getHex(sc - 1./s);
  vec4 h3 = getHex(sc + 1./s);

  vec2 p = h.xy;

  float eDist = hex(p);
  float cDist = dot(p, p);

  float rnd = hash21(h.zw);
  //float aRnd = sin(rnd*6.283 + iTime*1.5)*.5 + .5;

#ifdef INTERLACING
  //vec3 lRnd = vec3(rnd*14.4 + .81, fract(rnd*21.3 + .97), fract(rnd*7.2 + .63));
  vec3 lRnd = vec3(hash21(h.zw + .23), hash21(h.zw + .96), hash21(h.zw + .47));
#endif

  //rnd = mod(h.z + h.w, 2.);

  float dir = 1.;

  vec2 q;

  if(rnd>.5) p.y = -p.y;

  const float r = 1.;
  const float th = .2;

  q = p - vec2(0, r)/s;
  vec3 da = vec3(q, dfPol(q));

  q = r2(3.14159*2./3.)*p - vec2(0, r)/s;
  vec3 db = vec3(q, dfPol(q));

  q = r2(3.14159*4./3.)*p - vec2(0, r)/s;
  vec3 dc = vec3(q, dfPol(q));

  vec3 q3 = da.z<db.z && da.z<dc.z? da : db.z<dc.z ? db : dc;

  q3.z -= .57735/2. + th/2.;  // Circular and dodecahedral arc/curves.
  //q3.z -= .5/2. + th/2.;  // Hexagon curve.
  //q3.z -= .7071/2. + th/2.;  // Triangle curve.

  q3.z = max(q3.z, -th - q3.z);

  float d = q3.z;

#ifdef MAZE_ONLY
  d += 1e5;
#endif

  float dBord = max(d - .015, -d);

  q = q3.xy;
  const float lnTh = .05;
  q = abs(q);

  float arcBord = hex(q);
  //float arcBord = length(q);
  //float arcBord = max(hex(q), hex(r2(3.14159/6.)*q));

  float lnOuter = max(arcBord - .5, -(arcBord - .5 + lnTh)); //.57735

#ifdef INTERLACING
  float ln = min(lnOuter, (q.y*.866025 + q.x*.5, q.x) - lnTh);
#else
  float ln = min(lnOuter, arcBord - lnTh);
#endif
  float lnBord = ln - .03;

  float a = getPolarCoord(q3.xy, dir);
  float d2 = df(vec2(q3.z, a), dir); 

  float dMask = smoothstep(0., .015, d);

  vec3 bg =  mix(vec3(0, .4, .6), vec3(0, .3, .7), dot(sin(u*6. - cos(u*3.)), vec2(.4/2.)) + .4); 
  bg = mix(bg, bg.xzy, dot(sin(u*6. - cos(u*3.)), vec2(.4/2.)) + .4);
  bg = mix(bg, bg.zxy, dot(sin(u*3. + cos(u*3.)), vec2(.1/2.)) + .1);

#ifdef INTERLACING
  float hLines = smoothstep(0., .02, eDist - .5 + .02);
  bg = mix(bg, vec3(0), smoothstep(0., .02, ln)*dMask*hLines);
#endif

  const float tr = 1.;

  float eDist2 = hex(h2.xy);
  float hLines2 = smoothstep(0., .02, eDist2 - .5 + .02);
#ifdef INTERLACING
  if(rnd>.5 && lRnd.x<.5) hLines2 *= smoothstep(0., .02, ln);
  if(lRnd.x>.5) hLines2 *= dMask;
#else
  if(rnd>.5) hLines2 *= smoothstep(0., .02, ln);
  hLines2 *= dMask;
#endif
  bg = mix(bg, vec3(0), hLines2*tr);

  float eDist3 = hex(h3.xy);
  float hLines3 = smoothstep(0., .02, eDist3 - .5 + .02);
#ifdef INTERLACING
  if(rnd<=.5 && lRnd.x>.5) hLines3 *= smoothstep(0., .02, ln);
  if(lRnd.x>.5) hLines3 *= dMask;
#else
  if(rnd<=.5) hLines3 *= smoothstep(0., .02, ln);
  hLines3 *= dMask;
#endif
  bg = mix(bg, vec3(0), hLines3*tr);

  float shade = max(1.25 - dot(h2.xy, h2.xy)*2., 0.);
  shade = min(shade, max(dot(h3.xy, h3.xy)*3. + .25, 0.));
  bg = mix(bg, vec3(0), (1.-shade)*.5); 

  vec3 dotCol = bg.zyx*vec3(1.5, .4, .4);
  vec3 bCol = mix(bg.zyx, bg.yyy, .25);
  bg = mix(bg.yyy, bg.zyx, .25);

#ifdef INTERLACING
  if(lRnd.x>.5){
      bg = mix(bg, vec3(0), (1. - smoothstep(0., .015, lnBord)));
      bg = mix(bg, bCol, (1. - smoothstep(0., .015, ln))); 
      // Center lines.
      bg = mix(bg, vec3(0), smoothstep(0., .02, eDist3 - .5 + .02)*tr);
  }
#else
  bg = mix(bg, vec3(0), (1. - smoothstep(0., .015, lnBord)));
  bg = mix(bg, bCol, (1. - smoothstep(0., .015, ln)));
#endif

  bg = mix(bg, vec3(0), (1. - smoothstep(0., .07, d))*.5);

  //vec3 col = mix(bg, vec3(1)*max(-d*3. + .7, 0.), (1. - dMask)*.65);
  vec3 col = mix(bg, vec3(1)*max(-d*9. + .4, 0.), (1. - dMask)*.65);

  //dotCol = mix(dotCol, dotCol.xzy, dot(sin(u*3.14159*2. - cos(u.yx*3.14159*2.)*3.14159), vec2(.25)) + .5);
  col = mix(col, vec3(0), (1. - dMask)*(1. - smoothstep(0., .02, d2)));
  col = mix(col, dotCol, (1. - dMask)*(1. - smoothstep(0., .02, d2 + .125)));

  col = mix(col, vec3(0), 1. - smoothstep(0., .015, dBord));

#ifdef INTERLACING
  if(lRnd.x<=.5){
      col = mix(col, vec3(0), (1. - smoothstep(0., .015, lnBord)));
      col = mix(col, bCol, (1. - smoothstep(0., .015, ln)));  
      // Center lines.
      col = mix(col, vec3(0), smoothstep(0., .02, eDist2 - .5 + .02)*tr);
  }
#endif

  //if(rnd>.5) h3.y = -h3.y;
#ifdef INTERLACING
  float trSn = max(dMask, 1. - smoothstep(0., .015, lnBord))*.75 + .25;
#else
  float trSn = dMask*.75 + .25;
#endif
  col = mix(col, vec3(0), trSn*(1. - hex(s/2.+h2.xy)));
  col = mix(col, vec3(0), trSn*(1. - hex(s/2.-h3.xy)));

  //if (rnd>.5) h.xy = -h.yx;
  //float cont = clamp(cos(hex(h.xy)*6.283*12.)*1.5 + 1.25, 0., 1.);
  //col = mix(col, vec3(0), (1. - smoothstep(0., .015, ln))*(smoothstep(0., .015, d))*(1.-cont)*.5);

  float gr = dot(col, vec3(.299, .587, .114));
  float hatch = (gr<.45)? clamp(sin((sc.x - sc.y)*3.14159*40.)*2. + 1.5, 0., 1.) : 1.;
  float hatch2 = (gr<.25)? clamp(sin((sc.x + sc.y)*3.14159*40.)*2. + 1.5, 0., 1.) : 1.;

  col *= min(hatch, hatch2)*.5 + .5;    
  col *= clamp(sin((sc.x - sc.y)*3.14159*80.)*1.5 + .75, 0., 1.)*.25 + 1.;

  u = gl_FragCoord.xy/iResolution.xy;
  col *= pow(16.*u.x*u.y*(1. - u.x)*(1. - u.y) , .125) + .25;
  //col = mix(pow(min(vec3(1.5, 1, 1)*col, 1.), vec3(1, 3, 16)), col,
          //pow(16.*u.x*u.y*(1. - u.x)*(1. - u.y) , .25)*.75 + .25);

  gl_FragColor = vec4(sqrt(max(col, 0.)), 1);
}
