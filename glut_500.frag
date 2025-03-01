uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

#define FAR 2.

int id = 0;

vec3
tex3D (sampler2D tex, in vec3 p, in vec3 n)
{
  n = max((abs(n) - .2), .001);
  n /= (n.x + n.y + n.z );

  p = (texture(tex, p.yz)*n.x + texture(tex, p.zx)*n.y + texture(tex, p.xy)*n.z).xyz;

  return p * p;
}

float
n3D (vec3 p)
{
  const vec3 s = vec3(7, 157, 113);
  vec3 ip = floor(p); p -= ip; 
  vec4 h = vec4(0., s.yz, s.y + s.z) + dot(ip, s);
  p = p*p*(3. - 2.*p); //p *= p*p*(p*(p * 6. - 15.) + 10.);
  h = mix(fract(sin(h)*43758.5453), fract(sin(h + s.x)*43758.5453), p.x);
  h.xy = mix(h.xz, h.yw, p.y);
  return mix(h.x, h.y, p.z); // Range: [0, 1].
}

vec2
hash22 (vec2 p)
{
  float n = sin(dot(p, vec2(41, 289)));
  //return fract(vec2(262144, 32768)*n); 

  p = fract(vec2(262144, 32768)*n); 

  return sin( p*6.2831853 + iTime )*.45 + .5; 
}

float
Voronoi (vec2 p)
{
  vec2 g = floor(p), o; p -= g;

  vec3 d = vec3(1);

  for(int y = -1; y <= 1; y++){
    for(int x = -1; x <= 1; x++){
            
      o = vec2(x, y);
            o += hash22(g + o) - p;
            
      d.z = dot(o, o); 
            // More distance metrics.
            //o = abs(o);
            //d.z = max(o.x*.8666 + o.y*.5, o.y);// 
            //d.z = max(o.x, o.y);
            //d.z = (o.x*.7 + o.y*.7);
            
            d.y = max(d.x, min(d.y, d.z));
            d.x = min(d.x, d.z); 
                       
    }
  }
  
    return max(d.y/1.2 - d.x*1., 0.)/1.2;
    //return d.y - d.x; // return 1.-d.x; // etc.
    
}

float
heightMap (vec3 p)
{
    id =0;
    float c = Voronoi(p.xy*4.);

    if (c<.07)
    {
      c = smoothstep(0.7, 1., 1.-c)*.2; id = 1;
    }

    return c;
}

float
m (vec3 p)
{
  float h = heightMap(p); // texture(iChannel0, p.xy/2.).x; // Texture work too.
    
  return 1. - p.z - h*.1;
}

vec3
nr (vec3 p, inout float edge)
{
  vec2 e = vec2(.005, 0);

  float d1 = m(p + e.xyy), d2 = m(p - e.xyy);
  float d3 = m(p + e.yxy), d4 = m(p - e.yxy);
  float d5 = m(p + e.yyx), d6 = m(p - e.yyx);
  float d = m(p)*2.;

  edge = abs(d1 + d2 - d) + abs(d3 + d4 - d) + abs(d5 + d6 - d);
  //edge = max(max(abs(d1 + d2 - d), abs(d3 + d4 - d)), abs(d5 + d6 - d)); // Etc.
    
  edge = smoothstep(0., 1., sqrt(edge/e.x*2.));
  
  return normalize(vec3(d1 - d2, d3 - d4, d5 - d6));
}

vec3
eMap (vec3 rd, vec3 sn)
{
  vec3 sRd = rd; // Save rd, just for some mixing at the end.
    
  // Add a time component, scale, then pass into the noise function.
  rd.xy -= iTime*.25;
  rd *= 3.;
    
  //vec3 tx = tex3D(iChannel0, rd/3., sn);
  //float c = dot(tx*tx, vec3(.299, .587, .114));
    
  float c = n3D(rd)*.57 + n3D(rd*2.)*.28 + n3D(rd*4.)*.15; // Noise value.
  c = smoothstep(0.5, 1., c); // Darken and add contast for more of a spotlight look.
    
  //vec3 col = vec3(c, c*c, c*c*c*c).zyx; // Simple, warm coloring.
  vec3 col = vec3(min(c*1.5, 1.), pow(c, 2.5), pow(c, 12.)).zyx; // More color.
    
  // Mix in some more red to tone it down and return.
  return mix(col, col.yzx, sRd*.25+.25); 
}

void
main ()
{
  vec3 r = normalize(vec3(gl_FragCoord.xy - iResolution.xy*.5, iResolution.y)), 
        o = vec3(0), l = o + vec3(0, 0, -1);

  vec2 a = sin(vec2(1.570796, 0) + iTime/8.);
  r.xy = mat2(a, -a.y, a.x) * r.xy;

  float d, t = 0.;

  for(int i=0; i<32;i++)
  {
    d = m(o + r*t);
    // There isn't really a far plane to go beyond, but it's there anyway.
    if(abs(d)<0.001 || t>FAR) break;
    t += d*.7;
  }

  t = min(t, FAR);

  // Set the initial scene color to black.
  gl_FragColor = vec4(0);

  float edge = 0.; // Edge value - to be passed into the normal.

  if(t<FAR)
  {
    vec3 p = o + r*t, n = nr(p, edge);

    l -= p;
    d = max(length(l), 0.001);
    l /= d;

    float hm = heightMap(p);
        
    vec3 tx = tex3D (iChannel0, (p*2. + hm*.2), n);
    //tx = floor(tx*15.999)/15.;

    gl_FragColor.xyz = vec3(1.)*(hm*.8 + .2);

    gl_FragColor.xyz *= vec3(1.5)*tx;

    gl_FragColor.x = dot(gl_FragColor.xyz, vec3(.299, .587, .114));
    if (id == 0)
      gl_FragColor.xyz *= vec3(min(gl_FragColor.x*1.5, 1.), pow(gl_FragColor.x, 5.), pow(gl_FragColor.x, 24.))*2.;
    else
      gl_FragColor.xyz *= .1;

    float df = max(dot(l, n), 0.);
    float sp = pow(max(dot(reflect(-l, n), -r), 0.), 32.);
        
    if(id == 1)
      sp *= sp;

    gl_FragColor.xyz = gl_FragColor.xyz*(df + .75) + vec3(1, .97, .92)*sp + vec3(.5, .7, 1)*pow(sp, 32.);

    vec3 em = eMap(reflect(r, n), n);
    if (id == 1)
      em *= .5;
    gl_FragColor.xyz += em;

    //if(id == 0) gl_FragColor.xyz += edge*.1;
    gl_FragColor.xyz *= 1. - edge*.8;

    gl_FragColor.xyz *= 1./(1. + d*d*.125);
  }

  gl_FragColor = vec4(sqrt(clamp(gl_FragColor.xyz, 0., 1.)), 1.);
}
