//precision highp float;

// These are our passed in information from the sketch.js
uniform vec2 iResolution;
uniform float iTime;
//uniform vec2 iMouse;

varying vec2 vTexCoord;

float
HexDist (vec2 p)
{
  p = abs (p);
  float c = dot (p, normalize (vec2 (1, 1.73)));
  c= max (c, p.x);

  return c;
}

vec4
HexCoord (vec2 uv)
{
  vec2 r = vec2 (1, 1.73);
  vec2 h = r * 0.5;
  vec2 a = mod (uv, r) - h;
  vec2 b = mod (uv - h, r) - h;
    
  vec2 gv;
  if (length (a) < length (b))
    gv = a;
  else 
    gv = b;

  float x = atan (gv.x, gv.y);
  float y = 0.5 - HexDist (gv);
  vec2 id = uv - gv;

  return vec4 (x, y, id.x, id.y);
}

vec3
palette (float t)
{
  //0.8, 0.5, 0.4  0.2, 0.4, 0.2  2.0, 1.0, 1.0	0.00, 0.25, 0.25
  vec3 a = vec3 (.3, 0.3, 0.9);
  vec3 b = vec3 (0.2, 0.4, 0.2);
  vec3 c = vec3 (1.2, 0.1, 0.4532);
  vec3 d = vec3 (0.34, 0.2345, 0.95);

  return a + b*cos (6.28318 * (c * t + d));
}

float
pattern (vec2 uv, float t)
{
  float speed = 0.5;
  return sin (dot (uv, uv) + t * speed);
}

void
main ()
{
  // Map vTexCoord to normalized device coordinates (NDC) [-1, 1]
  vec2 uv = vTexCoord * 2.0 - 1.0;

  // Define a scale factor
  const float scale = 2.0;

  // Adjust for aspect ratio and scale the coordinates
  uv.x *= scale * iResolution.x / iResolution.y;
  uv.y *= scale;

  uv *= 13.0;

  vec4 hc = HexCoord(uv);

  vec3 col = palette(length(vec2(hc.z, hc.w + iTime * 0.2)));

  float v =  hc.y * pattern(vec2(hc.z, hc.w), iTime) - 0.035;

  float c = smoothstep(0.0, 25.0/iResolution.y * 0.5, v );
  col -= c;

  // Output the final color with full opacity
  gl_FragColor = vec4 (col, 1.0);
}
