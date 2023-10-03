object SkyDlg: TSkyDlg
  Left = 192
  Top = 114
  AutoSize = True
  BorderStyle = bsDialog
  Caption = 'Sky settings'
  ClientHeight = 257
  ClientWidth = 321
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object SettingsControl: TPageControl
    Left = 0
    Top = 0
    Width = 321
    Height = 257
    ActivePage = SunSheet
    TabIndex = 0
    TabOrder = 0
    object SunSheet: TTabSheet
      Caption = 'Sun'
      OnShow = SunSheetShow
      object SunColorLbl: TLabel
        Left = 48
        Top = 16
        Width = 46
        Height = 13
        Caption = 'Sun Color'
      end
      object ApplySunBtn: TButton
        Left = 0
        Top = 200
        Width = 75
        Height = 25
        Caption = '&Apply'
        TabOrder = 0
        OnClick = ApplySunBtnClick
      end
      object SunChb: TCheckBox
        Left = 8
        Top = 152
        Width = 81
        Height = 17
        Caption = 'Enable Sun'
        TabOrder = 1
      end
      object ResetSunBtn: TButton
        Left = 232
        Top = 200
        Width = 75
        Height = 25
        Caption = '&Reset'
        TabOrder = 2
        OnClick = ResetSunBtnClick
      end
      object DirectionBox: TGroupBox
        Left = 168
        Top = 16
        Width = 129
        Height = 113
        Caption = 'Direction'
        TabOrder = 3
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
          Left = 8
          Top = 32
          Width = 97
          Height = 21
          TabOrder = 0
          Text = '0'
        end
        object EdtYDir: TEdit
          Left = 8
          Top = 72
          Width = 97
          Height = 21
          TabOrder = 1
          Text = '0'
        end
      end
      object SunColorPnl: TPanel
        Left = 8
        Top = 40
        Width = 137
        Height = 81
        BevelInner = bvLowered
        BevelOuter = bvLowered
        TabOrder = 4
        OnClick = SunColorPnlClick
      end
    end
    object SkySheet: TTabSheet
      Caption = 'Sky box'
      ImageIndex = 1
      object SideLbl: TLabel
        Left = 8
        Top = 8
        Width = 21
        Height = 13
        Caption = 'Side'
      end
      object PreviewLbl: TLabel
        Left = 136
        Top = 8
        Width = 38
        Height = 13
        Caption = 'Preview'
      end
      object PreviewImage: TImage
        Left = 136
        Top = 24
        Width = 169
        Height = 169
        Stretch = True
      end
      object SkyBoxList: TListBox
        Left = 8
        Top = 24
        Width = 121
        Height = 169
        ItemHeight = 13
        Items.Strings = (
          'FRONT'
          'BACK'
          'LEFT'
          'RIGHT'
          'TOP'
          'BOTTOM')
        TabOrder = 0
        OnClick = SkyBoxListClick
      end
      object ClearBtn: TButton
        Left = 8
        Top = 200
        Width = 75
        Height = 25
        Caption = '&Clear current'
        TabOrder = 1
        OnClick = ClearBtnClick
      end
      object SetBtn: TButton
        Left = 88
        Top = 200
        Width = 75
        Height = 25
        Caption = '&Set current'
        TabOrder = 2
        OnClick = SetBtnClick
      end
    end
  end
  object OpenDialog1: TOpenDialog
    Filter = 
      'Windows Bitmap(*.bmp)|*.bmp|Portable Network Graphic(*.png)|*.pn' +
      'g|Zsoft Paintbrush(*.pcx)|*.pcx|Jpeg(*.jpg)|*.jpg|Engine Picture' +
      '(*.enp)|*.enp|All files(*.*)|*.*'
    Left = 112
  end
  object ColorDialog1: TColorDialog
    Ctl3D = True
    Left = 144
  end
end
