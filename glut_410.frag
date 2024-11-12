//precision mediump float;

uniform vec2 iResolution;
uniform sampler2D iImage;
uniform vec2 iMouse;

varying vec2 vTexCoord;

vec2
pixelate (vec2 uv, vec2 grid)
{
  uv *= grid;
  vec2 iuv = floor (uv) + .5;  // +.5 to center to cell
  return iuv / grid;
}

vec2
pixelate (vec2 uv, vec2 pos, vec2 grid)
{
  uv -= pos;
  uv *= grid;
  vec2 iuv = floor (uv) + .5;  // +.5 to center to cell
  return iuv / grid;
}

vec2
pixelate (vec2 uv, vec2 pos, vec2 size, vec2 grid)
{
  if (pos.x > uv.x || uv.x > pos.x+size.x ||
      pos.y > uv.y || uv.y > pos.y+size.y)
    return uv;

  uv -= pos;
  uv /= size;
  uv *= grid;
  vec2 iuv = floor (uv) + .5;  // +.5 to center to cell
  return iuv / grid * size + pos;
}

void
main ()
{
  vec2 uv = vTexCoord;
  vec2 mouse = iMouse.xy / iResolution.xy;
  uv.y = 1. - uv.y;
  mouse.y = 1. - mouse.y;

  // position and size of the area to be pixelated
  vec2 pos = vec2 (.1, .35);
  vec2 size = vec2 (.7, .2);

  // grid dimensions for pixelation
  vec2 grid = vec2 (128., 128.) * mouse;
  grid = floor (grid);
  grid = clamp (grid, vec2 (1.), vec2 (128.));

  // pixelate our uv coordinates
  vec2 puv = pixelate (uv, pos, size, grid);

  // apply pixelated uv coordinates to image
  gl_FragColor = texture2D (iImage, puv);
}
