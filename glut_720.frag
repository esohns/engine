#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform samplerCube iChannel0;

const float hex_factor = 0.8660254037844386;
const vec3 fog_color = vec3(0.9, 0.95, 1.0);

#define HEX_FROM_CART(p) vec2(p.x / hex_factor, p.y)
#define CART_FROM_HEX(g) vec2(g.x * hex_factor, g.y)

float
hexDist (vec2 p)
{
  p = abs(p);
  return max(dot(p, vec2(hex_factor, 0.5)), p.y) - 1.0;
}

vec2
nearestHexCell (vec2 pos)
{
  vec2 gpos = HEX_FROM_CART(pos);
  vec2 hex_int = floor(gpos);

  float sy = step(2.0, mod(hex_int.x + 1.0, 4.0));
  hex_int += mod(vec2(hex_int.x, hex_int.y + sy), 2.0);

  vec2 gdiff = gpos - hex_int;

  if (dot(abs(gdiff), vec2(hex_factor * hex_factor, 0.5)) > 1.0)
  {
    vec2 delta = sign(gdiff) * vec2(2.0, 1.0);
    hex_int += delta;
  }

  return hex_int;
   
}

vec2
alignNormal (vec2 h, vec2 d)
{
  return h * sign(dot(h, CART_FROM_HEX(d)));
}

vec3
rayHexIntersect (vec2 ro, vec2 rd, vec2 h)
{
  vec2 n = CART_FROM_HEX(h);

  float u = (1.0 - dot(n, ro)) / dot(n, rd);

  return vec3(h, u);
}

vec3
rayMin (vec3 a, vec3 b)
{
  return a.z < b.z ? a : b;
}

#define HASHSCALE3 vec3(.1031, .1030, .0973)
vec3
hash32 (vec2 p)
{
  vec3 p3 = fract(vec3(p.xyx) * HASHSCALE3);
  p3 += dot(p3, p3.yxz + 19.19);
  return fract((p3.xxy + p3.yzz) * p3.zyx);
}

float
height_for_pos (vec2 pos)
{
  pos += vec2(2.0 * sin(iTime * 0.3 + 0.2), 2.0 * cos(iTime * 0.1 + 0.5));
    
  float x2 = dot(pos, pos);
  float x = sqrt(x2);
    
  return 6.0 * cos(x * 0.2 + iTime) * exp(-x2 / 128.0);
}

vec4
surface (vec3 rd, vec2 cell, vec4 hit_nt, float bdist)
{
  float fogc = exp(-length(hit_nt.w * rd) * 0.02);

  vec3 n = hit_nt.xyz;

  vec3 noise = (hash32(cell) - 0.5) * 0.15;
  n = normalize(n + noise);

  float border_scale = 2.0 / iResolution.y;

  const float border_size = 0.04;

  float border = smoothstep(0.0, border_scale * hit_nt.w, abs(bdist) - border_size);

  border = mix(border, 0.75, smoothstep(18.0, 45.0, hit_nt.w));

  vec3 L = normalize(vec3(3, 1, 4));

  float diffamb = (clamp(dot(n, L), 0.0, 1.0) * 0.8 + 0.2);

  vec3 color = vec3(1.0);

  color = mix(vec3(0.1, 0, 0.08), color, border);

  color *= diffamb;

  color = mix(color, texture(iChannel0, reflect(rd, n)).yzx, 0.4 * border);

  color = mix(fog_color, color, fogc);
    
  return vec4(color, border);
}

vec3
shade (vec3 ro, vec3 rd)
{
  vec3 color = fog_color;

  vec2 cur_cell = nearestHexCell(ro.xy);

  vec2 h0 = alignNormal(vec2(0.0, 1.0), rd.xy);
  vec2 h1 = alignNormal(vec2(1.0, 0.5), rd.xy);
  vec2 h2 = alignNormal(vec2(1.0, -0.5), rd.xy);

  float cell_height = height_for_pos(CART_FROM_HEX(cur_cell));
    
  float alpha = 1.0;

  for (int i = 0; i < 80; ++i)
  {
    bool hit = false;
    vec4 hit_nt;
    float bdist = 1e5;

    vec2 cur_center = CART_FROM_HEX(cur_cell);
    vec2 rdelta = ro.xy - cur_center;
        
    vec3 ht = rayHexIntersect(rdelta, rd.xy, h0);
    ht = rayMin(ht, rayHexIntersect(rdelta, rd.xy, h1));
    ht = rayMin(ht, rayHexIntersect(rdelta, rd.xy, h2));

    float tz = (cell_height - ro.z) / rd.z;

    if (ro.z > cell_height && rd.z < 0.0 && tz < ht.z)
    {
      hit = true;
      hit_nt = vec4(0, 0, 1.0, tz);
      vec2 pinter = ro.xy + rd.xy * tz;

      bdist = hexDist(pinter - cur_center);
    }
    else
    {
      cur_cell += 2.0 * ht.xy;
            
      vec2 n = CART_FROM_HEX(ht.xy);
      cur_center = CART_FROM_HEX(cur_cell);

      float prev_cell_height = cell_height;
      cell_height = height_for_pos(cur_center);

      vec3 p_intersect = ro + rd * ht.z;

      if (p_intersect.z < cell_height)
      {
        hit_nt = vec4(n, 0.0, ht.z);
        hit = true;

        bdist = cell_height - p_intersect.z;

        bdist = min(bdist, p_intersect.z - prev_cell_height);

        vec2 p = p_intersect.xy - cur_center;
        p -= n * dot(p, n);
        bdist = min(bdist, abs(length(p) - 0.5 / hex_factor));
      }
    }

    if (hit)
    {
      vec4 hit_color = surface(rd, cur_cell, hit_nt, bdist);

      color = mix(color, hit_color.xyz, alpha);

      alpha *= 0.17 * hit_color.w;

      ro = ro + rd * hit_nt.w;
      rd = reflect(rd, hit_nt.xyz);
      ro += 1e-3 * hit_nt.xyz;

      h0 = alignNormal(vec2(0.0, 1.0), rd.xy);
      h1 = alignNormal(vec2(1.0, 0.5), rd.xy);
      h2 = alignNormal(vec2(1.0, -0.5), rd.xy);
    }
  }
    
  color = mix(color, fog_color, alpha);

  return color;
}

void
main ()
{
  const float yscl = 720.0;
  const float f = 500.0;
  
  vec2 uvn = (gl_FragCoord.xy - 0.5 * iResolution.xy) / iResolution.y;
  vec2 uv = uvn * yscl;

  vec3 pos = vec3(-12.0, 0.0, 10.0);
  vec3 tgt = vec3(0.);
  vec3 up = vec3(0.0, 0.0, 1.0);

  vec3 rz = normalize(tgt - pos);
  vec3 rx = normalize(cross(rz, up));
  vec3 ry = cross(rx, rz);

  float s = 1.0 + dot(uvn, uvn) * 1.5;

  vec3 rd = mat3(rx, ry, rz) * normalize(vec3(uv * s, f));
  vec3 ro = pos;

  float thetax = -0.35 - 0.2 * cos(0.031513 * iTime);
  float thetay = -0.02 * iTime;

  if (iMouse.y > 10.0 || iMouse.x > 10.0)
  {
    thetax = (iMouse.y - 0.5 * iResolution.y) * -1.25 / iResolution.y;
    thetay = (iMouse.x - 0.5 * iResolution.x) * 6.28 / iResolution.x;
  }

  float cx = cos(thetax);
  float sx = sin(thetax);
  float cy = cos(thetay);
  float sy = sin(thetay);

  mat3 Rx = mat3(1.0, 0.0, 0.0,
                 0.0, cx, sx,
                 0.0, -sx, cx);
  
  mat3 Ry = mat3(cy, 0.0, -sy,
                 0.0, 1.0, 0.0,
                 sy, 0.0, cy);
    
  mat3 R = mat3(0.0, 0.0, 1.0,
                -1.0, 0.0, 0.0,
                0.0, 1.0, 0.0);
  
  rd = transpose(R) * Ry * Rx * R * rd;
  ro = transpose(R) * Ry * Rx * R * (pos - tgt) + tgt;

  vec3 color = shade(ro, rd);
  color = sqrt(color);
    
  vec2 q = gl_FragCoord.xy / iResolution.xy;

  color *= pow(16.0 * q.x * q.y * (1.0 - q.x) * (1.0 - q.y), 0.1);

  gl_FragColor = vec4(color, 1.0);
}
