#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

const int max_iterations = 45;
const float stop_threshold = 0.001;
const float grad_step = 0.001;
const float clip_far = 4.0;
const float PI = 3.14159265359;

const float furDepth = 0.5;
const int furLayers = 50;
const float rayStep = furDepth * 2.0 / float(furLayers);
const float furThreshold = 0.4;
const float uvScale = 0.5;

float r = 1.;
float rotation;
float elevation;

mat3
rotX (float g)
{
  g = radians(g);
  vec2 a = vec2(cos(g), sin(g));
  return mat3(1.0, 0.0, 0.0,
              0.0, a.x, -a.y,
              0.0, a.y, a.x);
}
  
mat3
rotY (float g)
{
  g = radians(g);
  vec2 a = vec2(cos(g), sin(g));
  return mat3(a.x, 0.0, a.y,
              0.0, 1.0, 0.0,
             -a.y, 0.0, a.x);
}

vec3
map (vec3 p)
{
  p.y -= elevation;
  p *= rotY(rotation);
  float s = (length(p) - r) - 0.12 * sin(6. * p.x) * cos(6. * p.y)* sin(6. * p.z);
  return vec3(s * 0.5);	
}

vec2
cartesianToSpherical (vec3 p)
{
  float r = length(p);
  p = normalize(p);
  float t = (r - (1.0 - furDepth)) / furDepth;
  p.zyx *= rotX(rotation);
  vec2 uv = vec2(atan(p.y, p.x), acos(p.z));

  uv.y -= t * t * elevation;

  return uv;
}

float
furDensity (vec3 pos)
{
  vec2 uv = cartesianToSpherical(pos.xzy);	
  vec4 tex = textureLod(iChannel0, uv * uvScale, 0.0);

  float density = smoothstep(furThreshold, 1.0, tex.x);

  pos.y -= elevation;
  vec3 p = pos;
  p *= rotY(rotation);
  float r = length(pos) - 0.12 * sin(6. * p.x) * cos(6. * p.y)* sin(6. * p.z);;
  float t = (r - (1. - furDepth)) / furDepth;

  return density * (1. - t);
    
}

vec3
furNormal (vec3 pos, float density)
{
  float eps = 0.0001;
  vec3 n;
  n.x = furDensity( vec3(pos.x + eps, pos.y, pos.z)) - density;
  n.y = furDensity( vec3(pos.x, pos.y + eps, pos.z)) - density;
  n.z = furDensity( vec3(pos.x, pos.y, pos.z + eps)) - density;
  return normalize(n);
}

vec3
furShade (vec3 pos, vec3 color, vec3 light, vec3 eye, float density)
{
  vec3 v = normalize(light - pos);
  vec3 n = furNormal(pos, density);
  vec3 ev = normalize(pos - eye);
  vec3 h = reflect(ev, n);

  float diff = max(0.0, dot(v, n)) + 0.4;
  float spec = pow(max(0.0, dot(v, h)), 64.);

  float r = length(pos);
  float t = (r - (1.0 - furDepth)) / furDepth;
  t = clamp(t, 0.3, 1.);

  diff = mix(diff, 1., 0.5);

  return color * t * (diff + 1.9 * spec);
}
  
vec3
gradient (vec3 v)
{
  const vec3 delta = vec3( grad_step, 0.0, 0.0 );
  float va = map( v ).x;
  return normalize (
    vec3(
      map( v + delta.xyy).x - va,
      map( v + delta.yxy).x - va,
      map( v + delta.yyx).x - va			
    )
  );
}

vec3
ray_marching (vec3 origin, vec3 dir, float start, float end)
{
  float depth = start;
  vec3 salida = vec3(end);
  vec3 dist = vec3(0.1);

  for ( int i = 0; i < max_iterations; i++ )
  {
    if ( dist.x < stop_threshold || depth > end )
      break;
    dist = map( origin + dir * depth );
    depth += dist.x;
  }

  salida = vec3(depth, dist.y, dist.z);
  return salida;
}

vec3
shading (vec3 v, vec3 n, vec3 eye, vec3 lightMix)
{
  vec3 final = vec3( 0.0 );
  vec3 ev = normalize( v - eye );
  vec3 ref_ev = reflect( ev, n );
  vec3 light_pos   = vec3(0.0, 2.0, -2.0);
  vec3 vl = normalize( light_pos - v );
  float diffuse  = max( 0.0, dot( vl, n ) );
  float specular = max( 0.0, dot( vl, ref_ev ) );
  specular = pow( specular, lightMix.x );
  final += vec3( 0.9 ) * ( diffuse * lightMix.y + specular * lightMix.z);	
  final += vec3(0.2);

  return final;
}

vec3
ray_dir (float fov, vec2 size, vec2 pos)
{
  vec2 xy = pos - size * 0.5;

  float cot_half_fov = tan(radians( 90.0 - fov * 0.5 ));	
  float z = size.y * 0.5 * cot_half_fov;

  return normalize( vec3( xy, z ) );
}

void
main ()
{
  rotation = 60. * (iTime);
  elevation = 0.25 * cos(iTime * 2.5);

  vec3 rd = ray_dir(60.0, iResolution, gl_FragCoord.xy );

  vec3 eye = vec3( .0, .0, -2.8 );

  vec3 lightMix = vec3(6., 0.45, 0.15);
  vec3 color = vec3(0.);

  vec3 data = ray_marching( eye, rd, 0.0, clip_far );
  if ( data.x < clip_far )
  {
    vec3 pos = eye + rd * data.x;
    vec3 n = gradient( pos );
      
    vec3 lightColor =  shading( pos, n, eye, lightMix) * 2.0;			
    color = vec3(1.) * lightColor;
        
    vec4 c = vec4(0.0);
      
    for(int i=0; i<furLayers; i++)
    {
      vec4 sampleCol;
      sampleCol.a = furDensity(pos);
      if( c.a > 0.99 )
        break;
      if (sampleCol.a > 0.)
      {
        sampleCol.rgb = sampleCol.a * furShade(pos, vec3(85., 72., 56) * lightColor / 255., vec3(0.0, 2.0, -2.0), eye, sampleCol.a);
        c = c + sampleCol * (1.0 - c.a);
      }
      pos += rd * rayStep;
    }

    color = c.xyz;
  }

  gl_FragColor = vec4(2. * color, 1.0 );
}
