//---------------------------------------------------------------------------

#ifndef IO_RequestH
#define IO_RequestH

//---------------------------------------------------------------------------
class IO_Request : public OVERLAPPED
{
private:
	void *m_pvBuf;
	size_t m_nBufSize;
	int m_type;
public:
	IO_Request(){
		Internal = InternalHigh = 0;
		Offset = OffsetHigh = 0;
		hEvent = NULL;
	}

	~IO_Request(){
		FreeBuf();
	}

	void FreeBuf(){
		if(m_pvBuf != NULL){
			VirtualFree(m_pvBuf, 0, MEM_RELEASE);
		}
	}

	bool AllocBuf(size_t nBufSize){
		m_nBufSize = nBufSize;
		FreeBuf();
		m_pvBuf = VirtualAlloc(NULL, m_nBufSize, MEM_COMMIT, PAGE_READWRITE);
		return (m_pvBuf != NULL);
	}

	bool Read(HANDLE hDevice, PLARGE_INTEGER pliOffset = NULL){
		if(pliOffset != NULL){
			Offset = pliOffset->LowPart;
			OffsetHigh = pliOffset->HighPart;
		}
		return (ReadFile(hDevice, m_pvBuf, m_nBufSize, NULL, this));
	}

	bool Write(HANDLE hDevice, PLARGE_INTEGER pliOffset = NULL){
		if(pliOffset != NULL){
			Offset = pliOffset->LowPart;
			OffsetHigh = pliOffset->HighPart;
		}
		return (WriteFile(hDevice, m_pvBuf, m_nBufSize, NULL, this));
	}

	const void * GetBufAddr(){
		return m_pvBuf;
//		memcpy(pchrBuf, m_pvBuf, m_nBufSize);
	}


//	void SetBuf(const unsigned char *pcmd){
//
//	}

	const int GetBufSize(){
		return m_nBufSize;
//		memcpy(pchrBuf, m_pvBuf, m_nBufSize);
	}

	void SetType(int type){
		m_type = type;
	}

	int GetType(){
        return m_type;
	}
//	BOOL WINAPI CancelIo(_In_  HANDLE hFile);
};
//---------------------------------------------------------------------------
#endif
