uniform vec2 iResolution;
uniform float iTime;

#define time iTime
#define resolution iResolution

const float count = 7.0;

float
Hash (vec2 p,  float s)
{
  vec3 p2 = vec3(p.xy,360.0 * abs(sin(s)));

  return fract(sin(dot(p2,vec3(27.1,61.7, 12.4)))*273758.5453123);
}

float
noise (vec2 p, float s)
{
  vec2 i = floor(p);
  vec2 f = fract(p);
  f *= f * f * (3.0-2.0*f);

  return mix(mix(Hash(i + vec2(0.,0.), s), Hash(i + vec2(1.,0.), s),f.x),
             mix(Hash(i + vec2(0.,1.), s), Hash(i + vec2(1.,1.), s),f.x),
             f.y) * s;
}

float
fbm (vec2 p)
{
  float v = 0.0;
  v -= noise(p*.5, 0.35);
  v -= noise(p*2., 0.25);
  v -= noise(p*2., 0.25);
  v -= noise(p*2., 0.25);
  v -= noise(p*4., 0.125);
  v -= noise(p*8., 0.0625);

  return v;
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy / iResolution.xy) - .5;
  vec4 O = gl_FragColor;
  vec2 C = gl_FragCoord.xy;
  O-=O;

  for(float i=0.;i<500.;i++)    
    O+=vec4((1.-cos((vec3(3,7,99)+i*9.)))*exp(-9.*length(cross((abs(fract(fract(99.*sin((vec3(1,5,9)+i*9.)))+iTime*.022)*2.-1.)*2.-1.)*9., vec3(gl_FragCoord.xy/iResolution.y-.5,1)))),1)+1e-4;

  vec2 uv3 = ( gl_FragCoord.xy / resolution ) * 2.0 - 1.0;
  uv3.x *= resolution.x/resolution.y;

  vec2 uv4 = ( gl_FragCoord.xy / resolution ) * 2.0 - 1.0;
  uv4.x *= resolution.x/resolution.y;
  float timeMult = .2;
  vec3 finalColor = vec3( 0.0 );
  for( float i=1.; i < count; ++i )
  {
    uv3*=mat2(cos(iTime),sin(iTime),-sin(iTime), cos(iTime));
    uv4*=mat2(cos(iTime),-sin(iTime),sin(iTime), cos(iTime));
    float t = abs(1.0 / ((uv3.x + fbm( uv3 + time/i)) * (i*120.0)));
    float t2 = abs(1.3 / ((uv3.y + fbm( uv3 + time/i)) * (i*120.0)));
    float t3 = abs(1.2 / ((uv4.y - fbm( uv3 - time/i)) * (i*20.0)));
    finalColor +=  t2*t * vec3( i * 0.1, 0.3, i*0.9 )*t3;
  }

  finalColor *= 2340.0 / count;     
  float t = iTime * .1 + ((.25 + .05 * sin(iTime * .1))/(length(uv.xy) + .07)) * 2.2;
  float si = sin(t);
  float co = cos(t);
  mat2 ma = mat2(co, si, -si, co);

  float v1, v2, v3;
  v1 = v2 = v3 = 0.0;

  float s = 0.0;
  for (int i = 0; i < 90; i++)
  {
    vec3 p = s * normalize(vec3(uv, 1.0221))+O.xyz;

    p += vec3(.22, .3, s - 1.5 - sin(iTime * .13) * .1);
    for (int i = 0; i < 8; i++)	p = abs(p) / dot(p,p) - 0.659;
    v1 += dot(p,p) * .0015 * (1.8 + sin(length(uv.xy * 13.0) + .5  - iTime * .2));
    v2 += dot(p,p) * .0013 * (1.5 + sin(length(uv.xy * 14.5) + 1.2 - iTime * .3));
    v3 += length(p.xy*10.) * .0003;
    s  += .035;
  }

  float len = length(uv);
  v1 *= smoothstep(.7, .0, len);
  v2 *= smoothstep(.5, .0, len);
  v3 *= smoothstep(.3, .3, len);

  vec3 col = vec3( v3 * (1.5 + sin(iTime * .2) * .4), (v1 + v3) * .3, v2) + smoothstep(0.2, .0, len) * .85 + smoothstep(.0, .6, v3) * .3;

  gl_FragColor = vec4(min(pow(abs(O.xyz)*finalColor*102., vec3(1.2)), 1.0), 1.0);
}
