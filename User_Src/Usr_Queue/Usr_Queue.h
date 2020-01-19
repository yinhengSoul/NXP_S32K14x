// ------------------------------------------------------------------------------------------------------
// Copyright (C), 2016 - 2018, LIHUA Tech. Co., Ltd.
// ------------------------------------------------------------------------------------------------------
// 文件名		: Usr_Queue.h
// ------------------------------------------------------------------------------------------------------
// 作者			: yinheng
// ------------------------------------------------------------------------------------------------------
// 版本号		: 0.0.1
// ------------------------------------------------------------------------------------------------------
// 创建日期		: 22016/1/30
// ------------------------------------------------------------------------------------------------------
// 描述			: 数据队列模块源文件, 包含了对数据队列创建、读、写等操作.
// ------------------------------------------------------------------------------------------------------
// 函数列表		: 1. QListInit			数据队列控制模块池链表初始化函数
//				  2. QCreate			数据队列创建函数  
//				  3. QDel				数据队列删除函数
//				  3. QRead				数据队列读操作函数
//				  3. QWrite				数据队列以FIFO方式写操作函数
//				  4. QWriteFront		数据队列以LIFO方式写操作函数
//				  5. QGetUseData		数据队列读已有数据个数函数
//				  6. QGetMaxData		数据队列读数据空间大小函数
//				  7. QFlush				数据队列清空操作函数
// ------------------------------------------------------------------------------------------------------

#ifndef _Usr_Queue_H_
#define _Usr_Queue_H

#ifdef __cplusplus
extern "C" {
#endif// End of #ifdef __cplusplus

#include "Std_Types.h"
#include "FreeRTOS.h"
#include "task.h"

#define QUEUE_NO_ERR            200u    // 数据队列返回操作成功
#define QUEUE_FULL_ERR          201u    // 数据队列返回队列已满
#define QUEUE_EMPTY_ERR         202u    // 数据队列返回队列已空
#define QUEUE_PARA_ERR			203u	// 数据队列返回参数错误

/*
*********************************************************************************************************
*                                            FIFO配置宏定义
*********************************************************************************************************
*/
#define Q_MAX_QUEUES				   10u		/* 数据队列控制模块最大个数, 它决定了数据队列控制模块池的大小 */

#define Q_EN						    1		/* 禁止(0)或允许(1)数据队列                                   */
#define Q_EN_DEL					  	1		/* 禁止(0)或允许(1)删除数据队列函数QDel()                     */
#define Q_EN_WRITE						1		/* 禁止(0)或允许(1)FIFO发送数据函数QWrite()                   */
#define Q_EN_WRITE_FRONT			    0		/* 禁止(0)或允许(1)LIFO发送数据函数QWriteFront()              */
#define Q_EN_USE_DATA					1		/* 禁止(0)或允许(1)取得数据队列数据个数函数QGetUseData()      */
#define Q_EN_MAX_DATA					1		/* 禁止(0)或允许(1)取得数据队列空间大小函数QGetMaxData()      */
#define Q_EN_FLUSH						1		/* 禁止(0)或允许(1)清空队列函数QFlush()                       */


typedef struct q_dcb
{
	struct q_dcb *QPtr;					// 数据队列控制模块链表指针

    unsigned char		 *QOut;					// 数据队列数据空间输出位置
    unsigned char		 *QIn;				    // 数据队列数据空间输入位置
    unsigned char		 *QEnd;		            // 数据队列数据空间的结束位置

    unsigned char		 *QDataBuf;            	// 数据数据的空间指针

    unsigned int 		  QUseData;             // 数据队列已有数据个数记录变量
    unsigned int		  QMaxData;             // 数据队列数据空间大小记录变量
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
