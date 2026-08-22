uniform vec2 iResolution;
uniform float iTime;

#define T iTime
#define P(z) (vec3(cos((z) * .02) *20., cos((z) * .05) * 3.+.0, (z)))
#define R(a) mat2(cos(a+vec4(0,33,11,0)))
mat3 G = mat3(-.57,.81,.1, -.28,-.3,.9, .77, .49, .4);
#define n(p) dot( cos(G*(p)), sin(1.6*(p)*G) )

vec3 c (float t)
{
  return clamp(vec3(0.455, 0.322, 0.216) + vec3(-0.073, 0.119, 0.150) * cos(6.28318 * (vec3(20.000, 1.000, 1.000) * t + vec3(0.100, -0.256, -0.231))), 0.0, 1.0); // sRGB
}

void
main ()
{
  float i,y,d,s;

  vec3  g,
        p = P(T*1e1),
        Z = normalize( P(T*1e1+1.) - p), 
        X = normalize(vec3(Z.z,0,-Z)),
        rd = vec3((gl_FragCoord.xy - 0.5 * iResolution.xy) / iResolution.y, 2) * mat3(-X, cross(X, Z), Z);

  vec4 o;

  for (;i++ < 150.;)
  {
    s = 0.001 + abs(s) * 0.1;
    d += s;
    o.rgb += c(p.z*.1+i*2.) / s ;
    p = rd*d+P(T*1e1);
    g = p;
    p.xy *= R(length(P(T).xy)*.05 );
    s =(sin(p.z + (p.y)) * 0.1)-.2;
    y = abs(n(p)+n(p/8.)*4.);
    s += y+y*.2;
    s = max(1.5+sin(p.z*.2+p.y*.4)-length((g-P(p.z)).xy), s);
  }

  gl_FragColor = tanh(o * o /2e8)*1.5;
}
