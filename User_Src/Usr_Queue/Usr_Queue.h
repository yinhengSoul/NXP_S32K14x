// ------------------------------------------------------------------------------------------------------
// Copyright (C), 2016 - 2018, LIHUA Tech. Co., Ltd.
// ------------------------------------------------------------------------------------------------------
// �ļ���		: Usr_Queue.h
// ------------------------------------------------------------------------------------------------------
// ����			: yinheng
// ------------------------------------------------------------------------------------------------------
// �汾��		: 0.0.1
// ------------------------------------------------------------------------------------------------------
// ��������		: 22016/1/30
// ------------------------------------------------------------------------------------------------------
// ����			: ���ݶ���ģ��Դ�ļ�, �����˶����ݶ��д���������д�Ȳ���.
// ------------------------------------------------------------------------------------------------------
// �����б�		: 1. QListInit			���ݶ��п���ģ��������ʼ������
//				  2. QCreate			���ݶ��д�������  
//				  3. QDel				���ݶ���ɾ������
//				  3. QRead				���ݶ��ж���������
//				  3. QWrite				���ݶ�����FIFO��ʽд��������
//				  4. QWriteFront		���ݶ�����LIFO��ʽд��������
//				  5. QGetUseData		���ݶ��ж��������ݸ�������
//				  6. QGetMaxData		���ݶ��ж����ݿռ��С����
//				  7. QFlush				���ݶ�����ղ�������
// ------------------------------------------------------------------------------------------------------

#ifndef _Usr_Queue_H_
#define _Usr_Queue_H

#ifdef __cplusplus
extern "C" {
#endif// End of #ifdef __cplusplus

#include "Std_Types.h"
#include "FreeRTOS.h"
#include "task.h"

#define QUEUE_NO_ERR            200u    // ���ݶ��з��ز����ɹ�
#define QUEUE_FULL_ERR          201u    // ���ݶ��з��ض�������
#define QUEUE_EMPTY_ERR         202u    // ���ݶ��з��ض����ѿ�
#define QUEUE_PARA_ERR			203u	// ���ݶ��з��ز�������

/*
*********************************************************************************************************
*                                            FIFO���ú궨��
*********************************************************************************************************
*/
#define Q_MAX_QUEUES				   10u		/* ���ݶ��п���ģ��������, �����������ݶ��п���ģ��صĴ�С */

#define Q_EN						    1		/* ��ֹ(0)������(1)���ݶ���                                   */
#define Q_EN_DEL					  	1		/* ��ֹ(0)������(1)ɾ�����ݶ��к���QDel()                     */
#define Q_EN_WRITE						1		/* ��ֹ(0)������(1)FIFO�������ݺ���QWrite()                   */
#define Q_EN_WRITE_FRONT			    0		/* ��ֹ(0)������(1)LIFO�������ݺ���QWriteFront()              */
#define Q_EN_USE_DATA					1		/* ��ֹ(0)������(1)ȡ�����ݶ������ݸ�������QGetUseData()      */
#define Q_EN_MAX_DATA					1		/* ��ֹ(0)������(1)ȡ�����ݶ��пռ��С����QGetMaxData()      */
#define Q_EN_FLUSH						1		/* ��ֹ(0)������(1)��ն��к���QFlush()                       */


typedef struct q_dcb
{
	struct q_dcb *QPtr;					// ���ݶ��п���ģ������ָ��

    unsigned char		 *QOut;					// ���ݶ������ݿռ����λ��
    unsigned char		 *QIn;				    // ���ݶ������ݿռ�����λ��
    unsigned char		 *QEnd;		            // ���ݶ������ݿռ�Ľ���λ��

    unsigned char		 *QDataBuf;            	// �������ݵĿռ�ָ��

    unsigned int 		  QUseData;             // ���ݶ����������ݸ�����¼����
    unsigned int		  QMaxData;             // ���ݶ������ݿռ��С��¼����
} Q_DCB;

#if (Q_EN > 0)
void QListInit (void);
#endif// End of #if (Q_EN > 0)

#if (Q_EN > 0)
Q_DCB *QCreate (INT8U *uc_q_buf, INT16U ui_q_size);
#endif// End of #if (Q_EN > 0)

#if (Q_EN > 0)
extern unsigned char QRead (Q_DCB *p_q, unsigned char *uc_q_read_buf);
#endif// End of #if (Q_EN > 0)

#if (Q_EN > 0) && (Q_EN_DEL > 0)
extern Q_DCB *QDel (Q_DCB *p_q, unsigned char *uc_err);
#endif// End of #if (Q_EN > 0) && (Q_EN_DEL > 0)

#if (Q_EN > 0) && (Q_EN_WRITE > 0)
extern unsigned char QWrite (Q_DCB *p_q, unsigned char *uc_q_write_buf);
#endif// End of #if (Q_EN > 0) && (Q_EN_WRITE > 0)

#if (Q_EN > 0) && (Q_EN_WRITE_FRONT > 0)
extern unsigned char QWriteFront (Q_DCB *p_q, unsigned char *uc_q_write_buf);
#endif// End of #if (Q_EN > 0) && (Q_EN_WRITE_FRONT > 0)

#if (Q_EN > 0) && (Q_EN_USE_DATA > 0)
extern unsigned char QGetUseData (Q_DCB *p_q, INT16U *pui_q_use_data);
#endif// End of #if (Q_EN > 0) && (Q_EN_USE_DATA > 0)

#if (Q_EN > 0) && (Q_EN_MAX_DATA > 0)
INT8U QGetMaxData (Q_DCB *p_q, INT16U *pui_q_max_data);
#endif// End of #if (Q_EN > 0) && (Q_EN_MAX_DATA > 0)

#if (Q_EN > 0) && (Q_EN_FLUSH > 0)
extern unsigned char QFlush (Q_DCB *p_q);
#endif// End of #if (Q_EN > 0) && (Q_EN_FLUSH > 0)

#ifdef __cplusplus
}
#endif// End of #ifdef __cplusplus
#endif// End of #ifndef _QUEUE_H_
