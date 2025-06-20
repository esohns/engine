#version 130

uniform vec2 iResolution;
uniform float iTime;

#define BRIGHTNESS 1.0
#define COLOR_BASE 1.5
#define COLOR_SPEED 0.5
#define RGB vec3(0.0, 1.0, 2.0)
#define COLOR_WAVE 14.0
#define COLOR_DOT vec3(1,-1,0)

#define WAVE_STEPS 8
#define WAVE_FREQ 5.0
#define WAVE_AMP 0.6
#define WAVE_EXP 1.8
#define WAVE_VELOCITY vec3(0.2)

#define PASSTHROUGH 0.2
#define SOFTNESS 0.005
#define STEPS 100
#define SKY 10.0
#define FOV 1.0

void
main ()
{
  float z = 0.0;
  float d = 0.0;
  float s = 0.0;
  vec3 dir = normalize( vec3(2.0*gl_FragCoord.xy - iResolution, - FOV * iResolution.y));

  vec3 col = vec3(0.);

  for(int i = 0; i<STEPS; i++)
  {
    vec3 p = z * dir;

    float f = WAVE_FREQ;
    for(int j = 0; j<WAVE_STEPS; j++, f *= WAVE_EXP)
      p += WAVE_AMP*sin(p*f - WAVE_VELOCITY*iTime).yzx / f;

    s = 0.3 - abs(p.y);
    d = SOFTNESS + max(s, -s*PASSTHROUGH) / 4.0;
    z += d;
    float phase = COLOR_WAVE * s + dot(p,COLOR_DOT) + COLOR_SPEED*iTime;
    col += (cos(phase - RGB) + COLOR_BASE) * exp(s*SKY) / d;
  }
  col *= SOFTNESS / STEPS * BRIGHTNESS;

  gl_FragColor = vec4(tanh(col * col), 1.0);
}
