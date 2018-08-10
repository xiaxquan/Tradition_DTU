/**
  * @file   GUIdisplay.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    03-06-2018
  * @brief   GUI显示
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

uint8_t *userGUIBuff; /* 用于内存分配 */
const static uint8_t *modfiyKey[14] = {/* 软按键 */
	"0","1","2","3","4",".","esc","5","6","7","8","9","<-","ok"};
static enum KeyStatus keyStatus;	/* 按键值 */
static struct DZModfiy dZModfiy;	/* 定值修改 */
static struct Message MessageIs;	/* 消息管理 */
static struct PassWordPipe passWordPipe;		/* 密码管理 */
static struct PasswordSave passwordSave;		/* 保存密码 */
static enum DingZhiSetOrSee dingZhiSetOrSee;	/* 定值查询或者修改标志 */
static uint8_t stepTab[STEP_ALLNUM];
static uint8_t DingzhiChangeFlag,DzChangefinshflag;
/* 菜单句柄 */
lkdMenu HmiMenu;
/* 菜单栈 */
#define MENUSTACK_NUM 8
MenuStack userMenuStack[MENUSTACK_NUM];

/* 窗口定义 */
WINDOW HomeWindow = {0,0,160,160,NULL,HomeWindowFun,NULL};/* 主窗口 */
WINDOW MenuWindow = {0,0,160,160,"菜单管理窗口",MenuWindowFun,NULL};

WINDOW TimeModfiyWin  = {12,44,136,66,"时间修改窗口",TimeModfiyFun,NULL};
WINDOW CmdSendWin = {0,0,160,160,"命令下发窗口",CmdSendWinFun,NULL};
WINDOW VersionWin = {0,0,160,160,"版本信息管理",VersionWinFun,NULL};
WINDOW PasswordModfiyWin = {0,0,160,160,"修改密码",PasswordModfiyWinFun,NULL};

WINDOW YaoxinWin     = {0,0,160,160,"遥信状态窗口",YaoxinFun,NULL};
WINDOW SOEWin   	 = {0,0,160,160,"SOE记录窗口",SoeCoWinFun,&soeInfo};
WINDOW COWin   	 = {0,0,160,160,"操作记录窗口",SoeCoWinFun,&coInfo};
WINDOW FaultEventWin = {0,0,160,160,"故障事件窗口",FaultEventFun,NULL};
WINDOW CheckRecordWin = {0,0,160,160,"诊断信息窗口",SoeCoWinFun,&ulogInfo};

WINDOW BasicSetWin   = {0,0,160,160,"基本设置窗口",DingzhiWinFun,&dzhi0Info[ME_BASIC_SET]};
WINDOW ZeroDriftWin  = {0,0,160,160,"零漂设置窗口",DingzhiWinFun,&dzhi0Info[ME_ZERODRIFT]};
//WINDOW DeadZoneWin 	 = {0,0,160,160,"死区设置窗口",DingzhiWinFun,&dzhi0Info[ME_UART_COM]};
WINDOW SerialWin     = {0,0,160,160,"串口设置窗口",DingzhiWinFun,&dzhi0Info[ME_UART_COM]};
WINDOW InternetWin 	 = {0,0,160,160,"网口设置窗口",DingzhiWinFun,&dzhi0Info[ME_NET_COM]};

WINDOW YaoCe1Win   = {0,0,160,160,"一次遥测窗口",YaoceWinFun,&yceInfo[0]};
WINDOW YaoCe2Win   = {0,0,160,160,"二次遥测窗口",YaoceWinFun,&yceInfo[1]};
WINDOW HarmonicWin = {0,0,160,160,"谐波分量窗口",YaoceWinFun,&yceInfo[2]};

/* 公用窗口 */
WINDOW DZModfiyWin = {14,19,130,134,"定值修改窗口",DZModfiyFun,NULL};
WINDOW MessageWin = {23,40,110,75,"消息窗口",MessageFun,NULL};
WINDOW PassWordWin = {23,40,110,75,"密码窗口",PassWordFun,NULL};
WINDOW DzhiChangeWin = {23,40,110,80,"信息提示",DzhiChangeFun,&DzChangefinshflag};

/* 菜单节点定义 */
/* -----------------------------------------------菜单结构---------------------------------------------------- */
lkdMenuNode Node22 = {MID_YAOXIN,"遥信查询",NULL,NULL,&YaoxinWin};
lkdMenuNode Node21 = {MID_HARMONIC_COMPONENT,"谐波分量",&Node22,NULL,&HarmonicWin};
lkdMenuNode Node20 = {MID_SECOND_TELEMETRY,"二次遥测",&Node21,NULL,&YaoCe2Win};
lkdMenuNode Node19 = {MID_ONE_TELEMETRY,"一次遥测",&Node20,NULL,&YaoCe1Win};

lkdMenuNode Node18 = {MID_BASICSET,"基本设置",NULL,NULL,&BasicSetWin};

lkdMenuNode Node23 = {MID_PASSWORDMODFIY,"密码修改",NULL,NULL,&PasswordModfiyWin};
lkdMenuNode Node17 = {MID_TIMEMODFIY,"时间修改",&Node23,NULL,&TimeModfiyWin};
lkdMenuNode Node16 = {MID_ZERODRIFT,"零漂设置",&Node17,NULL,&ZeroDriftWin};
//lkdMenuNode Node15 = {MID_DEADEZONE,"死区设置",&Node16,NULL,&DeadZoneWin};
lkdMenuNode Node14 = {MID_INTERNET,"网口设置",&Node16,NULL,&InternetWin};
lkdMenuNode Node13 = {MID_SERIAL,"串口配置",&Node14,NULL,&SerialWin};

lkdMenuNode Node12 = {MID_CHECKRECORD,"自检记录",NULL,NULL,&CheckRecordWin};
lkdMenuNode Node11 = {MID_DEFAULTEVENT,"故障事件",&Node12,NULL,&FaultEventWin};
lkdMenuNode Node10 = {MID_OPERATERECORD,"操作事件",&Node11,NULL,&COWin};
lkdMenuNode Node9 = {MID_SOE,"SOE查询",&Node10,NULL,&SOEWin};

lkdMenuNode Node8 = {MID_VERSIONINFO,"版本信息",NULL,NULL,&VersionWin};
lkdMenuNode Node7 = {MID_SYSTEM_PARAMTER_SETTING,"系统设置",&Node8,&Node18,NULL};
lkdMenuNode Node6 = {MID_DEVICE_PARAMTER_SETTING,"装置设置",&Node7,&Node13,NULL};
lkdMenuNode Node5 = {MID_DZHISET,"定值设置",&Node6,NULL,NULL};
lkdMenuNode Node4 = {MID_CMDSEND,"命令下发",&Node5,NULL,&CmdSendWin};
lkdMenuNode Node3 = {MID_DEVICEEVENTRECORD,"事件记录",&Node4,&Node9,NULL};
lkdMenuNode Node2 = {MID_DZHISCAN,"定值查询",&Node3,NULL,NULL};
lkdMenuNode Node1 = {MID_REALTIMEDATA,"实时数据",&Node2,&Node19,NULL};
lkdMenuNode Node0 = {MID_ROOT,"root",NULL,&Node1,NULL};
/* ----------------------------------------------------------------------------------------------------------- */

/**
  *@brief  菜单初始化
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
  *@brief  获取时钟
  *@param  None
  *@retval 当前时钟滴答
  */	
static uint32_t getCurrentTick(void)
{
	return GetTimer1Tick();
}

/**
  *@brief  获取间隔时间
  *@param  开始时钟滴答
  *@retval 间隔时间
  */
static uint32_t GetIntervalTick(uint32_t beginTick)
{
	return GetTimer1IntervalTick(beginTick);
}

/**
  *@brief 修改定值设置或者浏览标志
  *@param  setOrSee 设置、浏览 枚举
  *@retval None
  */
static void SetDingZhiSetOrSee(enum DingZhiSetOrSee setOrSee)
{
	dingZhiSetOrSee = setOrSee;
}

/**
  *@brief 获取定值设置或者浏览标志
  *@param  None
  *@retval DingZhiSetOrSee 设置、浏览 枚举
  */
static enum DingZhiSetOrSee GetDingZhiSetOrSee(void)
{
	return dingZhiSetOrSee;
}

/**
  *@brief lcd显示开关
  *@param  key 按键状态
  *@retval lcd显示状态 LCDDISPLAYON LCDDISPLAYOFF
  */
static uint8_t GUIDisplayONOFF(enum KeyStatus *key)
{
	static uint32_t LcdDisPlayTick;//计时
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
				if(GetIntervalTick(LcdDisPlayTick) > LCDDISPLAYOFFTIME){//时间
					CloseLcdDisplay();
					lcdONOFF = LCDDISPLAYOFF;
				}
			}
		}
	}
	return lcdONOFF;
}

/**
  *@brief GUI在线更新
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
	GuiFont12Align(2,82,156,FONT_MID,"正在更新......");
	GuiExchangeColor();
	GuiUpdateDisplayAll();
}

/**
  *@brief GUI等待图标
  *@param  x,y 起始坐标
  *@retval None
  */
void GUIWiatMessage(uint8_t x,uint8_t y)
{
	GuiFillRect(x,y,x + 88,y + 16, backcolor);
	GuiHLine(x + 2,y + 16,x + 86,forecolor);
	GuiHLine(x + 3,y + 17,x + 87,forecolor);
	GuiRLine(x + 86,y + 2,y + 16,forecolor);
	GuiRLine(x + 87,y + 3,y + 16,forecolor);
	GuiFont12Align(x + 20,y + 2,70,FONT_LEFT,"加载中.....");
	GuiFillRect(x + 2,y + 2,x + 14,y + 14, forecolor);
	GuiHLine(x + 2,y + 8,x + 14,backcolor);
	GuiRLine(x + 8,y + 2,y + 14,backcolor);
	GuiFillRect(x + 2,y + 2,32 + 12,y + 14, forecolor);
	GuiHLine(x + 2,y + 8,x + 14,backcolor);
	GuiRLine(x + 8,y + 2,y + 14,backcolor);
	GuiUpdateDisplayAll();
}

/**
  *@brief 定值变化处理
  *@param  x,y 起始坐标
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
  *@brief GUI初始化，添加主窗口
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
  *@brief GUI主函数，用于主函数调用
  *@param  None
  *@retval None
  */
void GUIDisplayMian(void)
{
	static uint8_t flag;
	keyStatus = GetKeyStatus();//获取按键状态
	SetKeyIsNoKey();
	if(CheckUpdataProgram() != 0){
		if(flag == 0){
			OpenLcdDisplay();
			GUIUpdataProgram();//在线更新
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
  *@brief 设置密码窗口相关值
  *@param  type 密码类型
  *@param  returnflag 返回状态指针
  *@retval None
  */
void SetPassWordWin(uint8_t type,uint8_t *returnflag)
{
	passWordPipe.passWordType = type;
	passWordPipe.returnFlag = returnflag;
}

/**
  *@brief 密码窗口
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
			PassWordItems[passWordPipe.passWordType].name);//提示语
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
		}//需要执行CancelKey程序，所以没有break
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
  *@brief 设置消息窗口相应变量
  *@param  x ,y 起始坐标
  *@param  pStr 说明字符串
  *@param  flag 返回标志 0 取消 1 确定
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
  *@brief 消息窗口处理函数
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
			MessageWin.wide - 1,FONT_MID,MessageIs.str);//提示语
		winHight = MessageWin.y + MessageWin.hight - 23;
		GuiHPointLine(MessageWin.x+1,winHight,\
			MessageWin.x + MessageWin.wide - 1,2,forecolor);
		if(keyFlag == 0){
			winWide =  MessageWin.x + (MessageWin.wide/2 - 45)/2;
			GuiButton(winWide,winHight + 3,"确认",0);
			winWide +=  MessageWin.wide/2;
			GuiButton(winWide,winHight + 3,"取消",1);
		}
		else{
			winWide =  MessageWin.x + (MessageWin.wide/2 - 45)/2;
			GuiButton(winWide,winHight + 3,"确认",1);
			winWide +=  MessageWin.wide/2;
			GuiButton(winWide,winHight + 3,"取消",0);
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
  *@brief  设置定值修改相关变量
  *@param  info 定值结构体指针
  *@param  flag 标志字节，未使用
  *@param  itemIs 定值当前Item
  *@param  str 要修改字符串的指针
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
  *@brief 定值修改初始化
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
		DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);//状态栏
	GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + DZModfiyWin.hight - 14,80,FONT_LEFT,"当前状态:");
	
	if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType){//汉字
		x = DZModfiyWin.x + DZModfiyWin.wide;
		y = DZModfiyWin.y + 25 + (DZModfiyWin.hight - 43)/2 - 8;
		GuiFillRect(DZModfiyWin.x+9,y,x - 10,y + 17, backcolor);
		GuiFillRect(DZModfiyWin.x+17,y + 2,x - 20,y + 15, forecolor);
		GuiFont12Align(DZModfiyWin.x+7,y+4,10,FONT_LEFT,"<");
		GuiFont12Align(x - 14,y+4,10,FONT_LEFT,">");
	}
	else{//float
		GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + 32,40,FONT_LEFT,"最大值");
		tRange = dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].valMax;
		sprintf((char *)tRangeStr,"%.3f",tRange);
		tRangeStr[15] = '\0';
		GuiFont12Align(DZModfiyWin.x + 50,DZModfiyWin.y + 32,80,FONT_MID,tRangeStr);
		GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + 45,\
			DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);
		GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + 47,40,FONT_LEFT,"最小值");
		tRange = dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].valMin;
		sprintf((char *)tRangeStr,"%.3f",tRange);
		tRangeStr[15] = '\0';
		GuiFont12Align(DZModfiyWin.x + 50,DZModfiyWin.y + 47,80,FONT_MID,tRangeStr);
		GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + 60,\
			DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);
		GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + 62,40,FONT_LEFT,"当前值");
		GuiFillRect(DZModfiyWin.x + 50,DZModfiyWin.y + 62,\
			DZModfiyWin.x + 128,DZModfiyWin.y + 74, backcolor);
		GuiFont12Align(DZModfiyWin.x + 50,\
			DZModfiyWin.y + 62,80,FONT_MID,dZModfiy.str);
		GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + 75,\
			DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);
		GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + 77,40,FONT_LEFT,"修改为");
		GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + 90,\
			DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);
		GuiRPointLine(DZModfiyWin.x + 47,DZModfiyWin.y + 30,\
			DZModfiyWin.y + 90,2,forecolor);	
	}
}

/**
  *@brief 画定值修改按键
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
  *@brief 定值修改窗口函数
  *@param  None
  *@retval None
  */
void DZModfiyFun(void *param)
{
	static uint8_t modfiyStr[16];//修改暂存buff
	static uint8_t modfiyStrP = 0;
	static uint8_t keyIs = 0;//按键记录
	static uint32_t inputCursorTick;//光标闪烁记时
	int16_t x,y;
	float fValue;
	if(stepTab[STEP_DZHIMODFIY] == 0){//初始化
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
	if(stepTab[STEP_DZHIMODFIY] == 1){//显示
		if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType){//汉字处理
			GuiExchangeColor();
			x = DZModfiyWin.x + DZModfiyWin.wide;
			y = DZModfiyWin.y + 25 + (DZModfiyWin.hight - 43)/2 - 8;
			GuiFillRect(DZModfiyWin.x+18,y + 3,x - 20,y + 15, backcolor);
			GuiFont12Align(DZModfiyWin.x+18,y + 3,x - DZModfiyWin.x - 40,FONT_MID,\
				(uint8_t *)dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].pContent[keyIs]);
			GuiExchangeColor();
			stepTab[STEP_DZHIMODFIY] = 3;
		}
		else{//float处理
			DrawModfiyKey(keyIs);
			GuiFillRect(DZModfiyWin.x + 50,DZModfiyWin.y + 77,\
				DZModfiyWin.x + 128,DZModfiyWin.y + 89, backcolor);
			GuiFont12Align(DZModfiyWin.x + 50,\
				DZModfiyWin.y + 77,78,FONT_MID,modfiyStr);
			stepTab[STEP_DZHIMODFIY] = 2;
		}
		GuiUpdateDisplayAll();
	}
	if(stepTab[STEP_DZHIMODFIY] == 2){//光标闪动处理
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
	if(stepTab[STEP_DZHIMODFIY] == 3){//等待
	}
	if(stepTab[STEP_DZHIMODFIY] == 4){//发送命令
		if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType){//汉字		
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
			else{//超出范围
				GuiFillRect(DZModfiyWin.x + 57,DZModfiyWin.y + DZModfiyWin.hight - 14,\
					DZModfiyWin.x + DZModfiyWin.wide - 2,DZModfiyWin.y + DZModfiyWin.hight - 2, backcolor);
				GuiFont12Align(DZModfiyWin.x + 57,DZModfiyWin.y + DZModfiyWin.hight - 14,70,FONT_RIGHT,"超出范围");
				modfiyStr[modfiyStrP] = ' ';
				modfiyStr[modfiyStrP + 1] = '\0';
				stepTab[STEP_DZHIMODFIY] = 2;
			}
		}
	}
	if(stepTab[STEP_DZHIMODFIY] == 5){//发送完后等待回复
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
		if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType){//命令发送
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
				stepTab[STEP_DZHIMODFIY] = 4;//命令发送
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
  *@brief 绘制定值表格函数
  *@param  pText:表格内容指针。
  *@param  currentRow:保存当前所在行。
  *@retval None
  */
static void DisplayDZ(struct DZItemsManage *dz_items_manage)
{		
	uint8_t drawx = 0;
	uint8_t drawy = 18;										/* 定值表格开始坐标 */
	
	uint8_t name_disp_len = 158;							/* 定值名字一行显示的最大长度 */
	uint8_t value_disp_len = 158 / 2;						/* 对应的值显示的最大长度 */
	uint8_t unit_disp_len = name_disp_len - value_disp_len;	/* 单位显示的最大长度 */
	
	dz_items_manage->start_itemes = dz_items_manage->current_items / DZDISPLAYROW * DZDISPLAYROW;
	/* 清屏 */
	GuiFillRect(drawx + 1, drawy, GUIXMAX - 1, GUIYMAX - 1,backcolor);
	
	for(uint8_t i = dz_items_manage->start_itemes; i < dz_items_manage->items_num; ++i){
		GuiHLine(drawx, drawy, GUIXMAX - 1, forecolor);
		if(dz_items_manage->current_items == i){	/* 选中项目高亮 */
			GuiFillRect(drawx, drawy, GUIYMAX - 1, drawy + 2*LIST_CONTENT_ROWHIGHT, forecolor);
			GuiExchangeColor();
		}
		/* 写定值名 */
		GuiFont12Align(drawx + 2,drawy + 1, name_disp_len, FONT_MID, *(dz_items_manage->pText + i * DZDISPLAYCOL + 0));
		drawy += LIST_CONTENT_ROWHIGHT;
		GuiHPointLine(drawx, drawy, GUIXMAX - 1, 2, forecolor);
		/* 写数值 */
		GuiFont12Align(drawx + 2, drawy + 1, value_disp_len, FONT_MID, *(dz_items_manage->pText + i * DZDISPLAYCOL + 1));
		drawx += value_disp_len;
		GuiRPointLine(drawx, drawy, drawy + LIST_CONTENT_ROWHIGHT, 2, forecolor);
		/* 写单位 */
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
  *@brief 通用定值显示函数
  *@param  Items 定值结构指针
  *@param  itemsNum 定值数
  *@param  cmd 命令
  *@param  flag 步骤标志
  *@retval None
  */
void DZModfiyDisplay(DzhiDisplayInfo *info,uint8_t *flag)
{
	static SCROLL *Scroll;//进度条
	static uint8_t **pText;//列表内容指针
	static uint8_t *col1Data;//显示内容指针
	static struct DZItemsManage dz_items_manage;
	uint8_t i,itemsNum;
	uint16_t j ;
	uint16_t memMall;//用于内存分配
	float tempFloat;
	
	if(*flag == 0){//初始化
		if(info->num == 0){//如果没有Item,直接退出
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
			if(info->pRoot[info->pBuff[i]].dataType){//汉字
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
	if(*flag == 2){//指定表格内容指针
		
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
		if(GetDingZhiSetOrSee() == SET_DINGZHI){//定值修改
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
	if(stepTab[STEP_HOMEWIN] == 0){//绘制主页
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
		
		GuiFont12Align(HomeWindow.x+9,102,16,FONT_LEFT,"信");
		GuiFont12Align(HomeWindow.x+9,121,16,FONT_LEFT,"息");
		GuiFont12Align(HomeWindow.x+9,140,16,FONT_LEFT,"栏");
		GuiRLine(HomeWindow.x+30,96,158,forecolor);
		
		for(i=0;i<4;i++){
			GuiFont12Align(HomeWindow.x+31,96+2+i*16,90,FONT_MID,"----");
			GuiRPointLine(HomeWindow.x+31+91,96,96+15+i*16,2,forecolor);
			GuiFont12Align(HomeWindow.x+31+93,96+2+i*16,34,FONT_MID,"--");
			GuiHPointLine(HomeWindow.x+30,96+15+i*16,159,2,forecolor);
		}
		
		GuiFont12Align(HomeWindow.x+31,96+34,90,FONT_MID,"开关状态");
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
			GuiFont12Align(HomeWindow.x+31+93,96+34,34,FONT_MID,"合");
		}
		else{
			GuiFont12Align(HomeWindow.x+31+93,96+34,34,FONT_MID,"分");
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
  *@brief CmdSend deal with function 命令下发
  *@param  None
  *@retval None
  */
static void CmdSendWinFun(void *param)
{
	const  uint8_t listCol[2][1] = {{156},{FONT_MID}};
	static SCROLL *Scroll;//进度条
	static ListColSet colset;
	static LIST  *list;//列表控件
	static uint8_t **pText;//列表内容指针
	static uint8_t messageFlag = 0;//步骤标志
	static int8_t currentNum;
	uint16_t memMall;
	uint8_t i;
	if(stepTab[STEP_NORMAL] == 0){//初始化，分配内存
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
	if(stepTab[STEP_NORMAL] == 1){//获取相应数据	
		for(i = 0;i < hcmdInfo.itemsNum;i++){
			*(pText + i) = hcmdInfo.pHmiCmd[i].name;
		}
		stepTab[STEP_NORMAL] = 2;
	}
	if(stepTab[STEP_NORMAL] == 2){//显示
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
		SetMessageWin(23,40,"确定执行?",&messageFlag);
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
  *@brief  版本信息数据处理
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
  *@brief  版本信息
  *@param  None
  *@retval None
  */
static void VersionWinFun(void *param)
{
	static SCROLL *Scroll;//进度条
	static uint8_t *col1Data;//显示内容指针		
	static int8_t currentNum;
	uint16_t memMall;
	uint8_t i,y;
	if(stepTab[STEP_NORMAL] == 0){//初始化，分配内存
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
	if(stepTab[STEP_NORMAL] == 1){//获取相应数据
		stepTab[STEP_NORMAL] = 2;
	}
	if(stepTab[STEP_NORMAL] == 2){//显示
		y = VersionWin.y + 18;
		GuiFillRect(VersionWin.x+1,y,155,158, backcolor);
		for(i = 0; i < CONSTPARDISPLAYROW; i++){
			if((i + currentNum * CONSTPARDISPLAYROW) >= versInfo.num){
				GuiHLine(VersionWin.x,y+i*28+0,155,forecolor);//水平线
				break;
			}
			GuiHLine(VersionWin.x,y+i*28+0,155,forecolor);//水平线		
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
  *@brief  密码修改函数
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
	
	if(stepTab[STEP_NORMAL] == 1) {	/* 初始化 */
		memset(userGUIBuff,0,sizeof(userGUIBuff)/sizeof(userGUIBuff[0]));
		memMall = 0;
		hins = (uint8_t*)&userGUIBuff[memMall];
		currentNum = 0;
		
		memset(passWordStr,'0',sizeof(passWordStr) / sizeof(passWordStr[0]));
		passwordStep = 0;
		GuiFillRect(1,20,158,158,backcolor);
		stepTab[STEP_NORMAL] = 2;
	}
	if(stepTab[STEP_NORMAL] == 2) {	/* 选择修改几级密码 */
		GuiFillRect(1,20,158,158,backcolor);
		hins[0] = SelectPasswordLevel + 1 + 48;
		strncpy((char*)hins + 1,"级密码修改",strlen("级密码修改") + 1);
		GuiFillRect(41,23,116,38,forecolor);
		GuiExchangeColor();
		GuiFont12Align(42,25,116 - 42,FONT_MID,hins);
		GuiExchangeColor();
		GuiFont12Align(22,25,12,FONT_MID,"<");
		GuiFont12Align(125,25,12,FONT_MID,">");
		
		GuiFont12Align(15,55,72,FONT_LEFT,"原始密码:");
		GuiRect(90,53,132,67,forecolor);
		GuiFont12Align(15,75,72,FONT_LEFT,"新密码:");
		GuiRect(90,73,132,87,forecolor);
		GuiFont12Align(15,95,72,FONT_LEFT,"确认新密码:");
		GuiRect(90,93,132,107,forecolor);
		
		GuiUpdateDisplayAll();
	}
	if(stepTab[STEP_NORMAL] == 3) {	/* 修改密码 */
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
	if(stepTab[STEP_NORMAL] == 4) {	/* 确认修改密码 */
		if(strncmp((const char*)savePassword[0],(const char*)PassWordItems[SelectPasswordLevel].passWord,6) == 0 || strncmp((const char*)savePassword[0],MANUFACTURER_PASSWORD,6) == 0) {	/* 原密码正确 */
			if(strncmp((const char*)savePassword[1],(const char*)savePassword[2],6) == 0)	{	/* 两次新密码一致 */
				strncpy((char*)passwordSave.password[SelectPasswordLevel],(const char*)savePassword[1],6);
				strncpy((char*)PassWordItems[SelectPasswordLevel].passWord,(const char*)savePassword[1],6);
				/* 存储到Flash中 */
				strncpy((char*)passwordSaveBuff,(const char*)passwordSave.password[0],6);
				strncpy((char*)passwordSaveBuff + 6,(const char*)passwordSave.password[1],6);
				strncpy((char*)passwordSaveBuff + 12,"0000",4);
				rt_multi_common_data_fram_record_write(PASSPHRASE, passwordSaveBuff, 16);
				
				strcpy((char*)hins,"修改成功");
				stepTab[STEP_NORMAL] = 5;
			}
			else {
				strcpy((char*)hins,"两次密码不一致");
			}
		}
		else {
			strcpy((char*)hins,"原密码错误");
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
  *@brief 遥信处理函数
  *@param  None
  *@retval None
  */
static void YaoxinFun(void *param)
{
	const  uint8_t listCol[2][2] = {{90,66},{FONT_LEFT,FONT_MID}};
	static SCROLL *Scroll;//进度条
	static ListColSet colset;
	static LIST  *list;//列表控件
	static uint8_t **pText;//列表内容指针
	static uint8_t *col1Data;//显示内容指针	
	static uint32_t YaoXinTick;
	static int16_t currentNum;
	uint16_t memMall;
	uint8_t i;
	uint8_t itemsNum;
	
	if(stepTab[STEP_NORMAL] == 0){//初始化，分配内存
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
	if(stepTab[STEP_NORMAL] == 1){//获取相应数据
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
	if(stepTab[STEP_NORMAL] == 2){//显示
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
	if(stepTab[STEP_NORMAL] == 3){//检测遥信变化
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
  *@brief 遥测处理显示
  *@param  items 遥测显示项
  *@param  itemsNum 遥测数量
  *@param  items modbus命令
  *@retval None
  */
void yaoCeDisplay(YaoceDisplayInfo *info)
{
	const uint8_t listCol[2][YaoCeDISPLAYCOL] = {//表格列修饰
		{50,75,31},{FONT_LEFT,FONT_MID,FONT_MID}};
	static SCROLL *Scroll;//进度条
	static ListColSet colset;
	static LIST  *list;//列表控件
	static uint8_t **pText;//列表内容指针
	static uint8_t *col1Data;//显示内容指针		
	static uint32_t YaoCeTick;//循环计时
	static int16_t currentNum;
	uint16_t memMall;
	uint8_t i;
	uint8_t itemsNum = info->num;/* 可显示条目 */
	float tempFloat;	
	if(stepTab[STEP_NORMAL] == 0){//初始化，分配内存
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
	if(stepTab[STEP_NORMAL] == 2){//显示
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
		if(GetIntervalTick(YaoCeTick) > YAOCE_UPDATATIME){//检测更新
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
  *@brief 遥测处理函数
  *@param  None
  *@retval None
  */
void YaoceWinFun(void *param)
{
	yaoCeDisplay(param);
}

/**
  *@brief Coe/Co处理函数
  *@param  pInfo Coe/Co信息结构体
  *@retval None
  */
static void SoeCoDisplay(SoeDisplayInfo *pInfo)
{
	static struct SOEDisplay *soeStr;
	static SCROLL Scroll ={156,18,141,2,1};//进度条
	static uint8_t currentNum;
	SystemTimeDisplay *tTim;
	uint8_t tRes;
	uint8_t i;
	uint16_t y;
	
	if(stepTab[STEP_NORMAL] == 0){//初始化，分配内存
		soeStr = (struct SOEDisplay *)&userGUIBuff[0];
		soeStr->pRead = 0;
		soeStr->allNum = pInfo->GetNum();
		currentNum = 1;
		stepTab[STEP_NORMAL] = 1;
	}
	if(stepTab[STEP_NORMAL] == 1){
		if(soeStr->allNum == 0){//没有SOE
			GuiFont12Align(SOEWin.x+2,SOEWin.y + 40,SOEWin.x+SOEWin.wide-4,FONT_MID,"当前没有信息记录");
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
		if(soeStr->pRead >= soeStr->allNum){//SOE最后一条
			stepTab[STEP_NORMAL] = 3;
		}
		y = SOEWin.y + 18;
		GuiFillRect(SOEWin.x+1,y,155,158, backcolor);
		for(i = 0; i < SOEDISPLAYROW; i ++){
			if(soeStr->pRead >= soeStr->allNum){//SOE最后一条
				GuiHLine(SOEWin.x,y+i*28,155,forecolor);
				break;
			}
			tRes = pInfo->GetNoContent(soeStr->pRead,&soeStr->pSoe);
			if(tRes != 0){//获取SOE有错
				GuiHLine(SOEWin.x,y+i*28,155,forecolor);
				break;
			}
			tTim = &soeStr->pSoe.time;
			sprintf((char *)soeStr->time,"%02d/%02d/%02d-%02d:%02d:%02d.%03d",\
				tTim->year,tTim->month,tTim->day,tTim->hour,tTim->min,tTim->s,tTim->ms);
			soeStr->time[23] = '\0';
			sprintf((char *)soeStr->itemNum,"%d",soeStr->pRead + 1);
			soeStr->itemNum[3] = '\0';
			GuiHLine(SOEWin.x,y+i*28+0,155,forecolor);//水平线
			GuiFillRect(SOEWin.x+1,y+i*28,SOEWin.x+20,y+i*28+13, forecolor);
			GuiExchangeColor();
			GuiFont12Align(SOEWin.x+1,y + i*28+1,19,FONT_MID,soeStr->itemNum);//序号
			GuiExchangeColor();	
			GuiRPointLine(SOEWin.x+20,y+i*28+1,y+i*28+13,2,forecolor);//垂直线
			GuiFont12Align(SOEWin.x + 21,y + i*28+1,133,FONT_RIGHT,soeStr->time);
			GuiHPointLine(SOEWin.x,y+i*28+13,155,2,forecolor);
			GuiFont12Align(SOEWin.x+2,y + i*28+15,72,FONT_MID,soeStr->pSoe.pName);
			GuiRPointLine(SOEWin.x+10+72,y+i*28+15,y+i*28+27,2,forecolor);
			if(soeStr->pSoe.highlight){//高亮显示
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
		if(pInfo->CheckUpdata()){//Co有更新
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
  *@brief Soe/CO处理函数
  *@param  None
  *@retval None
  */
void SoeCoWinFun(void *param)
{
	SoeCoDisplay(param);
}
/**
  *@brief 故障事件处理函数
  *@param  None
  *@retval None
  */
static void FaultEventFun(void *param)
{
	static struct EventDisplay *pEventStr;
	static SCROLL Scroll ={156,18,141,2,1};//进度条
	static SCROLL yaoCeScroll ={153,47,112,2,1};//进度条
	static int16_t currentNum;
	static int8_t yaoCeNum;
	uint8_t i;
	uint16_t y;
	
	if(stepTab[STEP_NORMAL] == 0){//初始化，分配内存
		pEventStr = (struct EventDisplay *)&userGUIBuff[0];
		pEventStr->pRead = 0;
		pEventStr->allNum = GetFeventNum();
		currentNum = 1;
		yaoCeNum = 1;
		stepTab[STEP_NORMAL] = 1;
	}
	if(stepTab[STEP_NORMAL] == 1){
		if(pEventStr->allNum == 0){//没有故障事件
			GuiFont12Align(FaultEventWin.x+2,FaultEventWin.y + 50,\
			FaultEventWin.x+FaultEventWin.wide-4,FONT_MID,"当前没有故障事件");
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
	if(stepTab[STEP_NORMAL] == 2){//画图
		y = FaultEventWin.y + 18;
		GuiFillRect(FaultEventWin.x+1,y,155,158, backcolor);
		GuiHLine(FaultEventWin.x,y,155,forecolor);//水平线	
		GuiFillRect(FaultEventWin.x+1,y,FaultEventWin.x+20,y+13, forecolor);
		SystemTimeDisplay *tTim;
		tTim = &pEventStr->pFevent.time;
		sprintf((char *)pEventStr->time,"%02d/%02d/%02d-%02d:%02d:%02d.%03d",\
			tTim->year,tTim->month,tTim->day,tTim->hour,tTim->min,tTim->s,tTim->ms);
		pEventStr->time[23] = '\0';
		sprintf((char *)pEventStr->itemNum,"%d",pEventStr->pRead + 1);
		pEventStr->itemNum[3] = '\0';
		GuiExchangeColor();
		GuiFont12Align(FaultEventWin.x+1,y+1,20,FONT_MID,pEventStr->itemNum);//序号
		GuiExchangeColor();	
		GuiRPointLine(FaultEventWin.x+20,y+1,y+13,2,forecolor);//垂直线
		GuiFont12Align(FaultEventWin.x + 21,y+1,133,FONT_RIGHT,pEventStr->time);
		GuiHPointLine(FaultEventWin.x,y+13,155,2,forecolor);
		GuiFont12Align(FaultEventWin.x+2,y+15,72,FONT_LEFT,pEventStr->pFevent.pName);
		GuiRPointLine(FaultEventWin.x+10+72,y+15,y+27,2,forecolor);
		GuiFont12Align(FaultEventWin.x+10+73,y+15,70,FONT_RIGHT,pEventStr->pFevent.pContent);	
		Scroll.lump = currentNum;
		GuiVScroll(&Scroll);
		GuiHLine(FaultEventWin.x,y+28,155,forecolor);//水平线	
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
		if(CheckFeventUpdata()){//SOE有更新
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
  *@brief 通用定值显示
  *@param  param 参数
  *@retval None
  */
void DingzhiWinFun(void *param)
{
	DZModfiyDisplay(param,&stepTab[STEP_NORMAL]);
}
/**
  *@brief 时间移动修改
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
	if(updown == 1){//减
		switch(pMoveMap[pmove])
		{
			case 1://年高
				pTime->year = pTime->year - 10;
				if(pTime->year > 99){
					pTime->year = 99;
				}break;
			case 2://年低
				if(pTime->year - 1 < 0){
					pTime->year = 99;
				}
				else{
					pTime->year = pTime->year - 1;
				}break;
			case 3://月高
				pTime->month = pTime->month - 10;
				if(pTime->month > 12){
					pTime->month = 12;
				}break;
			case 4://月低
				if(pTime->month - 1 < 0){
					pTime->month = 12;
				}
				else{
					pTime->month = pTime->month - 1;
				}break;
			case 5://日高
				pTime->day = pTime->day - 10;
				if(pTime->day > 31){
					pTime->day = 31;
				}break;
			case 6://日低
				if(pTime->day - 1 < 0){
					pTime->day = 31;
				}
				else{
					pTime->day = pTime->day - 1;
				}break;
			case 7://时高
				pTime->hour = pTime->hour - 10;
				if(pTime->hour > 23){
					pTime->hour = 23;
				}break;
			case 8://时低
				if(pTime->hour - 1 < 0){
					pTime->hour = 23;
				}
				else{
					pTime->hour = pTime->hour - 1;
				}break;
			case 9://分高
				pTime->min = pTime->min - 10;
				if(pTime->min > 59){
					pTime->min = 59;
				}break;
			case 10://分低
				if(pTime->min - 1 < 0){
					pTime->min = 59;
				}
				else{
					pTime->min = pTime->min - 1;
				}break;
			case 11://秒高
				pTime->s = pTime->s - 10;
				if(pTime->s > 59){
					pTime->s = 59;
				}break;
			case 12://秒低
				if(pTime->s - 1 < 0){
					pTime->s = 59;
				}
				else{
					pTime->s = pTime->s - 1;
				}break;
		}
	}
	else if(updown == 2){//加
		switch(pMoveMap[pmove])
		{
			case 1://年高
				pTime->year = pTime->year + 10;
				if(pTime->year > 99){
					pTime->year = 0;
				}break;
			case 2://年低
				if(pTime->year + 1 > 99){
					pTime->year = 0;
				}
				else{
					pTime->year = pTime->year + 1;
				}break;
			case 3://月高
				pTime->month = pTime->month + 10;
				if(pTime->month > 12){
					pTime->month = 0;
				}break;
			case 4://月低
				if(pTime->month + 1 > 12){
					pTime->month = 0;
				}
				else{
					pTime->month = pTime->month + 1;
				}break;
			case 5://日高
				pTime->day = pTime->day + 10;
				if(pTime->day > 31){
					pTime->day = 0;
				}break;
			case 6://日低
				if(pTime->day + 1 > 31){
					pTime->day = 0;
				}
				else{
					pTime->day = pTime->day + 1;
				}break;
			case 7://时高
				pTime->hour = pTime->hour + 10;
				if(pTime->hour > 23){
					pTime->hour = 0;
				}break;
			case 8://时低
				if(pTime->hour + 1 > 23){
					pTime->hour = 0;
				}
				else{
					pTime->hour = pTime->hour + 1;
				}break;
			case 9://分高
				pTime->min = pTime->min + 10;
				if(pTime->min > 59){
					pTime->min = 0;
				}break;
			case 10://分低
				if(pTime->min + 1 > 59){
					pTime->min = 0;
				}
				else{
					pTime->min = pTime->min + 1;
				}break;
			case 11://秒高
				pTime->s = pTime->s + 10;
				if(pTime->s > 59){
					pTime->s = 0;
				}break;
			case 12://秒低
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
  *@brief 时间修改处理函数
  *@param  None
  *@retval None
  */
static void TimeModfiyFun(void *param)
{
	static uint8_t *pTimeStr;
	static SystemTimeDisplay *pTime;
	static uint8_t pMove,tpMove,timeStrEnd,upDown;
	if(stepTab[STEP_NORMAL] == 0){//初始化，分配内存
		pTimeStr = &userGUIBuff[0];
		pTime = (SystemTimeDisplay *)&userGUIBuff[32];
		GetDisplayTime(pTime);
		sprintf((char *)pTimeStr,"20%02d-%02d-%02d  %02d:%02d:%02d",pTime->year,\
			pTime->month,pTime->day,pTime->hour,pTime->min,pTime->s);
		pMove = 2;/* 定位到年的第三位,前两位不可修改 */
		tpMove = 2;
		timeStrEnd = strlen((char *)pTimeStr) - 1;
		upDown = 0; 
		stepTab[STEP_NORMAL] = 1;
	}
	if(stepTab[STEP_NORMAL] == 1){//画图
		WINDOW *pWin = &TimeModfiyWin;
		GuiHPointLine(pWin->x,pWin->y + pWin->hight - 18,pWin->x + pWin->wide - 1,2,forecolor);
		GuiRPointLine(pWin->x + 40,pWin->y + pWin->hight - 18,pWin->y + pWin->hight - 1,2,forecolor);
		GuiFont12Align(pWin->x + 2,pWin->y + pWin->hight - 15,37,FONT_LEFT,"状态:");
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
		GuiFont12Align(pWin->x + 42,pWin->y + pWin->hight - 15,pWin->wide - 44,FONT_RIGHT,"修改中...");
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
		while(1){//左右移动 屏蔽无效字符
			if(pTimeStr[pMove] < '0' || pTimeStr[pMove] > '9'){
				if(tflag == 1){//右移
					if(++pMove > timeStrEnd){
						pMove = 2;
					}
				}
				else{
					if(--pMove < 2){//左移
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
		pWin->wide - 44,FONT_RIGHT,"修改成功");
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
  *@brief 定值切换窗口函数
  *@param  param 参数
  *@retval None
  */
void DzhiChangeFun(void *param)
{
	if(stepTab[STEP_DZHICHANGE] == 0){
		GuiFont12Align(DzhiChangeWin.x + 2,DzhiChangeWin.y + 30,\
			DzhiChangeWin.wide - 4,FONT_MID,"定值切换成功");
		GuiFont12Align(DzhiChangeWin.x + 2,DzhiChangeWin.y + 45,\
			DzhiChangeWin.wide - 4,FONT_MID,"请按任意键退出");
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
  *@brief 检测当前菜单节点是否需要密码
  *@param  菜单ID
  *@retval 菜单密码类型 -1 不需要密码 >= 0 密码类型
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
  *@brief  用户节点信息控制
  *@param  菜单ID
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
  *@brief  菜单项处理
  *@param  step 步骤 pNode 菜单节点
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
	if(*step == 2){//等待密码窗口返回
		if(passWordState == 1){//密码正确
			*step = 3;
		}
		//如果密码不正确，密码窗口退出时，由刺激取消将步骤清零
	}
	if(*step == 3){//菜单处理
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
  *@brief  菜单控制函数
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
  *@brief  设置定值菜单子节点
  *@param  None
  *@retval None
  */
void SetDzhiSonNode(lkdMenuNode *pSon)
{
	MenuTreeSNodeInsert(&Node5,pSon);
	MenuTreeSNodeInsert(&Node2,pSon);
}

/**
  *@brief  定值菜单结构变化回调
  *@param  None
  *@retval None
  */
void DzhiChangeCallBack(void)
{
	DingzhiChangeFlag = 1;
}

/* END */
