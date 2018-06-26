// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ADCSettings.h"
#include "TLCardData.h"
#include "Filters.h"
#include "unSQLDbModule.h"
// ---------------------------------------------------------------------------
#ifdef BERLIN_10
//
#else
  #pragma package(smart_init)
#endif
#pragma resource "*.dfm"
TADCSettForm *ADCSettForm;

 //---------------------------------------------------------------------------

__fastcall TADCSettForm::TADCSettForm(TComponent* Owner,TLCard791 *_l�ard791) : TForm(Owner)
{
	//ini=_ini;
	sL�ard791=_l�ard791;
	KeyPreview = true;
}

// ---------------------------------------------------------------------------
void __fastcall TADCSettForm::FormCreate(TObject *Sender)
{
	//LoadFormPos(this, ini);
	AnsiString title="��������� ���������";
//	int space=12;
	int err=-99;
	//title+=ini->ReadString("Default", "TypeSize", "1");
	Caption=title;
	//Par = new RLCard_parameters(ini);

	int vmar = 40, hmar = 55;
	int height = 15, width = 60;

	int hspace = 4;
	int vspace = 12;

	int ltop;
	TFRSensor* p;

	ltop = vspace+Label1->Top+Label1->Height;
	p = new TFRSensor(gbSG, sL�ard791->FindChByName("chBarkgausen",err), "����������");
	p->Parent = gbSG;
	p->Name = "chBarkgausen";
	p->Left = hspace;
	p->Top = ltop;
	ltop+=p->Height+vspace;
	listSens.Add(p);

	p = new TFRSensor(gbSG, sL�ard791->FindChByName("chVoltageSol",err), "���������� �������:");
	p->Parent = gbSG;
	p->Name = "chVoltageSol";
	p->Left = hspace;
	p->Top = ltop;
	ltop+=p->Height+vspace;
	listSens.Add(p);

	p = new TFRSensor(gbSG, sL�ard791->FindChByName("chCurrentControl",err), "��� ��������������:");
	p->Parent = gbSG;
	p->Name = "chCurrentControl";
	p->Left = hspace;
	p->Top = ltop;
	ltop+=p->Height+vspace;
	listSens.Add(p);

	tbCheckMuftaLevel->SetTick(0);
	tbCheckMuftaLevel->SetTick(8192);
	tbCheckMuftaLevel->Refresh();
	FillAllControls();
    tbCheckMuftaLevelChange(this);
	bool RHKret = RegisterHotKey(Handle,
		// Handle ����, �������� ���������� ��������� WM_HOTKEY
		0x00F, // �������� ������������� �������� �����
		MOD_ALT + MOD_CONTROL, // ������������
		VK_RETURN // ��� �������
		);

	flag = false;
	block();
	need_redraw = false;
}
// ---------------------------------------------------------------------------
void __fastcall TADCSettForm::FormDestroy(TObject *Sender)
{
	//delete Par;
	 listSens.Clear();
	 //UnRegisterHotkey( Handle, 0x00F );
	//SaveFormPos(this, ini);
}
// ---------------------------------------------------------------------------
void TADCSettForm::FillAllControls()
{
	// ������ ��� ��������� �� ����� � �����
	readRate->Text = FloatToStr(sL�ard791->frequencyPerChannel_Hz);
	sendRate->Text = FloatToStr(sL�ard791->sendFrequency);
	tbCheckMuftaLevel->Position = sL�ard791->globalSettings->checkMuftaLevel;
	//��������� �������
	FSSG->Load("SG");
}
// ---------------------------------------------------------------------------
//CTRL+Alt+Enter
void TADCSettForm::block()
{
	readRate->Enabled = flag;
	sendRate->Enabled = flag;
	for (int i = 0; i < listSens.Count(); i++)
		listSens[i]->SetEnabledR(flag);
	tbCheckMuftaLevel->Enabled = flag;
	FSSG->SetEnable(flag);
}
// ---------------------------------------------------------------------------
void __fastcall TADCSettForm::ApplicationEventsMessage(tagMSG &Msg, bool &Handled)
{
	// ��������� �� ������� ��������� ������ Ctrl+Alt+Enter
	if (Msg.message == WM_HOTKEY) // ��������� ����
	{
		if (Msg.wParam == 0x00F) // ������������� ���
		{
			Handled = true;
			if (flag)
				flag = false;
			else
				flag = true;
			block();
		}
	}
}
// ---------------------------------------------------------------------------
void __fastcall TADCSettForm::bSaveClick(TObject *Sender)
{    int err=-99;
	// ��������� ������ �� ��������
	for (int i = 0; i < listSens.Count(); i++)
		listSens[i]->SaveSettings();
	// ��������� ������ ���������
	sL�ard791->frequencyPerChannel_Hz=(double)StrToFloat(this->readRate->Text);
	sL�ard791->sendFrequency=(double)StrToFloat(this->sendRate->Text);
	sL�ard791->globalSettings->checkMuftaLevel = tbCheckMuftaLevel->Position;
	//Par->SaveSettings();

	//�� ������������� - ������ �� ���������
	//ini->WriteInteger("LCard", "SyncMode", 0);
	//ini->WriteInteger("LCard", "SyncStartMode", 0);
	SqlDBModule->UpdIntSql("L791Params","frequencyPerChannel_Hz",sL�ard791->frequencyPerChannel_Hz,NULL);
	SqlDBModule->UpdIntSql("L791Params","measures_frequency_kHz",sL�ard791->measures_frequency_kHz,NULL);
	SqlDBModule->UpdFloatSql("SettingsGlobal","checkMuftaLevel",sL�ard791->globalSettings->checkMuftaLevel,NULL);
	sL�ard791->LoadAndSetSettings(sL�ard791->vecLogChannels);
	FSSG->Save("SG");
	//ini->WriteBool("Filters", "isFilterSG", FSSG->IsUsed());
	SGFilter->setSettingsFromDB();
	sL�ard791->globalSettings->isFilter = SqlDBModule->GetIntFieldSQL("SettingsGlobal","isFilter",NULL,1,err);
	need_redraw = true;
}
// ---------------------------------------------------------------------------
void __fastcall TADCSettForm::bExitClick(TObject *Sender)
{
	Close();
}
// ---------------------------------------------------------------------------
void __fastcall TADCSettForm::eThresholdResistKeyPress(TObject *Sender, wchar_t &Key)
{
	if(!((Key >= '0' && Key <= '9') || (Key == VK_BACK) || (Key == ',')))
		Key = 0x00;
}
// ---------------------------------------------------------------------------
void __fastcall TADCSettForm::ContextPopup(TObject *Sender, TPoint &MousePos, bool &Handled)
{
//	if (((TComboBox*)Sender)->Tag == 0)
//		CrossItemIndex = ((TComboBox*)Sender)->ItemIndex;
//	else
//		LinearItemIndex = ((TComboBox*)Sender)->ItemIndex;
}
// ---------------------------------------------------------------------------

void __fastcall TADCSettForm::FormActivate(TObject *Sender)
{
	//bool RHKret=RegisterHotKey(ADCSettForm->Handle, // Handle ����, �������� ���������� ��������� WM_HOTKEY
//	bool RHKret=RegisterHotKey(this->Handle,
//		0x00F, // �������� ������������� �������� �����
//		MOD_ALT+MOD_CONTROL, // ������������
//		VK_RETURN // ��� �������
//		);
	FillAllControls();
}
//---------------------------------------------------------------------------

void __fastcall TADCSettForm::tbCheckMuftaLevelChange(TObject *Sender)
{
	lCheckMuftaLevel->Caption = IntToStr(tbCheckMuftaLevel->Position);
}
//---------------------------------------------------------------------------


