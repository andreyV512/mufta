// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "unFmViewMeasure.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmViewMeasure *fmViewMeasure;

// ---------------------------------------------------------------------------
__fastcall TfmViewMeasure::TfmViewMeasure(TComponent* Owner,
	TMeasuresData* _measuresData, TGlobalSettings* _programSettings,
	vector<int> _ThresVal, vector<double> _BarkVal): TForm(Owner)
{
	// title = _title;
	pProgramSettings = _programSettings;
	pMeasuresData = _measuresData;
	ThresVal = _ThresVal;
	BarkVal = _BarkVal;
	KeyPreview = true;
	// colorsSeries[]={ clMaroon, clGreen, clOlive, clNavy, clPurple, clTeal, clSilver, clRed, clLime, clYellow, clBlue, clAqua, clFuchsia, clCream,clWhite, clBlack};
}

// ---------------------------------------------------------------------------
// закроем форму по Esc
void __fastcall TfmViewMeasure::FormKeyPress(TObject *Sender, wchar_t &Key) {
	if (Key == VK_ESCAPE)
		Close();
}

// ---------------------------------------------------------------------------
void TfmViewMeasure::SetInner(bool _v) {
	if (_v)
		ChartView->LeftAxis->Minimum = -100;
	else
		ChartView->LeftAxis->Minimum = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TfmViewMeasure::FormDestroy(TObject *Sender) {
	// Params("Defect.Wins").SaveFormPos(this);
}

// ---------------------------------------------------------------------------
void TfmViewMeasure::SetCaption(AnsiString _cap) {
	Caption = title + _cap;
}

void TfmViewMeasure::SetMetric(double _start, double _zone_size) {
	start = _start;
	zone_size = _zone_size;
	if (start < 0)
		start = 0;
	if (zone_size < 0)
		zone_size = 0;
}

void TfmViewMeasure::Clear(void) {
	for (int i = 0; i < ChartView->SeriesCount(); i++)
		ChartView->Series[i]->Clear();
};

// ---------------------------------------------------------------------------
void TfmViewMeasure::SetBorder(TLineSeries* _series, double _level,
	TColor _color) {
	_series->Clear();
	_series->AddXY(0, _level, "", _color);
}

void TfmViewMeasure::SetCurrPoint(int _curr_zone, int _curr_sens,
	bool _IsLinear) {
	curr_zone = _curr_zone;
	curr_sens = _curr_sens;
	IsLinear = _IsLinear;
}
// ---------------------------------------------------------------------------

void TfmViewMeasure::ViewEtalonCheck() {
	//
}

// ---------------------------------------------------------------------------
int TfmViewMeasure::CalcMeasFromRange(int _x) {
	//
}

// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------

void __fastcall TfmViewMeasure::cbEtalonTubeDefectsKeyUp(TObject *Sender,
	WORD &Key, TShiftState Shift) {
	if (Key != VK_ESCAPE) {
		cbEtalonTubeDefects->ItemIndex = curr_defect;
		KeyUpFlag = true;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TfmViewMeasure::ChartViewMouseMove(TObject *Sender,
	TShiftState Shift, int X, int Y) {
	//
}

// ---------------------------------------------------------------------------
int __fastcall TfmViewMeasure::PrepareChartToShow(TChart *_chart,
	TMeasuresData* _measuresData, int _countLogCh, int _maxY, int _maxX,
	AnsiString _title) {
	int err = -1;
	// TZoneData selZone=TZoneData
	_chart->SeriesList->Clear();
	try {
		if (_measuresData == NULL) {
			MessageDlg("Данные отсутствуют!", mtError,
				TMsgDlgButtons() << mbOK, NULL);
			// ModalResult=mrCancel;
			err = -2;
			return err;
		}
		else {
			if (_measuresData->vecSensorsData.size() == 0) {
				MessageDlg("Данные отсутствуют!", mtError,
					TMsgDlgButtons() << mbOK, NULL);
				// ModalResult=mrCancel;
				err = -3;
				return err;
			}
			else {
				//
			}
		}

		Caption = _title;
		AnsiString msg = "";
		StatusBar->Panels->Items[0]->Text = msg;
		offsetView = 5;
		err = 0;

		// настраиваем чарт
		// вырубаем 3Д
		_chart->View3D = False;
		_chart->View3DWalls = False;

		if(_maxY!=0)
		{
			// вырубаем автоматику черта
			_chart->BottomAxis->Automatic = false;
			_chart->BottomAxis->AutomaticMaximum = false;
			_chart->BottomAxis->AutomaticMinimum = false;
			_chart->LeftAxis->Automatic = false;
			_chart->LeftAxis->AutomaticMaximum = false;
			_chart->LeftAxis->AutomaticMinimum = false;
			_chart->RightAxis->Automatic = false;
			_chart->RightAxis->AutomaticMaximum = false;
			_chart->RightAxis->AutomaticMinimum = false;
			// число точек   по вертикали, оси
			_chart->LeftAxis->Minimum = -_maxY;
			savedLeftAxisMinimum = _chart->LeftAxis->Minimum;
			_chart->LeftAxis->Maximum = _maxY;
			savedLeftAxisMaximum = _chart->LeftAxis->Maximum;
			_chart->LeftAxis->StartPosition = 1;
			_chart->RightAxis->Minimum = -_maxY;
			_chart->RightAxis->Maximum = _maxY;
			_chart->RightAxis->StartPosition = 1;
		}
		else
		{
			//врубаем автоматику
			_chart->BottomAxis->Automatic = true;
			_chart->BottomAxis->AutomaticMaximum = true;
			_chart->BottomAxis->AutomaticMinimum = true;
			_chart->LeftAxis->Automatic = true;
			_chart->LeftAxis->AutomaticMaximum = true;
			_chart->LeftAxis->AutomaticMinimum = true;
			_chart->RightAxis->Automatic = true;
			_chart->RightAxis->AutomaticMaximum = true;
			_chart->RightAxis->AutomaticMinimum = true;
		}
		int szData = _measuresData->vecSensorsData[0].size();
		// сделаем просмотр чуть покороче
		_chart->BottomAxis->Minimum = 0;
		_chart->BottomAxis->MinimumOffset = 0; // смещение в начале
		_chart->BottomAxis->Maximum = szData;
		_chart->SeriesList->Clear();
		// int chCount = vzZoneData->countLogCh;
		int chCount = _countLogCh;
		for (int i = 0; i < (chCount + 1); i++) {
			TFastLineSeries* series = new TFastLineSeries(_chart);
			series->AutoRepaint = false;
			series->Depth = 4;
			series->ColorEachPoint = true;
			series->Marks->Visible = false;
			// скругленный
			// series->DrawStyle = dsCurve;
			// толщина линии
			series->LinePen->Width = 2;
			series->ShowInLegend = true;
			series->Title = "Ch " + IntToStr(i);
			// series->Dark3D = false;
			// TColor colors[] = {clWhite, clBlack, clMaroon, clGreen, clOlive, clNavy, clPurple, clTeal, clSilver, clRed, clLime, clYellow, clBlue, clAqua, clFuchsia, clCream};
			series->Color = clBlack;
			series->Visible = false;
			// толщина линии
			series->LinePen->Width = 1;
			_chart->AddSeries(series);
		}
		int serCount = _chart->SeriesCount();
		// а массив для чарта чуть побольше
		 double* valueY1 = new double[szData];
		 double* valueY2 = new double[szData];
		 double* valueY3 = new double[szData];

		double* valueY0 = new double[szData];
			for (int i = 0; i < (szData); i++) {
				// valueY0[i] = 0;
				valueY1[i] = _measuresData->vecSensorsData[0][i];
				valueY2[i] = _measuresData->vecSensorsData[1][i];
				valueY3[i] = _measuresData->vecSensorsData[2][i];
			}

		for (int i = 0; i < chCount; i++) {
			_chart->Series[i]->Pen->Width = 1;
			_chart->Series[i]->Visible = true;
		}
		_chart->Series[0]->AddArray(valueY1,szData-1);
		_chart->Series[1]->AddArray(valueY2,szData-1);
		_chart->Series[2]->AddArray(valueY3,szData-1);
		_chart->Series[0]->Color = clBlue;
		_chart->Series[1]->Color = clRed;
		_chart->Series[2]->Color = clGreen;
	// нарисуем кривую контрольных точек
//		for (int i = 0; i < (szData); i++) {
//			valueY0[i] = 0;
//		}
		_chart->Series[chCount]->Color = clBlack;
//		_chart->Series[chCount]->AddArray(valueY0,szData-1);
		double PeriodProc = szData / 300.0;
        int res = 0;
		for (int i = 0; i < ThresVal.size(); i++) {
			res = (int)(ThresVal[i]*PeriodProc);
			_chart->Series[chCount]->AddXY(res,BarkVal[i], "", clBlack);
		}
//		_chart->Series[chCount]->Style=
		_chart->Series[chCount]->Pen->Width = 3;
		_chart->Series[chCount]->Visible = true;
		delete valueY0;
		delete valueY1;
		delete valueY2;
		delete valueY3;
		err = 0;
	}
	catch (Exception *ex) {
		err = -2;
		TLog::ErrFullSaveLog(ex);
		// MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);

	}
	return err;

}

int __fastcall TfmViewMeasure::PrepareSensor(int _selZone, int _selSensor) {
	int err = -1;
	// TZoneData selZone=TZoneData
	try {
		// -
		err = 0;
	}
	catch (Exception *ex) {
		err = -2;
		TLog::ErrFullSaveLog(ex);
		// MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);

	}
	return err;
}

void __fastcall TfmViewMeasure::FormShow(TObject * Sender) {
	int err = -1;
	try {

	}
	catch (Exception *ex) {
		err = -2;
		TLog::ErrFullSaveLog(ex);
		// MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TfmViewMeasure::ZoomUpClick(TObject *Sender) {
	ChartView->LeftAxis->Maximum = (int)ChartView->LeftAxis->Maximum / 1.5;
	ChartView->LeftAxis->Minimum = (int)ChartView->LeftAxis->Minimum / 1.5;
}
// ---------------------------------------------------------------------------

void __fastcall TfmViewMeasure::mnuZoomResetClick(TObject *Sender) {
	ChartView->LeftAxis->Maximum = savedLeftAxisMaximum;
	ChartView->LeftAxis->Minimum = savedLeftAxisMinimum;
}
// ---------------------------------------------------------------------------

void __fastcall TfmViewMeasure::ZoomDownClick(TObject *Sender) {
	ChartView->LeftAxis->Maximum = (int)ChartView->LeftAxis->Maximum * 1.5;
	ChartView->LeftAxis->Minimum = (int)ChartView->LeftAxis->Minimum * 1.5;
}
// ---------------------------------------------------------------------------

void __fastcall TfmViewMeasure::FormCreate(TObject *Sender) {
	PrepareChartToShow(ChartView, pMeasuresData,pMeasuresData->vecSensorsData.size(), 0, 0,
		"Эталон скачков");
	// TChart *_chart, TMeasuresData* _measuresData, int _countLogCh, int _maxY, int _maxX, AnsiString _title
}
// ---------------------------------------------------------------------------
