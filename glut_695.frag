uniform vec2 iResolution;
uniform float iTime;

mat2 rot2 (float th) { vec2 a = sin(vec2(1.5707963, 0) + th); return mat2(a, -a.y, a.x); }

float tri (float x) { return abs(x-floor(x)-.5); }
vec2 tri (vec2 x) { return abs(x-floor(x)-.5); }
//vec2 tri (vec2 x) { return cos(x*6.2831853)*0.25+0.25; } // smooth version

float hLyr (vec2 p) { return dot(tri(p/1.5 + tri(p.yx/3. + .25)), vec2(1)); }

float
hMap (vec2 p)
{
  float ret = 0., a = 1.;

  for(int i=0; i<3; i++)
  {
    ret += abs(a)*hLyr(p/a);
    p = mat2(.866025, .57735, -.57735, .866025)*p;
    a *= -0.3;
  }

  ret = smoothstep(-.2, 1., ret*ret/1.39/1.39);

  return ret*.975 + tri(ret*12.)*.05;
}

float map (vec3 p) { return (p.y - hMap(p.xz)*.35)*.75; }

vec3
normal (vec3 p, inout float edge)
{
  vec2 e = vec2(-1., 1.)*.5/iResolution.y;  
    
  float d1 = map(p + e.yxx), d2 = map(p + e.xxy);
  float d3 = map(p + e.xyx), d4 = map(p + e.yyy); 
  float d = map(p);
    
  edge = abs(d1 + d2 + d3 + d4 - d*4.);
  edge = smoothstep(0., 1., sqrt(edge/e.y*2.));

  e = vec2(-1., 1.)*.001;  
  d1 = map(p + e.yxx), d2 = map(p + e.xxy);
  d3 = map(p + e.xyx), d4 = map(p + e.yyy); 

  return normalize(e.yxx*d1 + e.xxy*d2 + e.xyx*d3 + e.yyy*d4 );   
}

void
main ()
{
  vec3 rd = normalize(vec3(gl_FragCoord.xy - iResolution.y*.5, iResolution.y));
  rd.yz = rot2(.35)*rd.yz;
  vec3 ro = vec3(iTime*.4, .5, iTime*.2);

  float t=0., d;
  for(int i=0; i<96; i++)
  {
    d = map(ro + rd*t);

    if(abs(d)<.001*(t*.125 + 1.) || t>20.)
      break;

    t += (step(1., t)*.3 + .7)*d;
  }
    
  vec3 sp = ro + rd*t;

  vec3 ld = vec3(-0.676, 0.408, 0.613); // Normalized, or pretty close.

  float edge;
  vec3 n = normal(sp, edge);

  float dif = max(dot(ld, n), 0.); // Diffuse.
  float spe = pow(max(dot(reflect(rd, n), ld), 0.), 16.);

  float sh = hMap(sp.xz);
    
  float rnd = fract(sin(dot(floor(sp.xz*512.), vec2(41.73, 289.67)))*43758.5453)*.5 + .5;

  vec3 fog = mix(vec3(.75,.77, .78), vec3(1.04, .95, .87), (pow(1.0 + dot(rd, ld), 3.))*.35);

  vec3 c = mix((vec3(1.1, 1.05, 1)*rnd*(dif + .1)*sh + fog*spe)*(1. - edge*.7), fog*fog, min(1., t*.3));

  gl_FragColor = vec4(c, 1);
}
