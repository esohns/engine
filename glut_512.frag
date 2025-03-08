#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

#define FAR 40.

mat2 rot2( float a ){ vec2 v = sin(vec2(1.570796, 0) - a);	return mat2(v, -v.y, v.x); }

float
drawObject (vec3 p)
{
  p = abs(fract(p)-.5);
  return dot(p, vec3(.5));
}

float
cellTile (vec3 p)
{
  p /= 5.5;
  vec4 v, d; 
  d.x = drawObject(p - vec3(.81, .62, .53));
  p.xy = vec2(p.y-p.x, p.y + p.x)*.7071;
  d.y = drawObject(p - vec3(.39, .2, .11));
  p.yz = vec2(p.z-p.y, p.z + p.y)*.7071;
  d.z = drawObject(p - vec3(.62, .24, .06));
  p.xz = vec2(p.z-p.x, p.z + p.x)*.7071;
  d.w = drawObject(p - vec3(.2, .82, .64));

  v.xy = min(d.xz, d.yw), v.z = min(max(d.x, d.y), max(d.z, d.w)), v.w = max(v.x, v.y); 

  d.x =  min(v.z, v.w) - min(v.x, v.y);
  //d.x =  min(v.x, v.y);

  return d.x*2.66;
}

vec3
cellTileColor (vec3 p)
{
  int cellID = 0;

  p/=5.5;

  vec3 d = (vec3(.75));

  d.z = drawObject(p - vec3(.81, .62, .53)); if(d.z<d.x)cellID = 1;
  d.y = max(d.x, min(d.y, d.z)); d.x = min(d.x, d.z);

  p.xy = vec2(p.y-p.x, p.y + p.x)*.7071;
  d.z = drawObject(p - vec3(.39, .2, .11)); if(d.z<d.x)cellID = 2;
  d.y = max(d.x, min(d.y, d.z)); d.x = min(d.x, d.z);

  p.yz = vec2(p.z-p.y, p.z + p.y)*.7071;
  d.z = drawObject(p - vec3(.62, .24, .06)); if(d.z<d.x)cellID = 3;
  d.y = max(d.x, min(d.y, d.z)); d.x = min(d.x, d.z);

  p.xz = vec2(p.z-p.x, p.z + p.x)*.7071; 
  d.z = drawObject(p - vec3(.2, .82, .64)); if(d.z<d.x)cellID = 4;
  d.y = max(d.x, min(d.y, d.z)); d.x = min(d.x, d.z);

  vec3 col = vec3(.25);//vec3(.7, .8, 1);

  if (cellID == 3) col = vec3(1, .05, .15);
    
  //if (cellID == 4) col = vec3(.1, .8, .0);//vec3(.5, .4, .35);
    
  //col *= (vec3(clamp(sin(d.x*24.*6.283)*2., 0., 1.)) + .5);

  return col;
}

float
map (vec3 p)
{
  float n = (.5-cellTile(p))*1.5;
  return p.y + dot(sin(p/2. + cos(p.yzx/2. + 3.14159/2.)), vec3(.5)) + n;
}

float
trace (vec3 ro, vec3 rd)
{
  float t = 0.0;

  for (int i = 0; i < 96; i++)
  {
    float d = map (ro + rd*t);

    if (abs (d) < 0.0025 * (t * .125 + 1.) || t > FAR)
      break;

    t += d*.7;
  }

  return min(t, FAR);
}

vec3
getNormal (vec3 p)
{
  const vec2 e = vec2(0.005, 0);
  return normalize(vec3(map(p + e.xyy) - map(p - e.xyy), map(p + e.yxy) - map(p - e.yxy),	map(p + e.yyx) - map(p - e.yyx)));
}

float
calculateAO (vec3 pos, vec3 nor)
{
  float sca = 2.0, occ = 0.0;
  for (int i=0; i<5; i++)
  {
    float hr = 0.01 + float(i)*0.5/4.0;
    float dd = map(nor * hr + pos);
    occ += (hr - dd)*sca;
    sca *= 0.7;
  }

  return clamp( 1.0 - occ, 0.0, 1.0 );    
}

vec3
tex3D (sampler2D tex, vec3 p, vec3 n)
{
  n = max((abs(n) - 0.2)*7., 0.001);
  n /= (n.x + n.y + n.z );
  return (texture(tex, p.yz)*n.x + texture(tex, p.zx)*n.y + texture(tex, p.xy)*n.z).xyz;
}

vec3
texBump (sampler2D tx, vec3 p, vec3 n, float bf)
{
  const vec2 e = vec2(0.002, 0);

  mat3 m = mat3( tex3D(tx, p - e.xyy, n), tex3D(tx, p - e.yxy, n), tex3D(tx, p - e.yyx, n));

  vec3 g = vec3(0.299, 0.587, 0.114)*m; // Converting to greyscale.
  g = (g - dot(tex3D(tx,  p , n), vec3(0.299, 0.587, 0.114)) )/e.x; g -= n*dot(n, g);

  return normalize( n + g*bf );
}

float
curve (vec3 p, float w)
{
  vec2 e = vec2(-1., 1.)*w;

  float t1 = map(p + e.yxx), t2 = map(p + e.xxy);
  float t3 = map(p + e.xyx), t4 = map(p + e.yyy);

  return 0.125/(w*w) *(t1 + t2 + t3 + t4 - 4.*map(p));
}

vec3
envMap (vec3 p)
{
  float c = cellTile(p*6.);
  c = smoothstep(0.2, 1., c);

  return vec3(pow(c, 8.), c*c, c);
  //return vec3(min(c*1.5, 1.), pow(c, 2.5), pow(c, 12.));
}

vec2
path (float z)
{
  float s = sin(z/36.)*cos(z/18.);
  return vec2(s*16., 0.);
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - iResolution.xy*.5)/iResolution.y;

  vec3 lk = vec3(0, 3.5, iTime*6.);
  vec3 ro = lk + vec3(0, .25, -.25);

  vec3 lp = ro + vec3(0, .75, 2);
  vec3 lp2 = ro + vec3(0, .75, 9);

  lk.xy += path(lk.z);
  ro.xy += path(ro.z);
  lp.xy += path(lp.z);
  lp2.xy += path(lp2.z);

  float FOV = 1.57; // FOV - Field of view.
  vec3 fwd = normalize(lk-ro);
  vec3 rgt = normalize(vec3(fwd.z, 0., -fwd.x )); 
  vec3 up = cross(fwd, rgt); 

  vec3 rd = normalize(fwd + FOV*uv.x*rgt + FOV*uv.y*up);

  rd.xy *= rot2( path(lk.z).x/64. );

  /*
  // Works better if the line above is commented out.   
  vec2 ms = vec2(0);
  if (iMouse.z > 1.0) ms = (2.*iMouse.xy - iResolution.xy)/iResolution.xy;
  vec2 a = sin(vec2(1.5707963, 0) - ms.x); 
  mat2 rM = mat2(a, -a.y, a.x);
  rd.xz = rd.xz*rM; 
  a = sin(vec2(1.5707963, 0) - ms.y); 
  rM = mat2(a, -a.y, a.x);
  rd.yz = rd.yz*rM;
  */

  float t = trace(ro, rd);

  vec3 sceneCol = vec3(0.);

  if (t < FAR)
  {
    vec3 sp = ro + rd*t;
    vec3 sn = getNormal(sp);

    const float tSize0 = 1./2.;
    sn = texBump(iChannel0, sp*tSize0, sn, 0.01);

    vec3 texCol = tex3D(iChannel0, sp*tSize0, sn);

    float ao = calculateAO(sp, sn);

    vec3 ld = lp-sp;
    vec3 ld2 = lp2-sp;

    float lDist = max(length(ld), 0.001);
    float lDist2 = max(length(ld2), 0.001);

    ld /= lDist;
    ld2 /= lDist2;

    float atten = 1./(1. + lDist*lDist*0.025);
    float atten2 = 1./(1. +lDist2*lDist2*0.025);

    float ambience = 0.1;

    float diff = max( dot(sn, ld), 0.0);
    float diff2 = max( dot(sn, ld2), 0.0);

    float spec = pow(max( dot( reflect(-ld, sn), -rd ), 0.0 ), 8.);
    float spec2 = pow(max( dot( reflect(-ld2, sn), -rd ), 0.0 ), 8.);

    float crv = clamp(curve(sp, 0.125)*0.5+0.5, .0, 1.);

    float fre = pow( clamp(dot(sn, rd) + 1., .0, 1.), 1.);

    float shading =  crv*0.5+0.5; //smoothstep(-.05, .1, cellTile(sp));
    shading *= smoothstep(-.1, .15, cellTile(sp));

    vec3 env = envMap(reflect(rd, sn))*.5;

    vec3 rCol = cellTileColor(sp)*dot(texCol, vec3(.299, .587, .114));
    sceneCol += (rCol*(diff + ambience) + vec3(.8, .95, 1)*spec*1.5 + env)*atten;
    sceneCol += (rCol*(diff2 + ambience) + vec3(.8, .95, 1)*spec2*1.5 + env)*atten2;
        
    //vec3 rfCol = texture(iChannel1, reflect(rd, sn)).xyz; // Forest scene.
    //sceneCol += rfCol*rfCol*.25;

    sceneCol *= shading*ao;
  }

  sceneCol = mix(sceneCol, vec3(.0, .003, .01), smoothstep(0., FAR-5., t));

  gl_FragColor = vec4(sqrt(clamp(sceneCol, 0., 1.)), 1.0);
}
