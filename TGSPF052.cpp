// ---------------------------------------------------------------------------

#pragma hdrstop

#pragma package(smart_init)

// ���� RshApi.cpp ���������� ���������� #include, ����� ������������ � ��� ����������� ������� � �������
// ���� ��������������, ����� ��������� �������� ��� ��������.
//
// � ������� �������� ����� ���� ������� ������� ����-������� rshsdkwrapper.cpp, ������ �������� ���������
// #include <RshApi.cpp>
// � ������� ��� ������ �������.
//
#include "TGSPF052.h"
#include "TProtocol.h"
#include "unTExtFunction.h"
// #include <RshApi.cpp>

// GSPF052::GSPF052(TIniFile* _ini)
TGSPF052::TGSPF052(TGlobalSettings* _globalSettings, int &_err)
{
	try
	{
		gspfGlobalSettings = _globalSettings;
		_err = -99;
		isStartedGSPF052 = false;
		rshClient = new RshDllClient();
		// // ��������� ��� ����������, � ������� ����� �������� ���������.
		BoardName = "GSPF052PCI";
		// //����� ����������. ���� � ������� ����������� ��������� ���������� ���������, ����� �������, � ������ �� ��� ������������. ��������� ���������� � 1.
		DeviceID = 1;
		// // ������� �������������
		SampleFreq = 10000000.0;
		// // ������� ������������� ������ � ������
		SignalFreq = 2000.0;
		// // ��������� ������������� ������ � �������
		SignalAmplitude = 2.0;
		// // ���� ������ ������. (� ������ ������� � ���������� ��������� ���������� ����)
		ErrorLanguage = RSH_LANGUAGE_RUSSIAN;
		// //const RSH_LANGUAGE ErrorLanguage = RSH_LANGUAGE_ENGLISH;
		// //1��� ������������� ��������. L ������ ��������� ������������ ��� ������� ��������� ���������� ����������.
		OutR = 1000000.0f;

		// ini = _ini;
		// ������ ��� ������ � ��������
		AnsiString Str;
		// ������ ���������� ��������. RSH_API_SUCCESS � ������ ������, ���� ��� ������
		U32 st = RSH_API_SUCCESS;
		// ��������� �� ������ � ����������� IRshDevice, ������� ������������ ��� ������� � ����������.
		rshDevice = 0;
		// //����� ��� �������� ���������� ����������.
		// RshDllClient Client;

		// //===================== ��������� ������ ������������������ ��������� =====================
		// //� ������ ������� ���������� ����� ������ �� ������������,
		// //��� ��� �� ������� ����� ��� ���������� (��������� BoardName)
		// toProtocol(
		// "��� ��� �� ������� ����� ��� ���������� (��������� BoardName):");
		// + BoardName);
		// std::vector<std::string> *allDevices;
		// st = Client.GetRegisteredList(allDevices);
		// if(st == RSH_API_SUCCESS)
		// {
		// pr("List of registered device libraries:");
		// for(std::vector<std::string>::iterator *it = allDevices.begin();
		// it != allDevices.end(); ++it)
		// {
		// toProtocol((AnsiString)*it);
		// }
		// }
		// toProtocol("----------");
		// ===================== �������� ������ ��� ������ � ����������� �� ���������� =====================
		// �������������� ������������ ����.
		RshDllInterfaceKey DevKey(BoardName, (void**)&rshDevice);
		// �������� �������� ������ ����������.
		toProtocol("Loading device interface from dynamic library... ");
		st = rshClient->GetDeviceInterface(DevKey);
		if (st != RSH_API_SUCCESS)
		{
			// Close(Client, st, "Client.GetDeviceInterface(DevKey)");
			_err = -1;
			// toProtocol("ERROR Client.GetDeviceInterface(DevKey)");
			CloseCard("Client.GetDeviceInterface(DevKey)");
		}
		else
			toProtocol("Success!");

		// ����� ��������� ���������� ������ Client.GetDeviceInterface()
		// ��������� device ��������� �� ���������������� ������,
		// ������� ����� ������������ ��� ���������� �����������

		// ===================== ���������� �� ������������ ����������� =====================
		RSH_S8P libname = 0;
		st = rshDevice->Get(RSH_GET_LIBRARY_FILENAME, &libname);
		if (st != RSH_API_SUCCESS)
		{
			// Close(Client, st, "device->Get(RSH_GET_LIBRARY_FILENAME)");
			_err = -2;
			// toProtocol(" ERROR device->Get(RSH_GET_LIBRARY_FILENAME)");
			CloseCard("device->Get(RSH_GET_LIBRARY_FILENAME)");
		}
		else
			toProtocol(AnsiString("Library Name: ") + (char*)(libname.data));

		RSH_S8P libVersion = 0;
		st = rshDevice->Get(RSH_GET_LIBRARY_VERSION_STR, &libVersion);
		if (st != RSH_API_SUCCESS)
		{
			// Close(Client, st, "device->Get(RSH_GET_LIBRARY_VERSION_STR)");
			_err - 3;
			CloseCard("device->Get(RSH_GET_LIBRARY_VERSION_STR)");
		}
		else
			toProtocol(AnsiString("Library Version: ") +
				(char*)(libVersion.data));

		RSH_S8P libCoreName = 0;
		st = rshDevice->Get(RSH_GET_CORELIB_FILENAME, &libCoreName);
		if (st != RSH_API_SUCCESS)
		{
			// Close(Client, st, "device->Get(RSH_GET_CORELIB_FILENAME)");
			_err = -4;
			CloseCard("device->Get(RSH_GET_CORELIB_FILENAME)");
			throw(Exception("�� ������ ������� ����!!! RSH_GET_CORELIB_FILENAME err=" +IntToStr(_err)));
		}
		else
			toProtocol(AnsiString("Core Library Name: ") +
				(char*)(libCoreName.data));

		RSH_S8P libCoreVersion = 0;
		st = rshDevice->Get(RSH_GET_CORELIB_VERSION_STR, &libCoreVersion);
		if (st != RSH_API_SUCCESS)
		{
			// Close(Client, st, "device->Get(RSH_GET_CORELIB_VERSION_STR)");
			_err - 5; CloseCard("device->Get(RSH_GET_CORELIB_VERSION_STR)");
			throw(Exception
				("�� ������ ������� ����!!! RSH_GET_CORELIB_VERSION_STR err=" +
					IntToStr(_err)));
		}
		else
			toProtocol(AnsiString("Core Library Version: ") +
				(char*)(libCoreVersion.data));

		// ============================= �������� ������������� =============================
		RSH_U32 caps = RSH_CAPS_SOFT_GENERATION_IS_AVAILABLE;
		// ��������, ������������ �� ����� ��������� ����������� �������.
		st = rshDevice->Get(RSH_GET_DEVICE_IS_CAPABLE, &caps);
		if (st != RSH_API_SUCCESS)
		{
			// Close(Client, st, "device->Get(RSH_GET_DEVICE_IS_CAPABLE)");
			_err = -6;
			CloseCard("device->Get(RSH_GET_DEVICE_IS_CAPABLE)");
			throw(Exception
				("�� ������ ������� ����!!! Get(RSH_GET_DEVICE_IS_CAPABLE err="
					+ IntToStr(_err)));
		}
		// ========================= ����������� � ���������� ====================================
		// ������� ����, �� �������� ����� ����������� ����������� (� ������ ������
		// ������������ ������� �����)
		RshDeviceKey connectKey(DeviceID);

		// ������������ � ����������
		toProtocol("Connecting to device (it may take some time)... ");
		st = rshDevice->Connect(&connectKey);
		if (st != RSH_API_SUCCESS)
		{
			_err = -61;
			CloseCard("device->Connect()");
			throw(Exception("�� ������ ������� ����!!! Connect err=" +
				IntToStr(_err)));
			// Close(Client, st,"device->Connect()");
		}
		else
			toProtocol("Success!");
		// ������ (������) ��� ���������� � ��������� ����� �������� ������ ����� ����������� � ����������
		RSH_S8P fullDeviceName = 0;
		st = rshDevice->Get(RSH_GET_DEVICE_NAME_VERBOSE, &fullDeviceName);
		if (st != RSH_API_SUCCESS)
		{
			// Close(Client, st, "device->Get(RSH_GET_DEVICE_NAME_VERBOSE)");
			_err = -7;
			CloseCard("device->Get(RSH_GET_DEVICE_NAME_VERBOSE)");
			throw(Exception
				("�� ������ ������� ����!!! RSH_GET_DEVICE_NAME_VERBOSE err=" +
					IntToStr(_err)));
		}
		else {toProtocol((AnsiString)"The name of the connected device: " +
				(char *)fullDeviceName.data);
			// toProtocol(Str);
		}
		RSH_U32 serialNumber = 0;
		st = rshDevice->Get(RSH_GET_DEVICE_SERIAL_NUMBER, &serialNumber);
		if (st != RSH_API_SUCCESS)
		{
			// Close(Client, st, "device->Get(RSH_GET_DEVICE_SERIAL_NUMBER)");
			_err = -8;
			CloseCard("device->Get(RSH_GET_DEVICE_SERIAL_NUMBER)");
			throw(Exception
				("�� ������ ������� ����!!! RSH_GET_DEVICE_SERIAL_NUMBER err=" +
					IntToStr(_err)));}
		else
			toProtocol("Serial number of the connected device:  " +
				(AnsiString)(serialNumber.data));
		// ========================= �������������� ����������� ���������� ====================================

		toProtocol("----------");
		toProtocol("Additional device features: ");

		// ����������� ����
		RSH_U32 DACResolution = 0;
		st = rshDevice->Get(RSH_GET_DEVICE_DATA_BITS, &DACResolution);
		if (st != RSH_API_SUCCESS)
		{
			// Close(Client, st, "device->Get(RSH_GET_DEVICE_DATA_BITS)");
			_err = -9;
			CloseCard("device->Get(RSH_GET_DEVICE_DATA_BITS)");
			throw(Exception
				("�� ������ ������� ����!!! RSH_GET_DEVICE_DATA_BITS err=" +
					IntToStr(_err)));
		}
		toProtocol((AnsiString)"DAC resolution: " + (DACResolution.data) + "bits");

		// ����������� ������� �������������
		RSH_DOUBLE DACMinFreq = 0.0;
		rshDevice->Get(RSH_GET_DEVICE_MIN_FREQUENCY, &DACMinFreq);
		if (st != RSH_API_SUCCESS)
		{
			// Close(Client, st, "device->Get(RSH_GET_DEVICE_MIN_FREQUENCY)");
			_err = -10;
			CloseCard("device->Get(RSH_GET_DEVICE_MIN_FREQUENCY)");
			throw(Exception
				("�� ������ ������� ����!!! RSH_GET_DEVICE_MIN_FREQUENCY err=" +
					IntToStr(_err)));
		}
		toProtocol((AnsiString)"Minimum frequency: "+FloatToStr(DACMinFreq.data)+"Hz");

		// ������������ ������� �������������
		RSH_DOUBLE DACMaxFreq = 0.0;
		rshDevice->Get(RSH_GET_DEVICE_MAX_FREQUENCY, &DACMaxFreq);
		if (st != RSH_API_SUCCESS)
		{
			// Close(Client, st, "device->Get(RSH_GET_DEVICE_MAX_FREQUENCY)");
			_err = -11;
			CloseCard("device->Get(RSH_GET_DEVICE_MAX_FREQUENCY)");
			throw(Exception
				("�� ������ ������� ����!!! RSH_GET_DEVICE_MAX_FREQUENCY err=" +
					IntToStr(_err)));
		}
		toProtocol((AnsiString)"Maximum frequency: "+FloatToStr(DACMaxFreq.data)+"Hz");

		// ������������ ��������� � �������
		RSH_DOUBLE DACRealMaxAmp;
		rshDevice->Get(RSH_GET_DEVICE_OUTPUT_RANGE_VOLTS, &DACRealMaxAmp);
		toProtocol((AnsiString)"Maximum out amplitude: " +
			FloatToStr(DACRealMaxAmp.data) + "V");

		// ������ �������� ������������ ��������� � ����������� �� ������������� ��������
		DACMaxAmp = 0.0;
		DACMaxAmp = OutR * (DACRealMaxAmp.data / (OutR + 50.0));
		toProtocol((AnsiString) "Maximum out amplitude for current output resist: " +
			FloatToStr(DACMaxAmp) + "V");

		// ========================= ������������� ====================================

		// //��������� �������������
		// ������ ���������� - �����������
		gspfPar.startType = RshInitGSPF::Program;

		// ���������� �� ������������
		gspfPar.attenuator = RshInitGSPF::AttenuationOff;

		// �������� ����������� ������������ �������.
		// ������������ - ������ ���������� ������� ������������ �����:
		// gspfPar.control |= RshInitGSPF::PlayLoop;
		gspfPar.control |= RshInitGSPF::PlayLoop; // SetPlayModeLoop();
		// �������� ����������, ��� ������
		gspfPar.control |= RshInitGSPF::SynthesizerOff;
		// ������� �������������
		gspfPar.frequency = SampleFreq;
		// ������������� ����������
		st = rshDevice->Init(&gspfPar);
		if (st != RSH_API_SUCCESS)
		{
			_err = -99;
			CloseCard("device->Init(&gspfPar)");
			throw(Exception("�� ������ ������� ����!!! device->Init err=" +
				IntToStr(_err))); return;}
		else {
			//
		}

		// todo ��� ��������� ��������� � ������?
		// � ������?
		// AnsiString StrA = "Initialization parameters: " + gspfPar.;
		// toProtocol(StrA);
		_err = 0;
	}
	catch (Exception *ex)
	{
		TLog::ErrFullSaveLog(ex);
		MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);
	}
}

// ---------------------------------------------------------------------------
TGSPF052::~TGSPF052(void) {
	//
}

int TGSPF052::FormSignal(int _valueFrec, double _valueAmpl)
{
	int err = -101;
	AnsiString msg = "";
	// ========================= ������������ ������� (�����) ====================================
	// ������ ���������� ��������. RSH_API_SUCCESS - 0 � ������ ������, ���� ��� ������
	U32 status = RSH_API_SUCCESS;

	// ������ ������ ��� ������� � ��������
	// ���� ������ ������� ������� �������
	const U32 bSize = 100000;

	// �������������� ����� ��� ������
	RSH_BUFFER_S16 buffer(bSize);

	// ������ ����� ��������� ������� �������� �����
	SignalFreq = _valueFrec;
	// ini->ReadFloat("SignalFrequencies", str, 1000.0);
	SignalAmplitude = _valueAmpl;

	// ��������, �� ���������� �� �������� ��� ��������� ������ 0
	if (RSH_CMPDOUBLE(DACMaxAmp, 0.0))
	{
		// Close(Client, RSH_API_PARAMETER_INVALID, "RSH_CMPDOUBLE(DACMaxAmp, 0.0)");
		err = -1;
		CloseCard("RSH_CMPDOUBLE(DACMaxAmp, 0.0)");
		return err;
	}

	// ����������� ��� ��������� �� ����� � ���
	double amplitudeCoef = SignalAmplitude / DACMaxAmp;

	// ��������� ����� �������
	int old_size = buffer.PSize();
	// buffer.reserve(sz);
	for (U32 i = 0; i < buffer.PSize(); ++i)
	{
		S16 value = static_cast<S16>(amplitudeCoef * 32768 * sin
			(i * (2 * 3.1415926 * SignalFreq) / gspfPar.frequency));
		buffer.PushBack(value);
	}
	// ���� ����� ������������ ������ ����� ������, ����� ������� ������
	// (�� ������� ������ ������)
	// todo �������� �� ��������� �����     //�������� ����, ��� �� ������ ���������
	U32 new_size = (int) gspfPar.frequency / SignalFreq;
	if (new_size % 2 != 0) // ����� ������ ������ ������ ���� ������!
		new_size++;
	if (new_size < bSize)
		buffer.SetSize(new_size);
	else {msg =
			"�� ������� ������� ������ ��� ���������� ������� ������ - ���������� ������� �������";
		toProtocol(msg);
		TExtFunction::ShowBigModalMessage(msg, clRed);
	}
	// new_size ������ ���� ������ ��� ����� buffer.PSize()
	// ���������� ����� � ����������
	status = rshDevice->GetData(&buffer);
	if (status != RSH_API_SUCCESS)
	{
		err = -2; msg = "������ ���������� ����� � ����������!!! \n";
		TExtFunction::ShowBigModalMessage(msg, clRed);
		// Close(Client, st, "device->GetData(&buffer)");
		CloseCard("device->GetData(&buffer)");
		return err;
	}
	err = 0;
	return err;
}

// ---------------------------------------------------------------------------
int TGSPF052::StartGSPF052()
{
	int err = -99;
	AnsiString msg;
	// ������ ���������� ��������. RSH_API_SUCCESS � ������ ������, ���� ��� ������
	U32 status = RSH_API_SUCCESS;
	// ��������� ������������ ������
	status = rshDevice->Start();
	if (status != RSH_API_SUCCESS)
	{
		// Close(Client, st, "device->Start()");
		err = -1; msg = "device->Start()";
		toProtocol(msg);
		TExtFunction::ShowBigModalMessage(msg, clRed);
		CloseCard(msg.c_str());
	}
	else
	{
		isStartedGSPF052 = true;
		err = 0;
	}
	return err;
}

// ---------------------------------------------------------------------------
int TGSPF052::WaitInterrupt()
{
	int err = -99;
	// ������ ���������� ��������. RSH_API_SUCCESS � ������ ������, ���� ��� ������
	U32 status = RSH_API_SUCCESS;
	toProtocol("Waiting for interrupt after buffer generation completion...");

	// �������� ���������� (���������� ������ ��� ��� ���������� ������������ ������)
	status = rshDevice->Get(RSH_GET_WAIT_BUFFER_READY_EVENT);
	if (status != RSH_API_SUCCESS)
	{
		// Close(Client, st, "device->Get(RSH_GET_WAIT_BUFFER_READY_EVENT)");
		CloseCard("device->Get(RSH_GET_WAIT_BUFFER_READY_EVENT)");
		err = -1;
	}
	else
	{
		err = 0; toProtocol("Interrupt has taken place!");
		// Close(Client, RSH_API_SUCCESS);
	}
	return err;
}

// ---------------------------------------------------------------------------
int TGSPF052::StopGSPF052()
{
	int err = -99;
	// ������ ���������� ��������. RSH_API_SUCCESS � ������ ������, ���� ��� ������
	U32 status = RSH_API_SUCCESS;
	if (isStartedGSPF052 && rshDevice)
	{
		// ������������� ������������ ������
		isStartedGSPF052 = false;
		status = rshDevice->Stop();
		if (status != RSH_API_SUCCESS)
		{
			// Close(Client, st, "device->Stop()");
			err = -1;
			CloseCard("device->Stop()");
		}
		else
		{
			err = 0;
		}
	}
	else
	{
		err = 1;
	}
	return err;
}

// ---------------------------------------------------------------------------
// ������� ������� ��������� �� ������ � ��������� ������ �����
int TGSPF052::CloseCard(AnsiString desc)
{
	int err = -99;
	AnsiString msg;
	toProtocol("----------");
	if (desc != "ok")
	{
		msg = "Error on " + desc + " call.";
		toProtocol(msg);
		TExtFunction::ShowBigModalMessage(msg, clRed);
	}
	else
		toProtocol("GSPF normal close");
	// RshError::PrintError(st, ErrorLanguage, true); //todo ��������� ���� �������
	// ����� Free() ��������� ��� ����������� ������� RshDllClient ����������.
	try
	{
		rshClient->Free();
		rshClient = NULL;
		err = 0;
		// pr("Press 'Enter' key to end up the program.");
		// cin.ignore();
	}
	catch (Exception *ex)
	{
		// TLog::ErrFullSaveLog(ex);
		err = -3;
		MessageDlg(ex->Message, mtError, TMsgDlgButtons() << mbOK, NULL);
	}
	return err;
	// return (unsigned int) st;
}

// ---------------------------------------------------------------------------
// ����� ��������� � ��������
void TGSPF052::toProtocol(AnsiString _msg)
{
	AnsiString a; a = "GSPF052: ";
	a += _msg;
	TProtocol::ProtocolSave(a);
}

// ---------------------------------------------------------------------------
// ���������� ������� �������������
void TGSPF052::SetSampleFreq(double _SampleFreq)
{
	// ������� ������� �������������
	gspfPar.frequency = _SampleFreq;
	// ������������� ����������
	U32 st = RSH_API_SUCCESS;
	st = rshDevice->Init(&gspfPar);
	if (st != RSH_API_SUCCESS)
	{
		CloseCard("device->Init(&gspfPar)");
		return;
	}
}
// ---------------------------------------------------------------------------

