//---------------------------------------------------------------------------
#ifndef MainWndUnitH
#define MainWndUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ImgList.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include "ENDrawClass.h"
//---------------------------------------------------------------------------
class TMainWnd : public TForm
{
__published:	// IDE-managed Components
        TMainMenu *MainMenu1;
        TMenuItem *Windows1;
        TMenuItem *Cascade1;
        TMenuItem *File1;
        TMenuItem *Exit1;
        TMenuItem *Tile1;
        TMenuItem *Showall1;
        TPopupMenu *PopupMenu1;
        TMenuItem *Camera1;
        TMenuItem *MoveCamera1;
        TMenuItem *RotateCamera1;
        TMenuItem *ZoomCamera1;
        TMenuItem *Add1;
        TMenuItem *Vertex1;
        TMenuItem *New1;
        TMenuItem *Selection1;
        TMenuItem *Select1;
        TMenuItem *Selectall1;
        TMenuItem *Invertselection1;
        TMenuItem *Selectnothing1;
        TMenuItem *Help1;
        TMenuItem *About1;
        TMenuItem *Face1;
        TOpenDialog *OpenDialog1;
        TSaveDialog *SaveDialog1;
        TMenuItem *Open1;
        TMenuItem *Save1;
        TMenuItem *Object1;
        TMenuItem *Moveobject1;
        TMenuItem *Rotateobject1;
        TMenuItem *Scaleobject1;
        TMenuItem *Mode1;
        TMenuItem *Wireframe1;
        TMenuItem *Solid1;
        TMenuItem *Textured1;
        TMenuItem *Options1;
        TMenuItem *SaveAs1;
        TMenuItem *N1;
        TMenuItem *N2;
        TMenuItem *N3;
        TMenuItem *Cube1;
        TStatusBar *MainWndStat;
        TMenuItem *Frame1;
        TMenuItem *Frames1;
        TMenuItem *Addframe1;
        TScrollBar *ScrollFrames;
        TMenuItem *Cone1;
        TMenuItem *Cylinder1;
        TMenuItem *Sphere1;
        TPanel *GroupPnl;
        TMenuItem *Groupmanager1;
        TListBox *GroupList;
        TPanel *GroupTitlePnl;
        TLabel *GroupLbl;
        TLabel *GroupNameLbl;
        TEdit *GroupName;
        TButton *RenameGroupBtn;
        TButton *SelectGroupBtn;
        TButton *CreateGroupBtn;
        TButton *DeleteGroupBtn;
        TButton *AddGeometryBtn;
        TMenuItem *Modelstatus1;
        TMenuItem *Edit1;
        TMenuItem *DeleteFaces1;
        TMenuItem *DeleteVertexes1;
        TMenuItem *Selectconnected1;
        TMenuItem *FlipNormals1;
        TMenuItem *Optimize1;
        TMenuItem *N4;
        TMenuItem *N5;
        TMenuItem *N6;
        TMenuItem *SkinEditor1;
        TMenuItem *Deleteframe1;
        TMenuItem *Import3DS1;
        TMenuItem *ExportEAM1;
        TSaveDialog *SaveDialog2;
        TOpenDialog *OpenDialog2;
        TMenuItem *ImportEAM1;
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
        TSpeedButton *ModelStatBtn;
        TSpeedButton *SkinEditorBtn;
        TBevel *Bevel4;
        TSpeedButton *AddFrameBtn;
        TSpeedButton *DeleteFrameBtn;
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
        void __fastcall Cascade1Click(TObject *Sender);
        void __fastcall Exit1Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall Tile1Click(TObject *Sender);
        void __fastcall Showall1Click(TObject *Sender);
        void __fastcall MoveCamera1Click(TObject *Sender);
        void __fastcall RotateCamera1Click(TObject *Sender);
        void __fastcall ZoomCamera1Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall Vertex1Click(TObject *Sender);
        void __fastcall New1Click(TObject *Sender);
        void __fastcall Selectall1Click(TObject *Sender);
        void __fastcall Invertselection1Click(TObject *Sender);
        void __fastcall Select1Click(TObject *Sender);
        void __fastcall Selectnothing1Click(TObject *Sender);
        void __fastcall About1Click(TObject *Sender);
        void __fastcall Face1Click(TObject *Sender);
        void __fastcall Save1Click(TObject *Sender);
        void __fastcall Open1Click(TObject *Sender);
        void __fastcall Moveobject1Click(TObject *Sender);
        void __fastcall Rotateobject1Click(TObject *Sender);
        void __fastcall Scaleobject1Click(TObject *Sender);
        void __fastcall OnViewMode(TObject *Sender);
        void __fastcall Options1Click(TObject *Sender);
        void __fastcall SaveAs1Click(TObject *Sender);
        void __fastcall Cube1Click(TObject *Sender);
        void __fastcall Addframe1Click(TObject *Sender);
        void __fastcall ScrollFramesChange(TObject *Sender);
        void __fastcall Cone1Click(TObject *Sender);
        void __fastcall Cylinder1Click(TObject *Sender);
        void __fastcall Sphere1Click(TObject *Sender);
        void __fastcall Groupmanager1Click(TObject *Sender);
        void __fastcall CreateGroupBtnClick(TObject *Sender);
        void __fastcall RenameGroupBtnClick(TObject *Sender);
        void __fastcall DeleteGroupBtnClick(TObject *Sender);
        void __fastcall Modelstatus1Click(TObject *Sender);
        void __fastcall SelectGroupBtnClick(TObject *Sender);
        void __fastcall AddGeometryBtnClick(TObject *Sender);
        void __fastcall DeleteFaces1Click(TObject *Sender);
        void __fastcall DeleteVertexes1Click(TObject *Sender);
        void __fastcall Selectconnected1Click(TObject *Sender);
        void __fastcall FlipNormals1Click(TObject *Sender);
        void __fastcall Optimize1Click(TObject *Sender);
        void __fastcall SkinEditor1Click(TObject *Sender);
        void __fastcall Deleteframe1Click(TObject *Sender);
        void __fastcall Import3DS1Click(TObject *Sender);
        void __fastcall ExportEAM1Click(TObject *Sender);
        void __fastcall ImportEAM1Click(TObject *Sender);
        void __fastcall BtnDirClick(TObject *Sender);
private:	// User declarations
        void  SaveQuestion();
        void  UpdateToolStat();
        void  UpdateFrameStat();
        void  UpdateScrollFrames();
        void  UpdateGroupList();
        AnsiString ToolStat2Text(ENDrawClass::ENDrawClassTool ctool);
        TStatusPanel  *ToolStat,*FrameStat,*VertexStat;
public:		// User declarations
        __fastcall TMainWnd(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainWnd *MainWnd;
//---------------------------------------------------------------------------
#endif
