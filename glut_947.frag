#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

vec2
W (vec2 p)
{
  p = (p + 3.)*4.;

  float t = iTime/2.;

  for (int i=0; i<3; i++)
  {
    p += cos(p.yx*3. + vec2(t, 1.57))/3.;
    p += sin(p.yx + t + vec2(1.57, 0))/2.;
    p *= 1.3;
  }

  p +=  fract(sin(p+vec2(13, 7))*5e5)*.03 - .015;

  return mod(p, 2.) - 1.;
}

float
bumpFunc (vec2 p)
{
  return length(W(p))*.7071;
}

vec3 smoothFract (vec3 x) { x = fract(x); return min(x, x*(1.-x)*12.); }

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - iResolution*.5)/iResolution.y;

//float th = sin(iTime*0.1)*sin(iTime*0.12)*2.;
//float cs = cos(th), si = sin(th);
//uv *= mat2(cs, -si, si, cs);

  vec3 sp = vec3(uv, 0);
  vec3 rd = normalize(vec3(uv, 1));
  vec3 lp = vec3(cos(iTime)*.5, sin(iTime)*.2, -1);
  vec3 sn = vec3(0, 0, -1);

  vec2 eps = vec2(4./iResolution.y, 0);
  float f = bumpFunc(sp.xy);
  float fx = bumpFunc(sp.xy - eps.xy);
  float fy = bumpFunc(sp.xy - eps.yx);

  const float bumpFactor = .05;

  fx = (fx - f)/eps.x;
  fy = (fy - f)/eps.x;
  sn = normalize(sn + vec3(fx, fy, 0)*bumpFactor);   

  vec3 ld = lp - sp;
  float lDist = max(length(ld), .0001);
  ld /= lDist;

  float atten = 1./(1. + lDist*lDist*.15);
  atten *= f*.9 + .1;

  float diff = max(dot(sn, ld), 0.);  
  diff = pow(diff, 4.)*.66 + pow(diff, 8.)*.34; 

  float spec = pow(max(dot( reflect(-ld, sn), -rd), 0.), 12.); 

  vec3 texCol = texture(iChannel0, sp.xy + W(sp.xy)/8.).xyz; 
  texCol *= texCol;
  texCol = smoothstep(.05, .75, pow(texCol, vec3(.75, .8, .85)));    
  //vec3 texCol = smoothFract( W(sp.xy).xyy )*.1 + .2;

  vec3 col = (texCol*(diff*vec3(1, .97, .92)*2. + .5) + vec3(1, .6, .2)*spec*2.)*atten;

  float ref = max(dot(reflect(rd, sn), vec3(1)), 0.);
  col += col*pow(ref, 4.)*vec3(.25, .5, 1)*3.;

  gl_FragColor = vec4(sqrt(clamp(col, 0., 1.)), 1.);
}
