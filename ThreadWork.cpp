// ---------------------------------------------------------------------------

#pragma hdrstop

#include "ThreadWork.h"
#include <IOUtils.hpp>
#include <math.h>
// #include "TLCard791.h"
// #include "GSPF052.h"
#include "TSG.h"
#include "TProtocol.h"
#include <ctime.h>
#include "unTExtFunction.h"
//#include "TLogCh791Params.h"
#include "SignalListDef.h"
#include "unSQLDbModule.h"
#include "TypeSizeFrequencies.h"
#include "unTUtils.h"
// #include <boost\ratio\ratio.hpp>
// #include <chrono.hpp>
#include "DebugMess.h"
// ---------------------------------------------------------------------------
#ifdef BERLIN_10
//
#else
  #pragma package(smart_init)
#endif
 #ifndef TVIRTLCARD791
 #define DEBUG_LEN(txt, o)
 #else
  #define DEBUG_LEN(txt, o)\
  dprint("DEBUG_LEN %s %d\n", txt, o->vecMeasuresData[0].vecSensorsData[0].size())
 #endif
// ----����������� - ��������� ������� ���������� �� ����������---------------
__fastcall ThreadWork::ThreadWork(bool _createSuspended, TLCardData* _lCardData,TGlobalSettings* _globalSettings, TGSPF052* _gen)
{
	thGlobalSettings = _globalSettings;
	lCardData = _lCardData;
	int err=-101;
	GSPF = _gen;//new TGSPF052(thGlobalSettings,err);
#ifdef TVIRTLCARD791
	TLog::Load3TxtVectDoubleFile(
		"C:\\Users\\Andrey\\Desktop\\eclipce\\tmp\\SavedEtalons\\E_2018_03_26_12_12_07_55000_TS_73_SG_K_FHZ.csv"
		, lCardData,0);
	DEBUG_LEN("10", lCardData);
#endif
	solidGroup = new TSG(thGlobalSettings,lCardData);
	DEBUG_LEN("20", lCardData);
	Collect = true;
	// ini = _ini;
	// AnsiString sect = "Type_"; // + ini->ReadString("Default", "TypeSize", "1");
	// delay = 10; // = ini->ReadInteger("OtherSettings", "OnLineCycleDelay", 10);
	// pauseWorkSpeed = 1050; // ini->ReadInteger(sect, "PauseWorkSpeed", 1050);
	// pauseStop = 500; // ini->ReadInteger(sect, "PauseStop", 500);
	cs = new TCriticalSection();
	calc_event = new TEvent(NULL, false, false, "calc_event", true);
	// SGFix = _SGFix;

}

// ---------------------------------------------------------------------------
__fastcall ThreadWork::~ThreadWork(void) {
	Terminate();
	WaitFor();
	//delete treadADCdouble0;
	if (GSPF) {
		delete GSPF;
		GSPF=NULL;
	}
	if (solidGroup) {
		delete solidGroup;
		solidGroup=NULL;
	}
	delete calc_event;
	delete cs;
}

// -----������ ������ ������--------------------------------------------------
void __fastcall ThreadWork::Execute() {
	// NameThreadForDebugging("WorkOnlineThread",0xFFFF);
	SetStext1("����� \"������\"");
	SetStext2("������� � ��������� ���������");
	TProtocol::ProtocolSave("-----------");
	TProtocol::ProtocolSave(stext1);
	Post(UPDATE_STATUS);

	UnicodeString prepare_result = PrepareForWork();
	if (prepare_result != "ok") {
		SetStext1("����� \"������\" �� ��������!");
		SetStext2(prepare_result);
		TProtocol::ProtocolSave("������: " + stext2);
		Post(UPDATE_STATUS);
		Finally();
		Post(COMPLETE, 0);
		return;
	}
	TProtocol::ProtocolSave("������: ���������� ������ �������");
	bool ret = OnlineCycle();
	if (Terminated)
	{
		SetStext1("����� \"������\" �� ��������!");
		TProtocol::ProtocolSave("����� \"������\" �� ��������!");
		Post(UPDATE_STATUS);
		Finally();
		return;
	}
	Post(COMPUTE);
	TProtocol::ProtocolSave("���� ����������� �� Main");
	calc_event->WaitFor(INFINITE);
	calc_event->ResetEvent();
	if (Terminated) {
		Post(COMPLETE, 0);
		return;
	}
	TProtocol::ProtocolSave("�������� ����������");
	if (!ret) {
		SetStext1("����� \"������\" �� ��������!");
		TProtocol::ProtocolSave("����� \"������\" �� ��������!");
		Post(UPDATE_STATUS);
		Finally();
		Post(COMPLETE, 0);
		return;
	}
	Sleep(200);
	// ������ ������ � ��
//	SaveMuftToDB();
	// -----
	Post(COMPLETE, (LPARAM)1);
}

// ---------------------------------------------------------------------------
void ThreadWork::Post(WPARAM _w, LPARAM _l) {
  //PostMessage(, WM_USER + 101, 33, 0);
	if (PostMessage(Application->Handle, thGlobalSettings->threadMsg, _w, _l) == 0)
		TExtFunction::FATAL("ThreadOnLine::Post: �� ���� ������� ���������");
	AnsiString a = "ThreadOnLine::Post: ������� ";
	a += _w;
	a += " ";
	a += _l;
	TProtocol::ProtocolSave(a);
}

// ---------------------------------------------------------------------------
// -----���������� � ������
UnicodeString ThreadWork::PrepareForWork() {
	// ���������, ������� �� ������ ������ ���������
	// if (!lcard->GetDinValue("ON"))
	// return "�������� ������ ���������������!";
	//todo �������� �����
//	if (SLD->iINMODULE->Get())
//		SetStext2("������� ����� �� ���������!");
//	while (SLD->iINMODULE->Get()){
//		Sleep(3000);
//	   TExtFunction::ShowBigModalMessage("������� ����� �� ���������!",clRed);
//	}

//   	SetStext2("���� ����������");
	SetStext2("��������� ������");
	Post(UPDATE_STATUS);

	Post(NEXT_MUFT);
	return "ok";
}

// -------------------------------------------------------------------------------
// ----������ ����, ���������� ���������� � ����������� ��� �������---------------
#ifndef TVIRTLCARD791
 #define DEBUG_LEN(txt, o)
 #else
  #define DEBUG_LEN(txt, o)\
  dprint("DEBUG_LEN %s %d\n", txt, o->vecMeasuresData[0].vecSensorsData[0].size())
 #endif
bool ThreadWork::OnlineCycle() {
	TProtocol::ProtocolSave("������: ����� ������");
	bool result = true; // ����� ��������� ���������� �����
	AnsiString msg = "";
	// ���� �����
	bool timeFlag;
	timeFlag = CheckMufta(false,60000);
	if (!timeFlag) // ���� ��������� ����� ��������, �� �������
	{
		//��������� ������� ����� �������
		if (SLD->iCC->WasConst(false, 50))
			msg = "������: ������ ������ ���� ����������";
		else
			msg = "������: �� ��������� ���� ������ �����";
		SetStext2(msg);
		Post(UPDATE_STATUS);
//		TExtFunction::ShowBigModalMessage(msg,clBlue);
		TProtocol::ProtocolSave(msg);
		return false;
	}
#ifndef TVIRTLCARD791
	timeFlag = CheckMufta(true,60000);
	if (!timeFlag) // ���� ��������� ����� ��������, �� �������
	{
        //��������� ������� ����� �������
		if (SLD->iCC->WasConst(false, 50))
			msg = "������: ������ ������ ���� ����������";
		else
			msg = "������: �� ��������� ���� �������� �����";
		SetStext2(msg);
		Post(UPDATE_STATUS);
//		TExtFunction::ShowBigModalMessage(msg,clBlue);
		TProtocol::ProtocolSave(msg);
		return false;
	}
#endif
   DEBUG_LEN("30", lCardData);
	if (solidGroup!=NULL)
	{
		delete solidGroup;
		solidGroup = NULL;
	}
#ifndef TVIRTLCARD791
	lCardData->ClearSGM();
#endif
	TSFrequencies TSFreqs = TSFrequencies(thGlobalSettings->indexCurrentTypeSize);
	if(TSFreqs.Frequency.size() <= 0)
	{
		TProtocol::ProtocolSave("������: ������ ��� ����� ����������� ��� � ���� ������");
		TExtFunction::ShowBigModalMessage("������: ������ ��� ����� ����������� ��� � ���� ������",clBlue);
		return false;
	}
	 DEBUG_LEN("100", lCardData);
	solidGroup = new TSG(thGlobalSettings, lCardData);
	 DEBUG_LEN("110", lCardData);
	Sleep(1500); // �������� �� �������� �����
	DWORD LastTime = GetTickCount();
	SetStext2("�������� ����");
	Post(UPDATE_STATUS);
	 DEBUG_LEN("120", lCardData);
	// �������� ������� �������
	SLD->oSENSORON->Set(true);
#ifndef TVIRTLCARD791
	// �������� ����
	GSPF->SetSampleFreq(thGlobalSettings->discrFrecGSPF);
	GSPF->FormSignal(TSFreqs.Frequency[0],TSFreqs.Amplitude[0]);
	GSPF->StartGSPF052();
	Sleep(100);
	// ������ �����
	while (true) {
		if (solidGroup->Exec(0) == 2) {
			break;
		}
	}
	// ���������� ��������� �� � ������������� ����
	solidGroup->ResetState();
	GSPF->StopGSPF052();
#endif
	// ��������� ������� �������
	SLD->oSENSORON->Set(false);
	// SLD->oSENSLOWPOW->Set(false);
	SetStext2("��������� ����");
	Post(UPDATE_STATUS);
	//������������ ��
	int err = 0;
	csg = solidGroup->Get1FrecSG();
	csg.group = SqlDBModule->GetStrFieldSQL("SolidGroups","SGName"
		,"rec_id="+IntToStr(csg.group_id),"X",err);
	csg.color = (TColor)SqlDBModule->GetIntFieldSQL("SolidGroups","Color"
		,"rec_id="+IntToStr(csg.group_id),65535,err);
//	PanelSG->Caption = csg.group+" "+IntToStr((int)(csg.probability*100))+"%";
//	PanelSG->Color = csg.color;
	// ������� �� ����� �������
	Post(REDRAW);
	//------
	if (Terminated) // ���� ��������
	{
		Collect = false;
		result = false;
		TProtocol::ProtocolSave("������: ����� � Terminated");
		Finally();
	}
	TProtocol::ProtocolSave("OnlineCycle ��������");
	return result;
}

void ThreadWork::Finally() {
	// ������������� LCard
	lCardData->StopSGM();
	// ��������� ������� �������
	SLD->oSENSORON->Set(false);
}

void ThreadWork::ErrFinally(AnsiString _msg, bool* _Collect, bool* _result) {
	Finally();
	*_Collect = false;
	*_result = false;
	TProtocol::ProtocolSave(_msg);
	SetStext2(_msg);
	Post(UPDATE_STATUS);
}

// ---------------------------------------------------------------------------
AnsiString ThreadWork::GetStext1(void) {
	AnsiString ret;
	cs->Enter(); {
		ret = stext1;
	} cs->Leave();
	return (ret);
}

// ---------------------------------------------------------------------------
AnsiString ThreadWork::GetStext2(void) {
	AnsiString ret;
	cs->Enter(); {
		ret = stext2;
	} cs->Leave();
	return (ret);
}

// ---------------------------------------------------------------------------
void ThreadWork::SetStext1(AnsiString _s) {
	cs->Enter(); {
		stext1 = _s;
	} cs->Leave();
}

// ---------------------------------------------------------------------------
void ThreadWork::SetStext2(AnsiString _s) {
	cs->Enter(); {
		stext2 = _s;
	} cs->Leave();
}

// ---------------------------------------------------------------------------
void ThreadWork::SetCalc() // (bool _IsBrak)
{
	// IsBrak = _IsBrak;
	calc_event->SetEvent();
}

// ---------------------------------------------------------------------------
void ThreadWork::pr(AnsiString _msg) {
	TProtocol::ProtocolSave(_msg);
}

// ---------------------------------------------------------------------------
void ThreadWork::SaveMuftToDB() {
	AnsiString SQL;
	AnsiString ret;

	// CExecSQL* E;
	// ��������� ����� �������
	// CSelect S1("SELECT COUNT(TubeNum) as nn from dbo.TubesStat");
	// if (S1.AsInt("nn") > 60000)
	// {
	// // ������� ����� ������ ������
	// CSelect S2("Select top " + IntToStr(S1.AsInt("nn") - 60000)
	// + " * from dbo.TubesStat order by IDTube ASC");
	// while (!S2.isLast())
	// {
	// // ������� ������ ������ � ���� �������
	// SQL = "Delete from dbo.TubesStat where TubeNum=" + S2.AsInt("TubeNum");
	// E = new CExecSQL(SQL);
	// delete E;
	// S2.Next();
	// }
	// }

	// // ���� ��������� ����� �����
	// CSelect S3("Select top 1 * from dbo.TubesStat order by IDTube Desc");
	// // �������� ����� ����� �����
	// int TubeNum = 0;
	int TubeNum = 1; // S3.AsInt("TubeNum");
	if (TubeNum == 0 || TubeNum == 200000)
		TubeNum = 1;
	else
		TubeNum++;

	// ������� ������ ������ � ���� �������
	// SQL = "Delete from dbo.TubesStat where TubeNum=";
	// SQL += TubeNum;
	// E = new CExecSQL(SQL);
	// ret = E->IsOk();
	// delete E;
	// if (ret != "Ok") {
	// pr(AnsiString("SaveMuftToDB: ") + ret);
	// }

	// ������ ����� ������ �����
	// SQL = "insert into dbo.TubesStat (TubeNum,DateTime,TypeSize,SolidGroup) values(";
	// SQL += TubeNum;
	// SQL += ",'";
	// SQL += DateTimeToStr(Date() + Time());
	// SQL += "','";
	// SQL += ini->ReadString("Default", "TypeSize", "1");
	// SQL += "','";
	// SQL += csg.group;
	// SQL += "')";
	// pr(SQL);
	// E = new CExecSQL(SQL);
	// ret = E->IsOk();
	// delete E;
	// if (ret != "Ok") {
	// pr(AnsiString("SaveMuftToDB: ") + ret);
	// }
}

// ----------------------------------------------------------------------------
CSG ThreadWork::GetSG(void) {
	CSG ret;
	cs->Enter(); {
		ret = csg;
	} cs->Leave();
	return (ret);
}
// ----------------------------------------------------------------------------
void ThreadWork::SetSG(CSG _csg) {
	cs->Enter(); {
		csg = _csg;
	} cs->Leave();
}
// ---------------------------------------------------------------------------
// �������� ������� �����. ���������� true ���� �� ��������� ����� ������ ��������� �� ���������
bool ThreadWork::CheckMufta(bool _waitStatus, int _waitTime)
{
	// ���� �����
	bool timeFlag = false;
	DWORD StartTime = GetTickCount();
	if (_waitStatus)
		SetStext2("��������� ����� � ������!");
	else
		SetStext2("������� ����� �� �������!");
	Post(UPDATE_STATUS);

	TLCardData * muft�SearchData = new TLCardData(lCardData->GetLCard791(), 1 , lCardData->GetLCard791()->countLogCh);
	TSG* muft�Search = new TSG(thGlobalSettings, muft�SearchData);
	while (true) {
		while (true) {
			if (muft�Search->Exec(0) == 2)
				break;
		}
		muft�Search->ResetState();
		bool result = muft�SearchData->CheckMufta(thGlobalSettings->checkMuftaChannel);
		if (result == _waitStatus) {
			break;
		}
		muft�SearchData->ClearSGM();
		if (GetTickCount() - StartTime > _waitTime)
		{
			timeFlag = true;
			break;
		}
		//��������� ������� ����� �������
		if (SLD->iCC->WasConst(false, 50))
		{
			TProtocol::ProtocolSave("������: ������ ������ ���� ����������");
			timeFlag = true;
			SetStext2("������: ������ ������ ���� ����������");
			Post(UPDATE_STATUS);
			break;
		}
		// �������, �� ���� �� ������
		if (Terminated)
		{
			TProtocol::ProtocolSave("������: ����� � Terminated");
			timeFlag = true;
			break;
		}
	}
	delete muft�Search;
	muft�SearchData->ClearSGM();
	delete muft�SearchData;
	if (timeFlag) // ���� ��������� ����� ��������, �� false
		return false;
	else          // ���� ��������� ������� ������, �� true
		return true;
}
// ---------------------------------------------------------------------------

