object ImportQuake3: TImportQuake3
  Left = 282
  Top = 189
  BorderStyle = bsDialog
  Caption = 'ImportQuake3'
  ClientHeight = 176
  ClientWidth = 429
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  PixelsPerInch = 96
  TextHeight = 13
  object FileNameLbl: TLabel
    Left = 8
    Top = 8
    Width = 44
    Height = 13
    Caption = 'FileName'
  end
  object Label1: TLabel
    Left = 8
    Top = 48
    Width = 42
    Height = 13
    Caption = 'Directory'
  end
  object MeshChb: TCheckBox
    Left = 120
    Top = 120
    Width = 105
    Height = 17
    Caption = 'Import mesh'
    Checked = True
    State = cbChecked
    TabOrder = 0
  end
  object PolyChb: TCheckBox
    Left = 120
    Top = 96
    Width = 105
    Height = 17
    Caption = 'Import polygones'
    Checked = True
    State = cbChecked
    TabOrder = 1
  end
  object PatchChb: TCheckBox
    Left = 8
    Top = 96
    Width = 105
    Height = 17
    Caption = 'Import patches'
    Checked = True
    State = cbChecked
    TabOrder = 2
  end
  object BillChb: TCheckBox
    Left = 8
    Top = 120
    Width = 105
    Height = 17
    Caption = 'Import billboards'
    Checked = True
    State = cbChecked
    TabOrder = 3
  end
  object TexChb: TCheckBox
    Left = 224
    Top = 96
    Width = 105
    Height = 17
    Caption = 'Import textures'
    Checked = True
    State = cbChecked
    TabOrder = 4
  end
  object FileNameEdt: TEdit
    Left = 8
    Top = 24
    Width = 329
    Height = 21
    TabOrder = 5
  end
  object SearchBtn: TButton
    Left = 344
    Top = 24
    Width = 75
    Height = 25
    Caption = '&Search'
    TabOrder = 6
    OnClick = SearchBtnClick
  end
  object ApplyBtn: TButton
    Left = 8
    Top = 144
    Width = 75
    Height = 25
    Caption = '&Apply'
    TabOrder = 7
    OnClick = ApplyBtnClick
  end
  object CloseBtn: TButton
    Left = 344
    Top = 144
    Width = 75
    Height = 25
    Caption = '&Close'
    TabOrder = 8
    OnClick = CloseBtnClick
  end
  object DirEdt: TEdit
    Left = 8
    Top = 64
    Width = 329
    Height = 21
    TabOrder = 9
  end
  object OpenDialog1: TOpenDialog
    Filter = 'Quake 3 bsp map(*.bsp)|*.bsp'
    Left = 112
    Top = 65528
  end
end
