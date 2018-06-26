// ---------------------------------------------------------------------------

#pragma hdrstop

#include "x502_thread.h"
#include "unTUtils.h"
#include "unTExtFunction.h"
// ---------------------------------------------------------------------------

#pragma package(smart_init)
// 8388608
// #define RECV_BUF_SIZE  8*1024*1024
// 512000 байт
// #define RECV_BUF_SIZE  8*32*2000
// ожидание 250 мс
#define RECV_TOUT      250
#define strStatus "Статус: "

__fastcall X502_ProcessThread::X502_ProcessThread(bool CreateSuspended, int _countLogCh, int _adcFreqLch, uint32_t _rcvSizeBufSensor, TTubeData* _tubeData,
	TGlobalSettings *_thProgramSettings) : TThread(CreateSuspended), stopThread(false), err(X502_ERR_OK) {
	tmpInt = 0;
	threadCountLogCh = _countLogCh;
	threadAdcFreqLch = _adcFreqLch;
	// размер буфера возьмем исходя из количества лог каналов и частоты сбора на канал
	threadRcvSizeBufZone = _rcvSizeBufSensor * _countLogCh;
	threadTubeData = _tubeData;
	thProgramSettings = _thProgramSettings;
	isNewZone = false;
	currentZone = 0;
	//treadADCdouble0 = new double[X502_LTABLE_MAX_CH_CNT];
}

// ---------------------------------------------------------------------------
__fastcall X502_ProcessThread::~X502_ProcessThread(void) {
	// Stop();
	Terminate();
	WaitFor();
    //delete treadADCdouble0;
}

// обновление индикаторов формы результатами последнего измерения.
// Метод должен выполняться только через Syncronize
void __fastcall X502_ProcessThread::updateData() {
	int err = -1;
	uint32_t lch_cnt = 0;
	bool wrfile = false;
	try {
		err = X502_GetLChannelCount(threadHndX502, &lch_cnt); // проверка по числу каналов, должно совпадать с threadCountCh
		if (err == X502_ERR_OK) {
			// забираем значение первого отсчета из массива
			// for (uint32_t i = 0; (i < lch_cnt) && (i < adcSize); i++)
			// lchResEdits[(firstLch + i) % lch_cnt]->Text = FloatToStrF(adcData[i], ffFixed, 4, 8);
			uint32_t n = 0;
			for (uint32_t i = 0; (i < lch_cnt) && (i < adcSize); i++) {
				n = (firstLch + i) % lch_cnt;
				// это для баловства
				// пять массивов, в которых данные по каналу с отступом
				*treadADCdouble0[n] = adcData[i + lch_cnt]; // 1-й кадр
				// if ((adcSize / lch_cnt) > 100) {
				// *treadADCdouble1[n] = adcData[i + lch_cnt * 10]; // десятый кадр
				// *treadADCdouble2[n] = adcData[i + lch_cnt * 25];
				// *treadADCdouble3[n] = adcData[i + lch_cnt * 50];
				// *treadADCdouble4[n] = adcData[i + lch_cnt * 100];
				// }
				// else {
				// *treadADCdouble1[n] = 1.0; // десятый кадр
				// *treadADCdouble2[n] = 2.5;
				// *treadADCdouble3[n] = 5.0;
				// *treadADCdouble4[n] = 10.0;
				// }
				//
			}
			// если есть данные цифрового выхода, то устанавливаем индикатор
			// dinResEdit->Text = IntToHex((int)dinData[0] & 0x3FFFF, 5);
			// дошли до максимума зон надо еще озадачиться временем
			if (currentZone < thProgramSettings->maxZones && thelaps_time < thProgramSettings->tubeTime) {
				thend_time = clock(); // конечное время
				thelaps_time = thend_time - thstart_time; // искомое время
			}
			else {
				stopThread = true;
				thProgramSettings->needData = false;
                thend_time = clock();
				return;
			}
			// если нет необходимости заполнения зон
			if (!thProgramSettings->needData) {
				return;
			}
			else {
				// adcSize расчитываем заранее исходя из количества в буфере и нужного для заполнения зоны количества
				threadTubeData->AddPartZone(currentZone, threadTubeData->vectZones[currentZone], adcData, adcSize);
				if (threadTubeData->vectZones[currentZone]->fullZoneSize == threadTubeData->vectZones[currentZone]->realZoneSize) {
					// если предыдущая зона заполнена, начинаем новую
					isNewZone = true;
					currentZone++;
				}
				else {
					isNewZone = false;
				}
			}
			// threadWorkData->vectDataZones[currentZone]->isUsed = true;
			if (isNewZone) {
				if (wrfile) { // для отладки
					AnsiString str = "";
					str += "Zone: " + IntToStr(currentZone) + "\n";
					str += "Sensor0Full: " + IntToStr(threadTubeData->vectZones[currentZone]->vectSensors[0]->fullSensorSize) + "\n";
					str += "Sensor0Real: " + IntToStr(threadTubeData->vectZones[currentZone]->vectSensors[0]->realSensorSize) + "\n";
					for (int j = 0; j < threadTubeData->vectZones[currentZone]->vectSensors[0]->fullSensorSize; j++) {
						double sdouble = threadTubeData->vectZones[currentZone]->vectSensors[0]->vecSensorData[j];
						str = str += FloatToStrF(sdouble, ffFixed, 6, 6) + ";";

					}
					str += "\n";
					TLog::SaveStrTxtFile("testA.txt", str);
				}
				else {
					//
				}
			}
			else {
				// currentZone++;
			}

			if (dinSize) {
				// lch_cnt=dinData[0];
				*threadSyncDIN0 = (int)dinData[0];
			}
			else {
				*threadSyncDIN0 = 0;
			}
			err = 0;
		}
		else {
			err = -3;
		}
	}
	catch (Exception *ex) {
		err = -2;
		thProgramSettings->needData = false;
		TLog::ErrFullSaveLog(ex);
		thProgramSettings->colorMSG=thProgramSettings->colorBrak;
		TExtFunction::UpdateStatusBar(thProgramSettings->gsStatusBar, strStatus, ex->Message,thProgramSettings->colorMSG );
		TExtFunction::UpdateLabelStatus(thProgramSettings->gsStatusLabel, strStatus, ex->Message, thProgramSettings->colorMSG);
		MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);
	}
}

void __fastcall X502_ProcessThread::Execute() {
	if (ThreadRun(msgErr) < 0) {
		thProgramSettings->needData = false;
		Terminate();
		WaitFor();
	}
	else {
		//
	}
}

int X502_ProcessThread::ThreadRun(AnsiString &_msgErr) {
	int err = -1;
	/* выделяем буферы под принимаемые данные */
	uint32_t *rcv_buf = new uint32_t[threadRcvSizeBufZone]; // #define RECV_BUF_SIZE  8*1024*1024
	adcData = new double[threadRcvSizeBufZone];
	dinData = new uint32_t[threadRcvSizeBufZone];
	// сколько готово для считывания
	threadRcvCountReady = 0;
	// сколько надо до заполнения зоны
	threadRcvPartNeed = 0;
	// сколько уже cчитали на зону
	// threadRcvZoneSizeReal = 0;
	// сколько забрали за шаг цикла
	threadRcvSizeRealStep = 0;
	bool debug = false;
	try {
		try {
			if (!rcv_buf || !dinData || !adcData) {
				err = X502_ERR_MEMORY_ALLOC;
				_msgErr = "X502_ERR_MEMORY_ALLOC: " + String(X502_GetErrorString(err));
			}
			else {
				// запускаем синхронные потоки
				err = X502_StreamsStart(threadHndX502);
				if (err == X502_ERR_OK) {
					// если первый проход, то выставим признак и поспим маленько
					Sleep(50);
					thstart_time=0;
                    thstart_time = clock();
					// выполняем прием пока не произойдет ошибка или не будет запроса на останов от основного приложения
					while ((!stopThread) && (err == X502_ERR_OK)) {
						// Функция возвращает количество отсчетов, которые были приняты из модуля
						// во внутренний буфер и готовы для считывания
						err = X502_GetRecvReadyCount(threadHndX502, &threadRcvCountReady);
						// принимаем данные синхронного ввода
						// RECV_BUF_SIZE Количество считываемых отсчетов (32-битных слов)
						// функция возвратит столько отсчетов, сколько было в буфере
						// rcvSizeFact = X502_Recv(threadHndX502, rcv_buf, RECV_BUF_SIZE, RECV_TOUT);
						// ограничим пробеги по количеству зон, иначе получим ошибку доступа по несуществующему адресу
						// сколько нужно до заполнения текущей зоны
						int fullNeed = 0;
						if (currentZone < thProgramSettings->maxZones) {
							fullNeed = threadTubeData->vectZones[currentZone]->fullZoneSize - threadTubeData->vectZones[currentZone]->realZoneSize;
						}
						else {
							stopThread = true;
							break;
						}
						// -------------------
						// если готово больше или столько же, что нужно
						if (threadRcvCountReady >= fullNeed) {
							threadRcvPartNeed = fullNeed;
						}
						else {
							// если меньше, то нужно взять кратно каналам
							int k = (int)(threadRcvCountReady / threadCountLogCh);
							threadRcvPartNeed = threadCountLogCh * k;
						}
						// читаем сколько посчитаем нужным
						threadRcvSizeRealStep = X502_Recv(threadHndX502, rcv_buf, threadRcvPartNeed, RECV_TOUT);
						// threadRcvAllSizeReal = threadRcvAllSizeReal + threadRcvSizeRealStep;
						// значение меньше нуля означает ошибку...
						if (threadRcvSizeRealStep < 0) {
							err = threadRcvSizeRealStep;
							_msgErr = "Ошибка: " + String(X502_GetErrorString(err));
							TLog::SaveStrLog(_msgErr);
							stopThread = true;
							break;
						}
						else {
							if (debug) { // для отладки
								AnsiString str = "";
								str += "currentZone: " + IntToStr(currentZone) + "\n";
								str += "fullNeed: " + IntToStr(fullNeed) + "\n";
								str += " threadRcvPartNeed: " + UIntToStr(threadRcvPartNeed) + "\n";
								str += "\n";
								str += " threadRcvSizeRealStep: " + UIntToStr(threadRcvSizeRealStep);
								str += "\n";
								str += " threadTubeData->vectZones[currentZone]->realZoneSize: " + IntToStr(threadTubeData->vectZones[currentZone]->realZoneSize);
								TLog::SaveStrTxtFile("ThreadRun.txt", str);
							}
							else {
								//
							}
							// если больше нуля - значит приняли новые данные
							dinSize = threadRcvSizeRealStep;
							adcSize = threadRcvSizeRealStep;
							// получаем номер лог. канала, соответствующий первому отсчету АЦП, так как до этого могли обработать некратное количество кадров
							err = X502_GetNextExpectedLchNum(threadHndX502, &firstLch);
							if (err == X502_ERR_OK) {
								// разбираем данные на синхронный ввод и отсчеты АЦП и переводим АЦП в Вольты
								err = X502_ProcessData(threadHndX502, rcv_buf, threadRcvSizeRealStep, L502_PROC_FLAGS_VOLT, adcData, &adcSize, dinData, &dinSize);
							}
							if (err == X502_ERR_OK) {
								// обновляем значения элементов управления и глобальные переменные
								Synchronize(updateData);
							}
							else {
								thProgramSettings->needData = false;
								_msgErr = String(X502_GetErrorString(err));
								TLog::SaveStrLog(_msgErr);
								stopThread = true;
								break;
							}
						}

					}
					// по выходу из цикла останавливаем поток.Чтобы не сбросить код ошибки (если вышли по ошибке)
					// результат останова сохраняем в отдельную переменную
					int32_t stop_err = X502_StreamsStop(threadHndX502);
					if (err == X502_ERR_OK) {
						err = stop_err;
					}
					else {
						_msgErr = "Ошибка останова потока сбора: " + String(X502_GetErrorString(stop_err));
					}
					_msgErr = "По выходу из цикла: " + String(X502_GetErrorString(err));
				}
				_msgErr = "Сбор данных завершен: " + String(X502_GetErrorString(err));
				return err;
			}
		}
		catch (Exception *ex) {
			int32_t err = X502_StreamsStop(threadHndX502);
			err = -11;
			thProgramSettings->needData = false;
			TLog::ErrFullSaveLog(ex);
			_msgErr = "Ошибка: " + ex->Message;
		}
	}
	__finally {
		// освобождаем выделенную память
		delete rcv_buf;
		delete dinData;
		delete adcData;
	}
	return err;
}
