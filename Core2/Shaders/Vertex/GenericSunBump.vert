/////////
//// Generic sun bumpmapping Vertex shader
//// Written by Scratchy
//// E-Mail: ScratchyItsch@hotmail.com
/////////
//Varying
varying mat3   TangentMatrix;
//Global
vec4 TransVec;

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
 //Transform face normal
 gl_Normal=gl_NormalMatrix*gl_Normal;
 gl_Normal=normalize(gl_Normal);
 //Transform vertex
 TransVec=gl_ModelViewMatrix*gl_Vertex;
 //Tangenten matrix
 TangentMatrix=ENTangentMatrix(gl_Normal);   
 //TexCoord for the texture
 gl_TexCoord[0]=gl_MultiTexCoord0;
 //Transformated vertex
 gl_Position = ftransform();  
}