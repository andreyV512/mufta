// ---------------------------------------------------------------------------

#pragma hdrstop

#include "TMeasuresData.h"

TMeasuresData::TMeasuresData(int _chLogCount) {
//	realSensorSize = 0;
//	fullSensorSize = _fullSensorSize;
	// ������� ������ ��������
	vecSensorsData.reserve(_chLogCount);
	namesChannels.reserve(_chLogCount);
	// ������� �������
	for (int s = 0; s < _chLogCount; s++) {
		AnsiString kn="�����: "+IntToStr(s);
		namesChannels.push_back(kn);
		vector<double> vecTMP;
		vecSensorsData.push_back(vecTMP);
		//vecTMP.clear();
	}
}

TMeasuresData::~TMeasuresData(void) {
	// ������ �������
	int sz = vecSensorsData.size();
	for (int s = 0; s < sz; s++) {
//		for (int i = 0; i < fullSensorSize; i++) {
//			//
//		}
		vecSensorsData[s].clear();
	}
	vecSensorsData.clear();
}

int TMeasuresData::ClearData(void) {
//	realSensorSize = 0;
	int sz = vecSensorsData.size();
	for (int s = 0; s < sz; s++)
	{
		//�����: �� ���� ��� ������ >_<
//		for (int i = 0; i < fullSensorSize; i++) {
//			// �������� ������ 0
//			vecSensorsData[s][i] = 0;
//		}
        vecSensorsData[s].clear();
	}
}
