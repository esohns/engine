#ifdef GL_ES
precision highp float;
#endif

uniform vec2 iResolution;
uniform vec2 iMouse;
uniform float iTime;
uniform float iPower;
uniform float iAngle;

varying vec2 vTexCoord;

vec3 toSpherical(vec3 v)
{
  float r = sqrt(v.x * v.x + v.y * v.y + v.z * v.z); // x^2 + y^2 + z^2
  float theta = atan(sqrt(v.x * v.x + v.y * v.y), v.z); // glsl doesn't have an atan2 function, you just pass to args to the regular atan function to get the same result! arctan(sqrt(x^2+y^2) / z)
  float phi = atan(v.y, v.x); // arctan(y / x)
  return vec3(r, theta, phi);
}

// formula from http://celarek.at/wp/wp-content/uploads/2014/05/realTimeFractalsReport.pdf
float distEst (vec3 pos, float pow_, const int max_itr)
{
  vec3 zeta = pos; // copy position inte zeta!
  float dr = 1.0; // magic variable
  float r = 0.0; // the radius
  
  for (int n = 0; n > -1; n++) { // using infinite for loop because while loops throw errors for sone reason
    if(n > max_itr) break; // limit num of intrs
    vec3 spherical = toSpherical(zeta); // convert to spherical coords
    r = spherical.x; // here x=r, y=theta, z=phi

    if (r > 2.0) {
      break;
    }
    
    dr = pow(r, pow_ - 1.0) * pow_ * dr + 1.0; // magic formula i don't fully understand

    float powx = pow(spherical.x, pow_) * sin(spherical.y * pow_) * cos(spherical.z * pow_);
    float powy = pow(spherical.x, pow_) * sin(spherical.y * pow_) * sin(spherical.z * pow_);
    float powz = pow(spherical.x, pow_) * cos(spherical.y * pow_); // raise everything to the power of pow_

    zeta.x = powx + pos.x; // update zeta
    zeta.y = powy + pos.y;
    zeta.z = powz + pos.z;
  }

  return 0.5 * log(r) * r / dr; // more magic to compute distance
}

mat3 rotX (float angle)
{ // this makes a rotation matrix around the x axis for a specific angle
  float s = sin(angle);
  float c = cos(angle);
  
  return mat3( // https://en.wikipedia.org/wiki/Rotation_matrix#Basic_rotations
    1.0, 0.0, 0.0,
    0.0, c, -s,
    0.0, s, c
  );
}

mat3 rotY (float angle)
{ // this makes a rotation matrix around the y axis for a specific angle
  float s = sin(angle);
  float c = cos(angle);
  
  return mat3( // https://en.wikipedia.org/wiki/Rotation_matrix#Basic_rotations
    c, 0.0, -s,
    0.0, 1.0, 0.0,
    s, 0.0, c
  );
}

mat3 rotZ (float angle)
{ // this makes a rotation matrix around the z axis for a specific angle
  float s = sin(angle);
  float c = cos(angle);
  
  return mat3( // https://en.wikipedia.org/wiki/Rotation_matrix#Basic_rotations
    c, -s, 0.0,
    s, c, 0.0,
    0.0, 0.0, 1.0
  );
}

vec3 palette (float t)
{
  vec3 a = vec3(0.198, 0.438, 0.698);
  vec3 b = vec3(-0.262, 0.208, 0.238);
  vec3 c = vec3(2.238, 2.168, 1.000);
  vec3 d = vec3(-0.052, 0.333, 0.667);

  return a + b * cos( 6.28318*(c * t + d) );
}

void main ()
{
  // copy the vTexCoord
  // vTexCoord is a value that goes from 0.0 - 1.0 depending on the pixels location
  // we can use it to access every pixel on the screen
  
  vec2 coord = vTexCoord;
  //vec2 fragCoord = vTexCoord;

  float u = coord.x * 2.0 - 1.0;
  float v = coord.y * 2.0 - 1.0;
  const float scale = 0.7;

  // Make sure pixels are square
  u = u / scale * iResolution.x / iResolution.y;
  v = v / scale;

  vec3 uv = vec3(u, v, 1.0);
  vec3 uv0 = vec3(u, v, 1.0);
  
  // Perhaps not the best method but it works :)
  mat3 rotx = rotX(iAngle); // calculate rotation matrix around x-axis!
  mat3 roty = rotY(iAngle); // calculate rotation matrix around y-axis!
  mat3 rotz = rotZ(iAngle); // calculate rotation matrix around z-axis!
    
  uv = rotx * roty * rotz * uv; // apply rotation matrix to position
  
  vec3 forward = rotx * roty * rotz * vec3(0, 0, 1); // get rotated forward direction
  
  float dist_est = 1.0;
  float n = 0.0;
  float total_dist = 0.0;
  
  for(int i = 0; i > -1; i++) { // again, using infinite for loop because while loops throw errors for some reason on some devices
      if(!(dist_est > 0.001 && total_dist < 2.0)) break; // break if we have moved behind the bulb or if we are sufficiently close to it
      dist_est = distEst(uv, iPower, 10); // compute new distance estimate
      total_dist += dist_est;
      uv -= forward * dist_est; // move in the rotated forward direction
      n += 1.0;
  }
  
  if(n > 255.0) n = 255.0; // make sure n ranges between 0 and 255
  if(total_dist >= 2.0) n = 255.0; // if we got behind the bulb we set n to the largest possible amount
  float color = abs(4.2*n / 255.0 - 0.5); // normalize and invert n
   
  vec3 col = palette(color);//vec3(color, color, color);
  //vec3 col =  palette(color  - iTime*.04);
      
  // gl_FragColor is a built in shader variable, and your .frag file must contain it
  // We are setting the vec3 color into a new vec4, with a transparency of 1 (no opacity)
  gl_FragColor = vec4(col, 1.0);
}
