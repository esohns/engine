#version 130

uniform vec2 iResolution;
uniform float iTime;

#define Rot(a) mat2(cos(a),-sin(a),sin(a),cos(a))
#define antialiasing(n) n/min(iResolution.y,iResolution.x)
#define S(d,b) smoothstep(antialiasing(1.0),b,d)
#define DF(a,b) length(a) * cos( mod( atan(a.y,a.x)+6.28/(b*8.0), 6.28/((b*8.0)*0.5))+(b-1.)*6.28/(b*8.0) + vec2(0,11) )
#define B(p,s) max(abs(p).x-s.x,abs(p).y-s.y)
#define Tri(p,s,a) max(-dot(p,vec2(cos(-a),sin(-a))),max(dot(p,vec2(cos(a),sin(a))),max(abs(p).x-s.x,abs(p).y-s.y)))

float
SimpleVesicaDistance (vec2 p, float r, float d)
{
  p.x = abs(p.x);
  p.x+=d;
  return length(p)-r;
}

float
triOutside (vec2 p)
{
  vec2 prevP = p;

  p.x*=1.5;

  float d = abs(Tri(p,vec2(0.175),radians(45.)))-0.001;

  p*=1.5;
  p += vec2(0.0,0.05);
  float d2 = abs(Tri(p,vec2(0.175),radians(45.)))-0.001;
  d = min(d,d2);

  p*=1.5;
  p += vec2(0.0,0.06);
  d2 = abs(Tri(p,vec2(0.175),radians(45.)))-0.001;
  d = min(d,d2);

  p = prevP;
  p.y+=0.11;
  d2 = abs(length(p)-0.015)-0.001;    
  d = min(d,d2);

  return d;
}

float
triAnimation (vec2 p)
{
  vec2 prevP = p;

  p.y-=iTime*0.1+0.09;
  p.y = mod(p.y,0.16)-0.08;
  p.y -=0.01;
  p.x*=1.5;
  float d = Tri(p-vec2(0.0,0.032),vec2(0.015),radians(45.)); 
  float d2 = abs(Tri(p,vec2(0.02),radians(45.)))-0.001; 
  d = min(d,d2);
  d2 = abs(Tri(p-vec2(0.0,-0.04),vec2(0.03),radians(45.)))-0.001; 
  d = min(d,d2);

  p = prevP;
  d = max(abs(p.y)-0.06,d);

  return d;
}

float
waveCircle (vec2 p, float s, float numW, float amp, float deg, float thickness)
{
  float r = s+amp*cos(atan(p.y,p.x)*numW);
  float d = abs(length(p)-r)-thickness;    
  p*=Rot(radians(deg));
  r = s+amp*cos(atan(p.y,p.x)*numW);
  float d2 = abs(length(p)-r)-thickness;  
  d = min(d,d2);    
  return d;
}

vec3
centerGraphic (vec2 p, vec3 col)
{
  p*=1.2;
  vec2 prevP = p;
  float thickness = 0.003;
  float deg = -40.;
  float speed0 = -iTime*30.;
  float speed1 = iTime*20.;
  float speed2 = -iTime*10.;
  float speed3 = iTime*15.;
  float speed4 = iTime*25.;

  p*=Rot(radians(speed0));
  p = DF(p,3.0);
  p -= vec2(0.205);

  p*=Rot( radians(deg));
  float a = atan(p.x,p.y);
  float d = abs(length(p)-0.039)-thickness;
  d = max(-p.x,d);

  col = mix(col,vec3(1.)*a,S(d,0.0));

  p = prevP;
  p*=Rot(radians(speed0));

  float deg2 = -44.5;
  p*=Rot( radians(deg2));
  p = DF(p,3.0);
  p -= vec2(0.2);

  p*=-1.;
  p*=Rot( radians(deg));
  a = atan(p.x,p.y);
  d = abs(length(p)-0.04)-thickness;
  d = max(-p.x,d);
  col = mix(col,vec3(1.)*a,S(d,0.0));

  p = prevP;

  d = abs(length(p)-0.29)-thickness;
  col = mix(col,vec3(1.),S(d,0.0));

  p*=Rot(radians(speed1));
  p+=sin(p*25.)*0.025;
  d = abs(length(p)-0.37)-thickness*0.2;
  col = mix(col,vec3(1.),S(d,0.0));

  p = prevP;
  p*=Rot(radians(speed0));

  p = DF(p,3.0);
  p -= vec2(0.217);
  d = abs(length(p)-0.007)-0.0005;
  col = mix(col,vec3(1.),S(d,0.0));

  p = prevP;
  p*=Rot(radians(speed0));

  p*=Rot( radians(deg2));
  p = DF(p,3.0);
  p -= vec2(0.19);
  d = abs(length(p)-0.007)-0.0005;
  col = mix(col,vec3(1.),S(d,0.0));

  p = prevP;
  p*=Rot(radians(speed0));

  p*=Rot( radians(deg2));
  p = DF(p,3.0);
  p -= vec2(0.225);
  d = abs(length(p)-0.015)-0.002;
  col = mix(col,vec3(1.),S(d,0.0));    

  p = prevP;
  p*=Rot(radians(speed0));

  p = DF(p,3.0);
  p -= vec2(0.18);
  d = abs(length(p)-0.015)-0.0005;
  col = mix(col,vec3(1.),S(d,0.0));    

  p = prevP;
  p*=Rot(radians(speed2));
  p = DF(p,6.0);
  p -= vec2(0.161);
  d = max(-(length(p)-0.012),abs(length(prevP)-0.21)-0.02);
  p += vec2(0.024);
  d = max(-(length(p)-0.012),d);

  col = mix(col,vec3(1.),S(abs(d)-0.002,0.0));  

  p = prevP;
  p*=Rot(radians(speed3));
  p = DF(p,6.0);
  p -= vec2(0.11);
  p*=Rot( radians(45.));
  p.y*=2.0;
  d = abs(SimpleVesicaDistance(p,0.12,0.11))-0.0005;
  col = mix(col,vec3(1.),S(d,0.0));        

  p = prevP;
  p*=Rot(radians(speed4));
  p = DF(p,3.0);
  p -= vec2(0.075);
  p*=Rot( radians(45.));
  d = abs(B(p,vec2(0.02,0.01)))-0.002;
  col = mix(col,vec3(1.),S(d,0.0));    

  p = prevP;
  p*=Rot(radians(speed2));
  p*=Rot( radians(90.));
  p = DF(p,1.5);
  p -= vec2(0.04);
  d = abs(length(p)-0.025)-0.0005;
  col = mix(col,vec3(1.),S(d,0.0));        

  p = prevP;
  p*=Rot( radians(-60.));
  p = DF(p,1.5);
  p -= vec2(0.032);
  p*=Rot( radians(45.));
  d = B(p,vec2(0.04,0.003));
  col = mix(col,vec3(1.),S(d,0.0));

  p = prevP;
  p*=Rot( radians(45.));
  d = abs(B(p,vec2(0.015)))-0.001;
  col = mix(col,vec3(1.),S(d,0.0));

  p = prevP;
  p*=Rot(radians(-speed2*0.5));
  p = DF(p,24.0);
  p -= vec2(0.29);
  d = abs(B(p,vec2(0.005, 0.2)))-0.0005;
  col = mix(col,vec3(1.),S(d,0.0));

  p = prevP;
  p*=Rot(radians(-speed3));
  p = DF(p,2.);
  p -= vec2(0.4);
  p*=Rot( radians(45.));

  d = triOutside(p);

  d = max(-(length(prevP)-0.44),d);
  col = mix(col,vec3(1.),S(d,0.0)); 

  p = prevP;
  p*=Rot(radians(-speed3));
  p = DF(p,2.);
  p -= vec2(0.32);
  p*=Rot( radians(45.));
  d = B(p,vec2(0.12, 0.2));

  p = prevP;
  d = max(-d, abs(length(p)-0.47)-0.01);
  col = mix(col,vec3(1.),S(abs(d)-0.001,0.0)); 

  p = prevP;
  p*=Rot(radians(-speed3));
  p = DF(p,2.);
  p -= vec2(0.32);
  p*=Rot( radians(45.));
  d = B(p,vec2(0.08, 0.2));

  p = prevP;
  d = max(-d, abs(length(p)-0.53)-0.01);
  col = mix(col,vec3(1.),S(abs(d)-0.001,0.0));

  p = prevP;
  p = DF(p,1.999);
  p -= vec2(0.475);
  p*=Rot( radians(45.));
  d = triAnimation(p);
  col = mix(col,vec3(1.),S(d,0.0));

  p = prevP;
  p*=Rot(radians(-speed3));
  d = waveCircle(p,0.485,10.,0.052,18.,0.001);

  p = prevP;
  p*=Rot(radians(-speed2));
  float d2 = waveCircle(p,0.585,10.,0.012,18.,0.001);
  d = min(d,d2);
  col = mix(col,vec3(1.),S(d,0.0)); 

  return col;
}

float
graphicItem0 (vec2 p)
{
  vec2 prevP2 = p;
  p.x+=iTime*0.2;
  p.x+=0.2;
  p.x = mod(p.x,0.2)-0.1;
  p.x-=0.02;
  p*=1.5;
  p.x*=-1.;
  vec2 prevP = p;

  float d = B(p,vec2(0.1,0.015));
  float a = radians(45.0);
  p.x = abs(p.x)-0.065;
  d = max(dot(p,vec2(cos(a),sin(a))),d);
  p = prevP;
  float d2 = B(p-vec2(0.09,-0.008),vec2(0.08,0.007));
  d = min(d,d2);
  p.x-=0.145;
  a = radians(55.0);
  d = max(dot(p,vec2(cos(a),sin(a))),d);

  p = prevP;

  p-=vec2(0.0,-0.01);
  d2 = B(p,vec2(0.06,0.01));
  a = radians(45.0);
  p.x = abs(p.x)-0.035;
  d2 = max(dot(p,vec2(cos(a),sin(a))),d2);    
  d = max(-d2,d);

  p = prevP2;
  d = max(abs(p.x)-0.25,d);

  return d;
}

float
graphicItem1 (vec2 p)
{
  vec2 prevP = p;
  p.x-=iTime*0.1;
  p.y-=0.03;
  p.x = mod(p.x,0.06)-0.03;
  float d = abs(Tri(p,vec2(0.02),radians(45.)))-0.001;
  p = prevP;

  p.x+=iTime*0.06;
  p.x+=0.03;
  p.y+=0.03;
  p.x = mod(p.x,0.06)-0.03;
  p.y*=-1.0;

  float d2 = Tri(p,vec2(0.02),radians(45.));
  d = min(d,d2);

  p = prevP;
  d = max(abs(p.x)-0.2,d);

  return d;
}

float
graphicItem2 (vec2 p)
{
  vec2 prevP = p;

  p.x+=iTime*0.08;
  p.x = mod(p.x,0.12)-0.06;
  p.x+=0.05;
  p.x*=-1.;
  p*=Rot(radians(90.));
  p.x*=3.;
  p.y*=0.3;
  float d = Tri(p,vec2(0.03),radians(45.));

  p = prevP;
  d = max(abs(p.x)-0.2,d);

  return d;
}

float
graphicItem3 (vec2 p)
{
  vec2 prevP = p;
    
  p.x-=iTime*0.12;
  p.x = mod(p.x,0.3)-0.15;

  float d = length(p)-0.01;
  float d2 = length(p-vec2(-0.06,0.0))-0.013;
  d = min(d,d2);
  d2 = length(p-vec2(0.06,0.0))-0.007;
  d = min(d,d2);
  d2 = length(p-vec2(-0.12,0.0))-0.014;
  d = min(d,d2);
  d2 = length(p-vec2(0.12,0.0))-0.005;
  d = min(d,d2);

  p = prevP;
  d = max(abs(p.x)-0.25,d);

  return d;
}

float
graphicItem4 (vec2 p)
{
  vec2 prevP = p;
  float d = abs(length(p)-0.04)-0.001;
  p*=Rot(radians(iTime*50.));
  float d2 = abs(length(p)-0.026)-0.006;
  d2 = max(-(abs(p.x)-0.01),d2);
  d = min(d,abs(d2)-0.001);

  return d;
}

float
graphicItem5 (vec2 p)
{
  vec2 prevP = p;
  p.x*=0.8;
  float d = abs(Tri(p,vec2(0.06),radians(45.)))-0.001;
  float d2 = abs(Tri(p,vec2(0.06),radians(45.)))-0.005;
  p.y+=0.05;
  p*=Rot(radians(45.+iTime*-70.));
  d2 = max(-(abs(p.y)-0.02),d2);
  d = min(d,d2);

  return d;
}

float
graphicItem6 (vec2 p)
{
  vec2 prevP = p;
  float d = abs(Tri(p,vec2(0.025),radians(45.)))-0.001;
  float d2 = Tri(p-vec2(0.055,-0.01),vec2(0.015),radians(45.));
  d = min(d,d2);
  return d;
}

float
graphicItem7 (vec2 p)
{
  vec2 prevP = p;
  p.x-=iTime*0.12;
  p.x = mod(p.x,0.07)-0.035;
  p.x-=0.0325;

  p*=vec2(0.9,1.5);
  p*=Rot(radians(90.));
  float d = Tri(p,vec2(0.05),radians(45.));
  d = max(-Tri(p-vec2(0.,-0.03),vec2(0.05),radians(45.)),d);
  d = abs(d)-0.001;
  p = prevP;
  d = max(abs(p.x)-0.15,d);
  return d;
}

float
graphicItems (vec2 p)
{
  vec2 prevP = p;

  p-=vec2(-0.71,-0.33);
  p*=Rot(radians(-25.));
  float d = graphicItem0(p);

  p = prevP;

  p-=vec2(-0.7,-0.16);
  p*=Rot(radians(-15.));
  float d2 = graphicItem1(p);

  d = min(d,d2);

  p = prevP;

  p-=vec2(-0.585,-0.44);
  p*=Rot(radians(-35.));
  d2 = graphicItem2(p);

  d = min(d,d2);

  p = prevP;

  p-=vec2(-0.73,-0.26);
  p*=Rot(radians(-20.));
  d2 = abs(graphicItem3(p))-0.001;

  d = min(d,d2);    

  p = prevP;

  p-=vec2(-0.67,-0.4);
  p*=Rot(radians(-30.));
  d2 = graphicItem3(p);

  d = min(d,d2);    

  p = prevP;

  p-=vec2(-0.78,-0.09);
  p*=Rot(radians(-10.));
  d2 = graphicItem3(p);

  d = min(d,d2);

  p = prevP;
  d2 = graphicItem4(p-vec2(-0.34,-0.45));
  d = min(d,d2); 

  d2 = graphicItem5(p-vec2(-0.48,-0.42));
  d = min(d,d2); 

  d2 = graphicItem6(p-vec2(-0.265,-0.46));
  d = min(d,d2); 

  return d;
}

void
main ()
{
  vec2 p = (gl_FragCoord.xy-0.5*iResolution.xy)/iResolution.y;
  vec2 prevP = p;

  vec3 col = vec3(0.);

  col = centerGraphic(p,col);

  p = abs(p);
  p*=Rot(radians(180.));
  float d = graphicItems(p);
  col = mix(col,vec3(0.6),S(d,0.));

  p = prevP;
  p.x = abs(p.x)-0.77;
  d = graphicItem7(p);
  col = mix(col,vec3(0.6),S(d,0.));

  gl_FragColor = vec4(col,1.0);
}
