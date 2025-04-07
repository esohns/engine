uniform vec2 iResolution;
uniform float iTime;
//uniform vec4 iMouse;

#define T iTime
#define r(v,t) { float a = (t)*T, c=cos(a),s=sin(a); v*=mat2(c,s,-s,c); }
#define SQRT3_2  1.26
#define SQRT2_3  1.732
#define smin(a,b) (1./(1./(a)+1./(b)))

void
main ()
{
  vec4 f;
  vec2 w = gl_FragCoord.xy;

  vec4 p = vec4(w,0,1)/iResolution.yyxy-.5, d,c; p.x-=.4;
  r(p.xz,.13); r(p.yz,.2); r(p.xy,.1);
  d = p;
  p.z += 5. *T;
  //vec2 mouse = iMouse.xy/iResolution.xy;
  float closest = 999.0;
  f = vec4(0);

  for (float i=1.; i>0.; i-=.01)
  {
    vec4 u = floor(p/8.), t = mod(p, 8.)-4., ta;
    u = sin(78.*(u+u.yzxw));
    c = p/p*1.2; // *vec4(1,0,0,0);

    float x1,x2,x3,x=1e9, t4 = clamp(cos(T/4.),0.,1.), t8 = clamp(cos(T/8.),0.,1.);
    ta = abs(t);
    x2 = smin(length(t.xy),smin(length(t.yz),length(t.xz))) -.7;   
    ta = abs(mod(p, 1.)-.2); x1 = min(ta.x,min(ta.y,ta.z)); x1 = sqrt(x1*x1+x2*x2)-.0;
    ta = abs(mod(p, 1.)+.2); x3 = min(ta.x,min(ta.y,ta.z)); x3 = sqrt(x3*x3+x2*x2)-.0;
    x = min(x1,x3);
    x3 = min(length(t.xy),min(length(t.yz),length(t.xz))) -.0; //x = min(x,x3);
    f += .7*vec4(1,1,0,0) * exp(-10.*x3)*clamp(dot(.3+.7*sin(3.*(p+8.*T+u)),vec4(1,1,1,0)),0.,1.)*t8*t4;

    if(x2<.01)
      f += .01*vec4(1.5,.3,.3,0)*t4;

    if(x<.01)
    {
      f += i*c*1.;
      break;
    }

    p -= d*x;
  }

  gl_FragColor = f;
}
