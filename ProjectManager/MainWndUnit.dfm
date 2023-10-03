object MainWnd: TMainWnd
  Left = 192
  Top = 114
  Width = 696
  Height = 123
  Align = alTop
  Caption = 'Project Manager'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object MainMenu1: TMainMenu
    Left = 168
    Top = 40
    object File1: TMenuItem
      Caption = '&File'
      object N1: TMenuItem
        Caption = '-'
      end
      object Exit1: TMenuItem
        Caption = 'E&xit'
        OnClick = Exit1Click
      end
    end
    object Project1: TMenuItem
      Caption = '&Project'
      object Addfile1: TMenuItem
        Caption = '&Add file'
        OnClick = Addfile1Click
      end
    end
  end
  object AddFileDlg: TOpenDialog
    Filter = 'All files(*.*)|*.*'
    Options = [ofHideReadOnly, ofAllowMultiSelect, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Left = 8
    Top = 8
  end
end
