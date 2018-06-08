// Module:
// Notices: Copyright(c) 2015 Robbin Hsiao.
// Purpose:
// Compiler: Embarcadero RAD Studio XE8
// OS: Windows 8.1 x64
// Version: 0.0.0.2
//---------------------------------------------------------------------------
DEFINE_GUID(GUID_CLASS_USB_DEVICE_USB, 0xb6779553, 0x7531, 0x4c04, 0xb5, 0xad, 0x7c, 0xa0, 0x45, 0x8e, 0xe9, 0x49); //Online GUID Generator Merlin 20150603
#ifndef DeviceNotificationH
#define DeviceNotificationH

#include <dbt.h>

#ifdef __cplusplus
	extern "C" {
#endif
#include <initguid.h>
#ifdef __cplusplus
	}
#endif
//---------------------------------------------------------------------------
class DeviceNotification
{
private:
	HDEVNOTIFY hdevnotify;
private:
	inline void Unregister(){
		if(hdevnotify){UnregisterDeviceNotification(hdevnotify);hdevnotify = NULL;}
	}

	bool Register(HWND hwnd, GUID guid, DWORD flags){
		// 註冊要取得的裝置消息
		// #include <dbt.h>
		DEV_BROADCAST_DEVICEINTERFACE notification_Filter;

		ZeroMemory(&notification_Filter, sizeof(notification_Filter));
		notification_Filter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
		notification_Filter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;

		// 註冊獲取裝置訊息
		notification_Filter.dbcc_classguid = guid;

		hdevnotify = RegisterDeviceNotification(
			hwnd, &notification_Filter, flags);

		return(hdevnotify != NULL);
	}
public:
	bool RegisterWindowsDeviceInterfaceNotification(HWND hwnd, GUID guid){
		return Register(hwnd, guid, DEVICE_NOTIFY_WINDOW_HANDLE);
	}
public:
	DeviceNotification():hdevnotify(NULL){}
	~DeviceNotification(){Unregister();}
};
//---------------------------------------------------------------------------
#endif
