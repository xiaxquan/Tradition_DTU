/**
 * @file gooseparser.h
 * @brief 对goose的ini文件进行解析，生成相应的结构体
 * @copyright Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * @company SOJO
 * @date 2018.08.24
 *
 * @author Lee
 * @version ver 1.0
 **/


#ifndef _GOOSEPARSER_H_
#define _GOOSEPARSER_H_

#include<stdint.h>


#define MAXSIZE 40
#define TYPESIZE 8
#define APPIDSIZE 16


/*---------------------------------------------------------------------------
                                New struct types
 ---------------------------------------------------------------------------*/

/**
  @brief    struct fcda
 */
typedef struct TagFuncConDatAttr
{
    rt_uint8_t ref[MAXSIZE];
    rt_uint8_t type[TYPESIZE];
    rt_uint8_t inVarName[MAXSIZE];
    rt_uint32_t act;

}FuncConDatAttr;


/**
  @brief    struct input
 */
typedef struct TagGooseInput
{
    rt_uint32_t gocbIndex;
    rt_uint32_t gocbEntryIntex;
    rt_uint8_t ref[MAXSIZE];
    rt_uint8_t type[TYPESIZE];
    rt_uint8_t outVarName[MAXSIZE];

}GooseInput;

/**
  @brief    struct goose control block rx
 */
typedef struct TagGooseControlBlockRx
{
    rt_uint8_t addr[8];
    rt_uint32_t appid;
    rt_uint8_t gocbRef[MAXSIZE];
    rt_uint8_t appID[APPIDSIZE];
    rt_uint8_t datSet[MAXSIZE];
    rt_uint32_t confRev;
    rt_uint32_t numDatSetEntriess;

}GooseControlBlockRx;

/**
  @brief    struct goose control block tx
 */
typedef struct TagGooseControlBlockTx
{
    rt_uint8_t gocbRef[MAXSIZE];
    rt_uint8_t appID[APPIDSIZE];
    rt_uint8_t datSet[MAXSIZE];
    rt_uint32_t confRev;
    rt_uint32_t numDatSetEntriess;
    rt_uint8_t addr[8];
    rt_uint32_t priority;
    rt_uint32_t vid;
    rt_uint32_t appid;
    rt_uint32_t minTime;
    rt_uint32_t maxTime;
    FuncConDatAttr* fcda;

}GooseControlBlockTx;

/**
  @brief    struct goose Tx Message
 */
typedef struct TagGooseTxMessage
{
    rt_uint32_t numGoCb;
    GooseControlBlockTx* gocd;

}GooseTxMessage;

/**
  @brief    struct goose Rx Message
 */
typedef struct TagGooseRxMessage
{
    rt_uint32_t numGoCb;
    rt_uint32_t numInput;
    GooseControlBlockRx* gocd;
    GooseInput* input;

}GooseRxMessage;



/*---------------------------------------------------------------------------
                                Function
 ---------------------------------------------------------------------------*/
rt_uint32_t GooseIniParser(rt_uint8_t* argv, GooseTxMessage* gooseTxMessage, GooseRxMessage* gooseRxMessage);
void FreeGooseMessageMem(GooseTxMessage* gooseTxMessage, GooseRxMessage* gooseRxMessage);
void PrintGooseTxRxMessage(GooseTxMessage* gooseTxMessage, GooseRxMessage* gooseRxMessage);



#endif