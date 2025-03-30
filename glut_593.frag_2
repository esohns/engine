#version 130

// glut_593_common.glsl
#define dt .15
// lower value for vorticity threshold means higher viscosity
// and vice versa (max .3). Setting it to 0. disables it.
#define vorticityThreshold .25
#define velocityThreshold 24.
// higher this threshold, lower the viscosity (max .8)
#define viscosityThreshold .64

vec4
fluidSolver (sampler2D velocityField, vec2 uv, vec2 stepSize, vec4 mouse, vec4 prevMouse)
{
  float k = .2, s = k / dt;

  vec4 fluidData = textureLod(velocityField, uv, 0.);
  vec4 fr = textureLod(velocityField, uv + vec2(stepSize.x, 0.), 0.);
  vec4 fl = textureLod(velocityField, uv - vec2(stepSize.x, 0.), 0.);
  vec4 ft = textureLod(velocityField, uv + vec2(0., stepSize.y), 0.);
  vec4 fd = textureLod(velocityField, uv - vec2(0., stepSize.y), 0.);

  vec3 ddx = (fr - fl).xyz * .5;
  vec3 ddy = (ft - fd).xyz * .5;
  float divergence = ddx.x + ddy.y;
  vec2 densityDiff = vec2(ddx.z, ddy.z);

  // Solving for density
  fluidData.z -= dt*dot(vec3(densityDiff, divergence), fluidData.xyz);

  // Solving for velocity
  vec2 laplacian = fr.xy + fl.xy + ft.xy + fd.xy - 4.*fluidData.xy;
  vec2 viscosityForce = viscosityThreshold * laplacian;

  // Semi-lagrangian advection
  vec2 densityInvariance = s * densityDiff;
  vec2 uvHistory = uv - dt * fluidData.xy * stepSize;
  fluidData.xyw = textureLod(velocityField, uvHistory, 0.).xyw;

  // Calc external force from mouse input
  vec2 extForce = vec2(0.);

  if (mouse.z > 1. && prevMouse.z > 1.)
  {
    vec2 dragDir = clamp((mouse.xy - prevMouse.xy) * stepSize * 600., -10., 10.);
    vec2 p = uv - mouse.xy*stepSize;
    // extForce.xy += .0008 / (dot(p, p) + 1e-5) * (.5 - uv);
    extForce.xy += .001/(dot(p, p)) * dragDir;
  }

  fluidData.xy += dt*(viscosityForce - densityInvariance + extForce);

  // velocity decay
  fluidData.xy = max(vec2(0.), abs(fluidData.xy) - 5e-6)*sign(fluidData.xy);

  // Vorticity confinement
  fluidData.w = (fd.x - ft.x + fr.y - fl.y); // curl stored in the w channel
  vec2 vorticity = vec2(abs(ft.w) - abs(fd.w), abs(fl.w) - abs(fr.w));
  vorticity *= vorticityThreshold / (length(vorticity) + 1e-5) * fluidData.w;
  fluidData.xy += vorticity;

  // Boundary conditions
  fluidData.y *= smoothstep(.5,.48,abs(uv.y - .5));
  fluidData.x *= smoothstep(.5,.49,abs(uv.x - .5));

  // density stability
  fluidData = clamp(fluidData, vec4(vec2(-velocityThreshold), 0.5 , -vorticityThreshold), vec4(vec2(velocityThreshold), 3.0 , vorticityThreshold));

  return fluidData;
}
// glut_593_common.glsl

uniform vec2 iResolution;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

void
main ()
{
  vec2 uv = gl_FragCoord.xy / iResolution.xy;
  vec2 stepSize = 1./iResolution.xy;
  vec4 prevMouse = textureLod(iChannel0, vec2(0.), 0.);
  vec4 col = fluidSolver(iChannel0, uv, stepSize, iMouse, prevMouse);

  if (gl_FragCoord.y < 1.)
  {
  	col = iMouse;
  }

  gl_FragColor = col;
}
