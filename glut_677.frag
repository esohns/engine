uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform bvec4 iKeyboard; // space

#define TWO_RINGS
#define SHOW_ARROWS
//#define FORCE_GRID_ON

#define SPIN_RATE 2.0
#define OUTLINE_WIDTH 1.0
#define RING_RADIUS 80.0

#define pi 3.141592
#define tau (pi * 2.0)

vec2
rotate (vec2 coord, float angle)
{
  float s = sin(angle);
  float c = cos(angle);

  return vec2(c * coord.x - s * coord.y, s * coord.x + c * coord.y);
}

float
lineArrow (vec2 p, vec2 p1, vec2 p2, float a)
{
  vec2 v = p2 - p1;
  vec2 w = p - p1;
  float d = clamp(dot(w,v) / dot(v,v), 0., 1.);
  float l = length(w-v*d);
  a = max(a*d-3., l-a*d);
  return min(l, a)-.5;
}

float
sdSegment (vec2 p, vec2 a, vec2 b)
{
  vec2 pa = p-a, ba = b-a;
  float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
  return length( pa - ba*h );
}

vec4
ring (vec2 uv, float aa, float speed, vec2 dir)
{
  float dist = length(uv);

  float ring_radius = 0.6;

  float edge = min(1.0 - dist, dist - ring_radius) / aa;
  float outline_edge = edge - OUTLINE_WIDTH;

  float dirMag = min(1.0, length(dir));
  float dirFade = 0.0;
  float arrow = 0.0;

  if (dir != vec2(0.0))
  {
    float dir_cosine = dot(normalize(dir), normalize(uv));

    if (speed < 0.0)
      dir = -dir;

    speed *= sign(dir_cosine);

    dirFade = 1.0 - clamp(abs(dir_cosine / 0.7), 0.0, 1.0);

    arrow = 1.0 - lineArrow(uv * RING_RADIUS, normalize(dir) * (RING_RADIUS * 0.25), -normalize(dir) * (RING_RADIUS * 0.25), 20.0);
  }
  else
  {
    vec4 arrowStartEnd = vec4(RING_RADIUS * 0.1, 0.0, RING_RADIUS * 0.35, 0.0);
    arrowStartEnd = speed > 0.0 ? arrowStartEnd : arrowStartEnd.zwxy;

    float ur = dot(vec2(1.0, 1.0), uv);
    float ul = dot(vec2(-1.0, 1.0), uv);

    arrowStartEnd = ul > 0.0 ? -arrowStartEnd : arrowStartEnd;
    arrowStartEnd = (ul > 0.0 ? ur : -ur) > 0.0 ? -arrowStartEnd.yxwz : arrowStartEnd;

    arrow = 1.0 - lineArrow(uv * RING_RADIUS, arrowStartEnd.zw, arrowStartEnd.xy, 10.0);
  }
  arrow = smoothstep(-1.5, 0.0, arrow);
  arrow *= abs(speed);

  float rotation = iTime * tau * SPIN_RATE;
  vec2 rotuv_inside = rotate(uv.xy, rotation);

  speed = dist > ((1.0 + ring_radius) * 0.5) ? speed : -speed;
  vec2 rotuv_outline = rotate(uv.xy, rotation - pi * speed * 0.25);

  vec3 yellow = vec3(0.97, 0.77, 0.2);
  vec3 blue = vec3(0.1, 0.25, 0.81);
  vec3 arrow_color = vec3(0.0);

  edge = smoothstep(-0.75, 0.75, edge);
  outline_edge = smoothstep(-0.75, 0.75, outline_edge + (OUTLINE_WIDTH + 1.5) * dirFade);

  float inside_mask = smoothstep(-aa, aa, rotuv_inside.x);
  inside_mask = mix(1.0 - inside_mask, inside_mask, smoothstep(-aa, aa, rotuv_inside.y));

  float outline_mask = smoothstep(-aa, aa, rotuv_outline.x);
  outline_mask = mix(1.0 - outline_mask, outline_mask, smoothstep(-aa, aa, rotuv_outline.y));

  vec3 col = mix(yellow, blue, inside_mask);
  col = mix(mix(yellow, blue, outline_mask), col, outline_edge);

#if defined(SHOW_ARROWS)
  col = dist < (ring_radius - aa * 2.0) ? arrow_color : col;
  edge += arrow;
#endif

  return vec4(col, edge);
}

void
main ()
{
  vec2 uv = gl_FragCoord.xy;
  vec2 halfResolution = iResolution.xy * 0.5;
  uv -= halfResolution;
  uv /= RING_RADIUS;

  vec3 col = vec3(0.5);

#if !defined (FORCE_GRID_ON)
  if (iKeyboard.x)
#endif
  {
    float grid = mod((floor(uv.x * 3.0) + floor(uv.y * 3.0)) + 1000.0, 2.0);
    col = vec3(grid == 0.0 ? 0.475 : 0.5);
  }

  float speed = smoothstep(0.25, 0.75, abs(fract(iTime * 0.15) * 2.0 - 1.0)) * 2.0 - 1.0;

  float moveAngle = -iTime * 0.451;
  vec2 dir = vec2(sin(moveAngle), cos(moveAngle));

  speed *= abs(smoothstep(0.3, 0.5, abs(fract(iTime * 0.025) * 2.0 - 1.0)) * 2.0 - 1.0);
  dir = abs(fract(iTime * 0.025) * 2.0 - 1.0) > 0.4 ? dir : vec2(0.0);

  if (iMouse.z > 0.0)
  {
    vec2 mouseRelativePos = iMouse.xy - halfResolution;
#if defined(TWO_RINGS)
    float mouseDist = sdSegment(mouseRelativePos, vec2(-RING_RADIUS * 1.33, 0.0), vec2(RING_RADIUS * 1.33, 0.0));
#else
    float mouseDist = length(mouseRelativePos);
#endif

    if (mouseDist > RING_RADIUS * 2.0)
    {
      dir = normalize(mouseRelativePos);
      speed = clamp((mouseDist - RING_RADIUS * 2.1) / min(RING_RADIUS, halfResolution.y - RING_RADIUS), 0.0, 1.0);
    }
    else
    {
      dir = vec2(0.0);
#if defined(TWO_RINGS)
      speed = clamp(mouseRelativePos.x / (RING_RADIUS * 1.33), -1.0, 1.0);
#else
      speed = clamp(mouseDist / RING_RADIUS * 3.0 - 2.0, -1.0, 1.0);
#endif
    }
  }

  vec2 ringUV = uv;

#if defined(TWO_RINGS)
  if (uv.x < 0.0)
    ringUV.x += 1.33;
  else
  {
    speed = -speed;
    ringUV.x -= 1.33;
  }
  col = mix(vec3(0.0), col, smoothstep(1.5, 3.5, length(uv * RING_RADIUS)));
#endif

  float aa = 1.0 / RING_RADIUS;

  vec4 ringCol = ring(ringUV, aa, speed, dir);
  col = mix(col, ringCol.rgb, ringCol.a);

  gl_FragColor = vec4(col,1.0);
}
