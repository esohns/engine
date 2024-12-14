//precision mediump float;

uniform sampler2D uSampler;
uniform float u_time;
uniform float u_speed;
uniform float u_tiling;
uniform float u_strength;

varying vec4 v_uv;

void
main ()
{
  vec2 texcoord = vec2 (v_uv.x - sin (u_time * u_speed) * 0.05 * cos (v_uv.y * u_tiling) * u_strength,
                        v_uv.y - cos (u_time * u_speed) * 0.05 * sin (v_uv.x * u_tiling) * u_strength);
  vec4 col = texture2D (uSampler, texcoord);
  gl_FragColor = col;
}
