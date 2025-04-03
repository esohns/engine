uniform vec2 iResolution;
uniform float iTime;

#define pi (acos(-1.))
#define tau (pi*2.)

mat2
rotate (float a)
{
  float c = cos(a);
  float s = sin(a);
  return mat2(c,-s,s,c);
}

float
sdBox (vec3 p, vec3 b, float r)
{
  vec3 d = abs(p) - (b-r);
  return length(max(d,0.0)) + min(max(d.x,max(d.y,d.z)),0.0)-r;
}

float
sdRect (vec2 p, vec2 b, float r)
{
  vec2 d = abs(p) - (b-r);
  return length(max(d,0.0)) + min(max(d.x,d.y),0.0)-r;
}

float
scene (vec3 p)
{
  float flip = step(gl_FragCoord.x,iResolution.x*.5)*2.-1.;
  float time = flip*iTime;

  p.yz *= rotate(pi*.25*flip);

  vec3 a = vec3(
      length(p.xz),
      p.y,
      atan(p.x,p.z)
  );

  a.z = fract((a.z/tau)*8.+time)-.5;
  a.x -= 1.;
  a.xy = abs(a.xy)-.25;

  float r = .02;
  float d = min(
    sdBox(a,vec3(.2,.2,.4),r),
    sdRect(a.xy,vec2(.15),r)
  );

  return d;
}

vec3
trace (vec3 cam, vec3 dir)
{
  vec3 color = vec3(.2,.1,.9);

  float t=0.;
  float k=0.;
  for(int i=0;i<100;++i)
  {
    k = scene(cam+dir*t);
    t += k;
    if (abs(k) < .001)
      break;
  }

  if (abs(k) < .001)
  {
    vec3 h = cam+dir*t;
    vec2 o = vec2(.001, 0);
    vec3 n = normalize(vec3(
        scene(h+o.xyy)-scene(h-o.xyy),
        scene(h+o.yxy)-scene(h-o.yxy),
        scene(h+o.yyx)-scene(h-o.yyx)
    ));

    float A = .1;
    float B = scene(h+n*A);
    float fakeAO = clamp(B/A,0.,1.);
    fakeAO = pow(fakeAO,.6)*.5+.5;

    float light = 0.;
    vec3 lightDir = normalize(vec3(1,4,-4));

    light += dot(n,lightDir)*.7+.3;

    vec3 H = normalize(lightDir+dir);
    float NdotH = dot(n, H);
    light += pow(max(NdotH,0.), 3.)*30.;

    return light * fakeAO * color;
  }

  float gradient = pow(.8,dot(cam.xy,cam.xy));
  return mix(color,vec3(1),gradient);
}

void
main ()
{
  vec2 uv = gl_FragCoord.xy / iResolution.xy-.5;
  uv.x *= iResolution.x / iResolution.y;

  vec3 cam = vec3(uv*3.,-5.);
  vec3 dir = vec3(0,0,1);

  gl_FragColor.rgb = pow(trace(cam,dir),vec3(.45));
}
