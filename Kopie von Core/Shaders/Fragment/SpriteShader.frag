/////////
//// Sprite shader
//// Written by Scratchy
//// E-Mail: ScratchyItsch@hotmail.com
/////////
//Uniforms
uniform sampler2D Frame1;
uniform sampler2D Frame2;
//Varying
varying vec4 LightColor;

void main()
{
 vec4 c1=texture2D(Frame1,gl_TexCoord[0].xy);
 vec4 c2=texture2D(Frame2,gl_TexCoord[0].xy);
 gl_FragColor=c1+(c2-c1)*gl_TexCoord[1].x;
 gl_FragColor*=LightColor;
}


