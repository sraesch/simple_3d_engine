//---------------------------------------------------------------------------
#ifndef EngineSystemH
#define EngineSystemH
//---------------------------------------------------------------------------
#include <stdio.h>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/wglext.h>
//////////////////////////////////////////////////////////////
///BASIC DATA TYPES
//////////////////////////////////////////////////////////////
typedef bool				ENbool;
typedef signed   char		        ENbyte;
typedef unsigned char		        ENubyte;
typedef signed   int		        ENint;
typedef unsigned int		        ENuint;
typedef signed short    	        ENshort;
typedef unsigned short  	        ENushort;
typedef float				ENfloat;
typedef double				ENdouble;
typedef unsigned long                   ENhandle;
typedef FILE*                           ENFile;
typedef long                            ENenum;
//////////////////////////////////////////////////////////////
///ADVANCED DATA TYPES
//////////////////////////////////////////////////////////////
struct ENVector4
{
 __declspec(dllexport) ENVector4();
 __declspec(dllexport) ENVector4(const ENfloat _x,const ENfloat _y,const ENfloat _z,const ENfloat _w);
 __declspec(dllexport) ENVector4 operator+ (const ENVector4& vVector) const;
 __declspec(dllexport) ENVector4 operator+ (const ENfloat val) const;
 __declspec(dllexport) ENVector4 operator- (const ENVector4& vVector) const;
 __declspec(dllexport) ENVector4 operator- (const ENfloat val) const;
 __declspec(dllexport) ENVector4 operator* (const ENVector4& vVector) const;
 __declspec(dllexport) ENVector4 operator* (const ENfloat num) const;
 __declspec(dllexport) ENVector4 operator/ (const ENfloat num) const;
 __declspec(dllexport) ENbool   operator==(const ENVector4& vVector) const;
 __declspec(dllexport) ENbool   operator!=(const ENVector4& vVector) const;
 __declspec(dllexport) ENbool   operator<(const ENVector4& vVector) const;
 __declspec(dllexport) ENbool   operator>(const ENVector4& vVector) const;
 __declspec(dllexport) ENbool   operator<=(const ENVector4& vVector) const;
 __declspec(dllexport) ENbool   operator>=(const ENVector4& vVector) const;
 ENfloat v[4];
};

struct ENVector
{
 __declspec(dllexport) ENVector();
 __declspec(dllexport) ENVector(const ENfloat _x,const ENfloat _y,const ENfloat _z);
 __declspec(dllexport) ENVector operator+ (const ENVector& vVector) const;
 __declspec(dllexport) ENVector operator+ (const ENfloat  val) const;
 __declspec(dllexport) ENVector operator- (const ENVector& vVector) const;
 __declspec(dllexport) ENVector operator- (const ENfloat  val) const;
 __declspec(dllexport) ENVector operator* (const ENVector& vVector) const;
 __declspec(dllexport) ENVector operator* (const ENfloat num) const;
 __declspec(dllexport) ENVector operator/ (const ENfloat num) const;
 __declspec(dllexport) ENbool   operator==(const ENVector& vVector) const;
 __declspec(dllexport) ENbool   operator!=(const ENVector& vVector) const;
 __declspec(dllexport) ENbool   operator<(const ENVector& vVector) const;
 __declspec(dllexport) ENbool   operator>(const ENVector& vVector) const;
 __declspec(dllexport) ENbool   operator<=(const ENVector& vVector) const;
 __declspec(dllexport) ENbool   operator>=(const ENVector& vVector) const;
 ENfloat v[3];
};

struct ENVector2
{
 __declspec(dllexport) ENVector2();
 __declspec(dllexport) ENVector2(const ENfloat _x,const ENfloat _y);
 __declspec(dllexport) ENVector2 operator+ (const ENVector2& vVector) const;
 __declspec(dllexport) ENVector2 operator+ (const ENfloat val) const;
 __declspec(dllexport) ENVector2 operator- (const ENVector2& vVector) const;
 __declspec(dllexport) ENVector2 operator- (const ENfloat val) const;
 __declspec(dllexport) ENVector2 operator* (const ENVector2& vVector) const;
 __declspec(dllexport) ENVector2 operator* (const ENfloat num) const;
 __declspec(dllexport) ENVector2 operator/ (const ENfloat num) const;
 __declspec(dllexport) ENbool   operator==(const ENVector2& vVector) const;
 __declspec(dllexport) ENbool   operator!=(const ENVector2& vVector) const;
 __declspec(dllexport) ENbool   operator<(const ENVector2& vVector) const;
 __declspec(dllexport) ENbool   operator>(const ENVector2& vVector) const;
 __declspec(dllexport) ENbool   operator<=(const ENVector2& vVector) const;
 __declspec(dllexport) ENbool   operator>=(const ENVector2& vVector) const;
 ENfloat v[2];
};

struct ENColor
{
 __declspec(dllexport) ENColor();
 __declspec(dllexport) ENColor(ENubyte red,ENubyte green,ENubyte blue,ENubyte alpha);
 __declspec(dllexport) ENColor operator+ (ENColor vColor);
 __declspec(dllexport) ENColor operator- (ENColor vColor);
 __declspec(dllexport) ENColor operator* (ENfloat num);
 __declspec(dllexport) ENColor operator/ (ENfloat num);
 __declspec(dllexport) ENbool   operator==(ENColor vColor);
 __declspec(dllexport) ENbool   operator!=(ENColor vColor);

 ENubyte r,g,b,a;
};

typedef struct _ENCamera
{
 ENVector Pos,Angle;
 ENfloat Arc,Range;
}ENCamera;

struct ENMatrixSpec
{
 __declspec(dllexport) void ENGetMatrixSpec();
 ENdouble modelMatrix[16];
 ENdouble projMatrix[16];
 ENint    viewport[4];
};

struct ENPlane
{
 enum Classify {ENCOINCIDENT,ENIN_BACK_OF,ENIN_FRONT_OF,ENDOINTERSECT};

 ENVector normal;
 ENfloat  dist;
 __declspec(dllexport) void              BuildPlane(const ENVector &vNormal,const ENVector &point);
 __declspec(dllexport) ENfloat           PlaneDistance(const ENVector &Point);
 __declspec(dllexport) ENPlane::Classify ClassifyPoint(const ENVector &Point);
 __declspec(dllexport) ENPlane::Classify ClassifyTriangle(const ENVector tridata[]);
 __declspec(dllexport) ENPlane::Classify ClassifyPoly(const ENVector polydata[],const ENuint num);
 __declspec(dllexport) ENPlane::Classify ClassifySphere(const ENVector &pos,const ENfloat range);
 __declspec(dllexport) ENPlane::Classify ClassifySphere(const ENVector &pos,const ENfloat range,ENfloat &distance);
};

struct ENMatrix4X4;

struct ENLight
{
 __declspec(dllexport) ENLight();
 __declspec(dllexport) ENLight(ENColor _color,ENVector _pos,ENfloat _range,ENVector2 _dir,ENfloat _spotcut);
 __declspec(dllexport) void SetGLLight(GLenum index);
 __declspec(dllexport) void SetGLLightTrans(GLenum index);
 __declspec(dllexport) void BuildLightMatrix(ENMatrix4X4 *lmat);
 __declspec(dllexport) void BuildOmniLightMatrix(ENMatrix4X4 *lmat,ENuint ind);
 __declspec(dllexport) ENbool isSpotLight();
 __declspec(dllexport) ENVector CalculateLightColor(ENVector vec);

 ENColor   color;
 ENVector  pos;
 ENVector2 dir;
 ENfloat   range,spotcut;
};

struct ENSoundSrc
{
 __declspec(dllexport) ENSoundSrc();
 __declspec(dllexport) ENSoundSrc(ENVector _pos,ENVector2 _dir,ENfloat _range,
                                  ENfloat _speed,ENfloat _volume,ENfloat _angle,
                                  ENbool _loop);

 ENVector  pos;
 ENVector2 dir;
 ENfloat   range,speed,volume,angle;
 ENbool    loop;
 char      src[128];
};
//////////////////////////////////////////////////////////////
///DIFFERENT MATH FUNCTIONS
//////////////////////////////////////////////////////////////
#ifndef M_PI
#define M_PI 3.14159265358979
#endif

#ifndef M_PI_2
#define M_PI_2 3.14159265358979/2
#endif

struct ENEngineFrustum
{
 ENfloat Frustum[6][4];
};

struct ENMatrix2X2
{
 ENfloat val[2][2];
 //Constructor
 __declspec(dllexport) ENMatrix2X2();
 __declspec(dllexport) ENMatrix2X2(const ENfloat x1,const ENfloat y1,
                                   const ENfloat x2,const ENfloat y2);
 //--------Operators------------//
 //Scalar
 __declspec(dllexport) ENMatrix2X2 operator+(const ENfloat num) const;
 __declspec(dllexport) ENMatrix2X2 operator-(const ENfloat num) const;
 __declspec(dllexport) ENMatrix2X2 operator*(const ENfloat num) const;
 __declspec(dllexport) ENMatrix2X2 operator/(const ENfloat num) const;
 //Matrix Operations
 __declspec(dllexport) ENMatrix2X2 operator+(const ENMatrix2X2& mat) const;
 __declspec(dllexport) ENMatrix2X2 operator-(const ENMatrix2X2& mat) const;
 __declspec(dllexport) ENMatrix2X2 operator*(const ENMatrix2X2& mat) const;
 //Vector Operations
 __declspec(dllexport) ENVector2   operator*(const ENVector2& vec) const;
};

struct ENMatrix3X3
{
 ENfloat val[3][3];
 //Constructor
 __declspec(dllexport) ENMatrix3X3();
 __declspec(dllexport) ENMatrix3X3(const ENfloat x1,const ENfloat y1,const ENfloat z1,
                                   const ENfloat x2,const ENfloat y2,const ENfloat z2,
                                   const ENfloat x3,const ENfloat y3,const ENfloat z3);
 //--------Operators------------//
 //Scalar
 __declspec(dllexport) ENMatrix3X3 operator+(const ENfloat num) const;
 __declspec(dllexport) ENMatrix3X3 operator-(const ENfloat num) const;
 __declspec(dllexport) ENMatrix3X3 operator*(const ENfloat num) const;
 __declspec(dllexport) ENMatrix3X3 operator/(const ENfloat num) const;
 //Matrix Operations
 __declspec(dllexport) ENMatrix3X3 operator+(const ENMatrix3X3& mat) const;
 __declspec(dllexport) ENMatrix3X3 operator-(const ENMatrix3X3& mat) const;
 __declspec(dllexport) ENMatrix3X3 operator*(const ENMatrix3X3& mat) const;
 //Vector Operations
 __declspec(dllexport) ENVector   operator*(const ENVector& vec) const;
};

struct ENMatrix4X4
{
 ENfloat val[4][4];
 //Constructor
 __declspec(dllexport) ENMatrix4X4();
 __declspec(dllexport) ENMatrix4X4(const ENfloat x1,const ENfloat y1,const ENfloat z1,const ENfloat w1,
                                   const ENfloat x2,const ENfloat y2,const ENfloat z2,const ENfloat w2,
                                   const ENfloat x3,const ENfloat y3,const ENfloat z3,const ENfloat w3,
                                   const ENfloat x4,const ENfloat y4,const ENfloat z4,const ENfloat w4);
 //--------Operators------------//
 //Scalar
 __declspec(dllexport) ENMatrix4X4 operator+(const ENfloat num) const;
 __declspec(dllexport) ENMatrix4X4 operator-(const ENfloat num) const;
 __declspec(dllexport) ENMatrix4X4 operator*(const ENfloat num) const;
 __declspec(dllexport) ENMatrix4X4 operator/(const ENfloat num) const;
 //Matrix Operations
 __declspec(dllexport) ENMatrix4X4 operator+(const ENMatrix4X4& mat) const;
 __declspec(dllexport) ENMatrix4X4 operator-(const ENMatrix4X4& mat) const;
 __declspec(dllexport) ENMatrix4X4 operator*(const ENMatrix4X4& mat) const;
 //Vector Operations
 __declspec(dllexport) ENVector4   operator*(const ENVector4& vec) const;
};

//Matrices
__declspec(dllexport)  ENfloat			ENDet(const ENMatrix2X2& mat);
__declspec(dllexport)  ENfloat			ENDet(const ENMatrix3X3& mat);
__declspec(dllexport)  ENfloat			ENDet(const ENMatrix4X4& mat);

__declspec(dllexport)  ENMatrix2X2		ENInv(const ENMatrix2X2& mat);
__declspec(dllexport)  ENMatrix3X3		ENInv(const ENMatrix3X3& mat);
__declspec(dllexport)  ENMatrix4X4		ENInv(const ENMatrix4X4& mat);

__declspec(dllexport)  ENMatrix2X2		ENTrans(const ENMatrix2X2& mat);
__declspec(dllexport)  ENMatrix3X3		ENTrans(const ENMatrix3X3& mat);
__declspec(dllexport)  ENMatrix4X4		ENTrans(const ENMatrix4X4& mat);

__declspec(dllexport)  ENbool		        ENInv2(ENMatrix2X2& mat);
__declspec(dllexport)  ENbool		        ENInv2(ENMatrix3X3& mat);
__declspec(dllexport)  ENbool		        ENInv2(ENMatrix4X4& mat);

__declspec(dllexport)  ENMatrix3X3		ENTangentMatrix(const ENVector& vNormal);

__declspec(dllexport)  ENMatrix3X3              ENRotMatrixX(const ENfloat angle);
__declspec(dllexport)  ENMatrix3X3              ENRotMatrixY(const ENfloat angle);
__declspec(dllexport)  ENMatrix3X3              ENRotMatrixZ(const ENfloat angle);

__declspec(dllexport)  ENMatrix4X4              ENBiasMatrix();
//Common math functions
__declspec(dllexport)  ENfloat			ENRadians(const ENfloat Degree);
__declspec(dllexport)  ENfloat			ENSign(const ENfloat num);
__declspec(dllexport)  ENfloat			ENAbsolute(const ENfloat num);
__declspec(dllexport)  ENVector2		ENAbsolute(const ENVector2& vec);
__declspec(dllexport)  ENVector 		ENAbsolute(const ENVector&  vec);
__declspec(dllexport)  ENVector4 		ENAbsolute(const ENVector4& vec);
__declspec(dllexport)  ENfloat                  ENLength(const ENVector& vec);
__declspec(dllexport)  ENfloat                  ENLength(const ENVector2& vec);
__declspec(dllexport)  ENfloat                  ENLength(const ENVector4& vec);
__declspec(dllexport)  ENVector2		ENNormalize(const ENVector2& vNormal);
__declspec(dllexport)  ENVector 		ENNormalize(const ENVector&  vNormal);
__declspec(dllexport)  ENVector4		ENNormalize(const ENVector4& vNormal);
__declspec(dllexport)  ENfloat                  ENDot(const ENVector2& vec1,const ENVector2& vec2);
__declspec(dllexport)  ENfloat                  ENDot(const ENVector&  vec1,const ENVector&  vec2);
__declspec(dllexport)  ENfloat                  ENDot(const ENVector4& vec1,const ENVector4& vec2);
__declspec(dllexport)  ENfloat                  ENDistance(const ENVector2& vec1,const ENVector2& vec2);
__declspec(dllexport)  ENfloat                  ENDistance(const ENVector&  vec1,const ENVector&  vec2);
__declspec(dllexport)  ENfloat                  ENDistance(const ENVector4& vec1,const ENVector4& vec2);
__declspec(dllexport)  ENfloat                  ENDistance2(const ENVector2& vec1,const ENVector2& vec2);
__declspec(dllexport)  ENfloat                  ENDistance2(const ENVector&  vec1,const ENVector&  vec2);
__declspec(dllexport)  ENfloat                  ENDistance2(const ENVector4& vec1,const ENVector4& vec2);
__declspec(dllexport)  ENint                    ENInterpLinear(const ENint a,const ENint b,const ENfloat fac);
__declspec(dllexport)  ENfloat                  ENInterpLinear(const ENfloat a,const ENfloat b,const ENfloat fac);
__declspec(dllexport)  ENVector2                ENInterpLinear(const ENVector2& a,const ENVector2& b,const ENfloat fac);
__declspec(dllexport)  ENVector                 ENInterpLinear(const ENVector& a,const ENVector& b,const ENfloat fac);
__declspec(dllexport)  ENVector4                ENInterpLinear(const ENVector4& a,const ENVector4& b,const ENfloat fac);
__declspec(dllexport)  ENVector			ENCross(const ENVector& vec1,const ENVector& vec2);
__declspec(dllexport)  ENVector			ENNormal(const ENVector tri[]);
__declspec(dllexport)  ENbool                   ENNormal(const ENVector tri[],ENVector &res);
__declspec(dllexport)  ENfloat                  ENPlaneDistance(ENVector Normal,ENVector Point);
__declspec(dllexport)  ENfloat                  ENTriangleVolume(ENVector tri[]);
__declspec(dllexport)  ENbool                   ENIntersectedPlane(ENVector vPoly[],ENVector vLine[],
                                                                   ENVector &vNormal,ENfloat &originDistance);
__declspec(dllexport)  ENVector                 ENIntersectionPoint(ENVector vNormal,ENVector vLine[],ENfloat distance);
__declspec(dllexport)  ENVector			ENClosestPointOnPlane(ENVector &vPos,ENVector &vNormal,ENVector &point);
__declspec(dllexport)  ENVector                 ENClosestPointOnLine(ENVector vA,ENVector vB,ENVector vPoint);
__declspec(dllexport)  ENbool                   ENBoxLineIntersect(ENVector vBox[],ENVector vLine[]);
__declspec(dllexport)  ENbool                   ENBoxLineIntersect(ENVector vBox[],ENVector vLine[],ENVector &pos);
__declspec(dllexport)  ENbool                   ENBoxLineIntersect2(ENVector vBox[],ENVector vLine[]);
__declspec(dllexport)  ENbool                   ENBoxLineIntersect2(ENVector vBox[],ENVector vLine[],ENVector &pos);
__declspec(dllexport)  ENfloat                  ENVecAngle(ENVector Vector1,ENVector Vector2);
__declspec(dllexport)  ENbool                   ENInsidePolygon(ENVector vIntersection,ENVector Poly[],long verticeCount);
__declspec(dllexport)  ENint                    ENClassifySphere(ENVector &vCenter,ENVector &vNormal,ENVector &vPoint,ENfloat radius,ENfloat &distance);
__declspec(dllexport)  ENbool                   ENEdgeSphereCollision(ENVector &vCenter,ENVector vPoly[],ENint vertexCount,ENfloat radius);
__declspec(dllexport)  ENbool                   ENIntersectedPolygon(ENVector vPoly[],ENVector vLine[],ENint verticeCount);
__declspec(dllexport)  ENbool                   ENIntersectedPolygon(ENVector vPoly[],ENVector vLine[],ENint verticeCount,ENVector &pos);
__declspec(dllexport)  ENbool                   ENSpherePolygonCollision(ENVector vPoly[],ENVector &vCenter,ENint vertexCount,ENfloat radius);
__declspec(dllexport)  ENVector                 ENVecDir(ENfloat range,ENVector2 Angles);
__declspec(dllexport)  ENVector2		ENPolar(ENVector vec);
//Frustum math functions
enum ENFrustumSide
{
 ENRIGHT	= 0,
 ENLEFT	= 1,
 ENBOTTOM	= 2,
 ENTOP	        = 3,
 ENBACK	= 4,
 ENFRONT	= 5
};

__declspec(dllexport)  void                     ENNormalizePlane(ENfloat frustum[6][4],ENint side);
__declspec(dllexport)  void                     ENCalculateFrustumPlane(ENVector *tri,ENEngineFrustum &vfrust,ENFrustumSide side);
__declspec(dllexport)  void                     ENCalculateFrustumPlanes(ENEngineFrustum &res);
__declspec(dllexport)  void                     ENCalculateFrustumPlanes(ENEngineFrustum &res,ENMatrix4X4 *matrix);
__declspec(dllexport)  bool             ENPointInFrustum(ENfloat x,ENfloat y,ENfloat z,
                        ENEngineFrustum *FrustumData);
__declspec(dllexport)  bool             ENSphereInFrustum(ENfloat x,ENfloat y,ENfloat z,ENfloat radius,
                        ENEngineFrustum *FrustumData);
__declspec(dllexport)  bool             ENCubeInFrustum(ENfloat x,ENfloat y,ENfloat z,ENfloat size,
                        ENEngineFrustum *FrustumData);
__declspec(dllexport)  bool             ENBoxInFrustum(ENfloat x,ENfloat y,ENfloat z,ENfloat x2,ENfloat y2,ENfloat z2,
                        ENEngineFrustum *FrustumData);
//////////////////////////////////////////////////////////////
///OPEN GL
//////////////////////////////////////////////////////////////
class ENGLClass
{
 public:
   __declspec(dllexport) ENGLClass(void *dst,ENbool OwnDC,ENbool UseGlobalRC);
   __declspec(dllexport) ~ENGLClass();
   __declspec(dllexport) ENbool       Init();
   __declspec(dllexport) void         Start();
   __declspec(dllexport) void         End();
   __declspec(dllexport) void         ConfigMatrix(ENfloat minx,ENfloat miny,
                                                   ENfloat maxx,ENfloat maxy);
   __declspec(dllexport) void         Set();
   __declspec(dllexport) HDC          GetObjDC();
   __declspec(dllexport) HGLRC        GetObjRC();
   __declspec(dllexport) static void  GenNormalizationCubeMap();
 protected:
   HDC          dc;
   ENbool       OwnDC,UseGlobalRC;
   HGLRC        rc;
   static ENuint NumGLObjs;
   static HGLRC GlobalRC;
   __declspec(dllexport) ENbool       SetupPixelFormat();
 private:
    __declspec(dllexport) static void  FormatVector(ENVector &vec);

};

class ENGLExtension
{
 public:
   //Types
   enum POINTERTYPE {EN_INT_1,EN_INT_2,EN_INT_3,EN_INT_4,
                     EN_FLOAT_1,EN_FLOAT_2,EN_FLOAT_3,EN_FLOAT_4};
   //Main functions
   __declspec(dllexport) static ENbool        Load();
   __declspec(dllexport) static ENbool        isLoaded();
   //WGL render buffer
   __declspec(dllexport) static HPBUFFERARB   CreateGLBuffer(HDC dc,ENint pixelformat,ENint width,
                                                             ENint height,const ENint Attrib[]);
   __declspec(dllexport) static ENbool        QueryPbuffer(HPBUFFERARB hPbuffer, int iAttribute, int *piValue);
   __declspec(dllexport) static HDC           GetPbufferDC(HPBUFFERARB hPbuffer);
   __declspec(dllexport) static ENint         ReleasePbufferDC(HPBUFFERARB hPbuffer, HDC hDC);
   __declspec(dllexport) static ENbool        DestroyPbuffer(HPBUFFERARB hPbuffer);
   __declspec(dllexport) static ENbool        ChoosePixelFormat(HDC hdc,const ENint *piAttribIList,
                                                                const ENfloat *pfAttribFList,ENuint nMaxFormats,
                                                                ENint *piFormats,ENuint *nNumFormats);
   //WGL render to texture
   __declspec(dllexport) static ENbool        BindTexImage(HPBUFFERARB hPbuffer,ENint iBuffer);
   __declspec(dllexport) static ENbool        ReleaseTexImage(HPBUFFERARB hPbuffer,ENint iBuffer);
   __declspec(dllexport) static ENbool        SetPbufferAttrib(HPBUFFERARB hPbuffer,const ENint *piAttribList);
   //Multitexturing functions
   __declspec(dllexport) static void          ActiveTexture(GLenum texture);
   __declspec(dllexport) static void          ActiveClientTexture(GLenum texture);
   __declspec(dllexport) static void          TexCoord(GLenum target,ENfloat s,ENfloat t);
   __declspec(dllexport) static void          TexCoord(GLenum target,ENVector2  coord);
   __declspec(dllexport) static void          TexCoord(GLenum target,ENVector2 *coord);
   __declspec(dllexport) static void          TexCoord(GLenum target,ENfloat s,ENfloat t,ENfloat u);
   __declspec(dllexport) static void          TexCoord(GLenum target,ENVector   coord);
   __declspec(dllexport) static void          TexCoord(GLenum target,ENVector  *coord);
   //Occlusion queries
   __declspec(dllexport) static void          GenerateQueries(ENuint num,ENuint *ids);
   __declspec(dllexport) static void          DeleteQueries(ENuint num,ENuint *ids);
   __declspec(dllexport) static void          BeginQuery(GLenum target,ENuint id);
   __declspec(dllexport) static void          EndQuery(GLenum target);
   __declspec(dllexport) static void          GetQueryObjectuiv(ENuint id,GLenum pname,GLuint *params);
   //OpenGL Shader language functions
   __declspec(dllexport) static ENhandle      CreateProgram();
   __declspec(dllexport) static void          DeleteObject(ENhandle object);
   __declspec(dllexport) static ENhandle      CreateShader(GLenum shaderType);
   __declspec(dllexport) static void          SetShaderSource(ENhandle shader,const char *source);
   __declspec(dllexport) static void          CompileShader(ENhandle shader);
   __declspec(dllexport) static void          AttachObject(ENhandle program,ENhandle shader);
   __declspec(dllexport) static void          LinkProgram(ENhandle program);
   __declspec(dllexport) static void          ValidateProgram(ENhandle program);
   __declspec(dllexport) static ENint         GetUniformHandle(ENhandle program,const char *name);
   __declspec(dllexport) static void          UseProgram(ENhandle program);
   __declspec(dllexport) static void          GetObjectParameter(ENhandle object,GLenum pname,ENint *params);
   __declspec(dllexport) static void          GetInfoLog(ENhandle object,ENuint maxLength,ENint &len,char *infoLog);
   __declspec(dllexport) static void          Uniform(ENint location,ENint v0);
   __declspec(dllexport) static void          Uniform(ENint location,ENint v0,ENint v1);
   __declspec(dllexport) static void          Uniform(ENint location,ENint v0,ENint v1,ENint v2);
   __declspec(dllexport) static void          Uniform(ENint location,ENint v0,ENint v1,ENint v2,ENint v3);
   __declspec(dllexport) static void          Uniform(ENint location,ENfloat v0);
   __declspec(dllexport) static void          Uniform(ENint location,ENfloat v0,ENfloat v1);
   __declspec(dllexport) static void          Uniform(ENint location,ENfloat v0,ENfloat v1,ENfloat v2);
   __declspec(dllexport) static void          Uniform(ENint location,ENfloat v0,ENfloat v1,ENfloat v2,ENfloat v3);
   __declspec(dllexport) static void          Uniform(ENint location,POINTERTYPE type,ENuint num,void *data);
   __declspec(dllexport) static void          Uniform(ENint location,ENbool transpose,ENMatrix2X2 matrix);
   __declspec(dllexport) static void          Uniform(ENint location,ENbool transpose,ENMatrix2X2 *matrix);
   __declspec(dllexport) static void          Uniform(ENint location,ENbool transpose,ENMatrix3X3 matrix);
   __declspec(dllexport) static void          Uniform(ENint location,ENbool transpose,ENMatrix3X3 *matrix);
   __declspec(dllexport) static void          Uniform(ENint location,ENbool transpose,ENMatrix4X4 matrix);
   __declspec(dllexport) static void          Uniform(ENint location,ENbool transpose,ENMatrix4X4 *matrix);

   __declspec(dllexport) static ENint         GetAttribHandle(ENhandle program,const char *name);
   __declspec(dllexport) static void          Attrib(ENint location,ENVector2 vec);
   __declspec(dllexport) static void          Attrib(ENint location,ENVector2 *vec);
   __declspec(dllexport) static void          Attrib(ENint location,ENVector vec);
   __declspec(dllexport) static void          Attrib(ENint location,ENVector *vec);
   __declspec(dllexport) static void          EnableAttribArray(ENuint location);
   __declspec(dllexport) static void          DisableAttribArray(ENuint location);
   __declspec(dllexport) static void          VertexAttribPointer(ENuint index,ENint size,GLenum type,
                                                                  ENbool normalized,ENuint stride,const void *pointer);
 private:
   static ENbool Loaded;
   //WGL render buffer
   static PFNWGLCREATEPBUFFERARBPROC            wglCreatePbufferARB;
   static PFNWGLGETPBUFFERDCARBPROC             wglGetPbufferDCARB;
   static PFNWGLRELEASEPBUFFERDCARBPROC         wglReleasePbufferDCARB;
   static PFNWGLDESTROYPBUFFERARBPROC           wglDestroyPbufferARB;
   static PFNWGLQUERYPBUFFERARBPROC             wglQueryPbufferARB;
   static PFNWGLCHOOSEPIXELFORMATARBPROC        wglChoosePixelFormatARB;
   //WGL render to texture
   static PFNWGLBINDTEXIMAGEARBPROC             wglBindTexImageARB;
   static PFNWGLRELEASETEXIMAGEARBPROC          wglReleaseTexImageARB;
   static PFNWGLSETPBUFFERATTRIBARBPROC         wglSetPbufferAttribARB;
   //Multitexturing extensions
   static PFNGLACTIVETEXTUREARBPROC             glActiveTextureARB;
   static PFNGLCLIENTACTIVETEXTUREARBPROC       glClientActiveTextureARB;
   static PFNGLMULTITEXCOORD2FARBPROC	        glMultiTexCoord2fARB;
   static PFNGLMULTITEXCOORD2FVARBPROC	        glMultiTexCoord2fvARB;
   static PFNGLMULTITEXCOORD3FARBPROC	        glMultiTexCoord3fARB;
   static PFNGLMULTITEXCOORD3FVARBPROC	        glMultiTexCoord3fvARB;
   //Occlusion queries
   static PFNGLGENQUERIESARBPROC                glGenQueriesARB;
   static PFNGLDELETEQUERIESARBPROC             glDeleteQueriesARB;
   static PFNGLBEGINQUERYARBPROC                glBeginQueryARB;
   static PFNGLENDQUERYARBPROC                  glEndQueryARB;
   static PFNGLGETQUERYOBJECTUIVARBPROC         glGetQueryObjectuivARB;
   //OpenGL Shader language extensions
   static PFNGLCREATEPROGRAMOBJECTARBPROC       glCreateProgramObjectARB;
   static PFNGLDELETEOBJECTARBPROC              glDeleteObjectARB;
   static PFNGLCREATESHADEROBJECTARBPROC        glCreateShaderObjectARB;
   static PFNGLSHADERSOURCEARBPROC              glShaderSourceARB;
   static PFNGLCOMPILESHADERARBPROC             glCompileShaderARB;
   static PFNGLATTACHOBJECTARBPROC              glAttachObjectARB;
   static PFNGLLINKPROGRAMARBPROC               glLinkProgramARB;
   static PFNGLVALIDATEPROGRAMARBPROC           glValidateProgramARB;
   static PFNGLUSEPROGRAMOBJECTARBPROC          glUseProgramObjectARB;
   static PFNGLGETOBJECTPARAMETERIVARBPROC      glGetObjectParameterivARB;
   static PFNGLGETINFOLOGARBPROC                glGetInfoLogARB;

   static PFNGLGETUNIFORMLOCATIONARBPROC        glGetUniformLocationARB;
   static PFNGLUNIFORM1IARBPROC                 glUniform1iARB;
   static PFNGLUNIFORM2IARBPROC                 glUniform2iARB;
   static PFNGLUNIFORM3IARBPROC                 glUniform3iARB;
   static PFNGLUNIFORM4IARBPROC                 glUniform4iARB;
   static PFNGLUNIFORM1FARBPROC                 glUniform1fARB;
   static PFNGLUNIFORM2FARBPROC                 glUniform2fARB;
   static PFNGLUNIFORM3FARBPROC                 glUniform3fARB;
   static PFNGLUNIFORM4FARBPROC                 glUniform4fARB;
   static PFNGLUNIFORM1IVARBPROC                glUniform1ivARB;
   static PFNGLUNIFORM2IVARBPROC                glUniform2ivARB;
   static PFNGLUNIFORM3IVARBPROC                glUniform3ivARB;
   static PFNGLUNIFORM4IVARBPROC                glUniform4ivARB;
   static PFNGLUNIFORM1FVARBPROC                glUniform1fvARB;
   static PFNGLUNIFORM2FVARBPROC                glUniform2fvARB;
   static PFNGLUNIFORM3FVARBPROC                glUniform3fvARB;
   static PFNGLUNIFORM4FVARBPROC                glUniform4fvARB;
   static PFNGLUNIFORMMATRIX2FVARBPROC          glUniformMatrix2fvARB;
   static PFNGLUNIFORMMATRIX3FVARBPROC          glUniformMatrix3fvARB;
   static PFNGLUNIFORMMATRIX4FVARBPROC          glUniformMatrix4fvARB;

   static PFNGLGETATTRIBLOCATIONARBPROC         glGetAttribLocationARB;
   static PFNGLVERTEXATTRIB2FARBPROC            glVertexAttrib2fARB;
   static PFNGLVERTEXATTRIB2FVARBPROC           glVertexAttrib2fvARB;
   static PFNGLVERTEXATTRIB3FARBPROC            glVertexAttrib3fARB;
   static PFNGLVERTEXATTRIB3FVARBPROC           glVertexAttrib3fvARB;
   static PFNGLENABLEVERTEXATTRIBARRAYARBPROC   glEnableVertexAttribArrayARB;
   static PFNGLDISABLEVERTEXATTRIBARRAYARBPROC  glDisableVertexAttribArrayARB;
   static PFNGLVERTEXATTRIBPOINTERARBPROC       glVertexAttribPointerARB;

   __declspec(dllexport) static ENbool InitShaderExt();
   __declspec(dllexport) static ENbool InitSndColorExt();
   __declspec(dllexport) static ENbool InitMultiTexturingExt();
   __declspec(dllexport) static ENbool InitOcclusionExt();
   __declspec(dllexport) static ENbool InitWGLExtensions();
};
//////////////////////////////////////////////////////////////
///MEMORY AND FILE FUNCTIONS
//////////////////////////////////////////////////////////////
__declspec(dllexport) void*  ENAllocMemory(ENuint size);
__declspec(dllexport) void*  ENReallocMemory(void *addr,ENuint size);
__declspec(dllexport) void   ENFreeMemory(void *addr);

__declspec(dllexport) ENFile ENOpenFile(char *filename,char *mode);
__declspec(dllexport) ENFile ENOpenFile(const char *filename, const char *mode);
__declspec(dllexport) ENint  ENSeekFile(ENFile stream,ENint offset,ENint whence);
__declspec(dllexport) ENint  ENTellFile(ENFile stream);
__declspec(dllexport) ENuint ENReadFile(void *ptr,ENuint size,ENuint n,ENFile stream);
__declspec(dllexport) ENuint ENWriteFile(void *ptr,ENuint size,ENuint n,ENFile stream);
__declspec(dllexport) ENint  ENCloseFile(ENFile stream);

__declspec(dllexport) ENubyte *ENAddElement(ENubyte *ptr,ENuint esize,ENuint &num,ENubyte *element);
__declspec(dllexport) ENubyte *ENAddElements(ENubyte *ptr,ENuint esize,ENuint &num,ENubyte *elements,
                                             ENuint numelements);
__declspec(dllexport) ENubyte *ENDelElement(ENubyte *ptr,ENuint esize,ENuint &num,ENuint ind);

__declspec(dllexport) ENint  ENFileSize(ENFile handle);
__declspec(dllexport) ENbool ENFileExists(char *FileName);
__declspec(dllexport) void   ENRemoveFileName(char *fn);
__declspec(dllexport) void   ENRemoveFilePath(char *fn);
__declspec(dllexport) void   ENReadStream(ENubyte *stream,ENuint &_c,void *dst,ENuint size,ENuint num);


class ENArrayBasic
{
 public:
   __declspec(dllexport) ENArrayBasic(ENuint ElementSize,ENuint allocp,ENuint freep);
   __declspec(dllexport) ~ENArrayBasic();
   __declspec(dllexport) void   Clear();
   __declspec(dllexport) void   AddElements(ENubyte *elements,ENuint num);
   __declspec(dllexport) void   AddElement(ENubyte *element);
   __declspec(dllexport) ENbool DelElement(ENuint ind);
   __declspec(dllexport) void   LoadFromHandle(ENFile handle);
   __declspec(dllexport) void   SaveToHandle(ENFile handle);
   __declspec(dllexport) void   SaveToHandleRaw(ENFile handle);

   __declspec(dllexport) ENuint GetNum();
   __declspec(dllexport) ENuint GetElementSize();
   __declspec(dllexport) ENbool GetElement(ENuint ind,ENubyte *element);
   __declspec(dllexport) void   GetElements(ENubyte *elements);

   __declspec(dllexport) ENbool SetElement(ENuint ind,ENubyte *element);
 protected:
   ENuint AllocStoragePack;
   ENuint FreeStoragePack;
   ENubyte *Data;
   ENuint   ElementSize;
   ENuint   NumElements;
   ENuint   NumStorage;
};

class ENArray : public ENArrayBasic
{
 public:
   __declspec(dllexport) ENArray(ENuint ElementSize);
   __declspec(dllexport) ENArray(ENArray *arrayobj);
};

class ENBooleanArray : public ENArray
{
 public:
   __declspec(dllexport) ENBooleanArray();
   __declspec(dllexport) void SetElement(ENubyte stat,ENuint index);
   __declspec(dllexport) void SetElements(ENubyte stat,ENuint ind1,ENuint ind2);
   __declspec(dllexport) void AddElement(ENubyte stat);
   __declspec(dllexport) void AddElements(ENubyte stat,ENuint num);
   __declspec(dllexport) ENubyte GetElement(ENuint ind);
};

class ENMemoryStream
{
 public:
   __declspec(dllexport) ENMemoryStream(ENubyte *addr);
   __declspec(dllexport) void Read(void *dst,ENuint size);
   __declspec(dllexport) void Seek(ENint diff);
 private:
   ENubyte *addr;
   ENuint   pos;
};
#endif
