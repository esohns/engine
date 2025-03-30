uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform sampler2D iChannel0;
uniform vec2 iChannel0Resolution;

#define USE_LINEAR_FOR_BUMPMAP
//#define SHOW_NORMAL_MAP
//#define SHOW_ALBEDO

struct C_Sample
{
  vec3 vAlbedo;
  vec3 vNormal;
};
  
C_Sample SampleMaterial (vec2 vUV, sampler2D sampler, vec2 vTextureSize, float fNormalScale)
{
  C_Sample result;

  vec2 vInvTextureSize = vec2(1.0) / vTextureSize;

  vec3 cSampleNegXNegY = texture(sampler, vUV + (vec2(-1.0, -1.0)) * vInvTextureSize.xy).rgb;
  vec3 cSampleZerXNegY = texture(sampler, vUV + (vec2( 0.0, -1.0)) * vInvTextureSize.xy).rgb;
  vec3 cSamplePosXNegY = texture(sampler, vUV + (vec2( 1.0, -1.0)) * vInvTextureSize.xy).rgb;

  vec3 cSampleNegXZerY = texture(sampler, vUV + (vec2(-1.0, 0.0)) * vInvTextureSize.xy).rgb;
  vec3 cSampleZerXZerY = texture(sampler, vUV + (vec2( 0.0, 0.0)) * vInvTextureSize.xy).rgb;
  vec3 cSamplePosXZerY = texture(sampler, vUV + (vec2( 1.0, 0.0)) * vInvTextureSize.xy).rgb;

  vec3 cSampleNegXPosY = texture(sampler, vUV + (vec2(-1.0,  1.0)) * vInvTextureSize.xy).rgb;
  vec3 cSampleZerXPosY = texture(sampler, vUV + (vec2( 0.0,  1.0)) * vInvTextureSize.xy).rgb;
  vec3 cSamplePosXPosY = texture(sampler, vUV + (vec2( 1.0,  1.0)) * vInvTextureSize.xy).rgb;

  vec3 cLSampleNegXNegY = cSampleNegXNegY * cSampleNegXNegY;
  vec3 cLSampleZerXNegY = cSampleZerXNegY * cSampleZerXNegY;
  vec3 cLSamplePosXNegY = cSamplePosXNegY * cSamplePosXNegY;

  vec3 cLSampleNegXZerY = cSampleNegXZerY * cSampleNegXZerY;
  vec3 cLSampleZerXZerY = cSampleZerXZerY * cSampleZerXZerY;
  vec3 cLSamplePosXZerY = cSamplePosXZerY * cSamplePosXZerY;

  vec3 cLSampleNegXPosY = cSampleNegXPosY * cSampleNegXPosY;
  vec3 cLSampleZerXPosY = cSampleZerXPosY * cSampleZerXPosY;
  vec3 cLSamplePosXPosY = cSamplePosXPosY * cSamplePosXPosY;

  result.vAlbedo = ( cLSampleNegXNegY + cLSampleZerXNegY + cLSamplePosXNegY 
                   + cLSampleNegXZerY + cLSampleZerXZerY + cLSamplePosXZerY
                   + cLSampleNegXPosY + cLSampleZerXPosY + cLSamplePosXPosY ) / 9.0;	

  vec3 vScale = vec3(0.3333);

#ifdef USE_LINEAR_FOR_BUMPMAP
  float fSampleNegXNegY = dot(cLSampleNegXNegY, vScale);
  float fSampleZerXNegY = dot(cLSampleZerXNegY, vScale);
  float fSamplePosXNegY = dot(cLSamplePosXNegY, vScale);
    
  float fSampleNegXZerY = dot(cLSampleNegXZerY, vScale);
  float fSampleZerXZerY = dot(cLSampleZerXZerY, vScale);
  float fSamplePosXZerY = dot(cLSamplePosXZerY, vScale);
    
  float fSampleNegXPosY = dot(cLSampleNegXPosY, vScale);
  float fSampleZerXPosY = dot(cLSampleZerXPosY, vScale);
  float fSamplePosXPosY = dot(cLSamplePosXPosY, vScale);
#else
  float fSampleNegXNegY = dot(cSampleNegXNegY, vScale);
  float fSampleZerXNegY = dot(cSampleZerXNegY, vScale);
  float fSamplePosXNegY = dot(cSamplePosXNegY, vScale);
    
  float fSampleNegXZerY = dot(cSampleNegXZerY, vScale);
  float fSampleZerXZerY = dot(cSampleZerXZerY, vScale);
  float fSamplePosXZerY = dot(cSamplePosXZerY, vScale);
    
  float fSampleNegXPosY = dot(cSampleNegXPosY, vScale);
  float fSampleZerXPosY = dot(cSampleZerXPosY, vScale);
  float fSamplePosXPosY = dot(cSamplePosXPosY, vScale);	
#endif

  vec2 vEdge;
  vEdge.x = (fSampleNegXNegY - fSamplePosXNegY) * 0.25 
          + (fSampleNegXZerY - fSamplePosXZerY) * 0.5
          + (fSampleNegXPosY - fSamplePosXPosY) * 0.25;

  vEdge.y = (fSampleNegXNegY - fSampleNegXPosY) * 0.25 
          + (fSampleZerXNegY - fSampleZerXPosY) * 0.5
          + (fSamplePosXNegY - fSamplePosXPosY) * 0.25;

  result.vNormal = normalize(vec3(vEdge * fNormalScale, 1.0));	

  return result;
}

void
main ()
{	
  vec2 vUV = gl_FragCoord.xy / iResolution.xy;

  C_Sample materialSample;

  float fNormalScale = 10.0;
  materialSample = SampleMaterial( vUV, iChannel0, iChannel0Resolution, fNormalScale );

  float fLightHeight = 0.2;
  float fViewHeight = 2.0;

  vec3 vSurfacePos = vec3(vUV, 0.0);

  vec3 vViewPos = vec3(0.5, 0.5, fViewHeight);

  vec3 vLightPos = vec3( vec2(sin(iTime),cos(iTime)) * 0.25 + 0.5 , fLightHeight);

  if( iMouse.z > 0.0 )
  {
    vLightPos = vec3(iMouse.xy / iResolution.xy, fLightHeight);
  }

  vec3 vDirToView = normalize( vViewPos - vSurfacePos );
  vec3 vDirToLight = normalize( vLightPos - vSurfacePos );

  float fNDotL = clamp( dot(materialSample.vNormal, vDirToLight), 0.0, 1.0);
  float fDiffuse = fNDotL;

  vec3 vHalf = normalize( vDirToView + vDirToLight );
  float fNDotH = clamp( dot(materialSample.vNormal, vHalf), 0.0, 1.0);
  float fSpec = pow(fNDotH, 10.0) * fNDotL * 0.5;

  vec3 vResult = materialSample.vAlbedo * fDiffuse + fSpec;

  vResult = sqrt(vResult);

#ifdef SHOW_NORMAL_MAP
  vResult = materialSample.vNormal * 0.5 + 0.5;
#endif

#ifdef SHOW_ALBEDO
  vResult = sqrt(materialSample.vAlbedo);
#endif

  gl_FragColor = vec4(vResult,1.0);
}
