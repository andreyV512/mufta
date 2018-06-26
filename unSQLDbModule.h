//---------------------------------------------------------------------------

#ifndef unSQLDbModuleH
#define unSQLDbModuleH
//---------------------------------------------------------------------------
//#include <System.Classes.hpp>
//#include <Classes.hpp>
//#include <Data.DB.hpp>
#include <DB.hpp>
//#include <Data.Win.ADODB.hpp>
#include <ADODB.hpp>
#include <StdCtrls.hpp>
#include <Classes.hpp>
//#include <ExtCtrls.hpp>
#include <vector>
//---------------------------------------------------------------------------
class TSqlDBModule : public TDataModule
{
__published:	// IDE-managed Components
	TADOConnection *ADOConnectionDB;
	TADOCommand *ADOCommandDB;
	TADOQuery *ADOQueryDB;
	TADOTable *ADOTableDB;
	TADODataSet *ADODataSet;
	TADOQuery *queryVirt;
    void __fastcall ADOQueryDBAfterClose(TDataSet *DataSet);
	void __fastcall ADOConnectionDBBeforeConnect(TObject *Sender);
private:	// User declarations
	AnsiString applPath;
	AnsiString pathUDL;

public:		// User declarations
	__fastcall TSqlDBModule(TComponent* Owner);
	int GetBackupDB(AnsiString _bdName, AnsiString& _msg);
		//�������� �������� ������ ���� ����������� ������������ _where!!!
	int GetIntFieldSQL(AnsiString _tableName, AnsiString _fieldName, AnsiString _where, int _default,int &err);
	bool GetBoolFieldSQL(AnsiString _tableName, AnsiString _fieldName, AnsiString _where, bool _default,int &err);
	AnsiString GetStrFieldSQL(AnsiString _tableName, AnsiString _fieldName, AnsiString _where, AnsiString _default,int &err);
	double GetFloatFieldSQL(AnsiString _tableName,AnsiString _fieldName, AnsiString _where, double _default,int &err);
	//�������� ������ ��� �������� ����
	double GetFloatExtFieldSQL(TADOQuery *_query, AnsiString _tableName,AnsiString _fieldName, AnsiString _where, double _default, int &err);
	int GetBackupDB(AnsiString _msg);
	//��������� ������
	int ExecStrSql(AnsiString _strSql);
	//��������� ������
	int SelectStrSql(AnsiString _strSql,int &_err);
	//��������� ������ � ComboBox, ������ � ������� ������ ���� ��������� ���� � ������ REC_ID!!!
	int FillComboBox(AnsiString _tableName, AnsiString _fieldName,TComboBox* _comboBox);
	//��������� ������ � ComboBox �� ������� ���� � ������� ������ ���� ����������, ��� as F1!!!  � � ������� ����������� ��������� ���� ���������� ����� rec_id
	int FillComboBoxFromSql(AnsiString _strSQL, TComboBox* _comboBox);
	//�������� �������� ���� ���� int � ������ F1
	int GetIntFromSql(AnsiString _strSql);
	// �������� �������� ����
	int UpdIntSql(AnsiString _tableName, AnsiString _fieldName, int _fieldValue, AnsiString _where);
	int UpdStrSql(AnsiString _tableName, AnsiString _fieldName, AnsiString _fieldValue, AnsiString _where);
	int UpdFloatSql(AnsiString _tableName, AnsiString _fieldName, double _fieldValue, AnsiString _where);

};
//---------------------------------------------------------------------------
extern PACKAGE TSqlDBModule *SqlDBModule;
//---------------------------------------------------------------------------
#endif
