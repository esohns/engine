uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform sampler2D iChannel0;
uniform samplerCube iChannel3;

const float PI = 3.14159265;

vec3 cmap1 (float x) { return pow(.5+.5*cos(PI * x + vec3(1,2,3)), vec3(2.5)); }

vec3
cmap2 (float x)
{
  vec3 col = vec3(.35, 1,1)*(cos(3.141592*x*vec3(1)+.75*vec3(2,1,3))*.5+.5);
  col *= col * col;
  return col;
}

vec3
cmap3 (float x)
{
  vec3 yellow = vec3(1.,.9,0);
  vec3 purple = vec3(.75,0,1);
    
  vec3 col = mix(purple, yellow, cos(x/1.25)*.5+.5);
  col*=col*col;
  return col;
}

vec3
cmap (float x)
{
  float t = mod(iTime, 30.);
  return (smoothstep(-1., 0., t) -smoothstep(9., 10., t))  * cmap1(x) + 
         (smoothstep(9., 10., t) -smoothstep(19., 20., t)) * cmap2(x) + 
         (smoothstep(19., 20., t)-smoothstep(29., 30., t)) * cmap3(x) +
         (smoothstep(29., 30., t)-smoothstep(39., 40., t)) * cmap1(x);
}

void
main ()
{
  vec2 uv = (2. * gl_FragCoord.xy - iResolution)/iResolution.y;

  float focal = 2.;
  vec3 ro = vec3(0, 0, 6.+cos(iTime*.25)*.75);

  float time = iTime * .5;

  float c = cos(time), s = sin(time);
  ro.xz *= mat2(c,s,-s,c);

  vec3 rd = normalize(vec3(uv, -focal));
  rd.xz *= mat2(c,s,-s,c);

  vec3 color = vec3(0);

  color += pow(texture(iChannel3, rd).rgb, vec3(2.2));

  time = iTime;
  {
    float t  = dot(0. - ro, rd);
    vec3 p   = t * rd + ro;
    float y2 = dot(p, p);
    float x2 = 4. - y2;
    if(y2 <= 4.)
    {
      float a = t-sqrt(x2);
      float b = t+sqrt(x2);

      color *= exp(-(b-a));

      t = a + texelFetch(iChannel0, (ivec2(gl_FragCoord.xy)+iFrame*335)%1024, 0).a*.01;
      for(int i = 0; i < 99 && t < b; i++)
      {
        vec3 p = t * rd + ro;

        float T = (t+time)/5.;
        float c = cos(T), s = sin(T);
        p.xy = mat2(c,-s,s,c) * p.xy;

        for(float f = 0.; f < 9.; f++) 
        {
          float a = exp(f)/exp2(f);
          p += cos(p.yzx * a + time)/a;
        }
        float d = 1./100. + abs((ro -p-vec3(0,1,0)).y-1.)/10.;
        color += cmap(t) * 1e-3 / d ;
        t += d*.25;
      }

      float R0 = 0.04;
      vec3 N = normalize(a * rd  + ro);
      float cosTheta = dot(-rd, N);
      float fresnel = R0 + (1.0 - R0) * pow(1.0 - cosTheta, 5.0);

      color *= 1.-fresnel;
      color += fresnel * pow(texture(iChannel3, reflect(rd, N)).rgb, vec3(2.2));
    }
  }

  color = 1.-exp(-color);
  color *= 1.-dot(uv*.55,uv*.55)*.15;
  color = pow(color, vec3(1./2.2));
  color *= 255.;
  color += 5.*(texelFetch(iChannel0, (ivec2(gl_FragCoord.xy)+iFrame*331)%1024, 0).rgb-.5);
  color /= 255.;
  color = clamp(color, 0., 1.);
  gl_FragColor = vec4(color, 1);
}
