//precision highp float;

varying vec2 vTexCoord;

const int number_of_balls = 20;
uniform vec3 metaballs[number_of_balls];

const float WIDTH = 1280.0;
const float HEIGHT = 800.0;

vec3 hsv2rgb (vec3 c)
{
    vec4 K = vec4 (1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs (fract (c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix (K.xxx, clamp (p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
  float x = vTexCoord.x * WIDTH;
  float y = vTexCoord.y * HEIGHT;
  float v = 0.0;

  for (int i = 0; i < number_of_balls; i++)
  {
    vec3 ball = metaballs[i];
    float dx = ball.x - x;
    float dy = ball.y - y;
    float r = ball.z;
    v += r * r / (dx * dx + dy * dy);
  }

  if (0.9 < v && v < 1.1)
  {
    float a = (v - 0.9) * 4.;
    gl_FragColor = vec4 (hsv2rgb (vec3 (a, 1., 1.)), 1.0);
  }
  else
    gl_FragColor = vec4 (0.0, 0.0, 0.0, 1.0);
}
