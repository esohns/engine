#version 130

uniform vec2 iResolution;
uniform int iFrame;
uniform vec4 iMouse;
uniform sampler2D iChannel2;

#define HOSE vec2(0.5,0.5)
#define ATTRACTOR vec2(1.5,0.5)
#define COL0 vec2(2.5,0.5)
#define COL1 vec2(3.5,0.5)
#define COL2 vec2(4.5,0.5)
#define HOSE_TARGET vec2(5.5,0.5)
#define MOUSE vec2(6.5,0.5)

void
main ()
{
  vec4 fragColor;
  vec2 fragCoord = gl_FragCoord.xy;

    if( fragCoord.y > 1. )
        return;
    
    vec4 hoseData = textureLod(iChannel2, HOSE/iResolution.xy, 0.);
    vec4 attrData = textureLod(iChannel2, ATTRACTOR/iResolution.xy, 0.);
    vec4 col0Data = textureLod(iChannel2, COL0/iResolution.xy, 0.);
    vec4 col1Data = textureLod(iChannel2, COL1/iResolution.xy, 0.);
    vec4 col2Data = textureLod(iChannel2, COL2/iResolution.xy, 0.);
    vec4 hoseTargetData = textureLod(iChannel2, HOSE_TARGET/iResolution.xy, 0.);
    vec4 mouseOld = textureLod(iChannel2, MOUSE/iResolution.xy, 0.);
    
    bool clickEvent = mouseOld.z <= 0. && iMouse.z > 0.;
    bool curDragging = hoseData.z + attrData.z + col0Data.z + col1Data.z + col2Data.z > 0.;
    
    if( fragCoord == HOSE )
    {
        if( iFrame == 0 )
            fragColor = vec4(50., 200., 0., 0.);
        else
            fragColor = hoseData;
        
        vec2 moff = iMouse.xy-fragColor.xy;
        if( iMouse.z <= 0. )
        {
            fragColor.z = 0.;
        }
        else if( (!curDragging && clickEvent && dot(moff,moff)<=11.*11.) || fragColor.z > 0. )
        {
            fragColor.z = 1.;
            fragColor.xy = iMouse.xy;
        }
    }
    else if( fragCoord == ATTRACTOR )
    {
        if( iFrame == 0 )
            fragColor = vec4( 510., 100., 0., 0.);
        else
            fragColor = attrData;
        
        vec2 moff = iMouse.xy-fragColor.xy;
        if( iMouse.z <= 0. )
        {
            fragColor.z = 0.;
        }
        else if( (!curDragging && clickEvent && dot(moff,moff)<=6.*6.) || fragColor.z > 0. )
        {
            fragColor.z = 1.;
            fragColor.xy = iMouse.xy;
        }
    }
    else if( fragCoord == COL0 )
    {
        if( iFrame == 0 )
            fragColor = vec4( iResolution.xy/2. + vec2(-100.,20.), 0., 0.);
        else
            fragColor = col0Data;
        
        vec2 moff = iMouse.xy-fragColor.xy;
        if( iMouse.z <= 0. )
        {
            fragColor.z = 0.;
        }
        else if( (!curDragging && clickEvent && dot(moff,moff)<=50.*50.) || fragColor.z > 0. )
        {
            fragColor.z = 1.;
            fragColor.xy = iMouse.xy;
        }
    }
    else if( fragCoord == COL1 )
    {
        if( iFrame == 0 )
            fragColor = vec4( iResolution.xy/2. + vec2(-130.,-100.), 0., 0.);
        else
            fragColor = col1Data;
        
        vec2 moff = iMouse.xy-fragColor.xy;
        if( iMouse.z <= 0. )
        {
            fragColor.z = 0.;
        }
        else if( (!curDragging && clickEvent && dot(moff,moff)<=50.*50.) || fragColor.z > 0. )
        {
            fragColor.z = 1.;
            fragColor.xy = iMouse.xy;
        }
    }
    else if( fragCoord == COL2 )
    {
        if( iFrame == 0 )
            fragColor = vec4( iResolution.xy/2. + vec2(-20.,-80.), 0., 0.);
        else
            fragColor = col2Data;
        
        vec2 moff = iMouse.xy-fragColor.xy;
        if( iMouse.z <= 0. )
        {
            fragColor.z = 0.;
        }
        else if( (!curDragging && clickEvent && dot(moff,moff)<=50.*50.) || fragColor.z > 0. )
        {
            fragColor.z = 1.;
            fragColor.xy = iMouse.xy;
        }
    }
    else if( fragCoord == HOSE_TARGET )
    {
        if( iFrame == 0 )
            fragColor = vec4(iResolution.xy/2.,0.,0.);
        else
            fragColor = hoseTargetData;
        
        if( !curDragging && iMouse.z > 0. && !clickEvent )
            fragColor.xy = iMouse.xy;
    }
    else if( fragCoord == MOUSE )
    {
        fragColor = iMouse;
    }

    gl_FragColor = fragColor;
}
