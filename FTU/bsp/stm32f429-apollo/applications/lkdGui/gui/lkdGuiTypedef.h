
#ifndef __LKDGUITYPEDEF_H
#define	__LKDGUITYPEDEF_H


/* ���ڶ��� */
struct WINDOW_
{
	int16_t x;							// ��ʼ����
	int16_t y;							// ��ʼ����
	int16_t wide;						// ���
	int16_t hight;						// �߶�
	struct WINDOW_  *fatherMenu;			// ��������
	uint8_t   *title;		// ���������ָ��(NULLΪ����ʾ)
	void(*WindowFunction)(void);	// ���ڶ�Ӧ�ķ������
};
typedef  struct WINDOW_ WINDOW;

/* ���������� */
typedef struct{
	int16_t x;
	int16_t y;
	uint16_t hight;
	uint16_t max;
	uint16_t lump;
}SCROLL;

#endif /* __LKDGUITYPEDEF_H */

/* END */