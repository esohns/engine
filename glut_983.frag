#version 130

// glut_983_common.glsl
const float g = 9.81;
const float l1 = 0.5;
const float l2 = 0.5;
const float m1 = 1.0;
const float m2 = 1.0;
const float dt = 0.016;
const vec2 origin = vec2(0.0, 0.3);

float
drawLine (vec2 uv, vec2 a, vec2 b, float thickness)
{
  vec2 pa = uv - a;
  vec2 ba = b - a;
  float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
  float d = length(pa - ba * h);

  return smoothstep(thickness, thickness * 0.5, d);
}

float
drawCircle (vec2 uv, vec2 center, float radius)
{
  float dist = length(uv - center);

  return smoothstep(radius, radius * 0.9, dist);
}

vec2
getJoint (vec2 start, float len, float angle)
{
  return start + len * vec2(sin(angle), -cos(angle));
}
// glut_983_common.glsl

uniform int iFrame;
uniform sampler2D iChannel0;

void
main ()
{
  if (iFrame == 0)
  {
    float a1 = radians(120.0);
    float a2 = radians(90.0);
    float w1 = 0.0;
    float w2 = 0.0;
    gl_FragColor = vec4(a1, a2, w1, w2);

    return;
  }

  vec4 state = texelFetch(iChannel0, ivec2(0, 0), 0);
  float a1 = state.x;
  float a2 = state.y;
  float w1 = state.z;
  float w2 = state.w;

  float sin1 = sin(a1);
  float sin2 = sin(a2);
  float cos1 = cos(a1);
  float cos2 = cos(a2);
  float sin12 = sin(a1 - a2);
  float cos12 = cos(a1 - a2);

  float d1 =
    -g * (2.0 * m1 + m2) * sin1 - m2 * g * sin(a1 - 2.0 * a2) - 2.0 * sin12 * m2 * (w2 * w2 * l2 + w1 * w1 * l1 * cos12);

  float d2 = 2.0 * sin12 *
    (w1 * w1 * l1 * (m1 + m2) + g * (m1 + m2) * cos1 + w2 * w2 * l2 * m2 * cos12);

  float denom1 = l1 * (2.0 * m1 + m2 - m2 * cos(2.0 * a1 - 2.0 * a2));
  float denom2 = l2 * (2.0 * m1 + m2 - m2 * cos(2.0 * a1 - 2.0 * a2));

  float alpha1 = d1 / denom1;
  float alpha2 = d2 / denom2;

  w1 += alpha1 * dt;
  w2 += alpha2 * dt;
  a1 += w1 * dt;
  a2 += w2 * dt;

  gl_FragColor = vec4(a1, a2, w1, w2);
}
