uniform vec2 iResolution;
uniform float iTime;

#define R mat2(cos(a/4.+vec4(0,11,33,0)))

void
main ()
{
  vec4 O;
  vec2 I = gl_FragCoord.xy;

  vec4 h; O=++h;

    vec2 u,r=iResolution.xy;
    for(float A,l,L,a,i=7.;--i>0.;
        O=mix(h=sin(i+a/3.+vec4(1,3,5,0))*.2+.7,O, A=min(--l*r.y*.02,1.))*(l + h + .5*A*u.y/L )/L)
        a-=sin(a-=sin(a=iTime*4.+i*.4)),
        u=(I+I-r)/r.y/.1,
        L = l = max(length(u -= R*clamp(u*R,-i,i)),1.);

  gl_FragColor = O;
}
