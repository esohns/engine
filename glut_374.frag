//precision highp float;
#version 330

uniform vec2 iResolution;
uniform vec2 iMouse;
uniform float iTime;
uniform int noctaves;

float
hash31 (vec3 p)
{
  float h = dot (p, vec3 (17, 1527, 113));
  return fract (sin (h) * 43758.5453123);
}

//iq noise and derivatives
float
noised (vec3 p)
{
  vec3 ip = floor(p);
  vec3 fp = fract(p);
    
  vec3 u = fp*fp*fp*(fp*(fp*6.0-15.0)+10.0);
  //vec3 du = 30.0*fp*fp*(fp*(fp-2.0)+1.0);

  float a = hash31( ip+vec3(0,0,0) );
  float b = hash31( ip+vec3(1,0,0) );
  float c = hash31( ip+vec3(0,1,0) );
  float d = hash31( ip+vec3(1,1,0) );
  float e = hash31( ip+vec3(0,0,1) );
  float f = hash31( ip+vec3(1,0,1) );
  float g = hash31( ip+vec3(0,1,1) );
  float h = hash31( ip+vec3(1,1,1) );

  float k0 =   a;
  float k1 =   b - a;
  float k2 =   c - a;
  float k3 =   e - a;
  float k4 =   a - b - c + d;
  float k5 =   a - c - e + g;
  float k6 =   a - b - e + f;
  float k7 = - a + b + c - d + e - f - g + h;

  return k0 + k1*u.x + k2*u.y + k3*u.z + k4*u.x*u.y + k5*u.y*u.z + k6*u.z*u.x + k7*u.x*u.y*u.z;
      /*vec4( (k0 + k1*u.x + k2*u.y + k3*u.z + k4*u.x*u.y + k5*u.y*u.z + k6*u.z*u.x + k7*u.x*u.y*u.z), 
                    2.0* du * vec3( k1 + k4*u.y + k6*u.z + k7*u.y*u.z,
                                    k2 + k5*u.z + k4*u.x + k7*u.z*u.x,
                                    k3 + k6*u.x + k5*u.y + k7*u.x*u.y ) );*/
}

float
noise3D (vec3 p)
{
  // Just some random figures, analogous to stride. You can change this, if you want.
  const vec3 s = vec3 (7, 157, 113);

  vec3 ip = floor(p); // Unique unit cell ID.
    
  // Setting up the stride vector for randomization and interpolation, kind of. 
  // All kinds of shortcuts are taken here. Refer to IQ's original formula.
  vec4 h = vec4(0., s.yz, s.y + s.z) + dot(ip, s);
    
  vec3 fp = p-ip; // Cell's fractional component.

  // A bit of quintic smoothing, to give the noise that rounded look.
  //fp = fp*fp*fp*(fp*(fp * 6. - 15.) + 10.);
  //or cubic
  fp=fp*fp*(3.0-2.0*fp); 
  // Standard 3D noise stuff. Retrieving 8 random scalar values for each cube corner,
  // then interpolating along X. There are countless ways to randomize, but this is
  // the way most are familar with: fract(sin(x)*largeNumber).
  h = mix(fract(sin(h)*43758.5453), fract(sin(h + s.x)*43758.5453), fp.x);

  // Interpolating along Y.
  h.xy = mix(h.xz, h.yw, fp.y);
    
  // Interpolating along Z, and returning the 3D noise value.
  return mix(h.x, h.y, fp.z); // Range: [0, 1].
}

const mat3 rot = mat3 (0.80, 0.60, 0.0, -0.60, 0.80, 0.0, 0.0, 0.0, 1.0);
float
fbm (vec3 _st, int noctaves)
{
  float v = 0.0;
  float a = 0.5;
  vec3 shift = vec3 (100.0);
  for (int i = 0; i < 20; ++i)
  {
    if (i >= noctaves)
      break;
#define iq 0
#if iq
    v += a * noised (_st);
#else
    v += a * noise3D (_st);
#endif
    _st = rot * _st *2.0 + shift;
    a *= 0.5;
  }

  return v;
}

float
map (vec3 p)
{
  // One layer of noise at isolevel "0.3" - I have no idea whether that's the
  // correct terminology. :)

  return fbm (p * 2.0, noctaves) - 0.4;
}

void
main ()
{
  // Screen coordinates.
  vec2 uv = (gl_FragCoord.xy - iResolution.xy * 0.5 ) / iResolution.y;

  // Unit direction ray. The last term is one of many ways to fish-lens the camera.
  // For a regular view, set "rd.z" to something like "0.5."
  vec3 rd = normalize (vec3 (uv, 0.5));//, (1.-dot(uv, uv)*.5)*.5)); // Fish lens, for that 1337, but tryhardish, demo look. :)

  // There are a few ways to hide artifacts and inconsistencies. Making things go fast is one of them. :)
  // Ray origin, scene color, and surface postion vector.
  vec3 ro = vec3 (0.0, iTime * 0.25, iTime * 0.25), col = vec3 (0.0), sp;

  // Swivel the unit ray to look around the scene.
  // Compact 2D rotation matrix, courtesy of Shadertoy user, "Fabrice Neyret."
  vec2 a = sin (vec2 (1.5707963, 0.0) + iTime * 0.175);
  rd.xz = mat2 (a, -a.y, a.x) * rd.xz;
  rd.xy = mat2 (a, -a.y, a.x) * rd.xy;

  // Ray distance, bail out layer number, surface distance and normalized accumulated distance.
  float t = 0.0, layers = 0.0, d, aD;

  t = 1.5; 

  // Surface distance threshold. Smaller numbers gives a thinner membrane, but lessens detail... 
  // hard to explain. It's easier to check it out for yourself.
  float thD = 0.02; // + smoothstep (-0.2, 0.2, sin (iTime * 0.75 - 3.14159 * 0.4)) * 0.025;

  // Only a few iterations seemed to be enough. Obviously, more looks better, but is slower.
  for (int i = 0; i < 56; i++)
  {
    // Break conditions. Anything that can help you bail early usually increases frame rate.
    if (layers > 25.0 || col.x > 1.0 || t > 20.0)
      break;

    // Current ray postion. Slightly redundant here, but sometimes you may wish to reuse
    // it during the accumulation stage.
    sp = ro + rd * t;
    d = map (sp); // Distance to nearest point on the noise surface.

    // If we get within a certain distance of the surface, accumulate some surface values.
    // Values further away have less influence on the total.
    //
    // aD - Accumulated distance. You could smoothly interpolate it, if you wanted.
    //
    // 1/.(1. + t*t*0.1) - Basic distance attenuation. Feel free to substitute your own.
        
    // Normalized distance from the surface threshold value to our current isosurface value.
    aD = (thD - abs (d) * 23.0 / 24.0) / thD;

    // If we're within the surface threshold, accumulate some color.
    // Two "if" statements in a shader loop makes me nervous. I don't suspect there'll be any
    // problems, but if there are, let us know.
    if (aD > 0.0)
    {
      // Add the accumulated surface distance value, along with some basic falloff (fog, if 
      // you prefer) using the camera to surface distance, "t." If you wanted, you could
      // add a light, and use the light position to ray position distance instead.
      // There's a bit of color jitter there, too.
            
      //col += aD*aD*(3.-2.*aD)/(1. + t*t*0.125)*.1 ;

      col += vec3 (aD / (1.0 + t * 0.15) * 0.07, aD / (1.0 + t * t * 0.1) * 0.07, aD / (1.0 + t * t * 0.1) * 0.1);

      // The layer number is worth noting. Accumulating more layers gives a bit more glow.
      // Lower layer numbers allow a quicker bailout. A lot of it is guess work.
      layers++;  
    }

    // Kind of weird the way this works. I think not allowing the ray to hone in properly is
    // the very thing that gives an even spread of values. The figures are based on a bit 
    // of knowledge versus trial and error. If you have a faster computer, feel free to tweak
    // them a bit.
    t += max (abs (d) * 0.7, thD * 0.7);
  }

  // Clamping "col" above zero, but just to be safe.
  col = max (col, 0.0);

  // Presenting the color to the screen. Note that there's no gamma correction. It was a style
  // choice - to give more contrast, but it's usually best to have it.
  gl_FragColor = vec4 (clamp (col, 0.0, 1.0), 1.0);
}
