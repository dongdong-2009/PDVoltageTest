
//---------------------------------------------------------------------------
#ifndef HIDH
#define HIDH

#define DEVICE_PATH_LENGTH 95
#define DEVICE_SYMBOL_LENGTH 38

#include <windows.h>
//#include <guiddef.h>
extern "C"
{
#include "setupapi.h"
#include "hidsdi.h"  //HidD_GetHidGuid(&HidGuid);
}

#include <vector>
using namespace std;
typedef std::basic_string<TCHAR> tstring;
//#ifndef UNICODE
//  typedef std::string  tstring;
//#else
//  typedef std::wstring tstring;
//#endif
//---------------------------------------------------------------------------
class HID {
private:
	bool m_configured;
	GUID m_classGuid; //HID ªº Ãþ§O GUID
	TCHAR m_vid[5], m_pid[5], m_mi[3], m_col[3];
	TCHAR m_symbol[DEVICE_SYMBOL_LENGTH];
	TCHAR m_path[DEVICE_PATH_LENGTH];
	unsigned short m_RxBytes, m_TxBytes;
protected:
	bool IsConfigured();
	bool Find(vector<tstring> *pPaths = NULL);
	bool GetPath(HDEVINFO hDevinfo, SP_DEVICE_INTERFACE_DATA *pData, vector<tstring> *pPaths);
	bool CreateHandle();
	void ReleaseHandle();
public:
	bool m_online;
	HANDLE m_hWrite; // = INVALID_HANDLE_VALUE;
	HANDLE m_hRead; // = INVALID_HANDLE_VALUE;

	HID();
	~HID();

	bool Configure(TCHAR vid[5], TCHAR pid[5], TCHAR mi[3], TCHAR col[3]);
	bool Open();
	bool IsOnline();
	bool Write(const unsigned char *cmd);
	bool Read(unsigned char *cmd);
	unsigned char ExecuteTask(const unsigned char writeBuffer[1], unsigned char readBuffer[1]);
	TCHAR* GetSymbol();
	TCHAR* GetPath();
	unsigned short GetRxBytes();
	unsigned short GetTxBytes();
	void GetRxTxBytes();
	void Close();
	void EnumPresentHidPaths(vector<tstring> *pPaths);

};
// ---------------------------------------------------------------------------
#endif
