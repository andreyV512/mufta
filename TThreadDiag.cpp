// ---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop

#include "TThreadDiag.h"
#include "unTUtils.h"
#include "unTExtFunction.h"
#pragma package(smart_init)
// ---------------------------------------------------------------------------

// Important: Methods and properties of objects in VCL can only be
// used in a method called using Synchronize, for example:
//
// Synchronize(&UpdateCaption);
//
// where UpdateCaption could look like:
//
// void __fastcall TThBlink::UpdateCaption()
// {
// Form1->Caption = "Updated in a thread";
// }
// ---------------------------------------------------------------------------

__fastcall TThDiag::TThDiag(bool CreateSuspended, TChart *_chartGPSF052,
	TLCard791* _thL�ard791, TGSPF052* _Gen, int _chCount, int _timeGen)
	: TThread(CreateSuspended) {
	stopThread = false;
	thChartGPSF052 = _chartGPSF052;
	thL�ard791 = _thL�ard791;
	thGen = _Gen;
	chCount = _chCount;
	timeGen = _timeGen;
	countMeasure = 0;
	vecMeasure.reserve(_chCount);
	for (int i = 0; i < chCount; i++) {
		int szSensor=_timeGen*1000*thL�ard791->frequencyPerChannel_Hz;
		double* sensorData = new double[szSensor];
		for (int d = 0; d < szSensor; d++) {
			sensorData[d] = -99;
		}
		vecMeasure.push_back(sensorData);
	}
}


// ---------------------------------------------------------------------------
void __fastcall TThDiag::Execute() {
	try {
		countMeasure = 0;
		NameThreadForDebugging(System::String(L"TThDiagChart"));
		bool* ret = false;
        // �������� ������� �������
		SLD->oSENSORON->Set(true);
		startTime = clock(); // ��������� �����
		thGen->StartGSPF052();
		while (!stopThread && elapsTime < timeGen * 1000) {
			currTime = clock();
			int tt = vecMeasure.size();
			Y0 = thL�ard791->GetValueMeasure(0, 1, false, ret);
			// Y1 = thL�ard791->GetValueVolt(Y0, 1) * 100.0;
			vecMeasure[0][countMeasure] = thL�ard791->GetValueProcent(Y0);
			Y0 = thL�ard791->GetValueMeasure(1, 0, false, ret);
			// Y3 = thL�ard791->GetValueVolt(Y0, 1) * 100.0;
			vecMeasure[1][countMeasure] = thL�ard791->GetValueProcent(Y0);
			Y0 = thL�ard791->GetValueMeasure(2, 1, false, ret);
			// Y5 = thL�ard791->GetValueVolt(Y0, 1) * 100.0;
			vecMeasure[2][countMeasure] = thL�ard791->GetValueProcent(Y0);
			Y0 = thL�ard791->GetValueMeasure(3, 2, false, ret);
			vecMeasure[3][countMeasure] = thL�ard791->GetValueProcent(Y0);
			Y0 = thL�ard791->GetValueMeasure(4, 3, false, ret);
			vecMeasure[4][countMeasure] = thL�ard791->GetValueProcent(Y0);
			countMeasure++;
			Synchronize(UpdateChart);
			elapsTime = currTime - startTime;
		}
		thGen->StopGSPF052();
		// ��������� ������� �������
		SLD->oSENSORON->Set(false);
		PostMessage(Application->Handle, WM_USER + 99, 33, 0);
	}
	catch (Exception *ex) {
		TLog::ErrFullSaveLog(ex);
		this->Terminate();
		this->WaitFor();
		PostMessage(Application->Handle, WM_USER + 99, 33, 0);
		MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);
	}
}
// ---------------------------------------------------------------------------

void __fastcall TThDiag::UpdateChart() {
	// thChartGPSF052->Series[0]->AddY(Y1, "", clWhite);
	// thChartGPSF052->Series[1]->AddY(Y3, "", clWhite);
	// thChartGPSF052->Series[2]->AddY(Y5, "", clWhite);
}

__fastcall TThDiag::~TThDiag(void) {
	Terminate();
	WaitFor();
}
