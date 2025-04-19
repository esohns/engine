#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

#define FAR 60.

mat2 rot2 (float a) { vec2 v = sin(vec2(1.570796, 0) + a);	return mat2(v, -v.y, v.x); }

vec3
tex3D (sampler2D tex, vec3 p, vec3 n)
{
  n = max((abs(n) - 0.2)*7., 0.001); // n = max(abs(n), 0.001);// etc.
  n /= (n.x + n.y + n.z ); 
  p = (texture(tex, p.yz)*n.x + texture(tex, p.zx)*n.y + texture(tex, p.xy)*n.z).xyz;
  return p*p;
}

vec2
path (float z)
{
  //return vec2(0); // Straight.
  float a = sin(z * 0.11);
  float b = cos(z * 0.14);
  return vec2(a*4. -b*1.5, b*1.7 + a*1.5); 
  //return vec2(a*4. -b*1.5, 0.); // Just X.
  //return vec2(0, b*1.7 + a*1.5); // Just Y.
}

float
n3D (vec3 p)
{
  const vec3 s = vec3(7, 157, 113);
  vec3 ip = floor(p); p -= ip; 
  vec4 h = vec4(0., s.yz, s.y + s.z) + dot(ip, s);
  p = p*p*(3. - 2.*p); //p *= p*p*(p*(p * 6. - 15.) + 10.);
  h = mix(fract(sin(h)*43758.5453), fract(sin(h + s.x)*43758.5453), p.x);
  h.xy = mix(h.xz, h.yw, p.y);
  return mix(h.x, h.y, p.z); // Range: [0, 1].
}

float
map (vec3 p)
{
  p.xy -= path(p.z);

  p = cos(p*.1575 + sin(p.zxy*.4375));

  float n = dot(p, p); 

  p = sin(p*3. + cos(p.yzx*3.));

  return n - p.x*p.y*p.z*.35 - .9;
}

const float w2h = 1.;
const float mortW = .05;

float
brickShade (vec2 p)
{
  p = fract(p);
  return pow(16.*p.x*p.y*(1.-p.x)*(1.-p.y), 0.25);
}

float
brickMorter (vec2 p)
{
  p.x -= .5;

  p = abs(fract(p + vec2(0, .5)) - .5)*2.;

  return smoothstep(0., mortW, p.x)*smoothstep(0., mortW*w2h, p.y);
}

float
brick (vec2 p)
{
  p = fract(p*vec2(0.5/w2h, 0.5))*2.;

  return brickMorter(p)*(brickShade(p)*.5 + .5);
}

float
bumpSurf (vec3 p, vec3 n)
{
  n = abs(n);

  if (n.x>0.5) p.xy = p.zy;
  else if (n.y>0.5) p.xy = p.zx;

  return brick(p.xy);
}

vec3
doBumpMap (vec3 p, vec3 nor, float bumpfactor)
{
  const vec2 e = vec2(0.001, 0);
  float ref = bumpSurf(p, nor);
  vec3 grad = (vec3(bumpSurf(p - e.xyy, nor),
                    bumpSurf(p - e.yxy, nor),
                    bumpSurf(p - e.yyx, nor) )-ref)/e.x;

  grad -= nor*dot(nor, grad);

  return normalize( nor + grad*bumpfactor );
}

vec3
doBumpMap (sampler2D tx, vec3 p, vec3 n, float bf)
{
  const vec2 e = vec2(0.001, 0);

  mat3 m = mat3( tex3D(tx, p - e.xyy, n), tex3D(tx, p - e.yxy, n), tex3D(tx, p - e.yyx, n));

  vec3 g = vec3(0.299, 0.587, 0.114)*m;
  g = (g - dot(tex3D(tx,  p , n), vec3(0.299, 0.587, 0.114)) )/e.x; g -= n*dot(n, g);

  return normalize( n + g*bf );
}

vec3
voxelTrace (vec3 ro, vec3 rd, out vec3 mask)
{
  vec3 p = floor(ro) + .5;

  vec3 dRd = 1./abs(rd); // 1./max(abs(rd), vec3(.0001));
  rd = sign(rd);
  vec3 side = dRd*(rd * (p - ro) + 0.5);

  mask = vec3(0);

  for (int i = 0; i < 80; i++)
  {
    if (map(p)<0.)
      break;

    mask = step(side, side.yzx)*(1.-step(side.zxy, side));
    side += mask*dRd;
    p += mask * rd;
  }

  return p;
}

float
voxShadow (vec3 ro, vec3 rd, float end)
{
  float shade = 1.0;
  vec3 p = floor(ro) + .5;

  vec3 dRd = 1./abs(rd);//1./max(abs(rd), vec3(.0001));
  rd = sign(rd);
  vec3 side = dRd*(rd * (p - ro) + 0.5);

  vec3 mask = vec3(0);

  float d = 1.;

  for (int i = 0; i < 16; i++)
  {
    d = map(p);

    if (d<0. || length(p-ro)>end)
      break;

    mask = step(side, side.yzx)*(1.-step(side.zxy, side));
    side += mask*dRd;
    p += mask * rd;
  }

  return shade = step(0., d)*.7 + .3;
}

vec4
voxelAO (vec3 p, vec3 d1, vec3 d2)
{
  vec4 side = vec4(map(p + d1), map(p + d2), map(p - d1), map(p - d2));
  vec4 corner = vec4(map(p + d1 + d2), map(p - d1 + d2), map(p - d1 - d2), map(p + d1 - d2));

  side = step(side, vec4(0));
  corner = step(corner, vec4(0));

  return 1. - (side + side.yzwx + max(corner, side*side.yzwx))/3.;    
}

float
calcVoxAO (vec3 vp, vec3 sp, vec3 rd, vec3 mask)
{
  vec4 vAO = voxelAO(vp - sign(rd)*mask, mask.zxy, mask.yzx);

  sp = fract(sp);
  vec2 uv = sp.yz*mask.x + sp.zx*mask.y + sp.xy*mask.z;
  return mix(mix(vAO.z, vAO.w, uv.x), mix(vAO.y, vAO.x, uv.x), uv.y);
}

float
thickness (vec3 p, vec3 n, float maxDist, float falloff)
{
  const float nbIte = 6.0;
  float ao = 0.0;

  for( float i=1.; i< nbIte+.5; i++ )
  {
    float l = (i*.75 + fract(cos(i)*45758.5453)*.25)/nbIte*maxDist;

    ao += (l + map( p -n*l )) / pow(1. + l, falloff);
  }

  return clamp( 1.-ao/nbIte, 0., 1.);
}

vec3
envMap (vec3 rd, vec3 sn)
{
  float c = n3D(rd*3.)*.66 + n3D(rd*6.)*.34;
  c = smoothstep(0.4, 1., c);
  return vec3(min(c*1.5, 1.), pow(c, 3.), pow(c, 16.));
  //vec3 col = tex3D(iChannel1, rd/4., sn);
  //return smoothstep(.0, 1., col*col*2.);//col*col*2.;
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - iResolution.xy*.5)/iResolution.y;

  vec3 camPos = vec3(0, 0, iTime*8.);
  vec3 lookAt = camPos + vec3(0, 0, .25);

  vec3 lightPos = camPos + vec3(0, 1, 5);

  lookAt.xy += path(lookAt.z);
  camPos.xy += path(camPos.z);
  lightPos.xy += path(lightPos.z);

  float FOV = 3.14159265/3.; // FOV - Field of view.
  vec3 forward = normalize(lookAt-camPos);
  vec3 right = normalize(vec3(forward.z, 0, -forward.x )); 
  vec3 up = cross(forward, right);

  vec3 rd = normalize(forward + FOV*uv.x*right + FOV*uv.y*up);
  //rd = normalize(vec3(rd.xy, rd.z - dot(rd.xy, rd.xy)*.25));    

  rd.xy = rot2( path(lookAt.z).x/24. )*rd.xy;

  vec3 mask;
  vec3 vPos = voxelTrace(camPos, rd, mask);

  vec3 tCube = (vPos-camPos - .5*sign(rd))/rd;
  float t = max(max(tCube.x, tCube.y), tCube.z);

  vec3 sceneCol = vec3(0);

  if(t<FAR)
  {
    vec3 sp = camPos + rd*t;

    vec3 sn = -(mask * sign( rd ));

    vec3 snNoBump = sn;

    const float tSize0 = 1./4.;
    sn = doBumpMap(iChannel0, sp*tSize0, sn, 0.01);

    sn = doBumpMap(sp, sn, .1);

    float ao = calcVoxAO(vPos, sp, rd, mask);

    vec3 ld = lightPos - sp;

    float lDist = max(length(ld), 0.001);
    ld /= lDist;

    float atten = 1./(1. + lDist*.2 + lDist*.1);

    float ambience = 0.25;
    float diff = max( dot(sn, ld), 0.0);
    float spec = pow(max( dot( reflect(-ld, sn), -rd ), 0.0 ), 32.);
  
    //vec3 texCol = vec3(1, .75, .5);//vec3(1, .05, .15);//vec3(1, .5, .15);//vec3(1, .6, .4) + step(abs(snNoBump.y), .5)*vec3(0,.4, .6);
    vec3 texCol = vec3(.55, .7, 1.3);

    //float rnd = fract(sin(dot(vPos, vec3(7, 157, 113)))*43758.5453);
    //texCol *= rnd*.5+.5;
    //if(rnd>.5) texCol = vec3(1);

    texCol *= tex3D(iChannel0, sp*tSize0, sn)*4.;

    texCol *= bumpSurf( sp, sn)*.5 + .5;

    vec3 hf =  normalize(ld + sn);
    float th = thickness( sp, sn, 1., 1. );
    float tdiff =  pow( clamp( dot(rd, -hf), 0., 1.), 1.);
    float trans = (tdiff + .0)*th;  
    trans = pow(trans, 4.);        

    float shading = voxShadow(sp + snNoBump*.01, ld, lDist);

    sceneCol = texCol*(diff + ambience) + vec3(.7, .9, 1.)*spec;
    sceneCol += vec3(1, 0.05, .15)*trans*2.;
    sceneCol += envMap(reflect(rd, sn), sn);

    sceneCol *= atten*shading*ao;
  }

  vec3 fog = mix(vec3(.96, .48, .36), vec3(.24, .32, .64), -rd.y*.5 + .5);
  //vec3 fog = mix(vec3(.32, .28, .16)*3., vec3(.32, .04, .08)*2., -rd.y*.5 + .5);

  sceneCol = mix(sceneCol, fog*sqrt(fog)*1.2, smoothstep(0., .95, t/FAR));

  gl_FragColor = vec4(sqrt(clamp(sceneCol, 0., 1.)), 1.0);
}
