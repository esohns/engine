#version 130

// glut_852_common.glsl
//#define VORONOI
//#define TRACKGRID

#define smoothfac 0.9
#define swirlrad 200.
#define swirlstrength 1.5
#define wallsqueeze 5.
#define colormagnitude 16.
#define fakeior 4.
#define hashEdge 2
#define diameter 64.
#define particleEdge ivec2(floor(iResolution/diameter))
#define stiffness 0.05
#define damping 0.1
#define gravity 0.05

vec2
noise2D (vec2 p)
{
  vec3 p3 = fract(vec3(p.xyx) * vec3(.1031, .1030, .0973));
  p3 += dot(p3, p3.yzx+33.33);
  return fract((p3.xx+p3.yz)*p3.zy);
}
// glut_852_common.glsl

uniform vec2 iResolution;
uniform int iFrame;
uniform sampler2D iChannel0;

void
main ()
{
  vec4 state;

  ivec2 maxIndex = particleEdge * hashEdge;
  ivec2 ind = ivec2(floor(gl_FragCoord.xy));

  if (ind.x >= maxIndex.x || ind.y >= maxIndex.y)
    discard;

  ivec2 id = ind/hashEdge;
  ivec2 hashOffset = ind % hashEdge;
  int hashId = hashOffset.x + hashEdge * hashOffset.y;

  state = vec4(-1., -1., 0., 0.);

  if (iFrame == 0)
  {
    if (hashOffset == ivec2(0) && ind.x + ind.y > particleEdge.x)
      state = vec4(vec2(id)*diameter+diameter*.5, 0.5 * sin(float(id.y)), 0.);
  }
  else
  {
    int entranceCount = 0;

    for (int i=-1; i<=1; i++)
      for (int j=-1; j<=1; j++)
      {
          ivec2 disp = ivec2(i, j);
          ivec2 otherid = id + disp;
          if (otherid.x < 0 || otherid.y < 0 || otherid.x >= particleEdge.x || otherid.y >= particleEdge.y)
            continue;
          for (int k=0; k<hashEdge; k++)
          {
            for (int l=0; l<hashEdge; l++)
            {
                ivec2 offset = ivec2(k, l);
                vec4 otherstate = texelFetch(iChannel0, otherid*hashEdge + offset, 0);
                ivec2 id2 = ivec2(floor(otherstate.xy/diameter));
                if (id2 == id)
                {
                  if (entranceCount == hashId)
                  {
                    state = otherstate;
                    gl_FragColor = state;
                    return;
                  }

                  ++entranceCount;
                }
            }
          }
      }
  }

  gl_FragColor = state;
}
