uniform vec2 iResolution;
uniform float iTime;

#define AA 1

const vec3 lig = normalize(vec3(-.1,.2,-1));

vec3
ACES (vec3 x)
{
    float a = 2.51;
    float b =  .03;
    float c = 2.43;
    float d =  .59;
    float e =  .14;
    return (x*(a*x+b))/(x*(c*x+d)+e);
}

mat2
rot (float a)
{
  float s = sin(a), c = cos(a);
  return mat2(c, -s, s, c);
}

float hash (float n) { return fract(sin(n)*43758.5453123); }

float
noise (vec3 x)
{
  vec3 p = floor(x);
  vec3 f = fract(x);
  f = f*f*(3.-2.*f);

  float n = p.x + p.y*157. + 113.*p.z;

  return mix(mix(mix(hash(n+  0.), hash(n+  1.),f.x),
                 mix(hash(n+157.), hash(n+158.),f.x),f.y),
             mix(mix(hash(n+113.), hash(n+114.),f.x),
                 mix(hash(n+270.), hash(n+271.),f.x),f.y),f.z);
}

float
fbm (vec3 p)
{
  float f = 0.;
  f += .5*noise(p);
  f += .25*noise(2.*p);
  f += .125*noise(4.*p);
  f += .0625*noise(8.*p);
  f += .03125*noise(16.*p);

  return f;
}

float
sdBox2D (vec2 p, vec2 s)
{
  vec2 q = abs(p) - s;
  return length(max(q,0.)) + min(max(q.x,q.y),0.);
}

float
building (vec3 p)
{
  float d = sdBox2D(p.xz,vec2(.1,.2))-.2;
  d = max(d,p.y+.6);
  d = min(d, abs(sdBox2D(p.xz,vec2(.08,.16))-.16+.02)-.02);
  d = max(d, p.y+.35);
  d = min(d, abs(sdBox2D(p.xz-vec2(.05,0),vec2(.0,.08)+.05)-.08+.02)-.02);
  d = max(d, p.y+.2);
  return d;
}

float
city (vec3 p)
{
  vec3 q = p-vec3(-3,-.4,-8);

  float s = 2.;
  q.xz = (fract(q.xz/s+.5)-.5)*s;

  float d = mod(floor(p.x/s)+round(p.z/s),2.)==0. ? 
            building(q-vec3(0,-.2,0)) : 
            building((q.zyx)*vec3(1,1.5,1.2));

  d = max(d, max(p.x+.5,abs(p.z)-5.5));
  d = max(d, 2.5-max(abs(p.x),abs(p.z+3.)));

  return d;
}

float
highBuilding (vec3 p)
{
  float d = sdBox2D(p.xz,vec2(.1))-.5;
  d = max(d, p.y+.3);
  d = min(d, sdBox2D(p.xz-.25,vec2(.1))-.3);
  d = max(d, p.y-.3);
  d = min(d, sdBox2D(p.xz+.25,vec2(.1))-.25);
  d = max(d, p.y-.9);
  d = min(d, sdBox2D(p.xz-.15,vec2(.1))-.2);
  d = max(d, p.y-1.5);
  d = min(d, sdBox2D(p.xz+.1,vec2(.1))-.15);
  d = max(d, p.y-2.1);
  d = min(d, sdBox2D(p.xz-.07,vec2(.1))-.1);
  d = max(d, p.y-2.7);
  d = min(d, sdBox2D(p.xz+.04,vec2(.1))-.05);
  d = max(d, p.y-3.3);
  d = min(d, sdBox2D(p.xz-.02,vec2(.1)));
  d = max(d, p.y-3.9);
  d = min(d, sdBox2D(p.xz+.01,vec2(.1))+.05);
  d = max(d, p.y-4.5);
  d = min(d, sdBox2D(p.xz-.005,vec2(.1))+.08);
  d = max(d, p.y-5.1);

  return d;
}

float
map (vec3 p)
{
  float d = building((p-vec3(1.3,-.4,0)).zyx*vec3(1,1.5,1.3));
  d = min(d, building((p-vec3(-3.5,.2,-1.8))*vec3(2,1.3,1))/2.);
  d = min(d, building((p-vec3(-3.,-.4,-.3)).zyx*vec3(1,1,1.4))/1.4);
  d = min(d, building((p-vec3(-6,-.4,-2.4))*vec3(-1,1.8,1))/1.4);
  d = min(d, city(p*vec3(-1,1,1)-vec3(-2,0,-3)));
  d = min(d, building((p-vec3(.4,.1,-10))*vec3(.6,1.8,1))/1.);
  d = min(d, building((p-vec3(-.6,-.55,-7)).zyx*vec3(1,1.3,1.3))/1.);
  d = min(d, building((p-vec3(8,0.4,-20))*.5));
  d = min(d, highBuilding(p-vec3(-8,0,-12)));

  return d;
}

float
intersect (vec3 ro, vec3 rd, float tmax)
{
  float t = 0.;
  for (int i=0; i<256 && t<tmax; i++)
  {
    vec3 p = ro + rd*t;

    float h = map(p);
    if (h<.001)
      break;

    t += h;
  }

  return t;
}

vec3
calcNormal (vec3 p)
{
  float h = map(p);
  const vec2 e = vec2(.0001,0);

  return normalize(h - vec3(map(p-e.xyy),
                            map(p-e.yxy),
                            map(p-e.yyx)));
}

float
shadow (vec3 ro, vec3 rd, float tmax, float k)
{
  float res = 1.;
  for (float t=0.; t<tmax;)
  {
    vec3 p = ro + rd*t;
    float h = map(p);
    if (h<.001)
      return 0.;
    res = min(res, k*h/t);
    t += h;
  }

  return res*res*(3.-2.*res);
}

float
calcAO (vec3 p, vec3 n, float k)
{
  return clamp(.5+.5*map(p+n*k)/k,0.,1.);
}

vec3
fog (float t)
{
  return 1.-exp(-t*t*.0005*vec3(1.3,1.3,1.7));
}

float
mapClouds (vec3 p)
{
  float f = 2.5*fbm(1.2*p+vec3(.5,.9,1)*.1*iTime);
  f = mix(f,f*.1,.4+.6*noise(.5*p+vec3(.5,.3,1)*.05*iTime));

  return 1.5*f - 2. - p.y;
}

vec4
renderClouds (vec3 bgCol, float depth, vec3 ro, vec3 rd)
{
  float tmin = (-.4-ro.y)/rd.y;
  float tmax = min((-3.-ro.y)/rd.y,depth);

  vec4 sum = vec4(0);
  if (tmin<0.)
    return sum;

  float t = tmin;
  t += .2*hash(iTime+gl_FragCoord.x*8315.9213/iResolution.x+gl_FragCoord.y*2942.5192/iResolution.y);

  for (int i=0; i<64; i++)
  {
    vec3 p = ro + rd*t;
    float h = mapClouds(p);

    if(h>0.)
    {
      float k = .2;
      float dif = clamp((h - mapClouds(p + k*lig))/k,0.,1.);

      float sha = shadow(p, lig, 16., 16.);

      float occ = exp(-h*4.);
      occ *= 1.-exp(-map(p)*16.);

      vec3 col = vec3(0);
      col += .4*vec3(.75,.75,1)*occ;
      float glare = .8+1.4*pow(dot(rd,lig),8.);
      col += vec3(1,.55,.3)*1.7*dif*sha*occ*glare;

      col = mix(col, bgCol, fog(t));

      sum += h*vec4(col,1)*(1.-sum.a);
    }
    t += t/64.;
    
    if(t>tmax || sum.a>.99)
      break;
  }

  return sum;
}

vec3
shade (vec3 bgCol, vec3 p, vec3 rd, float t)
{
  vec3 n = calcNormal(p);

  float dif = clamp(dot(n, lig),0.,1.);
  float bac = clamp(dot(n, -lig),0.,1.);
  float sha = shadow(p+n*.002, lig, 16., 16.);
  float occ = sqrt(calcAO(p, n, .1)*calcAO(p, n, .05));
  float spe = clamp(dot(reflect(rd,n), lig),0.,1.);

  vec3 mat = vec3(.8);
  float f = noise(16.*p+4.*fbm(32.*p));
  mat *= .8+.2*f;

  vec3 col = vec3(0);
  col += mat*2.*vec3(1,.7,.5)*dif*sha;
  col += mat*.11*vec3(.75,.75,1)*(.75+.25*bac)*occ;
  col += f*mat*4.*vec3(1,.7,.5)*pow(spe,12.)*sha*dif*occ;

  col = mix(col, bgCol, fog(t));

  return col;
}

vec3
render (vec3 ro, vec3 rd)
{
  vec3 col = .8*mix(vec3(.85,.7,.75),vec3(.12,.1,.23),clamp(1.-exp(-rd.y*7.),0.,1.));
  float sun = clamp(dot(rd,lig),0.,1.);

  col += vec3(1,.6,.3)*.001/(1.-pow(sun,4.));

  float tmax = 64.;
  float t = intersect(ro, rd, tmax);

  if (t<tmax)
  {
    vec3 p = ro + rd*t;
    col = mix(col, shade(col, p, rd, t), smoothstep(-3.,-2.,p.y));
  }

  vec4 res = renderClouds(col, min(t,tmax), ro, rd);
  col = col*(1.-res.a)+res.rgb;

  col += vec3(1,.4,.2)*pow(sun,14.);

  return clamp(col*.9-.02,0.,1.);
}

mat3
setCamera (vec3 ro, vec3 ta)
{
  vec3 w = normalize(ta - ro);
  vec3 u = normalize(cross(w, vec3(0,1,0)));
  vec3 v = cross(u, w);

  return mat3(u, v, w);
}

void
main ()
{
  vec3 tot = vec3(0);

  for (int m=0; m<AA; m++)
  for (int n=0; n<AA; n++)
  {
    vec2 off = vec2(m,n)/float(AA) - .5;

    vec2 p = (gl_FragCoord.xy+off - .5*iResolution) / iResolution.y;

    vec3 ro = vec3(.2+.2*sin(.4*iTime),.7,4);
    vec3 ta = vec3(0);
    mat3 ca = setCamera(ro, ta);

    vec3 rd = ca * normalize(vec3(p,1));

    vec3 col = render(ro, rd);
    tot += col;
  }
  tot /= float(AA*AA);

  tot = tot*.6+.4*ACES(tot);
  tot = pow(tot, vec3(.4545));
  tot = clamp(tot,0.,1.);
  tot = tot*.3+.7*tot*tot*(3.-2.*tot);

  tot = pow(tot,vec3(1,.9,.8))+vec3(.03,.03,0);

  vec2 q = gl_FragCoord.xy/iResolution;
  tot *= .5+.5*pow(16. * q.x*q.y*(1.-q.x)*(1.-q.y), .1);

  tot *= .97+.05*hash(q.x*13.+q.y*432.12);

  gl_FragColor = vec4(tot,1.0);
}
