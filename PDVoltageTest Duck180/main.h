//---------------------------------------------------------------------------

#ifndef mainH
#define mainH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
#include <dbt.h> //DBT_常數 註冊要取得的裝置消息
#include "HID.h"
#include "IO_Request.h"
#include "DeviceNotify.h"
#include <Buttons.hpp>
#include <Registry.hpp>
#include <ExtCtrls.hpp>
#include <Mask.hpp>
#include <Menus.hpp>
#include <Graphics.hpp>

#include "COM_control.h"
#include <queue>
#include <fstream.h>
#include <string>

const AnsiString APP_TITLE = "PD Voltage Test Tool ver.Dock180 ( Action Star )";
//
const AnsiString SetVoltage[5] = //5V , 9V , 12V ,15V ,20V
{
 "03 14 04 70 33 01 01" ,
 "03 14 04 70 33 01 02" ,
 "03 14 04 70 33 01 03" ,
 "03 14 04 70 33 01 04" ,
 "03 14 04 70 33 01 05"
 };
const AnsiString ExecuteVoltageSetting = "03 14 07 70 08 04 41 4E 65 67";
const AnsiString ReadADValue = "03 13 E8 FF 90";
const AnsiString PASSWORD  = "SET";

const float UNIT = 0.0195; //單位元電壓值

//GUID
const char GUID_USB_HUB[] = "{F18A0E88-C30C-11D0-8815-00A0C906BED8}";
const char GUID_USB_DEVICE[] = "{A5DCBF10-6530-11D2-901F-00C04FB951ED}";
const char GUID_HID[] = "{4d1e55b2-f16f-11cf-88cb-001111000030}";

// HID PVID
const wchar_t PD_BOARD_PVID[] = L"hid#vid_0835&pid_fe00";
//
#define GET_VALUE_TIMEOUT_MS 500
#define TIME_INTERVAL 200
#define HARDWARE_VOLTAGE_COMPENSATION 13

#define HID_IS_ONLINE		0
#define HID_TURN_ON			1
#define SET_VOLTAGE			2
#define EXECUTE_SETTING 	3
#define TEST_VOLTAGE_END	6
#define HID_NOT_FIND		7
#define DL_NOT_FIND		    8
#define TEST_PASS		    9
#define TEST_FAIL		    10
#define READ_NO_LOAD_VOLTAGE_VALUE		4
#define READ_FULL_LOAD_VOLTAGE_VALUE	5

#define DEBUG(String)    FrmMain->moDebug->Lines->Add(String)

//---------------------------------------------------------------------------

class TFrmMain : public TForm
{
__published:	// IDE-managed Components
	TPanel *pl_Set_Switch;
	TPanel *Panel1;
	TPanel *Panel2;
	TPanel *Panel3;
	TPanel *Panel4;
	TPanel *Panel5;
	TPanel *Panel6;
	TPanel *pl_read_5v_voltage;
	TPanel *pl_read_9v_voltage;
	TPanel *pl_read_12v_voltage;
	TPanel *pl_read_15v_voltage;
	TPanel *pl_read_20v_voltage;
	TBitBtn *btnStart;
	TPanel *pl_Auto;
	TPanel *pl_Auto9V;
	TPanel *pl_Auto5V;
	TPanel *pl_Auto12V;
	TPanel *pl_Auto15V;
	TPanel *pl_Auto20V;
	TCheckBox *ckbAuto;
	TPanel *Panel7;
	TPanel *pl_DutVPID;
	TPanel *Panel9;
	TPanel *Panel10;
	TPanel *Panel11;
	TPanel *Panel12;
	TEdit *edtVID;
	TEdit *edtPID;
	TPanel *plSet5V;
	TPanel *plSet9V;
	TPanel *plSet12V;
	TPanel *plSet15V;
	TPanel *plSet20V;
	TPanel *pl_Memo_Switch;
	TPanel *pl_Set;
	TPanel *Panel71;
	TEdit *edtPassWord;
	TBitBtn *btnSet;
	TPanel *pl_set_5;
	TPanel *plAD0;
	TEdit *edt_min5;
	TPanel *pl_set_4;
	TPanel *Panel13;
	TEdit *edt_min4;
	TPanel *pl_set_3;
	TPanel *Panel18;
	TEdit *edt_min3;
	TPanel *pl_set_2;
	TPanel *Panel23;
	TEdit *edt_min2;
	TPanel *pl_set_1;
	TPanel *Panel28;
	TEdit *edt_min1;
	TPanel *pl_ckb1_2;
	TPanel *pl_ckb2_2;
	TPanel *pl_ckb3_2;
	TPanel *pl_ckb4_2;
	TPanel *pl_ckb5_2;
	TEdit *edt_max1;
	TEdit *edt_max2;
	TEdit *edt_max3;
	TEdit *edt_max4;
	TEdit *edt_max5;
	TPanel *Panel16;
	TPanel *Panel21;
	TPanel *Panel26;
	TPanel *Panel31;
	TPanel *Panel37;
	TPanel *Panel63;
	TPanel *Panel64;
	TPanel *Panel65;
	TPanel *Panel14;
	TPanel *Panel15;
	TPanel *Panel17;
	TPanel *pl_Debug;
	TMemo *moDebug;
	TPanel *Panel20;
	TPanel *Panel19;
	TPanel *Panel41;
	TPanel *Panel42;
	TPanel *Panel43;
	TLabel *Label1;
	TPanel *pl_full_load_voltage5;
	TPanel *Panel45;
	TPanel *pl_full_load_voltage9;
	TPanel *pl_full_load_voltage12;
	TPanel *pl_full_load_voltage15;
	TPanel *pl_full_load_voltage20;
	TPanel *plFullLoad_1;
	TPanel *plFullLoad_2;
	TPanel *plFullLoad_3;
	TPanel *plFullLoad_4;
	TPanel *plFullLoad_5;
	TEdit *edt_load_min1;
	TEdit *edt_load_min2;
	TEdit *edt_load_min3;
	TEdit *edt_load_min4;
	TEdit *edt_load_min5;
	TPanel *Panel50;
	TPanel *Panel51;
	TPanel *Panel52;
	TPanel *Panel53;
	TPanel *Panel54;
	TEdit *edt_load_max1;
	TEdit *edt_load_max2;
	TEdit *edt_load_max3;
	TEdit *edt_load_max4;
	TEdit *edt_load_max5;
	TPanel *Panel8;
	TEdit *edtSetCurrent;
	TPanel *Panel22;
	TPanel *plIniName;
	TPanel *plResult;
	TPanel *Panel24;
	TTimer *Timer1;
	bool __fastcall HID_ReadReport(AnsiString Value);
	void __fastcall EnumHID();
	void __fastcall UI_DisplayCmd(unsigned char *pBuf, int size);
	void __fastcall btnStartClick(TObject *Sender);
	void __fastcall pl_Auto5VClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ckbAutoClick(TObject *Sender);
	void __fastcall edtVIDChange(TObject *Sender);
	void __fastcall plSet5VClick(TObject *Sender);

	void __fastcall moDebugChange(TObject *Sender);
	void __fastcall pl_Memo_SwitchClick(TObject *Sender);
	void __fastcall edtPassWordEnter(TObject *Sender);
	void __fastcall btnSetClick(TObject *Sender);
	void __fastcall pl_ckb1_1Click(TObject *Sender);
	void __fastcall edt_min1Change(TObject *Sender);
	void __fastcall edt_min1Exit(TObject *Sender);
	void __fastcall pl_Set_SwitchClick(TObject *Sender);
	void __fastcall plResultDblClick(TObject *Sender);
	void __fastcall edtPassWordKeyPress(TObject *Sender, wchar_t &Key);
private:	// User declarations
	HID m_hid;
	IO_Request m_ir, m_or;
	AnsiString m_ADValue;
	TPanel *zVoltageSetting[5];
	TPanel *zRead_AD_Voltage[5];
	TPanel *zRead_FULL_LOAD_Voltage[5];
	TPanel *zAuto_Test[5];

	void Tx(AnsiString Value);
	bool Rx(AnsiString Value);
	void Rx_ValueAnalyze(AnsiString Value);
	bool HID_TurnOn();
	void HID_TurnOff();
	void Voltage_Test(AnsiString CMD);
	void SetValueEnabled(bool Enabled);

	int  DL_CMD(AnsiString CMD,AnsiString PDVol);

	int  HexToInt(AnsiString HexStr);
	void ReadRegSet();
	void SetRegVal(AnsiString numTestItem,bool AutoTest,AnsiString DutVPID);
	bool CheckVPIDSET(TEdit * edt);
	bool SaveVoltageRangeSetting();
	bool FindIniFile();
	bool ReadInISet();
	bool SetInIVal(AnsiString numTestItem,bool AutoTest,AnsiString DutVPID);
	AnsiString Findfilemsg(AnsiString filename, AnsiString findmsg,
	int rownum);
	AnsiString FILE_DUT_SET_INI;
	//USB拔插消息
	DeviceNotification g_DeviceNogification;
	TWndMethod OldWindowProc;
	void __fastcall MyWindowProc(TMessage&);
public:		// User declarations

	bool g_bError;
	cCOM *CL_DEV_CONTROL;
	queue<AnsiString> que_cmd;
	void Delay(ULONG iMilliSeconds);
	bool bTEST_END_SWITCH_5V;
	__fastcall TFrmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMain *FrmMain;
//---------------------------------------------------------------------------
#endif
