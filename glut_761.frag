uniform vec2 iResolution;
uniform float iTime;

#define HW_PERFORMANCE 1

void
main ()
{
  vec2 op = (2.0 * gl_FragCoord.xy - iResolution.xy) / iResolution.y;
  float opx = 2.0 / iResolution.y;
    
  const float kt = 6.283185307;
  const float kh = (1.0 + sqrt(5.0)) / 2.0;
  const float k2 = 4.0 * log2(kh);

  float ran = fract(sin(gl_FragCoord.x * 7.0 + 17.0 * gl_FragCoord.y) * 1.317);

  vec3 tot = vec3(0.0);
#if HW_PERFORMANCE==0
  const int kNumSamples = 6;
#else
  const int kNumSamples = 12;
#endif
  for (int mb = 0; mb < kNumSamples; mb++)
  {
    float time = iTime + (0.5 / 60.0) * (float(mb) + ran) / float(kNumSamples);
    float ft = fract(time / 1.0);
    float it = floor(time / 1.0);

    float sca = 0.5 * exp2(-ft * k2);
    vec2 p = sca * op;
    float px = sca * opx;

    vec3 col = vec3(0.0);
    {
      float d = 1e20;
      float w = 1.0;
      vec2 q = p + vec2(3, -1) / sqrt(5.0);
      for (int i = 0; i < 20; i++)
      {
        float t = max(abs(q.x), abs(q.y)) - w;

        if (t < 0.0)
        {
          float id = float(i) + it * 4.0;
          col = vec3(0.7, 0.5, 0.4) + vec3(0.1, 0.2, 0.2) * cos(kt * id / 12.0 + vec3(2.0, 2.5, 3.0));
          col += 0.04 * cos(kt * p.x * 8.0 / w) * cos(kt * p.y * 8.0 / w);
        }

        d = min(d, abs(t) - 0.001 * w);

        q -= w * vec2(kh, 2.0 - kh);
        q = vec2(-q.y, q.x);
        w *= kh - 1.0;
      }
      col *= smoothstep(0.0, 1.5 * px, d - 0.001 * sca);
    }

    {
      p /= (3.0 - kh);
      px /= (3.0 - kh);
      float ra = length(p);
      float an = atan(-p.x, p.y) / kt;
      float id = round(log2(ra) / k2 - an);
      if (id > -1.5 || (id > -2.5 && an > 0.5 - ft))
      {
        float d = abs(ra - exp2(k2 * (an + id)));
        col = mix(col, vec3(1.0), smoothstep(2.0 * px, 0.0, d - 0.005 * sca));
      }
    }

    tot += col;
  }
  tot /= float(kNumSamples);

  tot *= 1.2 - 0.25 * length(op);

  tot += (1.0 / 255.0) * ran;

  gl_FragColor = vec4(tot, 1.0);
}
