uniform vec2 iResolution;
uniform float iTime;

#define RAYCASTSTEPS 30

#define GRIDSIZE 10.
#define GRIDSIZESMALL 7.
#define MAXHEIGHT 30.
#define SPEED 20.
#define FPS 30.
#define MAXDISTANCE 260.
#define MAXSHADOWDISTANCE 20.

#define time iTime

#define HASHSCALE1 .1031
#define HASHSCALE3 vec3(.1031, .1030, .0973)
#define HASHSCALE4 vec4(1031, .1030, .0973, .1099)

float
hash12 (vec2 p)
{
  vec3 p3  = fract(vec3(p.xyx) * HASHSCALE1);
  p3 += dot(p3, p3.yzx + 19.19);
  return fract((p3.x + p3.y) * p3.z);
}

vec2
hash22 (vec2 p)
{
  vec3 p3 = fract(vec3(p.xyx) * HASHSCALE3);
  p3 += dot(p3, p3.yzx+19.19);
  return fract(vec2((p3.x + p3.y)*p3.z, (p3.x+p3.z)*p3.y));
}

bool
intersectPlane (vec3 ro, vec3 rd, float height, out float dist)
{
  if (rd.y==0.0)
  {
    return false;
  }

  float d = -(ro.y - height)/rd.y;
  d = min(100000.0, d);
  if ( d > 0. )
  {
    dist = d;
    return true;
  }

  return false;
}

vec2
iSphere (vec3 ro, vec3 rd, vec4 sp, vec3 ve, out vec3 nor)
{
  float t = -1.0;
  float s = 0.0;
  nor = vec3(0.0);

  vec3  rc = ro - sp.xyz;
  float A = dot(rc,rd);
  float B = dot(rc,rc) - sp.w*sp.w;
  float C = dot(ve,ve);
  float D = dot(rc,ve);
  float E = dot(rd,ve);
  float aab = A*A - B;
  float eec = E*E - C;
  float aed = A*E - D;
  float k = aed*aed - eec*aab;

  if( k>0.0 )
  {
    k = sqrt(k);
    float hb = (aed - k)/eec;
    float ha = (aed + k)/eec;

    float ta = max( 0.0, ha );
    float tb = min( 1.0, hb );

    if( ta < tb )
    {
      ta = 0.5*(ta+tb);			
      t = -(A-E*ta) - sqrt( (A-E*ta)*(A-E*ta) - (B+C*ta*ta-2.0*D*ta) );
      nor = normalize( (ro+rd*t) - (sp.xyz+ta*ve ) );
      s = 2.0*(tb - ta);
    }
  }

  return vec2(t,s);
}

vec3  lig = normalize( vec3(-0.6, 0.7, -0.5) );
vec3
shade (float d, vec3 col, float shadow, vec3 nor, vec3 ref, vec3 sky)
{
  float amb = max(0., 0.5+0.5*nor.y);
  float dif = max(0., dot( normalize(nor), lig ) );
  float spe = pow(clamp( dot(normalize(ref), lig ), 0.0, 1.0 ),16.0);

  dif *= shadow;

  vec3 lin = 1.20*dif*vec3(1.00,0.85,0.55);
  lin += 0.50*amb*vec3(0.50,0.70,1.00);
  col = col*lin;
  col += spe*dif;

  col = mix( col, sky, smoothstep( MAXDISTANCE * .8, MAXDISTANCE, d ) );

  return col;
}

void
getSphereOffset (vec2 grid, inout vec2 center)
{
  center = (hash22( grid ) - vec2(0.5) )*(GRIDSIZESMALL);
}

void
getMovingSpherePosition (vec2 grid, vec2 sphereOffset, inout vec4 center, inout vec3 speed)
{
  float s = 0.1+hash12( grid );
  float t = fract(14.*s + time/s*.3);	
  float y =  s * MAXHEIGHT * abs( 4.*t*(1.-t) );

  speed = vec3(0, s * MAXHEIGHT * ( 8.*t - 4. ), 0 ) * (1./FPS);

  vec2 offset = grid + sphereOffset;

  center = vec4(  offset.x + 0.5*GRIDSIZE, 1. + y, offset.y + 0.5*GRIDSIZE, 1. );
}

void
getSpherePosition (vec2 grid, vec2 sphereOffset, inout vec4 center)
{
  vec2 offset = grid + sphereOffset;
  center = vec4( offset.x + 0.5*GRIDSIZE, 1., offset.y + 0.5*GRIDSIZE, 1. );
}

vec3
getSphereColor (vec2 grid)
{
  float m = hash12( grid.yx ) * 12.;
  return vec3(1.-m*0.08, m*0.03, m*0.06);
}

vec3
render (vec3 ro, vec3 rd, vec3 cameraSpeed, mat3 rot)
{
  vec3 nor, ref, speed;

  float dist = MAXDISTANCE;

  vec3 sky = clamp( vec3(1,1.5,2.5)*(1.0-0.8*rd.y), vec3(0.), vec3(1.));
  vec3 colBackground, sphereSpeed, col = vec3(0.);

  vec4 sphereCenter;    
  vec3 pos = floor(ro/GRIDSIZE)*GRIDSIZE;
  vec2 offset;

  if (intersectPlane( ro,  rd, 0., dist))
  {
    vec3 interSectionPoint = ro + rd * dist;

    speed = rot * (interSectionPoint.xyz - ro) + cameraSpeed;   

    vec2 c1 = mod(interSectionPoint.xz * .25, vec2(2.));

    float w = (abs( fract(c1.x*abs(rd.x)) -.5 ) + abs( fract(c1.y*abs(rd.y)) -.5 ));        

    colBackground = mix(mod(floor(c1.x) + floor(c1.y), 2.) < 1. ? vec3( 0.4 ) : vec3( .6 ),
                        vec3(.5), clamp( (w + .8) * .007 * length(speed.xz) * FPS , 0., 1.));

    float shadow = 0.;
    vec3 shadowStartPos = interSectionPoint - lig;
    vec2 shadowGridPos = floor((ro + rd * dist).xz/GRIDSIZE);
    for( float x=-1.; x<=1.; x++)
    {
      for( float y=-1.; y<=1.; y++)
      {
        vec2 gridpos = (shadowGridPos+vec2(x,y))*GRIDSIZE;
        getSphereOffset( gridpos, offset );

        getMovingSpherePosition( gridpos, -offset, sphereCenter, sphereSpeed );

        vec2 res = iSphere( shadowStartPos, lig, sphereCenter, sphereSpeed + cameraSpeed, nor );
        if( res.x>0.0 )
        {
          shadow = clamp( shadow+mix(res.y,0., res.x/MAXSHADOWDISTANCE), 0., 1.);
        }

        getSpherePosition( gridpos, offset, sphereCenter );

        res = iSphere( shadowStartPos, lig, sphereCenter, cameraSpeed, nor );
        if( res.x>0.0 )
        {            
          shadow = clamp( shadow+mix(res.y,0., res.x/MAXSHADOWDISTANCE), 0., 1.);
        }
      }
    }

    ref = reflect( rd, vec3( 0., 1., 0. ) );
    colBackground = shade( dist, colBackground, 1.-shadow, vec3( 0., 1., 0. ), ref, sky );            
  }
  else
  {
    colBackground = sky;
  }

  vec3 ri = 1.0/rd;
  vec3 rs = sign(rd) * GRIDSIZE;
  vec3 dis = (pos-ro + 0.5  * GRIDSIZE + rs*0.5) * ri;
  vec3 mm = vec3(0.0);

  float alpha = 1.;

  for( int i=0; i<RAYCASTSTEPS; i++ )
  {
    if( alpha < .01 )
      break;

    getSphereOffset( pos.xz, offset );

    getMovingSpherePosition( pos.xz, -offset, sphereCenter, sphereSpeed );

    speed = rot * (sphereCenter.xyz - ro) + sphereSpeed + cameraSpeed;
    vec2 res = iSphere( ro, rd, sphereCenter, speed, nor );
    if( res.x>0.0 )
    {            
      ref = reflect( rd, nor );
      vec3  lcol = shade( res.x, getSphereColor(-offset), 1., nor, ref, sky);
      col += lcol * res.y * alpha;
      alpha *= (1.-res.y);
    }

    getSpherePosition( pos.xz, offset, sphereCenter );

    speed = rot * (sphereCenter.xyz - ro) + cameraSpeed;        
    res = iSphere( ro, rd, sphereCenter, speed, nor );
    if( res.x>0.0 )
    {            
      ref = reflect( rd, nor );
      vec3  lcol = shade( res.x, getSphereColor(-offset), 1., nor, ref, sky);
      col += lcol * res.y * alpha;
      alpha *= (1.-res.y);
    }

    mm = step(dis.xyz, dis.zyx);
    dis += mm * rs * ri;
    pos += mm * rs;		
  }	

  col += colBackground * alpha;

  return col;
}

void
path (float time, out vec3 ro, out vec3 ta)
{
  ro = vec3( 16.0*cos(0.2+0.5*.4*time*1.5) * SPEED, 5.6+3.*sin(time), 16.0*sin(0.1+0.5*0.11*time*1.5) * SPEED);
  time += 1.6;
  ta = vec3( 16.0*cos(0.2+0.5*.4*time*1.5) * SPEED, -.1 + 2.*sin(time), 16.0*sin(0.1+0.5*0.11*time*1.5) * SPEED);
}

mat3
setCamera (float time, out vec3 ro)
{
  vec3 ta;

  path(time, ro, ta);
  float roll = -0.15*sin(.732*time);

  vec3 cw = normalize(ta-ro);
  vec3 cp = vec3(sin(roll), cos(roll), 0.);
  vec3 cu = normalize( cross(cw,cp) );
  vec3 cv = normalize( cross(cu,cw) );
  return mat3( cu, cv, cw );
}

void
main ()
{
  vec2 q = gl_FragCoord.xy/iResolution.xy;
  vec2 p = -1.0+2.0*q;
  p.x *= iResolution.x/iResolution.y;

  vec3 ro0, ro1, ta;

  mat3 ca0 = setCamera( time - 1./FPS, ro0 );
  vec3 rd0 = ca0 * normalize( vec3(p.xy,2.0) );

  mat3 ca1 = setCamera( time, ro1 );
  vec3 rd1 = ca1 * normalize( vec3(p.xy,2.0) );

  mat3 rot = ca1 * mat3( ca0[0].x, ca0[1].x, ca0[2].x,
                         ca0[0].y, ca0[1].y, ca0[2].y,
                         ca0[0].z, ca0[1].z, ca0[2].z);

  rot -= mat3( 1,0,0, 0,1,0, 0,0,1);

  vec3 col = render(ro0, rd0, ro1-ro0, rot );

  col = pow( col, vec3(0.5) );

  gl_FragColor = vec4 (col,1.0);
}
