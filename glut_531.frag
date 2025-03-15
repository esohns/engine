uniform vec2 iResolution;
uniform sampler2D iChannel3;

#define CUBECOUNT 16u

vec4 readTex3(uint cx,uint cy)
{
    return textureLod(iChannel3,vec2((float(cx)+0.1)/iResolution.x,(float(cy)+0.1)/iResolution.y),0.);
}

vec3 getCubePos(uint ci)
{
    return readTex3(ci,0u).xyz;
}
vec4 getCubeQuat(uint ci)
{
    return readTex3(ci,1u).xyzw;
}
vec3 rotate(vec4 quat,vec3 v)
{
    float sinsqr = (1.0-quat.w*quat.w);
    if (sinsqr!=0.0)
    {
        v=v*quat.w + quat.xyz*((dot(v,quat.xyz)*(1.0-quat.w))*(1.0/sinsqr)) + cross(v,quat.xyz);
        v=v*quat.w + quat.xyz*((dot(v,quat.xyz)*(1.0-quat.w))*(1.0/sinsqr)) + cross(v,quat.xyz);
    }
    return v;
}
vec3 cubeTransform(uint ci,vec3 lp)
{
    lp.z*=0.5;
    return getCubePos(ci) + rotate(getCubeQuat(ci),lp);
}

vec4 getCubePlane(uint ci,uint k)
{
    vec3 norm = vec3( k%3u==1u?1.0:0.0, k%3u==0u?1.0:0.0, k%3u==2u?2.0:0.0)*(float(k/3u)*-2.0+1.0);
    norm = rotate(getCubeQuat(ci),norm);
    float offset = 1.0 + dot(getCubePos(ci),norm);
    return vec4(norm,offset);
}


vec3 getWCubeVert(uint ci,uint j)
{
    return cubeTransform(ci,vec3(float(j&1u),float((j&2u)/2u),float((j&4u)/4u))*2.0-1.0);
}
vec3 rotateAxisAngle(vec3 axis,float angle,vec3 v)
{
  v = v*cos(angle) + axis*((v*axis) * (1.0-cos(angle))) + cross(v,axis)*sin(angle);
    return v;
}

uint pixelx,pixely;

vec4 findSeparatingPlane_planA()
{

    uint cia = pixelx/6u;
    uint cib = pixely/5u;
    
    if (cia>=CUBECOUNT) discard;
    if (cia<=cib) discard;
    
    
    float bestoffset=-1e30;
    vec3 bestplane;
    float bestplaneoffset;
    
//    for(uint m=0;m<2;m++)
//    for(uint k=0;k<30;k++)
    uint k = (pixelx%6u)+(pixely%5u)*6u;
    {
        vec3 sep;
        vec3 edgea = vec3( k%3u==0u?1.0:0.0, k%3u==1u?1.0:0.0, k%3u==2u?1.0:0.0);
        edgea = rotate(getCubeQuat(cia),edgea);
        vec3 edgeb = vec3( k/3u%3u==0u?1.0:0.0, k/3u%3u==1u?1.0:0.0, k/3u%3u==2u?1.0:0.0);
        edgeb = rotate(getCubeQuat(cib),edgeb);
        if (k%15u<9u)
        { 
            // edge crossings
            if (length(cross(edgea,edgeb))<0.001)
                sep = vec3(0.,0.,0.);  // parallel edges fail
            else
              sep = normalize(cross(edgea,edgeb));
        }
        else
        {  // normals
            if (k<9u+3u)
            {
                sep = edgea;
            }
            else
            {
                sep =  vec3( k%3u==0u?1.0:0.0, k%3u==1u?1.0:0.0, k%3u==2u?1.0:0.0);
                sep = rotate(getCubeQuat(cib),sep);
            }
        }
        if (k>=15u) sep=-sep;
        
        if (cib==0u)
        {
            sep = vec3(0.,-1.,0.);
        }
        
        float minoffset = -1e30;
        for(uint j=0u;j<8u;j++)
        {
            vec3 v = getWCubeVert(cia,j);
            if (dot(v,sep)>minoffset)
            {
                minoffset = dot(v,sep);
            }
        }

        float maxoffset = 1e30;
        for(uint j=0u;j<8u;j++)
        {
            vec3 v = getWCubeVert(cib,j);
            if (dot(v,sep)<maxoffset)
            {
                maxoffset = dot(v,sep);
            }
        }
        float offset = -minoffset+maxoffset;
        
//        if (offset>bestoffset && offset!=0.0) // no improvement
        {
            bestoffset = offset;
            bestplaneoffset = (minoffset+maxoffset)*0.5;
            bestplane = sep;
        }
    }
    
    if (bestoffset>=0.0)
    {
        return vec4(0.,9999.0,0.,0.);
    }
    
    return vec4(-bestplane*(2.0-bestoffset),-bestplaneoffset);
}

// this alg isn't good, beacuse it finds some local maximum instead of the best solution, 
// but it works ok if the cubes are uintersecting, a separating plane cannot be put, 
// but the plane with the least overlap is found, which is needed in the solver.
vec4 findSeparatingPlane_planB()  
{
    if (pixelx<=pixely) discard;
    if (pixelx>=(CUBECOUNT)) discard;    

    uint cia = pixelx;
    uint cib = pixely;

    if ((length(getCubePos(cia)-getCubePos(cib))>6.0 && cib!=0u)) discard;
    
    vec3 sep = normalize(getCubePos(cib)-getCubePos(cia));
    float offset =0.0;
    float dangle = 0.2;
    float lastoffset=-1e30;
    vec3 lastsep=sep;
    vec3 diff = vec3(0.0,0.0,0.0);
    
    for(uint k=0u;k<64u;k++)
    {
        if (cib==0u)
        {
            sep = vec3(0.,-1.,0.);
        }
        
        float minoffset = -1e30;
        vec3 minvert=vec3(0.0,0.0,0.0);
        for(uint j=0u;j<8u;j++)
        {
            vec3 v = getWCubeVert(cia,j);
            if (dot(v,sep)>minoffset)
            {
                minoffset = dot(v,sep);
                minvert = v;
            }
        }

        float maxoffset = 1e30;
        vec3 maxvert=vec3(0.0,0.0,0.0);
        for(uint j=0u;j<8u;j++)
        {
            vec3 v = getWCubeVert(cib,j);
            if (dot(v,sep)<maxoffset)
            {
                maxoffset = dot(v,sep);
                maxvert = v;
            }
        }
        offset = dot(maxvert-minvert,sep);
        
        dangle*=1.2;
        if (offset<lastoffset) // no improvement
        {
            sep=lastsep;
            dangle*=0.5/1.2;
            offset = lastoffset;
        }
        else
        {
           diff = maxvert-minvert;
        }
        
        vec3 axis = normalize(cross(diff,sep));
        lastsep = sep;
        lastoffset = offset;
        sep = rotateAxisAngle(axis,dangle,sep);
        offset = (maxoffset+minoffset)*0.5;

    }
    
    return vec4(sep,offset);
}


vec3 edge(uint i,uint j)
{
    vec3 pa,pb;
    if (i<8u)
    {
        pa.x = float(i%4u<2u?1:-1);
        pa.y = float((i+1u)%4u<2u?1:-1);
        pa.z = float(i/4u<1u?1:-1);

        pb.x = float((i+1u)%4u<2u?1:-1);
        pb.y = float((i+2u)%4u<2u?1:-1);
        pb.z = float(i/4u<1u?1:-1);
    }
    else
    {
        pa.x = float(i%4u<2u?1:-1);
        pa.y = float((i+1u)%4u<2u?1:-1);
        pa.z = -1.0;
        pb = vec3(pa.xy,1.0);
                
    }
    
    return j==0u?pa:pb;
}

vec4 findCollisionPouint()
{
    uint ci = pixelx/12u;
    uint cj = pixely/4u;
    if (cj>=ci) discard;
    
    if (length(getCubePos(ci)-getCubePos(cj))>6.0 && cj!=0u) // bounding check
    {
        return vec4(0.,0.,0.,0.);
    }
    
    uint j = pixelx%12u;
    
    if (pixely%4u<2u) // swap the two cubes to check collision both ways
    {
        uint t = ci;
        ci = cj;
        cj = t;
    }

    vec3 pa = cubeTransform(cj,edge(j,0u)); // a world space edge of cube j
    vec3 pb = cubeTransform(cj,edge(j,1u));
    float ea=0.0;
    float eb=1.0;
    for(uint l=0u;l<((ci==0u)?1u:6u);l++) // clamp it with the 6 planes of cube i
    {
        vec4 pl = getCubePlane(ci,l);
        pl/=length(pl.xyz);
        if (abs(dot(pl.xyz,pb-pa))>0.0001)
        {
            float e = -(dot(pl.xyz,pa)-pl.w)/dot(pl.xyz,pb-pa);
            if (dot(pb-pa,pl.xyz)>0.0)
            {
                eb=min(eb,e);
            }
            else
            {
                ea=max(ea,e);
            }
        }
        else
        {
            ea=999999.0; // edge is parallel to plane
        }
    }
    
    vec3 coll = pa+(pb-pa)*((pixely%2u==0u)?ea:eb);
    if (eb<=ea || cj==0u)
    {
        coll = vec3(0.,0.,0.);
    }
    
    
    return  vec4(coll,0.0);
}

void main ()
{
  pixelx = uint(floor(gl_FragCoord+0.01).x);
  pixely = uint(floor(gl_FragCoord+0.01).y);

  if (pixelx>=(CUBECOUNT*12u))
  {
    pixelx-=(CUBECOUNT*12u);
    gl_FragColor = findSeparatingPlane_planA();
    return;
  }

  gl_FragColor = findCollisionPouint();   
}
