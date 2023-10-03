/////////
//// Generic lighting Vertex shader for model faces
//// Written by Scratchy
//// E-Mail: ScratchyItsch@hotmail.com
/////////
//Uniforms
uniform mat4   ModelMatrix;
//Varying
varying vec3   FragPos;
varying vec3   FaceDir;
varying vec4   projCoord;
//Global
vec4 TransVec;

vec4 ShCoord(vec4 vec)
{ 
 vec=gl_TextureMatrix[0]*vec;
 vec.xyz=0.5*(vec3(vec)+vec.w);
 return vec;
}

void main()
{ 
 //Transform vertex
 TransVec=ModelMatrix*gl_Vertex;
 //Transform face normal
 FaceDir=mat3(ModelMatrix)*gl_Normal;
 FaceDir=normalize(FaceDir);  
 //Set fragment position
 FragPos=vec3(TransVec);
 //Coord for the shadow 
 projCoord=ShCoord(TransVec); 
 //Transformated vertex
 gl_Position = ftransform();  
}