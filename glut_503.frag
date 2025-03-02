#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

#define FAR 65.

const float freqA = .15/3.75;
const float freqB = .25/2.75;
const float ampA = 20.;
const float ampB = 4.;

mat2 rot2( float th ){ vec2 a = sin(vec2(1.5707963, 0) + th); return mat2(a, -a.y, a.x); }

float hash(float n){ return fract(cos(mod(n, 6.2831853))*45758.5453); }
float hash(vec3 p){ return fract(sin(mod(dot(p, vec3(7, 157, 113)), 6.2831853))*45758.5453); }

float getGrey (vec3 p) { return dot(p, vec3(0.299, 0.587, 0.114)); }

float
smaxP (float a, float b, float s)
{
  float h = clamp(.5 + .5*(a - b)/s, 0., 1.);
  return mix(b, a, h) + h*(1. - h)*s;
}

vec2 path (float z) { return vec2(ampA*sin(z * freqA), ampB*cos(z * freqB) + 3.*(sin(z*0.025)  - 1.)); }

float
map (vec3 p)
{
  float tx = textureLod (iChannel0, p.xz/16. + p.xy/80., 0.).x;

  vec3 q = p*.25;
  float h = dot(sin(q)*cos(q.yzx), vec3(.222)) + dot(sin(q*1.5)*cos(q.yzx*1.5), vec3(.111));

  float d = p.y + h*6.;

  q = sin(p*.5 + h);
  h = q.x*q.y*q.z;

  p.xy -= path(p.z);
  float tnl = 1.5 - length(p.xy*vec2(.33, .66)) + h + (1. - tx)*.25;

  return smaxP(d, tnl, 2.) - tx*.5 + tnl*.8; 
}


float logBisectTrace (vec3 ro, vec3 rd)
{
  float t = 0., told = 0., mid, dn;
  float d = map(rd*t + ro);
  float sgn = sign(d);

  for (int i = 0; i<96; i++)
  {
    if (sign(d) != sgn || d < 0.001 || t > FAR)
      break;

    told = t;

    t += step(d, 1.)*(log(abs(d) + 1.1) - d) + d;

    d = map(rd*t + ro);
  }

  if (sign (d) != sgn)
  {
    dn = sign(map(rd*told + ro));

    vec2 iv = vec2(told, t);

    for (int ii = 0; ii < 8; ii++)
    {
      mid = dot(iv, vec2(.5));
      float d = map(rd*mid + ro);
      if (abs(d) < 0.001)
        break;
      iv = mix(vec2(iv.x, mid), vec2(mid, iv.y), step(0.0, d*dn));
    }

    t = mid; 
  }

  return min(t, FAR);
}

vec3
normal (vec3 p)
{
  vec2 e = vec2 (-1, 1)*.001;
  return normalize(e.yxx*map(p + e.yxx) + e.xxy*map(p + e.xxy) +
                   e.xyx*map(p + e.xyx) + e.yyy*map(p + e.yyy));
}

vec3
tex3D (sampler2D tex, vec3 p, vec3 n)
{
  n = max (n * n, .001);
  n /= (n.x + n.y + n.z );  

  return (texture (tex, p.yz) * n.x + texture (tex, p.zx) * n.y + texture (tex, p.xy) * n.z).xyz;
}

vec3
doBumpMap (sampler2D tex, vec3 p, vec3 nor, float bumpfactor)
{
  const float eps = .001;
  vec3 grad = vec3 (getGrey (tex3D (tex, vec3 (p.x - eps, p.y, p.z), nor)),
                    getGrey (tex3D (tex, vec3 (p.x, p.y - eps, p.z), nor)),
                    getGrey (tex3D (tex, vec3 (p.x, p.y, p.z - eps), nor)));

  grad = (grad - getGrey(tex3D(tex, p, nor)))/eps; 

  grad -= nor*dot(nor, grad);

  return normalize(nor + grad*bumpfactor);
}

float
softShadow (vec3 ro, vec3 rd, float start, float end, float k)
{
  float shade = 1.;
  const int maxIterationsShad = 10; 

  float dist = start;
  float stepDist = end/float(maxIterationsShad);

  for (int i = 0; i < maxIterationsShad; i++)
  {
    float h = map(ro + rd*dist);
    //shade = min(shade, k*h/dist);
    shade = min(shade, smoothstep(0., 1., k*h/dist));

    dist += clamp(h, .2, stepDist*2.);

    if (abs (h) < .001 || dist > end)
      break;
  }

  return min(max(shade, 0.) + .1, 1.); 
}

float
calculateAO (vec3 p, vec3 n, float maxDist)
{
  float ao = 0., l;
  const float nbIte = 6.;
  //const float falloff = .9;
  for (float i = 1.; i < nbIte + .5; i++)
  {
    l = (i + hash(i))*.5/nbIte*maxDist;
    ao += (l - map( p + n*l))/(1. + l);// / pow(1.+l, falloff);
  }

  return clamp(1. - ao/nbIte, 0., 1.);
}

float
noise3D (vec3 p)
{
  const vec3 s = vec3(7, 157, 113);

  vec3 ip = floor(p);

  vec4 h = vec4(0., s.yz, s.y + s.z) + dot(ip, s);

  p -= ip;

  p = p*p*(3. - 2.*p);

  h = mix(fract(sin(mod(h, 6.2831853))*43758.5453), 
          fract(sin(mod(h + s.x, 6.2831853))*43758.5453), p.x);

  h.xy = mix(h.xz, h.yw, p.y);

  return mix(h.x, h.y, p.z);
}

float
fbm (in vec3 p)
{
  return .5333*noise3D(p) + .2667*noise3D(p*2.02) + .1333*noise3D(p*4.03) + .0667*noise3D(p*8.03);
}

vec3
getSky (vec3 ro, vec3 rd, vec3 sunDir)
{
  float sun = max(dot(rd, sunDir), 0.); // Sun strength.
  float horiz = pow(1.0-max(rd.y, 0.), 3.)*.35; // Horizon strength.
  
  vec3 col = mix(vec3(.25, .35, .5), vec3(.4, .375, .35), sun*.75);//.zyx;
  col = mix(col, vec3(1, .9, .7), horiz);

  col += .25*vec3(1, .7, .4)*pow(sun, 5.);
  col += .25*vec3(1, .8, .6)*pow(sun, 64.);
  col += .2*vec3(1, .9, .7)*max(pow(sun, 512.), .3);

  col = clamp(col + hash(rd)*.05 - .025, 0., 1.);

  vec3 sc = ro + rd*FAR*100.; sc.y *= 3.;

  return mix( col, vec3(1, .95, 1), .5*smoothstep(.5, 1., fbm(.001*sc)) * clamp(rd.y*4., 0., 1.) );
}

float
curve (vec3 p)
{
  const float eps = .05, amp = 4., ampInit = .5;

  vec2 e = vec2(-1, 1)*eps; // 0.05->3.5 - 0.04->5.5 - 0.03->10.->0.1->1.

  float t1 = map(p + e.yxx), t2 = map(p + e.xxy);
  float t3 = map(p + e.xyx), t4 = map(p + e.yyy);

  return clamp((t1 + t2 + t3 + t4 - 4.*map(p))*amp + ampInit, 0., 1.);
}

void
main ()
{
  vec2 u = (gl_FragCoord.xy - iResolution.xy*.5)/iResolution.y;

  vec3 lookAt = vec3(0, 0, iTime*8.);
  vec3 ro = lookAt + vec3(0, 0, -.25);

  lookAt.xy += path(lookAt.z);
  ro.xy += path(ro.z);

  float FOV = 3.14159/3.;
  vec3 forward = normalize(lookAt - ro);
  vec3 right = normalize(vec3(forward.z, 0, -forward.x )); 
  vec3 up = cross(forward, right);

  vec3 rd = normalize(forward + FOV*u.x*right + FOV*u.y*up);

  rd.xy = rot2( path(lookAt.z).x/64. )*rd.xy;

  vec3 lp = vec3(FAR*.5, FAR, FAR) + vec3(0, 0, ro.z);

  float t = logBisectTrace(ro, rd);

  vec3 sky = getSky(ro, rd, normalize(lp - ro));

  vec3 col = sky;

  if (t < FAR)
  {
    vec3 sp = ro+t*rd;
    vec3 sn = normal(sp);

    vec3 ld = lp-sp;
    ld /= max(length(ld), 0.001); // Normalize the light direct vector.

    const float tSize1 = 1./6.;

    sn = doBumpMap(iChannel1, sp*tSize1, sn, .007/(1. + t/FAR));//max(1.-length(fwidth(sn)), .001)*hash(sp)/(1.+t/FAR)

    float shd = softShadow(sp, ld, .05, FAR, 8.); // Shadows.
    float curv = curve(sp)*.9 +.1; // Surface curvature.
    float ao = calculateAO(sp, sn, 4.); // Ambient occlusion.

    float dif = max( dot( ld, sn ), 0.); // Diffuse term.
    float spe = pow(max( dot( reflect(-ld, sn), -rd ), 0. ), 5.); // Specular term.
    float fre = clamp(1.0 + dot(rd, sn), 0., 1.); // Fresnel reflection term.

    float Schlick = pow( 1. - max(dot(rd, normalize(rd + ld)), 0.), 5.);
    float fre2 = mix(.2, 1., Schlick);  //F0 = .2 - Hard clay... or close enough.

    float amb = fre*fre2 + .06*ao;

    col = clamp(mix(vec3(.8, .5, .3), vec3(.5, .25, .125),(sp.y+1.)*.15), vec3(.5, .25, .125), vec3(1));
        
    col =  smoothstep(-.5, 1., tex3D(iChannel1, sp*tSize1, sn))*(col + .25);
    col = clamp(col + noise3D(sp*48.)*.3 - .15, 0., 1.);

    col = pow(col, vec3(1.5));
        
    curv = smoothstep(0., .7, curv);
    col *= vec3(curv, curv*.95, curv*.85);

    col += getSky(sp, reflect(rd, sn), ld)*fre*fre2*.5;

    col = (col*(dif + .1) + fre2*spe)*shd*ao + amb*col;
  }

  col = mix(col, sky, sqrt(smoothstep(FAR - 15., FAR, t)));

  col = pow(max(col, 0.), vec3(.75));

  u = gl_FragCoord.xy/iResolution.xy;
  col *= pow(16.*u.x*u.y*(1. - u.x)*(1. - u.y) , .0625);

  gl_FragColor = vec4(clamp(col, 0., 1.), 1);
}
