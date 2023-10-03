/////////
//// Generic sun bumpmapping Fragment shader
//// Written by Scratchy
//// E-Mail: ScratchyItsch@hotmail.com
/////////
//Uniforms
uniform sampler2D   	BumpTex;
uniform samplerCUBE 	NormalTex;
//Varying
varying mat3   TangentMatrix;

void main()
{
 vec3 LiVec;
 //Calculate light vector 
 LiVec=normalize(TangentMatrix*gl_LightSource[0].spotDirection);
 //Fragment direction
 vec3 BT=(texture2D(BumpTex,gl_TexCoord[0].xy).rgb-0.5f)*2.0f;
 //Light direction
 vec3 CT=(textureCube(NormalTex,LiVec).rgb-0.5f)*2.0f;
 //Calculate light
 gl_FragColor=gl_LightSource[0].ambient*((dot(BT,CT)+1.0f)/2.0f);
}


