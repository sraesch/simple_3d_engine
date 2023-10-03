/////////
//// Generic omnilight Fragment shader
//// Written by Scratchy
//// E-Mail: ScratchyItsch@hotmail.com
/////////
//Uniforms
uniform sampler2DShadow ShadowMap;
//Varying
varying vec3   FragPos;
varying vec3   FaceDir;
varying vec4   projCoord;
uniform int    EnableShadow;
//Global
vec3 ShadowUV;

#define diff 0.0078125f

float ENShadowFac()
{
 float shadowfac=0.0f; 
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
 if(EnableShadow>0)
   return res*dot(normalize(A),FaceDir)*ENShadowFac();
 else   
   return res*dot(normalize(A),FaceDir);
}

void main()
{
 //Calculate shadow coord
 ShadowUV = projCoord.xyz / projCoord.q;
 //Draw lightdata for fragment if visible
 if((ShadowUV.z<=1.0f&&ShadowUV.z>=0.0f&&
     ShadowUV.y<=1.0f&&ShadowUV.y>=0.0f&&
     ShadowUV.x<=1.0f&&ShadowUV.x>=0.0f)||EnableShadow<=0)
 {
  //Set light color 
  gl_FragColor.rgb=gl_LightSource[0].ambient.rgb;
  //Calculate light for fragment
  gl_FragColor.rgb*=ENLightFunc(vec3(gl_LightSource[0].position),
                                gl_LightSource[0].linearAttenuation,
                                gl_LightSource[0].spotDirection,
                                gl_LightSource[0].spotCutoff);
 }
 else
   gl_FragColor.rgb=0.0f;
}


