#version 130

uniform vec2 iResolution;
uniform float iTime;

#define MAXSTEPS 50
#define HITTHRESHOLD 0.009
#define FAR 25.
#define AA 2
#define NIFS 6
#define SCALE 2.3
#define TRANSLATE 3.5

mat2x2
rot (float angle)
{
  float c = cos(angle);
  float s = sin(angle);
  return mat2x2(c, -s,
                s, c);
}

vec4
sd2d (vec2 p, float o)
{
  float time = 0.2*o+0.6*iTime;
  float s =0.5;
  p*= s;
  float RADIUS =(1.+sin(iTime));
  int i;
  vec3 col;  
  p = p*rot(-0.4*time);// twist

  for ( i = 0; i<NIFS; i++)
  {
    if (p.x<0.) {p.x = -p.x; col.r++;}
    p = p*rot(0.9*sin(time));
    if (p.y<0.) {p.y = -p.y; col.g++; }
    if (p.x-p.y<0.){ p.xy = p.yx; col.b++;}
    p = p*SCALE-TRANSLATE;
    p = p*rot(0.3*(iTime));
  }

  float d = 0.425*(length(p)-RADIUS) * pow(SCALE, float(-i))/s;
  col/=float(NIFS);
  vec3 oc = mix(vec3(0.7,col.g,0.2),vec3(0.2,col.r,0.7), col.b);
    
  return vec4(oc,d);
}

vec4
map (vec3 p)
{
  return sd2d(p.xz,p.y);
}

float
shadow (vec3 ro, vec3 rd)
{
  float h = 0.;
  float k =3.5;
  float res = 1.;
  float t = 0.2;
  for (int i = 0; t < 15.; i++)
  {
    h = map(ro + rd * t).w;
    res = min(res, k*h / t);
    if (h < HITTHRESHOLD)
      break;
    t = t + h;
  }
  return clamp(res+0.05,0.,1.);
}

void
main ()
{
  float height = -0.4;
  float rot=iTime*0.1;
  float dist= 9.+1.*sin(0.5*iTime);
  vec3 ro = dist * vec3(cos(rot),height,sin(rot));
  vec3 lookAt = vec3 (0.,0.,0.);
  vec3 fw = normalize(lookAt-ro);
  vec3 right = normalize(cross(vec3(0.,1.,1.0), fw));
  vec3 up = normalize(cross (fw, right));
  right = normalize(cross(up,fw));

  rot+=sin(iTime)*0.2;
  vec3 lightPos =  dist * vec3(cos(rot),height,sin(rot));

  vec3 pos, closest;
  float t;
  float smallest;
  int i;
  vec3 sdfCol; 
  vec3 col;

  for (int x=0; x<AA;x++)
  for (int y=0; y<AA;y++)
  {
    t = 0.; smallest = 500.;
    vec2 o = vec2(float(x),float(y)) / float(AA) - 0.5;
    vec2 uv = (gl_FragCoord.xy+o)/iResolution.xy;
    uv -= 0.5;
    uv.x *= iResolution.x/iResolution.y; 
    vec3 rd = normalize( fw *0.5 + right * uv.x + up * uv.y);  

    for ( i=0; i<MAXSTEPS; i++)
    {
      pos = ro + rd *t;   
      vec4 mr = map(pos);
      float d = mr.w;
      if (d < smallest) smallest = d; closest = pos; sdfCol = mr.rgb;
      if (abs(d)<HITTHRESHOLD || t> FAR)
        break;
      t +=d;
    }
    pos = closest;
    vec3 c;
    if (t<FAR)
    { 
      c = sdfCol; 
      vec3 toLight = normalize(lightPos-pos);
      float s = shadow(pos,toLight);
      c*=s; 
      c = mix(c, 1.5*c,1.-s);
    }
    else 
      c = vec3(0.);                
    col += c;
  }
  col/=float(AA*AA);

  gl_FragColor = vec4 (col,t);
}
