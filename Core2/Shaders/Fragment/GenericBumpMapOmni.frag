/////////
//// Generic bumpmapping Fragment shader for map faces omnilights
//// Written by Scratchy
//// E-Mail: ScratchyItsch@hotmail.com
/////////
//Uniforms
uniform sampler2DShadow ShadowMap;
uniform sampler2D   	BumpTex;
uniform samplerCUBE 	NormalTex;
//Varying
varying vec3   FragPos;
varying vec3   FaceDir;
varying vec3   LiVec;
varying vec4   projCoord;

float ENShadowFac()
{
 vec3 shadowUV = projCoord.xyz / projCoord.q;
 if(shadowUV.z>1.0f||shadowUV.z<0.0f||
    shadowUV.y>1.0f||shadowUV.y<0.0f||
    shadowUV.x>1.0f||shadowUV.x<0.0f)
   return 0.0f;
 else
 {
  float shadowfac=0.0f;
  float diff=1.0f/128.0f;
  shadowfac+=clamp(shadow2D(ShadowMap,shadowUV+vec3(-diff,-diff,0.0f)),0.0f,1.0f);
  shadowfac+=clamp(shadow2D(ShadowMap,shadowUV+vec3(-diff, diff,0.0f)),0.0f,1.0f);
  shadowfac+=clamp(shadow2D(ShadowMap,shadowUV+vec3( diff,-diff,0.0f)),0.0f,1.0f);
  shadowfac+=clamp(shadow2D(ShadowMap,shadowUV+vec3( diff, diff,0.0f)),0.0f,1.0f);
  shadowfac+=clamp(shadow2D(ShadowMap,shadowUV),0.0f,1.0f);
  return clamp(shadowfac/5.0f,0.1f,1.0f); 
 }  
}

float ENLightFunc(vec3 lpos,float lrange,vec3 ldir,float lcut)
{
 vec3 A;
 float res;
 
 A=FragPos-lpos;//Vector from light position to fragment
 res=dot(A,A);
 res/=lrange;
 res=clamp(1-res,0.0f,1.0f);
 A=normalize(A);
 
 return res*ENShadowFac();
}

void main()
{
 //Fragment direction
 vec3 BT=(texture2D(BumpTex,gl_TexCoord[0].xy).rgb-0.5f)*2.0f;
 //Light direction
 vec3 CT=(textureCube(NormalTex,LiVec).rgb-0.5f)*2.0f;
 //Calculate light
 gl_FragColor.rgb=ENLightFunc(vec3(gl_LightSource[0].position),
                        gl_LightSource[0].linearAttenuation,gl_LightSource[0].spotDirection,
                        gl_LightSource[0].spotCutoff)*(dot(BT,CT)+1)/2*gl_LightSource[0].ambient; 
}


