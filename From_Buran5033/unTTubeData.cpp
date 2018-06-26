// ---------------------------------------------------------------------------

#pragma hdrstop

#include "unTTubeData.h"
#include "unTUtils.h"
#include "unTExtFunction.h"
#include <memory>
// --------------
#define strStatus "Статус: "
// ---------------------------------------------------------------------------
#pragma package(smart_init)

TTubeData::TTubeData(int _countZones, int _firstSensorsThick, int _countSensorsThick, int _firstSensorsCross, int _countSensorsCross, int _wdCountLOGchX502, int _wdFullSensorSize,
	TGlobalSettings* _programSettings, double *_wdZone_ADC_Range_Value) {
	try {
		setlocale(LC_ALL, "Russian");
		wdFirstSensorsThick = _firstSensorsThick;
		wdCountSensorsThick = _countSensorsThick;
		wdFirstSensorsCross = _firstSensorsCross;
		wdCountSensorsCross = _countSensorsCross;
		wdFullSensorSize = _wdFullSensorSize;
		// applPath = ExtractFilePath(Application->ExeName);
		wdProgramSettings = _programSettings;
		wdCountLOGchX502 = _wdCountLOGchX502;
		countZonesAll = 0;
		countUsedZones = 0;
		vectZones.reserve(_countZones);
		// добавим зоны
		for (int i = 0; i < _countZones; i++) {
			AddNewZone(i, wdFullSensorSize, wdFirstSensorsThick, wdCountSensorsThick, wdFirstSensorsCross, wdCountSensorsCross, wdCountLOGchX502, _wdZone_ADC_Range_Value);
		}
	}
	catch (Exception *ex) {
		TLog::ErrFullSaveLog(ex);
		// wdProgramSettings->colorMSG = wdProgramSettings->colorSelZone;
		// TExtFunction::UpdateStatusBar(wdProgramSettings->gsStatusBar, strStatus, ex->Message, wdProgramSettings->colorMSG);
		// MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);
	}
};

TTubeData::~TTubeData(void) {
	try {
		// Здесь обход вектора
		// for (vector<TStorageZoneData>::iterator it = vectDataZones.begin(); it != vectDataZones.end(); it++) {
		// TStorageZoneData* szd = static_cast<TStorageZoneData*>(vectDataZones[it]);
		// delete szd;
		// };
		// int k = vectDataZones.size();
		// перед тем, как очистить вектор, необходимо прибить объекты
		for (int i = 0; i < vectZones.size(); i++) {
			delete vectZones[i];
			// vectDataZones[i] = NULL;
		}
		vectZones.clear();
		// delete vectDataZones;
		// vectDataZones=NULL;
	}
	catch (Exception *ex) {
		TLog::ErrFullSaveLog(ex);
		// wdProgramSettings->colorMSG = wdProgramSettings->colorSelZone;
		// TExtFunction::UpdateStatusBar(wdProgramSettings->gsStatusBar, strStatus, ex->Message, wdProgramSettings->colorMSG);
		// MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);
	}

};

int TTubeData::AddNewZone(int _zoneNum, int _fullSensorSize, int _firstSensorsThick, int _countSensorsThick, int _firstSensorsCross, int _countSensorsCross, int _countLogCh,
	double *_wdZone_ADC_Range_Value) {
	int err = -1;
	try {
		// новый класс зоны
		TZoneData* newZone = NULL;
		// newZone = new TZoneData(_zoneNum, wdFullSensorSize, wdFirstSensorsThick, wdCountSensorsThick, wdFirstSensorsCross, wdCountSensorsCross, wdCountLOGchX502);
		// TZoneData(TGlobalSettings* zProgramSettings,int _zoneNum,int _fullSensorSize,int _countLogCh)
		newZone = new TZoneData(wdProgramSettings, _zoneNum, wdFullSensorSize, wdCountLOGchX502, _wdZone_ADC_Range_Value);
		// dataAll.resize(_zoneNum + 1, newZone);
		// запихиваем в вектор зон
		vectZones.push_back(newZone);
		int k = vectZones.size();
		// vectDataZones.insert(vectDataZones.end(), newZone);
		countZonesAll++;
		err = 0;
	}
	catch (Exception *ex) {
		err = 2;
		TLog::ErrFullSaveLog(ex);
		wdProgramSettings->colorMSG = wdProgramSettings->colorSelZone;
		TExtFunction::UpdateStatusBar(wdProgramSettings->gsStatusBar, strStatus, ex->Message, wdProgramSettings->colorMSG);
		TExtFunction::UpdateLabelStatus(wdProgramSettings->gsStatusLabel, strStatus, ex->Message, wdProgramSettings->colorMSG);
		// MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);
	}
	return err;
}

// внимательней к размеру загружаемого массива!!! int _needZoneAdd
int TTubeData::AddPartZone(int _zoneNum, TZoneData* _zoneData, double* _adcData, int _needZoneAdd) {
	int err = -1;
	TSensorData* sensor;
	TZoneData* zone;
	int offset = _zoneData->realZoneSize;
	for (int i = 0; i < _needZoneAdd; i++) {
		_zoneData->arrDataZone[offset + i] = _adcData[i];
		_zoneData->realZoneSize++;
		if (_zoneData->realZoneSize > _zoneData->fullZoneSize) {
			break;
		}
	}
	zone = vectZones[_zoneNum];
	// если эта новая зона, то ставим признак заполненной частично или полностью и увеличиваем счетчик  используемых зон
	if (!zone->isUsed) {
		zone->isUsed = true;
		countUsedZones++;
	}
	else {
		//
	}

	try {
		// в зоне заполняем датчики
		// для всех логических каналов
		for (int nс = 0; nс < wdCountLOGchX502; nс++) {
			// пока для тестирования
			int lch = _zoneData->countLogCh;
			// ----------
			sensor = static_cast<TSensorData*>(_zoneData->vectSensors[nс]);
			int offset = sensor->realSensorSize;
			int end = (int)_needZoneAdd / wdCountLOGchX502;
			for (int nd = 0; nd < end; nd++) {
				// номер канала + номер элемента  * количество каналов
				int kk = nс + nd * wdCountLOGchX502;
				sensor->vecSensorData[offset + nd] = _adcData[nс + nd * wdCountLOGchX502];
			}
			// дополним размер сенсора
			sensor->realSensorSize = sensor->realSensorSize + end;
		}
		err = 0;

	}
	catch (Exception *ex) {
		err = -2;
		TLog::ErrFullSaveLog(ex);
		wdProgramSettings->colorMSG = wdProgramSettings->colorSelZone;
		TExtFunction::UpdateStatusBar(wdProgramSettings->gsStatusBar, strStatus, ex->Message, wdProgramSettings->colorMSG);
        TExtFunction::UpdateLabelStatus(wdProgramSettings->gsStatusLabel, strStatus, ex->Message, wdProgramSettings->colorMSG);
		// MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);
	}
	return err;
}

int TTubeData::GetSensorArrayData(int _zoneNum, int _sensorNum, double* _arrayData, int _beginRead, int _needRead, AnsiString &_msg) {
	int err = -1;
	try {
		// int size = vectDataZones[_zoneNum]-> vectDataZones[_sensorNum]->countLogCh;
		for (int i = 0; i < vectZones[_zoneNum]->vectSensors[_sensorNum]->realSensorSize; i++) {
			_arrayData[i] = vectZones[_zoneNum]->vectSensors[_sensorNum]->vecSensorData[i];
		}
		err = 0;
	}
	catch (Exception *ex) {
		err = -2;
		TLog::ErrFullSaveLog(ex);
		_msg = ex->Message;
		// TExtFunction::UpdateStatusBar(StatusBarBottom, strStatus, ex->Message, programSettings.colorBrak);
		// MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);
	}

	return err;
}

int TTubeData::LoadFromFileTxt(AnsiString _fullFileName) {
	int err = -1;
	try {
		// -
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

// формат записи
// заголовок Количество ЗОН - только в начале файла
// -ZonesCount: (двузначное число) + перенос строки
// номер зоны
// -ZoneN: (двузначное число) + перенос строки - повторяется по зонам
// тип датчика,номер датчика, полный размер данных датчика, реальный размер данных датчика - повторяется по датчикам
// первыми идут данные по поперечнику -SС:(номер)-FullSize:(размер блока)-RealSize:(размер блока) + перенос строки
// данные по датчику
// ..........
int TTubeData::SaveToFileTxt(AnsiString _fullFileName) {
	int err = -1;
	AnsiString str = "";
	try {
		if (countUsedZones < 10) {
			str += "ZonesCount:0" + IntToStr(countUsedZones);
		}
		else {
			str += "-ZonesCount:" + IntToStr(countUsedZones);
		}

		// зоны
		for (int z = 0; z < countUsedZones; z++) {
			// str += "Zone: " + IntToStr(z) + "\n";
			if (z < 10) {
				str += "-ZoneN:0" + IntToStr(z) + "\n";
			}
			else {
				str += "-ZoneN:" + IntToStr(z) + "\n";
			}
			// сенсоры
			for (int s = 0; s < vectZones[z]->countSensors; s++) {
				if (true) {
					str += "-SС:";
				}
				else {
					str += "-ST:";
				}
				int fsz = vectZones[z]->vectSensors[0]->fullSensorSize;
				int rsz = vectZones[z]->vectSensors[0]->realSensorSize;
				str += IntToStr(s) + "-FullSize:" + IntToStr(fsz) + "-RealSize:" + IntToStr(rsz) + "\n";
				for (int j = 0; j < fsz; j++) {
					// данные по сенсору с точностью до микровольт :)
					str += FloatToStrF(vectZones[z]->vectSensors[s]->vecSensorData[j], ffFixed, 6, 6) + ";";
				}
				str += "\n";
			}
			// str += "\n";
		}
		TLog::SaveStrTxtFile(_fullFileName, str);
		str = "";
		err = 0;
	}
	catch (Exception *ex) {
		err = -2;
		AnsiString emsg = "";
		emsg = ex->ClassName() + " ";
		emsg += ex->Message;
		TLog::SaveStrLog(emsg);
		// TExtFunction::UpdateStatusBar(StatusBarBottom, strStatus, ex->Message, programSettings.colorBrak);
		// MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);
	}
	return err;
}

int TTubeData::ResetAllData(AnsiString &_msg) {
	int err = -1;
	try {
		for (int i = 0; i < countZonesAll; i++) {
			vectZones[i]->ResetZone();
			if (countUsedZones > 0) {
				countUsedZones--;
			}
			else {
				//
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
	return err;
}

//
int TTubeData::TstResetData(AnsiString &_msg) {
	int err = -1;
	try {
		for (int i = 0; i < countZonesAll; i++) {
			// vectZones[i]->ResetZone();
			// заливаем 0 по данным зоны
			for (int n = 0; n < vectZones[i]->fullZoneSize; n++) {
				vectZones[i]->arrDataZone[n] = 0;
			}
			vectZones[i]->distribute = false;
			vectZones[i]->realZoneSize = 0;
			for (int k = 0; k < vectZones[i]->countLogCh; k++) {
				// чистим датчики
				for (int m = 0; m < vectZones[i]->vectSensors[k]->fullSensorSize; m++) {
					vectZones[i]->vectSensors[k]->vecSensorData[m] = 0;
				}
				vectZones[i]->vectSensors[k]->realSensorSize = 0;
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
	return err;
}

// ------------------------
// а запишем-ка мы зоны в бинарник
// формат записи
// заголовок количество зон,
// номер перводо датчика толщиномера, количество датчиков толщиномера
// номер перводо датчика поперечника, количество датчиков поперечника
// полный размер датчика
// конец заголовка
// ZC:ХХ FST:XX ST:XX FSC:XX SC:XX FSS:XXXX-EOH
int TTubeData::SaveTubeToFileBin(AnsiString _fullFileName) {
	int err = -1;
	AnsiString str = "";
	int binFileHandle = -1;
	try {
		try {
			// заголовок
			if (countUsedZones < 10) {
				str += "ZC:0";
			}
			else {
				str += "ZC:";
			}
			str += IntToStr(countUsedZones);
			if (wdFirstSensorsThick < 10) {
				str += " FST:0";
			}
			else {
				str += " FST:";
			}
			str += IntToStr(wdFirstSensorsThick);
			if (wdCountSensorsThick < 10) {
				str += " ST:0";
			}
			else {
				str += " ST:";
			}
			str += IntToStr(wdCountSensorsThick);
			if (wdFirstSensorsCross < 10) {
				str += " FSC:0";
			}
			else {
				str += " FSC:";
			}
			str += IntToStr(wdFirstSensorsCross);
			if (wdCountSensorsCross < 10) {
				str += " SC:0";
			}
			else {
				str += " SC:";
			}
			str += IntToStr(wdCountSensorsCross);
			if (wdFullSensorSize < 10000) {
				if (wdFullSensorSize < 1000) {
					str += " FSS:00";
				}
				else {
					str += " FSS:0";
				}
			}
			else {
				str += " FSS:";
			}
			str += IntToStr(wdFullSensorSize);
			str += "-EOH:";
			setlocale(LC_ALL, "Russian");
			if (!FileExists(_fullFileName)) {
				binFileHandle = FileCreate(_fullFileName, FILE_WRITE_DATA, FILE_SHARE_WRITE);
			}
			else {
				binFileHandle = FileOpen(_fullFileName, fmOpenRead | fmOpenWrite | fmShareDenyNone);
			}
			err = FileWrite(binFileHandle, str.c_str(), str.Length());
			for (int z = 0; z < countUsedZones; z++) {
				// // SaveTxtArrDoubleFile(AnsiString _fullFileName, AnsiString _str, double *_arr, int _arrSize)
				// // SaveTxtArrDoubleFile(_fullFileName, str, vectZones[z]->arrDataZone, vectZones[z]->fullZoneSize);
				// FileSeek(binFileHandle, 0, 2);
				err = sizeof(double);
				err = FileWrite(binFileHandle, vectZones[z]->arrDataZone, vectZones[z]->fullZoneSize*sizeof(double));
				// // другой вариант  использовать в стиле C
				// // #include <stdio.h>
				// // #include <time.h>
				//
				// // FILE *f = fopen("c:\\file.bin", "wb");
				// // fprintf(f, " file : %s %c %d %I64 %f %7.2f", "строка :", 'c', 12345, time(0), 1.2, -34.23);
				// // fclose(f);
			}
			// str = "";
			err = 0;
		}

		catch (Exception *ex) {
			// err = -2;
			// TLog::ErrFullSaveLog(ex);
		}
		// return err;
	}
	__finally {
		if (binFileHandle > 0) {
			FileClose(binFileHandle);
		}
		else {
			//
		}
	}

}

// ------------------------
int TTubeData::LoadTubeFromFileBin(AnsiString _fullFileName) {
	int err = -1;
	int binFileHandle = -1;
	int iFileLength = 0;
	int iBytesRead = 0;
	char* charBuffer = NULL;
	int countCharBuffer = 128;
	// --------------- размер заголовка не более ХХ символов
	int headLength = 0; // реальная длина заголовка
	double* dlbBuffer = NULL;
	int countDlbBuf = 0;
	// сколько цифр надо вычитать
	int readInt = 0;
	int countZones = 0;
	int firstSensorThick;
	int sensorsThick;
	int firstSensorCross;
	int sensorsCross;
	int sensorSize;
	// AnsiString nameFile = applPath + "\\LOGs\\errLog_" + strCurrentD + ".log";
	AnsiString nameFile = _fullFileName;
	AnsiString strZ = "";
	AnsiString tmpStr = "";
	try {
		try {

			if (!FileExists(_fullFileName)) {
				tmpStr = "Не найден: " + _fullFileName;
				MessageDlg(tmpStr, mtError, TMsgDlgButtons() << mbOK, NULL);
				TLog::SaveStrLog(tmpStr);
				err = -11;
			}
			else {
				binFileHandle = FileOpen(_fullFileName, fmOpenRead);
				// binFileHandle = FileOpen(_fullFileName, fmOpenRead | fmOpenWrite | fmShareDenyNone);
				iFileLength = FileSeek(binFileHandle, 0, 2);
				FileSeek(binFileHandle, 0, 0);
				// std::tr1::shared_ptr<>
				// std::shared_ptr
				// auto ptr = std::make_shared<Foo>();
				// std::auto_ptr<char>charBuffer(new char[iFileLength + 1]);
				std::unique_ptr<char>charBuffer(new char[countCharBuffer]);
				// iBytesRead = FileRead(binFileHandle, charBuffer.get(), countCharBuffer);
				// --------------- разберем заголовок
				// ZC:ХХ FST:XX ST:XX FSC:XX SC:XX FSS:XXXX-EOH:
				// charBuffer = new char[countCharBuffer + 1];
				iBytesRead = FileRead(binFileHandle, charBuffer.get(), countCharBuffer);
				int iH = 0;
				while (iH < countCharBuffer) {
					strZ += charBuffer.get()[iH];
					iH++;
				}
				// ----- countZones
				tmpStr = strZ.SubString(4, 2);
				countZones = StrToInt(tmpStr);
				if (countZones != countZonesAll) {
					MessageDlg("Количество зон в файле \n и в настройках не совпадают! \n В файле: " + IntToStr(countZones) + " в настройках: " + IntToStr(countZonesAll),
						mtWarning, TMsgDlgButtons() << mbOK, NULL);
					// return err;
				}
				else {
					//
				}
				// --------------firstSensorThick
				tmpStr = strZ.SubString(11, 2);
				firstSensorThick = StrToInt(tmpStr);
				if (firstSensorThick != wdFirstSensorsThick) {
					MessageDlg("Первый датчик толщиномера в файле \n и в настройках не совпадают! \n В файле: " + IntToStr(firstSensorThick) + " в настройках: " +
						IntToStr(wdFirstSensorsThick) + "\n Загрузка детализации отменена!", mtError, TMsgDlgButtons() << mbOK, NULL);
					err = -2;
					return err;
				}
				else {
					//
				}
				// --------------sensorsThick
				tmpStr = strZ.SubString(17, 2);
				sensorsThick = StrToInt(tmpStr);
				if (sensorsThick != wdCountSensorsThick) {
					MessageDlg("Число датчиков толщиномера в файле \n и в настройках не совпадают! \n В файле: " + IntToStr(sensorsThick) + " в настройках: " +
						IntToStr(wdCountSensorsThick) + "\n Загрузка детализации отменена!", mtError, TMsgDlgButtons() << mbOK, NULL);
					err = -3;
					return err;
				}
				else {
					//
				}
				// --------------firstSensorCross
				tmpStr = strZ.SubString(24, 2);
				firstSensorCross = StrToInt(tmpStr);
				if (firstSensorCross != wdFirstSensorsCross) {
					MessageDlg("Первый датчик поперечника в файле \n и в настройках не совпадают! \n В файле: " + IntToStr(firstSensorCross) + " в настройках: " +
						IntToStr(wdFirstSensorsCross) + "\n Загрузка детализации отменена!", mtError, TMsgDlgButtons() << mbOK, NULL);
					err = -4;
					return err;
				}
				else {
					//
				}
				// -----sensorsCross
				tmpStr = strZ.SubString(30, 2);
				sensorsCross = StrToInt(tmpStr);
				if (sensorsCross != wdCountSensorsCross) {
					MessageDlg("Первый датчик поперечника в файле \n и в настройках не совпадают! \n В файле: " + IntToStr(sensorsCross) + " в настройках: " +
						IntToStr(wdCountSensorsCross) + "\n Загрузка детализации отменена!", mtError, TMsgDlgButtons() << mbOK, NULL);
					err = -5;
					return err;
				}
				else {
					//
				}
				// ---- sensorSize
				tmpStr = strZ.SubString(37, 5);
				sensorSize = StrToInt(tmpStr);
				if (sensorSize != wdFullSensorSize) {
					MessageDlg("Размер буфера датчика в файле \n и в настройках не совпадают! \n В файле: " + IntToStr(sensorSize) + " в настройках: " +
						IntToStr(wdFullSensorSize) + "\nЗагрузка детализации отменена!", mtError, TMsgDlgButtons() << mbOK, NULL);
					err = -6;
					return err;
				}
				else {
					//
				}
				// Длина заголовка
				headLength = strZ.Pos("-EOH:") + 4;
				// убежим в начало
				FileSeek(binFileHandle, 0, 0);
				// смещаемся на начало данных
				FileSeek(binFileHandle, headLength, 0);
				int szDbl = sizeof(double);
				// -----------размер буфера по по файлу в  double
				countDlbBuf = (int)(sensorSize * wdCountLOGchX502);
				dlbBuffer = new double[countDlbBuf];
				// pszBuffer = PChar(AllocMem(iFileLength + 1));
				// iBytesRead = FileRead(binFileHandle, pszBuffer, iFileLength);
				// очистим зоны
				// ResetAllData(lastMsg);
				for (int i = 0; i < countZones; i++) {
					// байт читать надо countDlbBuf*szDbl - учитывать размерность double
					iBytesRead = FileRead(binFileHandle, dlbBuffer, countDlbBuf * szDbl);
					int need = vectZones[i]->fullZoneSize - vectZones[i]->realZoneSize;
					if (need > countDlbBuf) {
						need = countDlbBuf;
					}
					else {
						need = vectZones[i]->fullZoneSize - vectZones[i]->realZoneSize;
					}
					AddPartZone(i, vectZones[i], dlbBuffer, need);
					err = iBytesRead;
				}
			}
			delete charBuffer;
			delete dlbBuffer;
			err = 0;
		}
		catch (Exception *ex) {
			err = -20;
			TLog::ErrFullSaveLog(ex);
			// TExtFunction::UpdateStatusBar(StatusBarBottom, strStatus, ex->Message, programSettings.colorBrak);
			MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);

		}
	}
	__finally {
		if (binFileHandle > 0) {
			FileClose(binFileHandle);
		}
		else {
			//
		}
	}
	return err;
}

// для тестовых зон - перезаписываем файл
int TTubeData::RefreshFile(AnsiString _fullFileName) {
	int err = -1;
	int binFileHandle = -1;
	try {
		if (FileExists(_fullFileName)) {
			DeleteFile(_fullFileName);
		}
		else {
			//
		}
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

// ------------------------
int TTubeData::Empty(void) {
	int err = -1;
	try {
		// -
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
