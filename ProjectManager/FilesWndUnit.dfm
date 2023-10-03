object FilesWnd: TFilesWnd
  Left = 192
  Top = 114
  Width = 239
  Height = 480
  Align = alLeft
  BorderStyle = bsSizeToolWin
  Caption = 'Files'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object FilePages: TPageControl
    Left = 0
    Top = 0
    Width = 231
    Height = 446
    ActivePage = MapPage
    Align = alClient
    TabIndex = 1
    TabOrder = 0
    object ScriptPage: TTabSheet
      Caption = 'scripts'
      object ScriptBox: TListBox
        Left = 0
        Top = 0
        Width = 223
        Height = 418
        Align = alClient
        ItemHeight = 13
        TabOrder = 0
      end
    end
    object MapPage: TTabSheet
      Caption = 'maps'
      ImageIndex = 1
      object MapBox: TListBox
        Left = 0
        Top = 0
        Width = 223
        Height = 418
        Align = alClient
        ItemHeight = 13
        TabOrder = 0
      end
    end
    object PackPage: TTabSheet
      Caption = 'packages'
      ImageIndex = 2
      object PackBox: TListBox
        Left = 0
        Top = 0
        Width = 223
        Height = 418
        Align = alClient
        ItemHeight = 13
        TabOrder = 0
      end
    end
    object UnknownPage: TTabSheet
      Caption = 'unknown'
      ImageIndex = 3
      object UnknownBox: TListBox
        Left = 0
        Top = 0
        Width = 223
        Height = 418
        Align = alClient
        ItemHeight = 13
        TabOrder = 0
      end
    end
  end
end
