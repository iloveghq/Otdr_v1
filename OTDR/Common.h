#pragma once

//logic command
enum Commands : int
{
	NONE = 0,

	//File Menu Command
	FILE_OPEN = 10,
	FILE_SAVE = 11,
	FILE_PRINT = 12,
	FILE_PRINT_PREVIEW = 13,

	//Tool Menu Command
	REMOTE_CONNECT = 20,
	REMOTE_SETTING = 21,
	LANG_SWITCH = 22,
	SCREEN_SETTING = 23,

	//View Menu Command
	TOOLBAR_VIEW = 30,
	//REMOTE_SETTING = 31,
	//LANG_SWITCH = 32,
	//SCREEN_SETTING = 33,

	//Event Analyse Condition
	EVENT_ANALYSE_CON = 40,

	//Report Menu Command
	REPORT_WIZARD = 50
};


//support language
enum LANG : int
{
	CHS = 0,   //Chinese
	ENG = 1,   //English
	PORT = 2   //Portugues
};

//report page
enum REPORT_PAGE : int
{
	PROP_SETTING = 0, //����1
	PROP_FILE_SELECT = 1,//����2
	PROP_CONFIG = 2,   //����3���뱨�����ʱ��һЩ������ݡ�lzy2014.9.12
	PROP_OUTPUT = 3, //����4
	
	//PROP_WAVE_ITEM = 1, //����2
	PROP_WAVE_SETTING = 10, //����2
	PROP_RPT_SETTING =11, //����3����
	
	
	
};

//ͼ�λ�������
enum CURSOR_SELECT : int
{
	CURSOR_NONE = 0,
	CURSOR_A = 1,		//ѡ��A
	CURSOR_B = 2,		//ѡ��B
	CURSOR_AB = 4,		//ѡ��AB
	CURSOR_EVENT = 8	//ѡ���¼�
};