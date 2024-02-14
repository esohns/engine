//precision highp float;

varying vec2 vTexCoord;

const int number_of_balls = 50;
uniform vec3 balls[number_of_balls];

const float WIDTH = 1112.0;
const float HEIGHT = 834.0;

void main ()
{
  float x = vTexCoord.x * WIDTH;
  float y = HEIGHT - vTexCoord.y * HEIGHT;
  float v = 0.0;

  for (int i = 0; i < number_of_balls; i++)
  {
    vec3 b = balls[i];
    float dx = b.x - x;
    float dy = b.y - y;
    float r = b.z;
    v += r * r / pow (pow (dx, 8.0) + pow (dy, 8.0), 2.0 / 8.0); // *TODO*: why doesn't this work ?
//    v += r * r / (dx * dx + dy * dy); // *NOTE*: this works just fine
  }

  float p = v;
  float s = v * 2.0 - 1.0;
  float t = v * 6.0 - 5.0;
  gl_FragColor = vec4 (t, s, p, 1.0);
}
