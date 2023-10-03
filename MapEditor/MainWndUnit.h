//---------------------------------------------------------------------------
#ifndef MainWndUnitH
#define MainWndUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <comctrls.hpp>
#include "ENDrawClass.h"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TMainWnd : public TForm
{
__published:	// IDE-managed Components
        TPopupMenu *PopupMenu1;
        TMenuItem *Selection1;
        TMenuItem *Select1;
        TMenuItem *Selectall1;
        TMenuItem *Invertselection1;
        TMenuItem *Selectnothing1;
        TMenuItem *Selectconnected1;
        TMenuItem *Camera1;
        TMenuItem *MoveCamera1;
        TMenuItem *RotateCamera1;
        TMenuItem *ZoomCamera1;
        TMenuItem *Object1;
        TMenuItem *Moveobject1;
        TMenuItem *Rotateobject1;
        TMenuItem *Scaleobject1;
        TMenuItem *Add1;
        TMenuItem *Vertex1;
        TMenuItem *Face1;
        TMenuItem *Cube1;
        TMenuItem *Cone1;
        TMenuItem *Cylinder1;
        TMenuItem *Sphere1;
        TMenuItem *Edit1;
        TMenuItem *DeleteFaces1;
        TMenuItem *DeleteVertexes1;
        TMenuItem *FlipNormals1;
        TStatusBar *MainWndStat;
        TMainMenu *MainMenu1;
        TMenuItem *File1;
        TMenuItem *New1;
        TMenuItem *N2;
        TMenuItem *Open1;
        TMenuItem *Import3DS1;
        TMenuItem *ImportEAM1;
        TMenuItem *N1;
        TMenuItem *Save1;
        TMenuItem *SaveAs1;
        TMenuItem *ExportEAM1;
        TMenuItem *N3;
        TMenuItem *Exit1;
        TMenuItem *Windows1;
        TMenuItem *Cascade1;
        TMenuItem *Tile1;
        TMenuItem *N5;
        TMenuItem *manager1;
        TMenuItem *Showall1;
        TMenuItem *N4;
        TMenuItem *Options1;
        TMenuItem *Mapstatus1;
        TMenuItem *N6;
        TMenuItem *Mode1;
        TMenuItem *Wireframe1;
        TMenuItem *Solid1;
        TMenuItem *Textured1;
        TMenuItem *Help1;
        TMenuItem *About1;
        TPanel *MainMenuPanel;
        TSpeedButton *NewBtn;
        TSpeedButton *BtnDir;
        TBevel *Bevel1;
        TSpeedButton *OpenBtn;
        TSpeedButton *Import3DSBtn;
        TSpeedButton *ImportEAMBtn;
        TBevel *Bevel2;
        TSpeedButton *SaveBtn;
        TSpeedButton *ExportEAMBtn;
        TBevel *Bevel3;
        TSpeedButton *OptionsBtn;
        TSpeedButton *MapStatBtn;
        TBevel *Bevel5;
        TBevel *Bevel6;
        TSpeedButton *SelectBtn;
        TBevel *Bevel7;
        TSpeedButton *MoveCameraBtn;
        TSpeedButton *RotateCameraBtn;
        TSpeedButton *ScaleCameraBtn;
        TBevel *Bevel8;
        TSpeedButton *MoveObjBtn;
        TSpeedButton *RotateObjBtn;
        TSpeedButton *ScaleObjBtn;
        TBevel *Bevel9;
        TSpeedButton *AddVertexBtn;
        TSpeedButton *AddFaceBtn;
        TBevel *Bevel10;
        TSpeedButton *DeleteBtn;
        TSpeedButton *DeleteFacesBtn;
        TSaveDialog *SaveDialog2;
        TOpenDialog *OpenDialog2;
        TSaveDialog *SaveDialog1;
        TOpenDialog *OpenDialog1;
        TPanel *ManagerPnl;
        TPanel *ManagerTitlePnl;
        TPageControl *ManagerPages;
        TTabSheet *GroupSheet;
        TTabSheet *TextureSheet;
        TLabel *GroupLbl;
        TListBox *GroupList;
        TEdit *GroupName;
        TButton *CreateGroupBtn;
        TButton *RenameGroupBtn;
        TButton *SelectGroupBtn;
        TButton *DeleteGroupBtn;
        TButton *AddGeometryBtn;
        TLabel *GroupNameLbl;
        TLabel *TextureLbl;
        TListBox *TextureList;
        TEdit *TextureName;
        TButton *CreateTextureBtn;
        TLabel *TextureNameLbl;
        TButton *RenameTextureBtn;
        TButton *AssignTextureBtn;
        TButton *DeleteTextureBtn;
        TOpenDialog *OpenMaterialDlg;
        TImage *TexPrevImg;
        TLabel *TexPrevLbl;
        TMenuItem *Light1;
        TMenuItem *EditLight1;
        TMenuItem *Editfacepropeties1;
        TMenuItem *Texturecoord1;
        TMenuItem *MoveTexture1;
        TMenuItem *RotateTexture1;
        TMenuItem *ScaleTexture1;
        TMenuItem *Object2;
        TMenuItem *Editobject1;
        TMenuItem *Build1;
        TSaveDialog *BuildDialog;
        TMenuItem *ImportQuake3map1;
        TMenuItem *Sounds1;
        TMenuItem *Editsound1;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall CreateGroupBtnClick(TObject *Sender);
        void __fastcall RenameGroupBtnClick(TObject *Sender);
        void __fastcall SelectGroupBtnClick(TObject *Sender);
        void __fastcall DeleteGroupBtnClick(TObject *Sender);
        void __fastcall AddGeometryBtnClick(TObject *Sender);
        void __fastcall Select1Click(TObject *Sender);
        void __fastcall Selectall1Click(TObject *Sender);
        void __fastcall Invertselection1Click(TObject *Sender);
        void __fastcall Selectnothing1Click(TObject *Sender);
        void __fastcall Selectconnected1Click(TObject *Sender);
        void __fastcall MoveCamera1Click(TObject *Sender);
        void __fastcall RotateCamera1Click(TObject *Sender);
        void __fastcall ZoomCamera1Click(TObject *Sender);
        void __fastcall Moveobject1Click(TObject *Sender);
        void __fastcall Rotateobject1Click(TObject *Sender);
        void __fastcall Scaleobject1Click(TObject *Sender);
        void __fastcall Vertex1Click(TObject *Sender);
        void __fastcall Wireframe1Click(TObject *Sender);
        void __fastcall Exit1Click(TObject *Sender);
        void __fastcall Cascade1Click(TObject *Sender);
        void __fastcall Tile1Click(TObject *Sender);
        void __fastcall manager1Click(TObject *Sender);
        void __fastcall Options1Click(TObject *Sender);
        void __fastcall About1Click(TObject *Sender);
        void __fastcall Face1Click(TObject *Sender);
        void __fastcall Cube1Click(TObject *Sender);
        void __fastcall Cone1Click(TObject *Sender);
        void __fastcall Cylinder1Click(TObject *Sender);
        void __fastcall Sphere1Click(TObject *Sender);
        void __fastcall DeleteFaces1Click(TObject *Sender);
        void __fastcall DeleteVertexes1Click(TObject *Sender);
        void __fastcall FlipNormals1Click(TObject *Sender);
        void __fastcall New1Click(TObject *Sender);
        void __fastcall SaveAs1Click(TObject *Sender);
        void __fastcall Save1Click(TObject *Sender);
        void __fastcall Open1Click(TObject *Sender);
        void __fastcall BtnDirClick(TObject *Sender);
        void __fastcall CreateTextureBtnClick(TObject *Sender);
        void __fastcall ImportEAM1Click(TObject *Sender);
        void __fastcall ExportEAM1Click(TObject *Sender);
        void __fastcall Mapstatus1Click(TObject *Sender);
        void __fastcall RenameTextureBtnClick(TObject *Sender);
        void __fastcall DeleteTextureBtnClick(TObject *Sender);
        void __fastcall AssignTextureBtnClick(TObject *Sender);
        void __fastcall TextureListClick(TObject *Sender);
        void __fastcall Light1Click(TObject *Sender);
        void __fastcall EditLight1Click(TObject *Sender);
        void __fastcall Editfacepropeties1Click(TObject *Sender);
        void __fastcall Showall1Click(TObject *Sender);
        void __fastcall MoveTexture1Click(TObject *Sender);
        void __fastcall RotateTexture1Click(TObject *Sender);
        void __fastcall ScaleTexture1Click(TObject *Sender);
        void __fastcall Object2Click(TObject *Sender);
        void __fastcall Editobject1Click(TObject *Sender);
        void __fastcall Build1Click(TObject *Sender);
        void __fastcall TextureListMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall ImportQuake3map1Click(TObject *Sender);
        void __fastcall Sounds1Click(TObject *Sender);
        void __fastcall Editsound1Click(TObject *Sender);
private:	// User declarations
        TStatusPanel  *ToolStat;
        void  UpdateToolStat();
        void  InitPreviewImage();
        AnsiString ToolStat2Text(ENDrawClass::ENDrawClassTool ctool);
public:		// User declarations
        __fastcall TMainWnd(TComponent* Owner);

        void  SaveQuestion();
        void  UpdateGroupList();
        void  UpdateTextureList();
};
//---------------------------------------------------------------------------
extern PACKAGE TMainWnd *MainWnd;
//---------------------------------------------------------------------------
#endif
