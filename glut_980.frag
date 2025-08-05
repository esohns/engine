uniform vec2 iResolution;
uniform float iTime;

#define AA 2

const float zoom_factor = 125.0;//50.0
const float speed_factor = 0.08;//0.06

float
calc (vec2 p, float time)
{
  float ltime = 0.5-0.5*cos(time*speed_factor);
  float zoom = pow( 0.9, zoom_factor*ltime );
  vec2  cen = vec2( 0.2655,0.301 ) + zoom*0.8*cos(4.0+2.0*ltime);

  vec2 c = vec2( -0.745, 0.186 ) - 0.045*zoom*(1.0-ltime*0.5);

  p = (2.0 * p - iResolution)/(iResolution.x / 2.0);
  vec2 z = cen + (p-cen)*zoom;

#if 0
  vec2 dz = vec2( 1.0, 0.0 );
  for( int i=0; i<256; i++ )
  {
    dz = 2.0*vec2(z.x*dz.x-z.y*dz.y, z.x*dz.y + z.y*dz.x );
    z = vec2( z.x*z.x - z.y*z.y, 2.0*z.x*z.y ) + c;
    if (dot(z,z)>200.0)
      break;
  }
  float d = sqrt( dot(z,z)/dot(dz,dz) )*log(dot(z,z));
#else
  float ld2 = 1.0;
  float lz2 = dot(z,z);
  for( int i=0; i<256; i++ )
  {
    ld2 *= 4.0*lz2;
    z = vec2( z.x*z.x - z.y*z.y, 2.0*z.x*z.y ) + c;
    lz2 = dot(z,z);
    if (lz2>200.0)
      break;
  }
  float d = sqrt(lz2/ld2)*log(lz2);
#endif

  return sqrt( clamp( (150.0/zoom)*d, 0.0, 1.0 ) );
}

void
main ()
{
#if 1
  float scol = calc( gl_FragCoord.xy, iTime );
 #else
  float scol = 0.0;
  for( int j=0; j<AA; j++ )
  for( int i=0; i<AA; i++ )
  {
    vec2 of = -0.5 + vec2( float(i), float(j) )/float(AA);
    scol += calc( gl_FragCoord.xy+of, iTime );
  }
  scol /= float(AA*AA);
#endif

  vec3 vcol = pow( vec3(scol), vec3(0.9,1.1,1.4) );

  vec2 uv = gl_FragCoord.xy/iResolution;
  vcol *= 0.7 + 0.3*pow(16.0*uv.x*uv.y*(1.0-uv.x)*(1.0-uv.y),0.25);

  gl_FragColor = vec4( vcol, 1.0 );
}
