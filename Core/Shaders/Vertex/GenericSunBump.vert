/////////
//// Generic sun bumpmapping Vertex shader
//// Written by Scratchy
//// E-Mail: ScratchyItsch@hotmail.com
/////////
//Varying
varying vec3   LiVec;
varying vec2   TexCoord;
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

void main()
{ 
 //Tangenten matrix
 TangentMatrix=ENTangentMatrix(gl_Normal);
 //TexCoord for the texture
 TexCoord=gl_MultiTexCoord0;
 //Calculate light vector 
 vec3 tldir=mat3(gl_TextureMatrix[0])*gl_LightSource[1].spotDirection;
 LiVec=normalize(TangentMatrix*tldir);
 //Transformated vertex
 gl_Position = ftransform();  
}