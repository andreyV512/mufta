object fmViewMeasure: TfmViewMeasure
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = 'fmViewMeasure'
  ClientHeight = 623
  ClientWidth = 1270
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyPress = FormKeyPress
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object ChartView: TChart
    Left = 0
    Top = 0
    Width = 1270
    Height = 623
    BackWall.Transparency = 30
    BackWall.Transparent = False
    Legend.FontSeriesColor = True
    MarginBottom = 16
    MarginLeft = 0
    MarginRight = 2
    MarginTop = 7
    MarginUnits = muPixels
    Title.Text.Strings = (
      'TChart')
    Title.Visible = False
    BottomAxis.LabelStyle = talValue
    LeftAxis.Automatic = False
    LeftAxis.AutomaticMaximum = False
    LeftAxis.AutomaticMinimum = False
    LeftAxis.Maximum = 100.000000000000000000
    View3D = False
    Align = alClient
    Color = clCream
    PopupMenu = popupMenuChartZone
    TabOrder = 0
    OnMouseMove = ChartViewMouseMove
    ColorPaletteIndex = 13
    object SBMedian: TSpeedButton
      Left = 60
      Top = 8
      Width = 17
      Height = 17
      Hint = #1052#1077#1076#1080#1072#1085#1085#1072#1103' '#1092#1080#1083#1100#1090#1088#1072#1094#1080#1103
      AllowAllUp = True
      GroupIndex = 2
      Caption = #1052
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      Visible = False
    end
    object SBFilterIn: TSpeedButton
      Left = 106
      Top = 8
      Width = 17
      Height = 17
      Hint = #1063#1072#1089#1090#1086#1090#1085#1072#1103' '#1092#1080#1083#1100#1090#1088#1072#1094#1080#1103' '#1074#1085#1091#1090#1088#1077#1085#1085#1103#1103' '
      AllowAllUp = True
      GroupIndex = 4
      Caption = #1092
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      Visible = False
    end
    object SBFilterOut: TSpeedButton
      Left = 83
      Top = 8
      Width = 17
      Height = 17
      Hint = #1063#1072#1089#1090#1086#1090#1085#1072#1103' '#1092#1080#1083#1100#1090#1088#1072#1094#1080#1103' '#1074#1085#1077#1096#1085#1103#1103
      AllowAllUp = True
      GroupIndex = 3
      Caption = #1060
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      Visible = False
    end
    object SBSource: TSpeedButton
      Left = 37
      Top = 8
      Width = 17
      Height = 17
      Hint = #1048#1089#1093#1086#1076#1085#1099#1077' '#1076#1072#1085#1085#1099#1077
      AllowAllUp = True
      GroupIndex = 1
      Caption = #1048
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      Visible = False
    end
    object StatusBar: TStatusBar
      Left = 1
      Top = 603
      Width = 1268
      Height = 19
      Panels = <
        item
          Text = #1056#1072#1089#1089#1090#1086#1103#1085#1080#1077':'
          Width = 50
        end>
    end
    object Panel1: TPanel
      Left = 1046
      Top = 8
      Width = 197
      Height = 33
      TabOrder = 1
      object cbEtalonTubeDefects: TComboBox
        Left = 7
        Top = 7
        Width = 184
        Height = 21
        Style = csDropDownList
        TabOrder = 0
        Visible = False
        OnKeyPress = FormKeyPress
        OnKeyUp = cbEtalonTubeDefectsKeyUp
      end
    end
    object Series1: TFastLineSeries
      Marks.Arrow.Visible = True
      Marks.Callout.Brush.Color = clBlack
      Marks.Callout.Arrow.Visible = True
      Marks.Visible = False
      SeriesColor = clRed
      LinePen.Color = clRed
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
  end
  object popupMenuChartZone: TPopupMenu
    Left = 608
    Top = 120
    object ZoomUp: TMenuItem
      Caption = #1059#1074#1077#1083#1080#1095#1080#1090#1100' '#1085#1072' 1,5'
      OnClick = ZoomUpClick
    end
    object ZoomDown: TMenuItem
      Caption = #1059#1084#1077#1085#1100#1096#1080#1090#1100' '#1085#1072' 1,5'
      OnClick = ZoomDownClick
    end
    object mnuZoomReset: TMenuItem
      Caption = #1057#1073#1088#1086#1089#1080#1090#1100' '#1091#1074#1077#1083#1080#1095#1077#1085#1080#1077
      OnClick = mnuZoomResetClick
    end
  end
end
