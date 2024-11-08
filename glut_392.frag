//precision mediump float;

uniform vec2 iResolution;
uniform float iTime;
uniform vec2 iMouse;
uniform bool iView;

varying vec2 vTexCoord;

// 2D rotation function
mat2
rot2D (float a)
{
  return mat2 (cos (a), -sin (a), sin (a), cos (a));
}

float
sdOctahedron (vec3 p, float s)
{
  p = abs (p);
  return (p.x + p.y + p.z - s) * 0.57735027;
}

float
sdSphere (vec3 p, float s)
{
  return length (p) - s;
}

float
sdBox (vec3 p, vec3 b)
{
  vec3 q = abs (p) - b;
  return length (max (q, 0.0)) + min (max (q.x, max (q.y, q.z)), 0.0);
}

float
sdTorus (vec3 p, vec2 t)
{
  vec2 q = vec2 (length (p.xz) - t.x, p.y);
  return length (q) - t.y;
}

float
sdBoxFrame (vec3 p, vec3 b, float e)
{
  p = abs (p) - b;
  vec3 q = abs (p + e) - e;
  return min (min (
      length (max (vec3 (p.x, q.y, q.z), 0.0)) + min (max (p.x, max (q.y, q.z)), 0.0),
      length (max (vec3 (q.x, p.y, q.z), 0.0)) + min (max (q.x, max (p.y, q.z)), 0.0)),
      length (max (vec3 (q.x, q.y, p.z), 0.0)) + min (max (q.x, max (q.y, p.z)), 0.0));
}

// Scene distance
float
map (vec3 p)
{
  p.z += iTime * 0.3;

  vec3 q = p;
  q = fract (p) - 0.5;

  //float verBeam = sdBox(q, vec3(0.05, 0.5, 0.05));
  //float horBeam = sdBox(q, vec3(0.5, 0.05, 0.05));
  
  //return min(verBeam, horBeam); // distance to an object
  return sdBoxFrame (q, vec3 (0.5), 0.025);
}

vec2 mousePrevious = vec2 (0.0, 0.0);

void
main ()
{
  // copy the vTexCoord
  // vTexCoord is a value that goes from 0.0 - 1.0 depending on the pixels location
  // we can use it to access every pixel on the screen
  vec2 coord = vTexCoord;

  vec2 uv = (coord * 2.0 - 1.0) * iResolution.xy / iResolution.y;

  vec2 m = (iMouse.xy * 2. - iResolution.xy) / iResolution.y;

  mousePrevious = m;
  // Default circular motion if mouse not clicked
  if (!iView)
    m = mousePrevious;

  vec3 ro = vec3 (0.0, 0.0, -3.0); // ray origin
  vec3 rd = normalize (vec3 (uv, 1.0)); // ray direction
  vec3 col = vec3 (0.0); // final pixel color

  float t = 0.0; // total distance travelled

  // Horizontal camera rotation
  ro.yz *= rot2D (-m.y);
  rd.yz *= rot2D (-m.y);

  // Horizontal camera rotation
  ro.xz *= rot2D (-m.x);
  rd.xz *= rot2D (-m.x);

  // Raymarching
  for (int i = 0; i < 80; i++)
  {
    vec3 p = ro + rd * t; // position along the ray

    float d = map (p);

    t += d;

    if (d < 0.001 || t > 100.0)
      break; // "d" early stop if close enough
             // "t" early stop if too far
  }

  // Coloring
  col = vec3 (t * 0.09);

  gl_FragColor = vec4 (col, 1.0);
}
