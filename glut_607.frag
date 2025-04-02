#version 130

uniform vec2 iResolution;
uniform vec4 iDate;
uniform sampler2D iChannel0;

#define CS(a)  vec2(cos(a),sin(a))
#define rnd(x) ( 2.* fract(456.68*sin(1e3*x+mod(iDate.w,100.))) -1.)
#define T(U) textureLod(iChannel0, (U)/R, 0.)
const float r = 1.5, N = 200., da = .1;

void
main ()
{
  vec4 O;
  vec2 U = gl_FragCoord.xy;
  vec2 R = iResolution.xy;

  if (T(R).x==0.)
  {
    U = abs(U/R*2.-1.);
    O  = vec4(max(U.x,U.y)>1.-r/R.y);
    O.w=0.;
    gl_FragColor = O;
    return;
  }

  if (U.y==.5 && T(U).w==0.)
  {
    O = vec4( R/2. + R/2.4* vec2(rnd(U.x),rnd(U.x+.1)) , 3.14 * rnd(U.x+.2), 1);
    if (T(O.xy).x>0.)
      O.w = 0.;
    gl_FragColor = O;
    return;
  }

  O = T(U);

  for (float x=.5; x<N; x++)
  {
    vec4 P = T(vec2(x,.5));
    if (P.w>0.)
      O += smoothstep(r,0., length(P.xy-U)) * (.5+.5*sin(6.3*x/N+vec4(0,-2.1,2.1,1)));
  }

  if (U.y==.5)
  {
    vec4 P = T(U);
    if (P.w>0.)
    {
      float a = P.z-1.6, a0=a;
#define next T(P.xy+(r+2.)*CS(a)).w
      while (next == 0. && a < 13.)
        a += da;
      a = max(a0, a-4.*da);
      if (next > 0.)
      {
        O.w = 0.;
        gl_FragColor = O;
        return;
      }
      O = vec4(P.xy+CS(a),mod(a,6.2832),P.w+1.);
    }
  }

  gl_FragColor = O;
}
