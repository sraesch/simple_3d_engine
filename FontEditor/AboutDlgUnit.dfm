object AboutDlg: TAboutDlg
  Left = 384
  Top = 163
  BorderStyle = bsDialog
  Caption = 'About Engine Font Editor'
  ClientHeight = 160
  ClientWidth = 298
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
  object Label1: TLabel
    Left = 0
    Top = 8
    Width = 297
    Height = 20
    Alignment = taCenter
    AutoSize = False
    Caption = 'Fonteditor V1.0a'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label2: TLabel
    Left = 0
    Top = 64
    Width = 297
    Height = 13
    Alignment = taCenter
    AutoSize = False
    Caption = 'Written by Scratchy'
  end
  object Label3: TLabel
    Left = 0
    Top = 80
    Width = 297
    Height = 13
    Alignment = taCenter
    AutoSize = False
    Caption = 'E-Mail: ScratchyItsch@hotmail.com'
  end
  object Button1: TButton
    Left = 112
    Top = 128
    Width = 75
    Height = 25
    Caption = '&OK'
    TabOrder = 0
    OnClick = Button1Click
  end
end
