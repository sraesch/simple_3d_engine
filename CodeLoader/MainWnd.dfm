object MainDlg: TMainDlg
  Left = 192
  Top = 114
  BorderStyle = bsDialog
  Caption = 'Code Loader'
  ClientHeight = 519
  ClientWidth = 789
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
  object CodeField: TMemo
    Left = 0
    Top = 29
    Width = 789
    Height = 490
    Align = alClient
    ReadOnly = True
    ScrollBars = ssBoth
    TabOrder = 0
  end
  object ToolBar1: TToolBar
    Left = 0
    Top = 0
    Width = 789
    Height = 29
    Caption = 'ToolBar1'
    TabOrder = 1
    object LoadBtn: TButton
      Left = 0
      Top = 2
      Width = 75
      Height = 22
      Caption = '&Load code'
      TabOrder = 0
      OnClick = LoadBtnClick
    end
  end
  object OpenDialog1: TOpenDialog
    Filter = 'ENGINECODE|ENGINECODE'
    Left = 120
    Top = 8
  end
end
