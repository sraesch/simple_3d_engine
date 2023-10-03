//---------------------------------------------------------------------------
#include "ENBSPBuilder.h"
//---------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////
//// ENBSPPolygon
//////////////////////////////////////////////////////////////////////////
ENBSPPolygon::ENBSPPolygon()
{
 verts=NULL;
 num=0;
}

ENBSPPolygon::ENBSPPolygon(const ENBSPPolygon& poly)
{
 num=poly.num;
 verts=(ENVector*)ENAllocMemory(poly.num*sizeof(ENVector));
 memcpy(verts,poly.verts,poly.num*sizeof(ENVector));
}
//////////////////////////////////////////////////////////////////////////
//// ENBSPTree
//////////////////////////////////////////////////////////////////////////
ENBSPTree::ENBSPTree()
{
 numpolys=0;
 front=NULL;
 back=NULL;
 polylist=NULL;
}

ENBSPTree::~ENBSPTree()
{
 for(ENuint a=0;a<numpolys;a++)
   ENFreeMemory(polylist[a].verts);
   
 if(polylist) delete polylist;
 if(front) delete front;
 if(back) delete back;
}

ENbool ENBSPTree::WriteBsp(char *FileName)
{
 //Variables
 ENFile handle;
 //Open file
 handle=ENOpenFile(FileName,"wb");
 if(!handle) return false;
 //Write
 ENbool res=WriteBsp(handle);
 //Finished
 ENCloseFile(handle);
 return res;
}

ENbool ENBSPTree::WriteBsp(ENFile handle)
{
 //Check handle
 if(!handle) return false;
 //Write tree
 WriteTree(handle,this);
 //Finished
 return true;
}

void ENBSPTree::WriteTree(ENFile handle,ENBSPTree *bsp)
{
 //Variables
 ENbool enabled;
 //Write tree
 ENWriteFile(&bsp->partition,sizeof(ENPlane),1,handle);
 ENWriteFile(&bsp->numpolys,sizeof(ENuint),1,handle);
 for(ENuint a=0;a<bsp->numpolys;a++)
 {
  ENWriteFile(&bsp->polylist[a].num,sizeof(ENuint),1,handle);
  ENWriteFile(bsp->polylist[a].verts,sizeof(ENVector)*bsp->polylist[a].num,1,handle);
 }
 //Front
 if(bsp->front)
 {
  enabled=true;
  ENWriteFile(&enabled,sizeof(enabled),1,handle);
  WriteTree(handle,bsp->front);
 }
 else
 {
  enabled=false;
  ENWriteFile(&enabled,sizeof(enabled),1,handle);
 }
 //Back
 if(bsp->back)
 {
  enabled=true;
  ENWriteFile(&enabled,sizeof(enabled),1,handle);
  WriteTree(handle,bsp->back);
 }
 else
 {
  enabled=false;
  ENWriteFile(&enabled,sizeof(enabled),1,handle);
 }
}

ENbool ENBSPTree::LoadBsp(char *FileName)
{
 //Variables
 ENFile handle;
 //Open file
 handle=ENOpenFile(FileName,"rb");
 if(!handle) return NULL;
 //Read
 ENbool res=LoadBsp(handle);
 //Finished
 ENCloseFile(handle);
 return res;
}

ENbool ENBSPTree::LoadBsp(ENFile handle)
{
 //Load tree
 if(!handle) return false;
 LoadTree(handle,this);
 return true;
}

void ENBSPTree::LoadTree(ENFile handle,ENBSPTree *bsp)
{
 //Variables
 ENbool enabled;
 //Read tree
 ENReadFile(&bsp->partition,sizeof(ENPlane),1,handle);
 ENReadFile(&bsp->numpolys,sizeof(ENuint),1,handle);
 bsp->polylist=new ENBSPPolygon[bsp->numpolys];
 for(ENuint a=0;a<bsp->numpolys;a++)
 {
  ENReadFile(&bsp->polylist[a].num,sizeof(ENuint),1,handle);
  bsp->polylist[a].verts=(ENVector*)ENAllocMemory(sizeof(ENVector)*bsp->polylist[a].num);
  ENReadFile(bsp->polylist[a].verts,sizeof(ENVector),bsp->polylist[a].num,handle);
 }
 //Front
 ENReadFile(&enabled,sizeof(ENbool),1,handle);
 if(enabled)
 {
  bsp->front=new ENBSPTree;
  LoadTree(handle,bsp->front);
 }
 else
   bsp->front=NULL;
 //Back
 ENReadFile(&enabled,sizeof(ENbool),1,handle);
 if(enabled)
 {
  bsp->back=new ENBSPTree;
  LoadTree(handle,bsp->back);
 }
 else
   bsp->back=NULL;
}

ENbool ENBSPTree::SphereCollision(const ENVector &pos,const ENfloat range)
{
 //Variables
 ENfloat distance;
 ENPlane::Classify sphereclass;
 ENVector vOffset,vPosition;
 ENuint numtris;
 //Check tree
 if(!bt) return false;
 //Classify sphere
 numCHPlanes++;
 sphereclass=bt->partition.ClassifySphere(pos,range,distance);
 //Compute classification
 switch(sphereclass)
 {
  case ENPlane::ENIN_FRONT_OF:
    return ENSphereTriangleCollision(bt->front,pos,range);
  case ENPlane::ENIN_BACK_OF:
    return ENSphereTriangleCollision(bt->back,pos,range);
  case ENPlane::ENDOINTERSECT:
    vOffset   = bt->partition.normal*distance;
    vPosition = pos-vOffset;
    //Check all polygones
    numtris=bt->numpolys;
    for(ENuint a=0;a<numtris;a++)
    {
     numCHTris++;
     if(ENInsidePolygon(vPosition,bt->polylist[a].verts,bt->polylist[a].num)) return true;
     if(ENEdgeSphereCollision(pos,bt->polylist[a].verts,bt->polylist[a].num,range)) return true;
    }
    //No collision, so we have to check both sides
    if(ENSphereTriangleCollision(bt->front,pos,range)) return true;
    return ENSphereTriangleCollision(bt->back,pos,range);
 }
 //Never happend (-:
 return false;
}
//////////////////////////////////////////////////////////////////////////
//// ENBSPBuilder
//////////////////////////////////////////////////////////////////////////
ENBSPBuilder::ENBSPBuilder(ENBSPBUILDERMSG msgfunc)
{
 this->msgfunc=msgfunc;
 polygonStack=new list<ENBSPPolygon>;
}

ENBSPBuilder::~ENBSPBuilder()
{
 delete polygonStack;
}

void ENBSPBuilder::AddPolygon(const ENBSPPolygon& poly)
{
 ENBSPPolygon ntree(poly);
 polygonStack->push_back(ntree);
}

void ENBSPBuilder::AddPolygon(const ENBSPPolygon *polygons,const ENuint num)
{
 for(ENuint a=0;a<num;a++)
 {
  ENBSPPolygon ntree(polygons[a]);
  polygonStack->push_back(ntree);
 }
}

ENBSPTree *ENBSPBuilder::BuildTree()
{
 //Variables
 char Buffer[80];
 const ENuint ticks=GetTickCount();
 ENBSPTree *tree=new ENBSPTree;
 //Build Bsp tree
 if(!polygonStack->empty())
 {
  msgvar[0]=polygonStack->size();
  msgvar[1]=polygonStack->size();
  BuildTreeIntern(tree,polygonStack,NULL);
 }
 else
 {
  delete tree;
  return NULL;
 }
 //State Message
 sprintf(Buffer,"Num polygons: %i",msgvar[1]);
 UpdateState(Buffer,msgvar[0],msgvar[1]);
 //Finished Message
 sprintf(Buffer,"Finished in %f seconds",(ENfloat)(GetTickCount()-ticks)/1000.0f);
 UpdateState(Buffer,msgvar[0],msgvar[1]);
 //Finished
 return tree;
}

void ENBSPBuilder::BuildPlane(const ENVector poly[],ENPlane &partition)
{
 //Build plane
 partition.BuildPlane(ENNormal(poly),*poly);
}

void ENBSPBuilder::BuildTreeIntern(ENBSPTree *tree,list<ENBSPPolygon> *polygons,ENPlane *parent)
{
 //Variables
 ENBSPPolygon root,polyangle;
 ENPlane::Classify polyclass;
 const ENuint startNum=polygons->size();
 stack<ENBSPPolygon> *polylist=new stack<ENBSPPolygon>; 
 list<ENBSPPolygon> *frontlist=new list<ENBSPPolygon>;
 list<ENBSPPolygon> *backlist=new list<ENBSPPolygon>;
 //Get root polyangle
 if(polygons->size()==1)
 {
  root=polygons->back();
  polygons->pop_back();
 }
 else
   GetRootPolygon(polygons,root,parent);

 UpdateState("Building tree...",msgvar[0],msgvar[1]);   
 //Calculate partition
 BuildPlane(root.verts,tree->partition);
 //Add root polyangle
 polylist->push(root);
 //Build all polygons
 while(!polygons->empty())
 {
  //Get polyangle
  polyangle=polygons->back();
  polygons->pop_back();
  //Classify polygon
  polyclass=tree->partition.ClassifyPoly(polyangle.verts,polyangle.num);
  //Do class
  switch(polyclass)
  {
   case ENPlane::ENCOINCIDENT:
     polylist->push(polyangle);
   break;
   case ENPlane::ENIN_BACK_OF:
     backlist->push_back(polyangle);
   break;
   case ENPlane::ENIN_FRONT_OF:
     frontlist->push_back(polyangle);
   break;
   case ENPlane::ENDOINTERSECT:
     SplitPolygon(tree->partition,polyangle,frontlist,backlist);
   break;
  }
 }
 //Free polygons
 if(polygons!=polygonStack) delete polygons;
 //Add polygons to bsp tree
 tree->numpolys=polylist->size();
 tree->polylist=new ENBSPPolygon[tree->numpolys];
 ENuint polyc=0;
 while(!polylist->empty())
 {
  tree->polylist[polyc]=polylist->top();
  polylist->pop();
  polyc++;
 }
 delete polylist;
 //Update remaining polygons
 ENuint numNewpolys=frontlist->size()+backlist->size()+tree->numpolys-startNum;
 msgvar[1]+=numNewpolys;
 msgvar[0]+=numNewpolys-tree->numpolys;
 UpdateState("Tree builded...",msgvar[0],msgvar[1]);
 //Rekursive call's
 if(!frontlist->empty())
 {
  tree->front=new ENBSPTree;
  BuildTreeIntern(tree->front,frontlist,&tree->partition);
 }
 else
 {
  tree->front=NULL;
  delete frontlist;
 }

 if(!backlist->empty())
 {
  tree->back=new ENBSPTree;
  BuildTreeIntern(tree->back,backlist,&tree->partition);
 }
 else
 {
  tree->back=NULL;
  delete backlist;
 }
}

void ENBSPBuilder::SplitPolygon(const ENPlane& partition,const ENBSPPolygon& poly,
                                list<ENBSPPolygon> *front,list<ENBSPPolygon> *back)
{
 //Variables
 stack<ENVector> pverts[2];
 ENfloat  dist,lastdist,sect;
 //Check all points
 for(ENuint a=0;a<poly.num+1;a++)
 {
  //Calculate distance to plane
  ENfloat dist=partition.PlaneDistance(poly.verts[a%poly.num]);
  //if last point and current point on different sides, calculate intersecetion
  if(a>0&&lastdist*dist<0.0f)
  {
   //Calculate direction vector
   ENVector v=poly.verts[a%poly.num]-poly.verts[a-1];
   //Calculate intersection point
   sect=-partition.PlaneDistance(poly.verts[a-1])/ENDot(partition.normal,v);
   v=poly.verts[a-1]+v*sect;
   //Add point to both sides
   pverts[0].push(v);
   pverts[1].push(v);
  }
  if(a<poly.num)
  {
   //Add vector to back side
   if(dist<0.0f)
     pverts[0].push(poly.verts[a]);
   else//Add vector to front side
   if(dist>0.0f)
     pverts[1].push(poly.verts[a]);
   else
   {//Add vector to both side
    pverts[0].push(poly.verts[a]);
    pverts[1].push(poly.verts[a]);
   }
  }
  //Update last distance
  lastdist=dist;
 }
 //Add polygons
 ENBSPPolygon newpoly;
 for(ENuint b=0;b<2;b++)
 {
  newpoly.num=pverts[b].size();
  newpoly.verts=(ENVector*)ENAllocMemory(sizeof(ENVector)*newpoly.num);
  for(ENint c=newpoly.num-1;c>=0;c--)
  {
   newpoly.verts[c]=pverts[b].top();
   pverts[b].pop();
  }
  if(b==0)
    back->push_back(newpoly);
  else
    front->push_back(newpoly);  
 }
}

void ENBSPBuilder::GetRootPolygon(list<ENBSPPolygon> *polygons,ENBSPPolygon &root,ENPlane *parent)
{
 //Variables
 const ENuint numpolys=polygons->size();
 ENfloat lastranking;
 //Init
 root=polygons->front();
 polygons->pop_front();
 lastranking=RootRanking(polygons,root,NULL,parent);
 //Go trough polygons
 UpdateState("Get root...",msgvar[0],msgvar[1]);
 for(ENuint a=1;a<numpolys;a++)
 {
  UpdateState(NULL,a,numpolys);
  //Get polyangle
  ENBSPPolygon poly=polygons->front();
  polygons->pop_front();
  //Count intersections
  ENfloat ranking=RootRanking(polygons,poly,&root,parent);
  //Update
  if(ranking<lastranking)
  {
   lastranking=ranking;
   polygons->push_back(root);
   root=poly;
  }
  else
    polygons->push_back(poly);
 }
}

ENfloat ENBSPBuilder::RootRanking(list<ENBSPPolygon> *polygons,const ENBSPPolygon& poly,
                                  ENBSPPolygon *root,ENPlane *parent)
{
 //Variables
 ENfloat f=0,b=0;
 ENPlane pl;
 list<ENBSPPolygon>::iterator i;
 //Get iterator
 i=polygons->begin();
 //Build plane
 BuildPlane(poly.verts,pl);
 for(ENuint a=0;a<polygons->size();a++)
 {
  switch(pl.ClassifyPoly(i->verts,i->num))
  {
   case ENPlane::ENIN_FRONT_OF: f++; break;
   case ENPlane::ENIN_BACK_OF:  b++; break;
  }
  i++;
 }
 //Add root
 if(root)
   switch(pl.ClassifyPoly(root->verts,root->num))
   {
    case ENPlane::ENIN_FRONT_OF: f++; break;
    case ENPlane::ENIN_BACK_OF:  b++; break;
   }
 //Finished
 if(f+b>0.0f)
   return ENAbsolute(f-b)/(f+b);
 else
   return 1.0f;
}

void ENBSPBuilder::UpdateState(char *msg,ENuint pos,ENuint max)
{
 if(msgfunc)
   msgfunc(msg,msgvar[0],msgvar[1]);
}




