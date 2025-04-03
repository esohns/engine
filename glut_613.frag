uniform vec2 iResolution;
uniform float iTime;

#define S(a,b,c) smoothstep(a,b,c)

vec3
premulMix (vec4 src, vec3 dst)
{
  return dst.rgb * (1.0 - src.a) + src.rgb;
}

vec4
premulMix (vec4 src, vec4 dst)
{
  vec4 res;
  res.rgb = premulMix(src, dst.rgb);
  res.a = 1.0 - (1.0 - src.a) * (1.0 - dst.a);
  return res;
}

vec4
roundPattern (vec2 uv)
{
  float dist = length(uv);

  float aa = 8. / iResolution.x;

  float triangle = abs(fract(dist * 11.0 + 0.3) - 0.5);
  float circles = S(0.25 - aa * 10.0, 0.25 + aa * 10.0, triangle);

  float grad = dist * 2.0;
  vec3 col = mix(vec3(0.0, 0.5, 0.6),  vec3(0.0, 0.2, 0.5), grad * grad);
  col = mix(col, vec3(1.0), circles);

  vec3 borderColor = vec3(0.7, 0.2, 0.2);
  col = mix(col, borderColor, S(0.44 - aa, 0.44 + aa, dist));
  col = mix(col, borderColor, S(0.05 + aa, 0.05 - aa, dist));

  float mask = S(0.5, 0.49, dist);
  float blur = 0.3;
  float shadow = S(0.5 + blur, 0.5 - blur, dist);

  return vec4(col * mask, clamp(mask + shadow * 0.55, 0.0, 1.0)); 
}

vec4
ring (vec2 uv, float angle, float angleOffet, float centerDist, float numcircles, float circlesRad)
{
  float quadId = floor(angle * numcircles + angleOffet);

  float quadAngle = (quadId + 0.5 - angleOffet) * (6.283 / numcircles);

  vec2 quadCenter = vec2(cos(quadAngle), sin(quadAngle)) * centerDist;

  vec2 circleUv = (uv + quadCenter) / circlesRad;
  return roundPattern(circleUv);
}

vec4
dblRing (vec2 uv, float angle, float centerDist, float numcircles, float circlesRad, float t)
{
  float s = sin(t * 3.0 + centerDist * 10.0) * 0.05;
  float d1 = 1.05 + s;
  float d2 = 1.05 - s;

  float rot = t * centerDist * 0.4 + sin(t + centerDist * 5.0) * 0.2;

  vec4 ring1 = ring(uv, angle, 0.0 + rot, centerDist * d1, numcircles, circlesRad);
  vec4 ring2 = ring(uv, angle, 0.5 + rot, centerDist * d2, numcircles, circlesRad);

  vec4 col = premulMix(ring1, ring2);

  col.rgb *= 1.0 - (centerDist * centerDist) * 0.4;

  return col;
}

vec4
autoRing (vec2 uv, float angle, float centerDist, float t)
{
  float nbCircles = 1.0 + floor(centerDist * 23.0);
  return dblRing(uv, angle, centerDist, nbCircles, 0.23, t);
}

vec3
fullImage (vec2 uv, float angle, float centerDist, float t)
{
  vec3 col;

  float space = 0.1;

  float ringRad = floor(centerDist / space) * space;

  vec4 ringCol1 = autoRing(uv, angle, ringRad - space, t);
  vec4 ringCol2 = autoRing(uv, angle, ringRad, t);
  vec4 ringCol3 = autoRing(uv, angle, ringRad + space, t);

  if (ringRad > 0.0)
  {
    col.rgb = ringCol3.rgb;
    col.rgb = premulMix(ringCol2, col.rgb);
    col.rgb = premulMix(ringCol1, col.rgb);
  }
  else
    col.rgb = ringCol2.rgb; 

  return col;
}

float
noise21 (vec2 uv)
{
  vec2 n = fract(uv* vec2(19.48, 139.9));
  n += sin(dot(uv, uv + 30.7)) * 47.0;
  return fract(n.x * n.y);
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - .5 * iResolution.xy) / iResolution.y; 

  uv *= 0.9;

  float angle = atan(uv.y, uv.x) / 6.283 + 0.5;
  float centerDist = length(uv);

  vec3 col = vec3(0.0);

  float noise = noise21(uv + iTime);
  for (float i = 0.0; i < 4.0; i++)
    col += fullImage(uv, angle, centerDist, iTime - ((i + noise) * 0.03));
  col /= 4.0;

  gl_FragColor = vec4(col,1.0);
}
