// ---------------------------------------------------------------------------

#pragma hdrstop

#include "TGlobalSettings.h"
// ���������� ���� �������� ������� ��� � ����� �����
// ---------------------------------------------------------------------------
#include "unSQLDbModule.h"
#include "unTUtils.h"

#pragma package(smart_init)

TGlobalSettings::TGlobalSettings() {
	// -----------
	workStatus = 0;
//	organizationName="������������ �����������";
//	serialTubeNum="�������� �����";
	//---------------
	//������ ��������� � ����� �������� ����

	WM_MsgReadyTstData= RegisterWindowMessage(L"WM_MsgReadyTstData");
	WM_MsgThread= RegisterWindowMessage(L"WM_threadMsg");
	int mainHandle=0;
};

TGlobalSettings::~TGlobalSettings() {
	// -------------
};

int TGlobalSettings::LoadPRGSettings(AnsiString _fullPathNameUDL) {
	// -------------
	AnsiString strSql;
	int err = -1;
	try {
		if (SqlDBModule->ADOConnectionDB->Connected) {
			SqlDBModule->ADOQueryDB->Close();
		}
		else {
			SqlDBModule->ADOConnectionDB->ConnectionString = "FILE NAME=" + _fullPathNameUDL;
			SqlDBModule->ADOConnectionDB->Open();
		}
		// ���������� ��������� ���������
		strSql = "SELECT REC_ID";
		strSql += ",isFilter,filterCurrentSubType,filterOrder";
		strSql += ",filterCutoffFrequency,filterCenterFrequency,filterWidthFrequency";
		strSql += ",filterRippleDb,filterRolloff,filterSampleRate";
		strSql += ",indexCurrentTypeSize,protocolInterval,protocolToFile,protocolVisible,zeroTransitionChannel";
		strSql += ",checkMuftaLevel,checkMuftaChannel,indexCurrentSGGost,calcSGMode,discrFrecGSPF";
		// ----------
		strSql += " FROM SettingsGlobal";
		SqlDBModule->ADOConnectionDB->Open();
		SqlDBModule->ADOQueryDB->SQL->Text = strSql;
		SqlDBModule->ADOQueryDB->Open();
		isFilter = (bool)SqlDBModule->ADOQueryDB->FieldByName("isFilter")->AsInteger;
		filterCurrentSubType = SqlDBModule->ADOQueryDB->FieldByName("filterCurrentSubType")->AsInteger;
		filterOrder = SqlDBModule->ADOQueryDB->FieldByName("filterOrder")->AsInteger;
		filterCutoffFrequency = SqlDBModule->ADOQueryDB->FieldByName("filterCutoffFrequency")->AsFloat;
		filterCenterFrequency = SqlDBModule->ADOQueryDB->FieldByName("filterCenterFrequency")->AsFloat;
		filterWidthFrequency = SqlDBModule->ADOQueryDB->FieldByName("filterWidthFrequency")->AsFloat;
		filterRippleDb = SqlDBModule->ADOQueryDB->FieldByName("filterRippleDb")->AsFloat;
		filterRolloff = SqlDBModule->ADOQueryDB->FieldByName("filterRolloff")->AsFloat;
		filterSampleRate = SqlDBModule->ADOQueryDB->FieldByName("filterSampleRate")->AsFloat;
		indexCurrentTypeSize=SqlDBModule->ADOQueryDB->FieldByName("indexCurrentTypeSize")->AsInteger;
		indexCurrentSGGost=SqlDBModule->ADOQueryDB->FieldByName("indexCurrentSGGost")->AsInteger;
		protocolInterval=SqlDBModule->ADOQueryDB->FieldByName("protocolInterval")->AsInteger;
		protocolToFile=SqlDBModule->ADOQueryDB->FieldByName("protocolToFile")->AsInteger;
		protocolVisible=SqlDBModule->ADOQueryDB->FieldByName("protocolVisible")->AsInteger;
		zeroTransitionChannel=SqlDBModule->ADOQueryDB->FieldByName("zeroTransitionChannel")->AsInteger;
		checkMuftaChannel=SqlDBModule->ADOQueryDB->FieldByName("checkMuftaChannel")->AsInteger;
		checkMuftaLevel=(int)SqlDBModule->ADOQueryDB->FieldByName("checkMuftaLevel")->AsFloat;
		calcSGMode=SqlDBModule->ADOQueryDB->FieldByName("calcSGMode")->AsInteger;
		discrFrecGSPF=SqlDBModule->ADOQueryDB->FieldByName("discrFrecGSPF")->AsInteger;
		// -------------
		SqlDBModule->ADOQueryDB->Close();
		int err=0;
		AnsiString where="rec_id="+IntToStr(indexCurrentTypeSize);
		nameCurrentTypeSize=SqlDBModule->GetStrFieldSQL("TypeSizes","TSName",where,"���",err);
		WM_MsgReadyTstData = WM_USER + 99;
		err = 0;
	}
	catch (Exception *ex) {
		err = -2;
		TLog::ErrFullSaveLog(ex);
		MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);
	}
	return err;
}

