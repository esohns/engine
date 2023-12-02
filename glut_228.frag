precision mediump float;

#define MAX_BALLS 300

// ball positions
uniform float u_ballsX[MAX_BALLS];
uniform float u_ballsY[MAX_BALLS];

// ball radii
uniform float u_radii[MAX_BALLS];

// number of balls we actually have (because arrays have to be defined with a constant size)
uniform int u_balls;

// distance field variables
uniform float u_distScale;
uniform float u_colorOffset;

// width and height of the canvas
uniform float u_width;
uniform float u_height;

varying vec2 v_uv;

void main()
{
  // this variable will hold our distance field info
  float df = 0.0; 

  // go through all balls (we have to bail out manually because for loops have to be declared with a constant value in WEBGL)
  for (int i = 0; i < MAX_BALLS; ++i)
  {
    if (i >= u_balls)
      break;

    // calculate the vector between this pixel and the current ball
    vec2 diff = vec2(u_ballsX[i], u_ballsY[i]) - vec2(v_uv.x*u_width, v_uv.y*u_height);

    // add the scaled distance to the distance field
    df += u_radii[i] * u_distScale/length(diff);
  } // end FOR

  // calculate a color from the distance field
  gl_FragColor = vec4(mod(df+u_colorOffset,1.0),mod(df+u_colorOffset*2.0,1.0),mod(df+u_colorOffset*3.0,1.0), 1.0);
}
