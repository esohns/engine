uniform vec2 iResolution;
uniform float iTime;

#define MM 0

#define VARIANT 1
#if VARIANT
float ofs = .5;
#else
float ofs = 0.;
#endif

float RATIO = 1.,
      CRACK_depth = 3.,
      CRACK_zebra_scale = 1.,
      CRACK_zebra_amp = .67,
      CRACK_profile = 1.,
      CRACK_slope = 50.,
      CRACK_width = .0;

vec3
hash3 (uvec3 x)
{
#define scramble  x = ( (x>>8U) ^ x.yzx ) * 1103515245U
  scramble; scramble; scramble; 
  return vec3(x) / float(0xffffffffU) + 1e-30;
}

#define hash22(p)  fract( 18.5453 * sin( p * mat2(127.1,311.7,269.5,183.3)) )
#define disp(p) ( -ofs + (1.+2.*ofs) * hash22(p) )
vec3
voronoi (vec2 u)
{
  vec2 iu = floor(u), v;
  float m = 1e9,d;
#if VARIANT
  for( int k=0; k < 25; k++ )
  {
    vec2  p = iu + vec2(k%5-2,k/5-2),
#else
  for( int k=0; k < 9; k++ )
  {
    vec2  p = iu + vec2(k%3-1,k/3-1),
#endif
    o = disp(p),
    r = p - u + o;
    d = dot(r,r);
    if( d < m ) m = d, v = r;
  }

  return vec3( sqrt(m), v+u );
}

vec3
voronoiB (vec2 u)
{
  vec2 iu = floor(u), C, P;
  float m = 1e9,d;
#if VARIANT
  for( int k=0; k < 25; k++ )
  {
    vec2  p = iu + vec2(k%5-2,k/5-2),
#else
  for( int k=0; k < 9; k++ )
  {
    vec2  p = iu + vec2(k%3-1,k/3-1),
#endif
          o = disp(p),
          r = p - u + o;
    d = dot(r,r);
    if( d < m ) m = d, C = p-iu, P = r;
  }

  m = 1e9;

  for( int k=0; k < 25; k++ )
  {
    vec2 p = iu+C + vec2(k%5-2,k/5-2),
         o = disp(p),
         r = p-u + o;

    if( dot(P-r,P-r)>1e-5 )
      m = min( m, .5*dot( (P+r), normalize(r-P) ) );
  }

  return vec3( m, P+u );
}

#define rot(a) mat2(cos(a),-sin(a),sin(a),cos(a))
int MOD = 1;  // type of Perlin noise

#define hash21(p) fract(sin(dot(p,vec2(127.1,311.7)))*43758.5453123)
float
noise2_ (vec2 p)
{
  vec2 i = floor(p);
  vec2 f = fract(p); f = f*f*(3.-2.*f);

  float v = mix(mix(hash21(i+vec2(0,0)),hash21(i+vec2(1,0)),f.x),
                mix(hash21(i+vec2(0,1)),hash21(i+vec2(1,1)),f.x), f.y);
  return   MOD==0 ? v
                  : MOD==1 ? 2.*v-1.
                           : MOD==2 ? abs(2.*v-1.)
                                    : 1.-abs(2.*v-1.);
}

float
fbm2 (vec2 p)
{
  float v = 0.,  a = .5;
  mat2 R = rot(.37);

  for (int i = 0; i < 9; i++, p*=2.,a/=2.) 
    p *= R,
    v += a * noise2_(p);

  return v;
}

#define noise2_2(p) vec2(noise2_(p),noise2_(p+17.7))
vec2
fbm22 (vec2 p)
{
  vec2 v = vec2(0);
  float a = .5;
  mat2 R = rot(.37);

  for (int i = 0; i < 6; i++, p*=2.,a/=2.) 
    p *= R,
    v += a * noise2_2(p);

  return v;
}

vec2
mfbm22 (vec2 p)
{
  vec2 v = vec2(1);
  float a = .5;
  mat2 R = rot(.37);

  for (int i = 0; i < 6; i++, p*=2.,a/=2.) 
    p *= R,
    v += v * a * noise2_2(p);

  return v-1.;
}

void
main ()
{
  vec4 O = vec4(0);
  vec2 U = gl_FragCoord.xy;
  U *= 4./iResolution.y;
  U.x += iTime;
  vec2 I = floor(U/2.); 
  bool vert = mod(I.x+I.y,2.)==0.;
  vec3 H0;

  for(float i=0.; i<CRACK_depth ; i++)
  {
    vec2 V =  U / vec2(RATIO,1),
         D = CRACK_zebra_amp * fbm22(U/CRACK_zebra_scale) * CRACK_zebra_scale;
    vec3  H = voronoiB( V + D ); if (i==0.) H0=H;
    float d = H.x;
    d = min( 1., CRACK_slope * pow(max(0.,d-CRACK_width),CRACK_profile) );
  
    O += vec4(1.-d) / exp2(i);
    U *= 1.5 * rot(.37);
  }
  if (vert)
    O = 1.-O;
  O *= vec4(.9,.85,.85,1);

#if MM
  O.g = hash3(uvec3(H0.yz,1)).x;
#endif

  gl_FragColor = O;
}
