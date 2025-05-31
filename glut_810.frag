uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;

#define pi 3.1415926535

float
h21 (vec2 a)
{
  return fract(sin(dot(a.xy,vec2(12.9898,78.233)))*43758.5453123);
}

float
h11 (float a)
{
  return fract(sin((a)*12.9898)*43758.5453123);
}

vec3
pal (float t, vec3 a, vec3 b, vec3 c, vec3 d)
{
  return a + b*cos(2.*pi*(c*t+d));
}

float
box (vec2 p, vec2 b)
{
  vec2 d = abs(p)-b;
  return max(d.x,d.y);
}

void
main ()
{
  vec2 R = iResolution.xy;
  vec2 uv = (gl_FragCoord.xy-0.5*R.xy)/R.y;
  vec3 col = vec3(0.);
  float t = mod(iTime,6000.)*0.8;
  float px = 1./iResolution.y;

  vec2 dMin = vec2(-0.5);
  vec2 dMax = vec2(0.5);
  dMin.x*=R.x/R.y;
  dMax.x*=R.x/R.y;
  vec2 dim = dMax - dMin;
  float id = 0.;
  float ITERS = 7.;
  float seed = 0.4;
 
  vec2 M = iMouse.xy/iResolution.xy;

  float MIN_SIZE = 0.01;
  //float ITERS = its;
  float BORDER_SIZE = 0.003;
  float MIN_ITERS = 1.;

  vec2 diff2 = vec2(1);
  for(float i = 0.;i<ITERS;i++)
  {
    vec2 divHash=tanh(vec2(sin(t*pi/3.+id+i*t*0.05),cos(t*pi/3.+h11(id)*100.+i*t*0.05))*7.)*0.35+0.5;
    //divHash=vec2(sin(t*pi/3.+id),cos(t*pi/3.+h11(id)*100.))*0.5+0.5;

    if(iMouse.z>0.5)
      divHash = mix(divHash,M,0.3);

    vec2 divide = divHash * dim + dMin;

    divide = clamp(divide, dMin + MIN_SIZE+0.01, dMax - MIN_SIZE-0.01);

    vec2 minAxis = min(abs(dMin - divide), abs(dMax - divide));
    float minSize = min( minAxis.x, minAxis.y);

    bool smallEnough = minSize < MIN_SIZE;
    if (smallEnough && i + 1. > MIN_ITERS)
      break;

    dMax = mix( dMax, divide, step( uv, divide ));
    dMin = mix( divide, dMin, step( uv, divide ));

    diff2 =step( uv, divide)-
    vec2(h11(diff2.x)*10.,h11(diff2.y)*10.);

    id = length(diff2)*100.0;

    dim = dMax - dMin;
  }

  vec2 center = (dMin + dMax)/2.0;
  float a = box(uv-center,dim*0.5);

  id = h11(id)*1000.0;
  vec3 e = vec3(0.5);
  vec3 al = pal(fract(id)*0.75+0.8,e*1.3,e,e*2.0,vec3(0,0.33,0.66));
  col = clamp(al,0.,1.);
  col -= smoothstep(-px,px,a+BORDER_SIZE);

  gl_FragColor = vec4(col,1.0);
}
