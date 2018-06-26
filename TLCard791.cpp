#pragma hdrstop
#include "TLCard791.h"
// #include "L791_include\ioctl.h"
// #include "L791_include\ifc_ldev.h"
// #include "L791_include\create.h"
// #include "L791_include\791cmd.h"
// ------serg
#include "CardsFiles\L791\ioctl.h"
#include "CardsFiles\L791\ifc_ldev.h"
#include "CardsFiles\L791\create.h"
#include "CardsFiles\L791\791cmd.h"
#include "unTExtFunction.h"
#include "unSQLDbModule.h"
#include "unTUtils.h"
#include "unFmMessage.h"
// -----
// #include "uFunctions.h"
#include "TProtocol.h"
#pragma package(smart_init)
// ---------------------------------------------------------------------------
CREATEFUNCPTR CreateInstance;
EXTERN_C const GUID DECLSPEC_SELECTANY IID_ILDEV = {
	0x32bb8320, 0xb41b, 0x11cf, {0xa6, 0xbb, 0x00, 0x80, 0xc7, 0xb2, 0xd6, 0x82}
};
TLCard791 *lcard;

// ---------------------------------------------------------------------------
// TLCard791::TLCard791(TIniFile* _ini)//, AnsiString _type)
TLCard791::TLCard791(TGlobalSettings* _mainGlobalSettings, int &_codeErr) {
	try {
		globalSettings=_mainGlobalSettings;
		_codeErr = -99;
		countLogCh = 0;
		started = false;

		// ini = _ini;
		// type = _type;
		// type.Unique();
		// slot = ini->ReadInteger("L791", "slot", 0);
		// ----------
		slotL791 = 0;//если плата одна то слот 0
		// заполним вектор каналов
		SqlDBModule->ADOQueryDB->Close();
		AnsiString str = "";
		str = "SELECT rec_id,InterframeDelay,InterchannelDelay ,adcRangeIndex,chName,numLogCh,collectedMode,chGain,numFizCh";
		str += " FROM L791ChParams where fEnable=1 order by numLogCh";
		SqlDBModule->ADOQueryDB->SQL->Text = str;
		SqlDBModule->ADOQueryDB->Open();
		//заведем количество каналов
		countLogCh =  SqlDBModule->ADOQueryDB->RecordCount;
		//countLogCh = SqlDBModule->GetIntFromSql("select count(*) as F1 FROM L791ChParams where fEnable=1");
		str = SqlDBModule->ADOQueryDB->FieldByName("chName")->AsString;
		vecLogChannels.reserve(countLogCh);
		SqlDBModule->ADOQueryDB->First();
		while (!SqlDBModule->ADOQueryDB->Eof) {
			TLogCh791Params* tlcp = new TLogCh791Params();
			tlcp->chName = SqlDBModule->ADOQueryDB->FieldByName("chName")
				->AsString;
			// tlcp->adcRange = SqlDBModule->ADOQueryDB->FieldByName("adcRange")
			// ->AsInteger;
			tlcp->adcRangeIndex = SqlDBModule->ADOQueryDB->FieldByName
				("adcRangeIndex")->AsInteger;
			tlcp->logicalChannel = SqlDBModule->ADOQueryDB->FieldByName
				("numLogCh")->AsInteger;
//			tlcp->fizChannel = SqlDBModule->ADOQueryDB->FieldByName("numFizCh")
//				->AsInteger;
			tlcp->collectedMode = SqlDBModule->ADOQueryDB->FieldByName
				("collectedMode")->AsInteger;
			tlcp->chGain = SqlDBModule->ADOQueryDB->FieldByName("chGain")
 				->AsInteger;
			vecLogChannels.push_back(*tlcp);
			SqlDBModule->ADOQueryDB->Next();
		}
		SqlDBModule->ADOQueryDB->Close();

		lastError = "";
		t_stop_event = CreateEvent(0, true, false, 0);
#ifndef TVIRTLCARD791
		hComponent = LoadLibrary(L"lcomp.dll");
		if (hComponent == NULL) {
			_codeErr = -1;
			LFATAL("Не могу загрузить библиотеку lcomp.dll");
		}
		else {
			//
		}
		CreateInstance = (CREATEFUNCPTR)GetProcAddress(hComponent,
			"CreateInstance");
		if (CreateInstance == NULL) {
			_codeErr = -2;
			LFATAL("Не могу найти функцию CreateInstance");
		}
		else {
			//
		}
		LUnknown* pIUnknown = NULL;
		IDaqLDevice* pInterface = NULL;

		pIUnknown = CreateInstance((ULONG) slotL791);
		if (pIUnknown == NULL) {
			_codeErr = -3;
			LFATAL("Не могу найти свободную плату");
		}
		else {
			//
		}
		HRESULT hr = pIUnknown->QueryInterface(IID_ILDEV, (void**)&pInterface);
		pIUnknown->Release();
		if (!SUCCEEDED(hr)) {
			pInterface = NULL;
			pIUnknown->Release();
			_codeErr = -4;
			LFATAL("Не могу получить интерфейс");
		}
		else {
			//
		}
		if (pInterface->OpenLDevice() == INVALID_HANDLE_VALUE) {
			_codeErr = -5;
			LFATAL("Не могу открыть устройство");
		}
		else {
			//
		}
		SLOT_PAR slotParam;
		if (pInterface->GetSlotParam(&slotParam) != L_SUCCESS) {
			_codeErr = -6;
			LFATAL("Не могу получить параметры");
		}
		else {
			//
		}
		if (slotParam.BoardType != L791) { //#define L791     19
			_codeErr = -7;
			LFATAL("Не верный тип платы");
		}
		else {
			//
		}
//		PLATA_DESCR_L791 plataDescr;
		if (pInterface->ReadPlataDescr(&plataDescr) != L_SUCCESS) {
			_codeErr = -8;
			LFATAL("Не могу получить описание");
		}
		else {
			//
		}
		AnsiString a;
		a = plataDescr.BrdName;
		if (a != "L791") {
			_codeErr = -9;
			LFATAL("Не верный тип платы");
		}
		else {
			//
		}
		pI_void=NULL;
		pI_void = pInterface;
		data_buf=NULL;
		int szBuf=1024 * 128 * countLogCh;
		data_buf = new double[szBuf];
		_codeErr = 0;
		LastRead = 0;
		// частота измерений
		// measures_frequency_kHz = 11;
		measures_frequency_kHz = SqlDBModule->GetFloatFieldSQL("L791Params",
			"measures_frequency_kHz", NULL, 11, _codeErr);
		// частота кадров (частота на канал)
		// frequencyPerChannel_Hz = 100;
		frequencyPerChannel_Hz = SqlDBModule->GetFloatFieldSQL("L791Params",
			"frequencyPerChannel_Hz", NULL, 100, _codeErr);
			//
		RECV_TOUT = SqlDBModule->GetIntFieldSQL("L791Params", "RECV_TOUT",
			NULL, 0, _codeErr);
		syncMode = SqlDBModule->GetIntFieldSQL("L791Params", "syncMode",
			NULL, 0, _codeErr);
		syncStartMode = SqlDBModule->GetIntFieldSQL("L791Params",
			"syncStartMode", NULL, 0, _codeErr);
		sendFrequency = SqlDBModule->GetFloatFieldSQL("L791Params",
			"sendFrequency", NULL, 1000, _codeErr);

		// --------- припишем настройки в плату
		LoadAndSetSettings(vecLogChannels);
		// -----------
#endif   //TVIRTLCARD791
	}
	catch (Exception *ex) {
		// TLog::ErrFullSaveLog(ex);
		MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);
	}
}

// ---------------------------------------------------------------------------
TLCard791::~TLCard791() {
#ifndef TVIRTLCARD791
	// Остановим плату
	Stop();
	// Закроем и освободим плату
	if (pI_void != NULL) {
		IDaqLDevice* pInterface = (IDaqLDevice*)pI_void;
		pInterface->CloseLDevice();
		pInterface->Release();
	}
	CreateInstance = NULL;
	FreeLibrary(hComponent);
	CloseHandle(t_stop_event);
	delete data_buf;
	#endif
}

// ---------------------------------------------------------------------------
// Читает настройки и вбивает в плату
void TLCard791::LoadAndSetSettings(vector<TLogCh791Params>& _logChannels) {
int err=0;
	try {

		IDaqLDevice* pInterface = (IDaqLDevice*)pI_void;
		err=GetLastError();
		ADC_PAR adcParams;
		// ADC_PAR_1 t2;
		// тип структуры (должен быть L_ADC_PARAM
		adcParams.t2.s_Type = L_ADC_PARAM;
		// ULONG AutoInit - флаг указывающий на тип сбора данных 0 - однократный 1 -циклический;
		adcParams.t2.AutoInit = 1;
		// размер половины аппаратного буфера FIFO на плате
		// возможные значения 1,2,4,8,16,32,64,128 отсчетов
		// Если установить больше 128, то она сама сбросит до128
		adcParams.t2.FIFO = 1024;
		// шаг генерации прерываний max ограничен 2048 отсчетами)
		adcParams.t2.IrqStep = 1024; //
		// размер кольцевого буфера в шагах прерываний;
		adcParams.t2.Pages = 64;//4 * countLogCh; // countLogCh =5
		// по идее буфер равен  adcParams.t2.Pages * adcParams.t2.IrqStep =1024*4*5= 20480
		// период измерений
		double measures_period_ms = countLogCh / measures_frequency_kHz;
		// период кадров
		double kadrs_period_ms = 1000 / frequencyPerChannel_Hz;
		// межкадровая задержка
		double kadrs_delay_ms = kadrs_period_ms - measures_period_ms;
		// double kadrs_delay_ms = 0;
		// частота опроса каналов в кадре (кГц);
		adcParams.t2.dRate = measures_frequency_kHz;
		// нтервал между кадрами (мс);
		adcParams.t2.dKadr = kadrs_delay_ms;
		adcParams.t2.Reserved1 = 0;
		// тип синхронизации
		//ULONG SynchroType - тип синхронизации
		adcParams.t2.SynchroType=0;// – просто внутренний старт;
	   //ULONG SynchroSensitivity - вид синхронизации;
		//ULONG SynchroMode - режим синхронизации;
		adcParams.t2.SynchroMode=0; //0 – по уровню «выше» или переходу «снизу-вверх»;
		adcParams.t2.SynchroSrc = 0;
		//NCh - количество опрашиваемых каналов ;
		adcParams.t2.NCh=countLogCh;
		// countLogCh = _logChannels.size();
		//ULONG Chn[128] - массив с номерами каналов и усилением на них; описывает порядок опроса каналов;
		for (int i = 0; i < countLogCh; i++) {
			// массив с номерами каналов и усилением на них. Описываетпорядок опроса каналов;
			adcParams.t2.Chn[i] = CalcPortBits(_logChannels[i].logicalChannel,
				_logChannels[i].adcRangeIndex,
				_logChannels[i].collectedMode != 0);
		}
		// разрешение генерации прерывания от платы (1/0);
		adcParams.t2.IrqEna = 0;
		// разрешение работы АЦП (1/0);
		adcParams.t2.AdcEna = 1;
		if (pInterface->FillDAQparameters(&adcParams.t2) != L_SUCCESS)
			LFATAL("не могу загрузить параметры");
//		//Включаем автокорректировку сигнала
//		if (pInterface->EnableCorrection(1) != L_SUCCESS)
//			LFATAL("не могу разрешить автокорректировку L791");
		if (adcParams.t2.dRate != measures_frequency_kHz) {
			AnsiString a;
			a = "Требуемая частота ";
			a += measures_frequency_kHz;
			a += "kHz не соответствует установленной ";
			a += adcParams.t2.dRate;
			a += "kHz";
			LFATAL(a);
		}
		// каналы*double*байты
		int su = sizeof(ULONG);
		// adcParams.t2.Pages * adcParams.t2.IrqStep =1024*4*5= 20480
		ULONG tm = 1024 * 128;//1024 * 4 * countLogCh;
		// tm =20480
		// Выделяет память в ОЗУ компьютера под большой кольцевой буфер. Память выделяется с выравниванием размера на 4096 байт.
		if (pInterface->RequestBufferStream(&tm, L_STREAM_ADC) != L_SUCCESS)
			LFATAL("не могу получить поток");

		int defSize = adcParams.t2.Pages * adcParams.t2.IrqStep;
		bufSize = 0;
		if (pInterface->SetParametersStream(NULL, &bufSize, (void**)&data,
			(void**)&sync, L_STREAM_ADC) != L_SUCCESS)
			LFATAL("не могу установить параметры потока");
		double rrr = (double)tm / bufSize;
		// int zzz=I_ADC_PCI_COUNT_L791;    //не используется
		sync += I_ADC_PCI_COUNT_L791;
		if (pInterface->InitStartLDevice() != L_SUCCESS)
			LFATAL("не могу сделать начальный старт устройства");
		double DPeriod_kadr = adcParams.t2.NCh;
		DPeriod_kadr /= adcParams.t2.dRate;
		DPeriod_kadr += adcParams.t2.dKadr;
		Period_kadr = (int)DPeriod_kadr;
		if (Period_kadr < 1)
			Period_kadr = 1;
		double DPeriod = DPeriod_kadr;
		DPeriod *= bufSize;
		DPeriod /= adcParams.t2.NCh;
		Period = (int)DPeriod;
//		AnsiString a;
//		ToProtocol(a.sprintf("Большой буфер, байт: %luK", tm / 1024));
//		// pr(a.sprintf("Rate=%lu",ap.t2.Rate));
//		ToProtocol(a.sprintf("UsedSize, words: %luK", bufSize / 1024));
//		ToProtocol(a.sprintf("Частота сбора, kHz: %10.5lf",
//			adcParams.t2.dRate));
//		ToProtocol(a.sprintf("Задержка в кадре, мс: %10.5lf",
//			adcParams.t2.dKadr));
//		ToProtocol(a.sprintf("Период буфера, мс: %d", Period));
		// if (Size != 128 * 1024)
		// if (Size != defSize) {
		delete data_buf;
		data_buf = new double[bufSize];
		// ToProtocol("!!! Значение буфера изменено !!!");
		// }
	}
	catch (Exception *ex) {
		// TLog::ErrFullSaveLog(ex);
		// AnsiString msg
		// programSettings.colorMSG = programSettings.colorBrak;
		// TExtFunction::UpdateStatusBar(programSettings.gsStatusBar, strStatus, _msg, programSettings.colorMSG);
		// TExtFunction::UpdateLabelStatus(programSettings.gsStatusLabel, strStatus, _msg, programSettings.colorMSG);
		MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);
	}
}

// ---------------------------------------------------------------------------
// Старт платы
void TLCard791::Start() {
	if (started)
		return;
	LoadAndSetSettings(vecLogChannels);
	started = true;
	IDaqLDevice* pInterface = (IDaqLDevice*)pI_void;
	if (pInterface->StartLDevice() != L_SUCCESS)
		LFATAL("не могу стартовать устройство");
	prev_sync = 0;
}

// ---------------------------------------------------------------------------
// Остановка платы
void TLCard791::Stop(void) {
	if (!started)
		return;
	started = false;
	IDaqLDevice* pInterface = (IDaqLDevice*)pI_void;
	if (pInterface->StopLDevice() != L_SUCCESS)
		LFATAL("не могу остановить устройство");
}

// ---------------------------------------------------------------------------
// Сообщает о критической ошибке
void TLCard791::LFATAL(AnsiString _msg) {
	AnsiString a = "Плата L791: слот ";
	a += slotL791;
	a += ": ";
	a += _msg;
	TExtFunction::ShowBigModalMessage(_msg, clRed);
	// FATAL(a);
}

// ---------------------------------------------------------------------------
// Получаем замер асинхронно!!!
ULONG TLCard791::GetValueRaw(int _ch, int _gain, bool _diff, bool* _ret) {
	IDaqLDevice* pInterface = (IDaqLDevice*)pI_void;
	ASYNC_PAR ap;
	ap.s_Type = L_ASYNC_ADC_INP;
	ap.Chn[0] = CalcPortBits(_ch, _gain, _diff);
	ap.Data[0] = 0;
	if (pInterface->IoAsync(&ap) != L_SUCCESS) {
		if (_ret != NULL)
			* _ret = false;
		return (0);
	}
	if (_ret != NULL)
		* _ret = true;
	return (ap.Data[0]);
}

// ---------------------------------------------------------------------------
// Выделяем значение замера
int TLCard791::GetValueMeasure(int _ch, int _gain, bool _diff, bool* _ret) {
	ULONG v = GetValueRaw(_ch, _gain, _diff, _ret);
	short* sv = (short*)(&v);
	return ((int)(*sv));
}

// ---------------------------------------------------------------------------
// Переводим значение в вольты
double TLCard791::GetValueVolt(int _val, int _gain) {
	double vf = _val;
	vf *= 10;
	vf /= 8192; // нормируем до -10...+10
	vf /= _gain;
	return (vf);
}

// ---------------------------------------------------------------------------
// Переводим значение в проценты
double TLCard791::GetValueProcent(int _val) {
	double vf = _val;
	vf *= 100;
	vf /= 8192; // нормируем до -100%...+100%
	return (vf);
}

// ---------------------------------------------------------------------------
// Получить значение замера по каналу
int TLCard791::GetValueByChannel(TLogCh791Params& _channel) {
	// return (GetValueMeasure(_channel.logicalChannel, _channel.adcRange,_channel.collectedMode != 0));
	return (GetValueMeasure(_channel.logicalChannel, _channel.adcRangeIndex,
		_channel.collectedMode != 0));
}

// ---------------------------------------------------------------------------
// Получить значение замера в вольтах по каналу
// double TLCard791::GetValueVolt(RChannel_parameters& _channel) {
double TLCard791::GetValueVolt(TLogCh791Params& _channel) {
	return (GetValueVolt(GetValueByChannel(_channel), _channel.adcRangeIndex));
}

// ---------------------------------------------------------------------------
// Переводит логический номер канала и усиление в формат для платы
ULONG TLCard791::CalcPortBits(int _port, int _gain, bool _diff) {
	ULONG ret;
	if (_diff)
		ret = _port & 15;
	else {
		ret = _port & 31;
		ret |= 1 << 5;
	}
	int gain;
	gain = _gain;
	if(gain < 0 || gain > 7)
		gain = 0;
//	switch (_gain) {
//	case 1:
//		gain = 0;
//		break;
//	case 2:
//		gain = 1;
//		break;
//	case 4:
//		gain = 2;
//		break;
//	case 8:
//		gain = 3;
//		break;
//	case 16:
//		gain = 4;
//		break;
//	case 32:
//		gain = 5;
//		break;
//	case 64:
//		gain = 6;
//		break;
//	case 128:
//		gain = 7;
//		break;
//	default:
//		gain = 0;
//	}
	ret |= gain << 6;
	return (ret);
}

// ---------------------------------------------------------------------------
// Вывод сообщения в протокол
void TLCard791::ToProtocol(AnsiString _msg) {
	AnsiString a;
	a = "LCard791: ";
	// a += type;
	// a += ": ";
	a += _msg;
	TProtocol::ProtocolSave(a);
}

// ---------------------------------------------------------------------------
// Старт сбора
void TLCard791::StartRead(void) {
	prev_sync = *sync;
	LastRead = GetTickCount();
	Sleep(Period_kadr*10);
	while(true) {
		int ch = GetChannel(data[prev_sync]);
//		 AnsiString a="ch=";
//		 a+=ch;
//		 a+=" prev_sync=";
//		 a+=prev_sync;
//		 TProtocol::ProtocolSave(a);
		if (ch == 0)
			break;
		prev_sync++;
		if (prev_sync >= bufSize)
			prev_sync = 0;
	}
	currChLIndex = 0;
}

// ---------------------------------------------------------------------------
// Остановка сбора
void TLCard791::StopRead(void) {
	LastRead = 0;
}

// ---------------------------------------------------------------------------
// Сбор данных
double* TLCard791::Read(int* _readSize)
{
	try
	{
		// pr("Read");
		DWORD CurRead = GetTickCount();
		if (LastRead != 0) {
			if (CurRead - LastRead > Period/ 2) {
				LFATAL("Превышено время периодического чтения платы, возможна потеря данных"
					);
			}
		}
		LastRead = CurRead;
		AnsiString a;
	//	int ch = GetChannel(data[prev_sync]);
	//			a += "Просто prev_sync=";
	//			a += prev_sync;
	//			ToProtocol(a);
		// это хитрый указатель на первый читаемый байт
		int cur_sync = *sync;
		*_readSize = 0;
		double val;
		//	double percent = 100;
	//	// 8192 хто ты? диапазон на L791 от -8191 до 8192
	//	percent /= 8192;
		for (; ;)
		{
			if (prev_sync == cur_sync)
				break;
			double val = *((short*)(data + prev_sync));
			//номер канала в буфере
			int ch = GetChannel(data[prev_sync]);
			//учтем калибровочные коэффициенты
			//plataDescr.KoefADC[0+0x0]; масштаб
			//plataDescr.KoefADC[0+0x8]; смещение
            int CInd = vecLogChannels[ch].adcRangeIndex;
			double mas=plataDescr.KoefADC[CInd+0x0];
			double ofs=plataDescr.KoefADC[CInd+0x8];
			double corrVal=(val+ofs)*mas;
			AnsiString str1=plataDescr.SerNum;
			AnsiString str2=plataDescr.BrdName;
			//Для перевода откорректированного значения из кода АЦП в напряжение (т.е. в Вольты)
			  //можно воспользоваться следующей формулой:
			 //U = Y × Range / 8192,
			if (currChLIndex != ch) {
				a = "Не совпадает последовательность датчиков: ";
				a += currChLIndex;
				a += " != ";
				a += ch;
				a += " prev_sync=";
				a += prev_sync;
				ToProtocol(a);
				// LFATAL(a);
			}
			currChLIndex++;
			if (currChLIndex == countLogCh)
				currChLIndex = 0;
	//		SqlDBModule->UpdIntSql("SettingsGlobal","breakout",*_readSize,NULL);
			//data_buf[(*_readSize)++] = val;// * percent;//todo  фигачим прямо в отсчетах
			data_buf[(*_readSize)++] = corrVal;//serg
			// data_buf[(*_size)++] = ch*5;
			prev_sync++;
			// data_buf[(*_size)++] = prev_sync++;
			if (prev_sync == bufSize)
				prev_sync = 0;
		}
	}
	catch (Exception *ex)
	{
		TLog::ErrFullSaveLog(ex);
		//AnsiString msg
		//programSettings.colorMSG = programSettings.colorBrak;
		//TExtFunction::UpdateStatusBar(programSettings.gsStatusBar, strStatus, _msg, programSettings.colorMSG);
		//TExtFunction::UpdateLabelStatus(programSettings.gsStatusLabel, strStatus, _msg, programSettings.colorMSG);
		MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);
	}
	return (data_buf);
}

// ---------------------------------------------------------------------------
// выделяет номер канала из замера с карты
int TLCard791::GetChannel(ULONG _v) {
	ULONG v = (_v >> 16) & 0x7F;
	return ((int)v);
}

// ---------------------------------------------------------------------------
// RChannel_parameters* RLCard_parameters::FindByName(AnsiString _name)
TLogCh791Params* TLCard791::FindChByName(AnsiString _name, int &err) {
	for (unsigned int i = 0; i < countLogCh; i++) {
		if (vecLogChannels[i].chName == _name) {
			err = 0;
			return (&vecLogChannels[i]);
		}
		else {
			err = -2;
		}

	}
	AnsiString a = "RLCard_parameters::FindPosByName: Канал не найден: ";
	a += _name;
	 //TExtFunction::FATAL(a);
	err = -1;
	return (NULL);
}

// один отсчет с 0 канала АЦП.
double TLCard791::GetValueAsync(int _chNum) {
	IDaqLDevice* pInterface = (IDaqLDevice*)pI_void;
	ASYNC_PAR pp;
	pp.s_Type = L_ASYNC_ADC_INP;
	pp.Chn[0] = 0x00;
	// 0 канал дифф. подключение (в общем случае лог. номер канала)
	pInterface->IoAsync(&pp);
	// cout << (short)pp.Data[0] << endl;
	return 1.1;
}
