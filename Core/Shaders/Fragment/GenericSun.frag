/////////
//// Generic sun Fragment shader
//// Written by Scratchy
//// E-Mail: ScratchyItsch@hotmail.com
/////////
//Varying
varying vec3 SunColor;

void main()
{
 gl_FragColor=vec4(SunColor,1.0f);
}


