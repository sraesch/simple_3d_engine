/////////
//// Generic bumpmapping Vertex shader for model faces
//// Written by Scratchy
//// E-Mail: ScratchyItsch@hotmail.com
/////////
//Uniforms
uniform mat4  ModelMatrix;
uniform mat3  RotMatrix;
//Varying
varying vec3   FragPos;
varying vec3   LiVec;
varying vec4   projCoord;
//Global
mat3 TangentMatrix;

mat3 ENTangentMatrix(vec3 C)
{
 vec3 A,B;
 mat3 res; 
 vec3 Abs=abs(C);
 if(Abs.x>=Abs.y&&Abs.x>=Abs.z)
   A=vec3(C.z,C.y,C.z-C.x);
 else
   if(Abs.y>=Abs.x&&Abs.y>=Abs.z)
     A=vec3(C.y,-C.x,C.z);
   else
	 if(Abs.z>=Abs.x&&Abs.z>Abs.y)
   	   A=vec3(C.x,-C.z,C.y);

 B=cross(A,C);
 B=normalize(B);
 //Fill matrix
 return mat3(A.x,B.x,C.x,A.y,B.y,C.y,A.z,B.z,C.z);
}

vec3 CalculateLightVec(vec3 lpos)
{
 vec3 res;
 res=RotMatrix*(lpos-FragPos); 
 res=normalize(TangentMatrix*res); 
 return res;
}

vec4 ShCoord(vec4 vec)
{ 
 vec=gl_TextureMatrix[0]*vec;
 vec.xyz=0.5*(vec3(vec)+vec.w);
 return vec;
}

void main()
{ 
 //Transform vertex 
 FragPos=ModelMatrix*gl_Vertex; 
 //Coord for the shadow 
 projCoord=ShCoord(vec4(FragPos,1));
 //Tangenten matrix
 TangentMatrix=ENTangentMatrix(gl_Normal);  
 //TexCoord for the texture
 gl_TexCoord[0]=gl_MultiTexCoord0;
 //Calculate light vector 
 LiVec=CalculateLightVec(vec3(gl_LightSource[0].position));
 //Transformated vertex
 gl_Position = ftransform(); 
}