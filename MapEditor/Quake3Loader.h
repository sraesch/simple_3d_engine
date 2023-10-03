//---------------------------------------------------------------------------
#ifndef Quake3LoaderH
#define Quake3LoaderH
//---------------------------------------------------------------------------
#include "EngineSystem.h"

struct Quake3Veci
{
 ENint x,y,z;
};

struct Quake3Header
{
 char  strID[4];			// This should always be 'IBSP'
 ENint version;				// This should be 0x2e for Quake 3 files
};

struct Quake3Lump
{
 ENint offset;				// The offset into the file for the start of this lump
 ENint length;				// The length in bytes for this lump
};

struct Quake3Vertex
{
  ENVector  vPosition;			// (x, y, z) position.
  ENVector2 vTextureCoord;		// (u, v) texture coordinate
  ENVector2 vLightmapCoord;	// (u, v) lightmap coordinate
  ENVector  vNormal;			// (x, y, z) normal vector
  ENubyte   color[4];				// RGBA color for the vertex
};

struct Quake3Face
{
 ENint textureID;				// The index into the texture array
 ENint effect;					// The index for the effects (or -1 = n/a)
 ENint type;					// 1=polygon, 2=patch, 3=mesh, 4=billboard
 ENint startVertIndex;			// The starting index into this face's first vertex
 ENint numOfVerts;				// The number of vertices for this face
 ENint meshVertIndex;			// The index into the first meshvertex
 ENint numMeshVerts;			// The number of mesh vertices
 ENint lightmapID;				// The texture index for the lightmap
 ENint lMapCorner[2];			// The face's lightmap corner in the image
 ENint lMapSize[2];			// The size of the lightmap section
 ENVector lMapPos;			// The 3D origin of lightmap.
 ENVector lMapVecs[2];		// The 3D space for s and t unit vectors.
 ENVector vNormal;			// The face normal.
 ENint size[2];				// The bezier patch dimensions.
};

struct Quake3Texture
{
 char strName[64];			// The name of the texture w/o the extension
 ENint flags;					// The surface flags (unknown)
 ENint contents;				// The content flags (unknown)
};

struct Quake3Lightmap
{
 ENubyte imageBits[128][128][3];   // The RGB data in a 128x128 image
};

// This is our lumps enumeration
enum eQuake3Lumps
{
    kEntities = 0,				// Stores player/object positions, etc...
    kTextures,					// Stores texture information
    kPlanes,				    // Stores the splitting planes
    kNodes,						// Stores the BSP nodes
    kLeafs,						// Stores the leafs of the nodes
    kLeafFaces,					// Stores the leaf's indices into the faces
    kLeafBrushes,				// Stores the leaf's indices into the brushes
    kModels,					// Stores the info of world models
    kBrushes,					// Stores the brushes info (for collision)
    kBrushSides,				// Stores the brush surfaces info
    kVertices,					// Stores the level vertices
    kMeshVerts,					// Stores the model vertices offsets
    kShaders,					// Stores the shader files (blending, anims..)
    kFaces,						// Stores the faces for the level
    kLightmaps,					// Stores the lightmaps for the level
    kLightVolumes,				// Stores extra world lighting information
    kVisData,					// Stores PVS and cluster info (visibility)
    kMaxLumps					// A constant to store the number of lumps
};

class Quake3Loader
{
 public:
   Quake3Loader();
   ~Quake3Loader();
   void   Clear();
   ENbool Load(char *FileName);

   ENuint       GetNumVerts();
   ENuint       GetNumFaces();
   ENuint       GetNumTextures();

   Quake3Vertex         GetVertex(ENuint ind);
   Quake3Face           GetFace(ENuint ind);
   Quake3Texture        GetTexture(ENuint ind);
 private:
   ENuint         NumVerts,NumFaces,NumTex,NumLightmaps;
   Quake3Vertex   *Verts;
   Quake3Face     *Faces;
   Quake3Texture  *Texture;
   Quake3Lightmap *Lightmaps;

   ENbool CheckHeader(Quake3Header *qheader);
   void   AllocSpace(Quake3Lump *qlumps);
   void   ReadVerts(Quake3Lump *qlumps,ENFile handle);
   void   CheckTexture(Quake3Texture *tex);
};

#endif
