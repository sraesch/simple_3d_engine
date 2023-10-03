object MainWnd: TMainWnd
  Left = 192
  Top = 114
  Width = 696
  Height = 480
  Caption = 'Main Window'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PopupMenu = PopupMenu1
  OnCreate = FormCreate
  OnDblClick = FormDblClick
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnPaint = FormPaint
  PixelsPerInch = 96
  TextHeight = 13
  object ProgressBar1: TProgressBar
    Left = 0
    Top = 432
    Width = 688
    Height = 14
    Align = alBottom
    Min = 0
    Max = 100
    Smooth = True
    TabOrder = 0
  end
  object Memo1: TMemo
    Left = 224
    Top = 80
    Width = 273
    Height = 161
    Lines.Strings = (
      'Memo1')
    ScrollBars = ssBoth
    TabOrder = 1
    OnDblClick = Memo1DblClick
    OnKeyDown = FormKeyDown
  end
  object OpenDialog1: TOpenDialog
    Filter = 'Engine ASCII mesh(*.eam)|*.eam'
    Left = 8
    Top = 8
  end
  object PopupMenu1: TPopupMenu
    Left = 40
    Top = 8
    object Loaddata1: TMenuItem
      Caption = '&Load data'
      OnClick = Button1Click
    end
    object Compute1: TMenuItem
      Caption = '&Compute'
      OnClick = Button2Click
    end
    object Loadbsp1: TMenuItem
      Caption = '&Load bsp'
      OnClick = Loadbsp1Click
    end
  end
  object Timer1: TTimer
    Interval = 100
    OnTimer = Timer1Timer
    Left = 120
    Top = 104
  end
  object SaveDialog1: TSaveDialog
    Left = 72
    Top = 8
  end
  object OpenDialog2: TOpenDialog
    Left = 104
    Top = 8
  end
end
