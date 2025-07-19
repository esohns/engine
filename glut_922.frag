uniform vec2 iResolution;
uniform float iTime;

#define twopi 6.28319

#define complexity 1
#if complexity == 1
const int nb_particles = 95;
#elif complexity == 2
const int nb_particles = 160;
#elif complexity == 3
const int nb_particles = 280;
#elif complexity == 4
const int nb_particles = 500;
#endif

const vec2 gen_scale = vec2(0.60, 0.45);
const vec2 middlepoint = vec2(0.35, 0.15);

const vec2 gravitation = vec2(-0., -4.5);
const vec3 main_x_freq = vec3(0.4, 0.66, 0.78);
const vec3 main_x_amp = vec3(0.8, 0.24, 0.18);
const vec3 main_x_phase = vec3(0., 45., 55.);
const vec3 main_y_freq = vec3(0.415, 0.61, 0.82);
const vec3 main_y_amp = vec3(0.72, 0.28, 0.15);
const vec3 main_y_phase = vec3(90., 120., 10.);
const float part_timefact_min = 6.;
const float part_timefact_max = 20.;
const vec2 part_max_mov = vec2(0.28, 0.28);

const float time_factor = 0.75;
const float start_time = 2.5;
const float grow_time_factor = 0.15;
#if complexity == 1
const float part_life_time_min = 0.9;
const float part_life_time_max = 1.9;
#elif complexity == 2
const float part_life_time_min = 1.0;
const float part_life_time_max = 2.5;
#elif complexity == 3
const float part_life_time_min = 1.1;
const float part_life_time_max = 3.2;
#elif complexity == 4
const float part_life_time_min = 1.2;
const float part_life_time_max = 4.0;
#endif

const float part_int_div = 40000.;
const float part_int_factor_min = 0.1;
const float part_int_factor_max = 3.2;
const float part_spark_min_int = 0.25;
const float part_spark_max_int = 0.88;
const float part_spark_min_freq = 2.5;
const float part_spark_max_freq = 6.0;
const float part_spark_time_freq_fact = 0.35;
const float mp_int = 12.;
const float dist_factor = 3.;
const float ppow = 2.3;

const float part_min_hue = -0.13;
const float part_max_hue = 0.13;
const float part_min_saturation = 0.5;
const float part_max_saturation = 0.9;
const float hue_time_factor = 0.035;
const float mp_hue = 0.5;
const float mp_saturation = 0.18;

const vec2 part_starhv_dfac = vec2(9., 0.32);
const float part_starhv_ifac = 0.25;
const vec2 part_stardiag_dfac = vec2(13., 0.61);
const float part_stardiag_ifac = 0.19;

const float mb_factor = 0.73;

float pst;
float plt;
float runnr;
float time2;
float time3;
float time4;

vec3
hsv2rgb (vec3 hsv)
{
  hsv.yz = clamp (hsv.yz, 0.0, 1.0);
  return hsv.z*(0.63*hsv.y*(cos(twopi*(hsv.x + vec3(0.0, 2.0/3.0, 1.0/3.0))) - 1.0) + 1.0);
}

float
random (float co)
{
  return fract(sin(co*12.989) * 43758.545);
}

float
getParticleStartTime (int partnr)
{
  return start_time*random(float(partnr*2));
}

float
harms (vec3 freq, vec3 amp, vec3 phase, float time)
{
  float val = 0.;
  for (int h=0; h<3; h++)
    val+= amp[h]*cos(time*freq[h]*twopi + phase[h]/360.*twopi);
  return (1. + val)/2.;
}

vec2
getParticlePosition (int partnr)
{  
  float part_timefact = mix(part_timefact_min, part_timefact_max, random(float(partnr*2 + 94) + runnr*1.5));
  float ptime = (runnr*plt + pst)*(-1./part_timefact + 1.) + time2/part_timefact;   
  vec2 ppos = vec2(harms(main_x_freq, main_x_amp, main_x_phase, ptime), harms(main_y_freq, main_y_amp, main_y_phase, ptime)) + middlepoint;

  vec2 delta_pos = part_max_mov*(vec2(random(float(partnr*3-23) + runnr*4.), random(float(partnr*7+632) - runnr*2.5))-0.5)*(time3 - pst);

  vec2 grav_pos = gravitation*pow(time4, 2.)/250.;
  return (ppos + delta_pos + grav_pos)*gen_scale;
}

vec2
getParticlePosition_mp ()
{
  vec2 ppos = vec2(harms(main_x_freq, main_x_amp, main_x_phase, time2), harms(main_y_freq, main_y_amp, main_y_phase, time2)) + middlepoint;
  return gen_scale*ppos;
}

vec3
getParticleColor (int partnr, float pint)
{
  float hue;
  float saturation;

  saturation = mix(part_min_saturation, part_max_saturation, random(float(partnr*6 + 44) + runnr*3.3))*0.45/pint;
  hue = mix(part_min_hue, part_max_hue, random(float(partnr + 124) + runnr*1.5)) + hue_time_factor*time2;

  return hsv2rgb(vec3(hue, saturation, pint));
}

vec3
getParticleColor_mp (float pint)
{
  float hue;
  float saturation;

  saturation = 0.75/pow(pint, 2.5) + mp_saturation;
  hue = hue_time_factor*time2 + mp_hue;

  return hsv2rgb(vec3(hue, saturation, pint));
}

vec3
drawParticles (vec2 uv, float timedelta)
{   
  time2 = time_factor*(iTime + timedelta);

  vec3 pcol = vec3(0.);

  for (int i=1; i<nb_particles; i++)
  {
    pst = getParticleStartTime(i);
    plt = mix(part_life_time_min, part_life_time_max, random(float(i*2-35)));
    time4 = mod(time2 - pst, plt);
    time3 = time4 + pst;

    runnr = floor((time2 - pst)/plt);
    vec2 ppos = getParticlePosition(i);
    float dist = distance(uv, ppos);

    vec2 uvppos = uv - ppos;
    float distv = distance(uvppos*part_starhv_dfac + ppos, ppos);
    float disth = distance(uvppos*part_starhv_dfac.yx + ppos, ppos);

    vec2 uvpposd = 0.707*vec2(dot(uvppos, vec2(1., 1.)), dot(uvppos, vec2(1., -1.)));
    float distd1 = distance(uvpposd*part_stardiag_dfac + ppos, ppos);
    float distd2 = distance(uvpposd*part_stardiag_dfac.yx + ppos, ppos);

    float pint0 = mix(part_int_factor_min, part_int_factor_max, random(runnr*4. + float(i-55)));

    float pint1 = 1./(dist*dist_factor + 0.015) + part_starhv_ifac/(disth*dist_factor + 0.01) + part_starhv_ifac/(distv*dist_factor + 0.01) + part_stardiag_ifac/(distd1*dist_factor + 0.01) + part_stardiag_ifac/(distd2*dist_factor + 0.01);

    float pint = pint0*(pow(pint1, ppow)/part_int_div)*(-time4/plt + 1.);
                
    pint*= smoothstep(0., grow_time_factor*plt, time4);

    float sparkfreq = clamp(part_spark_time_freq_fact*time4, 0., 1.)*part_spark_min_freq + random(float(i*5 + 72) - runnr*1.8)*(part_spark_max_freq - part_spark_min_freq);
    pint*= mix(part_spark_min_int, part_spark_max_int, random(float(i*7 - 621) - runnr*12.))*sin(sparkfreq*twopi*time2)/2. + 1.;

    pcol+= getParticleColor(i, pint);
  }

  vec2 ppos = getParticlePosition_mp();
  float dist = distance(uv, ppos);

  vec2 uvppos = uv - ppos;
  float distv = distance(uvppos*part_starhv_dfac + ppos, ppos);
  float disth = distance(uvppos*part_starhv_dfac.yx + ppos, ppos);

  vec2 uvpposd = 0.7071*vec2(dot(uvppos, vec2(1., 1.)), dot(uvppos, vec2(1., -1.)));
  float distd1 = distance(uvpposd*part_stardiag_dfac + ppos, ppos);
  float distd2 = distance(uvpposd*part_stardiag_dfac.yx + ppos, ppos);

  float pint1 = 1./(dist*dist_factor + 0.015) + part_starhv_ifac/(disth*dist_factor + 0.01) + part_starhv_ifac/(distv*dist_factor + 0.01) + part_stardiag_ifac/(distd1*dist_factor + 0.01) + part_stardiag_ifac/(distd2*dist_factor + 0.01);
        
  if (part_int_factor_max*pint1>6.)
  {
    float pint = part_int_factor_max*(pow(pint1, ppow)/part_int_div)*mp_int;
    pcol+= getParticleColor_mp(pint);
  }

  return pcol;
}

void
main ()
{
  vec2 uv = gl_FragCoord.xy / iResolution;

  //vec2 uv2 = fragCoord.xy / iResolution.xy;
//  vec3 pcolor = texture(iChannel0,uv2).rgb*mb_factor;

  vec3 pcolor = vec3(0., (0.6 - uv.y)/10., (1. - uv.y)/9.);
  //vec3 pcolor = texture(iChannel0,uv).rgb*0.4;

  pcolor += drawParticles(uv,0.)*0.9;

  gl_FragColor = vec4(pcolor, 0.);
}
