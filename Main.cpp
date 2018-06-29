// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "TProtocol.h"
#include "ColorSettings.h"
#include "Filters.h"
// ---------------------------------------------------------------------------
#include "unFmMessage.h"
#include "ABOUT_NTC_NK_URAN.h"
#include "unTExtFunction.h"
#include "unDiagnost.h"
#include "unFmEditSettings.h"
#include "uFSignalsState.h"
#include "SignalListDef.h"
#include "ADCSettings.h"
#include "SGSettings.h"
#include "EtalonDatas.h"
#include "DebugMess.h"
// -----------------------------------
#ifdef BERLIN_10
//
#else
#pragma package(smart_init)
#endif

#pragma resource "*.dfm"
TMainForm *MainForm;
#ifndef TVIRTLCARD791
#define DEBUG_OK(n) n
#else
 #define DEBUG_OK(n) true
#endif

#ifndef TVIRTLCARD791
 #define DEBUG_LEN(txt, o)
 #else
  #define DEBUG_LEN(txt, o)\
  dprint("DEBUG_LEN %s %d\n", txt, o->vecMeasuresData[0].vecSensorsData[0].size())
 #endif
// ---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner) : TForm(Owner) {

}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender) {
	dprint("Ok debug panel\n");
	AnsiString applPath = ExtractFilePath(Application->ExeName);
	mainGlobalSettings.applPath = applPath;
	applPath += "connectSQL.udl";
	mainGlobalSettings.LoadPRGSettings(applPath);
	mainGlobalSettings.threadMsg = RegisterWindowMessage(L"Thread");
	mainGlobalSettings.mainHandle = this->Handle;
	if (mainGlobalSettings.threadMsg == 0) {
		Application->MessageBoxW
			(L"TMainForm::FormCreate: �� ���� ���������������� ���������",
			L"������", MB_ICONERROR | MB_OK);
	}
	else {
		//
	}
	l�ard791 = new TLCard791(&mainGlobalSettings, codeERR);
	/*
	if (codeERR < 0) {
		Application->Terminate();
	}
	else {
		//
	}
	*/
	TSFreqs = new TSFrequencies(mainGlobalSettings.indexCurrentTypeSize);
	int freqCount = TSFreqs->Frequency.size();
	lCardData = new TLCardData(l�ard791, freqCount, l�ard791->countLogCh); // todo �������� 1 �������

	SLD = new SignalListDef();
	SGFilter = new Filters("SG");
	SGFilter->setSettingsFromDB();

	//����� �����������
	SqlDBModule->FillComboBox("TypeSizes", "TSName", cbTypeSize);
	for (int i = 0; i < cbTypeSize->Items->Count; i++)
	{
		if ((int)cbTypeSize->Items->Objects[i]==mainGlobalSettings.indexCurrentTypeSize)
		{
			cbTypeSize->ItemIndex=i;
			break;
		}
		else
		{
			cbTypeSize->ItemIndex=-1;
		}
	}
	//����� �����
	cbSGGost->ItemIndex = mainGlobalSettings.indexCurrentSGGost;
	
	TExtFunction::PrepareChartToTst(SignalChart, 3, 600, 2000);
	SignalChart->Series[0]->Title += " ����������";
	SignalChart->Series[1]->Title += " ����������";
	SignalChart->Series[2]->Title += " ���";
//	TExtFunction::PrepareFastLineChart(4, 2500, SignalChart);
//	SignalChart->LeftAxis->Automatic = false;
//	SignalChart->LeftAxis->Maximum = 110.0;
//	SignalChart->LeftAxis->Minimum = -110.0;
//	SignalChart->RightAxis->Automatic = false;
//	SignalChart->RightAxis->Maximum = 110.0;
//	SignalChart->RightAxis->Minimum = -110.0;
	ChangeColor();
	TExtFunction::PrepareChartToTst(EtalonChart, 3, 10, 2000);
	// �������� ������ ��
	SqlDBModule->FillComboBox("SolidGroups", "SGName", cbxSG);
	cbxSG->ItemIndex = -1;
	queryEtalon->Connection = SqlDBModule->ADOConnectionDB;
	GroupBoxNGr->Left=MainForm->Width-GroupBoxNGr->Width-50;
	PanelSG->Left=GroupBoxNGr->Left-PanelSG->Width-10;
	Panel1->Height=Panel2->Top-Panel1->Top;
	//�� ������ ������ ������� ������� ��� ���������
	SLD->oSENSORON->Set(false);
	//������� ����������
	SLD->oSENSLOWPOW->Set(true);
}

// ------------------------------------------------------------------------------
void __fastcall TMainForm::FormDestroy(TObject * Sender) {
	if (threadWork) {
		threadWork->Terminate();
		threadWork->WaitFor();
	}
	if (lCardData) {
		delete lCardData;
		lCardData = NULL;
	}
	if (l�ard791) {
		delete l�ard791;
		l�ard791 = NULL;
	}
	if (SLD) {
		SLD->oSENSORON->Set(false);
		SLD->oSENSLOWPOW->Set(false);
		delete SLD;
	}
	if (gen) {
		delete gen;
	}
	if (TSFreqs) {
		delete TSFreqs;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::MenuProtocolClick(TObject * Sender) {
	TProtocol::Show();
}

// ---------------------------------------------------------------------------
void TMainForm::LoadFromFile(UnicodeString FileName) {
	try {
		TLog::Load3TxtVectDoubleFile(AnsiString(FileName).c_str(), lCardData,0);
		TSG* solidGroup = new TSG(&mainGlobalSettings, lCardData);
		csg = solidGroup->Get1FrecSG();
		delete solidGroup;
		solidGroup = NULL;
		Redraw();
	}
	catch (Exception *ex) {
		TLog::ErrFullSaveLog(ex);
		MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);
	}
}

// ---------------------------------------------------------------------------
void TMainForm::SaveToFile(UnicodeString FileName) {
	try{
		// ���������� � ����
		int arrSize = lCardData->vecMeasuresData[0].vecSensorsData[0].size();
		TLog::Save3TxtVectDoubleFile(FileName,
			lCardData->vecMeasuresData[0].vecSensorsData[0],
			lCardData->vecMeasuresData[0].vecSensorsData[1],
			lCardData->vecMeasuresData[0].vecSensorsData[2], arrSize);
	}
	catch (Exception *ex) {
		TLog::ErrFullSaveLog(ex);
		MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::menuSaveClick(TObject * Sender) {
	if (lCardData->vecMeasuresData[0].vecSensorsData[0].size() == 0)
		return;
	if (SaveDialog->Execute())
		SaveToFile(SaveDialog->FileName);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::menuOpenClick(TObject * Sender) {
	 if (OpenDialog->Execute())
		 LoadFromFile(OpenDialog->FileName);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::WmDropFiles(TWMDropFiles & Message) {
	// HDROP drop_handle = (HDROP)Message.Drop;
	// wchar_t fName[MAXPATH];
	// int filenum = DragQueryFile(drop_handle, -1, NULL, NULL);
	// for (int i = 0; i < filenum; i++)
	// {
	// DragQueryFile(drop_handle, i, fName, MAXPATH);
	// ReadFile(fName);
	// }
	// DragFinish(drop_handle);
}

// ---------------------------------------------------------------------------------
// ����� ��������������, ��������� ���������� � ��������� ���� -----
void __fastcall TMainForm::ReadFile(AnsiString FileName) {
	// AnsiString str = ExtractFileExt(FileName);
	// if (str == ".sg")
	// MainForm->LoadFromFile(FileName);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::SignalChartClick(TObject * Sender) {
	// SignalChart->SetFocus();
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::SignalChartScroll(TObject * Sender) {
	lMinScrollbar->Caption =
		"" + IntToStr((int) SignalChart->BottomAxis->Minimum);
	lMaxScrollbar->Caption =
		"" + IntToStr((int) SignalChart->BottomAxis->Maximum);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::scbChartDraggerChange(TObject * Sender) {
	double length = 200;
	for (int i = 0; i < SignalChart->SeriesCount(); ++i) {
		if (length < SignalChart->Series[i]->MaxXValue()) {
			length = SignalChart->Series[i]->MaxXValue();
		}
	}
	double max = SignalChart->BottomAxis->Maximum;
	double min = SignalChart->BottomAxis->Minimum;
	double step = (double)(max - min) / 100.0;
	if (length > 0)
		scbChartDragger->Max =
			(int)ceil((double)length * 100.0 / (double)(max - min));
	else
		scbChartDragger->Max =
			(int)ceil((double)100.0 * 100.0 / (double)(max - min));
	// curPos = max/step;
	long change = scbChartDragger->Position - curPos;
	if (change > 0) {
		SignalChart->BottomAxis->Maximum = change * step + max;
		SignalChart->BottomAxis->Minimum = change * step + min;
	}
	else {
		SignalChart->BottomAxis->Minimum = change * step + min;
		SignalChart->BottomAxis->Maximum = change * step + max;
	}

	SignalChart->Refresh();
	curPos = scbChartDragger->Position;
	SignalChartScroll(Sender);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::ApplicationEventsMessage(tagMSG & Msg, bool &Handled)
{
	if (Msg.message == WM_HOTKEY) { // ��������� ����
		if (Msg.wParam == 0x00E) // ������������� ���
		{
			WinExec("hh ..\\..\\help\\Help.chm", SW_RESTORE);
			Handled = true;
		}
	}
	if (Msg.message == mainGlobalSettings.threadMsg) { // Handled = true;
		switch (Msg.wParam) {
			case ThreadWork::REDRAW:
			{
				TProtocol::ProtocolSave("���������: ������������");
				csg = threadWork->GetSG();
				Redraw();
				break;
			}
			case ThreadWork::COMPUTE: {
			DEBUG_LEN("32", lCardData);
				AnsiString a = "���������: ������ �����, ���: ";
				a += Msg.lParam;
				TProtocol::ProtocolSave(a);
				if(threadWork!=NULL)
					threadWork->SetCalc();
				break;
			}
			case ThreadWork::COMPLETE: {
				{
					AnsiString a = "���������: ���������� �����, ���: ";
					a += Msg.lParam;
					TProtocol::ProtocolSave(a);
				}
				threadWork->Terminate();
				SLD->LatchesTerminate();
				SLD->SetAlarm(false);
				threadWork->WaitFor();
				delete threadWork;
				threadWork = NULL;
				SetAbleButtons(true);

				if (Msg.lParam == 1 && !SLD->iCC->WasConst(false,100))
				{
					//���� ���� ��������� ������ ��������� - ��������
					Start();
				}
				else
				{
					if(SLD->iCC->WasConst(false,100))
                    	TProtocol::ProtocolSave("������� ���� ����������");
					//���� ���� ��������� ������ � ������� - ���������
					TProtocol::ProtocolSave("_return_code==false");
//					bWork->Caption = "������";
//					bCancelWork->Enabled = false;
				}
				break;
			}
			case ThreadWork::UPDATE_STATUS: {
				if(threadWork!=NULL)
					UpdateStatus(threadWork->GetStext1(), threadWork->GetStext2());
				break;
			}
		}
	}
}

// ---------------------------------------------------------------------------
void TMainForm::UpdateStatus(AnsiString _text1, AnsiString _text2) {
	StatusBar->Panels->Items[0]->Text = _text1;
	StatusBar->Panels->Items[1]->Text = _text2;
	StatusBar->Font->Size=20;
//	StatusBar->Font->Color=clRed;
	StatusBar->Refresh();
}

// ---------------------------------------------------------------------------
void TMainForm::ClearCharts(void) {
	// SignalChart->SeriesList->Clear();
}

// ---------------------------------------------------------------------------
void TMainForm::DrawOnChart() {
	// ClearCharts();
	// if (lSGSignal.size() < 100)
	// return;
	// SignalChart->BottomAxis->Automatic = true;
	// SignalChart->BottomAxis->SetMinMax(0, 1000);
	// SignalChart->LeftAxis->Automatic = true;
	// SignalChart->Refresh();
	// bool DrawNetU = ini->ReadBool("SignalView","NetU",true);
	// bool DrawSignalSG = ini->ReadBool("SignalView","SignalSG",true);
	// bool DrawSynchroDetector = ini->ReadBool("SignalView","SynchroDetector",true);
	// bool DrawInductionSignal = ini->ReadBool("SignalView","InductionSignal",true);
	// bool DrawSynchroSignal = ini->ReadBool("SignalView","SynchroSignal",true);
	// for (unsigned int i = 0; i < lSGSignal.size(); i++)
	// {
	////		Series->AddXY(i, lSGSignal[i].I, "", clBlue);
	////		Series1->AddXY(i, lSGSignal[i].U, "", clRed);
	// if(DrawNetU)SeriesNetU->AddXY(i, lSGSignal[i].NetU, "", SeriesNetU->SeriesColor);
	// if(DrawSignalSG)SeriesSignalSG->AddXY(i, lSGSignal[i].SignalSG, "", SeriesSignalSG->SeriesColor);
	// if(DrawSynchroDetector)SeriesSynchroDetector->AddXY(i, lSGSignal[i].SynchroDetector, "", SeriesSynchroDetector->SeriesColor);
	// if(DrawInductionSignal)SeriesInductionSignal->AddXY(i, lSGSignal[i].InductionSignal, "", SeriesInductionSignal->SeriesColor);
	// if(DrawSynchroSignal)SeriesSynchroSignal->AddXY(i, lSGSignal[i].SynchroSignal, "", SeriesSynchroSignal->SeriesColor);
	// }
	// vector<vector<SGPoint> >sgPoints = sgc->GetPoints(lSGSignal);
	// for (unsigned int t = 0; t < sgPoints.size(); t++)
	// {
	// for (unsigned int i = 0; i < sgPoints[t].size(); i++)
	// {
	////			Series2->AddXY(sgPoints[t][i].index, sgPoints[t][i].I, "", clBlue);
	////			Series3->AddXY(sgPoints[t][i].index, sgPoints[t][i].U, "", clRed);
	// SeriesDotsSignalSG->AddXY(sgPoints[t][i].index, sgPoints[t][i].SignalSG, "", SeriesDotsSignalSG->SeriesColor);
	// SeriesDotsNetU->AddXY(sgPoints[t][i].index, sgPoints[t][i].NetU, "", SeriesDotsNetU->SeriesColor);
	// }
	// }
	// vector<double>Coords = sgc->GetCoords(sgPoints);
	// String str = "";
	// for (unsigned i = 0; i < Coords.size(); i++)
	// str += FloatToStrF(Coords[i], ffFixed, 3, 3) + "   ";
	// StatusBar->Panels->Items[1]->Text = str;
	// StatusBar->Refresh();
}

// ---------------------------------------------------------------------------
void TMainForm::DrawThresholds() {
	// for (int i = 0; i < (int)local_th.size(); i++)
	// {
	// lsThresholds[i]->AddXY(0, local_th[i], "", clBlack);
	// lsThresholds[i]->AddXY(SignalChart->Series[0]->MaxXValue(), local_th[i], "", clBlack);
	// SignalChart->AddSeries(lsThresholds[i]);
	// }
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::Edit1Exit(TObject * Sender) {
	// TEdit* p = (TEdit*)Sender;
	// double val;
	// try
	// {
	// val = p->Text.ToDouble();
	// local_th[p->Tag] = val;
	// }
	// catch (...)
	// {
	// p->SetFocus();
	// }
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::bStartClick(TObject * Sender) {
	if (threadWork == NULL)
		Start();
	else
		ReStart();
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::bCancelClick(TObject * Sender) {
	Stop();
}
// ---------------------------------------------------------------------------


void TMainForm::Start()
{
	int err=0;
	try {

		bool isControl = DEBUG_OK(SLD->iCC->Get()); //��������� ���� ����������
		if (isControl) {
			SetAbleButtons(false);
			if (threadWork != NULL)
			{
				TProtocol::ProtocolSave("TMainForm::Start: ������ ��� ��������");
				return;
			}
			inWork = true;
			SLD->oSENSLOWPOW->Set(true);
//			SLD->oSENSORON->Set(true);//��� 12�


//			TProtocol::Clear();
//			bCancel->Enabled = true; //����� ������ ����� ��������
			if (threadWork != NULL) {
				threadWork->Terminate();
				threadWork->WaitFor();
				delete threadWork;
				threadWork = NULL;
			}
#ifndef TVIRTLCARD791
			if (!gen) {
				gen = new TGSPF052(&mainGlobalSettings, err);
			}
#endif
			threadWork = new ThreadWork(true, lCardData, &mainGlobalSettings, gen);
			SLD->SetAlarm(true);
		}
		else {
			AnsiString msg = "��� ������� ����� ����������!!! \n";
			TExtFunction::ShowBigModalMessage(msg, clRed);
            SetAbleButtons(true);
			return;
		}

	}
	catch (Exception *ex) {
		TLog::ErrFullSaveLog(ex);
		MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);
	}
}
// ---------------------------------------------------------------------------
void TMainForm::ReStart()
{
	//������������ ������ ��� �������� ��� ���������, ������� � ��� ���
	if (threadWork == NULL)
	{
		TProtocol::ProtocolSave("TMainForm::ReStart: ������ �� ��������");
		return;
	}
	threadWork->SetCalc();
}
// ---------------------------------------------------------------------------
void TMainForm::Stop()
{
	if (threadWork == NULL)
	{
		TProtocol::ProtocolSave("TMainForm::Stop: ������ �� ��������");
		return;
	}
	//������ �����
	threadWork->Terminate();
	threadWork->SetCalc();
	threadWork->WaitFor();
	delete threadWork;
	threadWork = NULL;
	//�������� ���������
	if (gen) {
	   delete gen;
	   gen = NULL;
	}
//	MainMenu->Items->Visible = true;
//	GroupBoxNGr->Visible = true;
//	bStart->Enabled = true;
	SetAbleButtons(true);
	inWork = false;
	SLD->oSENSORON->Set(false);
	SLD->LatchesTerminate();
	UpdateStatus("�������� ������", "������ \"������\"");
	TProtocol::ProtocolSave("User: �������� ������ ������");
}
// ---------------------------------------------------------------------------
void TMainForm::Redraw()
{
	//������� ��� ���������� ������
//	int MeasCount = lCardData->vecMeasuresData.size();
//	TExtFunction::PrepareFastLineChart
//		(MeasCount * l�ard791->countLogCh, 2500, SignalChart);
//	for (int k = 0; k < MeasCount; k++) {
//		for (int i = 0; i < l�ard791->countLogCh; i++) {
//			int sz =
//				lCardData->vecMeasuresData[k].vecSensorsData[0].size();
//			double* arr = new double[sz];
//			for (int j = 0; j < sz; j++) {
//				arr[j] =
//					lCardData->vecMeasuresData[k].vecSensorsData
//					[i][j] - k * 10.0; // ������ �����
//			}
//			SignalChart->Series[k * l�ard791->countLogCh + i]
//				->AddArray(arr, sz);
//		}
//	}
    DEBUG_LEN("35", lCardData);
	//��������� ����� ��������
	int chCount = lCardData->vecMeasuresData[0].vecSensorsData.size();
	TExtFunction::PrepareChartToTst(SignalChart, chCount , 0, 0);
	ChangeColor();
	SignalChart->Series[0]->Title += " ����������";
	SignalChart->Series[1]->Title += " ����������";
	SignalChart->Series[2]->Title += " ���";
	// ������� �� ����� �������
	int arrSize;
	for (int i = 0; i < chCount; i++) {
		arrSize = lCardData->vecMeasuresData[0].vecSensorsData[i].size();
		double* tmpArray = new double[arrSize];
		for (int j = 0; j < arrSize; j++) {
			tmpArray[j] = lCardData->vecMeasuresData[0].vecSensorsData[i][j];
		}
//		SignalChart->Series[i]->Color=colorSer[i+1];
		SignalChart->Series[i]->AddArray(tmpArray, arrSize - 1);
	}
	// ��������� �������� �� �������
	vector<int>Thresholds;
	int res;
	// ��������� ������ �� ����������  �����������
	queryEtalon->Close();
	AnsiString strSQL = "select threshold_value from TypeSizeThresholds where TS_id=:pTS_id order by threshold_value";
	queryEtalon->SQL->Text = strSQL;
	queryEtalon->Parameters->ParamByName("pTS_id")->Value = mainGlobalSettings.indexCurrentTypeSize;
	queryEtalon->Open();
	queryEtalon->First();
	int tst=queryEtalon->RecordCount;
	while (!queryEtalon->Eof) {
		res = queryEtalon->FieldByName("threshold_value")->AsInteger;
		Thresholds.push_back(res);
		queryEtalon->Next();
	}
	queryEtalon->Close();
	// ��������� ���� �������
	vector<double> BarkValues;
	DEBUG_LEN("40", lCardData);
	BarkValues = lCardData->GetBarkValues(Thresholds);
	//������� ����������� �����
	TFastLineSeries* series = new TFastLineSeries(SignalChart);
//	series->ColorEachPoint = true;
	series->Marks->Visible = false;
	series->LinePen->Width = 3;
	series->Color = clBlack;
	series->ShowInLegend = false;
	SignalChart->AddSeries(series);
	double PeriodProc = arrSize / 300.0;
	for (int i = 0; i < Thresholds.size(); i++) {
		res = (int)(Thresholds[i]*PeriodProc);
		SignalChart->Series[SignalChart->SeriesCount()-1]->AddXY(res,BarkValues[i], "", clBlack);
	}
//    //������ ����� ��� ��������
//	TColor colorSer[7];
//	colorSer[0] = clBlack;
//	colorSer[1] = clBlue;
//	colorSer[2] = clRed;
//	colorSer[3] = clGreen;
//	colorSer[4] = clFuchsia;
//	colorSer[5] = clYellow;
//	colorSer[6] = clWhite;
	//������ � ��
	PanelSG->Caption = csg.group+" "+IntToStr((int)(csg.probability*100))+"%";
//	if(csg.group_id < 7)
		PanelSG->Color = csg.color;//colorSer[csg.group_id]; //todo ���������!
		PanelSG->Font->Color = 16777215 - (int)csg.color;
//	else PanelSG->Color = clLime;
	//�������� ���� ������������������� ������� ��������
	EtalonDatas EDatas = EtalonDatas(mainGlobalSettings.indexCurrentTypeSize
		,mainGlobalSettings.indexCurrentSGGost);
	TExtFunction::PrepareChartToTst(EtalonChart, EDatas.Etalons.size(), 0, 0);

	// ������� �� ����� �������
	arrSize = EDatas.Etalons[0].BarkValues.size();
	for (int i = 0; i < EDatas.Etalons.size(); i++) {
//		arrSize = lCardData->vecMeasuresData[0].vecSensorsData[i].size();
		double* tmpArray = new double[arrSize];
		for (int j = 0; j < arrSize; j++) {
			tmpArray[j] = EDatas.Etalons[i].BarkValues[j];
		}
//		int sg_id = EDatas.Etalons[i].SG_id;
//		if(sg_id < 0)
//			sg_id = 0;
		EtalonChart->Series[i]->Title = EDatas.Etalons[i].SG_name;
//		if(sg_id < 7)
			EtalonChart->Series[i]->Color = EDatas.Etalons[i].Color;//colorSer[sg_id];
//		else EtalonChart->Series[i]->Color = clLime;
		EtalonChart->Series[i]->AddArray(tmpArray, arrSize - 1);
	}

	//�� ���� ��������� - ��� � ��� �� ���� ������ ��� ����
//	series = new TFastLineSeries(EtalonChart);
////	series->ColorEachPoint = true;
//	series->Marks->Visible = false;
//	series->LinePen->Width = 3;
//	series->Color = clBlack;
//	series->ShowInLegend = true;
//	EtalonChart->AddSeries(series);
	EtalonChart->Series[EtalonChart->SeriesCount()-1]->ShowInLegend=true;
	EtalonChart->Series[EtalonChart->SeriesCount()-1]->Title="����� ������";
//	EtalonChart->Series[SignalChart->SeriesCount()-1]->LinePen->Width = 3;
	for (int i = 0; i < Thresholds.size(); i++) {
		EtalonChart->Series[EtalonChart->SeriesCount()-1]->AddY(BarkValues[i], "", clBlack);
	}

}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::menuColorSettClick(TObject * Sender) {
	FormColor = new TFormColor(this);
	FormColor->Show();
//	delete FormColor;
	// FormColor->ShowModal();
	// ChangeColor();
}

// ---------------------------------------------------------------------------
void TMainForm::ChangeColor() {
	// ����� �����
	SignalChart->Series[0]->Color = clBlue; // ����������
	SignalChart->Series[1]->Color = clRed; // ����������
	SignalChart->Series[2]->Color = clGreen; // ���
	SignalChart->Series[3]->Color = clBlack; // DotsMeasure2
}

// ---------------------------------------------------------------------------

void __fastcall TMainForm::menuSettingsClick(TObject * Sender) {
	ADCSettForm = new TADCSettForm(this, l�ard791);
	ADCSettForm->ShowModal();
	delete ADCSettForm;
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::menuTypeSizeClick(TObject * Sender) {
	//SettingsForm->ShowModal();
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::menuAboutClick(TObject * Sender) {
	fmAboutBox = new TfmAboutBox(this);
	fmAboutBox->ShowModal();
	fmAboutBox->Close();
	fmAboutBox = NULL;
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::menuSGSettClick(TObject * Sender) {

	SGSettForm=new TSGSettForm(this,&mainGlobalSettings);
	SGSettForm->ShowModal();
	SGSettForm->Close();
	SGSettForm = NULL;
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::PanelSQClick(TObject * Sender) {
	ToolBarTop->Refresh();
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::PanelONClick(TObject * Sender) {
	ToolBarTop->Refresh();
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::mnuCheckGenClick(TObject * Sender) {
	fmDiagnost = new TfmDiagnost(this, &mainGlobalSettings, l�ard791, lCardData);
	fmDiagnost->Show();
	// fmDiagnost->ShowModal();
	// fmDiagnost->Close();
	// fmDiagnost = NULL;
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::mnuCheck1730Click(TObject * Sender) {
	FSignalsState = new TFSignalsState(this, &mainGlobalSettings, SLD);
	FSignalsState->Show();
	// FSignalsState->ShowModal();
	// FSignalsState->Close();
	// FSignalsState = NULL;
}
// ---------------------------------------------------------------------------
void __fastcall TMainForm::menuEtalonsClick(TObject *Sender)
{
	fmEditEtalon = new TfmEditEtalon(this,&mainGlobalSettings,lCardData);
	fmEditEtalon->ShowModal();
	fmEditEtalon=NULL;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::cbTypeSizeSelect(TObject *Sender)
{
	int ind = cbTypeSize->ItemIndex;
	mainGlobalSettings.indexCurrentTypeSize = (int)cbTypeSize->Items->Objects[ind];
	SqlDBModule->UpdIntSql("SettingsGlobal","indexCurrentTypeSize",mainGlobalSettings.indexCurrentTypeSize,NULL);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::cbSGGostSelect(TObject *Sender)
{
	int ind = cbSGGost->ItemIndex;
	mainGlobalSettings.indexCurrentSGGost = ind;
	SqlDBModule->UpdIntSql("SettingsGlobal","indexCurrentSGGost",mainGlobalSettings.indexCurrentSGGost,NULL);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::bbtCreateEtalonClick(TObject *Sender)
{
	AnsiString strSQL = "";
	AnsiString strCmdSql = "";
	int ind = 0;
	int err;
	double res = 0;
	try {
		if (cbxSG->ItemIndex == -1) {
			MessageDlg("�������� ������ ���������", mtError,
				TMsgDlgButtons() << mbOK, NULL);
		}
		else
		{
			AnsiString whereStr = "TS_id = "+IntToStr(mainGlobalSettings.indexCurrentTypeSize)+" ORDER BY frequency_value ASC";
			int freq = SqlDBModule->GetIntFieldSQL("TypeSizeFrequencies",
				"frequency_value",whereStr,50000,err);
			// ������� � ����, ��� ������ ��� ������ ������
			AnsiString fileName = "";
			fileName = "..\\..\\SavedEtalons\\�_";
			fileName += FormatDateTime("yyyy_mm_dd_hh_mm_ss_", Now());
			fileName += IntToStr(freq);
			fileName += "_TS_";
//			fileName += IntToStr(mainGlobalSettings.indexCurrentTypeSize);
			fileName += SqlDBModule->GetStrFieldSQL("TypeSizes","TSName"
				,"rec_id = "+IntToStr(mainGlobalSettings.indexCurrentTypeSize),0,err);
			fileName += "_SG_";
			fileName += cbxSG->Text;
			fileName += "_FHZ.csv";
			// �������� �������� � ����
			ind = cbxSG->ItemIndex;
			strSQL =
				"INSERT INTO Etalons(ts_id,fenable,frequency,address_file,sg_id) VALUES( ";
			strSQL += IntToStr(mainGlobalSettings.indexCurrentTypeSize);
			strSQL += ",";
			strSQL += IntToStr(1);
			strSQL += ",";
			strSQL += IntToStr(freq);
			strSQL += ",'";
			strSQL += fileName;
			strSQL += "',";
			strSQL += IntToStr((int)cbxSG->Items->Objects[ind]);
			strSQL += ");\n";
			strSQL += "SELECT @@IDENTITY AS 'LAST_ID';";
			queryEtalon->Close();
			queryEtalon->SQL->Text = strSQL;
			queryEtalon->Open();
			int etId = queryEtalon->FieldByName("LAST_ID")->AsInteger;
			queryEtalon->Close();
			strSQL = "INSERT INTO EtalonValues Select ";
			strSQL += IntToStr(etId);
			strSQL +=
				",threshold_value, 123 from TypeSizeThresholds where TS_id=";
			strSQL += IntToStr(mainGlobalSettings.indexCurrentTypeSize);
			strSQL += " order by threshold_value";
			queryEtalon->SQL->Text = strSQL;
			queryEtalon->ExecSQL();
			queryEtalon->Close();
			// fmEditEtalon = new TfmEditEtalon(this,pGlobalSettings,lCardData);
			// fmEditEtalon->ShowModal();
			// fmEditEtalon=NULL;
			// cbxSG->ItemIndex=-1;

			// ��������� �������� �� �������
			vector<int>Thresholds;

			// ��������� ������ �� ����������  �����������
			queryEtalon->Close();
			strSQL =
				"select threshold_value from TypeSizeThresholds where TS_id=:pTS_id order by threshold_value";
			queryEtalon->SQL->Text = strSQL;
			queryEtalon->Parameters->ParamByName("pTS_id")->Value = mainGlobalSettings.indexCurrentTypeSize;
			queryEtalon->Open();
			queryEtalon->First();
			int tst=queryEtalon->RecordCount;
			while (!queryEtalon->Eof)
			{
				res = queryEtalon->FieldByName("threshold_value")->AsInteger;
				Thresholds.push_back(res);
				queryEtalon->Next();
			}
			queryEtalon->Close();
			strSQL=	"SELECT rec_id as vrec_id, etalon_id,thres_val,barkgausen_val FROM EtalonValues where etalon_id = :petalon_id order by thres_val";
			queryEtalon->SQL->Text=strSQL;
			queryEtalon->Parameters->ParamByName("petalon_id")->Value = etId;
			// ��������� ���� �������
			vector<double> BarkValues;
			BarkValues = lCardData->GetBarkValues(Thresholds);
			// ������� � ����
			queryEtalon->Open();
			queryEtalon->First();
			int ii = 0;
			for (int i = 0; i < Thresholds.size(); i++)
			{
				strCmdSql =
					"UPDATE EtalonValues SET barkgausen_val = :pbarkgausen_val";
				// cmdEtalonVal->Parameters->ParamByName("pbarkgausen_val")->Value = 222;
				// double tst = BarkValues[i];
				cmdEtalonVal->Parameters->ParamByName("pbarkgausen_val")->Value = BarkValues[i];
				// strCmdSql+= FloatToStr(BarkValues[i]);
				strCmdSql += " WHERE rec_id=";
				strCmdSql +=IntToStr(queryEtalon->FieldByName("vrec_id")->AsInteger);
				cmdEtalonVal->CommandText = strCmdSql;
				// cmdEtalonVal->Parameters->ParamByName("prec_id")->Value = queryEtalonVal->FieldByName("vrec_id")->AsInteger;
				// cmdEtalonVal->Parameters->ParamByName("pbarkgausen_val")->Value = &BarkValues[i];
				cmdEtalonVal->Execute();
				queryEtalon->Next();
			}
			queryEtalon->Close();
			int freqNum=lCardData->getFreqNum();
			int arrSize = lCardData->vecMeasuresData[freqNum].vecSensorsData
				[0].size();
			TLog::Save3TxtVectDoubleFile(ExtractFilePath(Application->ExeName) +
				fileName, lCardData->vecMeasuresData[freqNum].vecSensorsData[0],
				lCardData->vecMeasuresData[freqNum].vecSensorsData[1],
				lCardData->vecMeasuresData[freqNum].vecSensorsData[2], arrSize);
			MessageDlg("������ ��������", mtConfirmation,
				TMsgDlgButtons() << mbOK, NULL);
			err = 0;
			//
			// queryTSz->Close();
			// queryTSz->Parameters->ParamByName("pTypeSize")->Value = (int)cbTypeSize->Items->Objects[ind];
			// queryTSz->Open();

		}
	}
	catch (Exception *ex) {
		TLog::ErrFullSaveLog(ex);
		MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);
	}
}

//---------------------------------------------------------------------------


void __fastcall TMainForm::FormResize(TObject *Sender)
{
	Panel1->Height=Panel2->Top-Panel1->Top;
	SignalChart->Height=Panel1->Height-SignalChart->MarginBottom-SignalChart->MarginTop;
}
//---------------------------------------------------------------------------
//��������� ������
void TMainForm::SetAbleButtons(bool _enable)
{
	MainMenu->Items->Find("���������")->Enabled = _enable;
	MainMenu->Items->Find("�����������")->Enabled = _enable;
	MainMenu->Items->Find("���������")->Enabled = _enable;
	MainMenu->Items->Find("������")->Enabled = _enable;
	GroupBoxNGr->Visible = _enable;
	bStart->Enabled = _enable;
	cbTypeSize->Enabled = _enable;
	cbSGGost->Enabled = _enable;
}

