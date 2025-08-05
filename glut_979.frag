#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;
uniform sampler2D iChannel2;
uniform sampler2D iChannel3;

float PI = 3.14159265;
vec3 sunCol = vec3(258.0, 208.0, 100.0) / 15.0;
vec3 environmentSphereColor = vec3(0.3001, 0.501, 0.901) * 0.0;

float distFromSphere;
vec3 normal;
vec3 texBlurry;

vec3 saturate(vec3 a) { return clamp(a, 0.0, 1.0); }
vec2 saturate(vec2 a) { return clamp(a, 0.0, 1.0); }
float saturate (float a) { return clamp(a, 0.0, 1.0); }

vec3
GetSunColor (vec3 rayDir, vec3 sunDir)
{
  float dense = 16.0;
  vec3 localRay = normalize(rayDir);
  float sunIntensity = 1.0 - (dot(localRay, sunDir) * 0.5 + 0.5);
  sunIntensity = 0.2 / sunIntensity;
  sunIntensity = min(sunIntensity, 40000.0);
  sunIntensity = max(0.0, sunIntensity - 3.0);

  localRay.x = localRay.x + 1.0 - iTime * 0.1;
  vec2 wrap = fract(localRay.xy*dense);
  vec4 rand = texture(iChannel2, floor(localRay.xy*dense)/dense);
  vec3 starColor = rand.xyz;
  starColor = starColor * 0.75 + 0.25;
  rand.xy = rand.xy * 2.0 - 1.0;
  vec2 center = vec2(0.5, 0.5) + rand.xy * 0.9;
  float star = length(wrap - center);
  float test = star;
  star = saturate((1.0 - star));
  float blink = texture(iChannel2, localRay.xy + iTime * 0.03).x;
  float cluster = 0.3;
  star = pow(star, 60.0 + saturate(rand.z - 0.0) * 250.0 * cluster);
  star *= blink;

  float milkyMask = saturate(0.25 - abs(localRay.x - 0.65));
  vec3 milkyway = texture(iChannel3, (localRay.yx*1.5 )+vec2(0.65, 0.3)).yxz;
  vec3 milkyLOD = texture(iChannel3, (localRay.yx*1.5 )+vec2(0.65, 0.3), 3.0).yxz;
  vec3 milkyDetail = texture(iChannel3, (-localRay.yx*8.0 )+vec2(0.65, 0.3)).yxz;
  milkyway *= milkyDetail.xxx;
  milkyway *= vec3(1.0, 0.8, 0.91)*1.5;
  milkyway = pow(milkyway, vec3(2.0, 2.0, 2.0)*3.0);
  milkyway += vec3(0.2, 0.0015, 1.001) * milkyLOD * 0.006;

  vec3 finalColor = milkyway * 10850.0;
  finalColor += environmentSphereColor + sunCol * sunIntensity + starColor * star * 12000.0 * cluster;

  return finalColor;
}

vec3
GetSunColorReflection (vec3 rayDir, vec3 sunDir)
{
  vec3 localRay = normalize(rayDir);
  float sunIntensity = 1.0 - (dot(localRay, sunDir) * 0.5 + 0.5);
  sunIntensity = 0.2 / sunIntensity;
  sunIntensity = min(sunIntensity, 40000.0);

  return environmentSphereColor + sunCol * sunIntensity;
}

vec3
LensFlare (vec2 uv, vec2 lfPos)
{
  vec2 delta = uv - lfPos;
  float dist = length(delta);
  float angle = atan(delta.x, delta.y);
  vec3 tex = texture(iChannel0, vec2(angle*5.0, dist*0.125)).xyz;
  float bump = sin(angle * 6.0) * 0.5 + 0.54;
  bump -= pow(dist, 0.0125);
  bump = saturate(bump);

  return sunCol * tex.x * 0.1 * bump / (dist);
}

float
IntersectSphereAndRay (vec3 pos, float radius, vec3 posA, vec3 posB, out vec3 intersectA2, out vec3 intersectB2)
{
  vec3 eyeVec2 = normalize(posB-posA);
  float dp = dot(eyeVec2, pos - posA);
  vec3 pointOnLine = eyeVec2 * dp + posA;
  float distance = length(pointOnLine - pos);
  float ac = radius*radius - distance*distance;
  float rightLen = 0.0;
  if (ac >= 0.0)
    rightLen = sqrt(ac);
  intersectA2 = pointOnLine - eyeVec2 * rightLen;
  intersectB2 = pointOnLine + eyeVec2 * rightLen;
  distFromSphere = distance - radius;
  if (distance <= radius)
    return 1.0;

  return 0.0;
}

vec2
Spiral (vec2 uv)
{
  float reps = 2.0;
  vec2 uv2 = fract(uv*reps);
  vec2 center = floor(fract(uv*reps)) + 0.5;
  vec2 delta = uv2 - center;
  float dist = length(delta);
  float angle = atan(delta.y, delta.x);
  float nudge = dist * 4.0;
  vec2 offset = vec2(delta.y, -delta.x);
  float blend = max(abs(delta.x), abs(delta.y))* 2.0;
  blend = clamp((0.5 - dist) * 2.0, 0.0, 1.0);
  blend = pow(blend, 1.5);
  offset *= clamp(blend, 0.0, 1.0);

  return uv + offset*vec2(1.0,1.0)*1.1*texBlurry.x;
}

void
main ()
{
  vec2 uv = gl_FragCoord.xy/iResolution - 0.5;

  vec3 camUp=vec3(0.,1.,0.);
  vec3 camLookat=vec3(0,0.0,0);

  float mx=-PI/2.0;//iMouse.x/iResolution.x*PI*2.0;
  float my=0.0;//-iMouse.y/iResolution.y*10.0;//*PI/2.01;
  vec3 camPos=vec3(cos(my)*cos(mx),sin(my),cos(my)*sin(mx))*2.5;

  vec3 camVec=normalize(camLookat - camPos);
  vec3 sideNorm=normalize(cross(camUp, camVec));
  vec3 upNorm=cross(camVec, sideNorm);
  vec3 worldFacing=(camPos + camVec);
  vec3 worldPix = worldFacing + uv.x * sideNorm * (iResolution.x/iResolution.y) + uv.y * upNorm;
  vec3 relVec = normalize(worldPix - camPos);

  vec3 planetPos = vec3(0.0,0.0,0.0);
  vec3 iA, iB, iA2, iB2;
  float t = iTime * 0.1 + 0.7 - iMouse.x*0.01;
  float cloudT = iTime * 0.1;
  float distFromSphere2;
  vec3 normal2;
  float hit2 = IntersectSphereAndRay(planetPos, 1.05, camPos, worldPix, iA2, iB2);
  normal2 = normal;
  distFromSphere2 = distFromSphere;
  float hit = IntersectSphereAndRay(planetPos, 1.0, camPos, worldPix, iA, iB);
  normal = normalize(iA - planetPos);
  vec2 polar = /*vec2(atan(normal.x, normal.z), acos(normal.y));
  polar.x = (polar.x + PI) / (PI * 2.0);
  polar.y = polar.y / PI;
  polar.x = (polar.x+2.03);
  polar.xy =*/ iA.xy;

  vec4 texNoise = texture(iChannel2, (polar.xy+vec2(t,0.)) * 2.0);
  texNoise.y = texture(iChannel2, (polar.xy+vec2(t,0.)) * 1.0).y;
  texNoise.z = texture(iChannel2, (polar.xy+vec2(t,0.)) * 4.0).z;
  texBlurry = texture(iChannel0, (polar.xy+vec2(t,0.))*0.03125*0.25).rgb;

  vec3 tex = texture(iChannel0, (polar.xy+vec2(t,0.))*1.0).rgb;
  tex *= tex;
  vec3 texFlip = texture(iChannel0, (1.0 - (polar.xy+vec2(t,0))*0.5)).rgb;
  texFlip *= texFlip;

  vec3 texS = texture(iChannel0, (Spiral(polar.xy+vec2(t,0))+vec2(cloudT*0.25,0))*1.0).rgb;
  texS *= texS;
  vec3 texFlipS = texture(iChannel0, (1.0 - (Spiral(polar.xy+vec2(t,0))+vec2(cloudT*0.25,0))*0.5)).rgb;
  texFlipS *= texFlipS;

  float atmosphereDensity = (1.45 + normal.z);
  vec3 atmosphereColor = vec3(0.075, 0.35, 0.99) * 0.45;
  float cloudDensity = max(0.0, (pow(texFlipS.x * texS.x, 0.7) * 3.0));
  vec3 finalAtmosphere = atmosphereColor * atmosphereDensity + cloudDensity;
  vec3 finalColor = finalAtmosphere;

  vec3 detailMap = min(texture(iChannel3, (polar.xy+vec2(t,0)) * 2.0).xyz, 0.25) * 4.0;
  float land = pow(max(0.0, texture(iChannel1, (polar.xy+vec2(t,0))* 0.25).z - 0.25), 0.4)*0.75;
  float land2 = land * texBlurry.x * 6.0;
  land *= detailMap.x;
  land2 = max(0.0, land2);
  land -= tex.x*0.65;
  land = max(0.0, land);
  float iceFactor = abs(pow(normal.y,2.0));
  vec3 landColor = max(vec3(0.0,0.0,0.0), vec3(0.13,0.65,0.01) * land);// * (1.0 + iceFactor*2.0);
  vec3 landColor2 = max(vec3(0.0,0.0,0.0), vec3(0.8,0.4,0.01) * land2);
  vec3 mixedLand = (landColor + landColor2)* 0.5;
  mixedLand *= (detailMap.zyx + 2.0) * 0.333;
  vec3 finalLand = mix(mixedLand, vec3(7.0, 7.0, 7.0) * land, iceFactor);
  finalLand = mix(atmosphereColor * 0.05, finalLand, pow(min(1.0,max(0.0,-distFromSphere*1.0)), 0.2));
  finalColor += finalLand;
  finalColor *= hit;

  float refNoise = (texNoise.x + texNoise.y + texNoise.z)* 0.3333;
  vec3 noiseNormal = normal;
  noiseNormal.x += refNoise*0.05*hit;
  noiseNormal.y += tex.x*hit*0.1;
  noiseNormal.z += texFlip.x*hit*0.1;
  noiseNormal = normalize(noiseNormal);
  vec3 ref = reflect(normalize(worldPix - camPos), noiseNormal);

  refNoise = refNoise*0.25 + 0.75;
  float orbitSpeed = 0.125;
  vec3 sunDir = normalize(vec3(-0.009 + sin(iTime*orbitSpeed), -0.13, -cos(iTime*orbitSpeed)));
  vec3 r = normalize(cross(sunDir, vec3(0.0,1.0,0.0)));
  vec3 up = normalize(cross(sunDir, r));
  float binarySpeed = 0.5;
  float binaryDist = 0.3;
  sunDir += r * sin(iTime*binarySpeed) * binaryDist + up * cos(iTime*binarySpeed) * binaryDist;
  sunDir = normalize(sunDir);

  vec3 sunDir2 = normalize(vec3(-0.009 + sin((iTime+0.2)*orbitSpeed), 0.13, -cos((iTime+0.2)*orbitSpeed)));
  r = normalize(cross(sunDir2, vec3(0.0,1.0,0.0)));
  up = normalize(cross(sunDir2, r));
  sunDir2 -= r * sin(iTime*binarySpeed) * binaryDist + up * cos(iTime*binarySpeed) * binaryDist;
  sunDir2 = normalize(sunDir2);

  vec3 refNorm = normalize(ref);
  float glance = saturate(dot(refNorm, sunDir) * saturate(sunDir.z - 0.65));
  float glance2 = saturate(dot(refNorm, sunDir2) * saturate(sunDir2.z - 0.65));
  float landMask = finalLand.x + finalLand.y * 1.5;
  vec3 sunRef = GetSunColorReflection(refNorm, sunDir)*0.005*hit * (1.0 - saturate(landMask*3.5)) * (1.0-texS.x) * refNoise;
  vec3 sunRef2 = GetSunColorReflection(refNorm, sunDir2)*0.005*hit * (1.0 - saturate(landMask*3.5)) * (1.0-texS.x) * refNoise;

  sunRef = mix(sunRef, vec3(3.75, 0.8, 0.02)* hit, glance);
  sunRef2 = mix(sunRef2, vec3(3.75, 0.8, 0.02)* hit, glance2);
  finalColor += sunRef;
  finalColor += sunRef2;

  vec3 sunsColor = GetSunColor(normalize(ref), sunDir) *0.000096*(1.0-hit) +
                   GetSunColor(normalize(ref), sunDir2)*0.000096*(1.0-hit);

  float outerGlow = 1.0 - clamp(distFromSphere * 20.0, 0.0, 1.0);
  outerGlow = pow(outerGlow, 5.2);
  finalColor += (atmosphereColor + vec3(0.2, 0.2, 0.2)) * outerGlow * (1.0 - hit);

  float light = saturate(dot(sunDir, noiseNormal));
  light += saturate(dot(sunDir2, noiseNormal));
  finalColor *= light * 0.75 + 0.001;
  finalColor += sunsColor;

  float scattering, scattering2;
  if (hit2 == 1.0) scattering = distance(iA2, iB2);
  scattering2 = scattering;
  scattering *= pow(saturate(dot(relVec, sunDir) - 0.96), 2.0);
  scattering2 *= pow(saturate(dot(relVec, sunDir2) - 0.96), 2.0);
  scattering *= hit2 * (1.0 - hit);
  scattering2 *= hit2 * (1.0 - hit);
  scattering *= outerGlow;
  scattering2 *= outerGlow;
  finalColor += vec3(1.0, 0.25, 0.05) * scattering * 3060.0;
  finalColor += vec3(1.0, 0.25, 0.05) * scattering2 * 3060.0;

  gl_FragColor = vec4(sqrt(finalColor),1.0);
}
