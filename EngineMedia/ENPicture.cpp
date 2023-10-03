//---------------------------------------------------------------------------
#include <math.h>
#include <windows.h>
#include "EngineMedia.h"
#include "jpeglib.h"
#include "png.h"
//---------------------------------------------------------------------------
///////////////////////////////////////////////////
///ENPicture Class
///////////////////////////////////////////////////
#define TGA_RGB		 2		// This tells us it's a normal RGB (really BGR) file
#define TGA_A		 3		// This tells us it's a ALPHA file
#define TGA_RLE		10		// This tells us that the targa is Run-Length Encoded (RLE)

const char *ENPicture::ENIMAGEEXT="All files(*.*)|*.*|Windows Bitmap(*.bmp)|*.bmp|"
                                  "Portable Network Graphic(*.png)|*.png|"
                                  "Zsoft Paintbrush(*.pcx)|*.pcx|"
                                  "Jpeg - JFIF Compilant(*.jpg;*.jif;*.jpeg)|"
                                  "*.jpg;*.jif;*.jpeg|Truevision Targa(*.tga)|"
                                  "*.tga|Engine Picture(*.enp)|*.enp";

struct ENPicture::ENPCXHEADER
{
 ENubyte   PCXID;
 ENubyte   VERSION;
 ENubyte   COMPRESS;
 ENubyte   BPP;
 ENshort   XMIN,YMIN,XMAX,YMAX;
 ENshort   HDPI;
 ENshort   VDPI;
 ENubyte   COLORMAP[16*3];
 ENubyte   RESERVED;
 ENubyte   COLORPLANES;
 ENshort   BPL;
 ENshort   PALNFO;
 ENshort   RESWIDTH;
 ENshort   RESHEIGHT;
 ENubyte   FILLER[54];
};

const char *ENPicture::ENIMAGEERRORSTRS[]={
                                "No error","Cannot open file",
                                "Unknown version or format",
                                "Cannot read compressed file",
                                "Invalid file","Cannot process file",
                                "Cannot write file","Unknown file","Internal error"};

ENPicture::ENPicture()
{
 width=0;
 height=0;
 bpp=3;
 LastError=ENGINE_IMAGEERROR_NO;
 data=NULL;
}

ENPicture::ENPicture(ENPicture *pic)
{
 width=pic->GetWidth();
 height=pic->GetHeight();
 bpp=pic->GetBpp();
 LastError=ENGINE_IMAGEERROR_NO;
 data=(ENubyte*)ENAllocMemory(width*height*bpp);
 pic->GetData(data);
}

ENPicture::ENPicture(ENubyte bytesperpixel,ENuint w,ENuint h,ENubyte *src)
{
 width=w;
 height=h;
 bpp=bytesperpixel;
 LastError=ENGINE_IMAGEERROR_NO;
 data=(ENubyte*)ENAllocMemory(width*height*bpp);
 memcpy(data,src,width*height*bpp);
}

ENPicture::~ENPicture()
{
 Clear();
}

void ENPicture::InitGLTex()
{
 GLenum fmt;
 if(bpp==3)
   fmt=GL_RGB;
 else
   fmt=GL_RGBA;
   
 glTexImage2D(GL_TEXTURE_2D,0,bpp,width,height,0,fmt,GL_UNSIGNED_BYTE,data);
}

void ENPicture::InitGLTexNoAlpha()
{
 GLenum fmt;
 fmt=GL_RGB;
 glTexImage2D(GL_TEXTURE_2D,0,bpp,width,height,0,fmt,GL_UNSIGNED_BYTE,data);
}

void ENPicture::InitGLMipMap()
{
 GLenum fmt;
 if(bpp==3)
   fmt=GL_RGB;
 else
   fmt=GL_RGBA;
   
 gluBuild2DMipmaps(GL_TEXTURE_2D,bpp,width,height,fmt,GL_UNSIGNED_BYTE,data);
}

void ENPicture::Set(ENPicture *pic)
{
 Clear();
 width=pic->GetWidth();
 height=pic->GetHeight();
 bpp=pic->GetBpp();
 data=(ENubyte*)ENAllocMemory(width*height*bpp);
 pic->GetData(data);
}

void ENPicture::Set(ENubyte bytesperpixel,ENuint w,ENuint h,ENubyte *src)
{
 Clear();
 width=w;
 height=h;
 bpp=bytesperpixel;
 LastError=ENGINE_IMAGEERROR_NO;
 data=(ENubyte*)ENAllocMemory(width*height*bpp);
 memcpy(data,src,width*height*bpp);
}

void ENPicture::Clear()
{
 ENFreeMemory(data);
 width=0;
 height=0;
 bpp=3;
 LastError=ENGINE_IMAGEERROR_NO;
 data=NULL;
}

ENbool ENPicture::LoadBMP(char *FileName)
{
 //Variables
 ENBITMAPFILEHEADER bmpheader;
 BITMAPINFOHEADER bmpinfo;
 ENuint sizeTotal;
 ENubyte *bmpbuffer;
 ENubyte  Colors[256*4];
 ENFile   handle;
 //Open file
 handle=ENOpenFile(FileName,"rb");
 if(!handle)
 {
  LastError=ENGINE_IMAGEERROR_OPEN;
  return false;
 }
 //Clear picture
 Clear();
 //Read header
 ENReadFile(&bmpheader,sizeof(ENBITMAPFILEHEADER),1,handle);
 ENReadFile(&bmpinfo,sizeof(BITMAPINFOHEADER),1,handle);
 //Check header
 if(!CheckBMPHeader(&bmpheader,&bmpinfo))
 {
  ENCloseFile(handle);
  return false;
 }
 //Set header data
 width=bmpinfo.biWidth;
 height=bmpinfo.biHeight;
 bpp=3;
 //If available, read out colors
 switch(bmpinfo.biBitCount)
 {
  case 4:
    ENReadFile(Colors,16,4,handle);
  break;
  case 8:
    ENReadFile(Colors,256,4,handle);
  break;
 };
 //Calculate important process data
 sizeTotal=bmpheader.bfSize-bmpheader.bfOffBits;
 //Alloc space for bitmap data
 bmpbuffer=(ENubyte*)ENAllocMemory(sizeTotal);
 //Jump over offset
 ENSeekFile(handle,bmpheader.bfOffBits,SEEK_SET);
 //Read bitmap data
 ENReadFile(bmpbuffer,sizeTotal,1,handle);
 //Close file handle
 ENCloseFile(handle);
 //Process bitmap data
 switch(bmpinfo.biBitCount)
 {
  case 24:
    ProcessBMP24Data(bmpbuffer,sizeTotal);
    ENFreeMemory(bmpbuffer);
  break;
  case 8:
    ProcessBMP8Data(bmpbuffer,sizeTotal,Colors);
    ENFreeMemory(bmpbuffer);
  break;
  case 4:
    ProcessBMP4Data(bmpbuffer,sizeTotal,Colors);
    ENFreeMemory(bmpbuffer);
  break;
  case 1:
    ProcessBMP1Data(bmpbuffer,sizeTotal);
    ENFreeMemory(bmpbuffer);
  break;
  default:
    Clear();
    ENFreeMemory(bmpbuffer);
    LastError=ENGINE_IMAGEERROR_PROCESS;
    return false;
 }
 //Finished
 return true;
}

ENbool ENPicture::LoadPCX(char *FileName)
{
 //Variables
 ENPCXHEADER pcxheader;
 ENFile      handle;
 //Open file
 handle=ENOpenFile(FileName,"rb");
 if(!handle)
 {
  LastError=ENGINE_IMAGEERROR_OPEN;
  return false;
 }
 //Clear picture
 Clear();
 //Read pcx header
 ENReadFile(&pcxheader,sizeof(ENPCXHEADER),1,handle);
 //Check pcx header
 if(!CheckPCXHeader(&pcxheader))
 {
  ENCloseFile(handle);
  return false;
 }
 //Set header data
 height=pcxheader.YMAX+1; 
 width=pcxheader.XMAX+1;
 bpp=3;
 //Process pcx data
 if(pcxheader.BPP==8&&pcxheader.COLORPLANES==3)
 {
  ProcessPCX24Data(handle,pcxheader.BPL);
  ENCloseFile(handle);
 }
 else
 {
  if(pcxheader.BPP==8&&pcxheader.COLORPLANES==1)
  {
   ProcessPCX8Data(handle,pcxheader.BPL);
   ENCloseFile(handle);
  }
  else
  {
   if(pcxheader.BPP==4&&pcxheader.COLORPLANES==1)
   {
    ProcessPCX4Data(handle,pcxheader.BPL,pcxheader.COLORMAP);
    ENCloseFile(handle);
   }
   else
   {
    if(pcxheader.BPP==1&&pcxheader.COLORPLANES==1)
    {
     ProcessPCX1Data(handle,pcxheader.BPL);
     ENCloseFile(handle);
    }
    else
    {
     Clear();
     ENCloseFile(handle);
     LastError=ENGINE_IMAGEERROR_PROCESS;
     return false;
    }
   }
  }
 }
 //Finished
 return true;
}

ENbool ENPicture::LoadJPG(char *FileName)
{
 //Variables
 struct jpeg_decompress_struct cinfo;
 jpeg_error_mgr jerr;
 ENFile         handle;
 //Open file
 handle=ENOpenFile(FileName,"rb");
 if(!handle)
 {
  LastError=ENGINE_IMAGEERROR_OPEN;
  return false;
 }
 //Clear picture
 Clear();
 //Create an error handler
 cinfo.err = jpeg_std_error(&jerr);
 //Initialize the decompression object
 jpeg_create_decompress(&cinfo);
 //Specify the data source(Our file pointer)
 jpeg_stdio_src(&cinfo,handle);
 //Decode the jpeg file and fill in the image data structure to pass back
 DecodeJPG(&cinfo);
 //This releases all the stored memory for reading and decoding the jpeg
 jpeg_destroy_decompress(&cinfo);
 //Finished
 ENCloseFile(handle);
 return true;
}

ENbool ENPicture::LoadPNG(char *FileName)
{
 //Variables
 png_structp png_ptr;
 png_infop info_ptr;
 unsigned int y;
 png_bytep * row_pointers;
 Byte color_type;
 ENFile handle;
 //Open file
 handle=ENOpenFile(FileName,"rb");
 if(!handle)
 {
  LastError=ENGINE_IMAGEERROR_OPEN;
  return false;
 }
 //Check header
 if(!CheckPNGHeader(handle)) return false;
 //Clear picture
 Clear();
 //Init
 png_ptr=png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
 if(!png_ptr)
 {
  LastError=ENGINE_IMAGEERROR_INTERNAL;
  return false;
 }
 info_ptr = png_create_info_struct(png_ptr);
 if(!info_ptr)
 {
  LastError=ENGINE_IMAGEERROR_INTERNAL;
  return false;
 }
 if(setjmp(png_jmpbuf(png_ptr)))
 {
  LastError=ENGINE_IMAGEERROR_INTERNAL;
  return false;
 }
 //Setup
 png_init_io(png_ptr,handle);
 png_set_sig_bytes(png_ptr,8);
 png_read_info(png_ptr,info_ptr);
 //Set header data
 width        = info_ptr->width;
 height       = info_ptr->height;
 color_type   = info_ptr->color_type;
 bpp=3;
 png_read_update_info(png_ptr, info_ptr);
 //Alloc space
 if(setjmp(png_jmpbuf(png_ptr)))
 {
  LastError=ENGINE_IMAGEERROR_INTERNAL;
  return false;
 }
 row_pointers=(png_bytep*)ENAllocMemory(sizeof(png_bytep)*height);
 for(y=0;y<height;y++)
   row_pointers[y]=(png_byte*)ENAllocMemory(info_ptr->rowbytes);
 //Read file
 png_read_image(png_ptr,row_pointers);
 //Process data
 switch(color_type)
 {
  case PNG_COLOR_TYPE_GRAY:
    ProcessPNG8GRAYData(row_pointers);
  break;
  case PNG_COLOR_TYPE_GRAY_ALPHA:
    ProcessPNG8GRAYAData(row_pointers);
  break;
  case PNG_COLOR_TYPE_PALETTE:
    ProcessPNG8PALData(row_pointers,(ENubyte*)info_ptr->palette);
  break;
  case PNG_COLOR_TYPE_RGB:
    ProcessPNG8RGBData(row_pointers);
  break;
  case PNG_COLOR_TYPE_RGBA:
    ProcessPNG8RGBAData(row_pointers);
  break;
  default:
    Clear();
    ENCloseFile(handle);
    LastError=ENGINE_IMAGEERROR_PROCESS;
    return false;  
 };
 //Free data
 for(y=0;y<height;y++)
   ENFreeMemory(row_pointers[y]);
 ENFreeMemory(row_pointers);
 //Finished
 ENCloseFile(handle);
 return true;
}

ENbool ENPicture::LoadTGA(char *FileName)
{
 //Variables
 ENFile  handle;
 ENubyte lenbyte,imageType,picbits;
 ENint   stride,i,y;
 WORD    picw,pich;
 //Open file
 handle=ENOpenFile(FileName,"rb");
 if(!handle)
 {
  LastError=ENGINE_IMAGEERROR_OPEN;
  return false;
 }
 Clear();
 //Read length byte
 ENReadFile(&lenbyte,1,1,handle);
 //Jump over one byte
 ENSeekFile(handle,1,SEEK_CUR);
 //Read in the imageType (RLE, RGB, etc...)
 ENReadFile(&imageType,sizeof(ENubyte),1,handle);
 //Skip past general information we don't care about
 ENSeekFile(handle,9,SEEK_CUR);
 //Read the width, height and bits per pixel (16, 24 or 32)
 ENReadFile(&picw,sizeof(WORD),1,handle);
 ENReadFile(&pich,sizeof(WORD),1,handle);
 ENReadFile(&picbits,sizeof(ENubyte),1,handle);

 width=picw;
 height=pich;
 //Now we move the file pointer to the pixel data
 ENSeekFile(handle,lenbyte+1,SEEK_CUR);
 //Check if the image is RLE compressed or not
 if(imageType!=TGA_RLE)
 {
  //Check if the image is a 24 or 32-bit image
  if(picbits==24||picbits==32)
  {
   //Calculate the channels (3 or 4) - (use bits >> 3 for more speed).
   //Next, we calculate the stride and allocate enough memory for the pixels.
   bpp = picbits/8;
   stride = bpp*width;
   data = ((ENubyte*)ENAllocMemory(sizeof(ENubyte)*stride*height));
   // Load in all the pixel data line by line
   for(y=0;y<(ENint)height;y++)
   {
    //Store a pointer to the current line of pixels
    ENubyte *pLine =data+stride*y;
    //Read in the current line of pixels
    ENReadFile(pLine,stride,1,handle);
    // Go through all of the pixels and swap the B and R values since TGA
    // files are stored as BGR instead of RGB (or use GL_BGR_EXT verses GL_RGB)
    for(i=0;i<stride;i+=bpp)
    {
     ENint temp=pLine[i];
     pLine[i]=pLine[i + 2];
     pLine[i+2]=temp;
    }
  }
 }
 // Check if the image is a 16 bit image (RGB stored in 1 unsigned short)
 else if(picbits==16)
 {
  ENushort pixels=0;
  ENint r=0,g=0,b=0;
  //Since we convert 16-bit images to 24 bit, we hardcode the channels to 3.
  //We then calculate the stride and allocate memory for the pixels.
  bpp = 3;
  stride = bpp*width;
  data=((ENubyte*)ENAllocMemory(sizeof(ENubyte)*stride*height));
  //Load in all the pixel data pixel by pixel
  for(i=0;i<(ENint)(width*height);i++)
  {
   //Read in the current pixel
   ENReadFile(&pixels,sizeof(ENushort),1,handle);
   //Convert the 16-bit pixel into an RGB
   b = (pixels & 0x1f) << 3;
   g = ((pixels >> 5) & 0x1f) << 3;
   r = ((pixels >> 10) & 0x1f) << 3;
   //This essentially assigns the color to our array and swaps the
   //B and R values at the same time.
   data[i*3+0]=r;
   data[i*3+1]=g;
   data[i*3+2]=b;
  }
 }
 else//Else return a NULL for a bad or unsupported pixel format
   return false;
 }
 else
 {
  //Create some variables to hold the rleID, current colors read, channels, & stride.
  ENubyte rleID=0;
  ENint colorsRead = 0;
  bpp=picbits/8;
  stride=bpp*width;
  //Next we want to allocate the memory for the pixels and create an array,
  //depending on the channel count, to read in for each pixel.
  data=((ENubyte*)ENAllocMemory(sizeof(ENubyte)*stride*height));
  ENubyte *pColors=((ENubyte*)ENAllocMemory(sizeof(ENubyte)*bpp));
  //Load in all the pixel data
  while(i<(ENint)(width*height))
  {
   //Read in the current color count + 1
   ENReadFile(&rleID,sizeof(ENubyte),1,handle);
   //Check if we don't have an encoded string of colors
   if(rleID<128)
   {
    //Increase the count by 1
    rleID++;
    //Go through and read all the unique colors found
    while(rleID)
    {
     //Read in the current color
     ENReadFile(pColors,sizeof(ENubyte)*bpp,1,handle);
     //Store the current pixel in our image array
     data[colorsRead+0]=pColors[2];
     data[colorsRead+1]=pColors[1];
     data[colorsRead+2]=pColors[0];
     //If we have a 4 channel 32-bit image, assign one more for the alpha
     if(picbits==32)
       data[colorsRead+3]=pColors[3];
     //Increase the current pixels read, decrease the amount
     //of pixels left, and increase the starting index for the next pixel.
     i++;
     rleID--;
     colorsRead+=bpp;
    }
   }
   else//Else, let's read in a string of the same character
   {
    //Minus the 128 ID + 1 (127) to get the color count that needs to be read
    rleID-=127;
    //Read in the current color, which is the same for a while
    ENReadFile(pColors,sizeof(ENubyte)*bpp,1,handle);
    //Go and read as many pixels as are the same
    while(rleID)
    {
     //Assign the current pixel to the current index in our pixel array
     data[colorsRead+0]=pColors[2];
     data[colorsRead+1]=pColors[1];
     data[colorsRead+2]=pColors[0];
     //If we have a 4 channel 32-bit image, assign one more for the alpha
     if(picbits==32)
       data[colorsRead+3]=pColors[3];
     //Increase the current pixels read, decrease the amount
     //of pixels left, and increase the starting index for the next pixel.
     i++;
     rleID--;
     colorsRead+=bpp;
    }
   }
  }
 }
 //Finished
 ENCloseFile(handle);
 return true;
}

ENbool ENPicture::LoadFromFile(char *FileName)
{
 //Variables
 ENFile handle;
 ENIMGAGEHEADER header;
 //Open file
 handle=ENOpenFile(FileName,"rb");
 if(!handle)
 {
  LastError=ENGINE_IMAGEERROR_OPEN;
  return false;
 }
 //Read header
 ENReadFile(&header,sizeof(ENIMGAGEHEADER),1,handle);
 if(header.VERSION!=1||header.ID[0]!='E'||header.ID[1]!='I')
 {
  LastError=ENGINE_IMAGEERROR_UNKNOWN_FMT;
  ENCloseFile(handle);
  return false;
 }
 //Set data
 Clear();
 width=header.width;
 height=header.height;
 bpp=header.bpp;
 data=(ENubyte*)ENAllocMemory(width*height*bpp);
 //Read pixel data
 ENReadFile(data,bpp,width*height,handle);
 //Finished
 ENCloseFile(handle);
 return true;
}

ENbool ENPicture::LoadFromHandle(ENFile handle)
{
 //Variables
 ENIMGAGEHEADER header;
 //Read header
 ENReadFile(&header,sizeof(ENIMGAGEHEADER),1,handle);
 if(header.VERSION!=1||header.ID[0]!='E'||header.ID[1]!='I')
 {
  LastError=ENGINE_IMAGEERROR_UNKNOWN_FMT;
  return false;
 }
 //Set data
 Clear();
 width=header.width;
 height=header.height;
 bpp=header.bpp;
 data=(ENubyte*)ENAllocMemory(width*height*bpp);
 //Read pixel data
 ENReadFile(data,bpp,width*height,handle);
 //Finished
 return true;
}

ENbool ENPicture::WriteBMP(char *FileName)
{
 //Variables
 ENFile handle;
 //Open file
 handle=ENOpenFile(FileName,"wb");
 if(!handle)
 {
  LastError=ENGINE_IMAGEERROR_WRITE;
  return false;
 }
 //Calculate offset for scanlines
 ENuint osl;
 if((width*3)%4==0)
   osl=0;
 else
   osl=4-(width*3)%4;
 //Write bitmap file header
 ENBITMAPFILEHEADER fheader;
 fheader.bfType='B'+('M'<<8);
 fheader.bfOffBits=sizeof(ENBITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
 fheader.bfSize=fheader.bfOffBits+(width*3+osl)*height;
 fheader.bfReserved1=0;
 fheader.bfReserved2=0;
 ENWriteFile(&fheader,sizeof(ENBITMAPFILEHEADER),1,handle);
 //Write bitmap info header
 BITMAPINFOHEADER iheader;
 iheader.biSize=sizeof(BITMAPINFOHEADER);
 iheader.biWidth=width;
 iheader.biHeight=height;
 iheader.biPlanes=1;
 iheader.biBitCount=24;
 iheader.biCompression=BI_RGB;
 iheader.biSizeImage=(width*3+osl)*height;
 iheader.biXPelsPerMeter=0;
 iheader.biYPelsPerMeter=0;
 iheader.biClrUsed=0;
 iheader.biClrImportant=0;
 ENWriteFile(&iheader,sizeof(BITMAPINFOHEADER),1,handle);
 //Write pixeldata
 for(ENuint y=0;y<height;y++)
 {
  for(ENuint x=0;x<width;x++)
  {
   ENubyte pixel[3];
   memcpy(pixel,data+((height-y-1)*width+x)*bpp,3);
   TwistBytes(pixel,3);
   ENWriteFile(pixel,3,1,handle);
  }
  ENSeekFile(handle,osl,SEEK_CUR);  
 }
 //Finished
 ENCloseFile(handle);
 return true;
}

ENbool ENPicture::WritePNG(char *FileName)
{
 //Variables
 ENFile handle;
 ENuint  x,y;
 png_structp png_ptr;
 png_infop   info_ptr;
 png_byte    color_type;
 png_byte  **row_pointers;
 //Open file
 handle=ENOpenFile(FileName,"wb");
 if(!handle)
 {
  LastError=ENGINE_IMAGEERROR_WRITE;
  return false;
 }
 //Init
 png_ptr=png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
 if(!png_ptr)
 {
  LastError=ENGINE_IMAGEERROR_INTERNAL;
  ENCloseFile(handle);
  return false;
 }
 info_ptr=png_create_info_struct(png_ptr);
 if(!info_ptr)
 {
  LastError=ENGINE_IMAGEERROR_INTERNAL;
  ENCloseFile(handle);
  return false;
 }
 if(setjmp(png_jmpbuf(png_ptr)))
 {
  LastError=ENGINE_IMAGEERROR_INTERNAL;
  ENCloseFile(handle);
  return false;
 }
 png_init_io(png_ptr,handle);
 //Process data
 row_pointers=(png_byte**)ENAllocMemory(sizeof(png_byte*)*height);
 for(y=0;y<height;y++)
 {
  row_pointers[y]=(png_byte*)ENAllocMemory(width*height*bpp);
  for(x=0;x<width;x++)
    memcpy(row_pointers[y],data+y*width*bpp,width*bpp);
 }  
 //Write header
 if(setjmp(png_jmpbuf(png_ptr)))
 {
  LastError=ENGINE_IMAGEERROR_INTERNAL;
  ENCloseFile(handle);
  return false;
 }
 if(bpp==3)
   color_type=PNG_COLOR_TYPE_RGB;
 else
   color_type=PNG_COLOR_TYPE_RGBA;
 png_set_IHDR(png_ptr,info_ptr,width,height,8,color_type,PNG_INTERLACE_NONE,
	      PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
 png_write_info(png_ptr, info_ptr);
 //Write data
 if(setjmp(png_jmpbuf(png_ptr)))
 {
  LastError=ENGINE_IMAGEERROR_INTERNAL;
  ENCloseFile(handle);
  return false;
 }
 png_write_image(png_ptr,row_pointers);
 //End write
 if(setjmp(png_jmpbuf(png_ptr)))
 {
  LastError=ENGINE_IMAGEERROR_INTERNAL;
  ENCloseFile(handle);
  return false;
 }
 png_write_end(png_ptr, NULL);
 //Finished
 for(y=0;y<height;y++)
   ENFreeMemory(row_pointers[y]);

 ENFreeMemory(row_pointers);
 ENCloseFile(handle);
 return true;
}

ENbool ENPicture::SaveToFile(char *FileName)
{
 //Variables
 ENFile handle;
 ENIMGAGEHEADER header;
 //Open file
 handle=ENOpenFile(FileName,"wb");
 if(!handle)
 {
  LastError=ENGINE_IMAGEERROR_WRITE;
  return false;
 }
 //Write header
 header.VERSION=1;
 header.ID[0]='E';
 header.ID[1]='I';
 header.width=width;
 header.height=height;
 header.bpp=bpp;
 ENWriteFile(&header,sizeof(ENIMGAGEHEADER),1,handle);
 //Write pixel data
 ENWriteFile(data,bpp,width*height,handle);
 //Finished
 ENCloseFile(handle);
 return true;
}

void ENPicture::SaveToHandle(ENFile handle)
{
 //Variables
 ENIMGAGEHEADER header;
 //Write header
 header.VERSION=1;
 header.ID[0]='E';
 header.ID[1]='I';
 header.width=width;
 header.height=height;
 header.bpp=bpp;
 ENWriteFile(&header,sizeof(ENIMGAGEHEADER),1,handle);
 //Write pixel data
 ENWriteFile(data,bpp,width*height,handle);
}

ENuint ENPicture::GetPixel(ENuint x,ENuint y)
{
 if(x>=width||y>=height)
   return 0;
 else
 {
  ENuint res=0;
  memcpy(&res,data+(y*width+x)*bpp,3);
  return res;
 }
}

ENubyte ENPicture::GetAlpha(ENuint x,ENuint y)
{
 if(x>=width||y>=height||bpp<=3)
   return 255;
 else
   return data[(y*width+x)*bpp+3];
}

void ENPicture::SetAlpha(ENuint x,ENuint y,ENubyte alpha)
{
 if(x<width||y<height||bpp>3)
   data[(y*width+x)*bpp+3]=alpha;
}

ENuint ENPicture::GetLastError()
{
 return LastError;
}

void ENPicture::GetLastErrorStr(char *errorstr)
{
 strcpy(errorstr,ENIMAGEERRORSTRS[LastError]);
}

ENuint ENPicture::GetWidth()
{
 return width;
}

ENuint ENPicture::GetHeight()
{
 return height;
}

ENuint ENPicture::GetBpp()
{
 return bpp;
}

void ENPicture::GetData(ENubyte *dst)
{
 memcpy(dst,data,width*height*bpp);
}

void ENPicture::GetScanLine(ENubyte *dst,ENuint y)
{
 memcpy(dst,data+y*width*bpp,width*bpp);
}

ENbool ENPicture::GetImgType(char *FileName,ENuint &ImgType)
{
 //Variables
 ENIMGAGEHEADER   ownheader;
 ENBITMAPFILEHEADER bmpheader;
 ENPCXHEADER      pcxheader;
 char             Buffer[MAX_PATH];
 char             jpgheader[10];
 unsigned char    pngheader[8];
 ENuint           len;
 ENFile handle;
 //Check for TGA
 len=strlen(FileName);
 if(len>4)
   if(strcmpi(FileName+len-4,".tga")==0)
   {
    ImgType=ENGINE_IMGTYPE_TGA;
    return true;
   }
 //Open file
 handle=ENOpenFile(FileName,"rb");
 if(!handle)
 {
  LastError=ENGINE_IMAGEERROR_OPEN;
  return false;
 }
 //Check for windows bitmap
 ENReadFile(&bmpheader,sizeof(ENBITMAPFILEHEADER),1,handle);
 if((char)(bmpheader.bfType)=='B'&&
    (char)(bmpheader.bfType>>8)=='M')
    {
     ImgType=ENGINE_IMGTYPE_BMP;
     ENCloseFile(handle);
     return true;
    }
 ENSeekFile(handle,0,SEEK_SET);
 //Check for zsoft pcx
 ENReadFile(&pcxheader,sizeof(ENPCXHEADER),1,handle);
 if(pcxheader.PCXID==0xA)
    {
     ImgType=ENGINE_IMGTYPE_PCX;
     ENCloseFile(handle);
     return true;
    }
 ENSeekFile(handle,0,SEEK_SET);
 //Check for jpeg
 ENReadFile(jpgheader,10,1,handle);
 if(jpgheader[6]=='J'&&jpgheader[7]=='F'&&
    jpgheader[8]=='I'&&jpgheader[9]=='F')
    {
     ImgType=ENGINE_IMGTYPE_JPG;
     ENCloseFile(handle);
     return true;
    }
 ENSeekFile(handle,0,SEEK_SET);
 //Check for png
 ENReadFile(pngheader,8,1,handle);
 if(!png_sig_cmp(pngheader,0,8))
 {
  ENCloseFile(handle);
  ImgType=ENGINE_IMGTYPE_PNG;
  return true;
 }
 ENSeekFile(handle,0,SEEK_SET);
 //Check for own format
 ENReadFile(&ownheader,sizeof(ENIMGAGEHEADER),1,handle);
 if(ownheader.ID[0]=='E'&&ownheader.ID[1]=='I')
 {
  ENCloseFile(handle);
  ImgType=ENGINE_IMGTYPE_OWN;
  return true;
 }
 ENSeekFile(handle,0,SEEK_SET);
 //Finished
 ENCloseFile(handle);
 ImgType=ENGINE_IMGTYPE_UNKNOWN;
 return true;
}

ENuint ENPicture::GetNumBytes()
{
 return sizeof(ENIMGAGEHEADER)+width*height*bpp;
}

void   ENPicture::GetBytes(ENubyte *bytes)
{
 //Variables
 ENIMGAGEHEADER header;
 //Write header
 header.VERSION=1;
 header.ID[0]='E';
 header.ID[1]='I';
 header.width=width;
 header.height=height;
 header.bpp=bpp;
 memcpy(bytes,&header,sizeof(ENIMGAGEHEADER));
 //Write pixel data
 memcpy(bytes+sizeof(ENIMGAGEHEADER),data,width*height*bpp);
}

ENbool ENPicture::IsPicture(char *FileName)
{
 //Variables
 ENIMGAGEHEADER   ownheader;
 ENBITMAPFILEHEADER bmpheader;
 ENPCXHEADER      pcxheader;
 char             jpgheader[10];
 unsigned char    pngheader[8];
 ENuint           len;
 ENFile handle;
 //Open file
 handle=ENOpenFile(FileName,"rb");
 if(!handle) return false;
 //Check for TGA
 len=strlen(FileName);
 if(len>4)
   if(strcmpi(FileName+len-4,".tga")==0)
     return true;
 //Check for windows bitmap
 ENReadFile(&bmpheader,sizeof(ENBITMAPFILEHEADER),1,handle);
 if((char)(bmpheader.bfType)=='B'&&
    (char)(bmpheader.bfType>>8)=='M')
    {
     ENCloseFile(handle);
     return true;
    }
 ENSeekFile(handle,0,SEEK_SET);
 //Check for zsoft pcx
 ENReadFile(&pcxheader,sizeof(ENPCXHEADER),1,handle);
 if(pcxheader.PCXID==0xA)
    {
     ENCloseFile(handle);
     return true;
    }
 ENSeekFile(handle,0,SEEK_SET);
 //Check for jpeg
 ENReadFile(jpgheader,10,1,handle);
 if(jpgheader[6]=='J'&&jpgheader[7]=='F'&&
    jpgheader[8]=='I'&&jpgheader[9]=='F')
    {
     ENCloseFile(handle);
     return true;
    }
 ENSeekFile(handle,0,SEEK_SET);
 //Check for png
 ENReadFile(pngheader,8,1,handle);
 if(!png_sig_cmp(pngheader,0,8))
 {
  ENCloseFile(handle);
  return true;
 }
 ENSeekFile(handle,0,SEEK_SET);
 //Check for own format
 ENReadFile(&ownheader,sizeof(ENIMGAGEHEADER),1,handle);
 if(ownheader.ID[0]=='E'&&ownheader.ID[1]=='I')
 {
  ENCloseFile(handle);
  return true;
 }
 ENSeekFile(handle,0,SEEK_SET);
 //Finished
 ENCloseFile(handle);
 return false;
}

const char *ENPicture::GetImageExtStr()
{
 return ENIMAGEEXT;
}

ENbool ENPicture::Load(char *FileName)
{
 //Variables
 ENuint ImgType;
 //Get type
 if(!GetImgType(FileName,ImgType)) return false;
 //Load file
 switch(ImgType)
 {
  case ENGINE_IMGTYPE_BMP:
    return LoadBMP(FileName);
  case ENGINE_IMGTYPE_PCX:
    return LoadPCX(FileName);
  case ENGINE_IMGTYPE_JPG:
    return LoadJPG(FileName);
  case ENGINE_IMGTYPE_PNG:
    return LoadPNG(FileName);
  case ENGINE_IMGTYPE_TGA:
    return LoadTGA(FileName);
  case ENGINE_IMGTYPE_OWN:
    return LoadFromFile(FileName);
  default:
    LastError=ENGINE_IMAGEERROR_UNKNOWN;
    return false;
 };
}

ENbool ENPicture::CheckBMPHeader(ENBITMAPFILEHEADER *bmpheader,BITMAPINFOHEADER *bmpinfo)
{
 //Check if it is a known format
 if((char)(bmpheader->bfType)!='B'||
    (char)(bmpheader->bfType>>8)!='M')
    {
     LastError=ENGINE_IMAGEERROR_UNKNOWN_FMT;
     return false;
    }
 //Check if it is a compressed bitmap
 if(bmpinfo->biCompression!=BI_RGB)
 {
  LastError=ENGINE_IMAGEERROR_COMPRESSED;
  return false;
 }
 //Finished, allright
 return true;
}

ENbool ENPicture::CheckPCXHeader(ENPCXHEADER *pcxheader)
{
 //Check if it is a known format
 if(pcxheader->PCXID!=0xA)
 {
  LastError=ENGINE_IMAGEERROR_UNKNOWN_FMT;
  return false;
 }
 //Check if it is a compressed pcx
 if(pcxheader->COMPRESS!=1)
 {
  LastError=ENGINE_IMAGEERROR_INVALID;
  return false;
 }
 //Finished
 return true;
}

ENbool ENPicture::CheckPNGHeader(ENFile handle)
{
 unsigned char pngheader[8];
 ENReadFile(pngheader,1,8,handle);
 if(png_sig_cmp(pngheader,0,8))
 {
  LastError=ENGINE_IMAGEERROR_UNKNOWN_FMT;
  return false;
 }
 else
   return true;
}

void ENPicture::ProcessBMP24Data(ENubyte *pixeldata,ENuint TotalSize)
{
 //Calculate scanline size
 ENuint ScanLineSize=TotalSize/height;
 //Alloc space for processes pixel data
 data=(ENubyte*)ENAllocMemory(width*height*bpp);
 //Process
 for(ENuint l=0;l<height;l++)
   for(ENuint s=0;s<width;s++)
   {
    memcpy(data+(l*width+s)*bpp,pixeldata+(height-l-1)*ScanLineSize+s*bpp,bpp);
    TwistBytes(data+(l*width+s)*bpp,bpp);
   }
}

void ENPicture::ProcessBMP8Data(ENubyte *pixeldata,ENuint TotalSize,ENubyte *Colors)
{
 ENubyte cind;
 //Convert colors
 for(ENuint c=0;c<256;c++)
   TwistBytes(Colors+c*4,3);
 //Calculate scanline size
 ENuint ScanLineSize=TotalSize/height;
 //Alloc space for processes pixel data
 data=(ENubyte*)ENAllocMemory(width*height*bpp);
 //Process
 for(ENuint l=0;l<height;l++)
   for(ENuint s=0;s<width;s++)
   {
    cind=pixeldata[(height-l-1)*ScanLineSize+s];
    memcpy(data+(l*width+s)*3,Colors+cind*4,3);
   }
}

void ENPicture::ProcessBMP4Data(ENubyte *pixeldata,ENuint TotalSize,ENubyte *Colors)
{
 ENubyte cind;
 //Convert colors
 for(ENuint c=0;c<16;c++)
   TwistBytes(Colors+c*4,3);
 //Calculate scanline size
 ENuint ScanLineSize=TotalSize/height;
 //Alloc space for processes pixel data
 data=(ENubyte*)ENAllocMemory(width*height*bpp);
 //Process
 for(ENuint l=0;l<height;l++)
   for(ENuint s=0;s<width;s++)
   {
    cind=pixeldata[(height-l-1)*ScanLineSize+s/2];
    if(s%2==0)
      cind=(ENubyte)(cind&0xF);
    else
      cind=(ENubyte)(cind>>4);
    memcpy(data+(l*width+s)*3,Colors+cind*4,3);
   }
}

void ENPicture::ProcessBMP1Data(ENubyte *pixeldata,ENuint TotalSize)
{
 ENubyte cind;
 //Calculate scanline size
 ENuint ScanLineSize=TotalSize/height;
 //Alloc space for processes pixel data
 data=(ENubyte*)ENAllocMemory(width*height*bpp);
 //Process
 for(ENuint l=0;l<height;l++)
   for(ENuint s=0;s<width;s++)
   {
    cind=pixeldata[(height-l-1)*ScanLineSize+s/8];
    cind&=(ENubyte)(1<<(7-s%8));
    if(cind)
      memset(data+(l*width+s)*3,255,3);
    else
      memset(data+(l*width+s)*3,0,3);
   }
}

void ENPicture::ProcessPCX24Data(ENFile handle,ENuint BPL)
{
 //Variables
 ENubyte pixel;
 ENuint x;
 //Alloc space for pcx data
 ENubyte *tmp=(ENubyte*)ENAllocMemory(BPL*3);
 data=(ENubyte*)ENAllocMemory(width*height*3);
 //Read pcx data
 for(ENuint y=0;y<height;y++)
 {
  x=0;
  while(x<BPL*3)
  {
   //Read pixel
   ENReadFile(&pixel,1,1,handle);
   //Check if byte is a length byte
   if(pixel>192)
   {
    ENuint num=pixel-192;
    ENReadFile(&pixel,1,1,handle);
    for(ENuint i=0;i<num;i++)
    {
     tmp[x]=pixel;
     x++;
    }
   }
   else//Byte is not a length byte
   {
    tmp[x]=pixel;
    x++;
   }
  }
  //Copy scanline data
  for(x=0;x<width;x++)
  {
   data[(y*width+x)*3]=tmp[x];
   data[(y*width+x)*3+1]=tmp[BPL+x];
   data[(y*width+x)*3+2]=tmp[2*BPL+x];
  }
 }
 //Free temporary memory
 ENFreeMemory(tmp);
}

void ENPicture::ProcessPCX8Data(ENFile handle,ENuint BPL)
{
 //Variables
 ENubyte pixel;
 ENuint x,y;
 //Alloc space for pcx data
 ENubyte *tmp=(ENubyte*)ENAllocMemory(BPL);
 data=(ENubyte*)ENAllocMemory(width*height*3);
 //Read pcx data
 for(y=0;y<height;y++)
 {
  x=0;
  while(x<BPL)
  {
   //Read pixel
   ENReadFile(&pixel,1,1,handle);
   //Check if byte is a length byte
   if(pixel>192)
   {
    ENuint num=pixel-192;
    ENReadFile(&pixel,1,1,handle);
    for(ENuint i=0;i<num;i++)
    {
     tmp[x]=pixel;
     x++;
    }
   }
   else//Byte is not a length byte
   {
    tmp[x]=pixel;
    x++;
   }
  }
  //Copy scanline data
  for(x=0;x<width;x++)
  {
   data[(y*width+x)*3]=tmp[x];
   data[(y*width+x)*3+1]=tmp[x];
   data[(y*width+x)*3+2]=tmp[x];
  }
 }
 //Free temporary memory
 ENFreeMemory(tmp);
 //Read colormap
 ENubyte  Colors[256*3];
 ENSeekFile(handle,1,SEEK_CUR);
 ENReadFile(Colors,3,256,handle);
 for(y=0;y<height;y++)
   for(x=0;x<width;x++)
   {
    data[(y*width+x)*3]=Colors[data[(y*width+x)*3]*3];
    data[(y*width+x)*3+1]=Colors[data[(y*width+x)*3+1]*3+1];
    data[(y*width+x)*3+2]=Colors[data[(y*width+x)*3+2]*3+2];
   }
}

void ENPicture::ProcessPCX4Data(ENFile handle,ENuint BPL,ENubyte *Colormap)
{
 //Variables
 ENubyte pixel,cind;
 ENuint x,y;
 //Alloc space for pcx data
 ENubyte *tmp=(ENubyte*)ENAllocMemory(BPL);
 data=(ENubyte*)ENAllocMemory(width*height*3);
 //Read pcx data
 for(y=0;y<height;y++)
 {
  x=0;
  while(x<BPL)
  {
   //Read pixel
   ENReadFile(&pixel,1,1,handle);
   //Check if byte is a length byte
   if(pixel>192)
   {
    ENuint num=pixel-192;
    ENReadFile(&pixel,1,1,handle);
    for(ENuint i=0;i<num;i++)
    {
     tmp[x]=pixel;
     x++;
    }
   }
   else//Byte is not a length byte
   {
    tmp[x]=pixel;
    x++;
   }
  }
  //Copy scanline data
  for(x=0;x<width;x++)
  {
   cind=tmp[x/2];
   if(x%2==0)
     cind=(ENubyte)(cind&0xF);
   else
     cind=(ENubyte)(cind>>4);

   data[(y*width+x)*3]=Colormap[cind*3];
   data[(y*width+x)*3+1]=Colormap[cind*3+1];
   data[(y*width+x)*3+2]=Colormap[cind*3+2];
  }
 }
 //Free temporary memory
 ENFreeMemory(tmp);
}

void ENPicture::ProcessPCX1Data(ENFile handle,ENuint BPL)
{
 //Variables
 ENubyte pixel,cind;
 ENuint x,y;
 //Alloc space for pcx data
 ENubyte *tmp=(ENubyte*)ENAllocMemory(BPL);
 data=(ENubyte*)ENAllocMemory(width*height*3);
 //Read pcx data
 for(y=0;y<height;y++)
 {
  x=0;
  while(x<BPL)
  {
   //Read pixel
   ENReadFile(&pixel,1,1,handle);
   //Check if byte is a length byte
   if(pixel>192)
   {
    ENuint num=pixel-192;
    ENReadFile(&pixel,1,1,handle);
    for(ENuint i=0;i<num;i++)
    {
     tmp[x]=pixel;
     x++;
    }
   }
   else//Byte is not a length byte
   {
    tmp[x]=pixel;
    x++;
   }
  }
  //Copy scanline data
  for(x=0;x<width;x++)
  {
   cind=tmp[x/8];
   cind&=(ENubyte)(1<<(7-x%8));
    if(cind)
      memset(data+(y*width+x)*3,255,3);
    else
      memset(data+(y*width+x)*3,0,3);
  }
 }
 //Free temporary memory
 ENFreeMemory(tmp);
}

void ENPicture::ProcessPNG8GRAYData(ENubyte **Rows)
{
 bpp=3;
 data=(unsigned char*)ENAllocMemory(width*height*bpp);
 for(ENuint y=0;y<height;y++)
   for(ENuint x=0;x<width;x++)
     memset(data+(y*width+x)*bpp,Rows[y][x],bpp);
}

void ENPicture::ProcessPNG8GRAYAData(ENubyte **Rows)
{
 bpp=4;
 data=(unsigned char*)ENAllocMemory(width*height*bpp);
 for(ENuint y=0;y<height;y++)
   for(ENuint x=0;x<width;x++)
   {
    memset(data+(y*width+x)*bpp,Rows[y][x*2],bpp);
    data[(y*width+x)*bpp+3]=Rows[y][x*2+1];
   }
}

void ENPicture::ProcessPNG8PALData(ENubyte **Rows,ENubyte *palette)
{
 bpp=3;
 data=(unsigned char*)ENAllocMemory(width*height*bpp);
 for(ENuint y=0;y<height;y++)
   for(ENuint x=0;x<width;x++)
   {
    ENuint ind=Rows[y][x];
    memcpy(data+(y*width+x)*bpp,palette+ind*bpp,bpp);
   }
}

void ENPicture::ProcessPNG8RGBData(ENubyte **Rows)
{
 bpp=3;
 data=(unsigned char*)ENAllocMemory(width*height*bpp);
 for(ENuint y=0;y<height;y++)
   memcpy(data+y*width*bpp,Rows[y],width*bpp);
}

void ENPicture::ProcessPNG8RGBAData(ENubyte **Rows)
{
 bpp=4;
 data=(unsigned char*)ENAllocMemory(width*height*bpp);
 for(ENuint y=0;y<height;y++)
   memcpy(data+y*width*bpp,Rows[y],width*bpp);
}

void ENPicture::TwistBytes(ENubyte *data,ENuint size)
{
 ENuint hs=size/2;
 for(ENuint a=0;a<hs;a++)
 {
  ENubyte b=data[a];
  data[a]=data[size-1-a];
  data[size-1-a]=b;
 }
}

void ENPicture::Resize(ENuint w,ENuint h)
{
 //Variables
 ENubyte *NewData=(ENubyte*)ENAllocMemory(w*h*bpp);
 ENubyte  Pixel[4];
 ENuint  nx,ny;
 FLOAT fx,fy;
 //Size factors
 fx=(FLOAT)width/w;
 fy=(FLOAT)height/h;

 for(ENuint x=0;x<w;x++)
   for(ENuint y=0;y<h;y++)
   {
    nx=x*fx;
    ny=y*fy;
    //Get from src
    memcpy(Pixel,&data[(ny*width+nx)*bpp],bpp);
    //Put into dst
    memcpy(&NewData[(y*w+x)*bpp],Pixel,bpp);
   }
 //Set new data
 ENFreeMemory(data);
 data=NewData;
 width=w;
 height=h;
}

void ENPicture::Power2()
{
 //Vars
 ENuint w,h;
 //Make size power of 2
 w=Make2(width);
 h=Make2(height);
 //Resize if needed
 if(w!=width||h!=height)
   Resize(w,h);
}

void ENPicture::DeleteAlpha()
{
 //Vars
 ENubyte *TempData;
 ENuint  x,y;
 ENubyte  r,g,b;
 //Check if alpha exists
 if(bpp<=3) return;
 //Create new buffer
 TempData=(ENubyte*)ENAllocMemory(width*height*3);
 for(x=0;x<width;x++)
   for(y=0;y<height;y++)
   {
    r=data[(y*width+x)*bpp];
    g=data[(y*width+x)*bpp+1];
    b=data[(y*width+x)*bpp+2];
    TempData[(y*width+x)*3]=r;
    TempData[(y*width+x)*3+1]=g;
    TempData[(y*width+x)*3+2]=b;
   }
 //Change bytes per pixel
 bpp=3;
 //Change buffer
 ENFreeMemory(data);
 data=TempData;
}

void ENPicture::SetStdAlpha()
{
 //Vars
 ENubyte *TempData;
 ENuint  x,y;
 ENubyte  r,g,b;
 //Create new buffer
 TempData=(ENubyte*)ENAllocMemory(width*height*4);
 for(x=0;x<width;x++)
   for(y=0;y<height;y++)
   {
    r=data[(y*width+x)*bpp];
    g=data[(y*width+x)*bpp+1];
    b=data[(y*width+x)*bpp+2];
    TempData[(y*width+x)*4]=r;
    TempData[(y*width+x)*4+1]=g;
    TempData[(y*width+x)*4+2]=b;
    TempData[(y*width+x)*4+3]=(ENubyte)((r+g+b)/3);
   }
 //Change bytes per pixel
 bpp=4;
 //Change buffer
 ENFreeMemory(data);
 data=TempData;
}

void ENPicture::SetTransparent(ENubyte red,ENubyte green,ENubyte blue)
{
 //Vars
 ENubyte *TempData;
 ENuint  x,y;
 ENubyte  r,g,b;
 //Create new buffer
 TempData=(ENubyte*)ENAllocMemory(width*height*4);
 for(x=0;x<width;x++)
   for(y=0;y<height;y++)
   {
    r=data[(y*width+x)*bpp];
    g=data[(y*width+x)*bpp+1];
    b=data[(y*width+x)*bpp+2];
    TempData[(y*width+x)*4]=r;
    TempData[(y*width+x)*4+1]=g;
    TempData[(y*width+x)*4+2]=b;
    if(r==red&&g==green&&b==blue)
      TempData[(y*width+x)*4+3]=0;
    else
      TempData[(y*width+x)*4+3]=255;
   }
 //Change bytes per pixel
 bpp=4;
 //Change buffer
 ENFreeMemory(data);
 data=TempData;
}

ENuint ENPicture::Make2(ENuint x)
{
 ENuint res;
 //Check if x is valid
 if(!x) return x;
 //Get exponent based to 2
 res=ceil(log(x)/log(2));
 //Calculate quadratic number
 res=pow(2,res);
 return res;
}

void ENPicture::DecodeJPG(void *_cinfo)
{
 //Variables
 ENuint rowSpan,rowsRead;
 jpeg_decompress_struct* cinfo=(jpeg_decompress_struct*)_cinfo;
 //Read jpg header
 jpeg_read_header(cinfo, TRUE);
 //Start to decompress the jpeg file
 jpeg_start_decompress(cinfo);
 //Get image info
 bpp    = cinfo->num_components;
 width  = cinfo->image_width;
 height = cinfo->image_height;
 //Get the row span in bytes for each row
 rowSpan = cinfo->image_width*cinfo->num_components;
 //Allocate memory
 data=(ENubyte*)ENAllocMemory(sizeof(ENubyte)*rowSpan*height);
 //Create an array of row pointers
 ENubyte** rowPtr = new ENubyte*[height];
 //Read out data
 for(ENuint i=0;i<height;i++)
    rowPtr[i]=&(data[i*rowSpan]);

 //Extract all the pixel data
 rowsRead=0;
 while(cinfo->output_scanline<cinfo->output_height)
   rowsRead+=jpeg_read_scanlines(cinfo,&rowPtr[rowsRead],cinfo->output_height-rowsRead);
 //Delete row pointers
 delete [] rowPtr;
 //Finish decompressing data
 jpeg_finish_decompress(cinfo);
}

///////////////////////////////////////////////////////////////////////
///Engine Picture loaded
///////////////////////////////////////////////////////////////////////
ENPictureLoaded::ENPictureLoaded(ENPicture *data)
{
 //Variables
 ENPicture *temp;
 //Generate IDs
 glGenTextures(3,IDs);
 //Base texture
 glBindTexture(GL_TEXTURE_2D,IDs[0]);
 SetTexFlags();
 if(data->GetBpp()==3)
   data->InitGLTex();
 else
 {
  temp=new ENPicture(data);
  temp->DeleteAlpha();
  temp->InitGLTex();
  delete temp;
 }
 //Transparent
 glBindTexture(GL_TEXTURE_2D,IDs[1]);
 SetTexFlags();
 temp=new ENPicture(data);
 temp->SetTransparent(0,0,0);
 temp->InitGLTex();
 delete temp;
 //Alpha
 glBindTexture(GL_TEXTURE_2D,IDs[2]);
 SetTexFlags();
 if(data->GetBpp()==3)
 {
  temp=new ENPicture(data);
  temp->SetStdAlpha();
  temp->InitGLTex();
  delete temp;
 }
 else
   data->InitGLTex();
}

ENPictureLoaded::~ENPictureLoaded()
{
 glDeleteTextures(3,IDs);
}

void ENPictureLoaded::Set(ENuint mode)
{
 glBindTexture(GL_TEXTURE_2D,IDs[mode%3]);
}

void ENPictureLoaded::SetTexFlags()
{
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
