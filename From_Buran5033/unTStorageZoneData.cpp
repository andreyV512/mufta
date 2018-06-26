// ---------------------------------------------------------------------------

#pragma hdrstop

#include "unTStorageZoneData.h"
#include "unTUtils.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ������ ��� ��������  ������ ���������
// �� ����
// ���������� �� �����������
// TZoneData::TZoneData(int _zoneNum, int _fullSensorSize, int _firstSensorsThick, int _countSensorsThick, int _firstSensorsCross, int _countSensorsCross, int _countLogCh) {
// ���������� ������� - ��������� ����� �� ���������� ��������
TZoneData::TZoneData(TGlobalSettings* _zProgramSettings, int _zoneNum, int _fullSensorSize, int _countLogCh, double *_zdADC_Range_Value) {
	int err = -1;
	try {
		zProgramSettings = NULL;
		zProgramSettings = _zProgramSettings;
		zdADC_Range_Value = NULL;
		zdADC_Range_Value = _zdADC_Range_Value;
		zoneNum = _zoneNum;
		// ����������
		// firstSensorsThick = _firstSensorsThick;
		// countSensorsThick = _countSensorsThick;
		// ����������
		// firstSensorsCross = _firstSensorsCross;
		// countSensorsCross = _countSensorsCross;
		// ������� ���������� �� ����
		countLogCh = _countLogCh;
		fullZoneSize = _fullSensorSize * countLogCh;
		realZoneSize = 0;
		countSensors = 0;
		statusZone = 0;
		// ������ ������ �� ����
		arrDataZone = new double[fullZoneSize];
		isUsed = false;
		// �������� 0 �� ������ ����
		for (int n = 0; n < fullZoneSize; n++) {
			arrDataZone[n] = 0;
		}
		distribute = false;
		vectSensors.reserve(countLogCh);
		// int k = vectSensors.size();
		// � ���� ������� �������
		TSensorData* newSensor = NULL;
		for (int i = 0; i < countLogCh; i++) {
			// ������� ������ ������� ��������
			newSensor = new TSensorData(i, _fullSensorSize, countLogCh);
			vectSensors.push_back(newSensor);
			countSensors++;
		}
		err = 0;
		// vectSensors[i]=0;
		// k = vectSensors.size();
		// std::vector<int> myVector(10);
	}
	catch (Exception *ex) {
		err = -2;
		TLog::ErrFullSaveLog(ex);
		// TExtFunction::UpdateStatusBar(StatusBarBottom, strStatus, ex->Message, programSettings.colorBrak);
		// MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);

	}

}

TZoneData::~TZoneData(void) {
	try {
		// ����� ���, ��� �������� ������, ���������� ������� �������
		for (int i = 0; i < vectSensors.size(); i++) {
			delete vectSensors[i];
			// vectDataSensors[i] = NULL;
		}
		vectSensors.clear();
		delete arrDataZone;
		// a.clear() Equivalent to a.erase(a.begin(), a.end())
		// �� .clear(), �� reserve(x) � ���� ������ �� �����������.
		// ���������� ������ ������ ������ ���: std::vector().swap(vec)
	}
	catch (Exception *ex) {
		TLog::ErrFullSaveLog(ex);
	}
}

// ���������������� ������ �� �������� �������������
int TZoneData::DistributeSensorsData(void) {
	int result = -1;
	try {
		// ���� ��� �� ������������ �������������, �� ���������������� �� ��������
		if (!distribute) {
			for (int i = 0; i < countLogCh; i++) {
				TSensorData* tmpSensorData = static_cast<TSensorData*>(vectSensors[i]);
				for (int j = 0; j < fullZoneSize; j++) {
					tmpSensorData->vecSensorData[j] = arrDataZone[i + j * countLogCh];
				}
			}
			distribute = true;
		}
		else {
			//
		};
		result = 0;
	}
	catch (Exception *ex) {
		TLog::ErrFullSaveLog(ex);
	}
	return result;
}

int TZoneData::ResetZone(void) {
	int err = -1;
	try {
		realZoneSize = 0;
		isUsed = false;
		// �������� 0 �� ������ ����
		for (int n = 0; n < fullZoneSize; n++) {
			arrDataZone[n] = 0;
		}
		distribute = false;
		for (int i = 0; i < countLogCh; i++) {
			// ������ �������
			vectSensors[i]->ResetSensor();
		}
		err = 0;
	}
	catch (Exception *ex) {
		err = -2;
		TLog::ErrFullSaveLog(ex);
		// TExtFunction::UpdateStatusBar(StatusBarBottom,strStatus, ex->Message, programSettings.colorBrak);
		// MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);

	}
	return err;
}

int TZoneData::SetStatusSensors(double _crossBorder10, double _crossBorder20, double _thickBorder10, double _thickBorder20) {
	int err = -1;
	int cMax = 0;
	double valueSens = 0;
	try {
		// // �������� ����������
		int maxSen = zProgramSettings->firstSensorsThick + zProgramSettings->countThickSensors;
		for (int tSen = zProgramSettings->firstSensorsThick; tSen < maxSen; tSen++) {
			for (int i = 0; i < vectSensors[tSen]->realSensorSize; i++) {
				double valueSens0 = vectSensors[tSen]->vecSensorData[i];
				valueSens = (vectSensors[tSen]->vecSensorData[i] * zProgramSettings->gainsXY[tSen]) / zdADC_Range_Value[tSen] * 100;
				if (abs(valueSens) > _thickBorder10) {
					vectSensors[tSen]->stastusSensor = 1;// ��1
                    //���� ��� ����, �� ����� ������ � ���������
					break;
				}
				else {
					if (vectSensors[tSen]->stastusSensor == 2) {
						// ��� ������
					}
					else {
						if (abs(valueSens) > _thickBorder20) {
							vectSensors[tSen]->stastusSensor = 2; // ���2
							break;
						}
						else {
							vectSensors[tSen]->stastusSensor = 0; // ���
						}
					}
				}
			}
		}
		// // �������� ����������
		maxSen = zProgramSettings->firstSensorsCross + zProgramSettings->countCrossSensors;
		for (int cSen = zProgramSettings->firstSensorsThick; cSen < maxSen; cSen++) {
			if (vectSensors[cSen]->stastusSensor == 1) {
				// ������� ��������� �����, ���� ������ �������� �����
			}
			//
			else {
				for (int i = 0; i < vectSensors[cSen]->realSensorSize; i++) {
					double valueSens0 = vectSensors[cSen]->vecSensorData[i];
					valueSens = (vectSensors[cSen]->vecSensorData[i] * zProgramSettings->gainsXY[cSen]) / zdADC_Range_Value[cSen] * 100;
					if (abs(valueSens) > _crossBorder10) {
						vectSensors[cSen]->stastusSensor = 1; // ��1
						break;
					}
					else {
						if (vectSensors[cSen]->stastusSensor == 2) {
							// ��� ������
						}
						else {
							if (abs(valueSens) > _crossBorder20) {
								vectSensors[cSen]->stastusSensor = 2; // ��2
                                break;
							}
							else {
								vectSensors[cSen]->stastusSensor = 0; // ���
							}
						}
					}
				}
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

// �� ��������
// ---------------------------------------------------------------------------
// ������� ������ �������/������� vector<double>vecDataSensor;
TSensorData::TSensorData(int _sensorNum, int _fullSensorSize, int _countLogCh) {
	int err;
	try {
		sensorNum = _sensorNum;
		countLogCh = _countLogCh;
		// fullSensorSize = (int)_fullZoneSize / countLogCh;
		fullSensorSize = _fullSensorSize;
		realSensorSize = 0;
		vecSensorData.reserve(fullSensorSize);
		stastusSensor = -1;
		// int k = vecDataSensor.size();
		for (int i = 0; i < fullSensorSize; i++) {
			// vecDataSensor[i]=0;
			vecSensorData.push_back(0);
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

TSensorData::~TSensorData(void) {
	// ������ ������ � �������
	vecSensorData.clear();
	// delete dataSensor;
}

// ---------------------------------------------------------------------------

int TSensorData::SensorDataAdd(double* _dataZoneBlock, int _blockZoneSize, int _countNeedAdd, AnsiString &_msg) {
	int err = -1;
	// k = vecDataSensor.size();
	try {
		if ((realSensorSize + _countNeedAdd) > fullSensorSize) {
			_msg = "���������� ������� ������� " + IntToStr(sensorNum);
			TLog::SaveStrLog(_msg);
		}
		else {
			//
		}
		// �� ������������ ������� �������� ������� ���� ��������
		for (int n = realSensorSize; n < (realSensorSize + _countNeedAdd); n++) {
			vecSensorData[n] = _dataZoneBlock[sensorNum + n * countLogCh];
			// vecDataSensor.insert(vecDataSensor.end(), _dataZone[sensorNum + i * countLogCh]);
			// vecDataSensor.push_back(_dataZone[sensorNum + i * countLogCh]);
			// vecDataSensor.size();
			// ptr_vect[i]
			// k = vecDataSensor.size();
		}
		err = vecSensorData.size();
		_msg = "��� ������ ��������� � ������� " + IntToStr(sensorNum) + " - " + IntToStr(_countNeedAdd);
	}
	catch (Exception *ex) {
		_msg = ex->Message;
		TLog::ErrFullSaveLog(ex);
	};
	return err;
}

int TSensorData::ResetSensor(void) {
	int err = -1;
	try {
		realSensorSize = 0;
		stastusSensor = -1;
		// int k = vecDataSensor.size();
		for (int i = 0; i < fullSensorSize; i++) {
			// vecDataSensor[i]=0;
			vecSensorData[i] = 0;
			int tmp = vecSensorData[i];
			tmp++;
		}
		err = 0;
	}
	catch (Exception *ex) {
		err = -2;
		TLog::ErrFullSaveLog(ex);
		// TExtFunction::UpdateStatusBar(StatusBarBottom,strStatus, ex->Message, programSettings.colorBrak);
		// MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);

	}
	return err;
}

int TSensorData::Empty(void) {
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
