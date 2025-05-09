#version 130

uniform vec2 iResolution;
uniform int iFrame;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

vec4
getTexture (sampler2D sam, vec2 g, vec2 p, vec2 s)
{
  vec2 gp = g + p;
  
  if (gp.x >= s.x)
    gp.x = gp.x - s.x;
  if (gp.y >= s.y)
    gp.y = gp.y - s.y;
  if (gp.x < 0.0)
    gp.x = s.x + gp.x;
  if (gp.y < 0.0)
    gp.y = s.y + gp.y;

  return texture(sam, gp / s);
}

vec4
getState (sampler2D sam, vec2 g, vec2 s, float n)
{
  vec4 p = vec4(0);
  for (float i = 0.; i < n; i++)
    p = getTexture(sam, g, -p.xy, s);
  return p;
}

#define tex(p) getTexture(iChannel0, g, p, s)
#define emit(v,k) if (length(g-(s * (0.5 + v))) < 5.) f.x = k, f.w = 1.

#define frameStep 10000.

void
main ()
{
  vec4 f;
  vec2 g = gl_FragCoord.xy;

  float frame = float(iFrame);
  //frame = mod(frame, frameStep);

  vec2 s = iResolution.xy;
  
  vec4 r = tex(vec2(1,0));
  vec4 t = tex(vec2(0,1));
  vec4 l = tex(vec2(-1,0));
  vec4 b = tex(vec2(0,-1));

  vec2 v = g / s;
    
  vec2 c = sin(v * 6.28318) * .5 + .5;
  float cc = c.x + c.y;

  f = getState(iChannel0, g, s, cc * 2. + 1.);

  f.xy += vec2(r.z - l.z, t.z - b.z);

  vec4 dp = (r + t + l + b) / 4.;
  float div = ((l - r).x + (b - t).y) / 20.;

  f.z = dp.z - div;

  emit(vec2(-0.45,0.), 50.0);
  emit(vec2(0.45,0.), -50.0);

  if (frame < 2. || iMouse.z > 0.)
    f *= 0.;

  gl_FragColor = f;
}
