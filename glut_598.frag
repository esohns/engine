#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

#define FAR 10. 

float objID = 0.;

float
dm (vec2 p)
{
  p = fract(p) - .5;
  return (dot(p, p)*4.*.25 + .75)*max(abs(p.x)*.866025 + p.y*.5, -p.y);
}

float
dm2 (vec2 p)
{
  p = fract(p) - .5;   
  return (dot(p, p)*4.*.25 + .75)*max(abs(p.x)*.866025 - p.y*.5, p.y);
}

float
cell (vec2 q)
{
  const mat2 m = mat2(-.5, .866025, -.866025, -.5);    

  vec2 p = q;
  const float offs = .666 - .166;
  vec2 a = sin(vec2(1.93, 0) + iTime)*.166;
  float d0 = dm(p + vec2(a.x, 0));
  float d1 = dm(p + vec2(0, offs + a.y));

  p = m*(p + .5);
  float d2 = dm(p + vec2(a.x, 0));
  float d3 = dm(p + vec2(0, offs + a.y)); 

  float l1 = min(min(d0, d1), min(d2, d3))*2.; 

  p = q;
  d0 = dm2(p + vec2(a.x, 0));
  d1 = dm2(p + vec2(0, offs + a.y));

  p = m*(p + .5);
  d2 = dm2(p + vec2(a.x, 0));
  d3 = dm2(p + vec2(0, offs + a.y)); 

  float l2 = min(min(d0, d1), min(d2, d3))*2.; 

  objID = step(l1, -(l2 - .4)); // Object, or pattern, ID.

  return max(l1, -(l2 - .4));
}

float
heightMap (vec3 p)
{
  return cell(p.xy*2.);
}

float
map (vec3 p)
{
  float tx = heightMap(p);

  return 1.2 - p.z + (.5 - tx)*.125;
}

vec3
nr (vec3 p, inout float edge, inout float crv)
{
  vec2 e = vec2(2./iResolution.y, 0);

  float d1 = map(p + e.xyy), d2 = map(p - e.xyy);
  float d3 = map(p + e.yxy), d4 = map(p - e.yxy);
  float d5 = map(p + e.yyx), d6 = map(p - e.yyx);
  float d = map(p)*2.;

  edge = abs(d1 + d2 - d) + abs(d3 + d4 - d) + abs(d5 + d6 - d);
  edge = smoothstep(0., 1., sqrt(edge/e.x*2.));

  e = vec2(12./450., 0);
  d1 = map(p + e.xyy), d2 = map(p - e.xyy);
  d3 = map(p + e.yxy), d4 = map(p - e.yxy);
  d5 = map(p + e.yyx), d6 = map(p - e.yyx);
  crv = clamp((d1 + d2 + d3 + d4 + d5 + d6 - d*3.)*32. + .5, 0., 1.);

  e = vec2(2./450., 0); //iResolution.y - Depending how you want different resolutions to look.
  d1 = map(p + e.xyy), d2 = map(p - e.xyy);
  d3 = map(p + e.yxy), d4 = map(p - e.yxy);
  d5 = map(p + e.yyx), d6 = map(p - e.yyx);

  return normalize(vec3(d1 - d2, d3 - d4, d5 - d6));
}

float
cao (vec3 p, vec3 n)
{
  float sca = 2., occ = 0.;
  for(float i=0.; i<6.; i++)
  {
    float hr = .01 + i*.75/5.;        
    float dd = map(n * hr + p);
    occ += (hr - dd)*sca;
    sca *= 0.8;
  }

  return clamp(1.0 - occ, 0., 1.);    
}

float
n3D (vec3 p)
{
  const vec3 s = vec3(7, 157, 113);
  vec3 ip = floor(p); p -= ip; 
  vec4 h = vec4(0., s.yz, s.y + s.z) + dot(ip, s);
  p = p*p*(3. - 2.*p); //p *= p*p*(p*(p * 6. - 15.) + 10.);
  h = mix(fract(sin(mod(h, 6.2831))*43758.5453), fract(sin(mod(h + s.x, 6.2831))*43758.5453), p.x);
  h.xy = mix(h.xz, h.yw, p.y);
  return mix(h.x, h.y, p.z); // Range: [0, 1].
}

vec3
eMap (vec3 rd, vec3 sn)
{
  vec3 sRd = rd;

  rd.xy -= iTime*.25;
  rd *= 3.;

  float c = n3D(rd)*.57 + n3D(rd*2.)*.28 + n3D(rd*4.)*.15; // Noise value.
  c = smoothstep(0.5, 1., c); // Darken and add contast for more of a spotlight look.

  vec3 col = vec3(min(c*1.5, 1.), pow(c, 2.5), pow(c, 12.)).zyx; // More color.

  return mix(col, col.yzx, sRd*.25+.25); 
}

void
main ()
{
  vec3 rd = normalize(vec3(gl_FragCoord.xy - iResolution.xy*.5, iResolution.y)),
        ro = vec3(-iTime*.125, -iTime*.05, 0), l = ro + vec3(.5, -1.5, -1.);

  float d, t = 0.;
  for(int i=0; i<64;i++)
  {
    d = map(ro + rd*t);
    if(abs(d)<0.001 || t>FAR)
      break;
    t += d*.86;
  }

  float svObjID = objID;

  vec3 sCol = vec3(0);

  float edge = 0., crv = 1.;

  if(t<FAR)
  {
    vec3 p = ro + rd*t, n = nr(p, edge, crv);//normalize(fract(p) - .5);

    l -= p;
    d = max(length(l), 0.001);
    l /= d;

    float atten = 1./(1. + d*d*.05);
    float shade = heightMap(p);

    vec2 tuv = vec2(0);
    vec3 q = p;
    const mat2 mr3 = mat2(.866025, .5, -.5, .866025);
    q.xy *= mr3;
    if((n.x)>.002) tuv = vec2((q.x)*.866 - q.y*.5, q.y);
    q.xy *= mr3*mr3;
    if (n.x<-.002) tuv = vec2((q.x)*.866 - q.y*.5, q.y);
    q.xy *= mr3*mr3; // Rotate twice.
    if (n.y>.002) tuv = vec2((q.x)*.866 - q.y*.5, q.y);

    vec3 tx = texture(iChannel0, tuv*2.).xyz;
    tx = smoothstep(.05, .5, tx*tx);

    if(svObjID>.5) tx *= vec3(2, .9, .3);

    float ao = cao(p, n);

    float diff = max(dot(l, n), 0.); // Diffuse.
    float spec = pow(max(dot(reflect(l, n), rd), 0.), 6.);

    if(svObjID>.5)
    {
      float rg = dot(sin(p*6. + cos(p.yzx*4. + 1.57/3.)), vec3(.333))*.5 + .5;
      tx = mix(tx, tx.zxy, smoothstep(0.6, 1., rg));
    }

    sCol = tx*(diff + .5) + vec3(1, .6, .2)*spec*3.;

    sCol += (sCol*.75 + .25)*eMap(reflect(rd, n), n)*3.;

    sCol *= (smoothstep(0., .5, shade)*.75 + .25);

    sCol *= min(crv, 1.)*.7 + .3;
    sCol *= 1. - edge*.85;
    sCol *= ao*atten;
  }

  gl_FragColor = vec4(sqrt(clamp(sCol, 0., 1.)), 1.);
}
