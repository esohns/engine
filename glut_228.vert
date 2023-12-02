attribute vec4 aPosition;

varying vec2 v_uv;

void main()
{
  // calculate uvs
  v_uv = aPosition.xy;
  v_uv.y *= -1.0;
  v_uv.x = v_uv.x * 0.5 + 0.5;
  v_uv.y = v_uv.y * 0.5 + 0.5;

  // set position
  gl_Position = aPosition;
}
