#version 130

uniform vec2 iResolution;
uniform float iTime;

#define T iTime
#define PI 3.141596
#define S smoothstep
#define FRAME_or_BOX 1 // 0 - box, 1 - frame

mat2
rotate (float a)
{
  float s = sin(a);
  float c = cos(a);

  return mat2(c,-s,s,c);
}

float
sdBoxFrame (vec3 p, vec3 b, float e)
{
       p = abs(p  )-b;
  vec3 q = abs(p+e)-e;
  return min(min(
      length(max(vec3(p.x,q.y,q.z),0.0))+min(max(p.x,max(q.y,q.z)),0.0),
      length(max(vec3(q.x,p.y,q.z),0.0))+min(max(q.x,max(p.y,q.z)),0.0)),
      length(max(vec3(q.x,q.y,p.z),0.0))+min(max(q.x,max(q.y,p.z)),0.0));
}

float
sdBox (vec3 p, vec3 b)
{
  vec3 q = abs(p) - b;
  float d = length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);

  return abs(d)+0.1;
}

void
main ()
{
  vec4 O;
  vec2 I = gl_FragCoord.xy;

  vec2 R = iResolution.xy;
  vec2 uv = (I*2.-R)/R.y;

  O *= 0.;

  vec3 ro = vec3(0.,0.,-10.);
  vec3 rd = normalize(vec3(uv, 1.));
  float z;
  float d=1e4;
  vec3 col = vec3(0);

  for (float i = 0.; i < 100.; i++)
  {
    vec3 p = ro + rd * z;

    for(float i=1.;i<5.;i++)
    {
      vec3 q = p;
      q.xz *= rotate(iTime * .2*i + i);
      q.xy *= rotate(iTime * .2*i + i);

#if FRAME_or_BOX
      float D = sdBoxFrame(q, vec3(i), 0.);
      col += (1.+sin(vec3(3,2,1)+q.x*0.1+i))/D;
#else
      float D = sdBox(q, vec3(i));
      col += (1.+sin(vec3(3,2,1)+q.x+i))/D*(1.-i*0.2)*0.01;
#endif

      d = min(d, D);
    }

    z += d*0.6;
    if(z>5e1 || d<1e-4)
      break;
  }

#if FRAME_or_BOX
  O.rgb = tanh(col*5e-3);
#else
  O.rgb = tanh(col*5e-1);
#endif

  gl_FragColor = O;
}
