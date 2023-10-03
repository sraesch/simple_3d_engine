object MainWnd: TMainWnd
  Left = 192
  Top = 114
  Width = 696
  Height = 480
  Caption = 'XMLLoader'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Memo1: TMemo
    Left = 80
    Top = 56
    Width = 185
    Height = 89
    Lines.Strings = (
      'Memo1')
    TabOrder = 0
  end
  object MainMenu1: TMainMenu
    Left = 152
    Top = 16
    object File1: TMenuItem
      Caption = '&File'
      object Loadxml1: TMenuItem
        Caption = '&Load xml'
        OnClick = Loadxml1Click
      end
    end
  end
  object OpenDialog1: TOpenDialog
    Filter = 'xml quake3 maps(*.xmap)|*.xmap'
    Left = 184
    Top = 16
  end
end
