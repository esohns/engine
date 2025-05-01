#version 130

uniform vec2 iResolution;
uniform int iFrame;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;
uniform sampler2D iChannel3;

#define R iResolution.xy

vec4 A (vec2 U) {return texture(iChannel0,U/R);}
vec4 B (vec2 U) {return texture(iChannel1,U/R);}
vec4 D (vec2 U) {return texture(iChannel3,U/R);}
vec4 T (vec2 U) {return A(U-A(U).xy);}

float ln (vec2 p, vec2 a, vec2 b) {return length(p-a-(b-a)*clamp(dot(p-a,b-a)/dot(b-a,b-a),0.,1.));}

void
main ()
{
  vec4 Q;
  vec2 U = gl_FragCoord.xy;

  Q = T(U);
  vec4 b = B(U),
       n = T(U+vec2(0,1)),
       e = T(U+vec2(1,0)),
       s = T(U-vec2(0,1)),
       w = T(U-vec2(1,0));
  Q.x -= .25*(e.z-w.z+Q.w*(n.w-s.w));
  Q.y -= .25*(n.z-s.z+Q.w*(e.w-w.w));
  Q.z  = .25*((s.y-n.y+w.x-e.x)+(n.z+e.z+s.z+w.z));
  Q.w  = .25*((s.x-n.x+w.y-e.y)-Q.w);
  float p = smoothstep(2.,0.,length(U-b.xy)-b.z);
  Q.z += .03*p;
  Q.z*=0.975;

  vec4 mo = texture(iChannel3,vec2(0));
  float l = ln(U,mo.xy,mo.zw);
  if (mo.z > 0. && length(mo.xy-mo.zw)>0.)
    Q.xy += .01*(mo.xy-mo.zw)*smoothstep(40.,0.,l);

  if (U.x < 2. || U.y < 2. || R.x-U.x<2.)
    Q.xy=mix(Q.xy,vec2(-.3,0),.99);

  if (iFrame < 1)
    Q = vec4(-.3,0,0,0);

  gl_FragColor = Q;
}
