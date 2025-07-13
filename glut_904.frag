uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;

vec3
palette (float t)
{
  return .5+.5*cos(6.28318*(t+vec3(.3,.416,.557)));
}

vec2
raySphere (vec3 ro, vec3 rd, vec3 center, float radius)
{
  vec3 oc = ro - center;
  float b = dot(oc, rd);
  float c = dot(oc, oc) - radius * radius;
  float h = b * b - c;
    
  if (h < 0.0) return vec2(-1.0);
    
  h = sqrt(h);
  return vec2(-b - h, -b + h);
}

float
getContours (vec3 p, float sphereRadius, vec2 mousePos, mat3 invRot)
{
  vec3 n = normalize(p);
    
  float waveTime = iTime * 0.3;
  float waveFreq1 = 3.0;
  float waveFreq2 = 5.0;
    
  vec2 mouseUV = (mousePos - 0.5 * iResolution.xy) / iResolution.y;
  vec3 mouseRay = normalize(vec3(mouseUV, -1.0));
    
  mouseRay = invRot * mouseRay;
    
  float mouseInfluence = dot(n, mouseRay);
  mouseInfluence = smoothstep(0.8, 0.95, mouseInfluence);
    
  float latBase = n.y * 3.0;
  float angle = atan(n.z, n.x); 

  float equatorFactor = length(n.xy);
  float latWave = (sin(angle * waveFreq1 + waveTime) * 0.6 +
                    sin(angle * waveFreq2 - waveTime * 0.7) * 0.03) * equatorFactor*(sin(waveTime)*5.);
    
  float mouseBend = sin(angle * 2.0) * mouseInfluence * 0.8;
  mouseBend += cos(length(n.xz) * 4.0) * mouseInfluence * 0.4;
    
  float lat = latBase + latWave + mouseBend;
    
  float lineDistance = abs(fract(lat) - 0.5) * 2.0;
  float lineWidth = 0.2;
  float aa = fwidth(lat) * 0.5;
  float latLines = 1.0 - smoothstep(lineWidth - aa, lineWidth + aa, lineDistance);
    
  return latLines;
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - 0.5 * iResolution) / iResolution.y;

  vec3 ro = vec3(0.0, 0.0, 3.0);
  vec3 rd = normalize(vec3(uv, -1.0));

  vec3 sphereCenter = vec3(0.0, 0.0, 0.0);
  float sphereRadius = 1.0;

  float t = iTime * 0.5;
  mat3 rot = mat3(
      cos(t), 0.0, sin(t),
      0.0, 1.0, 0.0,
      -sin(t), 0.0, cos(t)
  );
  mat3 invRot = mat3(
      cos(t), 0.0, -sin(t),
      0.0, 1.0, 0.0,
      sin(t), 0.0, cos(t)
  );

  vec2 hit = raySphere(ro, rd, sphereCenter, sphereRadius);

  vec3 color = vec3(0.0);

  if (hit.x > 0.0)
  {
    vec3 color1 = vec3(0.0);
    vec3 color2 = vec3(0.0);

    vec3 p1 = ro + rd * hit.x;
    p1 = rot * p1;
    vec3 n1 = normalize(p1);
    float contour1 = getContours(p1, sphereRadius, iMouse.xy, invRot);

    float gradientParam1 = (n1.y + n1.x * 0.5 + iTime * 0.2) * 0.5 + 0.5;
    vec3 gradientColor1 = palette(gradientParam1);

    if (hit.y > 0.0)
    {
      vec3 p2 = ro + rd * hit.y;
      p2 = rot * p2;
      vec3 n2 = normalize(p2);
      float contour2 = getContours(p2, sphereRadius, iMouse.xy, invRot);

      float gradientParam2 = (n2.y + n2.x * 0.5 + iTime * 0.2) * 0.5 + 0.5;
      vec3 gradientColor2 = palette(gradientParam2);

      color2 = gradientColor2 * contour2 * 0.8;
    }

    vec3 baseGradient = gradientColor1 * 0.08;
    vec3 contourColor = gradientColor1 * 2.2;
    color1 = mix(baseGradient, contourColor, contour1);

    vec3 normal = normalize(p1 - sphereCenter);
    float fresnel = pow(1.0 - abs(dot(normal, -rd)), 2.0);
    color1 += gradientColor1 * fresnel * 0.12;

    color = color1 + color2;
  }

  vec3 bg = mix(vec3(0.05, 0.1, 0.2), vec3(0.0, 0.0, 0.05), length(uv));
  color = mix(bg, color, min(1.0, length(color)));
    
  gl_FragColor = vec4(color, 1.0);
}
