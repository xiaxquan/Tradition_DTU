#ifndef __COMMON_H__
#define __COMMON_H__

#include <rtthread.h>


uint32_t GetTimer1Tick(void);/* ��ȡTimer1��ǰ����*/
/* ��ȡTimer1�������������ļ������*/
uint32_t GetTimer1IntervalTick(uint32_t beginTick);

void time_static_init(void); /* Timer1��ʼ��*/
void time_static_detach(void);
int8_t all_word_is_english(uint8_t *str);
uint8_t numberOfAlphabetOrNum(uint8_t *str);
#endif
