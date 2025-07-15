#version 130

uniform vec2 iResolution;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

void
main ()
{
  ivec2 ipos = ivec2(gl_FragCoord.xy);
  vec4 old = texelFetch(iChannel0, ipos, 0);
  gl_FragColor = old;
  if (ipos.x == 0)
  {
    vec4 t0 = texelFetch(iChannel0, ivec2(0, 0), 0);
    vec4 t1 = texelFetch(iChannel0, ivec2(0, 1), 0);
    vec2 s2 = texelFetch(iChannel0, ivec2(0, 2), 0).xy;
    vec2 s3 = texelFetch(iChannel0, ivec2(0, 3), 0).xy;
#define res iResolution
#define N( p ) ( p.xy - res/2. )/min( res.y, res.x )
    vec2 m      = N(iMouse.xy);
    bool md     = (iMouse.z > 0.0);
    bool prevMd = (t1.x > 0.0);
    vec2 o0 = t0.xy;
    vec2 w0 = md 
              ? o0 
              : ((!md && prevMd) 
                  ? (s3 + (m - s2)) 
                  : o0);
    float w1 = md ? 1.0 : 0.0;
    vec2 w2   = (md && !prevMd) ? m : s2;
    vec2 w3   = (md && !prevMd) ? o0 : s3;
    if (ipos.y == 0)
      gl_FragColor = vec4(w0.x, w0.y, 0.0, 1.0);
    else if (ipos.y == 1)
      gl_FragColor = vec4(w1, 0.0, 0.0, 1.0);
    else if (ipos.y == 2)
      gl_FragColor = vec4(w2.x, w2.y, 0.0, 1.0);
    else if (ipos.y == 3)
      gl_FragColor = vec4(w3.x, w3.y, 0.0, 1.0);
  }
}
