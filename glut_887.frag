#version 130

uniform vec2 iResolution;
uniform float iTime;

vec3
vec_tovec3 (vec3 a)
{
  return vec3(a.x, a.y, a.z);
}

vec3
noise_vec2_xyx (vec2 self)
{
  return vec3(self.x, self.y, self.x);
}

vec2
demos_redlandscape_vec4_xy (vec4 self)
{
  return vec2(self.x, self.y);
}

vec2
demos_redlandscape_vec3_xz (vec3 self)
{
  return vec2(self.x, self.z);
}

float
vec_smootherstep (float edge0, float edge1, float x)
{
  float t = clamp(((x - edge0) / (edge1 - edge0)), 0.0, 1.0);
  return (((t * t) * t) * ((t * ((t * 6.0) - 15.0)) + 10.0));
}

float
vec_fastmix (float a, float b, float t)
{
  return (a + ((b - a) * t));
}

vec3
vec_fastmix_1 (vec3 a, vec3 b, float t)
{
  return (a + ((b - a) * t));
}
vec2 sincos(float x) {
  return vec2(sin(x), cos(x));
}

vec2
demos_redlandscape_vec3_xy (vec3 self)
{
  return vec2(self.x, self.y);
}

vec3
noise_vec3_yzx (vec3 self)
{
  return vec3(self.y, self.z, self.x);
}

vec2
noise_vec3_xx (vec3 self)
{
  return vec2(self.x, self.x);
}

vec2
noise_vec3_yz (vec3 self)
{
  return vec2(self.y, self.z);
}

vec2
noise_vec3_zy (vec3 self)
{
  return vec2(self.z, self.y);
}

float
noise_hash1_2 (vec2 v)
{
  vec3 v3 = noise_vec2_xyx(v);
  v3 = fract((v3 * 0.1031));
  v3 = (v3 + dot(v3, (noise_vec3_yzx(v3) + 33.33)));
  return fract(((v3.x + v3.y) * v3.z));
}

vec2
noise_hash2_1 (vec2 v)
{
  vec3 v3 = noise_vec2_xyx(v);
  v3 = (v3 * vec3(0.1031, 0.103, 0.0973));
  v3 = (v3 + dot(v3, (noise_vec3_yzx(v3) + 33.33)));
  return fract(((noise_vec3_xx(v3) + noise_vec3_yz(v3)) * noise_vec3_zy(v3)));
}

float
noise_noisemix2 (float a, float b, float c, float d, vec2 f)
{
  vec2 u = ((f * f) * (3.0 - (2.0 * f)));
  return vec_fastmix(vec_fastmix(a, b, u.x), vec_fastmix(c, d, u.x), u.y);
}

float
noise_noise_white_1 (vec2 p)
{
  return noise_hash1_2(p);
}

float
noise_noise_value_1 (vec2 p)
{
  vec2 i = floor(p);
  vec2 f = fract(p);
  vec2 I = floor((i + 1.0));
  float a = noise_hash1_2(i);
  float b = noise_hash1_2(vec2(I.x, i.y));
  float c = noise_hash1_2(vec2(i.x, I.y));
  float d = noise_hash1_2(I);
  return noise_noisemix2(a, b, c, d, f);
}

float
noise_noise_gradient_1 (vec2 p)
{
  vec2 i = floor(p);
  vec2 f = fract(p);
  vec2 I = floor((i + 1.0));
  vec2 F = (f - 1.0);
  float a = dot((-0.5 + noise_hash2_1(i)), f);
  float b = dot((-0.5 + noise_hash2_1(vec2(I.x, i.y))), vec2(F.x, f.y));
  float c = dot((-0.5 + noise_hash2_1(vec2(i.x, I.y))), vec2(f.x, F.y));
  float d = dot((-0.5 + noise_hash2_1(I)), F);
  return (0.5 + noise_noisemix2(a, b, c, d, f));
}

vec3
colorgrade_colorgrade_tonemap_aces (vec3 col)
{
  return clamp(((col * ((2.51 * col) + 0.03)) / ((col * ((2.43 * col) + 0.59)) + 0.14)), 0.0, 1.0);
}

vec3
colorgrade_colorgrade_saturate (vec3 col, float sat)
{
  float grey = dot(col, vec3(0.2125, 0.7154, 0.0721));
  return (grey + (sat * (col - grey)));
}

vec3
colorgrade_colorgrade_tone_1 (vec3 col, vec3 gain, vec3 lift, vec3 invgamma)
{
  col = pow(col, vec_tovec3(invgamma));
  return (((gain - lift) * col) + lift);
}

vec3
colorgrade_colorgrade_gamma_correction (vec3 col)
{
  return ((1.12661 * sqrt(col)) - (0.12661 * col));
}

vec3
colorgrade_colorgrade_vignette (vec3 col, vec2 coord, float strength, float amount)
{
  return (col * ((1.0 - amount) + (amount * pow(((((16.0 * coord.x) * coord.y) * (1.0 - coord.x)) * (1.0 - coord.y)), strength))));
}

vec3
colorgrade_colorgrade_dither (vec3 col, vec2 coord, float amount)
{
  return clamp((col + (noise_noise_white_1(coord) * amount)), 0.0, 1.0);
}

vec3
camera_camera_perspective (vec3 lookfrom, vec3 lookat, float tilt, float vfov, vec2 uv)
{
  vec2 sc = sincos(tilt);
  vec3 vup = vec3(sc.x, sc.y, 0.0);
  vec3 w = normalize((lookat - lookfrom));
  vec3 u = normalize(cross(w, vup));
  vec3 v = cross(u, w);
  float wf = (1.0 / tan((vfov * 0.00872664626)));
  return normalize((((uv.x * u) + (uv.y * v)) + (wf * w)));
}

float
demos_redlandscape_fbm_terrain (vec2 p)
{
  float a = 1.0;
  float t = 0.0;
  t = (t + (a * noise_noise_value_1(p)));
  a = (a * 0.5);
  p = (2.0 * p);
  t = (t + (a * noise_noise_value_1(p)));
  a = (a * 0.5);
  p = (2.0 * p);
  t = (t + (a * noise_noise_value_1(p)));
  a = (a * 0.5);
  p = (2.0 * p);
  t = (t + (a * noise_noise_value_1(p)));
  a = (a * 0.5);
  p = (2.0 * p);
  return t;
}

float
demos_redlandscape_map (vec3 p)
{
  vec2 q = demos_redlandscape_vec3_xz(p);
  float h = (demos_redlandscape_fbm_terrain(q) * 0.5);
  float d = ((p.y + (h * 0.75)) + 0.0);
  return (d * 0.5);
}

float
demos_redlandscape_ray_march (vec3 ro, vec3 rd)
{
  float t = 0.0;
  for(int i = 1; i <= 256; i = i + 1)
  {
    vec3 p = (ro + (t * rd));
    float d = demos_redlandscape_map(p);
    if((d < (0.003 * t)) || (t >= 25.0))
      break;
    t = (t + d);
  }
  return t;
}

void
main ()
{
  vec2 res = demos_redlandscape_vec3_xy(iResolution.xyx);
  vec2 uv = (gl_FragCoord.xy / res);
  vec2 coord = ((2.0 * (gl_FragCoord.xy - (res * 0.5))) / res.y);
  float z = (iTime * 1.0);
  vec2 sc = sincos((iTime * 0.5));
  float y = 0.0;
  vec3 lookat = vec3((sc.x * 0.5), y, z);
  vec3 ro = vec3(((-sc.x) * 0.5), y, (z - 2.0));
  vec3 rd = camera_camera_perspective(ro, lookat, 0.0, 45.0, coord);
  vec3 col = vec3(0.0, 0.0, 0.0);
  vec3 sun_dir = normalize(vec3(0.3, 0.07, 1.0));
  vec3 hor_col = vec3(0.7, 0.05, 0.01);
  vec3 sun_col = vec3(0.9, 0.8, 0.7);
  vec3 bou_col = vec3(0.8, 0.3, 0.1);
  float t = demos_redlandscape_ray_march(ro, rd);
  vec3 p = (ro + (rd * t));

  vec3 back_col;
  back_col = vec_fastmix_1(hor_col, (hor_col * 0.3), vec_smootherstep(0.0, 0.25, rd.y));
  back_col = vec_fastmix_1(back_col, bou_col, max((0.1 - rd.y), 0.0));
  float sun_lightness = max(dot(rd, sun_dir), 0.0);
  back_col = (back_col + (sun_col * pow(sun_lightness, 2000.0)));
  back_col = (back_col + ((0.3 * sun_col) * pow(sun_lightness, 100.0)));
  back_col = (back_col + (vec3(0.3, 0.2, 0.1) * pow(sun_lightness, 4.0)));

  if(abs(coord.y) > 0.75)
    col = vec3(0.0, 0.0, 0.0);
  else if(t < 25.0)
  {
    float decay = (1.0 - exp((-0.12 * t)));
    col = mix(col, back_col, decay);
  }
  else
  {
    col = back_col;
    float clouds_altitude = 1000.0;
    float clouds_dist = ((1.0 - (ro.y / clouds_altitude)) / rd.y);
    if(clouds_dist > 0.0)
    {
      float clouds_zoom = 1.0;
      vec2 clouds_pos = (demos_redlandscape_vec3_xz(ro) + ((demos_redlandscape_vec3_xz(rd) * clouds_dist) * clouds_zoom));
      float clouds_lightness = max((noise_noise_gradient_1(clouds_pos) - 0.3), 0.0);
      float clouds_decay = vec_smootherstep(0.0, 0.3, rd.y);
      vec3 clouds_col = (2.0 * col);
      col = vec_fastmix_1(col, clouds_col, (clouds_lightness * clouds_decay));
    }
    col = clamp(col, 0.0, 1.0);
  }
  col = colorgrade_colorgrade_tonemap_aces(col);
  col = colorgrade_colorgrade_gamma_correction(col);
  col = colorgrade_colorgrade_tone_1(col, vec3(1.3, 0.9, 0.7), (vec3(0.5, 0.1, 0.1) * 0.1), vec3(3.0, 2.0, 1.2));
  col = colorgrade_colorgrade_saturate(col, 0.7);
  col = colorgrade_colorgrade_vignette(col, uv, 0.25, 0.7);
  col = colorgrade_colorgrade_dither(col, gl_FragCoord.xy, 0.01);

  gl_FragColor = vec4(col.x, col.y, col.z, 1.0);
}
