// ---------------------------------------------------------------------------
#pragma hdrstop
#include "TSG.h"
// #include "SignalListDef.h"
#include <Math.hpp>
#include <algorithm>
// #include "uFunctions.h"
#include "TProtocol.h"
#include "Filters.h"
#include "TLCardData.h"
#include "TLogCh791Params.h"
#include "TSGCoords.h"
#include "unTExtFunction.h"
#include "unSQLDbModule.h"
#include "EtalonDatas.h"
#include "TypeSizeFrequencies.h"
// ---------------------------------------------------------------------------
#ifdef BERLIN_10
//
#else
  #pragma package(smart_init)
#endif


// ---------------------------------------------------------------------------
// SG::SG(TIniFile* _ini, AnsiString _SGroupFix)
TSG::TSG(TGlobalSettings* _mainGlobalSettings,TLCardData* _sgLCardData) {
	// SGroupFix = _SGroupFix;
	sgGlobalSettings=_mainGlobalSettings;
	sgLCardData=_sgLCardData;
	state = 0;
	freqNum = 0;
}

// ---------------------------------------------------------------------------
TSG::~TSG(void) {
	// lcard->Stop();
	sgLCardData->StopSGM();
}

// ---------------------------------------------------------------------------
void TSG::pr(AnsiString _msg) {
	AnsiString a = "SG: ";
	a += _msg;
	TProtocol::ProtocolSave(a);
}

// ---------------------------------------------------------------------------
void TSG::pr0(AnsiString _msg) {
	TProtocol::ProtocolSave(_msg);
}

// ---------------------------------------------------------------------------
int TSG::Exec(int _freqNum) {
	freqNum = _freqNum;
	switch (state) {
	case 0:
	{
		startTime = GetTickCount();
		sgLCardData->StartSGM(freqNum);
		state = 1;
		break;
	}
	case 1:
	{
		if (!sgLCardData->Read(freqNum))
			TExtFunction::FATAL("SG::Exec: �� ������ ��������� ����� lcard");
//		int x = sgLCardData->GetBufTime()*1000;
//		int y = GetTickCount()-startTime;
		if((GetTickCount()-startTime) < sgLCardData->GetBufTime()*1000)
			break;
		sgLCardData->StopSGM();
		state = 2;
		break;
	}
	default:
		break;
	}
	return (state);
}
// ---------------------------------------------------------------------------
CSG TSG::GetSG()
{
	//������ ������ ���������
	//�������� ������ ������ �������� ����������� � �������� ������� �� ��������
	TSFrequencies TSFreqs = TSFrequencies(sgGlobalSettings->indexCurrentTypeSize);
	EtalonDatas EDatas = EtalonDatas(sgGlobalSettings->indexCurrentTypeSize
		,sgGlobalSettings->indexCurrentSGGost);
	TSThresholds TSThres = TSThresholds(sgGlobalSettings->indexCurrentTypeSize);
	double maxCorr = 0;
	vector<int> maxCorrSG_id;
	vector<double> BarkValues;
	//��� ������ ��������� �������
	for(int i = 0; i < sgLCardData->vecMeasuresData.size(); i++)
	{
		sgLCardData->setFreqNum(i);
		BarkValues = sgLCardData->GetBarkValues(TSThres.Thresholds);
		int currSG_id = 0;
		//��� ������� ������� � ����� �� �������� � ������������
		for(int j = 0; j < EDatas.Etalons.size(); j++)
		{
			if(TSFreqs.Frequency[i] == EDatas.Etalons[j].Frequency)
			{
				//������ �� �����������
				double currCorr = sgLCardData->GetCorrelation(BarkValues,EDatas.Etalons[j].BarkValues);
				//��������� ���������� ����������� � ��������� �� ������ ��� �� ��� ���� �������
				if(currCorr > maxCorr)
				{
					maxCorr = currCorr;
					currSG_id = EDatas.Etalons[j].SG_id;
				}
			}
		}
		maxCorrSG_id.push_back(currSG_id);
	}
	//��������� �������� ������ �� �� ���� ��������
	int maximal = 0;
	int element = 0;
	for (int i = 0; i < maxCorrSG_id.size();i++)
	{
		int currNum = maxCorrSG_id[i];
		int currCount = std::count(maxCorrSG_id.begin(), maxCorrSG_id.end(), currNum);
		if (currCount > maximal)
		{
			maximal = currCount;
			element = currNum;
		}
		if (currCount == maximal) //���� ���� 2 ���������, �� ������ �� ����������
		{
			element = 0;
		}
	}

	CSG ret;
	ret.group_id = element;
	return (ret);
}

// ---------------------------------------------------------------------------
TColor TSG::conv_color(int _col) {
	unsigned int ret;
	ret = _col >> 16 & 0xFF;
	ret |= _col & 0xFF00;
	ret |= _col << 16 & 0xFF0000;
	return ((TColor)ret);
}

// ---------------------------------------------------------------------------
CSG TSG::Get1FrecSG()
{
	//������ ������ ���������
	//�������� ������ ������ �������� ����������� � �������� ������� �� ��������
	TSFrequencies TSFreqs = TSFrequencies(sgGlobalSettings->indexCurrentTypeSize);
	EtalonDatas EDatas = EtalonDatas(sgGlobalSettings->indexCurrentTypeSize
		,sgGlobalSettings->indexCurrentSGGost);
	TSThresholds TSThres = TSThresholds(sgGlobalSettings->indexCurrentTypeSize);
	double probability = 0;
//	vector<int> maxCorrSG_id;
	vector<double> BarkValues;
	//��� ������ ��������� �������
	sgLCardData->setFreqNum(0);
	BarkValues = sgLCardData->GetBarkValues(TSThres.Thresholds);
	int currSG_id = 0;
	switch(sgGlobalSettings->calcSGMode)
	{
		case 1:
		{
			//�� ����������
			double minDev = 0;
			double maxDev = 1;
			//��� ������� ������� � ����� �� �������� � ������������
			for(int j = 0; j < EDatas.Etalons.size(); j++)
			{
				if(TSFreqs.Frequency[0] == EDatas.Etalons[j].Frequency)
				{
					//������ �� ����������
					double currDev = sgLCardData->GetSquareDeviation(BarkValues,EDatas.Etalons[j].BarkValues);
					//��������� ���������� � ���������� ���������� � ��������� ��� ������ ��� �� ��� ���� �������
					if((currDev < minDev) || (minDev == 0))
					{
						minDev = currDev;
						currSG_id = EDatas.Etalons[j].SG_id;
					}
					if((currDev > maxDev) || (maxDev == 1))
					{
						maxDev = currDev;
//						currSG_id = EDatas.Etalons[j].SG_id;
					}
				}
			}
			probability = (maxDev-minDev)/maxDev;
			break;
		}
//		case 2:
//		{
//			//�� �����������
//			vector<double> probValues;
//			double maxProb = 0;
//			//��� ������� ������� � ����� �� �������� � ������������
//			for(int j = 0; j < EDatas.Etalons.size(); j++)
//			{
//				if(TSFreqs.Frequency[0] == EDatas.Etalons[j].Frequency)
//				{
//					//������ �� ����������
//					double currDev = sgLCardData->GetSquareDeviation(BarkValues,EDatas.Etalons[j].BarkValues);
//					probValues.push_back(currDev);
//				}
//				else probValues.push_back(1.0);
//			}
//			//��������� ��������
//			double devider = 1;
//			for(int i = 0; i < probValues.size(); i++)
//				devider *= (1.0/probValues[i]);
//			//��� ������� ������� � ����� �� �������� � ������������
//			for(int j = 0; j < EDatas.Etalons.size(); j++)
//			{
//				if(TSFreqs.Frequency[0] == EDatas.Etalons[j].Frequency)
//				{
//					//������ �� ����������
//					double currProb = (1.0/probValues[j])/devider;
//					if(currProb > maxProb)
//					{
//						maxProb = currProb;
//						currSG_id = EDatas.Etalons[j].SG_id;
//					}
//				}
//			}
//			probability = maxProb;
//			break;
//		}
		default:
		{
			//�� ����������
			double maxCorr = 0;
			//��� ������� ������� � ����� �� �������� � ������������
			for(int j = 0; j < EDatas.Etalons.size(); j++)
			{
				if(TSFreqs.Frequency[0] == EDatas.Etalons[j].Frequency)
				{
					//������ �� �����������
					double currCorr = sgLCardData->GetCorrelation(BarkValues,EDatas.Etalons[j].BarkValues);
					//��������� ���������� ����������� � ��������� �� ������ ��� �� ��� ���� �������
					if(currCorr > maxCorr)
					{
						maxCorr = currCorr;
						currSG_id = EDatas.Etalons[j].SG_id;
					}
				}
			}
			probability = maxCorr;
		}
	}
	//���������
    int err;
	CSG ret;
	ret.group_id = currSG_id;
	ret.probability = probability;
	ret.group = SqlDBModule->GetStrFieldSQL("SolidGroups","SGName"
		,"rec_id="+IntToStr(ret.group_id),"X",err);
	ret.color = (TColor)SqlDBModule->GetIntFieldSQL("SolidGroups","Color"
		,"rec_id="+IntToStr(ret.group_id),65535,err);
	return (ret);
}

