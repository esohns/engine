#version 130

uniform vec2 iResolution;
uniform int iFrame;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

#define R    iResolution.xy
#define T(d) texelFetch(iChannel0, ivec2(d+U)%ivec2(R),0)

float _K0 = -20./6., // center weight
      _K1 =   4./6., // edge-neighbors
      _K2 =   1./6., // vertex-neighbors
       cs =  .25,    // curl scale
       ls =  .24,    // laplacian scale
       ps = -.06,    // laplacian of divergence scale
       ds = -.08,    // divergence scale
      pwr =  .2,     // power when deriving rotation angle from curl
      amp = 1.,      // self-amplification
      sq2 =  .7;     // diagonal weight

void
main ()
{
  vec4 O;
  vec2 U = gl_FragCoord.xy;

  vec4 uv = T(vec2( 0,  0 )),
        n = T(vec2( 0,  1 )),
        e = T(vec2( 1,  0 )),
        s = T(vec2( 0, -1 )),
        w = T(vec2(-1,  0 )),
        nw = T(vec2(-1,  1 )),
        sw = T(vec2(-1     )),
        ne = T(vec2( 1     )),
        se = T(vec2( 1, -1 ));

  vec4 lapl  = _K0*uv + _K1*(n + e + w + s) 
                      + _K2*(nw + sw + ne + se);
  float sp = ps * lapl.z;

  float curl = n.x - s.x - e.y + w.y + sq2 * (nw.x + nw.y + ne.x - ne.y + sw.y - sw.x - se.y - se.x);

  float a = cs * sign(curl) * pow(abs(curl), pwr);

  float div  = s.y - n.y - e.x + w.x + sq2 * (nw.x - nw.y - ne.x - ne.y + sw.x + sw.y + se.y - se.x);
  float sd = ds * div;

  vec2 norm = normalize(uv.xy);

  vec2 t = (amp * uv + ls * lapl + uv * sd).xy + norm * sp;
  t *= mat2(cos(a), -sin(a), sin(a), cos(a) );
  if(iFrame<10)
    O = -.5 + texture(iChannel1, U/R), O.a=0.;
  else 
    O = clamp(vec4(t,div,0), -1., 1.);

  gl_FragColor = O;
}
