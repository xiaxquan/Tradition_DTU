/**
  * @file   GUIdisplay.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    03-06-2018
  * @brief   GUI��ʾ
  */

#include "gui_common.h"
#include "GUIdisplay.h"
#include "GUIdisplaydef.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "userVariable.h"
#include "gui_101_cmd.h"
#include "hmiInOut.h"
#include "common_data.h"

uint8_t *userGUIBuff; /* �����ڴ���� */
const static uint8_t *modfiyKey[14] = {/* ���� */
	"0","1","2","3","4",".","esc","5","6","7","8","9","<-","ok"};
static enum KeyStatus keyStatus;	/* ����ֵ */
static struct DZModfiy dZModfiy;	/* ��ֵ�޸� */
static struct Message MessageIs;	/* ��Ϣ���� */
static struct PassWordPipe passWordPipe;		/* ������� */
static struct PasswordSave passwordSave;		/* �������� */
static enum DingZhiSetOrSee dingZhiSetOrSee;	/* ��ֵ��ѯ�����޸ı�־ */
static uint8_t stepTab[STEP_ALLNUM];
static uint8_t DingzhiChangeFlag,DzChangefinshflag;
/* �˵���� */
lkdMenu HmiMenu;
/* �˵�ջ */
#define MENUSTACK_NUM 8
MenuStack userMenuStack[MENUSTACK_NUM];

/* ���ڶ��� */
WINDOW HomeWindow = {0,0,160,160,NULL,HomeWindowFun,NULL};/* ������ */
WINDOW MenuWindow = {0,0,160,160,"�˵�������",MenuWindowFun,NULL};

WINDOW TimeModfiyWin  = {12,44,136,66,"ʱ���޸Ĵ���",TimeModfiyFun,NULL};
WINDOW CmdSendWin = {0,0,160,160,"�����·�����",CmdSendWinFun,NULL};
WINDOW VersionWin = {0,0,160,160,"�汾��Ϣ����",VersionWinFun,NULL};
WINDOW PasswordModfiyWin = {0,0,160,160,"�޸�����",PasswordModfiyWinFun,NULL};

WINDOW YaoxinWin     = {0,0,160,160,"ң��״̬����",YaoxinFun,NULL};
WINDOW SOEWin   	 = {0,0,160,160,"SOE��¼����",SoeCoWinFun,&soeInfo};
WINDOW COWin   	 = {0,0,160,160,"������¼����",SoeCoWinFun,&coInfo};
WINDOW FaultEventWin = {0,0,160,160,"�����¼�����",FaultEventFun,NULL};
WINDOW CheckRecordWin = {0,0,160,160,"�����Ϣ����",SoeCoWinFun,&ulogInfo};

WINDOW BasicSetWin   = {0,0,160,160,"�������ô���",DingzhiWinFun,&dzhi0Info[ME_BASIC_SET]};
WINDOW ZeroDriftWin  = {0,0,160,160,"��Ư���ô���",DingzhiWinFun,&dzhi0Info[ME_ZERODRIFT]};
//WINDOW DeadZoneWin 	 = {0,0,160,160,"�������ô���",DingzhiWinFun,&dzhi0Info[ME_UART_COM]};
WINDOW SerialWin     = {0,0,160,160,"�������ô���",DingzhiWinFun,&dzhi0Info[ME_UART_COM]};
WINDOW InternetWin 	 = {0,0,160,160,"�������ô���",DingzhiWinFun,&dzhi0Info[ME_NET_COM]};

WINDOW YaoCe1Win   = {0,0,160,160,"һ��ң�ⴰ��",YaoceWinFun,&yceInfo[0]};
WINDOW YaoCe2Win   = {0,0,160,160,"����ң�ⴰ��",YaoceWinFun,&yceInfo[1]};
WINDOW HarmonicWin = {0,0,160,160,"г����������",YaoceWinFun,&yceInfo[2]};

/* ���ô��� */
WINDOW DZModfiyWin = {14,19,130,134,"��ֵ�޸Ĵ���",DZModfiyFun,NULL};
WINDOW MessageWin = {23,40,110,75,"��Ϣ����",MessageFun,NULL};
WINDOW PassWordWin = {23,40,110,75,"���봰��",PassWordFun,NULL};
WINDOW DzhiChangeWin = {23,40,110,80,"��Ϣ��ʾ",DzhiChangeFun,&DzChangefinshflag};

/* �˵��ڵ㶨�� */
/* -----------------------------------------------�˵��ṹ---------------------------------------------------- */
lkdMenuNode Node22 = {MID_YAOXIN,"ң�Ų�ѯ",NULL,NULL,&YaoxinWin};
lkdMenuNode Node21 = {MID_HARMONIC_COMPONENT,"г������",&Node22,NULL,&HarmonicWin};
lkdMenuNode Node20 = {MID_SECOND_TELEMETRY,"����ң��",&Node21,NULL,&YaoCe2Win};
lkdMenuNode Node19 = {MID_ONE_TELEMETRY,"һ��ң��",&Node20,NULL,&YaoCe1Win};

lkdMenuNode Node18 = {MID_BASICSET,"��������",NULL,NULL,&BasicSetWin};

lkdMenuNode Node23 = {MID_PASSWORDMODFIY,"�����޸�",NULL,NULL,&PasswordModfiyWin};
lkdMenuNode Node17 = {MID_TIMEMODFIY,"ʱ���޸�",&Node23,NULL,&TimeModfiyWin};
lkdMenuNode Node16 = {MID_ZERODRIFT,"��Ư����",&Node17,NULL,&ZeroDriftWin};
//lkdMenuNode Node15 = {MID_DEADEZONE,"��������",&Node16,NULL,&DeadZoneWin};
lkdMenuNode Node14 = {MID_INTERNET,"��������",&Node16,NULL,&InternetWin};
lkdMenuNode Node13 = {MID_SERIAL,"��������",&Node14,NULL,&SerialWin};

lkdMenuNode Node12 = {MID_CHECKRECORD,"�Լ��¼",NULL,NULL,&CheckRecordWin};
lkdMenuNode Node11 = {MID_DEFAULTEVENT,"�����¼�",&Node12,NULL,&FaultEventWin};
lkdMenuNode Node10 = {MID_OPERATERECORD,"�����¼�",&Node11,NULL,&COWin};
lkdMenuNode Node9 = {MID_SOE,"SOE��ѯ",&Node10,NULL,&SOEWin};

lkdMenuNode Node8 = {MID_VERSIONINFO,"�汾��Ϣ",NULL,NULL,&VersionWin};
lkdMenuNode Node7 = {MID_SYSTEM_PARAMTER_SETTING,"ϵͳ����",&Node8,&Node18,NULL};
lkdMenuNode Node6 = {MID_DEVICE_PARAMTER_SETTING,"װ������",&Node7,&Node13,NULL};
lkdMenuNode Node5 = {MID_DZHISET,"��ֵ����",&Node6,NULL,NULL};
lkdMenuNode Node4 = {MID_CMDSEND,"�����·�",&Node5,NULL,&CmdSendWin};
lkdMenuNode Node3 = {MID_DEVICEEVENTRECORD,"�¼���¼",&Node4,&Node9,NULL};
lkdMenuNode Node2 = {MID_DZHISCAN,"��ֵ��ѯ",&Node3,NULL,NULL};
lkdMenuNode Node1 = {MID_REALTIMEDATA,"ʵʱ����",&Node2,&Node19,NULL};
lkdMenuNode Node0 = {MID_ROOT,"root",NULL,&Node1,NULL};
/* ----------------------------------------------------------------------------------------------------------- */

/**
  *@brief  �˵���ʼ��
  *@param  None
  *@retval None
  */
void UserMenuInit(void)
{
	HmiMenu.x = 1;
	HmiMenu.y = 16;
	HmiMenu.wide = 158;
	HmiMenu.hight = 142;
	HmiMenu.Itemshigh = 15;
	HmiMenu.ItemsWide = 75
    ;
	HmiMenu.stack = userMenuStack;
	HmiMenu.stackNum = 8;
	HmiMenu.Root = &Node0;
	HmiMenu.MenuDealWithFun = MenuControlFun;
	GuiMenuInit(&HmiMenu);
}

/**
  *@brief  ��ȡʱ��
  *@param  None
  *@retval ��ǰʱ�ӵδ�
  */	
static uint32_t getCurrentTick(void)
{
	return GetTimer1Tick();
}

/**
  *@brief  ��ȡ���ʱ��
  *@param  ��ʼʱ�ӵδ�
  *@retval ���ʱ��
  */
static uint32_t GetIntervalTick(uint32_t beginTick)
{
	return GetTimer1IntervalTick(beginTick);
}

/**
  *@brief �޸Ķ�ֵ���û��������־
  *@param  setOrSee ���á���� ö��
  *@retval None
  */
static void SetDingZhiSetOrSee(enum DingZhiSetOrSee setOrSee)
{
	dingZhiSetOrSee = setOrSee;
}

/**
  *@brief ��ȡ��ֵ���û��������־
  *@param  None
  *@retval DingZhiSetOrSee ���á���� ö��
  */
static enum DingZhiSetOrSee GetDingZhiSetOrSee(void)
{
	return dingZhiSetOrSee;
}

/**
  *@brief lcd��ʾ����
  *@param  key ����״̬
  *@retval lcd��ʾ״̬ LCDDISPLAYON LCDDISPLAYOFF
  */
static uint8_t GUIDisplayONOFF(enum KeyStatus *key)
{
	static uint32_t LcdDisPlayTick;//��ʱ
	static uint8_t lcdONOFF;
	if(stepTab[STEP_UPDATAPRO] == 0){
		OpenLcdDisplay();
		lcdONOFF = LCDDISPLAYON;
		LcdDisPlayTick = getCurrentTick();
		stepTab[STEP_UPDATAPRO] = 1;
	}
	if(stepTab[STEP_UPDATAPRO] == 1){
		if(*key != NoKey){
			LcdDisPlayTick = getCurrentTick();
			if(lcdONOFF == LCDDISPLAYOFF){
				*key = NoKey;
				OpenLcdDisplay();
				lcdONOFF = LCDDISPLAYON;
			}
		}
		else{
			if(lcdONOFF == LCDDISPLAYON){
				if(GetIntervalTick(LcdDisPlayTick) > LCDDISPLAYOFFTIME){//ʱ��
					CloseLcdDisplay();
					lcdONOFF = LCDDISPLAYOFF;
				}
			}
		}
	}
	return lcdONOFF;
}

/**
  *@brief GUI���߸���
  *@param  None
  *@retval None
  */
void GUIUpdataProgram(void)
{
	uint8_t i,j;
	lkdBitmap tImage;
	GuiFillRect(1,1,158,158, backcolor);
	tImage.number = 1;
	tImage.wide = 160;
	tImage.high = 30;
	tImage.beginx = 0;
	tImage.beginy = 0;
	tImage.bitmap = NULL;
	GuiAreaBitMap(&tImage,1, 25, 159, 55, 1);
	j = 110;
	for(i=1;i<24;i+=2){
		GuiHPointLine(1,i,j,2,forecolor);
		j -= 2;
	}
	for(i=25;i<55;i+=2){
		GuiHPointLine(1,i,159,2,forecolor);
	}
	j = 70;
	for(i=55;i<78;i+=2){
		GuiHPointLine(j,i,159,2,forecolor);
		j -= 2;
	}
	GuiHLine(1,78,158,forecolor);
	GuiFillRect(1,81,159,94, forecolor);
	GuiHLine(1,96,158,forecolor);
	GuiExchangeColor();
	GuiFont12Align(2,82,156,FONT_MID,"���ڸ���......");
	GuiExchangeColor();
	GuiUpdateDisplayAll();
}

/**
  *@brief GUI�ȴ�ͼ��
  *@param  x,y ��ʼ����
  *@retval None
  */
void GUIWiatMessage(uint8_t x,uint8_t y)
{
	GuiFillRect(x,y,x + 88,y + 16, backcolor);
	GuiHLine(x + 2,y + 16,x + 86,forecolor);
	GuiHLine(x + 3,y + 17,x + 87,forecolor);
	GuiRLine(x + 86,y + 2,y + 16,forecolor);
	GuiRLine(x + 87,y + 3,y + 16,forecolor);
	GuiFont12Align(x + 20,y + 2,70,FONT_LEFT,"������.....");
	GuiFillRect(x + 2,y + 2,x + 14,y + 14, forecolor);
	GuiHLine(x + 2,y + 8,x + 14,backcolor);
	GuiRLine(x + 8,y + 2,y + 14,backcolor);
	GuiFillRect(x + 2,y + 2,32 + 12,y + 14, forecolor);
	GuiHLine(x + 2,y + 8,x + 14,backcolor);
	GuiRLine(x + 8,y + 2,y + 14,backcolor);
	GuiUpdateDisplayAll();
}

/**
  *@brief ��ֵ�仯����
  *@param  x,y ��ʼ����
  *@retval None
  */
void DzhiChangeDealWith(void)
{
	if(DingzhiChangeFlag == 1 && DzChangefinshflag == 0){
		DzChangefinshflag = 1;
		userGUIWindowAdd(&DzhiChangeWin);
	}
}
/**
  *@brief GUI��ʼ�������������
  *@param  None
  *@retval None
  */
void GUIDisplayInit(void)
{
	UserMenuInit();
	WinManageInit();
	GuiSetForecolor(1);
	GuiSetbackcolor(0);
	for(uint8_t i = 0; i < STEP_ALLNUM; i++){
		stepTab[i] = 0;
	}
	DzChangefinshflag = 0;
	userGUIWindowAdd(&HomeWindow);
}

/**
  *@brief GUI����������������������
  *@param  None
  *@retval None
  */
void GUIDisplayMian(void)
{
	static uint8_t flag;
	keyStatus = GetKeyStatus();//��ȡ����״̬
	SetKeyIsNoKey();
	if(CheckUpdataProgram() != 0){
		if(flag == 0){
			OpenLcdDisplay();
			GUIUpdataProgram();//���߸���
			flag = 1;
		}
	}
	else if(GUIDisplayONOFF(&keyStatus) == LCDDISPLAYON){
		userGUITopWindowDisplay();
		DzhiChangeDealWith();
		if(flag == 1){
			flag = 0;
		}
	}
}

/**
  *@brief �������봰�����ֵ
  *@param  type ��������
  *@param  returnflag ����״ָ̬��
  *@retval None
  */
void SetPassWordWin(uint8_t type,uint8_t *returnflag)
{
	passWordPipe.passWordType = type;
	passWordPipe.returnFlag = returnflag;
}

/**
  *@brief ���봰��
  *@param  None
  *@retval None
  */
void PassWordFun(void *param)
{	
	static uint8_t passWordStr[8];
	static int8_t currentNum;
	uint8_t tempStr[2];
	uint8_t i;
	if(stepTab[STEP_PASSWORD] == 0){
		currentNum = 0;
		for(i=0;i<7;i++){
			passWordStr[i] = '0';
		}
		passWordStr[7] = '\0';
		GuiFillRect(PassWordWin.x+1,PassWordWin.y+16,\
			PassWordWin.x + PassWordWin.wide - 2,PassWordWin.hight - 1, backcolor);
		GuiHLine(PassWordWin.x+1,PassWordWin.y+18,\
			PassWordWin.x + PassWordWin.wide - 1,forecolor);
		GuiFont12Align(PassWordWin.x+1,PassWordWin.y +20,PassWordWin.wide - 1,FONT_LEFT,\
			PassWordItems[passWordPipe.passWordType].name);//��ʾ��
		GuiHPointLine(PassWordWin.x+1,PassWordWin.y +34,\
			PassWordWin.x + PassWordWin.wide - 1,2,forecolor);
		stepTab[STEP_PASSWORD] = 1;
	}
	if(stepTab[STEP_PASSWORD] == 1){
		for(i=0;i<6;i++){
			if(i == currentNum){
				tempStr[0] = passWordStr[currentNum];
				tempStr[1] = '\0';
				GuiFillRect(PassWordWin.x + 25 + i*10 - 1,PassWordWin.y +45,\
					PassWordWin.x + 25 + i*10 + 6,PassWordWin.y +57, forecolor);
				GuiExchangeColor();
				GuiFont12Align(PassWordWin.x + 25 + i*10,PassWordWin.y +45,12,FONT_LEFT,tempStr);
				GuiExchangeColor();
			}
			else{
				GuiFillRect(PassWordWin.x + 25 + i*10 - 1,PassWordWin.y +45,\
					PassWordWin.x + 25 + i*10 + 6,PassWordWin.y +57, backcolor);
				GuiFont12Align(PassWordWin.x + 25 + i*10,PassWordWin.y +45,12,FONT_LEFT,"*");
			}		
		}
		GuiUpdateDisplayAll();
		stepTab[STEP_PASSWORD] = 2;
	}
	switch(keyStatus){
	case LeftKey:
		currentNum --;
		if(currentNum < 0){
			currentNum = 5;
		}
		stepTab[STEP_PASSWORD] = 1;
		break;
	case DownKey:
		passWordStr[currentNum] --;
		if(passWordStr[currentNum]<'0'){
			passWordStr[currentNum] = '9';
		}
		stepTab[STEP_PASSWORD] = 1;
		break;
	case RightKey:
		currentNum ++;
		if(currentNum >= 6){
			currentNum = 0;
		}
		stepTab[STEP_PASSWORD] = 1;
		break;
	case UpKey:
		passWordStr[currentNum] ++;
		if(passWordStr[currentNum] > '9'){
			passWordStr[currentNum] = '0';
		}
		stepTab[STEP_PASSWORD] = 1;
		break;
	case OkKey:
		if(strncmp((const char *)passWordStr,(const char *)PassWordItems[passWordPipe.passWordType].passWord,6) == 0 ||\
			 strncmp((char *)passWordStr,MANUFACTURER_PASSWORD,6) == 0){
			*passWordPipe.returnFlag = 1;
		}
		else{
			*passWordPipe.returnFlag = 0;
		}//��Ҫִ��CancelKey��������û��break
	case CancelKey:
		stepTab[STEP_PASSWORD] = 0; 
		userGUITopWindowHide();
		userGUITopWindowRedraw();
		GuiMenuRedrawMenu(&HmiMenu);
		break;
	default:break;
	}
}
/**
  *@brief ������Ϣ������Ӧ����
  *@param  x ,y ��ʼ����
  *@param  pStr ˵���ַ���
  *@param  flag ���ر�־ 0 ȡ�� 1 ȷ��
  *@retval None
  */
void SetMessageWin(int16_t x,int16_t y,uint8_t *pStr,uint8_t *flag)
{
	MessageWin.x = x;
	MessageWin.y = y;
	MessageIs.str = pStr;
	MessageIs.flag = flag;
}
/**
  *@brief ��Ϣ���ڴ�����
  *@param  None
  *@retval None
  */
void MessageFun(void *param)
{
	static int8_t keyFlag;
	uint8_t winWide;
	uint8_t winHight;
	if(stepTab[STEP_MESSAGE] == 0){
		keyFlag = 0;
		stepTab[STEP_MESSAGE] = 1;
	}
	if(stepTab[STEP_MESSAGE] == 1){
		GuiFillRect(MessageWin.x+1,MessageWin.y+16,\
			MessageWin.x + MessageWin.wide - 2,MessageWin.hight - 1, backcolor);
		GuiHLine(MessageWin.x+1,MessageWin.y+18,\
			MessageWin.x + MessageWin.wide - 1,forecolor);
		winHight =  (MessageWin.hight - 41 - 12)/2;
		GuiFont12Align(MessageWin.x+1,winHight + MessageWin.y +19,\
			MessageWin.wide - 1,FONT_MID,MessageIs.str);//��ʾ��
		winHight = MessageWin.y + MessageWin.hight - 23;
		GuiHPointLine(MessageWin.x+1,winHight,\
			MessageWin.x + MessageWin.wide - 1,2,forecolor);
		if(keyFlag == 0){
			winWide =  MessageWin.x + (MessageWin.wide/2 - 45)/2;
			GuiButton(winWide,winHight + 3,"ȷ��",0);
			winWide +=  MessageWin.wide/2;
			GuiButton(winWide,winHight + 3,"ȡ��",1);
		}
		else{
			winWide =  MessageWin.x + (MessageWin.wide/2 - 45)/2;
			GuiButton(winWide,winHight + 3,"ȷ��",1);
			winWide +=  MessageWin.wide/2;
			GuiButton(winWide,winHight + 3,"ȡ��",0);
		}
		GuiUpdateDisplayAll();
		stepTab[STEP_MESSAGE] = 2;
	}
	if(stepTab[STEP_MESSAGE] == 2){
	}
	switch(keyStatus){
	case LeftKey:
		keyFlag--;
		if(keyFlag < 0){
			keyFlag = 1;
		}
		stepTab[STEP_MESSAGE] = 1;break;
	case UpKey:break;
	case RightKey:
		keyFlag++;
		if(keyFlag > 1){
			keyFlag = 0;
		}
		stepTab[STEP_MESSAGE] = 1;break;
	case DownKey:break;
	case OkKey:
		*MessageIs.flag = keyFlag;
		stepTab[STEP_MESSAGE] = 0;
		userGUITopWindowHide();
		userGUITopWindowRedraw();
		break;
	case CancelKey:
		*MessageIs.flag = 0;
		stepTab[STEP_MESSAGE] = 0;
		userGUITopWindowHide();
		userGUITopWindowRedraw();
		break;
	default:break;
	}
}

/**
  *@brief  ���ö�ֵ�޸���ر���
  *@param  info ��ֵ�ṹ��ָ��
  *@param  flag ��־�ֽڣ�δʹ��
  *@param  itemIs ��ֵ��ǰItem
  *@param  str Ҫ�޸��ַ�����ָ��
  *@retval None
  */
static void SetDingZhiModfiy(const DzhiDisplayInfo *info,uint8_t flag,uint8_t itemIs,uint8_t *str)
{
	dZModfiy.info = info;
	dZModfiy.flag = flag;
	dZModfiy.itemIs = itemIs;
	dZModfiy.str = str;	
}

/**
  *@brief ��ֵ�޸ĳ�ʼ��
  *@param  None
  *@retval None
  */
void DZModfiyInit(void)
{
	int16_t x,y;
	float tRange;
	uint8_t tRangeStr[16];
	GuiRect(DZModfiyWin.x,DZModfiyWin.y,DZModfiyWin.x + DZModfiyWin.wide, \
		DZModfiyWin.y + DZModfiyWin.hight, forecolor);
	GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + 17,150,FONT_LEFT,\
		(uint8_t *)dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].pName);
	GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + 30,\
		DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);
	GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + DZModfiyWin.hight - 16,\
		DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);//״̬��
	GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + DZModfiyWin.hight - 14,80,FONT_LEFT,"��ǰ״̬:");
	
	if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType){//����
		x = DZModfiyWin.x + DZModfiyWin.wide;
		y = DZModfiyWin.y + 25 + (DZModfiyWin.hight - 43)/2 - 8;
		GuiFillRect(DZModfiyWin.x+9,y,x - 10,y + 17, backcolor);
		GuiFillRect(DZModfiyWin.x+17,y + 2,x - 20,y + 15, forecolor);
		GuiFont12Align(DZModfiyWin.x+7,y+4,10,FONT_LEFT,"<");
		GuiFont12Align(x - 14,y+4,10,FONT_LEFT,">");
	}
	else{//float
		GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + 32,40,FONT_LEFT,"���ֵ");
		tRange = dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].valMax;
		sprintf((char *)tRangeStr,"%.3f",tRange);
		tRangeStr[15] = '\0';
		GuiFont12Align(DZModfiyWin.x + 50,DZModfiyWin.y + 32,80,FONT_MID,tRangeStr);
		GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + 45,\
			DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);
		GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + 47,40,FONT_LEFT,"��Сֵ");
		tRange = dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].valMin;
		sprintf((char *)tRangeStr,"%.3f",tRange);
		tRangeStr[15] = '\0';
		GuiFont12Align(DZModfiyWin.x + 50,DZModfiyWin.y + 47,80,FONT_MID,tRangeStr);
		GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + 60,\
			DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);
		GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + 62,40,FONT_LEFT,"��ǰֵ");
		GuiFillRect(DZModfiyWin.x + 50,DZModfiyWin.y + 62,\
			DZModfiyWin.x + 128,DZModfiyWin.y + 74, backcolor);
		GuiFont12Align(DZModfiyWin.x + 50,\
			DZModfiyWin.y + 62,80,FONT_MID,dZModfiy.str);
		GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + 75,\
			DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);
		GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + 77,40,FONT_LEFT,"�޸�Ϊ");
		GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + 90,\
			DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);
		GuiRPointLine(DZModfiyWin.x + 47,DZModfiyWin.y + 30,\
			DZModfiyWin.y + 90,2,forecolor);	
	}
}

/**
  *@brief ����ֵ�޸İ���
  *@param  None
  *@retval None
  */
void DrawModfiyKey(uint8_t key)
{
	uint8_t i;
	int16_t x,y;
	for(i=0;i<14;i++){
		if(i < 7){
			x = DZModfiyWin.x + i*18;
			y = DZModfiyWin.y + 92;
		}
		else{
			x = DZModfiyWin.x + (i - 7)*18;
			y = DZModfiyWin.y + 104;
		}
		if(i == key){
			GuiFillRect(x,y,x + 18,y + 11, forecolor);
			GuiExchangeColor();
			GuiFont12Align(x+1,y,18,FONT_MID,(uint8_t *)modfiyKey[i]);
			GuiExchangeColor();
		}
		else{
			GuiFillRect(x,y,x + 18,y + 11, backcolor);
			GuiFont12Align(x+1,y,18,FONT_MID,(uint8_t *)modfiyKey[i]);
		}
		GuiRLine(x,y,y + 11,forecolor);
	}
	GuiHLine(DZModfiyWin.x,DZModfiyWin.y + 92,\
		DZModfiyWin.x + DZModfiyWin.wide,forecolor);
	GuiHLine(DZModfiyWin.x,DZModfiyWin.y + 104,\
		DZModfiyWin.x + DZModfiyWin.wide,forecolor);
	GuiHLine(DZModfiyWin.x,DZModfiyWin.y + 116,\
		DZModfiyWin.x + DZModfiyWin.wide,forecolor);
	GuiUpdateDisplayAll();
}

/**
  *@brief ��ֵ�޸Ĵ��ں���
  *@param  None
  *@retval None
  */
void DZModfiyFun(void *param)
{
	static uint8_t modfiyStr[16];//�޸��ݴ�buff
	static uint8_t modfiyStrP = 0;
	static uint8_t keyIs = 0;//������¼
	static uint32_t inputCursorTick;//�����˸��ʱ
	int16_t x,y;
	float fValue;
	if(stepTab[STEP_DZHIMODFIY] == 0){//��ʼ��
		DZModfiyInit();
		if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType){
			keyIs = dZModfiy.str[0];
		}
		else{
			keyIs = 0;
		}
		modfiyStrP = 0;
		modfiyStr[0] = ' ';
		modfiyStr[1] = '\0';
		stepTab[STEP_DZHIMODFIY] = 1;
	}
	if(stepTab[STEP_DZHIMODFIY] == 1){//��ʾ
		if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType){//���ִ���
			GuiExchangeColor();
			x = DZModfiyWin.x + DZModfiyWin.wide;
			y = DZModfiyWin.y + 25 + (DZModfiyWin.hight - 43)/2 - 8;
			GuiFillRect(DZModfiyWin.x+18,y + 3,x - 20,y + 15, backcolor);
			GuiFont12Align(DZModfiyWin.x+18,y + 3,x - DZModfiyWin.x - 40,FONT_MID,\
				(uint8_t *)dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].pContent[keyIs]);
			GuiExchangeColor();
			stepTab[STEP_DZHIMODFIY] = 3;
		}
		else{//float����
			DrawModfiyKey(keyIs);
			GuiFillRect(DZModfiyWin.x + 50,DZModfiyWin.y + 77,\
				DZModfiyWin.x + 128,DZModfiyWin.y + 89, backcolor);
			GuiFont12Align(DZModfiyWin.x + 50,\
				DZModfiyWin.y + 77,78,FONT_MID,modfiyStr);
			stepTab[STEP_DZHIMODFIY] = 2;
		}
		GuiUpdateDisplayAll();
	}
	if(stepTab[STEP_DZHIMODFIY] == 2){//�����������
		if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType == 0){
			if(GetIntervalTick(inputCursorTick) < 1000){
				if(modfiyStr[modfiyStrP] != '|'){
					modfiyStr[modfiyStrP] = '|';
					modfiyStr[modfiyStrP + 1] = '\0';
					GuiFillRect(DZModfiyWin.x + 50,DZModfiyWin.y + 77,\
						DZModfiyWin.x + 128,DZModfiyWin.y + 89, backcolor);
					GuiFont12Align(DZModfiyWin.x + 50,DZModfiyWin.y + 77,78,FONT_MID,modfiyStr);
					GuiUpdateDisplayAll();
				}
			}
			else if(GetIntervalTick(inputCursorTick) < 2000){
				if(modfiyStr[modfiyStrP] != ' '){
					modfiyStr[modfiyStrP] = ' ';
					modfiyStr[modfiyStrP + 1] = '\0';
					GuiFillRect(DZModfiyWin.x + 50,DZModfiyWin.y + 77,\
						DZModfiyWin.x + 128,DZModfiyWin.y + 89, backcolor);
					GuiFont12Align(DZModfiyWin.x + 50,DZModfiyWin.y + 77,78,FONT_MID,modfiyStr);
					GuiUpdateDisplayAll();
				}
			}
			else{
				inputCursorTick = getCurrentTick();
			}				
		}
	}
	if(stepTab[STEP_DZHIMODFIY] == 3){//�ȴ�
	}
	if(stepTab[STEP_DZHIMODFIY] == 4){//��������
		if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType){//����		
			dZModfiy.str[0] = modfiyStr[0];
			*(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].pVal) = (float)modfiyStr[0];
			dZModfiy.info->SaveModify(0);
			stepTab[STEP_DZHIMODFIY] = 5;
		}
		else{
			modfiyStr[modfiyStrP] = '\0';
			fValue = atof((const char *)modfiyStr);
			if(fValue <= dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].valMax &&\
				fValue >= dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].valMin){
				*(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].pVal) = fValue;
				strcpy((char *)dZModfiy.str,(char *)modfiyStr);
				dZModfiy.info->SaveModify(0);
				stepTab[STEP_DZHIMODFIY] = 5;
			}
			else{//������Χ
				GuiFillRect(DZModfiyWin.x + 57,DZModfiyWin.y + DZModfiyWin.hight - 14,\
					DZModfiyWin.x + DZModfiyWin.wide - 2,DZModfiyWin.y + DZModfiyWin.hight - 2, backcolor);
				GuiFont12Align(DZModfiyWin.x + 57,DZModfiyWin.y + DZModfiyWin.hight - 14,70,FONT_RIGHT,"������Χ");
				modfiyStr[modfiyStrP] = ' ';
				modfiyStr[modfiyStrP + 1] = '\0';
				stepTab[STEP_DZHIMODFIY] = 2;
			}
		}
	}
	if(stepTab[STEP_DZHIMODFIY] == 5){//�������ȴ��ظ�
		stepTab[STEP_DZHIMODFIY] = 6;
	}
	if(stepTab[STEP_DZHIMODFIY] == 6){
		stepTab[STEP_DZHIMODFIY] = 0;
		userGUITopWindowHide();
	}
	if(keyStatus != CancelKey && stepTab[STEP_DZHIMODFIY] == 0){
		keyStatus = NoKey;
	}
	switch(keyStatus){
	case UpKey:
		if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType == 0){
			if(keyIs < 7)
				keyIs += 7;
			else
				keyIs -= 7;
		}
		stepTab[STEP_DZHIMODFIY] = 1;break;
	case DownKey:
		if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType == 0){
			if(keyIs >= 7)
				keyIs -= 7;
			else
				keyIs += 7;
		}
		stepTab[STEP_DZHIMODFIY] = 1;break;
	case LeftKey:
		if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType){
			if(keyIs == 0)
				keyIs = dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType - 1;
			else
				keyIs --; 
		}
		else{
			if(keyIs == 0 || keyIs == 7){	
				keyIs += 6;
			}
			else{
				keyIs --;
			}
		}
		stepTab[STEP_DZHIMODFIY] = 1;break;
	case RightKey:
		if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType){
			if(keyIs == (dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType - 1))
				keyIs = 0;
			else
				keyIs ++; 
		}
		else{
			if(keyIs == 6 || keyIs == 13)
				keyIs -= 6;
			else
				keyIs ++;
		}
		stepTab[STEP_DZHIMODFIY] = 1;break;
	case OkKey:
		if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType){//�����
			modfiyStr[0] = keyIs;
			stepTab[STEP_DZHIMODFIY] = 4;
		}
		else{
			if(keyIs < 12 && keyIs != 6){
				if(modfiyStrP < 12){
					modfiyStr[modfiyStrP] = *modfiyKey[keyIs];
					modfiyStrP ++;
				}
			}
			else if(keyIs == 6){//esc
				modfiyStr[1] = '\0';
				modfiyStrP = 0;
				keyIs = 0;
				stepTab[STEP_DZHIMODFIY] = 0;
				userGUITopWindowHide();
			}
			else if(keyIs == 12){
				if(modfiyStrP > 0){
					modfiyStr[modfiyStrP] = '\0';
					modfiyStrP --;
				}			
			}
			else if(keyIs == 13){//ok
				stepTab[STEP_DZHIMODFIY] = 4;//�����
			}
		}break;
	case CancelKey:
		stepTab[STEP_DZHIMODFIY] = 0;
		userGUITopWindowHide();
		break;
	default: break;
	}
}


/**
  *@brief ���ƶ�ֵ�����
  *@param  pText:�������ָ�롣
  *@param  currentRow:���浱ǰ�����С�
  *@retval None
  */
static void DisplayDZ(struct DZItemsManage *dz_items_manage)
{		
	uint8_t drawx = 0;
	uint8_t drawy = 18;										/* ��ֵ���ʼ���� */
	
	uint8_t name_disp_len = 158;							/* ��ֵ����һ����ʾ����󳤶� */
	uint8_t value_disp_len = 158 / 2;						/* ��Ӧ��ֵ��ʾ����󳤶� */
	uint8_t unit_disp_len = name_disp_len - value_disp_len;	/* ��λ��ʾ����󳤶� */
	
	dz_items_manage->start_itemes = dz_items_manage->current_items / DZDISPLAYROW * DZDISPLAYROW;
	/* ���� */
	GuiFillRect(drawx + 1, drawy, GUIXMAX - 1, GUIYMAX - 1,backcolor);
	
	for(uint8_t i = dz_items_manage->start_itemes; i < dz_items_manage->items_num; ++i){
		GuiHLine(drawx, drawy, GUIXMAX - 1, forecolor);
		if(dz_items_manage->current_items == i){	/* ѡ����Ŀ���� */
			GuiFillRect(drawx, drawy, GUIYMAX - 1, drawy + 2*LIST_CONTENT_ROWHIGHT, forecolor);
			GuiExchangeColor();
		}
		/* д��ֵ�� */
		GuiFont12Align(drawx + 2,drawy + 1, name_disp_len, FONT_MID, *(dz_items_manage->pText + i * DZDISPLAYCOL + 0));
		drawy += LIST_CONTENT_ROWHIGHT;
		GuiHPointLine(drawx, drawy, GUIXMAX - 1, 2, forecolor);
		/* д��ֵ */
		GuiFont12Align(drawx + 2, drawy + 1, value_disp_len, FONT_MID, *(dz_items_manage->pText + i * DZDISPLAYCOL + 1));
		drawx += value_disp_len;
		GuiRPointLine(drawx, drawy, drawy + LIST_CONTENT_ROWHIGHT, 2, forecolor);
		/* д��λ */
		GuiFont12Align(drawx + 2, drawy + 1, unit_disp_len, FONT_MID, *(dz_items_manage->pText + i * DZDISPLAYCOL + 2));
		if(dz_items_manage->current_items == i){
			GuiExchangeColor();
		}
		
		drawx = 0;
		drawy += LIST_CONTENT_ROWHIGHT;
		GuiHLine(drawx, drawy, GUIXMAX - 1, forecolor);
		if(drawy > GUIYMAX - 1)
			break;
	}
	GuiHLine(0, 159, GUIXMAX - 1, forecolor);
}

/**
  *@brief ͨ�ö�ֵ��ʾ����
  *@param  Items ��ֵ�ṹָ��
  *@param  itemsNum ��ֵ��
  *@param  cmd ����
  *@param  flag �����־
  *@retval None
  */
void DZModfiyDisplay(DzhiDisplayInfo *info,uint8_t *flag)
{
	static SCROLL *Scroll;//������
	static uint8_t **pText;//�б�����ָ��
	static uint8_t *col1Data;//��ʾ����ָ��
	static struct DZItemsManage dz_items_manage;
	uint8_t i,itemsNum;
	uint16_t j ;
	uint16_t memMall;//�����ڴ����
	float tempFloat;
	
	if(*flag == 0){//��ʼ��
		if(info->num == 0){//���û��Item,ֱ���˳�
			*flag = 0;
			userGUITopWindowHide();
			userGUITopWindowRedraw();
			GuiMenuRedrawMenu(&HmiMenu);
			return;
		}
		itemsNum = info->num;
		memMall = 0;
		Scroll = (SCROLL *)&userGUIBuff[memMall];
		memMall += sizeof(SCROLL) + (4 - sizeof(SCROLL)%4);
		pText = (uint8_t **)&userGUIBuff[memMall];
		memMall += itemsNum*DZDISPLAYCOL*4;
		col1Data = &userGUIBuff[memMall];
		Scroll->x = 156;
		Scroll->y = 18;
		Scroll->hight = 141;
		Scroll->max = itemsNum;
		Scroll->lump = 1;
		*flag = 1;
		for(j=0;j<itemsNum*16;j++){
			col1Data[j] = 0;
		}
		dz_items_manage.items_num = itemsNum;
		dz_items_manage.pText = pText;
		dz_items_manage.start_itemes = 0;
		dz_items_manage.current_items = 0;
	}
	if(*flag == 1){
		itemsNum = info->num;
		for(i=0;i<itemsNum;i++){
			*(pText + i*DZDISPLAYCOL + 0) = (uint8_t *)info->pRoot[info->pBuff[i]].pName;
			if(info->pRoot[info->pBuff[i]].dataType){//����
				tempFloat = *(info->pRoot[info->pBuff[i]].pVal);
				if((uint32_t)tempFloat >= info->pRoot[info->pBuff[i]].dataType){
					tempFloat = 0;
				}
				col1Data[i*16] = tempFloat;
				*(pText + i*DZDISPLAYCOL + 1) = (uint8_t *)info->pRoot[info->pBuff[i]].pContent[(uint8_t)tempFloat];
			}
			else{//float value
				tempFloat = *(info->pRoot[info->pBuff[i]].pVal);
				char tspStr[8];
				sprintf(tspStr,"%%.%df",info->pRoot[info->pBuff[i]].decimals);
				sprintf((char *)&col1Data[i*16],tspStr,tempFloat);
				col1Data[i*16 + 15] = '\0';
				*(pText + i*DZDISPLAYCOL + 1) = &col1Data[i*16];
			}
			*(pText + i*DZDISPLAYCOL + 2) = (uint8_t *)info->pRoot[info->pBuff[i]].pUnit;
		}
		*flag = 2;
	}
	if(*flag == 2){//ָ���������ָ��
		
		DisplayDZ(&dz_items_manage);
		Scroll->lump = dz_items_manage.current_items + 1;
		GuiVScroll(Scroll);
		GuiUpdateDisplayAll();
		*flag = 3;
	}
	if(keyStatus != CancelKey && *flag <= 1){
		keyStatus = NoKey;
	}
	switch(keyStatus){
	case UpKey:
		if(dz_items_manage.current_items > 0){
			dz_items_manage.current_items --;
		}
		*flag = 2;break;
	case DownKey:
		if(dz_items_manage.current_items < dz_items_manage.items_num - 1){
			dz_items_manage.current_items ++;
		}
		*flag = 2;break;
	case LeftKey:
		if(dz_items_manage.current_items >= DZDISPLAYROW){
			dz_items_manage.current_items -= DZDISPLAYROW;
		}
		else{
			dz_items_manage.current_items =0;
		}
		*flag = 2;break;
	case RightKey:
		if(dz_items_manage.current_items + DZDISPLAYROW < dz_items_manage.items_num){
			dz_items_manage.current_items += DZDISPLAYROW;
		}
		else{
			dz_items_manage.current_items = dz_items_manage.items_num - 1;
		}
		*flag = 2;break;
	case OkKey:
		if(GetDingZhiSetOrSee() == SET_DINGZHI){//��ֵ�޸�
			SetDingZhiModfiy(info,1,dz_items_manage.current_items,&col1Data[dz_items_manage.current_items*16]);
			userGUIWindowAdd(&DZModfiyWin);
			*flag = 1;
		}break;
	case CancelKey:
		*flag = 0;
		userGUITopWindowHide();
		userGUITopWindowRedraw();
		GuiMenuRedrawMenu(&HmiMenu);
		break;
	default:break;
	}
}	

/**
  *@brief HomeWindow deal with function
  *@param  None
  *@retval None
  */
void HomeWindowFun(void *param)
{
	static SystemTimeDisplay *disTime;
	static uint8_t *strTime;
	static uint32_t homeTick;
	static uint8_t tClose;
	uint8_t i,j;
	uint16_t memMall;
	if(stepTab[STEP_HOMEWIN] == 0){//������ҳ
		memMall = 0;
		strTime = &userGUIBuff[memMall];
		memMall += 32;
		disTime = (SystemTimeDisplay *)&userGUIBuff[memMall];
		
		lkdBitmap tImage;
		tImage.number = 1;
		tImage.wide = 160;
		tImage.high = 30;
		tImage.beginx = 0;
		tImage.beginy = 0;
		tImage.bitmap = NULL;
		GuiAreaBitMap(&tImage,HomeWindow.x+1,HomeWindow.y+25, 159, 55, 1);
		j = 110;
		for(i=1;i<24;i+=2){
			GuiHPointLine(HomeWindow.x+1,i,j,2,forecolor);
			j -= 2;
		}
		for(i=25;i<55;i+=2){
			GuiHPointLine(HomeWindow.x+1,i,159,2,forecolor);
		}
		j = 70;
		for(i=55;i<78;i+=2){
			GuiHPointLine(HomeWindow.x+j,i,159,2,forecolor);
			j -= 2;
		}
		GuiHLine(HomeWindow.x+1,78,158,forecolor);
		GuiFillRect(HomeWindow.x+1,81,159,94, forecolor);
		GuiHLine(HomeWindow.x+1,96,158,forecolor);
		
		GuiFont12Align(HomeWindow.x+9,102,16,FONT_LEFT,"��");
		GuiFont12Align(HomeWindow.x+9,121,16,FONT_LEFT,"Ϣ");
		GuiFont12Align(HomeWindow.x+9,140,16,FONT_LEFT,"��");
		GuiRLine(HomeWindow.x+30,96,158,forecolor);
		
		for(i=0;i<4;i++){
			GuiFont12Align(HomeWindow.x+31,96+2+i*16,90,FONT_MID,"----");
			GuiRPointLine(HomeWindow.x+31+91,96,96+15+i*16,2,forecolor);
			GuiFont12Align(HomeWindow.x+31+93,96+2+i*16,34,FONT_MID,"--");
			GuiHPointLine(HomeWindow.x+30,96+15+i*16,159,2,forecolor);
		}
		
		GuiFont12Align(HomeWindow.x+31,96+34,90,FONT_MID,"����״̬");
		stepTab[STEP_HOMEWIN] = 1;
	}
	if(stepTab[STEP_HOMEWIN] == 1){
		GuiExchangeColor();
		GetDisplayTime(disTime);
		sprintf((char *)strTime,"20%02d-%02d-%02d   %02d:%02d:%02d",disTime->year,\
			disTime->month,disTime->day,disTime->hour,disTime->min,disTime->s);
		GuiFont12Align(HomeWindow.x+2,82,156,FONT_MID,strTime);
		GuiExchangeColor();
		tClose = *(yxInfo.pRoot[1].pVal);
		if(tClose == 2){
			GuiFont12Align(HomeWindow.x+31+93,96+34,34,FONT_MID,"��");
		}
		else{
			GuiFont12Align(HomeWindow.x+31+93,96+34,34,FONT_MID,"��");
		}
		GuiUpdateDisplayAll();
		stepTab[STEP_HOMEWIN] = 2;
		homeTick = getCurrentTick();
	}
	if(stepTab[STEP_HOMEWIN] == 2){
		if(GetIntervalTick(homeTick) > 1000 || tClose != *(yxInfo.pRoot[1].pVal)){
			stepTab[STEP_HOMEWIN] = 1;
		}
	}
	switch(keyStatus){
	case UpKey:
	case DownKey:
		stepTab[STEP_HOMEWIN] = 0;
		userGUIWindowAdd(&YaoCe2Win);
		break;
	case LeftKey:
	case RightKey:
		stepTab[STEP_HOMEWIN] = 0;
		userGUIWindowAdd(&YaoxinWin);
		break;
	case OkKey:
		stepTab[STEP_HOMEWIN] = 0;
		userGUIWindowAdd(&MenuWindow);
		break;
	case CancelKey:break;
	default: break;
	}
}

//

/**
  *@brief CmdSend deal with function �����·�
  *@param  None
  *@retval None
  */
static void CmdSendWinFun(void *param)
{
	const  uint8_t listCol[2][1] = {{156},{FONT_MID}};
	static SCROLL *Scroll;//������
	static ListColSet colset;
	static LIST  *list;//�б�ؼ�
	static uint8_t **pText;//�б�����ָ��
	static uint8_t messageFlag = 0;//�����־
	static int8_t currentNum;
	uint16_t memMall;
	uint8_t i;
	if(stepTab[STEP_NORMAL] == 0){//��ʼ���������ڴ�
		memMall = 0;	
		list = (LIST  *)&userGUIBuff[memMall];
		memMall += sizeof(LIST) + (4 - sizeof(LIST)%4);
		Scroll = (SCROLL *)&userGUIBuff[memMall];
		memMall += sizeof(SCROLL) + (4 - sizeof(SCROLL)%4);
		pText = (uint8_t **)&userGUIBuff[memMall];
		Scroll->x = 156;
		Scroll->y = 18;
		Scroll->hight = 141;
		Scroll->max = hcmdInfo.itemsNum;
		Scroll->lump = 1;
		colset.colWide = (uint8_t *)listCol[0];
		colset.colFlag = (uint8_t *)listCol[1];
		list->x = 0;
		list->y = 18;
		list->wide = 156;
		list->hight = 141;
		list->row = hcmdInfo.itemsNum;
		list->col = 1;
		list->drawRow = 0;
		list->currentRow = 0;
		list->flag = LIST_USEBORDER_H;
		list->content = (uint8_t **)pText;
		list->colSet = &colset;	
		currentNum = 0;
		stepTab[STEP_NORMAL] = 1;
	}
	if(stepTab[STEP_NORMAL] == 1){//��ȡ��Ӧ����	
		for(i = 0;i < hcmdInfo.itemsNum;i++){
			*(pText + i) = hcmdInfo.pHmiCmd[i].name;
		}
		stepTab[STEP_NORMAL] = 2;
	}
	if(stepTab[STEP_NORMAL] == 2){//��ʾ
		list->currentRow = currentNum;
		Scroll->lump = currentNum + 1;
		if((list->currentRow /DISPLAYLISTROW)*DISPLAYLISTROW + DISPLAYLISTROW <= list->row){
			list->drawRow = (list->currentRow /DISPLAYLISTROW)*DISPLAYLISTROW;
		}
		else{
			if(list->row < DISPLAYLISTROW){
				list->drawRow = 0;
			}
			else{
				list->drawRow = list->row - DISPLAYLISTROW;
			}	
		}
		DrawList(list);
		GuiVScroll(Scroll);
		GuiUpdateDisplayAll();
		stepTab[STEP_NORMAL] = 3;
	}
	if(stepTab[STEP_NORMAL] == 3){
	}
	if(stepTab[STEP_NORMAL] == 4){
		if(messageFlag == 1){
			hcmdInfo.cmdfun(hcmdInfo.pHmiCmd[currentNum].cmd);
		}
		stepTab[STEP_NORMAL] = 2;
	}
	switch(keyStatus){
	case LeftKey:
	case UpKey:
		currentNum --;
		if(currentNum < 0){
			currentNum = Scroll->max - 1;
		}
		stepTab[STEP_NORMAL] = 2;
		break;
	case RightKey:
	case DownKey:
		currentNum ++;
		if(currentNum > Scroll->max - 1){
			currentNum = 0;
		}
		stepTab[STEP_NORMAL] = 2;
		break;
	case OkKey:
		stepTab[STEP_NORMAL] = 4;
		messageFlag = 0;
		SetMessageWin(23,40,"ȷ��ִ��?",&messageFlag);
		userGUIWindowAdd(&MessageWin);
		break;
	case CancelKey:
		stepTab[STEP_NORMAL] = 0;
		userGUITopWindowHide();
		userGUITopWindowRedraw();
		GuiMenuRedrawMenu(&HmiMenu);
		break;
	default:break;
	}
}

/**
  *@brief  �汾��Ϣ���ݴ���
  *@param  None
  *@retval None
  */
static void ConstParDataResult(const uint8_t *pData,uint8_t *outData,uint8_t len)
{
	int8_t j;
	strncpy((char *)outData,(char *)pData,len);
	for(j = len - 1; j >= 0 ; j--){
		if(outData[j] == ' '){
			outData[j] = '\0';
		}
		else{
			outData[j + 1] = '\0';
			break;
		}
	}
}
/**
  *@brief  �汾��Ϣ
  *@param  None
  *@retval None
  */
static void VersionWinFun(void *param)
{
	static SCROLL *Scroll;//������
	static uint8_t *col1Data;//��ʾ����ָ��		
	static int8_t currentNum;
	uint16_t memMall;
	uint8_t i,y;
	if(stepTab[STEP_NORMAL] == 0){//��ʼ���������ڴ�
		memMall = 0;
		col1Data = &userGUIBuff[memMall];
		memMall += 28;
		Scroll = (SCROLL *)&userGUIBuff[memMall];	
		Scroll->x = 156;
		Scroll->y = 18;
		Scroll->hight = 141;
		Scroll->max = versInfo.num / CONSTPARDISPLAYROW +\
			(versInfo.num % CONSTPARDISPLAYROW > 0 ? 1 : 0);
		Scroll->lump = 1;
		currentNum = 0;
		stepTab[STEP_NORMAL] = 1;
	}
	if(stepTab[STEP_NORMAL] == 1){//��ȡ��Ӧ����
		stepTab[STEP_NORMAL] = 2;
	}
	if(stepTab[STEP_NORMAL] == 2){//��ʾ
		y = VersionWin.y + 18;
		GuiFillRect(VersionWin.x+1,y,155,158, backcolor);
		for(i = 0; i < CONSTPARDISPLAYROW; i++){
			if((i + currentNum * CONSTPARDISPLAYROW) >= versInfo.num){
				GuiHLine(VersionWin.x,y+i*28+0,155,forecolor);//ˮƽ��
				break;
			}
			GuiHLine(VersionWin.x,y+i*28+0,155,forecolor);//ˮƽ��		
			GuiFillRect(VersionWin.x+1,y + i*28+1,155,y + i*28+14, forecolor);
			GuiExchangeColor();
			GuiFont12Align(VersionWin.x + 2,y + i*28+2,133,FONT_LEFT,
				(uint8_t *)versInfo.pItems[i + currentNum * CONSTPARDISPLAYROW].pName);
			GuiExchangeColor();
			ConstParDataResult((uint8_t *)versInfo.pItems[i + currentNum * CONSTPARDISPLAYROW].pVal,col1Data,24);
			GuiFont12Align(VersionWin.x+2,y + i*28+15,153,FONT_MID,col1Data);
		}
		Scroll->lump = currentNum + 1;
		GuiVScroll(Scroll);
		GuiUpdateDisplayAll();
		stepTab[STEP_NORMAL] = 3;
	}
	if(stepTab[STEP_NORMAL] == 3){
	}
	if(keyStatus != CancelKey && stepTab[STEP_NORMAL] <= 1){
		keyStatus = NoKey;
	}
	switch(keyStatus){
	case LeftKey:
	case UpKey:
		currentNum --;
		if(currentNum < 0){
			currentNum = Scroll->max - 1;
		}
		stepTab[STEP_NORMAL] = 1;
		break;
	case RightKey:
	case DownKey:
		currentNum ++;
		if(currentNum > Scroll->max - 1){
			currentNum = 0;
		}
		stepTab[STEP_NORMAL] = 1;
		break;
	case OkKey:break;
	case CancelKey:
		stepTab[STEP_NORMAL] = 0;
		userGUITopWindowHide();
		userGUITopWindowRedraw();
		GuiMenuRedrawMenu(&HmiMenu);
		break;
	default:break;
	}
}

/**
  *@brief  �����޸ĺ���
  *@param  None
  *@retval None
  */
void PasswordModfiyWinFun(void *param)
{
	static uint8_t currentNum;
	static uint8_t savePassword[3][7];
	static uint8_t passWordStr[7];
	static int8_t SelectPasswordLevel;
	static uint8_t passwordStep;
	static uint8_t *hins;
	static uint8_t count = 1;
	uint8_t passwordSaveBuff[16] = {0};
	uint16_t memMall;
	uint8_t i = 0;
	uint8_t tempStr[2];
	
	if(stepTab[STEP_NORMAL] == 0){
		SelectPasswordLevel = 0;
		stepTab[STEP_NORMAL] = 1;
	}
	
	if(stepTab[STEP_NORMAL] == 1) {	/* ��ʼ�� */
		memset(userGUIBuff,0,sizeof(userGUIBuff)/sizeof(userGUIBuff[0]));
		memMall = 0;
		hins = (uint8_t*)&userGUIBuff[memMall];
		currentNum = 0;
		
		memset(passWordStr,'0',sizeof(passWordStr) / sizeof(passWordStr[0]));
		passwordStep = 0;
		GuiFillRect(1,20,158,158,backcolor);
		stepTab[STEP_NORMAL] = 2;
	}
	if(stepTab[STEP_NORMAL] == 2) {	/* ѡ���޸ļ������� */
		GuiFillRect(1,20,158,158,backcolor);
		hins[0] = SelectPasswordLevel + 1 + 48;
		strncpy((char*)hins + 1,"�������޸�",strlen("�������޸�") + 1);
		GuiFillRect(41,23,116,38,forecolor);
		GuiExchangeColor();
		GuiFont12Align(42,25,116 - 42,FONT_MID,hins);
		GuiExchangeColor();
		GuiFont12Align(22,25,12,FONT_MID,"<");
		GuiFont12Align(125,25,12,FONT_MID,">");
		
		GuiFont12Align(15,55,72,FONT_LEFT,"ԭʼ����:");
		GuiRect(90,53,132,67,forecolor);
		GuiFont12Align(15,75,72,FONT_LEFT,"������:");
		GuiRect(90,73,132,87,forecolor);
		GuiFont12Align(15,95,72,FONT_LEFT,"ȷ��������:");
		GuiRect(90,93,132,107,forecolor);
		
		GuiUpdateDisplayAll();
	}
	if(stepTab[STEP_NORMAL] == 3) {	/* �޸����� */
		for(i = 0;i < 6;++i){
			if(i == currentNum){
				tempStr[0] = passWordStr[currentNum];
				tempStr[1] = '\0';
				savePassword[passwordStep][currentNum] = passWordStr[currentNum];
				savePassword[passwordStep][currentNum+1] = '\0';
				GuiExchangeColor();
				GuiFont12Align(93+currentNum*6,53+passwordStep*20,6,FONT_MID,tempStr);
				GuiExchangeColor();
			}
			else{
				GuiFont12Align(93+i*6,53+passwordStep*20,6,FONT_MID,"*");
			}
			if(passwordStep > 0)
				GuiFont12Align(93+i*6,53+(passwordStep-1)*20,6,FONT_MID,"*");
		}
		GuiUpdateDisplayAll();
	}
	if(stepTab[STEP_NORMAL] == 4) {	/* ȷ���޸����� */
		if(strncmp((const char*)savePassword[0],(const char*)PassWordItems[SelectPasswordLevel].passWord,6) == 0 || strncmp((const char*)savePassword[0],MANUFACTURER_PASSWORD,6) == 0) {	/* ԭ������ȷ */
			if(strncmp((const char*)savePassword[1],(const char*)savePassword[2],6) == 0)	{	/* ����������һ�� */
				strncpy((char*)passwordSave.password[SelectPasswordLevel],(const char*)savePassword[1],6);
				strncpy((char*)PassWordItems[SelectPasswordLevel].passWord,(const char*)savePassword[1],6);
				/* �洢��Flash�� */
				strncpy((char*)passwordSaveBuff,(const char*)passwordSave.password[0],6);
				strncpy((char*)passwordSaveBuff + 6,(const char*)passwordSave.password[1],6);
				strncpy((char*)passwordSaveBuff + 12,"0000",4);
				rt_multi_common_data_fram_record_write(PASSPHRASE, passwordSaveBuff, 16);
				
				strcpy((char*)hins,"�޸ĳɹ�");
				stepTab[STEP_NORMAL] = 5;
			}
			else {
				strcpy((char*)hins,"�������벻һ��");
			}
		}
		else {
			strcpy((char*)hins,"ԭ�������");
		}
		GuiFillRect(20,20,140,140,backcolor);
		GuiRect(20,20,140,140,forecolor);
		GuiFont12Align(20,80,120,FONT_MID,hins);
		GuiUpdateDisplayAll();
	}
	
	switch(keyStatus){
	case UpKey:
		if(stepTab[STEP_NORMAL] == 3) {
			if(passWordStr[currentNum] < '9')
				passWordStr[currentNum]++;
		}
		break;	
	case DownKey:	
		if(stepTab[STEP_NORMAL] == 3) {
			if(passWordStr[currentNum] > '0')
				passWordStr[currentNum]--;
		}
		break;
	case LeftKey:
		if(stepTab[STEP_NORMAL] == 2) {
			if(SelectPasswordLevel > 0)
				SelectPasswordLevel--;
		}
		if(stepTab[STEP_NORMAL] == 3) {
			if(currentNum > 0)
				currentNum--;
		}
		break;
	case RightKey:
		if(stepTab[STEP_NORMAL] == 2) {
			if(SelectPasswordLevel < MAX_PASSOWRD_LEVEL - 1)
				SelectPasswordLevel++;
		}
		if(stepTab[STEP_NORMAL] == 3) {
			count++;
			if(currentNum < 5)
				currentNum++;
		}
		break;
	case OkKey:
		if(stepTab[STEP_NORMAL] == 4 || stepTab[STEP_NORMAL] == 5) {
			stepTab[STEP_NORMAL] = 1;
		}
		else if(stepTab[STEP_NORMAL] == 3) {
			for(i = count; i < 6;++i)
				savePassword[passwordStep][i] = '0';
			count = 1;
			strncpy((char*)savePassword[passwordStep],(const char*)passWordStr,6);
			if(passwordStep == 2) {
				stepTab[STEP_NORMAL] = 4;
			}
			else {
				passwordStep++;
			}
			currentNum = 0;
			memset(passWordStr,'0',sizeof(passWordStr) / sizeof(passWordStr[0]));
		}
		else if(stepTab[STEP_NORMAL] == 2) {
			stepTab[STEP_NORMAL] = 3;
		}
		break;
	case CancelKey:
		if(stepTab[STEP_NORMAL] == 2 || stepTab[STEP_NORMAL] == 5) {
			stepTab[STEP_NORMAL] = 0;
			userGUITopWindowHide();
			userGUITopWindowRedraw();
			GuiMenuRedrawMenu(&HmiMenu);
		}
		if(stepTab[STEP_NORMAL] == 3 || stepTab[STEP_NORMAL] == 4) {
			stepTab[STEP_NORMAL] = 1;
		}
		break;
	default:break;
	}
}


/**
  *@brief ң�Ŵ�����
  *@param  None
  *@retval None
  */
static void YaoxinFun(void *param)
{
	const  uint8_t listCol[2][2] = {{90,66},{FONT_LEFT,FONT_MID}};
	static SCROLL *Scroll;//������
	static ListColSet colset;
	static LIST  *list;//�б�ؼ�
	static uint8_t **pText;//�б�����ָ��
	static uint8_t *col1Data;//��ʾ����ָ��	
	static uint32_t YaoXinTick;
	static int16_t currentNum;
	uint16_t memMall;
	uint8_t i;
	uint8_t itemsNum;
	
	if(stepTab[STEP_NORMAL] == 0){//��ʼ���������ڴ�
		itemsNum = yxInfo.Num;
		
		memMall = 0;
		list = (LIST  *)&userGUIBuff[memMall];
		memMall += sizeof(LIST) + (4 - sizeof(LIST)%4);
		Scroll = (SCROLL *)&userGUIBuff[memMall];
		memMall += sizeof(SCROLL) + (4 - sizeof(SCROLL)%4);
		pText = (uint8_t **)&userGUIBuff[memMall];
		memMall += itemsNum*2*4;
		col1Data = &userGUIBuff[memMall];

		Scroll->x = 156;
		Scroll->y = 18;
		Scroll->hight = 141;
		Scroll->max = itemsNum / DISPLAYLISTROW + (itemsNum % DISPLAYLISTROW > 0 ? 1:0);
		Scroll->lump = 1;
		colset.colWide = (uint8_t *)listCol[0];
		colset.colFlag = (uint8_t *)listCol[1];
		list->x = 0;
		list->y = 18;
		list->wide = 156;
		list->hight = 141;
		list->row = itemsNum;
		list->col = 2;
		list->drawRow = 0;
		list->currentRow = 0;
		list->flag = LIST_USEBORDER_H;
		list->content = (uint8_t **)pText;
		list->colSet = &colset;
		currentNum = 0;
		stepTab[STEP_NORMAL] = 1;
	}
	if(stepTab[STEP_NORMAL] == 1){//��ȡ��Ӧ����
		itemsNum = yxInfo.Num;
		for(i = 0;i < itemsNum;i++){
			col1Data[i] = (uint8_t )*(yxInfo.pRoot[yxInfo.pBuff[i]].pVal);
		}	
		for(i = 0;i < itemsNum;i++){
			*(pText + i*2 + 0) = (uint8_t *)yxInfo.pRoot[i].pName;
			if(col1Data[i] == 1)
				*(pText + i*2 + 1) = (uint8_t *)yxInfo.pRoot[yxInfo.pBuff[i]].pContentYx[0];
			else
				*(pText + i*2 + 1) = (uint8_t *)yxInfo.pRoot[yxInfo.pBuff[i]].pContentYx[1];
		}
		stepTab[STEP_NORMAL] = 2;
	}
	if(stepTab[STEP_NORMAL] == 2){//��ʾ
		list->drawRow = currentNum * DISPLAYLISTROW;
		list->currentRow = list->drawRow;
		if(currentNum * DISPLAYLISTROW + DISPLAYLISTROW > list->row){
			if(list->row - DISPLAYLISTROW > 0){
				list->drawRow = list->row - DISPLAYLISTROW;
				list->currentRow = list->drawRow + (currentNum + 1) * DISPLAYLISTROW - list->row;
			}
			else{
				list->currentRow = 0;
				list->drawRow = 0;
			}
		}
		Scroll->lump = currentNum + 1;
		DrawList(list);
		GuiVScroll(Scroll);
		GuiUpdateDisplayAll();
		YaoXinTick = getCurrentTick();
		stepTab[STEP_NORMAL] = 3;
	}
	if(stepTab[STEP_NORMAL] == 3){//���ң�ű仯
		if(GetIntervalTick(YaoXinTick) > 1000){
			stepTab[STEP_NORMAL] = 1;
		}
	}

	switch(keyStatus){
	case UpKey:
		currentNum --;
		if(currentNum < 0){
			currentNum = 0;
		}
		stepTab[STEP_NORMAL] = 2;
		break;	
	case DownKey:	
		currentNum ++;
		if(currentNum >= Scroll->max){
			currentNum = Scroll->max - 1;
		}
		stepTab[STEP_NORMAL] = 2;
		break;
	case LeftKey:break;
	case RightKey:break;
	case OkKey:break;
	case CancelKey:
		stepTab[STEP_NORMAL] = 0;
		userGUITopWindowHide();
		userGUITopWindowRedraw();
		GuiMenuRedrawMenu(&HmiMenu);
		break;
	default:break;
	}
}

/**
  *@brief ң�⴦����ʾ
  *@param  items ң����ʾ��
  *@param  itemsNum ң������
  *@param  items modbus����
  *@retval None
  */
void yaoCeDisplay(YaoceDisplayInfo *info)
{
	const uint8_t listCol[2][YaoCeDISPLAYCOL] = {//���������
		{50,75,31},{FONT_LEFT,FONT_MID,FONT_MID}};
	static SCROLL *Scroll;//������
	static ListColSet colset;
	static LIST  *list;//�б�ؼ�
	static uint8_t **pText;//�б�����ָ��
	static uint8_t *col1Data;//��ʾ����ָ��		
	static uint32_t YaoCeTick;//ѭ����ʱ
	static int16_t currentNum;
	uint16_t memMall;
	uint8_t i;
	uint8_t itemsNum = info->num;/* ����ʾ��Ŀ */
	float tempFloat;	
	if(stepTab[STEP_NORMAL] == 0){//��ʼ���������ڴ�
		memMall = 0;	
		list = (LIST  *)&userGUIBuff[memMall];
		memMall += sizeof(LIST) + (4 - sizeof(LIST)%4);
		Scroll = (SCROLL *)&userGUIBuff[memMall];
		memMall += sizeof(SCROLL) + (4 - sizeof(SCROLL)%4);
		pText = (uint8_t **)&userGUIBuff[memMall];
		memMall += itemsNum*YaoCeDISPLAYCOL*4;
		col1Data = &userGUIBuff[memMall];
		
		Scroll->x = 156;
		Scroll->y = 18;
		Scroll->hight = 141;
		Scroll->max = itemsNum / DISPLAYLISTROW + (itemsNum % DISPLAYLISTROW > 0 ? 1 : 0);
		Scroll->lump = 1;
		colset.colWide = (uint8_t *)listCol[0];
		colset.colFlag = (uint8_t *)listCol[1];
		list->x = 0;
		list->y = 18;
		list->wide = 156;
		list->hight = 141;
		list->row = itemsNum;
		list->col = YaoCeDISPLAYCOL;
		list->drawRow = 0;
		list->currentRow = 255;
		list->flag = LIST_USEBORDER_H;
		list->content = (uint8_t **)pText;
		list->colSet = &colset;
		currentNum = 0;
		stepTab[STEP_NORMAL] = 1;
	}
	if(stepTab[STEP_NORMAL] == 1){
		for(i = 0;i < itemsNum;i++){
			tempFloat = *(info->pRoot[info->pBuff[i]].pVal);
			if(((uint8_t)isnan(tempFloat)) != 0){
				sprintf((char *)&col1Data[i*16],"%s","NaN");
			}
			else{
				sprintf((char *)&col1Data[i*16],"%.3f",tempFloat);
			}
			col1Data[i*16 + 15] = '\0';
			*(pText + i*YaoCeDISPLAYCOL + 0) = (uint8_t *)info->pRoot[info->pBuff[i]].pName;
			*(pText + i*YaoCeDISPLAYCOL + 1) = &col1Data[i*16];
			*(pText + i*YaoCeDISPLAYCOL + 2) = (uint8_t *)info->pRoot[info->pBuff[i]].pUnit;
		}
		stepTab[STEP_NORMAL] = 2;
	}
	if(stepTab[STEP_NORMAL] == 2){//��ʾ
		list->drawRow = currentNum * DISPLAYLISTROW;
		list->currentRow = list->drawRow;
		if(currentNum * DISPLAYLISTROW + DISPLAYLISTROW > list->row){
			if(list->row - DISPLAYLISTROW > 0){
				list->drawRow = list->row - DISPLAYLISTROW;
				list->currentRow = list->drawRow + (currentNum + 1) * DISPLAYLISTROW - list->row;
			}
			else{
				list->currentRow = 0;
				list->drawRow = 0;
			}
		}
		Scroll->lump = currentNum + 1;
		DrawList(list);
		GuiVScroll(Scroll);
		GuiUpdateDisplayAll();
		YaoCeTick = getCurrentTick();
		stepTab[STEP_NORMAL] = 3;
	}
	if(stepTab[STEP_NORMAL] == 3){
		if(GetIntervalTick(YaoCeTick) > YAOCE_UPDATATIME){//������
			stepTab[STEP_NORMAL] = 1;
		}
	}
	switch(keyStatus){
	case UpKey:
		currentNum --;
		if(currentNum < 0){
			currentNum = 0;
		}
		stepTab[STEP_NORMAL] = 2;
		break;
	case DownKey:
		currentNum ++;
		if(currentNum >= Scroll->max){
			currentNum = Scroll->max - 1;
		}
		stepTab[STEP_NORMAL] = 2;
		break;
	case LeftKey:break;
	case RightKey:break;
	case OkKey:break;
	case CancelKey:
		stepTab[STEP_NORMAL] = 0;
		userGUITopWindowHide();
		userGUITopWindowRedraw();
		GuiMenuRedrawMenu(&HmiMenu);
		break;
	default:break;
	}
}

/**
  *@brief ң�⴦����
  *@param  None
  *@retval None
  */
void YaoceWinFun(void *param)
{
	yaoCeDisplay(param);
}

/**
  *@brief Coe/Co������
  *@param  pInfo Coe/Co��Ϣ�ṹ��
  *@retval None
  */
static void SoeCoDisplay(SoeDisplayInfo *pInfo)
{
	static struct SOEDisplay *soeStr;
	static SCROLL Scroll ={156,18,141,2,1};//������
	static uint8_t currentNum;
	SystemTimeDisplay *tTim;
	uint8_t tRes;
	uint8_t i;
	uint16_t y;
	
	if(stepTab[STEP_NORMAL] == 0){//��ʼ���������ڴ�
		soeStr = (struct SOEDisplay *)&userGUIBuff[0];
		soeStr->pRead = 0;
		soeStr->allNum = pInfo->GetNum();
		currentNum = 1;
		stepTab[STEP_NORMAL] = 1;
	}
	if(stepTab[STEP_NORMAL] == 1){
		if(soeStr->allNum == 0){//û��SOE
			GuiFont12Align(SOEWin.x+2,SOEWin.y + 40,SOEWin.x+SOEWin.wide-4,FONT_MID,"��ǰû����Ϣ��¼");
			GuiUpdateDisplayAll();
			stepTab[STEP_NORMAL] = 3;
		}
		else{
			if(soeStr->allNum % SOEDISPLAYROW){
				Scroll.max = soeStr->allNum / SOEDISPLAYROW + 1;
			}
			else{
				Scroll.max = soeStr->allNum / SOEDISPLAYROW;
			}
			stepTab[STEP_NORMAL] = 2;
		}
	}
	if(stepTab[STEP_NORMAL] == 2){
		soeStr->pRead = (currentNum - 1) * SOEDISPLAYROW;
		if(soeStr->pRead >= soeStr->allNum){//SOE���һ��
			stepTab[STEP_NORMAL] = 3;
		}
		y = SOEWin.y + 18;
		GuiFillRect(SOEWin.x+1,y,155,158, backcolor);
		for(i = 0; i < SOEDISPLAYROW; i ++){
			if(soeStr->pRead >= soeStr->allNum){//SOE���һ��
				GuiHLine(SOEWin.x,y+i*28,155,forecolor);
				break;
			}
			tRes = pInfo->GetNoContent(soeStr->pRead,&soeStr->pSoe);
			if(tRes != 0){//��ȡSOE�д�
				GuiHLine(SOEWin.x,y+i*28,155,forecolor);
				break;
			}
			tTim = &soeStr->pSoe.time;
			sprintf((char *)soeStr->time,"%02d/%02d/%02d-%02d:%02d:%02d.%03d",\
				tTim->year,tTim->month,tTim->day,tTim->hour,tTim->min,tTim->s,tTim->ms);
			soeStr->time[23] = '\0';
			sprintf((char *)soeStr->itemNum,"%d",soeStr->pRead + 1);
			soeStr->itemNum[3] = '\0';
			GuiHLine(SOEWin.x,y+i*28+0,155,forecolor);//ˮƽ��
			GuiFillRect(SOEWin.x+1,y+i*28,SOEWin.x+20,y+i*28+13, forecolor);
			GuiExchangeColor();
			GuiFont12Align(SOEWin.x+1,y + i*28+1,19,FONT_MID,soeStr->itemNum);//���
			GuiExchangeColor();	
			GuiRPointLine(SOEWin.x+20,y+i*28+1,y+i*28+13,2,forecolor);//��ֱ��
			GuiFont12Align(SOEWin.x + 21,y + i*28+1,133,FONT_RIGHT,soeStr->time);
			GuiHPointLine(SOEWin.x,y+i*28+13,155,2,forecolor);
			GuiFont12Align(SOEWin.x+2,y + i*28+15,72,FONT_MID,soeStr->pSoe.pName);
			GuiRPointLine(SOEWin.x+10+72,y+i*28+15,y+i*28+27,2,forecolor);
			if(soeStr->pSoe.highlight){//������ʾ
				GuiFillRect(SOEWin.x+10+73,y+i*28+14,158,y+i*28+28, forecolor);
				GuiExchangeColor();
				GuiFont12Align(SOEWin.x+10+73,y+i*28+15,72,FONT_RIGHT,soeStr->pSoe.pContent);
				GuiExchangeColor();
			}
			else{
				GuiFont12Align(SOEWin.x+10+73,y+i*28+15,72,FONT_RIGHT,soeStr->pSoe.pContent);
			}
			soeStr->pRead ++;
		}
		Scroll.lump = currentNum;
		GuiVScroll(&Scroll);
		GuiUpdateDisplayAll();
		stepTab[STEP_NORMAL] = 3;
	}
	if(stepTab[STEP_NORMAL] == 3){
		if(pInfo->CheckUpdata()){//Co�и���
			soeStr->allNum = pInfo->GetNum();
			soeStr->pRead = 0;
			currentNum = 1;
			stepTab[STEP_NORMAL] = 1;
		}
	}
	switch(keyStatus){
	case UpKey:
	case LeftKey:
		if(stepTab[STEP_NORMAL] > 1){
			currentNum --;
			if(currentNum < 1){
				currentNum = Scroll.max;
			}
		}
		stepTab[STEP_NORMAL] = 1;
		break;
	case DownKey:
	case RightKey:
		if(stepTab[STEP_NORMAL] > 1){
			currentNum ++;
			if(currentNum > Scroll.max){
				currentNum = 1;
			}
		}
		stepTab[STEP_NORMAL] = 1;
		break;
	case OkKey:break;
	case CancelKey:
		stepTab[STEP_NORMAL] = 0;
		userGUITopWindowHide();
		userGUITopWindowRedraw();
		GuiMenuRedrawMenu(&HmiMenu);
		break;
	default:break;
	}
}

/**
  *@brief Soe/CO������
  *@param  None
  *@retval None
  */
void SoeCoWinFun(void *param)
{
	SoeCoDisplay(param);
}
/**
  *@brief �����¼�������
  *@param  None
  *@retval None
  */
static void FaultEventFun(void *param)
{
	static struct EventDisplay *pEventStr;
	static SCROLL Scroll ={156,18,141,2,1};//������
	static SCROLL yaoCeScroll ={153,47,112,2,1};//������
	static int16_t currentNum;
	static int8_t yaoCeNum;
	uint8_t i;
	uint16_t y;
	
	if(stepTab[STEP_NORMAL] == 0){//��ʼ���������ڴ�
		pEventStr = (struct EventDisplay *)&userGUIBuff[0];
		pEventStr->pRead = 0;
		pEventStr->allNum = GetFeventNum();
		currentNum = 1;
		yaoCeNum = 1;
		stepTab[STEP_NORMAL] = 1;
	}
	if(stepTab[STEP_NORMAL] == 1){
		if(pEventStr->allNum == 0){//û�й����¼�
			GuiFont12Align(FaultEventWin.x+2,FaultEventWin.y + 50,\
			FaultEventWin.x+FaultEventWin.wide-4,FONT_MID,"��ǰû�й����¼�");
			GuiUpdateDisplayAll();
			stepTab[STEP_NORMAL] = 4;
		}
		else{
			pEventStr->pRead = currentNum - 1;
			GetFeventNoContent(pEventStr->pRead,&pEventStr->pFevent);
			Scroll.max = pEventStr->allNum;
			if(pEventStr->pFevent.yaoceNum <= 8){
				yaoCeScroll.max = 1;
			}
			else{
				yaoCeScroll.max = pEventStr->pFevent.yaoceNum - 8 + 1;
			}
			stepTab[STEP_NORMAL] = 2;
		}
	}
	if(stepTab[STEP_NORMAL] == 2){//��ͼ
		y = FaultEventWin.y + 18;
		GuiFillRect(FaultEventWin.x+1,y,155,158, backcolor);
		GuiHLine(FaultEventWin.x,y,155,forecolor);//ˮƽ��	
		GuiFillRect(FaultEventWin.x+1,y,FaultEventWin.x+20,y+13, forecolor);
		SystemTimeDisplay *tTim;
		tTim = &pEventStr->pFevent.time;
		sprintf((char *)pEventStr->time,"%02d/%02d/%02d-%02d:%02d:%02d.%03d",\
			tTim->year,tTim->month,tTim->day,tTim->hour,tTim->min,tTim->s,tTim->ms);
		pEventStr->time[23] = '\0';
		sprintf((char *)pEventStr->itemNum,"%d",pEventStr->pRead + 1);
		pEventStr->itemNum[3] = '\0';
		GuiExchangeColor();
		GuiFont12Align(FaultEventWin.x+1,y+1,20,FONT_MID,pEventStr->itemNum);//���
		GuiExchangeColor();	
		GuiRPointLine(FaultEventWin.x+20,y+1,y+13,2,forecolor);//��ֱ��
		GuiFont12Align(FaultEventWin.x + 21,y+1,133,FONT_RIGHT,pEventStr->time);
		GuiHPointLine(FaultEventWin.x,y+13,155,2,forecolor);
		GuiFont12Align(FaultEventWin.x+2,y+15,72,FONT_LEFT,pEventStr->pFevent.pName);
		GuiRPointLine(FaultEventWin.x+10+72,y+15,y+27,2,forecolor);
		GuiFont12Align(FaultEventWin.x+10+73,y+15,70,FONT_RIGHT,pEventStr->pFevent.pContent);	
		Scroll.lump = currentNum;
		GuiVScroll(&Scroll);
		GuiHLine(FaultEventWin.x,y+28,155,forecolor);//ˮƽ��	
		stepTab[STEP_NORMAL] = 3;
	}
	if(stepTab[STEP_NORMAL] == 3){
		y = FaultEventWin.y + 18 + 29;
		yaoCeScroll.lump = yaoCeNum;
		GuiVScroll(&yaoCeScroll);
		GuiFillRect(FaultEventWin.x+1,y,151,158, backcolor);
        uint8_t ycNum = 8;
        if(pEventStr->pFevent.yaoceNum < 8){
            ycNum = pEventStr->pFevent.yaoceNum;
        }
		for(i = 0;i < ycNum;i ++){
			GuiFont12Align(FaultEventWin.x+2,y+1+i*14,40,FONT_LEFT,\
				pEventStr->pFevent.pYaoceName[i + yaoCeNum - 1]);			
			GuiRPointLine(FaultEventWin.x+43,y+i*14,y+13+i*14,2,forecolor);
			sprintf((char *)pEventStr->yaoceValue,"%.3f",pEventStr->pFevent.pYaoceValue[i + yaoCeNum - 1]);
			pEventStr->yaoceValue[15] = '\0';
			GuiFont12Align(FaultEventWin.x+44,y+1+i*14,70,FONT_MID,pEventStr->yaoceValue);
			GuiRPointLine(FaultEventWin.x+115,y+i*14,y+13+i*14,2,forecolor);
			GuiFont12Align(FaultEventWin.x+116,y+1+i*14,35,FONT_RIGHT,\
				pEventStr->pFevent.pYaoceUnit[i + yaoCeNum - 1]);		
			GuiHPointLine(FaultEventWin.x,y+13+i*14,151,2,forecolor);
		}
		GuiUpdateDisplayAll();
		stepTab[STEP_NORMAL] = 4;
	}
	if(stepTab[STEP_NORMAL] == 4){
		if(CheckFeventUpdata()){//SOE�и���
			currentNum = 1;
			stepTab[STEP_NORMAL] = 1;
		}
	}
	
	switch(keyStatus){
	case UpKey:
		if(stepTab[STEP_NORMAL] > 3){
			yaoCeNum --;
			if(yaoCeNum < 1){
				yaoCeNum = yaoCeScroll.max;
			}
		}
		stepTab[STEP_NORMAL] = 3;
		break;
	case LeftKey:
		if(stepTab[STEP_NORMAL] > 1){
			currentNum --;
			if(currentNum < 1){
				currentNum = Scroll.max;
			}
		}
		stepTab[STEP_NORMAL] = 1;
		break;
	case DownKey:
		if(stepTab[STEP_NORMAL] > 3){
			yaoCeNum ++;
			if(yaoCeNum > yaoCeScroll.max){
				yaoCeNum = 1;
			}
		}
		stepTab[STEP_NORMAL] = 3;
		break;
	case RightKey:
		if(stepTab[STEP_NORMAL] > 1){
			currentNum ++;
			if(currentNum > Scroll.max){
				currentNum = 1;
			}
		}
		stepTab[STEP_NORMAL] = 1;
		break;
	case OkKey:break;
	case CancelKey:
		stepTab[STEP_NORMAL] = 0;
		userGUITopWindowHide();
		userGUITopWindowRedraw();
		GuiMenuRedrawMenu(&HmiMenu);
		break;
	default:break;
	}
}

/**
  *@brief ͨ�ö�ֵ��ʾ
  *@param  param ����
  *@retval None
  */
void DingzhiWinFun(void *param)
{
	DZModfiyDisplay(param,&stepTab[STEP_NORMAL]);
}
/**
  *@brief ʱ���ƶ��޸�
  *@param  None
  *@retval None
  */
static void TimeMoveModfiy(uint8_t updown, uint8_t pmove,SystemTimeDisplay *pTime)
{
	//2018-12-23  12:23
	const uint8_t pMoveMap[] = {0,0,1,2,0,3,4,0,5,6,0,0,7,8,0,9,10,0,11,12};
	if(pmove >= sizeof(pMoveMap)){
		return;
	}
	if(updown == 1){//��
		switch(pMoveMap[pmove])
		{
			case 1://���
				pTime->year = pTime->year - 10;
				if(pTime->year > 99){
					pTime->year = 99;
				}break;
			case 2://���
				if(pTime->year - 1 < 0){
					pTime->year = 99;
				}
				else{
					pTime->year = pTime->year - 1;
				}break;
			case 3://�¸�
				pTime->month = pTime->month - 10;
				if(pTime->month > 12){
					pTime->month = 12;
				}break;
			case 4://�µ�
				if(pTime->month - 1 < 0){
					pTime->month = 12;
				}
				else{
					pTime->month = pTime->month - 1;
				}break;
			case 5://�ո�
				pTime->day = pTime->day - 10;
				if(pTime->day > 31){
					pTime->day = 31;
				}break;
			case 6://�յ�
				if(pTime->day - 1 < 0){
					pTime->day = 31;
				}
				else{
					pTime->day = pTime->day - 1;
				}break;
			case 7://ʱ��
				pTime->hour = pTime->hour - 10;
				if(pTime->hour > 23){
					pTime->hour = 23;
				}break;
			case 8://ʱ��
				if(pTime->hour - 1 < 0){
					pTime->hour = 23;
				}
				else{
					pTime->hour = pTime->hour - 1;
				}break;
			case 9://�ָ�
				pTime->min = pTime->min - 10;
				if(pTime->min > 59){
					pTime->min = 59;
				}break;
			case 10://�ֵ�
				if(pTime->min - 1 < 0){
					pTime->min = 59;
				}
				else{
					pTime->min = pTime->min - 1;
				}break;
			case 11://���
				pTime->s = pTime->s - 10;
				if(pTime->s > 59){
					pTime->s = 59;
				}break;
			case 12://���
				if(pTime->s - 1 < 0){
					pTime->s = 59;
				}
				else{
					pTime->s = pTime->s - 1;
				}break;
		}
	}
	else if(updown == 2){//��
		switch(pMoveMap[pmove])
		{
			case 1://���
				pTime->year = pTime->year + 10;
				if(pTime->year > 99){
					pTime->year = 0;
				}break;
			case 2://���
				if(pTime->year + 1 > 99){
					pTime->year = 0;
				}
				else{
					pTime->year = pTime->year + 1;
				}break;
			case 3://�¸�
				pTime->month = pTime->month + 10;
				if(pTime->month > 12){
					pTime->month = 0;
				}break;
			case 4://�µ�
				if(pTime->month + 1 > 12){
					pTime->month = 0;
				}
				else{
					pTime->month = pTime->month + 1;
				}break;
			case 5://�ո�
				pTime->day = pTime->day + 10;
				if(pTime->day > 31){
					pTime->day = 0;
				}break;
			case 6://�յ�
				if(pTime->day + 1 > 31){
					pTime->day = 0;
				}
				else{
					pTime->day = pTime->day + 1;
				}break;
			case 7://ʱ��
				pTime->hour = pTime->hour + 10;
				if(pTime->hour > 23){
					pTime->hour = 0;
				}break;
			case 8://ʱ��
				if(pTime->hour + 1 > 23){
					pTime->hour = 0;
				}
				else{
					pTime->hour = pTime->hour + 1;
				}break;
			case 9://�ָ�
				pTime->min = pTime->min + 10;
				if(pTime->min > 59){
					pTime->min = 0;
				}break;
			case 10://�ֵ�
				if(pTime->min + 1 > 59){
					pTime->min = 0;
				}
				else{
					pTime->min = pTime->min + 1;
				}break;
			case 11://���
				pTime->s = pTime->s + 10;
				if(pTime->s > 59){
					pTime->s = 0;
				}break;
			case 12://���
				if(pTime->s + 1 > 59){
					pTime->s = 0;
				}
				else{
					pTime->s = pTime->s + 1;
				}break;
		}
	}
}
/**
  *@brief ʱ���޸Ĵ�����
  *@param  None
  *@retval None
  */
static void TimeModfiyFun(void *param)
{
	static uint8_t *pTimeStr;
	static SystemTimeDisplay *pTime;
	static uint8_t pMove,tpMove,timeStrEnd,upDown;
	if(stepTab[STEP_NORMAL] == 0){//��ʼ���������ڴ�
		pTimeStr = &userGUIBuff[0];
		pTime = (SystemTimeDisplay *)&userGUIBuff[32];
		GetDisplayTime(pTime);
		sprintf((char *)pTimeStr,"20%02d-%02d-%02d  %02d:%02d:%02d",pTime->year,\
			pTime->month,pTime->day,pTime->hour,pTime->min,pTime->s);
		pMove = 2;/* ��λ����ĵ���λ,ǰ��λ�����޸� */
		tpMove = 2;
		timeStrEnd = strlen((char *)pTimeStr) - 1;
		upDown = 0; 
		stepTab[STEP_NORMAL] = 1;
	}
	if(stepTab[STEP_NORMAL] == 1){//��ͼ
		WINDOW *pWin = &TimeModfiyWin;
		GuiHPointLine(pWin->x,pWin->y + pWin->hight - 18,pWin->x + pWin->wide - 1,2,forecolor);
		GuiRPointLine(pWin->x + 40,pWin->y + pWin->hight - 18,pWin->y + pWin->hight - 1,2,forecolor);
		GuiFont12Align(pWin->x + 2,pWin->y + pWin->hight - 15,37,FONT_LEFT,"״̬:");
		stepTab[STEP_NORMAL] = 2;
	}
	if(stepTab[STEP_NORMAL] == 2){
		WINDOW *pWin = &TimeModfiyWin;
		sprintf((char *)pTimeStr,"20%02d-%02d-%02d  %02d:%02d:%02d",pTime->year,\
			pTime->month,pTime->day,pTime->hour,pTime->min,pTime->s);
		GuiFont12Align(pWin->x + 8,pWin->y + 24,126,FONT_LEFT,pTimeStr);
		GuiExchangeColor();
		uint8_t pStr1[2] = {'\0','\0'};
		pStr1[0] = pTimeStr[pMove];
		GuiFont12Align(TimeModfiyWin.x + 8 + pMove*6,TimeModfiyWin.y + 24,6,FONT_LEFT,pStr1);
		GuiExchangeColor();
		GuiFont12Align(pWin->x + 42,pWin->y + pWin->hight - 15,pWin->wide - 44,FONT_RIGHT,"�޸���...");
		GuiUpdateDisplayAll();
		stepTab[STEP_NORMAL] = 3;
	}
	if(stepTab[STEP_NORMAL] == 3 && (upDown != 0 ||  pMove != tpMove)){
		uint8_t tflag;
		if(tpMove < pMove){
			 tflag = 1;
		}
		else{
			tflag = 2;
		}
		while(1){//�����ƶ� ������Ч�ַ�
			if(pTimeStr[pMove] < '0' || pTimeStr[pMove] > '9'){
				if(tflag == 1){//����
					if(++pMove > timeStrEnd){
						pMove = 2;
					}
				}
				else{
					if(--pMove < 2){//����
						pMove = timeStrEnd;
					}
				}
			}
			else{
				break;
			}
		}
		tpMove = pMove;
		TimeMoveModfiy(upDown, pMove,pTime);
		stepTab[STEP_NORMAL] = 1;
		upDown = 0;
	}
	if(stepTab[STEP_NORMAL] == 4){
		SetDisplayTime(pTime);
		WINDOW *pWin = &TimeModfiyWin;
		GuiFillRect(pWin->x + 42,pWin->y + pWin->hight - 15,\
			pWin->x + pWin->wide - 2,pWin->y + pWin->hight - 2,backcolor);
		GuiFont12Align(pWin->x + 42,pWin->y + pWin->hight - 15,\
		pWin->wide - 44,FONT_RIGHT,"�޸ĳɹ�");
		GuiUpdateDisplayAll();
		stepTab[STEP_NORMAL] = 3;
	}
	
	switch(keyStatus){
	case UpKey:upDown = 2;break;
	case LeftKey:
		pMove --;
		if(pMove < 2){
			pMove = timeStrEnd;
		}break;
	case DownKey:upDown = 1;break;
	case RightKey:
		pMove ++;
		if(pMove > timeStrEnd){
			pMove = 2;
		}break;
	case OkKey:
		stepTab[STEP_NORMAL] = 4;break;
	case CancelKey:
		stepTab[STEP_NORMAL] = 0;
		userGUITopWindowHide();
		userGUITopWindowRedraw();
		GuiMenuRedrawMenu(&HmiMenu);
		break;
	default:break;
	}
}

/**
  *@brief ��ֵ�л����ں���
  *@param  param ����
  *@retval None
  */
void DzhiChangeFun(void *param)
{
	if(stepTab[STEP_DZHICHANGE] == 0){
		GuiFont12Align(DzhiChangeWin.x + 2,DzhiChangeWin.y + 30,\
			DzhiChangeWin.wide - 4,FONT_MID,"��ֵ�л��ɹ�");
		GuiFont12Align(DzhiChangeWin.x + 2,DzhiChangeWin.y + 45,\
			DzhiChangeWin.wide - 4,FONT_MID,"�밴������˳�");
		GuiUpdateDisplayAll();
		stepTab[STEP_DZHICHANGE] = 1;
	}
	switch(keyStatus){
	case UpKey:
	case LeftKey:
	case DownKey:
	case RightKey:
	case OkKey:
	case CancelKey:
		UserDefineDzhiInit();
		GUIDisplayInit();
		DingzhiChangeFlag = 0;
		DzChangefinshflag = 0;
		stepTab[STEP_DZHICHANGE] = 0;break;
	default:break;
	}
}

/**
  *@brief ��⵱ǰ�˵��ڵ��Ƿ���Ҫ����
  *@param  �˵�ID
  *@retval �˵��������� -1 ����Ҫ���� >= 0 ��������
  */
int8_t NeedPasswordCheck(uint16_t cId)
{
	int8_t tType;
	switch(cId){
		case MID_DZHISET:
		case MID_CMDSEND:
		case MID_SYSTEM_PARAMTER_SETTING:
		case MID_DEVICE_PARAMTER_SETTING:
			tType = 0;break;
		default:tType = -1;break;
	}
	return tType;
}

/**
  *@brief  �û��ڵ���Ϣ����
  *@param  �˵�ID
  *@retval None
  */
void UserItemsUseDealWith(uint16_t cId)
{
	switch(cId){
		case MID_DZHISET:
		case MID_SYSTEM_PARAMTER_SETTING:
		case MID_DEVICE_PARAMTER_SETTING:
			SetDingZhiSetOrSee(SET_DINGZHI);break;
		case MID_DZHISCAN:
			SetDingZhiSetOrSee(SEE_DINGZHI);break;	
		default:break;
	}
}
/**
  *@brief  �˵����
  *@param  step ���� pNode �˵��ڵ�
  *@retval None
  */
static void MenuItemDealWith(uint8_t *step ,lkdMenuNode *pNode)
{
	static uint8_t passWordState;
	int8_t passWordType;
	if(*step == 1){
		passWordType = NeedPasswordCheck(pNode->nodeId);
		if(passWordType >= 0){
			passWordState = 3;
			SetPassWordWin(passWordType,&passWordState);
			userGUIWindowAdd(&PassWordWin);
			*step = 2;
		}
		else{
			*step = 3;
		}
	}
	if(*step == 2){//�ȴ����봰�ڷ���
		if(passWordState == 1){//������ȷ
			*step = 3;
		}
		//������벻��ȷ�����봰���˳�ʱ���ɴ̼�ȡ������������
	}
	if(*step == 3){//�˵�����
		UserItemsUseDealWith(pNode->nodeId);
		if(pNode->pSon != NULL){
			GuiMenuCurrentNodeSonUnfold(&HmiMenu);
		}
		else if(pNode->user != NULL){
			userGUIWindowAdd(pNode->user);
		}
		*step = 0;//
	}
}

/**
  *@brief  �˵����ƺ���
  *@param  None
  *@retval None
  */
void MenuControlFun(void)
{
	switch(keyStatus){
	case UpKey:GuiMenuItemUpMove(&HmiMenu);break;
	case DownKey:GuiMenuItemDownMove(&HmiMenu);break;
	case LeftKey:
	case CancelKey:
		stepTab[STEP_MAINMENU] = 0;
		GuiMenuCurrentNodeHide(&HmiMenu);
		if(HmiMenu.count == 0){
			userGUITopWindowHide();
			userGUITopWindowRedraw();
		}break;
	case RightKey:
	case OkKey:
		stepTab[STEP_MAINMENU] = 1;break;
	}
	if(stepTab[STEP_MAINMENU] > 0){
		MenuItemDealWith(&stepTab[STEP_MAINMENU],GuiMenuGetCurrentpNode(&HmiMenu));
	}
}

/**
  *@brief Menu Window deal with function
  *@param  None
  *@retval None
  */
static void MenuWindowFun(void *param)
{
	if(HmiMenu.count == 0){
		GuiMenuCurrentNodeSonUnfold(&HmiMenu);
	}
	HmiMenu.MenuDealWithFun();
}

/**
  *@brief  ���ö�ֵ�˵��ӽڵ�
  *@param  None
  *@retval None
  */
void SetDzhiSonNode(lkdMenuNode *pSon)
{
	MenuTreeSNodeInsert(&Node5,pSon);
	MenuTreeSNodeInsert(&Node2,pSon);
}

/**
  *@brief  ��ֵ�˵��ṹ�仯�ص�
  *@param  None
  *@retval None
  */
void DzhiChangeCallBack(void)
{
	DingzhiChangeFlag = 1;
}

/* END */
