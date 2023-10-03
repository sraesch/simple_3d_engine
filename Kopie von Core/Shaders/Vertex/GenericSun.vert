/////////
//// Generic sun Vertex shader
//// Written by Scratchy
//// E-Mail: ScratchyItsch@hotmail.com
/////////
//Varying
varying vec3 SunColor;

void CalculateSunColor()
{
 SunColor=gl_LightSource[0].ambient.rgb;
 SunColor*=(dot(gl_Normal,gl_LightSource[0].spotDirection)+1.0f)/2.0f;
}

void main()
{ 
 //Transform face normal
 gl_Normal=gl_NormalMatrix*gl_Normal;
 gl_Normal=normalize(gl_Normal);
 //Calculate sun color
 CalculateSunColor();
 //Transformated vertex
 gl_Position = ftransform();  
}