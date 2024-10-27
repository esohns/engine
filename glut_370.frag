//precision highp float;

#define MAX_PARTICLE_COUNT 90

uniform vec2 resolution;
uniform int particleCount;
uniform vec3 particles[MAX_PARTICLE_COUNT * 3];
uniform vec3 particleColors[MAX_PARTICLE_COUNT * 3];

void
main ()
{
  vec2 uv = gl_FragCoord.xy / resolution.xy;

  float r = 0.0;
  float g = 0.0;
  float b = 0.0;

  float mult = 0.0000006;

  for (int i = 0; i < MAX_PARTICLE_COUNT * 3; i++)
  {
    if (i < particleCount)
    {
      vec3 particle = particles[i];
      vec2 pos = particle.xy;
      float mass = particle.z;
      vec3 color = particleColors[i];

      r += color.r / distance (uv, pos) * mult * mass;
      g += color.g / distance (uv, pos) * mult * mass;
      b += color.b / distance (uv, pos) * mult * mass;
    }
  }

  vec4 finalColor = vec4 (r - 0.1, g - 0.1, b - 0.5, 1.0);
  gl_FragColor = finalColor;
}
