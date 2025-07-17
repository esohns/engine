uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;

#define PI 3.14159265359

vec3
palette (float t, vec3 color1, vec3 color2, vec3 color3, vec3 color4)
{
  return color1 + color2 * cos(2.0 * PI * (color3 * t + color4));
}

void
main ()
{
  vec2 uv = gl_FragCoord.xy / iResolution;
  float aspect = iResolution.x / iResolution.y;
  uv = (uv - 0.5) * vec2(aspect, 1.0) + 0.5;

  float time = iTime * 0.025;

  vec3 sumColor = vec3(0.0);
  float sumWeights = 0.; 
  float amount = 8.0;

  for (float i = 1.0; i <= amount; i += 1.0)
  {
    vec3 color = palette(i / amount, vec3(0.570, 0.470, 0.590), vec3(0.632, 0.385, 0.634), vec3(0.520, 0.800, 0.520), vec3(-0.430, -0.397, -0.083));

    float oscillationX = sin(time + i * PI * 0.25) * 0.3;
    float oscillationY = sin(time + i * PI * 0.5) * 0.5;
    vec2 initPos = vec2(0.5, 0.5);
    vec2 lightPos = initPos + vec2(oscillationX, oscillationY);

    float lightDist = length(uv - lightPos);

    float lightWeight = 1.0 / pow(lightDist, 2.); 

    lightWeight = sin(iTime  - lightDist * 60.);
    //lightWeight = 1. / lightDist * sin(time - lightDist * 20. * PI);

    sumColor += lightWeight * color;
    sumWeights += lightWeight;
  }

  if (iMouse.z > 0.0)
  {
    vec2 mousePos = iMouse.xy / iResolution;
    mousePos = (mousePos - 0.5) * vec2(aspect, 1.0) + 0.5; 

    float mouseDist = length(uv - mousePos);
    vec3 mouseColor = palette(0., vec3(0.570, 0.470, 0.590), vec3(0.632, 0.385, 0.634), vec3(0.520, 0.800, 0.520), vec3(-0.430, -0.397, -0.083));
    float mouseWeight = 1./ mouseDist * sin(time - mouseDist * 20. * PI);

    sumColor += mouseWeight * mouseColor;
    sumWeights += mouseWeight;
  }

  sumColor /= sumWeights;
  sumColor = pow(sumColor, vec3(1.0 / 2.2));

  gl_FragColor = vec4(sumColor, (sumColor.r + sumColor.g + sumColor.b) / 3. * 2.);
}
