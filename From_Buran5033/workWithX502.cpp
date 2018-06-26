// ---------------------------------------------------------------------------

#pragma hdrstop

#include "workWithX502.h"

// ---------------------------------------------------------------------------
#include <stdio.h>
#include<IniFiles.hpp>
#include "UnDataMod.h"
#include "unTUtils.h"
#include "unTExtFunction.h"

// ---------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// __fastcall WorkX502::WorkX502()
TWorkX502::TWorkX502(void) {
	int err = -1;
	try {
		x502EnableSyncDin = false;
		isX502Opened = false;
		// ���������� ������������ ����
		x502CountDev = 0;
		// ��������
		// ������� ������ ����������/����������� - ��������� ����������
		x502EnableSyncADC = 1;
		// ������ ������ � ����� ������
		x502ChMode = X502_LCH_MODE_COMM;
		// �������� ��� ���� ������� 0-10 �
		// x502ADC_RANGE_V = X502_ADC_RANGE_10;
		// ����� ������������� ����������
		// x502SyncMode = X502_SYNC_INTERNAL;
		// X502_SYNC_INTERNAL        = 0, /**< ���������� ������ */
		// X502_SYNC_DI_SYN1_RISE    = 2, /**< �� ������ ������� DI_SYN1 */
		// X502_SYNC_DI_SYN2_RISE    = 6, /**< �� ������ ������� DI_SYN2 */
		// x502SyncStartMode = X502_SYNC_INTERNAL; // ������� ������ ��� ������������� ����������� ����� (���������� ������ � ������)
		// ����� ��� �������
		// x502CountFIZch=32;
		// ����� ��� �������
		// x502CountLOGch=32;
		// �������� �� ���������
		x502ADCBufSensorSize = 2000;
		x502DinBufSize = 1000;
		for (int i = 0; i < X502_LTABLE_MAX_CH_CNT; i++) {
			adc_Range[i] = 0;
			adc_Range_Value[i] = 10.0;

		}
		for (int i = 0; i < 32; i++) {
			// �������� ������
			valuesIN[i] = 0;
			// �������� �� ������
			valuesOUT[i] = 0;
		}

		err = 0;
	}

	catch (Exception *ex) {
		err = -2;
		TLog::ErrFullSaveLog(ex);
		wwProgramSettings->colorMSG = wwProgramSettings->colorBrak;
		TExtFunction::UpdateStatusBar(wwProgramSettings->gsStatusBar, strStatus, ex->Message, wwProgramSettings->colorMSG);
		TExtFunction::UpdateLabelStatus(wwProgramSettings->gsStatusLabel, strStatus, ex->Message, wwProgramSettings->colorMSG);
		MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);

	}
}

// ------------------------------
TWorkX502::~TWorkX502(void) { //
	// ���� ������� �� ��������� ������ ������ - �������
	if (threadX502) {
		if (!threadX502->stopThread) {
			threadX502->Terminate();
			threadX502->WaitFor();
		}
		delete threadX502;
		threadX502 = NULL;
	}
	CloseDevice(msgLast);
}

// ------------------------------
// ����� ���������� ����� ����������� �������� ���������, � ����� ���������  ������
void TWorkX502::UpdateControls() {
	// cbbDevList->Enabled = (hndX502==NULL);
	// btnOpen->Caption = hndX502 == NULL ? "���������� ����� � �������" :
	// "��������� ����� � �������";

}

// ------------------------------
// ������� ���������� ���������
void TWorkX502::FreeDevList() {
	// if (devrecList.Length != 0) {
	// X502_FreeDevRecordList(&devrecList[0], devrecList.Length);
	// devrecList.Length = 0;
	// }
	// //cbbDevList->Items->Clear();
}

// ------------------------------
/* ���������� ������ ������������ ���� L502 � E502 */
void TWorkX502::RefreshDeviceList(int &_CountDev) {
	uint32_t pci_devcnt = 0;
	uint32_t usb_devcnt = 0;
	int32_t fnd_devcnt = 0;
	/* �������� ���������� ������������ ��������� �� ����������� PCI � USB */
	L502_GetDevRecordsList(NULL, 0, 0, &pci_devcnt);
	E502_UsbGetDevRecordsList(NULL, 0, 0, &usb_devcnt);
	/* �������� ������ ��� ������� ��� ���������� ����������
	 ���������� ������� */
	if ((pci_devcnt + usb_devcnt) != 0) {
		devrecList.Length = pci_devcnt + usb_devcnt;
		// * �������� ������ � ������� L502
		if (pci_devcnt != 0) {
			int32_t res = L502_GetDevRecordsList(&devrecList[fnd_devcnt], pci_devcnt, 0, NULL);
			if (res >= 0) {
				fnd_devcnt += res;
			}
		}
		// ��������� ������ � ������� E502, ������������ �� USB
		if (usb_devcnt != 0) {
			int32_t res = E502_UsbGetDevRecordsList(&devrecList[fnd_devcnt], usb_devcnt, 0, NULL);
			if (res >= 0) {
				fnd_devcnt += res;
			}
			_CountDev = pci_devcnt + usb_devcnt;
		}
	}
	else {
		_CountDev = 0;
	}
}

// ------------------------------
// ��������� ���������� ����� ������
int TWorkX502::SetupParams(int _x502CountLOGch, int _SyncMode, int _SyncStartMode) {
	int err = -1;
	uint32_t lch_cnt = _x502CountLOGch;
	// ������� ������������ �������� � ����� ������ ���������
	// static uint32_t f_mode_tbl[] = {X502_LCH_MODE_COMM, X502_LCH_MODE_DIFF, X502_LCH_MODE_ZERO};
	// ������� ������������ �������� � ����� ���������� ���
	// static uint32_t f_range_tbl[] = {X502_ADC_RANGE_10, X502_ADC_RANGE_5, X502_ADC_RANGE_2, X502_ADC_RANGE_1, X502_ADC_RANGE_05, X502_ADC_RANGE_02};
	/* ������� ������������ �������� ����� ��������� ������������� */
	// static uint32_t f_sync_tbl[] = {X502_SYNC_INTERNAL, X502_SYNC_EXTERNAL_MASTER, X502_SYNC_DI_SYN1_RISE, X502_SYNC_DI_SYN2_RISE, X502_SYNC_DI_SYN1_FALL, X502_SYNC_DI_SYN2_FALL};
	// ������������� ���-�� ���������� �������, ����� ��������� ��������� ������� �� �����
	err = X502_SetLChannelCount(hndX502, lch_cnt);
	// ��������� ���������� ����������� ������.
	// ������� ������������� ��������� ��������� ����������� ������ � ����������
	// ������� ���.
	for (int i = 0; i < _x502CountLOGch; i++) {
		if (err == X502_ERR_OK) {
			int val = GetADCRange(i);
			err = X502_SetLChannel(hndX502, i, i, x502ChMode, val, 0);
		}
	}

	// ����������� �������� ������� ������������� � ������� �����
	// ��������������, ��� ��� ����������
	if (err == X502_ERR_OK)
		// ������������� ��� ����� ����������� ������� ������� �������������
		// X502_SYNC_INTERNAL = 0< ���������� ������
		// X502_SYNC_EXTERNAL_MASTER = 1,  �� �������� ������� �� ������� ������������ �������������
		// X502_SYNC_DI_SYN1_RISE = 2, < �� ������ ������� DI_SYN1
		// X502_SYNC_DI_SYN1_FALL = 3, �� ����� ������� DI_SYN1
		// X502_SYNC_DI_SYN2_RISE = 6, < �� ������ ������� DI_SYN2 */
			err = X502_SetSyncMode(hndX502, _SyncMode);
	if (err == X502_ERR_OK)
		// ������� ������������� ������� ������� ����������� �����/������ ������.
			err = X502_SetSyncStartMode(hndX502, _SyncStartMode);

	// ����������� ������� ����� � ��� � ��������
	if (err == X502_ERR_OK)
		err = SetAdcFreq(x502AdcFreq, x502AdcFreqLch, x502DinFreq);

	/* ���������� ��������� � ������ */
	if (err == X502_ERR_OK)
		err = X502_Configure(hndX502, 0);
	// ������� ������� � ���������
	wwProgramSettings->adcFreqLch = x502AdcFreqLch;
	wwProgramSettings->dinFreqLch = x502DinFreq;
	return err;
}

// ------------------------------
//
int TWorkX502::SetAdcFreq(int &_x502AdcFreq, int &_x502AdcFreqLch, int &_x502DinFreq) {
	int err = -1;
	double f_acq = _x502AdcFreq; // ������� ����������� ����� ���
	double f_lch = _x502AdcFreqLch; // ������� ����������� ����� ��� �� �����
	// _x502AdcFreqLch - ������� ���������, �� ������������� ������������ ������� ����� ������ (������� ����������� ��������).
	double f_din = _x502DinFreq; // ������� ����������� �����  ��� ��������
	// ������������� ������� ����������� �����  ��� ���
	err = X502_SetAdcFreq(hndX502, &f_acq, &f_lch);
	if (err == X502_ERR_OK) {
		// �������� ������� �������������� �������
		_x502AdcFreq = f_acq;
		// (������� ����� �� ���������� �����) ���
		_x502AdcFreqLch = f_lch;
		// ������������� ������� ����������� �����  ��� ��������
		err = X502_SetDinFreq(hndX502, &f_din);
		if (err == X502_ERR_OK) {
			_x502DinFreq = f_din;
		}
		else {
			_x502DinFreq = -1;
		}
	}
	else {
		_x502AdcFreq = -1;
	}

	return err;
}

// ------------------------------
// ��������� ��� ��������� ����� ����������� ����� ��� �������� ������
int32_t TWorkX502::SetSyncDinStream(bool _EnableSyncDin) {
	int32_t err;
	//
	if (_EnableSyncDin) {
		err = X502_StreamsEnable(hndX502, X502_STREAM_DIN);
	}
	else {
		err = X502_StreamsDisable(hndX502, X502_STREAM_DIN);
	}
	return err;
}

// ------------------------------
void __fastcall TWorkX502::OnThreadTerminate(TObject *Sender) {
	// void TWorkX502::OnThreadTerminate() {
	AnsiString msg = "���� ������ ��������: " + String(X502_GetErrorString(threadX502->err));
	if (threadX502->err != X502_ERR_OK) {
		AlarmSituiation(msg);
		TLog::SaveStrLog(msg);
		wwProgramSettings->colorMSG = wwProgramSettings->colorBrak;
		// MessageDlg("���� ������ �������� � �������: " + String(X502_GetErrorString(threadX502->err)), mtError, TMsgDlgButtons() << mbOK, NULL);
	}
	else {
		wwProgramSettings->colorMSG = wwProgramSettings->colorSelZone;
	}
	threadRunning = false;
	// TExtFunction::UpdateStatusBar(wwProgramSettings->gsStatusBar, strStatus, msg, wwProgramSettings->colorMSG);
	// TExtFunction::UpdateLabelStatus(wwProgramSettings->gsStatusLabel, strStatus, msg, wwProgramSettings->colorMSG);
}

// ------------------------------
// ����������� ��� � ��������� ����
int TWorkX502::StartRead(bool _enableSYNCDin, AnsiString &_msg) {
	int err = -1;
	if (threadX502) {
		if (threadRunning) {
			threadX502->stopThread = true;
			_msg += "����� ��� �������, �������������.";
			return err;
		}
		else {
			err = 0;
		}
	}
	else {
		// ���� ������ ���, �� �������� ���������.
		err = 0;
	}

	// X502_STREAM_ADC 0x01 ����� ������ �� ���
	if (err == X502_ERR_OK) {
		// ���������� ���������� ������� �� ����/�����.
		err = X502_StreamsEnable(hndX502, X502_STREAM_ADC);
	}
	else {
		//
	}
	// ��������� ���������� ���� � �������� ����� � ����������� �� _enableSYNCDin
	if (err == X502_ERR_OK) {
		err = SetSyncDinStream(_enableSYNCDin);
	}
	else {
		//
	}

	if (err == X502_ERR_OK) {

		// workData = new TWorkData(wwProgramSettings->maxZones, wwProgramSettings->countCrossSensors, wwProgramSettings->firstSensorsCross,
		// wwProgramSettings->firstSensorsThick, wwProgramSettings->countThickSensors, x502CountLOGch, wwProgramSettings->bufReadSize,wwProgramSettings);

		// �������� ������ ������
		threadX502 = new X502_ProcessThread(true, x502CountLOGch, x502AdcFreqLch, x502ADCBufSensorSize, w502TubeData, wwProgramSettings);
		threadX502->threadHndX502 = hndX502;
		for (int i = 0; i < x502CountLOGch; i++) {
			threadX502->treadADCdouble0[i] = &kadr_data0[i];
			// threadX502->treadADCdouble1[i] = &kadr_data1[i];
			// threadX502->treadADCdouble2[i] = &kadr_data2[i];
			// threadX502->treadADCdouble3[i] = &kadr_data3[i];
			// threadX502->treadADCdouble4[i] = &kadr_data4[i];
		}
		// (uint32_t)
		threadX502->threadSyncDIN0 = &syncDIN0;
		// ������������� ������� �� ������� ���������� ������ (� ���������
		// ����� ���������, ���� ����� ���������� ��� ��-�� ������ ��� �����
		// ������)
		threadX502->OnTerminate = OnThreadTerminate;
		threadX502->Resume(); // ������ ������
		threadRunning = true;
		// updateControls();
	}
	_msg = "";
	_msg += "����� �������";
	return err;
}

// ---------------------------------------------------------------------------
void __fastcall TWorkX502::StopThread() {
	// /* ������������� ������ �� ���������� ������ */
	if (threadRunning) {
		threadX502->stopThread = true;
		// wwProgramSettings->needData=false;
	}
	else {
		//
	}
}

// ----------------------------------------------
// ���������� � �����
int TWorkX502::GetDevInfo(void) {
	int result = -1;
	t_x502_info info;
	int32_t err = X502_GetDevInfo(hndX502, &info);
	if (err == X502_ERR_OK) {
		// chkDacPresent->Checked = info.devflags & X502_DEVFLAGS_DAC_PRESENT;
		isDacPresent = info.devflags & X502_DEVFLAGS_DAC_PRESENT;
		// chkGalPresent->Checked = info.devflags & X502_DEVFLAGS_GAL_PRESENT;
		isGalPresent = info.devflags & X502_DEVFLAGS_GAL_PRESENT;
		isDspPresent = info.devflags & X502_DEVFLAGS_BF_PRESENT;
		isEthPresent = info.devflags & X502_DEVFLAGS_IFACE_SUPPORT_ETH;

		strPldaVer = IntToStr(info.plda_ver);
		strFpgaVer = IntToStr((info.fpga_ver >> 8) & 0xFF) + "." + IntToStr(info.fpga_ver & 0xFF);
		if (info.mcu_firmware_ver != 0) {
			strMcu_firmware_ver = IntToStr((int)(info.mcu_firmware_ver >> 24) & 0xFF) + "." + IntToStr((int)(info.mcu_firmware_ver >> 16) & 0xFF) + "." +
				IntToStr((int)(info.mcu_firmware_ver >> 8) & 0xFF) + "." + IntToStr((int)info.mcu_firmware_ver & 0xFF);
		}
		else {
			strMcu_firmware_ver = "";
		}
		result = 0;
	}
	else {
		result = -2;
		MessageDlg("������ ��������� ���������� � ������: " + String(X502_GetErrorString(err)), mtError, TMsgDlgButtons() << mbOK, NULL);

		// X502_Close(hndX502);
		// X502_Free(hndX502);
		// hndX502 = NULL;
	}
	return result;
}

// ----------------------------------------------
// ������� �������� ���������� � ��������� ������� ��� �������� ���
// � ��� ������ 0 - ������ ����
int TWorkX502::X502OpenClose(int _numDev, AnsiString &_msg) {
	int err = -1;
	try {
		// /* ���� ���������� ���� ������� - ������� ����� ��������� � ��������� ��� */
		if (hndX502 == NULL) {
			int idx = _numDev;
			if (idx >= 0) {
				hndX502 = X502_Create();
				if (hndX502 == NULL) {
					err = -2;
					_msg = "������ �������� ��������� ������";
					MessageDlg(_msg, mtError, TMsgDlgButtons() << mbOK, NULL);
				}
				else {
					// ��������� ������, ��������������� ������ � ��������� ��������
					err = X502_OpenByDevRecord(hndX502, &devrecList[idx]);
					if (err != X502_ERR_OK) {
						err = -3;
						_msg = "������ �������� ������: " + String(X502_GetErrorString(err));
						MessageDlg(_msg, mtError, TMsgDlgButtons() << mbOK, NULL);
						// throw new Exception("������ �������� ������!!!");
						// X502_Free(hndX502);
						// hndX502 = NULL;
					}
					else {
						// showDevInfo();
						// workX502.isX502Opened=true;
						_msg = "������ ������: " + String(X502_GetErrorString(err));
						err = 0;
					}
				}
			}
		}
		else {
			// ���� ���������� ���� ������� - ���������
			// �������� ����� � �������
			X502_Close(hndX502);
			// ������������ ���������
			X502_Free(hndX502);
			hndX502 = NULL;
			isX502Opened = false;
			err = -11;
			_msg = "���������� ���� ������� - ���������";
		}
		// updateControls();
		// err = 0;
	}
	catch (Exception *ex) {
		Application->MessageBoxW(ex->Message.c_str(), L"������!", MB_ICONERROR);
		TDateTime currentDT;
		AnsiString strCurrentDT = FormatDateTime("yyyy.mm.dd hh:mm:ss", currentDT.CurrentDateTime());

	}
	return err;
}

// ----------------------------------------------
// �������� IP �� ���������� �������
int TWorkX502::ParseIpAddr(AnsiString _addr) {
	int a[4], i;
	if (sscanf(_addr.c_str(), "%d.%d.%d.%d", &a[0], &a[1], &a[2], &a[3]) != 4) {
		throw new Exception("�������� ������ IP-������!!");
	}

	for (i = 0; i < 4; i++) {
		if ((a[i] < 0) || (a[i] > 255)) {
			throw new Exception("���������������� IP-�����!!");
		}
	}

	return (a[0] << 24) | (a[1] << 16) | (a[2] << 8) | a[3];
}

// ----------------------------------------------
// ��������� �� IP
void TWorkX502::OpenByIP(AnsiString _strIP) {
	t_x502_devrec devrec;
	int32_t err = E502_MakeDevRecordByIpAddr(&devrec, ParseIpAddr(_strIP), 0, 5000);
	if (err != X502_ERR_OK) {
		MessageDlg("������ �������� ������: " + String(X502_GetErrorString(err)), mtError, TMsgDlgButtons() << mbOK, NULL);
	}
	else {
		hndX502 = X502_Create();
		if (hndX502 == NULL) {
			MessageDlg("������ �������� ��������� ������", mtError, TMsgDlgButtons() << mbOK, NULL);
		}
		else {
			/* ������������� ����� �� ��������� ������ */ int32_t err = X502_OpenByDevRecord(hndX502, &devrec);
			if (err != X502_ERR_OK) {
				MessageDlg("������ �������� ������: " + String(X502_GetErrorString(err)), mtError, TMsgDlgButtons() << mbOK, NULL);
				X502_Free(hndX502);
				hndX502 = NULL;
			}
			else {
				// showDevInfo();
			}
		}
	}
	// updateControls();
}

// ---------------------------------------------------------------------------

void TWorkX502::CloseDevice(AnsiString &_msg) {
	if (hndX502 != NULL) {
		/* ���� ������� ����� ����� ������ - �� ������������� ��� */
		if (threadRunning) {
			threadX502->stopThread = 1;
			threadX502->WaitFor();
		}
		/* �������� ����� � ������� */ X502_Close(hndX502);
		/* ������������ ��������� */ X502_Free(hndX502);
		hndX502 = NULL;
	}
}
// ---------------------------------------------------------------------------

// ������������ ������ �� �������� �����
int TWorkX502::SetAsyncDigOut(int _AsyncDigOut, AnsiString &_msg) {
	int err = -1;
	if (hndX502 != NULL) {
		// ��������� ������ ��� �������� ������
		int mask = 0x0000;
		err = X502_AsyncOutDig(hndX502, _AsyncDigOut, mask);
		if (err != X502_ERR_OK) {
			_msg = "������ ������������ ������ �� �������� �����: " + String(X502_GetErrorString(err));
		}
	}
	return err;
}

// ---------------------------------------------------------------------------
// ������������� ���-�� ���������� �������, ����� ��������� ��������� ������� �� �����
int TWorkX502::AdcFreqChange(int _ChCnt, AnsiString &_msg) {
	int err = -1;
	if (hndX502 != NULL) {
		err = X502_SetLChannelCount(hndX502, x502CountLOGch);
		if (err == X502_ERR_OK) {
			err = SetAdcFreq(x502AdcFreq, x502AdcFreqLch, x502DinFreq);
		}
		else {
			err = -2;
			_msg = "TWorkX502::adcFreqChange ������ ��������� ������ X502_SetLChannelCount" + String(X502_GetErrorString(err));
			TLog::SaveStrLog(_msg);
		}
		if (err != X502_ERR_OK) {
			err = -3;
			_msg = "TWorkX502::adcFreqChange ������ ��������� ������� �����: " + String(X502_GetErrorString(err));
			TLog::SaveStrLog(_msg);
			// MessageDlg(msg, mtError, TMsgDlgButtons() << mbOK, NULL);

		}
		else {
			//
		}

	}
	return err;
}

// ---------------------------------------------------------------------------
// ����������/������� ����������� �����
int TWorkX502::SetSyncDin(AnsiString &_msg) {
	int err = -1;
	if (hndX502 != NULL) {
		err = SetSyncDinStream(x502EnableSyncDin);
		if (err != X502_ERR_OK) {
			_msg = "TWorkX502::setSyncDin ������ ����������/������� ����������� �����: " + String(X502_GetErrorString(err));
			TLog::SaveStrLog(_msg);
			// MessageDlg(msg, mtError, TMsgDlgButtons() << mbOK, NULL);
		}
	}
	return err;
}

// ---------------------------------------------------------------------------
// ������������ ����� � �������� �����
int TWorkX502::GetAsyncDigIn(AnsiString &_msg) {
	int err = -1;
	if (hndX502 != NULL) {
		uint32_t din;
		err = X502_AsyncInDig(hndX502, &din);
		if (err != X502_ERR_OK) {
			_msg = "������ ������������ ����� � �������� �����: " + String(X502_GetErrorString(err));
			TLog::SaveStrLog(_msg);
			// MessageDlg(msg, mtError, TMsgDlgButtons() << mbOK, NULL);
			return err;
		}
		else {
			err = (int)din;
		}
	}
	return err;
}

// ---------------------------------------------------------------------------
// ������ �� ���
int TWorkX502::SetAsyncDac1(double _valOut, AnsiString &_msg) {
	int err = -1;
	try {

		if (_valOut < 0 || _valOut > 5) {
			err = -2;
			_msg = "TWorkX502::setAsyncDac1 ����������  ������ ���� � ��������� �� 0 �� 5 �����!";
			TLog::SaveStrLog(_msg);
			// MessageDlg(msg, mtError, TMsgDlgButtons() << mbOK, NULL);
			return err;
		}
		else {
			// err=0;
		}
		if (hndX502 != NULL) {
			double val = _valOut;
			err = X502_AsyncOutDac(hndX502, X502_DAC_CH1, val, X502_DAC_FLAGS_CALIBR | X502_DAC_FLAGS_VOLT);
			if (err != X502_ERR_OK) {
				AnsiString _msg = "TWorkX502::setAsyncDac1  ������ ������ �� ��� 1: " + String(X502_GetErrorString(err));
				TLog::SaveStrLog(_msg);
				// MessageDlg(msg, mtError, TMsgDlgButtons() << mbOK, NULL);
				// err = -3;
			}
		}
		else {
			err = -3;
		}
	}
	catch (Exception *ex) {
		TLog::ErrFullSaveLog(ex);
		err = -33;
		// TPr::prWrite(strCurrentDT+AnsiString("PA: ") + ex->Message)
	}
	return err;
}
// ---------------------------------------------------------------------------

int TWorkX502::SetAsyncDac2(double _valOut, AnsiString &_msg) {
	int err = -1;
	try {
		if (_valOut < 0 || _valOut > 5) {
			err = -2;
			_msg = "TWorkX502::setAsyncDac2 ����������  ������ ���� � ��������� �� 0 �� 5 �����!";
			return err;
		}
		else {
			//
		}
		if (hndX502 != NULL) {
			double val = _valOut;
			err = X502_AsyncOutDac(hndX502, X502_DAC_CH2, val, X502_DAC_FLAGS_CALIBR | X502_DAC_FLAGS_VOLT);
			if (err != X502_ERR_OK) {
				_msg = "TWorkX502::setAsyncDac2 ������ ������ �� ��� 2 : " + String(X502_GetErrorString(err));
				// err = -3;
			}
		}
		else {
			err = -3;
		}
	}
	catch (Exception *ex) {
		TLog::ErrFullSaveLog(ex);
		err = -33;
		// TPr::prWrite(strCurrentDT+AnsiString("PA: ") + ex->Message)
	}
	return err;
}

// ---------------------------------------------------------------------------
// �������� ���� � �������
int TWorkX502::GetAsyncAdcFrame(AnsiString &_msg) {
	int err = -1;
	uint32_t lch_cnt;
	if (hndX502 != NULL) {
		err = X502_GetLChannelCount(hndX502, &lch_cnt);
		if (x502FLAGS_VOLT == 1) {
			err = X502_AsyncGetAdcFrame(hndX502, X502_PROC_FLAGS_VOLT, 100, kadr_data0);
		}
		else {
			err = X502_AsyncGetAdcFrame(hndX502, 0, 100, kadr_data0);
		}
		if (err != X502_ERR_OK) {
			// err = -2;
			_msg = " ������ ������ ����� ��� : " + String(X502_GetErrorString(err));
		}
		else {
			err = X502_ERR_OK;
		}
	}
	else {
		err = -2;
	}
	return err;
}

// ---------------------------------------------------------------------------
void TWorkX502::InfDevListChange() {
	// ��������
	// refreshDeviceList();
}
// ---------------------------------------------------------------------------

void TWorkX502::ChCntChange(int _ChCnt) {
	// ��������
}

// ---------------------------------------------------------------------------

void TWorkX502::LoadX502Settings(AnsiString _pathUDL, AnsiString &_msg) {
	// �������� ��������� X502

	try {
		if (DataModuleDB->ADOConnectionDB->Connected) {
			//
		}
		else {
			DataModuleDB->ADOConnectionDB->ConnectionString = "FILE NAME=" + _pathUDL;
			DataModuleDB->ADOConnectionDB->Open();
		}
		AnsiString strSql = "";
		strSql = "SELECT x502AdcFreq,x502DinFreq,x502AdcFreqLch,x502ChMode,x502CountFIZch,x502CountLOGch,";
		strSql += "x502EnableSyncADC,x502SyncMode,x502SyncStartMode,x502EnableSyncDin,x502FLAGS_VOLT,x502ADC_RANGE_V,x502ADC_RANGE_INT,x502ADCBufSensorSize,x502DinBufSize";
		for (int i = 0; i < 32; i++) {
			strSql += ",x502ADC_RANGE_V" + IntToStr(i);
		}
		// ���� ��� � �������� � ������ ���� ������ ���� ������
		strSql += " FROM SettingsX502 where rec_id=1";
		DataModuleDB->ADOQueryDB->Close();
		DataModuleDB->ADOQueryDB->SQL->Text = strSql;
		DataModuleDB->ADOQueryDB->Open();

		while (!DataModuleDB->ADOQueryDB->Eof) {
			x502AdcFreq = DataModuleDB->ADOQueryDB->FieldByName("x502AdcFreq")->AsInteger;
			x502DinFreq = DataModuleDB->ADOQueryDB->FieldByName("x502DinFreq")->AsInteger;
			x502AdcFreqLch = 9999; // DataModuleDB->ADOQueryDB->FieldByName("x502AdcFreqLch")->AsInteger;  ���� ��� ������ � ���������� ����������, ����������� ������ ������
			x502ChMode = DataModuleDB->ADOQueryDB->FieldByName("x502ChMode")->AsInteger;
			x502CountFIZch = DataModuleDB->ADOQueryDB->FieldByName("x502CountFIZch")->AsInteger;
			x502CountLOGch = DataModuleDB->ADOQueryDB->FieldByName("x502CountLOGch")->AsInteger;
			x502EnableSyncADC = DataModuleDB->ADOQueryDB->FieldByName("x502EnableSyncADC")->AsInteger;
			x502SyncMode = DataModuleDB->ADOQueryDB->FieldByName("x502SyncMode")->AsInteger;
			x502SyncStartMode = DataModuleDB->ADOQueryDB->FieldByName("x502SyncStartMode")->AsInteger;
			x502EnableSyncDin = (bool)DataModuleDB->ADOQueryDB->FieldByName("x502EnableSyncDin")->AsInteger;
			x502FLAGS_VOLT = DataModuleDB->ADOQueryDB->FieldByName("x502FLAGS_VOLT")->AsInteger;
			// x502ADC_RANGE_V = DataModuleDB->ADOQueryDB->FieldByName("x502ADC_RANGE_V")->AsInteger;
			// x502ADC_RANGE_INT = DataModuleDB->ADOQueryDB->FieldByName("x502ADC_RANGE_INT")->AsInteger;
			x502CountFIZch = DataModuleDB->ADOQueryDB->FieldByName("x502CountFIZch")->AsInteger;
			x502ADCBufSensorSize = DataModuleDB->ADOQueryDB->FieldByName("x502ADCBufSensorSize")->AsInteger;
			x502DinBufSize = DataModuleDB->ADOQueryDB->FieldByName("x502DinBufSize")->AsInteger;
			for (int i = 0; i < 32; i++) {
				AnsiString fn = "x502ADC_RANGE_V" + IntToStr(i);
				AnsiString rstr = DataModuleDB->ADOQueryDB->FieldByName("x502ADC_RANGE_V" + IntToStr(i))->AsString;
				SetADCRange(DataModuleDB->ADOQueryDB->FieldByName("x502ADC_RANGE_V" + IntToStr(i))->AsString, i);
			}
			DataModuleDB->ADOQueryDB->Next();
		}
		DataModuleDB->ADOQueryDB->Close();
		if (x502CountFIZch < x502CountLOGch) {
			_msg = "���������� ������� ������ ��� ����������!!!";
			MessageDlg(_msg, mtError, TMsgDlgButtons() << mbOK, NULL);
		}
		else {

		}
	}
	catch (Exception *ex) {
		TLog::ErrFullSaveLog(ex);

		// TPr::prWrite(strCurrentDT+AnsiString("PA: ") + ex->Message)
	}

}

// ---------------------------------------------------------------------------
void TWorkX502::SaveX502Settings(AnsiString _pathUDL, AnsiString &_msg) {
	// ������� ���������
	try {
		if (DataModuleDB->ADOConnectionDB->Connected) {
			//
		}
		else {
			DataModuleDB->ADOConnectionDB->ConnectionString = "FILE NAME=" + _pathUDL;
			DataModuleDB->ADOConnectionDB->Open();
		}
		// ��� ��������� ������ � ������ ������ ������� ������ ���� � �������� �� ������
		AnsiString strSQL = "";
		strSQL = "UPDATE SettingsX502 set";
		strSQL += " x502AdcFreq=:x502AdcFreq,";
		strSQL += " x502DinFreq=:x502DinFreq,";
		strSQL += " x502AdcFreqLch=:x502AdcFreqLch,";
		strSQL += " x502ChMode=:x502ChMode,";
		strSQL += " x502CountFIZch=:x502CountFIZch,";
		strSQL += " x502CountLOGch=:x502CountLOGch,";
		strSQL += " x502EnableSyncADC=:x502EnableSyncADC,";
		strSQL += " x502SyncMode=:x502SyncMode,";
		strSQL += " x502SyncStartMode=:x502SyncStartMode,";
		strSQL += " x502EnableSyncDin=:x502EnableSyncDin,";
		strSQL += " x502FLAGS_VOLT=:x502FLAGS_VOLT,";
		strSQL += " x502ADC_RANGE_V=:x502ADC_RANGE_V,";
		strSQL += " x502ADC_RANGE_INT=:x502ADC_RANGE_INT";

		// strSQL += " ,parCountCross=:parCountCross";
		// strSQL += " ,parCountThickness=:parCountThickness";
		strSQL += " where REC_ID=1";
		DataModuleDB->ADOQueryDB->SQL->Text = strSQL;
		// DataModuleDB->ADOQueryDB->Parameters->ParamByName("x502AdcFreq")->Value = (int)x502AdcFreq;
		// DataModuleDB->ADOQueryDB->Parameters->ParamByName("x502DinFreq")->Value = (int)x502DinFreq;
		// DataModuleDB->ADOQueryDB->Parameters->ParamByName("x502AdcFreqLch")->Value = (int)x502AdcFreqLch;
		// DataModuleDB->ADOQueryDB->Parameters->ParamByName("x502ChMode")->Value = x502ChMode;
		// DataModuleDB->ADOQueryDB->Parameters->ParamByName("x502CountFIZch")->Value = x502CountLOGch;
		// DataModuleDB->ADOQueryDB->Parameters->ParamByName("x502CountLOGch")->Value = x502CountFIZch;
		DataModuleDB->ADOQueryDB->Parameters->ParamByName("x502EnableSyncADC")->Value = x502EnableSyncADC;
		// DataModuleDB->ADOQueryDB->Parameters->ParamByName("x502SyncMode")->Value = x502SyncMode;
		// DataModuleDB->ADOQueryDB->Parameters->ParamByName("x502SyncStartMode")->Value = x502SyncStartMode;
		// DataModuleDB->ADOQueryDB->Parameters->ParamByName("x502EnableSyncDin")->Value = x502EnableSyncDin;
		// DataModuleDB->ADOQueryDB->Parameters->ParamByName("x502FLAGS_VOLT")->Value = x502FLAGS_VOLT;
		DataModuleDB->ADOQueryDB->Parameters->ParamByName("x502FLAGS_VOLT")->Value = 1; // ������ � �������
		// DataModuleDB->ADOQueryDB->Parameters->ParamByName("x502ADC_RANGE_V")->Value = x502ADC_RANGE_V;
		DataModuleDB->ADOQueryDB->Parameters->ParamByName("x502ADC_RANGE_V")->Value = 0; // �������� �� 10 �����
		DataModuleDB->ADOQueryDB->Parameters->ParamByName("x502ADC_RANGE_INT")->Value = 0; // ���� �� ����������
		DataModuleDB->ADOQueryDB->ExecSQL();
		DataModuleDB->ADOQueryDB->Close();
	}

	catch (Exception *ex) {
		TLog::ErrFullSaveLog(ex);
		wwProgramSettings->colorMSG = wwProgramSettings->colorBrak;
		TExtFunction::UpdateStatusBar(wwProgramSettings->gsStatusBar, strStatus, ex->Message, wwProgramSettings->colorBrak);
		TExtFunction::UpdateLabelStatus(wwProgramSettings->gsStatusLabel, strStatus, _msg, wwProgramSettings->colorMSG);
		MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);
	}
}

// ---------------------------------------------------------------------------

// ��������� �������� ������ � 1
void TWorkX502::SetDITo1(int _typeCard, bool _lowBytes, bool _hiBytes) {
	if (hndX502 != NULL) {
		int32_t err;
		// �� E502 �� �������������!
		// ���� ��� E502, �� �������� � �������
		if (_typeCard == 1 || _typeCard == 2) {
			MessageDlg(" ������ �������� ������ �<1> : �� E502 �� ������������� !", mtError, TMsgDlgButtons() << mbOK, NULL);
			return;
		}
		else {
			err = X502_SetDigInPullup(hndX502, X502_PULLUPS_DI_H);
			err = X502_SetDigInPullup(hndX502, X502_PULLUPS_DI_L);
		}

		if (err != X502_ERR_OK) {
			MessageDlg(" ������ �������� ������ �<1> : " + String(X502_GetErrorString(err)), mtError, TMsgDlgButtons() << mbOK, NULL);
		}
		else {
			MessageDlg(" �������� ������ �<1>����������� ", mtInformation, TMsgDlgButtons() << mbOK, NULL);
		}
	}

}

int TWorkX502::SetAsyncDac(int _numADC, double _valOut, AnsiString &_msg) {
	int err = -1;
	if (_valOut < 0 || _valOut > 5) {
		err = -2;
		MessageDlg("������ ������ �� ��� " + IntToStr(_numADC) + ": ����������  ������ ���� � ��������� �� 0 �� 5 �����!", mtError, TMsgDlgButtons() << mbOK, NULL);
		return err;
	}
	else {
		//
	}
	if (hndX502 != NULL) {
		double val = _valOut;
		err = X502_AsyncOutDac(hndX502, (_numADC - 1), val, X502_DAC_FLAGS_CALIBR | X502_DAC_FLAGS_VOLT);
		if (err != X502_ERR_OK) {
			_msg = " ������ ������ �� ��� " + IntToStr(_numADC) + " : " + String(X502_GetErrorString(err));
			// err = -3;
		}
	}
	else {
		err = -3;
	}
	return err;
}

int TWorkX502::X502GetRefFreqValue(double _freq) {
	int err = -1;
	err = X502_GetRefFreqValue(hndX502, &_freq);
	return err;
}

int TWorkX502::X502CreatePrepareWorkData(AnsiString &_msg) {
	int err = -1;
	try {
		if (!wwProgramSettings) {
			// ���� ��� �������� ���, �� ���� ������ ������
			_msg = "���� ��� �������� ���, �� ���� ������ ������";
			MessageDlg(_msg, mtError, TMsgDlgButtons() << mbOK, NULL);
			err = -2;
			return err;
		}
		else {
			//
		}
		if (wwProgramSettings->loadPRGSettings(applPath + "\\connectSQL.udl") < 0) {
			err = -3;
			_msg = "�� ������� ��������� ����������� ���������";
			MessageDlg(_msg, mtError, TMsgDlgButtons() << mbOK, NULL);
			return err;
		}
		else {
			//

		}
		// int _countZones, int _firstSensorsThick, int _countSensorsThick,   int _firstSensorsCross, int _countSensorsCross, int _wdCountLOGchX502, int _wdFullSensorSize
		//workData = new TWorkData(wwProgramSettings->maxZones, wwProgramSettings->firstSensorsThick, wwProgramSettings->countThickSensors, wwProgramSettings->firstSensorsCross,
		//	wwProgramSettings->countCrossSensors, x502CountLOGch, x502ADCBufSensorSize, wwProgramSettings, adc_Range_Value);
		_msg = "���!";
		err = 0;
	}
	catch (Exception *ex) {
		err = -2;
		TLog::ErrFullSaveLog(ex);
		wwProgramSettings->colorMSG = wwProgramSettings->colorBrak;
		TExtFunction::UpdateStatusBar(wwProgramSettings->gsStatusBar, strStatus, ex->Message, wwProgramSettings->colorMSG);
		TExtFunction::UpdateLabelStatus(wwProgramSettings->gsStatusLabel, strStatus, ex->Message, wwProgramSettings->colorMSG);
		MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);

	}
	return err;
}

int TWorkX502::GetVoltMaxLCh(int _numLCh) {
	// /** ��������� ��������� ��� ������ ��� */
	// typedef enum {
	// X502_ADC_RANGE_10 = 0,
	// /** < �������� +/-10V */
	// X502_ADC_RANGE_5 = 1,
	// /** < �������� +/-5V */
	// X502_ADC_RANGE_2 = 2,
	// /** < �������� +/-2V */
	// X502_ADC_RANGE_1 = 3,
	// /** < �������� +/-1V */
	// X502_ADC_RANGE_05 = 4,
	// /** < �������� +/-0.5V */
	// X502_ADC_RANGE_02 = 5
	// /** < �������� +/-0.2V */
	// } t_x502_adc_range;
	int err = -1;
	try {
		// -
		// t_x502_adc_range[0];
		err = 0;
	}
	catch (Exception *ex) {
		err = -2;
		TLog::ErrFullSaveLog(ex);
		// TExtFunction::UpdateStatusBar(StatusBarBottom, strStatus, ex->Message, programSettings.colorBrak);
		// MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);

	}
	return err;
}

int TWorkX502::StartDrive(double _speed, int _codeDOutDrive, AnsiString &_msg) {
	int err = -1;
	// �������� ��������
	err = SetAsyncDac2(_speed, _msg);
	// ������ ���������
	err = SetAsyncDigOut(_codeDOutDrive, _msg);
	Sleep(200);
	// �������� ������
	err = SetAsyncDigOut(0, _msg);
	return err;
}

int TWorkX502::StopDrive(int _codeDOut, AnsiString &_msg) {
	int err = -1;
	// ������� ��������
	err = SetAsyncDac2(0, _msg);
	// ���� ���������
	err = SetAsyncDigOut(_codeDOut, _msg);
	Sleep(200);
	// �������� ������
	err = SetAsyncDigOut(0, _msg);
	// �������� ����������
	Sleep(100);
	return err;
}

int TWorkX502::AlarmSituiation(AnsiString &_msg) {
	int err = -1;
	if (threadRunning) {
		Sleep(100);
		StopThread();
	}
	else {
		//
	}
	if (threadRunning) {
		threadX502->Terminate();
		threadX502->WaitFor();
	}
	else {
		//
	}
	// ������ ����� � �������
	X502_StreamsStop(hndX502);
	err = SetAsyncDac1(0, _msg);
	err = SetAsyncDac2(0, _msg);
	// ������ ������ ������
	err = SetAsyncDigOut(1, _msg);
	Sleep(100);
	// �������� ������
	err = SetAsyncDigOut(0, _msg);
	// �������� ����������
	Sleep(100);
	return err;
}

TWorkX502::SetADCRange(AnsiString _strRange, int _ind) {
	try {
		adc_Range[_ind] = X502_ADC_RANGE_5;
		adc_Range_Value[_ind] = 5.0;
		if (_strRange == "+/-10V") {
			adc_Range[_ind] = X502_ADC_RANGE_10;
			adc_Range_Value[_ind] = 10.0;
		}
		else {

		};
		if (_strRange == "+/-5V") {
			adc_Range[_ind] = X502_ADC_RANGE_5;
			adc_Range_Value[_ind] = 5.0;
		}
		else {

		};
		if (_strRange == "+/-2V") {
			adc_Range[_ind] = X502_ADC_RANGE_2;
			adc_Range_Value[_ind] = 2.0;
		}
		else {

		};
		if (_strRange == "+/-1V") {
			adc_Range[_ind] = X502_ADC_RANGE_1;
			adc_Range_Value[_ind] = 1;
		}
		else {

		};
		if (_strRange == "+/-0,5V") {
			adc_Range[_ind] = X502_ADC_RANGE_05;
			adc_Range_Value[_ind] = 0.5;
		}
		else {

		};
		if (_strRange == "+/-0,2V") {
			adc_Range[_ind] = X502_ADC_RANGE_02;
			adc_Range_Value[_ind] = 0.2;
		}
		else {

		};
	}
	catch (Exception *ex) {
		TLog::ErrFullSaveLog(ex);
		// vtProgramSettings->colorMSG = vtProgramSettings->colorBrak;
		// TExtFunction::UpdateStatusBar(vtProgramSettings->gsStatusBar, strStatus, ex->Message, vtProgramSettings->colorMSG);
		// TExtFunction::UpdateLabelStatus(vtProgramSettings->gsStatusLabel, strStatus, ex->Message, vtProgramSettings->colorMSG);
	}

}

int TWorkX502::GetADCRange(int _ind) {
	return adc_Range[_ind];
}

double TWorkX502::GetADCRangeValue(int _ind) {
	return adc_Range_Value[_ind];
}

// ����� �������
int TWorkX502::SYNK_X502StreamsStart(t_x502_hnd _hndX502) {
	int err = -1;
	err = X502_StreamsStart(_hndX502);
	return err;
}

// ���� �������
int TWorkX502::SYNK_X502StreamsStop(t_x502_hnd _hndX502) {
	int err = -1;
	err = X502_StreamsStop(_hndX502);
	return err;
}

// -------------------
// ����������� �������� �����
int TWorkX502::GetAsyncValuesDigIn(int _valuesIN[32], AnsiString &_msg) {
	int err = -1;
	// ����������� �������� �����
	try {
		try {
			int DIN = GetAsyncDigIn(msgLast);
			for (int i = 0; i < 16; i++) {
				if (DIN == 0) {
					break;
				}
				else {
					int res = DIN & 0x0001;
					if (res == 1) {
						_valuesIN[i] = 1;
						// ������������� ��������������� �������� �����
						// pWorkX502->setAsyncDigOut(i);
					}
					else {
						_valuesIN[i] = 0;
						// ���������� ��������������� �������� �����
						// pWorkX502->setAsyncDigOut(0);
					};
					// ��������� ������� ������ ���
					DIN = DIN >> 1;
				}

			}
			err = 0;
		}
		catch (Exception *ex) {
			err = -2;
			TLog::ErrFullSaveLog(ex);
			// TExtFunction::UpdateStatusBar(StatusBarBottom, strStatus, ex->Message, programSettings.colorBrak);
			// MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);

		}
	}
	__finally {
		// _msg = "��������";
	}
	return err;
}

// -----------------
// �������� ������ �������� �������
int TWorkX502::SetAsyncValuesDigOUT(int _valuesOUT[32], AnsiString &_msg) {
	int err = -1;
	int val = 0;
	try {
		for (int i = 0; i < 32; i++) {
			// x = pow(base, exp), ��� base - ��������, exp - ������� ����������
			if (_valuesOUT[i] == 1) {
				val = val + (int)pow(2.0, i);
			}
			else {
				//
			}
		}
		// ������������ ������ �� �������� �����
		err = SetAsyncDigOut(val, _msg);
	}
	catch (Exception *ex) {
		err = -55;
		TLog::ErrFullSaveLog(ex);

		// TPr::prWrite(strCurrentDT+AnsiString("PA: ") + ex->Message)
	}
	return err;
}

// ������� ������ ����������
int TWorkX502::ClearValuesDig(int _values[32]) {
	int err = -1;
	try {
		for (int i = 0; i < 32; i++) {
			_values[i] = 0;

		}
		err = 0;
	}
	catch (Exception *ex) {
		err = -56;
		TLog::ErrFullSaveLog(ex);
	}
	return err;
}

int TWorkX502::CheckCurrentDrive(int _numZone, int _sensDrvNum, int _firstValue, int _countValues, double _valueNominal, double _valueMax) {
	// int TWorkData::GetSensorArrayData(int _zoneNum, int _sensorNum,double* _arrayData, int _beginRead, int _needRead, AnsiString &_msg) {
	int err = -1;
	double diffVDrv = 0;
	double resSum = 0;
	double resAvg = 0;
	double* valueS = NULL;
	try {
		try {
			int sz = w502TubeData->vectZones[_numZone]->vectSensors[_sensDrvNum]->realSensorSize;
			valueS = new double[_countValues];
			for (int i = 0; i < sz; i++) {
				valueS[i] = 0;
			}
			w502TubeData->GetSensorArrayData(_numZone, _sensDrvNum, valueS, (sz - _countValues), _countValues, msgLast);
			for (int i = 0; i < _countValues; i++) {
				diffVDrv = abs(_valueNominal - abs(valueS[i]));
				resSum+=_valueNominal + diffVDrv;
			}
			resAvg = resSum / _countValues;
			if (resAvg > _valueMax) {
				err = -999; // ShowBigModalMessage("���������� ����!!! " +_driveName, clRed);
			}
			else {
				err = 0;
			}
		}
		catch (Exception *ex) {
			err = -2;
			TLog::SaveStrLog(ex->Message);
			// TExtFunction::UpdateStatusBar(StatusBarBottom, strStatus, ex->Message, programSettings.colorBrak);
		}
	}
	__finally {
		if (valueS) {
			delete valueS;
		}
		else {
			//
		}
	}
	return err;
}
