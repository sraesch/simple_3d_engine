/////////
//// Generic lighting Vertex shader for map faces
//// Written by Scratchy
//// E-Mail: ScratchyItsch@hotmail.com
/////////
//Varying
varying vec3   FragPos;
varying vec3   FaceDir;
varying vec4   projCoord;

vec4 ShCoord(vec4 vec)
{ 
 vec=gl_TextureMatrix[0]*vec; 
 vec.xyz=0.5*(vec3(vec)+vec.w);
 return vec;
}

void main()
{ 
 //Set face normal
 FaceDir=gl_Normal;
 //Set fragment position
 FragPos=vec3(gl_Vertex);
 //Coord for the shadow 
 projCoord=ShCoord(gl_Vertex); 
 //Transformated vertex
 gl_Position = ftransform();  
}