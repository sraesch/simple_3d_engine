object CameraDlg: TCameraDlg
  Left = 192
  Top = 114
  BorderStyle = bsToolWindow
  Caption = 'Camera settings'
  ClientHeight = 191
  ClientWidth = 293
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
  object Label7: TLabel
    Left = 8
    Top = 136
    Width = 16
    Height = 13
    Caption = 'Arc'
  end
  object Label8: TLabel
    Left = 136
    Top = 136
    Width = 32
    Height = 13
    Caption = 'Range'
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 8
    Width = 137
    Height = 113
    Caption = 'Position'
    TabOrder = 0
    object Label4: TLabel
      Left = 8
      Top = 24
      Width = 10
      Height = 13
      Caption = 'X:'
    end
    object Label5: TLabel
      Left = 8
      Top = 56
      Width = 10
      Height = 13
      Caption = 'Y:'
    end
    object Label6: TLabel
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
  object GroupBox1: TGroupBox
    Left = 149
    Top = 8
    Width = 137
    Height = 113
    Caption = 'Angles'
    TabOrder = 1
    object Label1: TLabel
      Left = 8
      Top = 24
      Width = 10
      Height = 13
      Caption = 'X:'
    end
    object Label2: TLabel
      Left = 8
      Top = 56
      Width = 10
      Height = 13
      Caption = 'Y:'
    end
    object Label3: TLabel
      Left = 8
      Top = 88
      Width = 10
      Height = 13
      Caption = 'Z:'
    end
    object EdtXAngle: TEdit
      Left = 24
      Top = 16
      Width = 97
      Height = 21
      TabOrder = 0
      Text = '0.0'
    end
    object EdtYAngle: TEdit
      Left = 24
      Top = 48
      Width = 97
      Height = 21
      TabOrder = 1
      Text = '0.0'
    end
    object EdtZAngle: TEdit
      Left = 24
      Top = 80
      Width = 97
      Height = 21
      TabOrder = 2
      Text = '0.0'
    end
  end
  object EdtArc: TEdit
    Left = 32
    Top = 128
    Width = 97
    Height = 21
    TabOrder = 2
    Text = '0'
  end
  object EdtRange: TEdit
    Left = 174
    Top = 128
    Width = 97
    Height = 21
    TabOrder = 3
    Text = '0'
  end
  object ApplyBtn: TButton
    Left = 8
    Top = 160
    Width = 75
    Height = 25
    Caption = '&Apply'
    TabOrder = 4
    OnClick = ApplyBtnClick
  end
  object CloseBtn: TButton
    Left = 208
    Top = 160
    Width = 75
    Height = 25
    Caption = '&Close'
    TabOrder = 5
    OnClick = CloseBtnClick
  end
end
