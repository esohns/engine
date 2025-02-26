uniform vec2 iResolution;
uniform float iTime;

float
heightMap (vec2 p)
{
  p *= 3.;
    
  vec2 h = vec2(p.x + p.y*.57735, p.y*1.1547);
    
  vec2 fh = floor(h);
  vec2 f = h - fh; h = fh;
  float c = fract((h.x + h.y)/3.);
  h =  c<.666 ?   c<.333 ?  h  :  h + 1.  :  h  + step(f.yx, f); 

  p -= vec2(h.x - h.y*.5, h.y*.8660254);
    
  c = fract(cos(dot(h, vec2(41, 289)))*43758.5453); // Reusing "c."
  p -= p*step(c, .5)*2.; // Equivalent to: if (c<.5) p *= -1.;
    
  p -= vec2(-1, 0);
  c = dot(p, p); // Reusing "c" again.
  p -= vec2(1.5, .8660254);
  c = min(c, dot(p, p));
  p -= vec2(0, -1.73205);
  c = min(c, dot(p, p));
    
  return sqrt(c);
    
  //c = sqrt(c);
  //c = cos(c*6.283*1.) + cos(c*6.283*2.);
  //return (clamp(c*.6+.5, 0., 1.));
}

float
map (vec3 p)
{
  float c = heightMap(p.xy); // Height map.
  c = cos(c*6.2831589) + cos(c*6.2831589*2.);
  c = (clamp(c*.6 +.5, 0., 1.));

  return 1. - p.z - c*.025;
}

vec3
getNormal (vec3 p, inout float edge, inout float crv)
{
  vec2 e = vec2(.01, 0); // Larger epsilon for greater sample spread, thus thicker edges.

  float d1 = map(p + e.xyy), d2 = map(p - e.xyy);
  float d3 = map(p + e.yxy), d4 = map(p - e.yxy);
  float d5 = map(p + e.yyx), d6 = map(p - e.yyx);
  float d = map(p)*2.;

  edge = abs(d1 + d2 - d) + abs(d3 + d4 - d) + abs(d5 + d6 - d);
  //edge = max(max(abs(d1 + d2 - d), abs(d3 + d4 - d)), abs(d5 + d6 - d)); // Etc.

  edge = smoothstep(0., 1., sqrt(edge/e.x*2.));

  crv = clamp((d1 + d2 + d3 + d4 + d5 + d6 - d*3.)*32. + .6, 0., 1.);

  e = vec2(.0025, 0);
  d1 = map(p + e.xyy), d2 = map(p - e.xyy);
  d3 = map(p + e.yxy), d4 = map(p - e.yxy);
  d5 = map(p + e.yyx), d6 = map(p - e.yyx); 

  return normalize(vec3(d1 - d2, d3 - d4, d5 - d6));
}

float
calculateAO (vec3 p,  vec3 n)
{
  float sca = 2., occ = 0.;
  for (int i = 0; i < 5; i++)
  {
    float hr = .01 + float (i) * 0.5 / 4.0;
    float dd = map(n * hr + p);
    occ += (hr - dd)*sca;
    sca *= 0.7;
  }

  return clamp (1.0 - occ, 0., 1.);
}

float
n3D (vec3 p)
{
  const vec3 s = vec3(7, 157, 113);
  vec3 ip = floor(p); p -= ip; 
  vec4 h = vec4(0., s.yz, s.y + s.z) + dot(ip, s);
  p = p*p*(3. - 2.*p); //p *= p*p*(p*(p * 6. - 15.) + 10.);
  h = mix(fract(sin(mod(h, 6.2831589))*43758.5453), 
          fract(sin(mod(h + s.x, 6.2831589))*43758.5453), p.x);
  h.xy = mix(h.xz, h.yw, p.y);
  return mix(h.x, h.y, p.z); // Range: [0, 1].
}

vec3
envMap (vec3 rd, vec3 sn)
{
  vec3 sRd = rd;

  rd.xy -= iTime*.25;
  rd *= 3.;

  float c = n3D(rd)*.57 + n3D(rd*2.)*.28 + n3D(rd*4.)*.15; // Noise value.
  c = smoothstep(.4, 1., c); // Darken and add contast for more of a spotlight look.

  vec3 col = vec3(c, c*c, c*c*c*c); // Simple, warm coloring.
  //vec3 col = vec3(min(c*1.5, 1.), pow(c, 2.5), pow(c, 12.)); // More color.

  return mix (col, col.yzx, sRd*.25+.25); 
}

vec2
hash22 (vec2 p)
{
  float n = sin(mod(dot(p, vec2(41, 289)), 6.2831589));
  return fract(vec2(262144, 32768)*n)*.75 + .25; 

  //p = fract(vec2(262144, 32768)*n); 
  //return sin( p*6.2831853 + iTime )*.35 + .65; 
}

float
Voronoi (vec2 p)
{
  vec2 g = floor(p), o; p -= g;

  vec3 d = vec3(1); // 1.4, etc. "d.z" holds the distance comparison value.

  for(int y = -1; y <= 1; y++)
  {
    for(int x = -1; x <= 1; x++)
    {
      o = vec2(x, y);
      o += hash22(g + o) - p;

      d.z = dot(o, o); 
      //o = abs(o);
      //d.z = max(o.x*.8666 + o.y*.5, o.y);// 
      //d.z = max(o.x, o.y);
      //d.z = (o.x*.7 + o.y*.7);

      d.y = max(d.x, min(d.y, d.z));
      d.x = min(d.x, d.z); 
    }
  }

  return max(d.y/1.2 - d.x*1., 0.)/1.2;
  //return d.y - d.x; // return 1.-d.x; // etc.
}

void
main ()
{
  vec3 rd = normalize(vec3(2.*gl_FragCoord.xy - iResolution.xy, iResolution.y));

  float tm = iTime/2.;
  vec2 a = sin(vec2(1.570796, 0) + sin(tm/4.)*.3);
  rd.xy = mat2(a, -a.y, a.x)*rd.xy;

  vec3 ro = vec3(tm, cos(tm/4.), 0.);

  vec3 lp = ro + vec3(cos(tm/2.)*.5, sin(tm/2.)*.5, -.5);

  float d, t=0.;
  for (int j=0;j<32;j++)
  {
      d = map(ro + rd*t);
      t += d*.7;

      if (d < 0.001)
        break;
  }

  float edge, crv;

  vec3 sp = ro + rd*t;
  vec3 sn = getNormal(sp, edge, crv);
  vec3 ld = lp - sp;

  float c = heightMap(sp.xy); 

  vec3 fold = cos(vec3(1, 2, 4)*c*6.2831589);

  float c2 = heightMap((sp.xy + sp.z*.025)*6.);
  c2 = cos(c2*6.2831589*3.);
  c2 = (clamp(c2 +.5, 0., 1.)); 

  //if(temp.x>0. || temp.y>0.) sn = dbF(sp, sn, .001);

  vec3 oC = vec3(1);

  if (fold.x > 0.)
    oC = vec3(1, .05, .1)*c2;

  if (fold.x<0.05 && (fold.y)<0.)
    oC = vec3(1, .7, .45)*(c2*.25 + .75);
  else if (fold.x<0.)
    oC = vec3(1, .8, .4)*c2;
  //oC *= n3D(sp*128.)*.35 + .65;

  float p1 = 1.0 - smoothstep(0., .1, fold.x*.5+.5);
  //float p2 = 1.0 - smoothstep(0., .1, cos(heightMap(sp.xy + 1. + iTime/4.)*6.283)*.5+.5);
  float p2 = 1.0 - smoothstep(0., .1, Voronoi(sp.xy*4. + vec2(tm, cos(tm/4.))));
  p1 = (p2 + .25)*p1;
  oC += oC.yxz*p1*p1;

  float lDist = max(length(ld), 0.001);
  float atten = 1./(1. + lDist*.125); // Light attenuation.
    
  ld /= lDist; // Normalizing the light direction vector.
    
  float diff = max(dot(ld, sn), 0.); // Diffuse.
  float spec = pow(max( dot( reflect(-ld, sn), -rd ), 0.0 ), 16.);
  float fre = pow(clamp(dot(sn, rd) + 1., .0, 1.), 3.);

  crv = crv*.9 + .1;
  float ao = calculateAO(sp, sn);

  vec3 col = oC*(diff + .5) + vec3(1., .7, .4)*spec*2. + vec3(.4, .7, 1)*fre;

  col += (oC*.5+.5)*envMap(reflect(rd, sn), sn)*6.; // Fake environment mapping.

  col *= 1. - edge*.85; // Darker edges.   

  col *= (atten*crv*ao);

  gl_FragColor = vec4(sqrt(clamp(col, 0., 1.)), 1.);
}
