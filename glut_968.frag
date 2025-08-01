uniform vec2 iResolution;
uniform float iTime;

float
sdOctahedron (vec3 p, float s)
{
  p = abs(p);

  return (p.x + p.y + p.z - s) * 0.57735027;
}

float
sdTorus (vec3 p, vec2 t)
{
  vec2 q = vec2(length(p.xy) - t.x, p.z);

  return length(q)-t.y;
}

float
opSmoothUnion (float d1, float d2, float k)
{
  float h = clamp(0.5 + 0.5 * (d2 - d1) / k, 0.0, 1.0);

  return mix(d2, d1, h) - k * h * (1.0 - h);
}

float
map (vec3 p)
{
  float res = 1e9;
    
  for (int i = 0; i < 6; i++)
  {
    float fi = float(i);
    float time = iTime * (0.5 + fi * 0.1);
    vec3 center = vec3(
      sin(time + fi * 1.57),
      cos(time * 0.8 + fi * 2.0),
      sin(time * 1.2 - fi * 1.0)
    ) * 1.5;

    vec3 p_local = p - center;

    float rot_angle = iTime * (1.0 + fi * 0.3);
    p_local.xz *= mat2(cos(rot_angle), -sin(rot_angle), sin(rot_angle), cos(rot_angle));
    p_local.xy *= mat2(cos(rot_angle*0.7), -sin(rot_angle*0.7), sin(rot_angle*0.7), cos(rot_angle*0.7));

    float torus = sdTorus(p_local, vec2(1.0, 0.25));
    res = opSmoothUnion(res, torus, 0.75);
  }

  return res;
}

vec3
getNormal (vec3 p)
{
  vec2 e = vec2(0.001, 0.0);
  return normalize(vec3(
    map(p + e.xyy) - map(p - e.xyy),
    map(p + e.yxy) - map(p - e.yxy),
    map(p + e.yyx) - map(p - e.yyx)
  ));
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy * 2.0 - iResolution) / iResolution.y;

  float angle = iTime * 0.15;
  vec3 ro = vec3(sin(angle) * 5.0, 1.5, cos(angle) * 8.0);
  vec3 ta = vec3(0.0, 0.0, 0.0);
  vec3 ww = normalize(ta - ro);
  vec3 uu = normalize(cross(vec3(0.0, 1.0, 0.0), ww));
  vec3 vv = cross(ww, uu);
  vec3 rd = normalize(uv.x * uu + uv.y * vv + 2.0 * ww);

  float t = 0.0;
  for (int i = 0; i < 100; i++)
  {
    vec3 p = ro + rd * t;
    float d = map(p);
    if (d < 0.001)
      break;
    t += d;
    if (t > 20.0)
      break;
  }

  vec3 col = vec3(0.0);
  if (t < 20.0)
  {
    vec3 p = ro + rd * t;
    vec3 n = getNormal(p);

    vec3 c1 = 0.5 + 0.5 * cos(iTime * 0.8 + vec3(0,1,2));
    vec3 c2 = 0.5 + 0.5 * cos(iTime * 0.5 + vec3(2,0,1));
    col = mix(c1, c2, smoothstep(-1.0, 1.0, n.y));

    float fresnel = pow(1.0 + dot(rd, n), 3.0);
    col += vec3(1.0) * fresnel * 0.5;
  }

  gl_FragColor = vec4(col, 1.0);
}
