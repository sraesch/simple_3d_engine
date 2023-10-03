/////////
//// Sprite shader
//// Written by Scratchy
//// E-Mail: ScratchyItsch@hotmail.com
/////////
//Varying
varying vec4 LightColor;

void main()
{ 
 //Set light intensitiy
 LightColor=gl_Color; 
 //Set texcoord
 gl_TexCoord[0]=gl_MultiTexCoord0;
 gl_TexCoord[1]=gl_MultiTexCoord1;
 //Transformated vertex
 gl_Position = ftransform();  
}