#version 130

// glut_708_common.glsl
#define FORCE_RANGE vec4(   25, 2.5,     0,0)
#define BLUR_DEPTH 40.
#define SPEED 2.
#define SEPARATION 50.
// glut_708_common.glsl

uniform vec2 iResolution;
uniform int iFrame;
uniform sampler2D iChannel0;
uniform sampler2D iChannel2;

vec2 R;
vec4 A (vec2 U) {return texture(iChannel0,U/R);}
//vec4 B (vec2 U) {return texture(iChannel1,U/R);}
vec4 C (vec2 U) {return texture(iChannel2,U/R);}

void
X (vec2 U, inout vec4 Q, vec2 u)
{
  vec4 p = A(U+u);
  if (length(p.xy - U) < length(Q.xy-U)) Q = p;
}

void
main ()
{
  vec4 Q;
  vec2 U = gl_FragCoord.xy;

  R = iResolution.xy;
  Q = A(U);

  for (int x = -1; x <= 1; x++)
    for (int y = -1; y <= 1; y++)
      X(U,Q,vec2(x,y));
  vec2 u = Q.xy;
  vec4 n = C(u+vec2(0,1)),
       e = C(u+vec2(1,0)),
       s = C(u+vec2(0,-1)),
       w = C(u+vec2(-1,0));

  vec3 dx = e.xyz-w.xyz;
  vec3 dy = n.xyz-s.xyz;
  vec2 v = -Q.w*vec2(dx.x,dy.x)-vec2(dx.y,dy.y);
  Q.xy += clamp(SPEED*v/(1.+0.2*abs(Q.w)),vec2(-1),vec2(1));

  if (iFrame < 1)
  {
    vec2 u = U;
    U = floor((u)/SEPARATION)*SEPARATION;
    if (U.x<0.55*R.x&&U.x>0.45*R.x)
    {
      U=floor(u/SEPARATION*4.)*SEPARATION/4.;
      Q = vec4(U,1,-0.5);
    }
    else
      Q = vec4(U,1,sign(U.x-0.5*R.x)*(U.y>0.5*R.y?-9.:2.));
  }

  gl_FragColor = Q;
}
