
#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 iResolution;
uniform sampler2D iCanvas;
uniform vec2 iMouse;
uniform float iTime;

varying vec2 vTexCoord;

float hash1(vec2 uv)
{
  return fract(sin(dot(uv, vec2(1234.5678, 567.8901)))*12345.67);
}

vec2 hash2(vec2 uv)
{
  float x = hash1(uv);
  return vec2(x, hash1(uv+x));
}

vec3 hash3(vec2 uv)
{
  vec2 xy = hash2(uv);
  return vec3(xy, hash1(uv+xy));
}

// Calculate smooth minimum
float smin(float a, float b, float t)
{
  float c = clamp(.5+(a-b)/t, 0., 1.);
  return (1.-c)*(a-.5*t*c)+c*b;
}

//	Input:
//		t: animate
//		mt: smooth minimum effect scale
//	Output:
//		moff: offset from current grid to the grid where the closest point is
//		mdst: distances
//		midst: interior distances
vec4 voronoi(vec2 uv, vec2 t, float mt)
{
#define TAU 6.28318530718

  vec2 fuv = fract(uv);
  vec2 iuv = floor(uv);
    
  vec2 moff, mdir, off, pos, dir;
  float dst, idst;

  float mdst = 8.;
  for (int x = -1; x <= 1; x++)
  for (int y = -1; y <= 1; y++) {
    off = vec2(float(x), float(y));
    pos = hash2(iuv+off);
    pos = .5+.49*sin(t+pos*TAU);
    dir = pos+off-fuv;
    dst = dot(dir, dir);
    if (dst < mdst) {
      mdst = dst;
      moff = off;
      mdir = dir;
    }
  }

  float midst = 8.;
  for (int x = -2; x <= 2; x++)
  for (int y = -2; y <= 2; y++) {
    off = moff+vec2(float(x), float(y));
    pos = hash2(iuv+off);
    pos = .5+.49*sin(t+pos*TAU);
    dir = pos+off-fuv;
    if (dot(mdir-dir, mdir-dir) > 0.00001) {
      idst = dot(.5*(mdir+dir), normalize(dir-mdir));
      midst = smin(midst, idst, abs(mt));
    }
  }

  return vec4(moff, mdst, midst);
}

void main ()
{
  vec2 uv = gl_FragCoord.xy/iResolution.xy;
  vec3 color = vec3(0.);

  // Center the origin and adjust aspect ratio
  uv -= .5;
  uv.x *= iResolution.x/iResolution.y;

  uv *= 3.*max((iMouse.xy/iResolution.xy).x, .1);
  vec2 t = vec2(iTime*0.01);
  float mt = 0.1;

  // Voronoi
  uv += hash2(uv)*.05;
  vec4 voro = voronoi(uv, t, mt);

  color += vec3(voro.z)+.1;

  // Distance
  color += vec3(voro.w);

  // Contour
  color.r *= (.5+.5*cos(voro.w*50.))*1.6;
  color.g *= (.5+.5*cos((voro.w+.01)*50.))*1.7;
  color.b *= (.5+.5*cos((voro.w+.02)*50.))*1.8;
  color = color*color+.15;

  // Edge
  color = mix(vec3(1.), color, smoothstep(.03, .06, voro.w));

  gl_FragColor = vec4(color, 1.);
}
