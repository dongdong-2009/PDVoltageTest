//---------------------------------------------------------------------------

#ifndef COM_controlH
#define COM_controlH


#define TEST_END			0
#define RS232_WRITE			1
#define RS232_READ			2
#define TEST_PASS			3
#define TEST_FAIL			4
#define TEST_OCP			5
#define Dev_NO_RESPOND		6

#define GetVoltageCount		5

#include <Registry.hpp>

//---------------------------------------------------------------------------
struct tag_cmd_desc {
	char *cmd;
	int (*pfn)(char *s);
	};
	typedef struct tag_cmd_desc CMD_DESC;
class cCOM{
	private:


	public:
		cCOM::cCOM(void);

		double GetNumOfString(AnsiString String);
		bool 	open_dev_com();
		int 	Dev_CMD_Test(AnsiString PDVol);
		void 	search_Reg_ComNum();
		void 	SearchDevCOM();

		void 	QueueClear();
		void 	Dev_Stop();

		bool 	bCOM_PORT_OPEN;
		bool 	bNoResponse;

};
#endif
