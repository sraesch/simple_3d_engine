/////////
//// Generic sun bumpmapping Fragment shader
//// Written by Scratchy
//// E-Mail: ScratchyItsch@hotmail.com
/////////
//Uniforms
uniform sampler2D   	BumpTex;
uniform samplerCUBE 	NormalTex;
//Varying
varying vec3   LiVec;
varying vec2   TexCoord;

void main()
{
 //Fragment direction
 vec3 BT=(texture2D(BumpTex,gl_TexCoord[0].xy).rgb-0.5f)*2.0f;
 //Light direction
 vec3 CT=(textureCube(NormalTex,LiVec).rgb-0.5f)*2.0f;     
 //Calculate light
 gl_FragColor=gl_LightSource[1].ambient*clamp(dot(BT,CT),0.0f,1.0f);
}


