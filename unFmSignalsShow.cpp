// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "unFmSignalsShow.h"
// -------------
#include "unTUtils.h"
#include "unTExtFunction.h"
#include "UnDataMod.h"
// ---------
#define strStatus "������: "
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmSignalsShow *fmSignalsShow;

// ---------------------------------------------------------------------------
__fastcall TfmSignalsShow::TfmSignalsShow(TComponent* Owner, TStatusBar* _mainStatusBar, TLabel* _mainStatusLabel, TGlobalSettings* _programSettings) : TForm(Owner) {
	ssStatusBar = _mainStatusBar;
	ssStatusLabel = _mainStatusLabel;
	ssProgramSettings = _programSettings;
	isError = false;
}

// ---------------------------------------------------------------------------
void __fastcall TfmSignalsShow::ButtonStartClick(TObject *Sender) {
	AnsiString msg;
	try {
		sshWorkX502->AlarmSituiation(msg);
		// �������� ���� �� 4 � 3 �
		if (sshWorkX502->SetAsyncDac1(4, msg) < 0 || sshWorkX502->SetAsyncDac2(3, msg) < 0) {
			msg = "������ ��������� ���: ";
			ssProgramSettings->colorMSG = ssProgramSettings->colorBrak;
			TExtFunction::UpdateStatusBar(ssStatusBar, strStatus, msg, ssProgramSettings->colorMSG);
			TExtFunction::UpdateLabelStatus(ssStatusLabel, strStatus, msg, ssProgramSettings->colorMSG);
			// MessageDlg(msg, mtError, TMsgDlgButtons() << mbOK, NULL);
			return;
		}
		msg = "������ ������� ����� ������! ";
		if (sshWorkX502->StartRead(false, msg) < 0) {
			ssProgramSettings->colorMSG = ssProgramSettings->colorBrak;
			TExtFunction::UpdateStatusBar(ssStatusBar, strStatus, msg, ssProgramSettings->colorMSG);
			TExtFunction::UpdateLabelStatus(ssStatusLabel, strStatus, msg, ssProgramSettings->colorMSG);
			// MessageDlg(msg, mtError, TMsgDlgButtons() << mbOK, NULL);
			return;
		}
		else {

		}
		timerCheckSignals->Enabled = true;
		ButtonStart->Enabled = false;
		ButtonStop->Enabled = true;
		// }
	}
	catch (Exception *ex) {
		TLog::ErrFullSaveLog(ex);
		ssProgramSettings->colorMSG = ssProgramSettings->colorBrak;
		TExtFunction::UpdateStatusBar(ssStatusBar, strStatus, ex->Message, ssProgramSettings->colorMSG);
		TExtFunction::UpdateLabelStatus(ssStatusLabel, strStatus, ex->Message, ssProgramSettings->colorMSG);
		MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);
	}
}
// ---------------------------------------------------------------------------

void __fastcall TfmSignalsShow::ButtonStopClick(TObject *Sender) {
	timerCheckSignals->Enabled = false;
	sshWorkX502->StopThread();
	sshWorkX502->AlarmSituiation(msgLast);
	ButtonStart->Enabled = true;
	ButtonStop->Enabled = false;
}

// ---------------------------------------------------------------------------
void __fastcall TfmSignalsShow::StringGridDIODrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State) {
	// if (ACol == 1 && ARow > 1) {
	//
	// AnsiString aStr = StringGridDIO->Cells[ACol][ARow];
	// if (aStr == "���") {
	// StringGridDIO->Canvas->Brush->Color = clLime;
	// }
	// else {
	// StringGridDIO->Canvas->Brush->Color = clYellow;
	// }
	// if (!timerCheckSignals->Enabled) {
	// StringGridDIO->Canvas->Brush->Color = clWhite;
	// }
	// else {
	// //
	// }
	// StringGridDIO->Canvas->FillRect(Rect);
	// StringGridDIO->Canvas->TextOut(Rect.Left, Rect.Top,
	// StringGridDIO->Cells[ACol][ARow]);
	// }
	// else {
	// //
	// }
}

// ---------------------------------------------------------------------------
void __fastcall TfmSignalsShow::FormCreate(TObject *Sender) {
	try {
		StringGridDIO->Cells[0][0] = "������";
		StringGridDIO->Cells[1][0] = "��������";
		StringGridDIO->Cells[2][0] = "���������";
		StringGridDIO->Cells[3][0] = "����������";
		// ---------------
		StringGridADC->Cells[0][0] = "�����";
		StringGridADC->Cells[1][0] = "��������";
		// pWorkX502->refreshDeviceList(pWorkX502->parCountDev);
	}
	catch (Exception *ex) {
		TLog::ErrFullSaveLog(ex);
		ssProgramSettings->colorMSG = ssProgramSettings->colorBrak;
		TExtFunction::UpdateStatusBar(ssStatusBar, strStatus, ex->Message, ssProgramSettings->colorMSG);
		TExtFunction::UpdateLabelStatus(ssStatusLabel, strStatus, ex->Message, ssProgramSettings->colorMSG);
		MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);
	}
}

// ---------------------------------------------------------------------------
int TfmSignalsShow::CheckSignals(void) {
	int err = -1;
	try {
		if (sshWorkX502->hndX502) {
			// // ��������� 32 ������ ���
			// if (pWorkX502->getAsyncAdcFrame() < 0) {
			// return result;
			// }
			// else {
			// //
			// }
			for (int i = 0; i < 32; i++) {
				if (i < 16) {
					StringGridADC->Cells[0][i + 1] = "X" + IntToStr(i);
				}
				else {
					StringGridADC->Cells[0][i + 1] = "Y" + IntToStr(i);
				}
				if (sshWorkX502->x502FLAGS_VOLT == 1) {
					double dtmp = sshWorkX502->kadr_data0[i];
					StringGridADC->Cells[1][i + 1] = FloatToStrF(dtmp, ffFixed, 2, 4) + " �";
				}
				else {
					StringGridADC->Cells[1][i + 1] = IntToStr((int)sshWorkX502->kadr_data0[i]) + " Int32";
				}
			}
			// ����������� �������� �����
			// int DIN=0;
			int DIN = sshWorkX502->GetAsyncDigIn(msgLast);
			for (int i = 0; i < 16; i++) {
				int res = DIN & 0x0001;
				if (res == 1) {
					StringGridDIO->Cells[1][i + 1] = "BK�";
					// ������������� ��������������� �������� �����
					// pWorkX502->setAsyncDigOut(i);
				}
				else {
					StringGridDIO->Cells[1][i + 1] = "����";
					// ���������� ��������������� �������� �����
					// pWorkX502->setAsyncDigOut(0);
				};
				// ��������� ������� ������ ���
				DIN = DIN >> 1;
			}
			err = 0;
		}
		else {
			Application->MessageBoxW(L"�� ������� ����� �502", L"������!", MB_ICONERROR);
			err = -2;
		}
	}
	catch (Exception *ex) {
		err = -3;
		isError = true;
		TLog::ErrFullSaveLog(ex);
		ssProgramSettings->colorMSG = ssProgramSettings->colorBrak;
		TExtFunction::UpdateStatusBar(ssStatusBar, strStatus, ex->Message, ssProgramSettings->colorMSG);
		TExtFunction::UpdateLabelStatus(ssStatusLabel, strStatus, ex->Message, ssProgramSettings->colorMSG);
		MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);
	}
	return err;
}

void __fastcall TfmSignalsShow::timerCheckSignalsTimer(TObject *Sender) {
	((TTimer*)Sender)->Enabled = false;
	// if(sshWorkX502->workData->
	CheckSignals();
	if (isError) {
		((TTimer*)Sender)->Enabled = false;
	}
	else {
		((TTimer*)Sender)->Enabled = true;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TfmSignalsShow::FormShow(TObject *Sender) {
	// timerCheckSignals->Enabled = true;
	// ������� �������� �������� � SQL
	try {
		if (DataModuleDB->ADOConnectionDB->Connected) {
			//
		}
		else {
			DataModuleDB->ADOConnectionDB->Open();
		}
		DataModuleDB->ADOQueryDB->SQL->Text = "SELECT REC_ID,ParamCode,ParamName,ParamComment FROM SettingsDefinitions order by OrderCode";
		// DataModuleDB->ADOQueryDB->SQL->Text +=
		// " FROM ParamsDefinitions order by ParamCode";
		DataModuleDB->ADOQueryDB->Open();
		DataModuleDB->ADOQueryDB->First();
		int i = 1;
		while (!DataModuleDB->ADOQueryDB->Eof) {
			if (i > StringGridDIO->RowCount) {
				// StringGridSignals->Rows->Add("");
				StringGridDIO->RowCount = i + 1;
			}
			else {
				//
			}
			if (i < 33) {
				// ���
				StringGridADC->Cells[0][i] = DataModuleDB->ADOQueryDB->FieldByName("ParamName")->AsString;
			}
			else {
				// �������� � ���
				StringGridDIO->Cells[0][i - 32] = DataModuleDB->ADOQueryDB->FieldByName("ParamName")->AsString;
				StringGridDIO->Cells[1][i - 32] = "0";
				StringGridDIO->Cells[2][i - 32] = DataModuleDB->ADOQueryDB->FieldByName("ParamCode")->AsString;
				StringGridDIO->Cells[3][i - 32] = DataModuleDB->ADOQueryDB->FieldByName("ParamComment")->AsString;
			}
			i++;
			DataModuleDB->ADOQueryDB->Next();
		}
		DataModuleDB->ADOQueryDB->Close();
	}

	catch (Exception *ex) {
		Application->MessageBoxW(ex->Message.c_str(), L"������!", MB_ICONERROR);
		TDateTime currentDT;
		AnsiString strCurrentDT = FormatDateTime("yyyy.mm.dd hh:mm:ss", currentDT.CurrentDateTime());
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfmSignalsShow::FormClose(TObject *Sender, TCloseAction &Action)
{
 PostMessage(Application->Handle, ssProgramSettings->msgCloseSigShowForm, 33, 0);
}
//---------------------------------------------------------------------------

