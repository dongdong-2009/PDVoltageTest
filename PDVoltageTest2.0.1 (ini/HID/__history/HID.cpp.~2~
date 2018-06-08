
//---------------------------------------------------------------------------
#include <tchar.h>
#include "HID.h"
#include <strsafe.h>
//---------------------------------------------------------------------------
//#pragma package(smart_init)
//---------------------------------------------------------------------------
void HID::EnumPresentHidPaths(vector<tstring> *pPaths){
    Find(pPaths);
}

unsigned short HID::GetRxBytes(){
	if(!IsConfigured()){
		return 0;
	}

	return m_RxBytes;
}

unsigned short HID::GetTxBytes(){
	if(!IsConfigured()){
		return 0;
	}
	return m_TxBytes;
}

TCHAR* HID::GetPath(){
    if(!IsConfigured()){
		return NULL;
	}
	return m_path;
}

void HID::GetRxTxBytes(){
	PHIDP_PREPARSED_DATA  pData;
	HIDP_CAPS caps;
	NTSTATUS nt = HIDP_STATUS_INVALID_PREPARSED_DATA;

	if(HidD_GetPreparsedData(m_hRead, &pData)){
		nt = HidP_GetCaps(pData, &caps);
		if(nt == HIDP_STATUS_SUCCESS){
			m_RxBytes = caps.InputReportByteLength;
			m_TxBytes = caps.OutputReportByteLength;
		}

		HidD_FreePreparsedData(pData);
	}
}

bool HID::IsConfigured(){
	if(!m_configured){
		//MessageBox(NULL, _T("HID物件未配置，請執行Configured()進行配置!"), _T("HID"), MB_ICONWARNING);
		return false;
	}

	return true;
}

TCHAR* HID::GetSymbol(){
	if(!IsConfigured()){
        return NULL;
	}

    return m_symbol;
}

bool HID::Configure(TCHAR vid[5], TCHAR pid[5], TCHAR mi[3], TCHAR col[3]){
	int err;

	if(_tcscpy_s(m_vid, 5, vid) != 0) return false;
	if(_tcscpy_s(m_pid, 5, pid) != 0) return false;
	if(_tcscpy_s(m_mi, 3, mi) != 0) return false;
	if(_tcscpy_s(m_col, 3, col) != 0) return false;

	//"\\\\?\\hid#vid_2010&pid_0231&mi_01&col02"
	err = _stprintf_s(m_symbol, DEVICE_SYMBOL_LENGTH, _T("\\\\?\\hid#vid_%s&pid_%s"),
		m_vid, m_pid);

	if(err == -1){
		return false;
	}

	if(m_mi[0] != 0){
		_tcscat_s(m_symbol, DEVICE_SYMBOL_LENGTH, _T("&mi_"));
		_tcscat_s(m_symbol, DEVICE_SYMBOL_LENGTH, m_mi);
	}

	if(m_col[0] != 0){
		_tcscat_s(m_symbol, DEVICE_SYMBOL_LENGTH, _T("&col"));
		_tcscat_s(m_symbol, DEVICE_SYMBOL_LENGTH, m_col);
	}


	m_configured = true;

	return true;
}

bool HID::Open(){
	bool bl;

	if(!IsConfigured()){
		return false;
	}

	bl = Find();

	if(bl){
		bl = CreateHandle();
	}

	if(bl){
		GetRxTxBytes();
		m_online = true;
	}else{
		m_online = false;
	}

	return bl;
}

void HID::Close(){
	m_online = false;
    ReleaseHandle();
}

bool HID::IsOnline(){
	if(Open()){
		Close();
	}

	return m_online;
}

bool HID::GetPath(HDEVINFO hDevinfo, SP_DEVICE_INTERFACE_DATA *pData, vector<tstring> *pPaths){
	bool bl;
	unsigned long bytes;
	SP_DEVICE_INTERFACE_DETAIL_DATA *pDetailData = {0};

	//清除緩衝區內容
	memset(m_path, DEVICE_PATH_LENGTH * sizeof(TCHAR), 0);

	//Get details about a device interface.
	//1. Get the required buffer size
	bl = SetupDiGetDeviceInterfaceDetail(
		hDevinfo,
		pData,
		NULL,
		0,
		&bytes,
		NULL);

	if(!bl && ERROR_INSUFFICIENT_BUFFER == GetLastError()){
		//Allocate an appropriately sized buffer and call the function again to get the interface details.
		//配置記憶體
		pDetailData = (SP_DEVICE_INTERFACE_DETAIL_DATA *)malloc(bytes);
		//初始結構
		pDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		//第二次呼叫取得裝置路徑
		bl = SetupDiGetDeviceInterfaceDetail(
			hDevinfo,       //取得裝置路徑
			pData,
			pDetailData,
			bytes,
			NULL,
			NULL);

		if(bl){
			if(pPaths){
                pPaths->push_back(pDetailData->DevicePath);
			}else{
				if(_tcsstr(pDetailData->DevicePath, m_symbol) != NULL){
					if(_tcscpy_s(m_path, DEVICE_PATH_LENGTH, pDetailData->DevicePath) != 0){
						bl = false;
					}
				}else{
					bl = false;
				}
			}
		}

		free(pDetailData);
	}else{
        bl = false;
	}

	return bl;
}

bool HID::Find(vector<tstring> *pPaths){
	bool bl = false;
	SP_DEVICE_INTERFACE_DATA data = {sizeof(SP_DEVICE_INTERFACE_DATA)};
	DWORD index = 0;

	//Get device information set
	//API function: SetupDiGetClassDevs
	//Returns: a handle to a device information set for all installed devices.
	//Requires: the GUID returned by GetHidGuid.
	//取得 HDEVINFO 在離開函式前釋放
	HDEVINFO hDevinfo = SetupDiGetClassDevs(
		&m_classGuid,   //取得所有HID訊息
		NULL,
		NULL,
		DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

	if (INVALID_HANDLE_VALUE == hDevinfo){
		::MessageBox(NULL, _T("_Get HID　device information set failed!"), _T("EnumHidDeviceSymvols"), MB_ICONSTOP);
		return false;
	}

	//Enum device information & Get details about a device interface.
	while(
		SetupDiEnumDeviceInterfaces(
			hDevinfo,    //辨識每一個HID介面
			NULL,
			&m_classGuid,
			index++,
			&data)
		&& ERROR_NO_MORE_ITEMS != GetLastError()
	){
		bl = GetPath(hDevinfo, &data, pPaths);

		if(bl && pPaths == NULL){
			break;
		}
	}//end while

	SetupDiDestroyDeviceInfoList(hDevinfo);
	hDevinfo = INVALID_HANDLE_VALUE;

	return bl;
}

bool HID::CreateHandle(){
	bool bl = false;
//	HANDLE handle[2] = {INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE};

	ReleaseHandle();

	__try {
		try {
			m_hWrite = CreateFile(
				m_path,
				GENERIC_WRITE|GENERIC_READ,
				FILE_SHARE_WRITE|FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_FLAG_OVERLAPPED,
				NULL);

			if(INVALID_HANDLE_VALUE == m_hWrite){ // cannot open the drive
				::MessageBox(NULL, _T("_Can't open the HID write Handle!"), _T("HID"), MB_ICONSTOP);
//			 	__leave;
				throw -1;
			}

			m_hRead = CreateFile(
				m_path,
				GENERIC_WRITE|GENERIC_READ,
				FILE_SHARE_WRITE|FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_FLAG_OVERLAPPED,
				NULL);

			if (INVALID_HANDLE_VALUE == m_hRead){
				::MessageBox(NULL, _T("_Can't open the HID read Handle!"), _T("HID"), MB_ICONSTOP);
//				__leave;
				throw -1;
			}
		} catch (...) {

		}

		bl = true;
	}__finally{
		if(!bl){
			ReleaseHandle();
		}
	}

	return bl;
}

void HID::ReleaseHandle(){
	if(m_hWrite != INVALID_HANDLE_VALUE){
		CloseHandle(m_hWrite);
		m_hWrite = INVALID_HANDLE_VALUE;
	}

	if(m_hRead != INVALID_HANDLE_VALUE){
		CloseHandle(m_hRead);
		m_hRead = INVALID_HANDLE_VALUE;
	}
}
//---------------------------------------------------------------------------
bool HID::Write(const unsigned char *cmd){
	bool bl;
	unsigned long numberOfBytes = 0;

	bl = WriteFile(m_hWrite, cmd, 5, &numberOfBytes, 0) && numberOfBytes == 8;

	return bl;
}

bool HID::Read(unsigned char *cmd){
	bool bl;
	unsigned long numberOfBytes = 0;
	::ZeroMemory(cmd, 8);

	bl = ReadFile(m_hRead, cmd, 8, &numberOfBytes,0) && numberOfBytes == 8;

	return bl;
}

unsigned char HID::ExecuteTask(const unsigned char writeBuffer[1], unsigned char readBuffer[1]){
	if(this->m_hWrite == INVALID_HANDLE_VALUE || this->m_hRead == INVALID_HANDLE_VALUE){
        MessageBox(NULL, _T("The HID not to be creatted!"), _T("HID"), MB_ICONSTOP);
		return 0x01;
	}

	if(!Write(writeBuffer)){
		MessageBox(NULL, _T("Write to HID fail!"), _T("HID"), MB_ICONSTOP);
		return 0x02;
	}

	if(!Read(readBuffer)){
		MessageBox(NULL, _T("Read from HID fail!"), _T("HID"), MB_ICONSTOP);
		return 0x03;
	}

	return 0x00;
}
//---------------------------------------------------------------------------
HID::HID() : m_configured(false),m_hWrite(INVALID_HANDLE_VALUE), m_hRead(INVALID_HANDLE_VALUE){
	 m_online = false;
	//Only user-mode applications can call HidD_GetHidGuid.
	//Get GUID for HIDClass devices.
	/*
	API function: HidD_GetHidGuid
	Get the GUID for all system HIDs.
	Returns: the GUID in HidGuid.
	*/

	HidD_GetHidGuid(&m_classGuid);
}

HID::~HID(){
	ReleaseHandle();
}
