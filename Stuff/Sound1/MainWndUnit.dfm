object MainWnd: TMainWnd
  Left = 192
  Top = 114
  Width = 696
  Height = 480
  Caption = 'MainWnd'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 72
    Width = 45
    Height = 13
    Caption = 'Distance:'
  end
  object Label2: TLabel
    Left = 8
    Top = 136
    Width = 45
    Height = 13
    Caption = 'Distance:'
  end
  object DistLbl: TLabel
    Left = 56
    Top = 136
    Width = 6
    Height = 13
    Caption = '0'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Button2: TButton
    Left = 8
    Top = 8
    Width = 75
    Height = 25
    Caption = '&Play Sound'
    TabOrder = 0
    OnClick = Button2Click
  end
  object Button1: TButton
    Left = 88
    Top = 8
    Width = 75
    Height = 25
    Caption = '&Stop Sound'
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button3: TButton
    Left = 8
    Top = 40
    Width = 75
    Height = 25
    Caption = '&Pause Sound'
    TabOrder = 2
    OnClick = Button3Click
  end
  object CheckBox1: TCheckBox
    Left = 88
    Top = 48
    Width = 97
    Height = 17
    Caption = 'Loop sound'
    TabOrder = 3
    OnClick = CheckBox1Click
  end
  object TrackBar1: TTrackBar
    Left = 8
    Top = 88
    Width = 673
    Height = 45
    Max = 100
    Orientation = trHorizontal
    Frequency = 1
    Position = 0
    SelEnd = 0
    SelStart = 0
    TabOrder = 4
    TickMarks = tmBottomRight
    TickStyle = tsAuto
    OnChange = TrackBar1Change
  end
end
