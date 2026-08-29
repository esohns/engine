#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

float density = .9;

void
main ()
{
  vec4 color;
  vec2 coord = gl_FragCoord.xy;
  vec2 o=vec2(1.)/500.;
  coord /= iResolution.xy;

  color.rg = texture (iChannel0, coord).gr;

  color.r = (texture (iChannel0, coord+o*vec2( 0.,+1.)).r+
             texture (iChannel0, coord+o*vec2( 0,-1)).r+
             texture (iChannel0, coord+o*vec2(-1, 0)).r+
             texture (iChannel0, coord+o*vec2(+1, 0)).r+
             texture (iChannel0, coord+o*vec2(-1,-1)).r+
             texture (iChannel0, coord+o*vec2(+1,-1)).r+
             texture (iChannel0, coord+o*vec2(-1,+1)).r+
             texture (iChannel0, coord+o*vec2(+1,+1)).r+
             texture (iChannel0, coord+o*vec2( 0, 0)).r)*2./9.-
             texture (iChannel0, coord+o*vec2( 0, 0)).g;

  float ba=iTime;
  mat3 boxxfrm=mat3(cos(ba),sin(ba),0,-sin(ba),cos(ba),0,0,0,1)*
               mat3(cos(ba),0,sin(ba),-sin(ba),0,cos(ba),0,1,0)*4.;
  vec3 bp=vec3(coord.x*2.-1.,.1,coord.y*2.-1.);
  vec3 bp2=boxxfrm*bp;
  float bd=length(max(vec3(0.),abs(bp2)-vec3(1.)));
    
  if(bd<1e-3)
   	color.r+=.03;

  float p=.01;
  float c=floor(mod(iTime,64.)/p);
  color.r += (1.-smoothstep(0.,.01,distance(coord,1.5*vec2(cos(c*11.),sin(c*7.1)))))*.8;    
    
  color.r *= density;

  gl_FragColor = color;
}
