object MainForm: TMainForm
  Left = 0
  Top = 0
  Caption = #1052#1086#1076#1091#1083#1100' '#1082#1086#1085#1090#1088#1086#1083#1103' '#1075#1088#1091#1087#1087#1099' '#1087#1088#1086#1095#1085#1086#1089#1090#1080' '#1084#1091#1092#1090' '#1053#1050#1058' "'#1041#1059#1056#1040#1053' 5009'#1052'"'
  ClientHeight = 644
  ClientWidth = 976
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsMDIForm
  Menu = MainMenu
  OldCreateOrder = False
  Position = poDefault
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnResize = FormResize
  DesignSize = (
    976
    644)
  PixelsPerInch = 96
  TextHeight = 13
  object StatusBar: TStatusBar
    Left = 0
    Top = 41
    Width = 976
    Height = 40
    Align = alTop
    Panels = <
      item
        Width = 400
      end
      item
        Width = 250
      end>
  end
  object ToolBarTop: TToolBar
    Left = 0
    Top = 0
    Width = 976
    Height = 41
    ButtonHeight = 37
    Caption = 'ToolBarTop'
    Images = ImageList1
    TabOrder = 1
    object cbTypeSize: TComboBox
      Left = 0
      Top = 0
      Width = 153
      Height = 37
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 0
      Align = alLeft
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      Text = 'cbTypeSize'
      OnSelect = cbTypeSizeSelect
    end
    object cbSGGost: TComboBox
      Left = 153
      Top = 0
      Width = 192
      Height = 37
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 0
      Align = alLeft
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      Text = 'cbSGGost'
      OnSelect = cbSGGostSelect
      Items.Strings = (
        #1043#1054#1057#1058' '#1056'53366-2009'
        #1043#1054#1057#1058' 633-80')
    end
    object bStart: TButton
      Left = 345
      Top = 0
      Width = 105
      Height = 37
      Align = alLeft
      Caption = #1055#1091#1089#1082
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = bStartClick
    end
    object bCancel: TButton
      Left = 450
      Top = 0
      Width = 105
      Height = 37
      Align = alLeft
      Caption = #1054#1090#1084#1077#1085#1072
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = bCancelClick
    end
  end
  object pThresholds: TPanel
    Left = 896
    Top = 369
    Width = 80
    Height = 2
    Align = alRight
    TabOrder = 2
    Visible = False
  end
  object BottomPanel: TPanel
    Left = 0
    Top = 628
    Width = 976
    Height = 16
    Align = alBottom
    TabOrder = 3
    object lMinScrollbar: TLabel
      Left = 1
      Top = 1
      Width = 16
      Height = 13
      Align = alLeft
      Alignment = taCenter
      Caption = 'Min'
      Transparent = True
      Visible = False
    end
    object lMaxScrollbar: TLabel
      Left = 955
      Top = 1
      Width = 20
      Height = 13
      Align = alRight
      Alignment = taCenter
      Caption = 'Max'
      Visible = False
    end
    object scbChartDragger: TScrollBar
      Left = 17
      Top = 1
      Width = 938
      Height = 14
      Align = alClient
      Max = 10000
      Min = 1
      PageSize = 0
      Position = 1
      TabOrder = 0
      Visible = False
      OnChange = scbChartDraggerChange
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 81
    Width = 976
    Height = 288
    Align = alTop
    BorderStyle = bsSingle
    Caption = 'Panel1'
    TabOrder = 5
    object SignalChart: TChart
      Left = 1
      Top = 1
      Width = 970
      Height = 285
      AllowPanning = pmNone
      BackWall.Size = 2
      BottomWall.Size = 2
      LeftWall.Color = clLime
      LeftWall.Size = 2
      Legend.CheckBoxes = True
      RightWall.Size = 2
      Title.Text.Strings = (
        'TChart')
      OnScroll = SignalChartScroll
      View3D = False
      Align = alTop
      TabOrder = 0
      PrintMargins = (
        15
        36
        15
        36)
      ColorPaletteIndex = 13
      object Series1: TFastLineSeries
        Marks.Arrow.Visible = True
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = True
        Marks.Visible = False
        LinePen.Color = 10708548
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Y'
        YValues.Order = loNone
      end
      object Series2: TFastLineSeries
        Marks.Arrow.Visible = True
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = True
        Marks.Visible = False
        LinePen.Color = 3513587
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Y'
        YValues.Order = loNone
      end
      object Series3: TFastLineSeries
        Marks.Arrow.Visible = True
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = True
        Marks.Visible = False
        LinePen.Color = 1330417
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Y'
        YValues.Order = loNone
      end
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 363
    Width = 976
    Height = 265
    Align = alBottom
    BorderStyle = bsSingle
    Caption = 'Panel2'
    TabOrder = 6
    object EtalonChart: TChart
      Left = 1
      Top = 1
      Width = 970
      Height = 259
      AllowPanning = pmNone
      BackWall.Size = 2
      BottomWall.Size = 2
      LeftWall.Color = clLime
      LeftWall.Size = 2
      Legend.CheckBoxes = True
      RightWall.Size = 2
      Title.Text.Strings = (
        'TChart')
      OnScroll = SignalChartScroll
      View3D = False
      Align = alClient
      TabOrder = 0
      PrintMargins = (
        15
        36
        15
        36)
      ColorPaletteIndex = 13
      object FastLineSeries1: TFastLineSeries
        Marks.Arrow.Visible = True
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = True
        Marks.Visible = False
        LinePen.Color = 10708548
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Y'
        YValues.Order = loNone
      end
      object FastLineSeries2: TFastLineSeries
        Marks.Arrow.Visible = True
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = True
        Marks.Visible = False
        LinePen.Color = 3513587
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Y'
        YValues.Order = loNone
      end
      object FastLineSeries3: TFastLineSeries
        Marks.Arrow.Visible = True
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = True
        Marks.Visible = False
        LinePen.Color = 1330417
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Y'
        YValues.Order = loNone
      end
    end
  end
  object GroupBoxNGr: TGroupBox
    Left = 786
    Top = 8
    Width = 169
    Height = 81
    Anchors = [akTop, akRight]
    Caption = #1053#1086#1074#1099#1081' '#1101#1090#1072#1083#1086#1085
    TabOrder = 4
    object bbtCreateEtalon: TBitBtn
      Left = 25
      Top = 48
      Width = 120
      Height = 25
      Caption = #1057#1086#1079#1076#1072#1090#1100
      DoubleBuffered = True
      ParentDoubleBuffered = False
      TabOrder = 0
      OnClick = bbtCreateEtalonClick
    end
    object cbxSG: TComboBox
      Left = 25
      Top = 17
      Width = 120
      Height = 21
      TabOrder = 1
      Text = #1042#1099#1073#1088#1072#1090#1100' '#1075#1088#1091#1087#1087#1091' '#1087#1088#1086#1095#1085#1086#1089#1090#1080
    end
  end
  object PanelSG: TPanel
    Left = 603
    Top = 8
    Width = 169
    Height = 81
    Anchors = [akTop, akRight]
    Caption = '???'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clHighlightText
    Font.Height = -32
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 7
  end
  object MainMenu: TMainMenu
    Images = ImageList1
    Left = 224
    Top = 232
    object N3: TMenuItem
      Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080
      object menuSettings: TMenuItem
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1080#1079#1084#1077#1088#1077#1085#1080#1081
        OnClick = menuSettingsClick
      end
      object menuSGSett: TMenuItem
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1043#1055
        OnClick = menuSGSettClick
      end
      object menuColorSett: TMenuItem
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1094#1074#1077#1090#1086#1074
        OnClick = menuColorSettClick
      end
      object menuTypeSize: TMenuItem
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1090#1080#1087#1086#1088#1072#1079#1084#1077#1088#1072
        Visible = False
        OnClick = menuTypeSizeClick
      end
      object menuEtalons: TMenuItem
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1101#1090#1072#1083#1086#1085#1086#1074
        OnClick = menuEtalonsClick
      end
    end
    object N6: TMenuItem
      Caption = #1044#1080#1072#1075#1085#1086#1089#1090#1080#1082#1072
      object menuProtocol: TMenuItem
        Caption = #1055#1088#1086#1090#1086#1082#1086#1083
        OnClick = MenuProtocolClick
      end
      object mnuCheckGen: TMenuItem
        Caption = #1055#1088#1086#1074#1077#1088#1082#1072' '#1075#1077#1085#1077#1088#1072#1090#1086#1088#1072
        OnClick = mnuCheckGenClick
      end
      object mnuCheck1730: TMenuItem
        Caption = #1055#1088#1086#1074#1077#1088#1082#1072' 1730'
        OnClick = mnuCheck1730Click
      end
    end
    object N9: TMenuItem
      Caption = #1056#1077#1079#1091#1083#1100#1090#1072#1090
      object menuSave: TMenuItem
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
        OnClick = menuSaveClick
      end
      object menuOpen: TMenuItem
        Caption = #1054#1090#1082#1088#1099#1090#1100
        OnClick = menuOpenClick
      end
    end
    object N10: TMenuItem
      Caption = #1055#1086#1084#1086#1097#1100
      object menuAbout: TMenuItem
        Caption = #1054' '#1087#1088#1086#1075#1088#1072#1084#1084#1077
        OnClick = menuAboutClick
      end
    end
  end
  object ImageList1: TImageList
    Left = 464
    Top = 176
    Bitmap = {
      494C010103000500FC0010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000001000000001002000000000000010
      0000000000000000000000000000000000008686860086868600868686008686
      8600868686008686860086868600868686008686860086868600FFFFFF00C0C0
      C00086868600C0C0C000FFFFFF00C0C0C0000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008686860086868600868686008686
      860086868600868686008686860086868600868686008686860086868600FFFF
      FF0086868600FFFFFF0086868600868686000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008000000080000000800000008000
      00000000000000000000868686008686860086868600FFFFFF00FFFFFF00FFFF
      FF00800000008000000080000000800000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008000
      0000FF00000080000000000000000000000086868600FFFFFF00FFFFFF00FFFF
      FF00800000000000000000000000000000000000000000000000008080000080
      8000008080000080800000808000008080000080800000808000008080000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000008080000080800000000000000000000000000000000000C0C0C0000000
      0000008080000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008000
      000080000000FF0000008000000000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00800000000000000000000000000000000000000000FFFF00000000000080
      8000008080000080800000808000008080000080800000808000008080000080
      8000000000000000000000000000000000000000000000000000000000000000
      0000008080000080800000000000000000000000000000000000C0C0C0000000
      0000008080000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008000
      0000FF00000080000000FF00000000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF008000000000000000000000000000000000000000FFFFFF0000FFFF000000
      0000008080000080800000808000008080000080800000808000008080000080
      8000008080000000000000000000000000000000000000000000000000000000
      0000008080000080800000000000000000000000000000000000000000000000
      0000008080000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008000
      000080000000FF0000008000000000000000FFFFFF00FFFF0000FFFFFF00FFFF
      0000800000000000000000000000000000000000000000FFFF00FFFFFF0000FF
      FF00000000000080800000808000008080000080800000808000008080000080
      8000008080000080800000000000000000000000000000000000000000000000
      0000008080000080800000808000008080000080800000808000008080000080
      8000008080000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008000
      0000FF00000080000000FF00000000000000FFFF0000FFFFFF00FFFF0000FFFF
      FF008000000000000000000000000000000000000000FFFFFF0000FFFF00FFFF
      FF0000FFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000008080000080800000000000000000000000000000000000000000000080
      8000008080000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008000
      000080000000FF0000008000000000000000FFFFFF00FFFF0000FFFFFF00FFFF
      0000800000000000000000000000000000000000000000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000080800000000000C0C0C000C0C0C000C0C0C000C0C0C000C0C0C0000000
      0000008080000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008000
      0000FF00000080000000FF00000000000000FFFF0000FFFFFF00FFFF0000FFFF
      FF008000000000000000000000000000000000000000FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000080800000000000C0C0C000C0C0C000C0C0C000C0C0C000C0C0C0000000
      0000008080000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008000
      0000800000008000000080000000800000008000000080000000800000008000
      0000800000000000000000000000000000000000000000FFFF00FFFFFF0000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000080800000000000C0C0C000C0C0C000C0C0C000C0C0C000C0C0C0000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000080800000000000C0C0C000C0C0C000C0C0C000C0C0C000C0C0C0000000
      0000C0C0C0000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000800000008000000080000000800000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000100000000100010000000000800000000000000000000000
      000000000000000000000000FFFFFF000000FFFFFF7E00000000FFFFBFFF0000
      0000001FF0030000E007000FE0030000E0070007E0030000E0070003E0030000
      E0070001E0030000E007000020030000E007001FE0020000E007001FE0030000
      E007001FE0030000FFFF8FF1E0030000F81FFFF9E0030000F81FFF75FFFF0000
      F81FFF8FBF7D0000FFFFFFFF7F7E000000000000000000000000000000000000
      000000000000}
  end
  object OpenDialog: TOpenDialog
    DefaultExt = '.csv'
    Filter = #1057#1080#1075#1085#1072#1083' '#1075#1088#1091#1087#1087#1099' '#1087#1088#1086#1095#1085#1086#1089#1090#1080' (*.csv)|*.csv'
    Options = [ofHideReadOnly, ofNoChangeDir, ofEnableSizing]
    Left = 680
    Top = 235
  end
  object SaveDialog: TSaveDialog
    DefaultExt = '.csv'
    Filter = #1057#1080#1075#1085#1072#1083' '#1075#1088#1091#1087#1087#1099' '#1087#1088#1086#1095#1085#1086#1089#1090#1080' (*.csv)|*.csv'
    Options = [ofHideReadOnly, ofNoChangeDir, ofEnableSizing]
    Left = 720
    Top = 219
  end
  object ApplicationEvents: TApplicationEvents
    OnMessage = ApplicationEventsMessage
    Left = 792
    Top = 232
  end
  object queryEtalon: TADOQuery
    Parameters = <>
    Left = 832
    Top = 68
  end
  object cmdEtalonVal: TADOCommand
    Connection = SqlDBModule.ADOConnectionDB
    Parameters = <
      item
        Name = 'pbarkgausen_val'
        DataType = ftFloat
        NumericScale = 8
        Precision = 8
        Value = Null
      end>
    Left = 832
    Top = 16
  end
end
