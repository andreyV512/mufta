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
		// количество подключенных карт
		x502CountDev = 0;
		// диапазон
		// признак режима синхронный/асинхронный - выставлен синхронный
		x502EnableSyncADC = 1;
		// каналы только с общей землей
		x502ChMode = X502_LCH_MODE_COMM;
		// диапазон АЦП всех каналов 0-10 В
		// x502ADC_RANGE_V = X502_ADC_RANGE_10;
		// режим синхронизации внутренний
		// x502SyncMode = X502_SYNC_INTERNAL;
		// X502_SYNC_INTERNAL        = 0, /**< Внутренний сигнал */
		// X502_SYNC_DI_SYN1_RISE    = 2, /**< По фронту сигнала DI_SYN1 */
		// X502_SYNC_DI_SYN2_RISE    = 6, /**< По фронту сигнала DI_SYN2 */
		// x502SyncStartMode = X502_SYNC_INTERNAL; // признак старта при использовании синхронного ввода (считывание данных с модуля)
		// число физ каналов
		// x502CountFIZch=32;
		// число лог каналов
		// x502CountLOGch=32;
		// значения по умолчанию
		x502ADCBufSensorSize = 2000;
		x502DinBufSize = 1000;
		for (int i = 0; i < X502_LTABLE_MAX_CH_CNT; i++) {
			adc_Range[i] = 0;
			adc_Range_Value[i] = 10.0;

		}
		for (int i = 0; i < 32; i++) {
			// значения входов
			valuesIN[i] = 0;
			// значения на выходе
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
	// если остался не удаленный объект потока - удаляем
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
// метод определяет какие управляющие элементы разрешены, а какие запрещены  пример
void TWorkX502::UpdateControls() {
	// cbbDevList->Enabled = (hndX502==NULL);
	// btnOpen->Caption = hndX502 == NULL ? "Установить связь с модулем" :
	// "Разорвать связь с модулем";

}

// ------------------------------
// очистка количества устройств
void TWorkX502::FreeDevList() {
	// if (devrecList.Length != 0) {
	// X502_FreeDevRecordList(&devrecList[0], devrecList.Length);
	// devrecList.Length = 0;
	// }
	// //cbbDevList->Items->Clear();
}

// ------------------------------
/* обновление списка подключенных плат L502 и E502 */
void TWorkX502::RefreshDeviceList(int &_CountDev) {
	uint32_t pci_devcnt = 0;
	uint32_t usb_devcnt = 0;
	int32_t fnd_devcnt = 0;
	/* получаем количество подключенных устройств по интерфейсам PCI и USB */
	L502_GetDevRecordsList(NULL, 0, 0, &pci_devcnt);
	E502_UsbGetDevRecordsList(NULL, 0, 0, &usb_devcnt);
	/* выделяем память для массива для сохранения найденного
	 количества записей */
	if ((pci_devcnt + usb_devcnt) != 0) {
		devrecList.Length = pci_devcnt + usb_devcnt;
		// * получаем записи о модулях L502
		if (pci_devcnt != 0) {
			int32_t res = L502_GetDevRecordsList(&devrecList[fnd_devcnt], pci_devcnt, 0, NULL);
			if (res >= 0) {
				fnd_devcnt += res;
			}
		}
		// добавляем записи о модулях E502, подключенных по USB
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
// Установка пераметров сбора модуля
int TWorkX502::SetupParams(int _x502CountLOGch, int _SyncMode, int _SyncStartMode) {
	int err = -1;
	uint32_t lch_cnt = _x502CountLOGch;
	// таблица соответствия индексов и кодов режима измерения
	// static uint32_t f_mode_tbl[] = {X502_LCH_MODE_COMM, X502_LCH_MODE_DIFF, X502_LCH_MODE_ZERO};
	// таблица соответствия индексов и кодов диапазонов АЦП
	// static uint32_t f_range_tbl[] = {X502_ADC_RANGE_10, X502_ADC_RANGE_5, X502_ADC_RANGE_2, X502_ADC_RANGE_1, X502_ADC_RANGE_05, X502_ADC_RANGE_02};
	/* таблица соответствия индексов кодов источника синхронизации */
	// static uint32_t f_sync_tbl[] = {X502_SYNC_INTERNAL, X502_SYNC_EXTERNAL_MASTER, X502_SYNC_DI_SYN1_RISE, X502_SYNC_DI_SYN2_RISE, X502_SYNC_DI_SYN1_FALL, X502_SYNC_DI_SYN2_FALL};
	// устанавливаем кол-во логических каналов, чтобы правильно расчитать частоту на канал
	err = X502_SetLChannelCount(hndX502, lch_cnt);
	// Установка параметров логического канала.
	// Функция устанавливает параметры заданного логического канала в логической
	// таблице АЦП.
	for (int i = 0; i < _x502CountLOGch; i++) {
		if (err == X502_ERR_OK) {
			int val = GetADCRange(i);
			err = X502_SetLChannel(hndX502, i, i, x502ChMode, val, 0);
		}
	}

	// Настраиваем источник частоты синхронизации и запуска сбора
	// предполагарось, что оба внутренние
	if (err == X502_ERR_OK)
		// устанавливает кто будет генератором опорной частоты синхронизации
		// X502_SYNC_INTERNAL = 0< Внутренний сигнал
		// X502_SYNC_EXTERNAL_MASTER = 1,  От внешнего мастера по разъему межмодульной синхронизации
		// X502_SYNC_DI_SYN1_RISE = 2, < По фронту сигнала DI_SYN1
		// X502_SYNC_DI_SYN1_FALL = 3, По спаду сигнала DI_SYN1
		// X502_SYNC_DI_SYN2_RISE = 6, < По фронту сигнала DI_SYN2 */
			err = X502_SetSyncMode(hndX502, _SyncMode);
	if (err == X502_ERR_OK)
		// Функция устанавливает условие запуска синхронного ввода/вывода данных.
			err = X502_SetSyncStartMode(hndX502, _SyncStartMode);

	// настраиваем частоту сбора с АЦП и цифровых
	if (err == X502_ERR_OK)
		err = SetAdcFreq(x502AdcFreq, x502AdcFreqLch, x502DinFreq);

	/* Записываем настройки в модуль */
	if (err == X502_ERR_OK)
		err = X502_Configure(hndX502, 0);
	// вытащим частоту в настройки
	wwProgramSettings->adcFreqLch = x502AdcFreqLch;
	wwProgramSettings->dinFreqLch = x502DinFreq;
	return err;
}

// ------------------------------
//
int TWorkX502::SetAdcFreq(int &_x502AdcFreq, int &_x502AdcFreqLch, int &_x502DinFreq) {
	int err = -1;
	double f_acq = _x502AdcFreq; // частота синхронного сбора АЦП
	double f_lch = _x502AdcFreqLch; // частота синхронного сбора АЦП на канал
	// _x502AdcFreqLch - нулевой указатель, то устанавливает максимальную частоту сбора кадров (нулевую межкадровую задержку).
	double f_din = _x502DinFreq; // частота синхронного сбора  для цифровых
	// Устанавливаем частоту синхронного сбора  для АЦП
	err = X502_SetAdcFreq(hndX502, &f_acq, &f_lch);
	if (err == X502_ERR_OK) {
		// значение реально установившейся частоты
		_x502AdcFreq = f_acq;
		// (частоты сбора на логический канал) АЦП
		_x502AdcFreqLch = f_lch;
		// Устанавливаем частоту синхронного сбора  для цифровых
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
// разрешаем или запрещаем поток синхронного ввода для цифровых входов
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
	AnsiString msg = "Сбор данных завершен: " + String(X502_GetErrorString(threadX502->err));
	if (threadX502->err != X502_ERR_OK) {
		AlarmSituiation(msg);
		TLog::SaveStrLog(msg);
		wwProgramSettings->colorMSG = wwProgramSettings->colorBrak;
		// MessageDlg("Сбор данных завершен с ошибкой: " + String(X502_GetErrorString(threadX502->err)), mtError, TMsgDlgButtons() << mbOK, NULL);
	}
	else {
		wwProgramSettings->colorMSG = wwProgramSettings->colorSelZone;
	}
	threadRunning = false;
	// TExtFunction::UpdateStatusBar(wwProgramSettings->gsStatusBar, strStatus, msg, wwProgramSettings->colorMSG);
	// TExtFunction::UpdateLabelStatus(wwProgramSettings->gsStatusLabel, strStatus, msg, wwProgramSettings->colorMSG);
}

// ------------------------------
// настраиваем АЦП и запускаем сбор
int TWorkX502::StartRead(bool _enableSYNCDin, AnsiString &_msg) {
	int err = -1;
	if (threadX502) {
		if (threadRunning) {
			threadX502->stopThread = true;
			_msg += "Поток был запущен, останавливаем.";
			return err;
		}
		else {
			err = 0;
		}
	}
	else {
		// если потока нет, то придется создавать.
		err = 0;
	}

	// X502_STREAM_ADC 0x01 Поток данных от АЦП
	if (err == X502_ERR_OK) {
		// Разрешение синхронных потоков на ввод/вывод.
		err = X502_StreamsEnable(hndX502, X502_STREAM_ADC);
	}
	else {
		//
	}
	// разрешаем синхронный ввод с цифровых линий в зависимости от _enableSYNCDin
	if (err == X502_ERR_OK) {
		err = SetSyncDinStream(_enableSYNCDin);
	}
	else {
		//
	}

	if (err == X502_ERR_OK) {

		// workData = new TWorkData(wwProgramSettings->maxZones, wwProgramSettings->countCrossSensors, wwProgramSettings->firstSensorsCross,
		// wwProgramSettings->firstSensorsThick, wwProgramSettings->countThickSensors, x502CountLOGch, wwProgramSettings->bufReadSize,wwProgramSettings);

		// создание класса потока
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
		// устанавливаем функцию на событие завершения потока (в частности
		// чтобы отследить, если поток завершился сам из-за ошибки при сборе
		// данных)
		threadX502->OnTerminate = OnThreadTerminate;
		threadX502->Resume(); // запуск потока
		threadRunning = true;
		// updateControls();
	}
	_msg = "";
	_msg += "Поток запущен";
	return err;
}

// ---------------------------------------------------------------------------
void __fastcall TWorkX502::StopThread() {
	// /* устанавливаем запрос на завершение потока */
	if (threadRunning) {
		threadX502->stopThread = true;
		// wwProgramSettings->needData=false;
	}
	else {
		//
	}
}

// ----------------------------------------------
// информация о карте
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
		MessageDlg("Ошибка получения информации о модуле: " + String(X502_GetErrorString(err)), mtError, TMsgDlgButtons() << mbOK, NULL);

		// X502_Close(hndX502);
		// X502_Free(hndX502);
		// hndX502 = NULL;
	}
	return result;
}

// ----------------------------------------------
// функция открытия устройства с выбранным номером или закрытия его
// у нас всегда 0 - девайс один
int TWorkX502::X502OpenClose(int _numDev, AnsiString &_msg) {
	int err = -1;
	try {
		// /* если устройство было закрыто - создаем новый описатель и открываем его */
		if (hndX502 == NULL) {
			int idx = _numDev;
			if (idx >= 0) {
				hndX502 = X502_Create();
				if (hndX502 == NULL) {
					err = -2;
					_msg = "Ошибка создания описателя модуля";
					MessageDlg(_msg, mtError, TMsgDlgButtons() << mbOK, NULL);
				}
				else {
					// открываем модуль, соответствующий записи с выбранным индексом
					err = X502_OpenByDevRecord(hndX502, &devrecList[idx]);
					if (err != X502_ERR_OK) {
						err = -3;
						_msg = "Ошибка открытия модуля: " + String(X502_GetErrorString(err));
						MessageDlg(_msg, mtError, TMsgDlgButtons() << mbOK, NULL);
						// throw new Exception("Ошибка открытия модуля!!!");
						// X502_Free(hndX502);
						// hndX502 = NULL;
					}
					else {
						// showDevInfo();
						// workX502.isX502Opened=true;
						_msg = "Модуль открыт: " + String(X502_GetErrorString(err));
						err = 0;
					}
				}
			}
		}
		else {
			// если устройство было открыто - закрываем
			// закрытие связи с модулем
			X502_Close(hndX502);
			// освобождение описателя
			X502_Free(hndX502);
			hndX502 = NULL;
			isX502Opened = false;
			err = -11;
			_msg = "Устройство было открыто - закрываем";
		}
		// updateControls();
		// err = 0;
	}
	catch (Exception *ex) {
		Application->MessageBoxW(ex->Message.c_str(), L"Ошибка!", MB_ICONERROR);
		TDateTime currentDT;
		AnsiString strCurrentDT = FormatDateTime("yyyy.mm.dd hh:mm:ss", currentDT.CurrentDateTime());

	}
	return err;
}

// ----------------------------------------------
// проверка IP на валидность формата
int TWorkX502::ParseIpAddr(AnsiString _addr) {
	int a[4], i;
	if (sscanf(_addr.c_str(), "%d.%d.%d.%d", &a[0], &a[1], &a[2], &a[3]) != 4) {
		throw new Exception("Неверный формат IP-адреса!!");
	}

	for (i = 0; i < 4; i++) {
		if ((a[i] < 0) || (a[i] > 255)) {
			throw new Exception("Недействительный IP-адрес!!");
		}
	}

	return (a[0] << 24) | (a[1] << 16) | (a[2] << 8) | a[3];
}

// ----------------------------------------------
// открываем по IP
void TWorkX502::OpenByIP(AnsiString _strIP) {
	t_x502_devrec devrec;
	int32_t err = E502_MakeDevRecordByIpAddr(&devrec, ParseIpAddr(_strIP), 0, 5000);
	if (err != X502_ERR_OK) {
		MessageDlg("Ошибка создания записи: " + String(X502_GetErrorString(err)), mtError, TMsgDlgButtons() << mbOK, NULL);
	}
	else {
		hndX502 = X502_Create();
		if (hndX502 == NULL) {
			MessageDlg("Ошибка создания описателя модуля", mtError, TMsgDlgButtons() << mbOK, NULL);
		}
		else {
			/* устанавливаем связь по созданной записи */ int32_t err = X502_OpenByDevRecord(hndX502, &devrec);
			if (err != X502_ERR_OK) {
				MessageDlg("Ошибка открытия модуля: " + String(X502_GetErrorString(err)), mtError, TMsgDlgButtons() << mbOK, NULL);
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
		/* если запущен поток сбора данных - то останавливаем его */
		if (threadRunning) {
			threadX502->stopThread = 1;
			threadX502->WaitFor();
		}
		/* закрытие связи с модулем */ X502_Close(hndX502);
		/* освобождение описателя */ X502_Free(hndX502);
		hndX502 = NULL;
	}
}
// ---------------------------------------------------------------------------

// асинхронного вывода на цифровые линии
int TWorkX502::SetAsyncDigOut(int _AsyncDigOut, AnsiString &_msg) {
	int err = -1;
	if (hndX502 != NULL) {
		// разрешаем менять все цифровые выходы
		int mask = 0x0000;
		err = X502_AsyncOutDig(hndX502, _AsyncDigOut, mask);
		if (err != X502_ERR_OK) {
			_msg = "Ошибка асинхронного вывода на цифровые линии: " + String(X502_GetErrorString(err));
		}
	}
	return err;
}

// ---------------------------------------------------------------------------
// устанавливаем кол-во логических каналов, чтобы правильно расчитать частоту на канал
int TWorkX502::AdcFreqChange(int _ChCnt, AnsiString &_msg) {
	int err = -1;
	if (hndX502 != NULL) {
		err = X502_SetLChannelCount(hndX502, x502CountLOGch);
		if (err == X502_ERR_OK) {
			err = SetAdcFreq(x502AdcFreq, x502AdcFreqLch, x502DinFreq);
		}
		else {
			err = -2;
			_msg = "TWorkX502::adcFreqChange ошибка получения данных X502_SetLChannelCount" + String(X502_GetErrorString(err));
			TLog::SaveStrLog(_msg);
		}
		if (err != X502_ERR_OK) {
			err = -3;
			_msg = "TWorkX502::adcFreqChange Ошибка установки частоты сбора: " + String(X502_GetErrorString(err));
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
// разрешения/запрета синхронного ввода
int TWorkX502::SetSyncDin(AnsiString &_msg) {
	int err = -1;
	if (hndX502 != NULL) {
		err = SetSyncDinStream(x502EnableSyncDin);
		if (err != X502_ERR_OK) {
			_msg = "TWorkX502::setSyncDin Ошибка разрешения/запрета синхронного ввода: " + String(X502_GetErrorString(err));
			TLog::SaveStrLog(_msg);
			// MessageDlg(msg, mtError, TMsgDlgButtons() << mbOK, NULL);
		}
	}
	return err;
}

// ---------------------------------------------------------------------------
// асинхронного ввода с цифровых линий
int TWorkX502::GetAsyncDigIn(AnsiString &_msg) {
	int err = -1;
	if (hndX502 != NULL) {
		uint32_t din;
		err = X502_AsyncInDig(hndX502, &din);
		if (err != X502_ERR_OK) {
			_msg = "Ошибка асинхронного ввода с цифровых линий: " + String(X502_GetErrorString(err));
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
// вывода на ЦАП
int TWorkX502::SetAsyncDac1(double _valOut, AnsiString &_msg) {
	int err = -1;
	try {

		if (_valOut < 0 || _valOut > 5) {
			err = -2;
			_msg = "TWorkX502::setAsyncDac1 Напряжение  должно быть в диапазоне от 0 до 5 Вольт!";
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
				AnsiString _msg = "TWorkX502::setAsyncDac1  Ошибка вывода на ЦАП 1: " + String(X502_GetErrorString(err));
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
			_msg = "TWorkX502::setAsyncDac2 Напряжение  должно быть в диапазоне от 0 до 5 Вольт!";
			return err;
		}
		else {
			//
		}
		if (hndX502 != NULL) {
			double val = _valOut;
			err = X502_AsyncOutDac(hndX502, X502_DAC_CH2, val, X502_DAC_FLAGS_CALIBR | X502_DAC_FLAGS_VOLT);
			if (err != X502_ERR_OK) {
				_msg = "TWorkX502::setAsyncDac2 Ошибка вывода на ЦАП 2 : " + String(X502_GetErrorString(err));
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
// забираем кадр в вольтах
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
			_msg = " Ошибка приема кадра АЦП : " + String(X502_GetErrorString(err));
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
	// заглушка
	// refreshDeviceList();
}
// ---------------------------------------------------------------------------

void TWorkX502::ChCntChange(int _ChCnt) {
	// заглушка
}

// ---------------------------------------------------------------------------

void TWorkX502::LoadX502Settings(AnsiString _pathUDL, AnsiString &_msg) {
	// загрузим настройки X502

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
		// хоть там в принципе и должна быть только одна запись
		strSql += " FROM SettingsX502 where rec_id=1";
		DataModuleDB->ADOQueryDB->Close();
		DataModuleDB->ADOQueryDB->SQL->Text = strSql;
		DataModuleDB->ADOQueryDB->Open();

		while (!DataModuleDB->ADOQueryDB->Eof) {
			x502AdcFreq = DataModuleDB->ADOQueryDB->FieldByName("x502AdcFreq")->AsInteger;
			x502DinFreq = DataModuleDB->ADOQueryDB->FieldByName("x502DinFreq")->AsInteger;
			x502AdcFreqLch = 9999; // DataModuleDB->ADOQueryDB->FieldByName("x502AdcFreqLch")->AsInteger;  один фиг читать и записывать бесполезно, формируется всегда заново
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
			_msg = "Физических каналов меньше чем логических!!!";
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
	// запишем настройки
	try {
		if (DataModuleDB->ADOConnectionDB->Connected) {
			//
		}
		else {
			DataModuleDB->ADOConnectionDB->ConnectionString = "FILE NAME=" + _pathUDL;
			DataModuleDB->ADOConnectionDB->Open();
		}
		// все настройки только в первой записи таблицы других быть в принципе не должно
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
		DataModuleDB->ADOQueryDB->Parameters->ParamByName("x502FLAGS_VOLT")->Value = 1; // всегда в вольтах
		// DataModuleDB->ADOQueryDB->Parameters->ParamByName("x502ADC_RANGE_V")->Value = x502ADC_RANGE_V;
		DataModuleDB->ADOQueryDB->Parameters->ParamByName("x502ADC_RANGE_V")->Value = 0; // дианазон до 10 вольт
		DataModuleDB->ADOQueryDB->Parameters->ParamByName("x502ADC_RANGE_INT")->Value = 0; // пока не используем
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

// установка цифровых входов в 1
void TWorkX502::SetDITo1(int _typeCard, bool _lowBytes, bool _hiBytes) {
	if (hndX502 != NULL) {
		int32_t err;
		// на E502 не привязываются!
		// если это E502, то ругаемся и выходим
		if (_typeCard == 1 || _typeCard == 2) {
			MessageDlg(" Ошибка привязки входов к<1> : на E502 не привязываются !", mtError, TMsgDlgButtons() << mbOK, NULL);
			return;
		}
		else {
			err = X502_SetDigInPullup(hndX502, X502_PULLUPS_DI_H);
			err = X502_SetDigInPullup(hndX502, X502_PULLUPS_DI_L);
		}

		if (err != X502_ERR_OK) {
			MessageDlg(" Ошибка привязки входов к<1> : " + String(X502_GetErrorString(err)), mtError, TMsgDlgButtons() << mbOK, NULL);
		}
		else {
			MessageDlg(" Привязка входов к<1>произведена ", mtInformation, TMsgDlgButtons() << mbOK, NULL);
		}
	}

}

int TWorkX502::SetAsyncDac(int _numADC, double _valOut, AnsiString &_msg) {
	int err = -1;
	if (_valOut < 0 || _valOut > 5) {
		err = -2;
		MessageDlg("Ошибка вывода на ЦАП " + IntToStr(_numADC) + ": Напряжение  должно быть в диапазоне от 0 до 5 Вольт!", mtError, TMsgDlgButtons() << mbOK, NULL);
		return err;
	}
	else {
		//
	}
	if (hndX502 != NULL) {
		double val = _valOut;
		err = X502_AsyncOutDac(hndX502, (_numADC - 1), val, X502_DAC_FLAGS_CALIBR | X502_DAC_FLAGS_VOLT);
		if (err != X502_ERR_OK) {
			_msg = " Ошибка вывода на ЦАП " + IntToStr(_numADC) + " : " + String(X502_GetErrorString(err));
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
			// если еще настроек нет, то надо выдать ошибку
			_msg = "если еще настроек нет, то надо выдать ошибку";
			MessageDlg(_msg, mtError, TMsgDlgButtons() << mbOK, NULL);
			err = -2;
			return err;
		}
		else {
			//
		}
		if (wwProgramSettings->loadPRGSettings(applPath + "\\connectSQL.udl") < 0) {
			err = -3;
			_msg = "Не удалось загрузить программные настройки";
			MessageDlg(_msg, mtError, TMsgDlgButtons() << mbOK, NULL);
			return err;
		}
		else {
			//

		}
		// int _countZones, int _firstSensorsThick, int _countSensorsThick,   int _firstSensorsCross, int _countSensorsCross, int _wdCountLOGchX502, int _wdFullSensorSize
		//workData = new TWorkData(wwProgramSettings->maxZones, wwProgramSettings->firstSensorsThick, wwProgramSettings->countThickSensors, wwProgramSettings->firstSensorsCross,
		//	wwProgramSettings->countCrossSensors, x502CountLOGch, x502ADCBufSensorSize, wwProgramSettings, adc_Range_Value);
		_msg = "Гуд!";
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
	// /** Диапазоны измерения для канала АЦП */
	// typedef enum {
	// X502_ADC_RANGE_10 = 0,
	// /** < Диапазон +/-10V */
	// X502_ADC_RANGE_5 = 1,
	// /** < Диапазон +/-5V */
	// X502_ADC_RANGE_2 = 2,
	// /** < Диапазон +/-2V */
	// X502_ADC_RANGE_1 = 3,
	// /** < Диапазон +/-1V */
	// X502_ADC_RANGE_05 = 4,
	// /** < Диапазон +/-0.5V */
	// X502_ADC_RANGE_02 = 5
	// /** < Диапазон +/-0.2V */
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
	// выставим скорость
	err = SetAsyncDac2(_speed, _msg);
	// запуск двигателя
	err = SetAsyncDigOut(_codeDOutDrive, _msg);
	Sleep(200);
	// отпустим кнопку
	err = SetAsyncDigOut(0, _msg);
	return err;
}

int TWorkX502::StopDrive(int _codeDOut, AnsiString &_msg) {
	int err = -1;
	// сбросим скорость
	err = SetAsyncDac2(0, _msg);
	// стоп двигателя
	err = SetAsyncDigOut(_codeDOut, _msg);
	Sleep(200);
	// отпустим кнопку
	err = SetAsyncDigOut(0, _msg);
	// дождемся исполнения
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
	// полный сброс и останов
	X502_StreamsStop(hndX502);
	err = SetAsyncDac1(0, _msg);
	err = SetAsyncDac2(0, _msg);
	// нажмем кнопку старта
	err = SetAsyncDigOut(1, _msg);
	Sleep(100);
	// отпустим кнопку
	err = SetAsyncDigOut(0, _msg);
	// дождемся исполнения
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

// старт потоков
int TWorkX502::SYNK_X502StreamsStart(t_x502_hnd _hndX502) {
	int err = -1;
	err = X502_StreamsStart(_hndX502);
	return err;
}

// стоп потоков
int TWorkX502::SYNK_X502StreamsStop(t_x502_hnd _hndX502) {
	int err = -1;
	err = X502_StreamsStop(_hndX502);
	return err;
}

// -------------------
// вылавливаем цифровые входы
int TWorkX502::GetAsyncValuesDigIn(int _valuesIN[32], AnsiString &_msg) {
	int err = -1;
	// вылавливаем цифровые входы
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
						// устанавливаем соответствующий цифловой выход
						// pWorkX502->setAsyncDigOut(i);
					}
					else {
						_valuesIN[i] = 0;
						// сбрасываем соответствующий цифловой выход
						// pWorkX502->setAsyncDigOut(0);
					};
					// прощупаем сдвигом каждый бит
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
		// _msg = "Пустышка";
	}
	return err;
}

// -----------------
// выставим выходы согласно массиву
int TWorkX502::SetAsyncValuesDigOUT(int _valuesOUT[32], AnsiString &_msg) {
	int err = -1;
	int val = 0;
	try {
		for (int i = 0; i < 32; i++) {
			// x = pow(base, exp), где base - значение, exp - степень возведения
			if (_valuesOUT[i] == 1) {
				val = val + (int)pow(2.0, i);
			}
			else {
				//
			}
		}
		// асинхронного вывода на цифровые линии
		err = SetAsyncDigOut(val, _msg);
	}
	catch (Exception *ex) {
		err = -55;
		TLog::ErrFullSaveLog(ex);

		// TPr::prWrite(strCurrentDT+AnsiString("PA: ") + ex->Message)
	}
	return err;
}

// очистим массив управления
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
				err = -999; // ShowBigModalMessage("ПРЕВЫШЕНИЕ ТОКА!!! " +_driveName, clRed);
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
