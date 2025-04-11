#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;

#define AA 4

#define SPHERE 1.0 
//#define SPHERE 0.0 

#define RAY_STEPS_PRIMARY 120
#define RAY_STEPS_SECOND 40

float
df2obj (vec3 p)
{
  p.x += iTime - 2.0;
  vec3 op = p;
  vec3 p2 = p;
  p.xz = mod(p.xz + vec2(2.0), 4.0) - vec2(2.0);
  float box = max(abs(p.x), max(abs(p.y), abs(p.z)));
  float sphere = length(p);
  return (mix(box, sphere, SPHERE) - 1.0);
}

float
df2floor (vec3 p)
{
  return p.y + 1.0;
}

float
df2 (vec3 p)
{
  return min(df2obj(p), df2floor(p));
}

mat2
matr (float a)
{
  float c = cos(a), s = sin(a);
  return mat2(c, s, -s, c);
}

#define NF(R,P,F) { vec2 e = vec2(0.0, 0.01); R = normalize(vec3(F(P-e.yxx),F(P-e.xyx),F(P-e.xxy))); }
#define RM(R,P,D,F,S) { R = P+D; for(int i=0; i<S; i++) { float t = F(R); R += t*D; } }

vec3
shading (vec3 p, vec3 d, vec3 n)
{
  vec3 c = vec3(0.0);

  vec3 bg = (d * 0.5 + 0.5) * 0.5;
  bg += sin(d.y * 4.0) * 0.5 + 0.5;
  bg *= max(cos(iTime * 0.2 - 10.0) *0.7 + 0.5, 0.0);

  if (df2(p) < 0.1)
  {
    float lt = iTime - 14.0;
    vec3 l = vec3(sin(lt * 0.1) * 14.0, 4.0 + cos(lt * 0.23) * 2.0, cos(lt * 0.13) * 14.0);
    vec3 ldir = normalize(vec3(p - l));
    float oa = (df2(p-n)+df2(p-n*.5)*2.0+df2(p-n*.25)*4.0)*.5*.3333+.5; //ambient occlusion
    float od = max(min(min(df2(p-ldir*.3)/0.3,df2(p-ldir)),df2(p-ldir*0.6)/.6),.0); //shadow
    float dist = distance(p,l);
        
    float spec = pow(max(.0,dot(normalize(reflect(d,n)),-ldir)*.5+.5),1000.0)*56.0; //specular component
        
    c = vec3(0.8, 0.7, 0.6) * max(0.0, dot(n, ldir) * 0.5) / (1.0 + dist * 0.1) * (od * 0.8 + 0.2) * 2.0;//diffuse component
    c += vec3(spec*od);
    c *= oa;
    c = mix(bg,c,1.0/(1.0+length(p)*0.01));//fogging
  }
  else
    c = bg;

  return c;
}

vec4
render (vec3 pos, vec3 dir)
{
  vec3 p = vec3(0.0);
    
  RM(p, pos, dir, df2, RAY_STEPS_PRIMARY);
  vec3 norm;
  NF(norm, p, df2);

  float fres = 1.0 - dot(dir, norm) * 0.9;
  vec4 color = vec4(0.0);

  float dist = distance(pos, p);
  color.w = dist;

  if (df2(p) < 0.1)
  {
    vec3 p2, n2, d2 = reflect(dir, norm);
    RM(p2, p, d2, df2, RAY_STEPS_SECOND);
    NF(n2, p2, df2);

    color.rgb += shading(p2, d2, n2) * fres;

    if (df2(p2) > 0.1)
      color.rgb += shading(p, dir, norm);
  }
  else
    color.rgb = shading(p,dir, norm);

  return color;
}

void
main ()
{
  vec3 pos = vec3(0.0, 0.0, -8.0);
  vec2 muvs = (iMouse.xy/iResolution.xy);
  if (dot(muvs, muvs) < 0.0005)
  {
    muvs.y = 0.7;
    muvs.x = 0.15;
  }

  mat2 rx = matr(muvs.y - 1.0);
  mat2 ry = matr(muvs.x * 4.0 + 4.5);
  pos.yz *= rx;
  pos.xz *= ry;

#if AA<2
  vec2 p = (2.0 * gl_FragCoord.xy - iResolution.xy) / iResolution.y;
  vec3 dir = normalize(vec3(p, 2.0));
  dir.yz *= rx;
  dir.xz *= ry;

  vec4 color = render(pos, dir);
  float dist = color.w;
#else
  vec4 color = vec4(0.0);
  for( int m=0; m<AA; m++ )
  for( int n=0; n<AA; n++ )
  {
    vec2 rr = vec2(float(m), float(n)) / float(AA) - 0.5;
    vec2 p = (2.0 * (gl_FragCoord.xy + rr) - iResolution.xy) / iResolution.y;

    vec3 dir = normalize(vec3(p, 2.0));
    dir.yz *= rx;
    dir.xz *= ry;

    color += render(pos, dir);
  }
  color.xyzw /= float(AA*AA);
#endif

  color.rgb = max(color.rgb, 0.0);

  gl_FragColor = vec4(color.rgb, color.w);
}
