object SoundDlg: TSoundDlg
  Left = 192
  Top = 114
  BorderStyle = bsToolWindow
  Caption = 'Edit Sound'
  ClientHeight = 264
  ClientWidth = 416
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object SRangeLbl: TLabel
    Left = 8
    Top = 176
    Width = 61
    Height = 13
    Caption = 'Sound range'
  end
  object SSpotLbl: TLabel
    Left = 8
    Top = 216
    Width = 52
    Height = 13
    Caption = 'Spot cutoff'
  end
  object SoundsLbl: TLabel
    Left = 280
    Top = 8
    Width = 36
    Height = 13
    Caption = 'Sounds'
  end
  object VolLbl: TLabel
    Left = 120
    Top = 176
    Width = 35
    Height = 13
    Caption = 'Volume'
  end
  object SpeedLbl: TLabel
    Left = 120
    Top = 216
    Width = 31
    Height = 13
    Caption = 'Speed'
  end
  object Label1: TLabel
    Left = 8
    Top = 128
    Width = 66
    Height = 13
    Caption = 'Sound source'
  end
  object PositionBox: TGroupBox
    Left = 8
    Top = 8
    Width = 129
    Height = 113
    Caption = 'Position'
    TabOrder = 0
    object XLbl: TLabel
      Left = 8
      Top = 24
      Width = 10
      Height = 13
      Caption = 'X:'
    end
    object YLbl: TLabel
      Left = 8
      Top = 56
      Width = 10
      Height = 13
      Caption = 'Y:'
    end
    object ZLbl: TLabel
      Left = 8
      Top = 88
      Width = 10
      Height = 13
      Caption = 'Z:'
    end
    object EdtXPos: TEdit
      Left = 24
      Top = 16
      Width = 97
      Height = 21
      TabOrder = 0
      Text = '0'
    end
    object EdtYPos: TEdit
      Left = 24
      Top = 48
      Width = 97
      Height = 21
      TabOrder = 1
      Text = '0'
    end
    object EdtZPos: TEdit
      Left = 24
      Top = 80
      Width = 97
      Height = 21
      TabOrder = 2
      Text = '0'
    end
  end
  object DirectionBox: TGroupBox
    Left = 144
    Top = 8
    Width = 129
    Height = 113
    Caption = 'Direction'
    TabOrder = 1
    object AngleXLbl: TLabel
      Left = 8
      Top = 16
      Width = 37
      Height = 13
      Caption = 'Angle X'
    end
    object AngleYLbl: TLabel
      Left = 8
      Top = 56
      Width = 37
      Height = 13
      Caption = 'Angle Y'
    end
    object EdtXDir: TEdit
      Left = 24
      Top = 32
      Width = 97
      Height = 21
      TabOrder = 0
      Text = '0'
    end
    object EdtYDir: TEdit
      Left = 24
      Top = 72
      Width = 97
      Height = 21
      TabOrder = 1
      Text = '0'
    end
  end
  object EdtSpot: TEdit
    Left = 8
    Top = 232
    Width = 97
    Height = 21
    TabOrder = 2
    Text = '0'
  end
  object EdtRange: TEdit
    Left = 8
    Top = 192
    Width = 97
    Height = 21
    TabOrder = 3
    Text = '0'
  end
  object OKBtn: TButton
    Left = 328
    Top = 184
    Width = 75
    Height = 25
    Caption = '&OK'
    TabOrder = 4
    OnClick = OKBtnClick
  end
  object CloseBtn: TButton
    Left = 328
    Top = 224
    Width = 75
    Height = 25
    Caption = '&Close'
    TabOrder = 5
    OnClick = CloseBtnClick
  end
  object LoopSnd: TCheckBox
    Left = 208
    Top = 192
    Width = 97
    Height = 17
    Caption = 'Loop sound'
    TabOrder = 6
  end
  object SoundsBox: TListBox
    Left = 280
    Top = 24
    Width = 129
    Height = 97
    ItemHeight = 13
    TabOrder = 7
    OnClick = SoundsBoxClick
  end
  object EdtVol: TEdit
    Left = 120
    Top = 192
    Width = 81
    Height = 21
    TabOrder = 8
    Text = '1.0'
  end
  object EdtSpeed: TEdit
    Left = 120
    Top = 232
    Width = 81
    Height = 21
    TabOrder = 9
    Text = '1.0'
  end
  object EdtSndSrc: TEdit
    Left = 8
    Top = 144
    Width = 401
    Height = 21
    TabOrder = 10
  end
end
