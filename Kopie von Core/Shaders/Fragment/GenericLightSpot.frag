/////////
//// Generic lighting Fragment shader for map faces spotlight
//// Written by Scratchy
//// E-Mail: ScratchyItsch@hotmail.com
/////////
//Uniforms
uniform sampler2DShadow ShadowMap;
//Varying
varying vec3   FragPos;
varying vec3   FaceDir;
varying vec4   projCoord;
//Global
vec3 ShadowUV;

float ENLightSpotFac(vec3 lpos,float lrange,vec3 ldir,float lcut)
{
 float res;
 if(lcut>-1.0f&&lcut<1.0f)
 {
  vec3 dir=normalize(lpos-FragPos);
  res=dot(dir,-normalize(ldir));
  res=(lcut-res)/(lcut-1.0f);
  res=clamp(res,0.0f,1.0f);
 }
 else
   res=1.0f;
  

 return res;
}

float ENShadowFac()
{
 float shadowfac=0.0f;
 float diff=1.0f/128.0f;
 shadowfac+=clamp(shadow2D(ShadowMap,ShadowUV+vec3(-diff,-diff,0.0f)),0.0f,1.0f);
 shadowfac+=clamp(shadow2D(ShadowMap,ShadowUV+vec3(-diff, diff,0.0f)),0.0f,1.0f);
 shadowfac+=clamp(shadow2D(ShadowMap,ShadowUV+vec3( diff,-diff,0.0f)),0.0f,1.0f);
 shadowfac+=clamp(shadow2D(ShadowMap,ShadowUV+vec3( diff, diff,0.0f)),0.0f,1.0f);
 shadowfac+=clamp(shadow2D(ShadowMap,ShadowUV),0.0f,1.0f);
 return clamp(shadowfac/5.0f,0.1f,1.0f); 
}

float ENLightFunc(vec3 lpos,float lrange,vec3 ldir,float lcut)
{
 vec3 A;
 float res;
 
 A=lpos-FragPos;//Vector from light position to fragment
 res=1.0f-clamp(dot(A,A)/lrange,0.0f,1.0f);
 A=normalize(A);
 
 return res*ENLightSpotFac(lpos,lrange,ldir,lcut)*dot(normalize(A),FaceDir)*ENShadowFac();
}

void main()
{
 //Calculate shadow coord
 ShadowUV = projCoord.xyz / projCoord.q;
 if(ShadowUV.z<=1.0f&&ShadowUV.z>=0.0f)
 {
  //Set light color 
  gl_FragColor.rgb=gl_LightSource[0].ambient.rgb;
  //Calculate light for fragment
  gl_FragColor.rgb*=ENLightFunc(vec3(gl_LightSource[0].position),
                         gl_LightSource[0].linearAttenuation,gl_LightSource[0].spotDirection,
                         gl_LightSource[0].spotCutoff);
 }
 else
   gl_FragColor.rgb=0.0f;                        
}


