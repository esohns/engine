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
    v += r * r / (dx * dx + dy * dy);
  }

  gl_FragColor = vec4 (v, v * 2.0 - 1.0, v * 6.0 - 5.0 ,1.0);
}
