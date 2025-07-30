#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

//#define FIRST_PERSON

vec2
Rand (vec2 pos)
{
  vec2 texSize = vec2(textureSize(iChannel0, 0));

  return textureLod(iChannel0, (pos + .5) / texSize, 0.0).xz;
}

vec3
VoronoiPoint (vec2 pos, vec2 delta)
{
  const float randScale = .5; 

  vec2 p = floor(pos)+delta;
  vec2 r = (Rand(p)-.5)*randScale;
  vec2 c = p+.5+r;

  float l = abs(c.x-pos.x)+abs(c.y-pos.y);

  return vec3(c,l);
}

vec3
Voronoi (vec2 pos)
{
  vec3 delta = vec3(-1,0,1);

  vec3 point_[9];
  point_[0] = VoronoiPoint( pos, delta.xx );
  point_[1] = VoronoiPoint( pos, delta.yx );
  point_[2] = VoronoiPoint( pos, delta.zx );
  point_[3] = VoronoiPoint( pos, delta.xy );
  point_[4] = VoronoiPoint( pos, delta.yy );
  point_[5] = VoronoiPoint( pos, delta.zy );
  point_[6] = VoronoiPoint( pos, delta.xz );
  point_[7] = VoronoiPoint( pos, delta.yz );
  point_[8] = VoronoiPoint( pos, delta.zz );

  vec3 closest;
  closest.z =
    min(
      min(
        min(
          min( point_[0].z, point_[1].z ),
          min( point_[2].z, point_[3].z )
        ), min(
          min( point_[4].z, point_[5].z ),
          min( point_[6].z, point_[7].z )
        )
      ), point_[8].z
    );

  closest.xy = point_[8].xy;
  for ( int i=0; i < 8; i++ )
  {
    if ( closest.z == point_[i].z )
    {
      closest = point_[i];
      point_[i] = point_[8];
    }
  }

  float t;
  t = min(
      min(
        min( point_[0].z, point_[1].z ),
        min( point_[2].z, point_[3].z )
      ), min(
        min( point_[4].z, point_[5].z ),
        min( point_[6].z, point_[7].z )
      )
    );

  return vec3( closest.xy, t-closest.z );
}

float
DistanceField (vec3 pos, float dist)
{
  vec3 v = Voronoi(pos.xz);
  vec2 r = Rand(v.xy*4.0);

  float f = (.2+.3*r.y-v.z)*.5;

  float h = r.x;
  h = mix(.2,2.0,pow(h,2.0));
  h = pos.y-h;

  h = max( min( h, .008*dist ), pos.y-2.0 );

  if ( f > 0.0 && h > 0.0 )
    f = sqrt(f*f+h*h);
  else
    f = max(f,h);

  f = min( f, pos.y );

  return f;
}

float
DistanceField (vec3 pos)
{
  return DistanceField( pos, 10.0 );
}

vec3
GetNormal (vec3 pos)
{
  vec3 n;
  vec2 delta = vec2(0,1);

#ifdef FIRST_PERSON
  delta *= .004;
#else
  delta *= .04;
#endif

  n.x = DistanceField( pos+delta.yxx ) - DistanceField( pos-delta.yxx );
  n.y = DistanceField( pos+delta.xyx ) - DistanceField( pos-delta.xyx );
  n.z = DistanceField( pos+delta.xxy ) - DistanceField( pos-delta.xxy );

  if ( dot(n,n) == 0.0 )
    n.y += 1.0;

  return normalize(n);
}

void
main ()
{
  vec2 mouse = iMouse.xy/iResolution;

  float h;

  vec2 rot = vec2(-.2+(iTime-sin(iTime/10.)*10.)*.03,.28+.1*(1.-cos(iTime/10.)))+vec2(1.6,.5)*mouse;

  float viewSize = 20.;
  vec3 rayStart = vec3( viewSize*(gl_FragCoord.xy-iResolution*.5)/iResolution.x, 0 );
  rayStart.yz = rayStart.yz*cos(rot.y)+rayStart.zy*sin(rot.y)*vec2(-1,1);
  rayStart.xz = rayStart.xz*cos(rot.x)+rayStart.zx*sin(rot.x)*vec2(1,-1);

  rayStart += vec3(0,8,0) + vec3(1,0,1)*iTime + vec3(0,6,0)*mouse.y;
  rayStart += -20.0*vec3(sin(rot.x),0,cos(rot.x));

  vec3 rayDir = vec3(sin(rot.x),0,cos(rot.x))*cos(rot.y)+vec3(0,-sin(rot.y),0);

  float t = 0.0;
  h = 1.0;
  for ( int i=0; i < 300; i++ )
  {
    if ( h < .001 )
      break;

    h = DistanceField(rayStart+rayDir*t, t);
    t += h;
  }

  vec3 pos = rayStart+rayDir*t;
  vec3 norm = GetNormal(pos);
  vec3 v = Voronoi(pos.xz);

  vec2 r = Rand( v.xy ).xy;
  vec4 albedo = mix( mix( vec4(.4,.2,.0,0), vec4(1,1,1,0), r.x ),
                     mix( vec4(0.,.2,.6,1), vec4(0,0,0,1), r.x ),
                     r.y );

  if ( fract(pos.y*8.0) < .4 )
    albedo = mix( vec4(0,0,0,0), vec4(1,1,1,0), r.x );

  albedo.w = mix ( albedo.w, 0.0, step( .2, norm.y ) );

  albedo = mix( vec4(.05,.05,.05,0), albedo, step( .07, abs(v.z-.08) ) );

  vec3 lighting = max(0.0,dot(norm,normalize(vec3(-2,3,-1))))*vec3(1,.9,.8);
  lighting += vec3(.2);

  vec3 result = albedo.xyz * lighting;

  float fresnel = pow(1.0+dot(norm,rayDir),5.0);
  if ( fresnel > 1.0 ) fresnel = 0.0;
  fresnel = mix( .2, 1.0, fresnel );

  vec3 reflection = texture( iChannel1, reflect(rayDir,norm).xz/8.0 ).rgb*3.0;

  result = mix( result, reflection, fresnel*albedo.w );

  if ( h > .01 )
    result = mix( vec3(.3), vec3(.85,1.0,1.2), smoothstep( -.1,.1,rayDir.y ) );

  result *= mix( .2, 1.0, smoothstep( 0.0, .7, pos.y ) );

  result *= pow( vec3(.7,.57,.3), vec3(t*.02) );
  result += vec3(.7,.9,1.2)*(1.0-exp(-t*.02));

  gl_FragColor = vec4(result,1);
}
