object FSignalsState: TFSignalsState
  Left = 0
  Top = 0
  Align = alRight
  Anchors = [akTop, akRight]
  BorderStyle = bsSizeToolWin
  Caption = #1057#1080#1075#1085#1072#1083#1099
  ClientHeight = 427
  ClientWidth = 254
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Visible = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnHide = FormHide
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object InOutTimer: TTimer
    Enabled = False
    Interval = 200
    OnTimer = InOutTimerTimer
    Left = 24
    Top = 24
  end
end
