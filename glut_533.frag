uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

#define PI 3.14159265
#define FAR 50.

// Frequencies and amplitudes of tunnel "A" and "B". See then "path" function.
const float freqA = 0.15;
const float freqB = 0.25;
const float ampA = 3.6;
const float ampB = .85;


// Standard 1x1 hash functions. Using "cos" for non-zero origin result.
float hash (float n) { return fract(cos(n)*45758.5453); }

// Non-standard vec3-to-vec3 hash function.
vec3
hash33 (vec3 p)
{
  float n = sin(dot(p, vec3(7, 157, 113)));    
  return fract(vec3(2097152, 262144, 32768)*n); 
}

// 2x2 matrix rotation.
mat2 rot2( float a ){ vec2 v = sin(vec2(1.570796, 0) + a);	return mat2(v, -v.y, v.x); }

vec3
tex3D (sampler2D t, vec3 p, vec3 n)
{
  n = max(abs(n) - .2, .001); // max(abs(n), 0.001), etc.
  n /= dot(n, vec3(1)); 
  //n /= length(n);
    
  vec3 tx = texture(t, p.yz).xyz;
  vec3 ty = texture(t, p.zx).xyz;
  vec3 tz = texture(t, p.xy).xyz;

  return mat3(tx*tx, ty*ty, tz*tz)*n;
}

float
noise3D (vec3 p)
{
  const vec3 s = vec3(7, 157, 113);

  vec3 ip = floor(p);

  vec4 h = vec4(0., s.yz, s.y + s.z) + dot(ip, s);

  p -= ip;

  p *= p*(3. - 2.*p);

  h = mix(fract(sin(mod(h, 6.2831589))*43758.5453), 
          fract(sin(mod(h + s.x, 6.2831589))*43758.5453), p.x);
  h.xy = mix(h.xz, h.yw, p.y);

  return mix(h.x, h.y, p.z); // Range: [0, 1].
}

float
drawSphere (vec3 p)
{
  p = fract(p)-.5;    
  return dot(p, p);

//p = abs(fract(p)-.5);
//return dot(p, vec3(.166));
}

float
cellTile (vec3 p)
{
  float c = .25;

  c = min(c, drawSphere(p - vec3(.81, .62, .53)));
  c = min(c, drawSphere(p - vec3(.39, .2, .11)));
    
  c = min(c, drawSphere(p - vec3(.62, .24, .06)));
  c = min(c, drawSphere(p - vec3(.2, .82, .64)));

  p *= 1.4142;

  c = min(c, drawSphere(p - vec3(.48, .29, .2)));
  c = min(c, drawSphere(p - vec3(.06, .87, .78)));

  //c = min(c, drawSphere(p - vec3(.6, .86, .0)));
  //c = min(c, drawSphere(p - vec3(.18, .44, .58)));

  return (c*4.); // Normalize.
}

float
cellTile2 (vec3 p)
{
    float c = .25;

    c = min(c, drawSphere(p - vec3(.81, .62, .53)));
    c = min(c, drawSphere(p - vec3(.39, .2, .11)));

    c = min(c, drawSphere(p - vec3(.62, .24, .06)));
    c = min(c, drawSphere(p - vec3(.2, .82, .64)));

    p *= 1.4142;

    c = min(c, drawSphere(p - vec3(.48, .29, .2)));
    c = min(c, drawSphere(p - vec3(.06, .87, .78)));

    c = min(c, drawSphere(p - vec3(.6, .86, .0)));
    c = min(c, drawSphere(p - vec3(.18, .44, .58)));

    return (c*4.);
}

vec2 path(in float z){ return vec2(ampA*sin(z * freqA), ampB*cos(z * freqB)); }

float
map (vec3 p)
{
  float sf = cellTile(p*.25);

  p.xy -= path(p.z);
  p.xy = rot2(p.z/12.)*p.xy;

  float n = dot(sin(p*1. + sin(p.yzx*.5 + iTime)), vec3(.25));

  return 2. - abs(p.y) + n + sf;

  //vec2 tun = p.xy - path(p.z);
  //return 3. - length(tun) - (0.5-surfFunc(p)) +  
  //       dot(sin(p*1. + sin(p.yzx*.5 + iTime)), vec3(.333))*.5+.5;
}

float
bumpSurf3D (vec3 p)
{
  float noi = noise3D(p*64.);
  float vor = cellTile2(p*.75);

  return vor*.98 + noi*.02;
}

vec3
doBumpMap (vec3 p, vec3 nor, float bumpfactor)
{
  const vec2 e = vec2(0.001, 0);
  float ref = bumpSurf3D(p);                 
  vec3 grad = (vec3(bumpSurf3D(p - e.xyy),
                    bumpSurf3D(p - e.yxy),
                    bumpSurf3D(p - e.yyx) )-ref)/e.x;                     

  grad -= nor*dot(nor, grad);          

  return normalize( nor + grad*bumpfactor );
}

float
trace (vec3 ro, vec3 rd)
{
  float t = 0.0, h;
  for(int i = 0; i < 80; i++)
  {
    h = map(ro+rd*t);
    if (abs (h) < 0.002 * (t * 0.25 + 1.0) || t > FAR)
      break;
    t += h * 0.8;
  }

  return clamp (t, 0., FAR);
}

vec3
getNormal (vec3 p)
{
  const vec2 e = vec2(.002, 0);
  return normalize(vec3(map(p + e.xyy) - map(p - e.xyy), map(p + e.yxy) - map(p - e.yxy),	
                          map(p + e.yyx) - map(p - e.yyx)));
}

float
thickness (vec3 p, vec3 n)
{
  float sNum = 4.;
  float sca = 1., occ = 0.;
  for (float i=0.; i<sNum + .001; i++)
  {
    float hr = .05 + .4*i/sNum; 
    //vec3 rn = normalize(n + RandomHemisphereDir(n, hr)*rad*.5);
    float dd = map(p - n*hr);
    occ += (hr - min(dd, 0.))*sca;
    sca *= .9;
  }
  return 1. - max(occ/sNum, 0.); 
}

float
calculateAO (vec3 p, vec3 n)
{
  float ao = 0.0, l;
  const float maxDist = 4.;
  const float nbIte = 6.0;
  //const float falloff = 0.9;
  for (float i=1.; i< nbIte+.5; i++)
  {
    l = (i + hash(i))*.5/nbIte*maxDist;
    ao += (l - map( p + n*l ))/(1.+ l);// / pow(1.+l, falloff);
  }

  return clamp(1.- ao/nbIte, 0., 1.);
}


float
curve (vec3 p, float w)
{
  vec2 e = vec2(-1., 1.)*w;
    
  float t1 = map(p + e.yxx), t2 = map(p + e.xxy);
  float t3 = map(p + e.xyx), t4 = map(p + e.yyy);
    
  return 0.125/(w*w) *(t1 + t2 + t3 + t4 - 4.*map(p));
}

float
trig3 (vec3 p)
{
  p = cos(p*2. + (cos(p.yzx) + 1. + iTime*4.)*1.57);
  return dot(p, vec3(.1666)) + .5;
}

float
trigNoise3D (vec3 p)
{
  const mat3 m3RotTheta = mat3(0.25, -0.866, 0.433,
                               0.9665, 0.25, -0.2455127, 
                               -0.058, 0.433, 0.899519)*1.5;

  float res = 0.;

  float t = trig3(p*PI);
  p += (t - iTime*0.25);
  p = m3RotTheta*p;
  //p = (p+0.7071)*1.5;
  res += t;

  t = trig3(p*PI); 
  p += (t - iTime*0.25)*0.7071;
  p = m3RotTheta*p;
//p = (p+0.7071)*1.5;
  res += t*0.7071;

  t = trig3(p*PI);
  res += t*0.5;

  return res/2.2071;
}

float
hash31 (vec3 p)
{
  return fract(sin(mod(dot(p, vec3(127.1, 311.7, 74.7)), 6.2831589))*43758.5453); 
}

float
getMist (vec3 ro, vec3 rd, vec3 lp, float t)
{
  float mist = 0.;
  ro += rd*t/8.; // Edge the ray a little forward to begin.

  for (int i = 0; i<4; i++)
  {
    float sDi = length(lp-ro)/FAR; 
    float sAtt = min(1./(1. + sDi*0.25 + sDi*sDi*0.05), 1.);
    mist += trigNoise3D(ro/2.)*sAtt;
    ro += rd*t/4.;
  }

  return clamp(mist/2. + hash31(ro)*.1 - .05, 0., 1.);
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - iResolution.xy*0.5)/iResolution.y;

  vec3 camPos = vec3(0, 0, iTime*6.);
  vec3 lookAt = camPos + vec3(0, 0, .1);

  vec3 light_pos = camPos + vec3(0, 1, 8);// Put it a bit in front of the camera.

  lookAt.xy += path(lookAt.z);
  camPos.xy += path(camPos.z);
  light_pos.xy += path(light_pos.z);

  float FOV = PI/2.; // FOV - Field of view.
  vec3 forward = normalize(lookAt-camPos);
  vec3 right = normalize(vec3(forward.z, 0., -forward.x )); 
  vec3 up = cross(forward, right);

  vec3 rd = normalize(uv.x*right + uv.y*up + forward/FOV);

  rd.xy = rot2( path(lookAt.z).x/16. )*rd.xy;

  float t = trace(camPos, rd);

  vec3 sceneCol = vec3(0);

  if (t < FAR)
  {
    vec3 sp = t * rd+camPos;
    vec3 sn = getNormal(sp);

    const float tSize0 = 1./4.; 

    sn = doBumpMap(sp, sn, .2);///(1.+t*.5/FAR)

    float ao = calculateAO(sp, sn);

    vec3 ld = light_pos-sp;

    float distlpsp = max(length(ld), 0.001);

    ld /= distlpsp;

    float atten = 1./(1. + distlpsp*0.3); // + distlpsp*distlpsp*0.025

    float ambience = 0.5;

    float diff = max( dot(sn, ld), 0.0);

    float spec = pow(max( dot( reflect(-ld, sn), -rd ), 0.0 ), 32.);

    float crv = clamp(curve(sp, 0.125)*0.5+0.5, .0, 1.);

    float fre = pow( clamp(dot(sn, rd) + 1., .0, 1.), 1.);

    vec3 ref = reflect(sn, rd);

    vec3 texCol = tex3D(iChannel0, sp*tSize0, sn);
    texCol = smoothstep(-.05, .95, texCol)*(smoothstep(-.5, 1., crv)*.75+.25);

    vec3 hf =  normalize(ld + sn);
    float th = thickness( sp, sn);
    float tdiff =  pow( clamp( dot(rd, -hf), 0., 1.), 1.);
    float trans = max((tdiff + .25)*th*1.5, 0.);  
    trans = pow(trans, 4.)*1.;        

    float shading = 1.;// crv*0.5+0.5; 

    sceneCol = texCol*(diff + ambience) + vec3(.7, .9, 1)*spec + 
                vec3(1, .6, .2)*spec*spec*spec*.5;
    sceneCol += texCol*vec3(.8, .95, 1)*pow(fre, 4.)*2.;
    sceneCol += texCol*mix(vec3(1, .05, 0), vec3(1, .05, .15), abs(hf.y))*trans*6.;

    sceneCol *= atten*shading*ao;
  }

  float mist = getMist(camPos, rd, light_pos, t);
  vec3 sky = vec3(2.5, 1.75, .875)* mix(1., .72, mist)*(rd.y*.25 + 1.);
  sceneCol = mix(sceneCol, sky, min(pow(t, 1.5)*.25/FAR, 1.));

  gl_FragColor = vec4(sqrt(clamp(sceneCol, 0., 1.)), 1.0);
}
