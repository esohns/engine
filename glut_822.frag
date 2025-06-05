uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

const float steps = 128.0;

#define JITTER 1
#define DISTORTION 1

void
main ()
{
  vec2 texcoord = (gl_FragCoord.xy - iResolution.xy*.5)/iResolution.y;

#if DISTORTION
  texcoord *= sqrt(1.+dot(texcoord, texcoord)*1.); 
#endif
       
#if JITTER
  float jitter = fract(sin(fract(iTime) + dot(texcoord, vec2(41.97, 289.13)))*43758.5453)*min(.25 + length(texcoord), 1.);
#else
  float jitter = 1.;
#endif

  vec3 color;
  vec2 uv;
  float time = iTime/5.;

  for (int i = 0; i < int(steps); i++)
  {
    float depth = (float(i) + jitter) / steps;

    uv = texcoord * depth;
    
    color = texture(iChannel0, uv + vec2(time, 0.)).rgb;
    color = vec3(color.r);

    if (1.-dot(color, vec3(0.33333)*0.2) < depth)
      break;
  }
    
  float diff = max(color.r - texture(iChannel0, uv + vec2(time, 0.) - .005).r, 0.)*3.;
  float diff2 = max(color.r - texture(iChannel0, uv + vec2(time, 0.) + .005).r, 0.)*3.;

  color *= vec3(.5)*color; 

  color += vec3(.4, .7, 1.)*diff*diff*5. + vec3(1, .7, .4)*diff2*diff2*5.; 

  color += jitter * .04 - .02; 

  gl_FragColor = vec4(sqrt(max(color, 0.)),1.0); 
}
