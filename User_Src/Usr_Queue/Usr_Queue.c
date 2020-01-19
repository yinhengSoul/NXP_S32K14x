// ------------------------------------------------------------------------------------------------------
// Copyright (C), 2016 - 2018, LIHUA Tech. Co., Ltd.
// ------------------------------------------------------------------------------------------------------
// 文件名		: Queue.c
// ------------------------------------------------------------------------------------------------------
// 作者			: yinheng
// ------------------------------------------------------------------------------------------------------
// 版本号		: 0.0.1
// ------------------------------------------------------------------------------------------------------
// 创建日期		: 2016/1/30
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
// 历史修改:
// ------------------------------------------------------------------------------------------------------
#include "Usr_Queue.h"

#define OS_ENTER_CRITICAL   taskENTER_CRITICAL
#define OS_EXIT_CRITICAL    taskEXIT_CRITICAL

static Q_DCB *QFreeList;
static Q_DCB  QCBTbl[Q_MAX_QUEUES];

// ------------------------------------------------------------------------------------------------------
// 函数名      :	QListInit
// ------------------------------------------------------------------------------------------------------
// 功能描述    :	数据队列控制模块池链表初始化. 该函数将数据队列控制模块数组QCBTbl[]初始化成一个单向链
//					表,而QFreeList始终指向队列控制模块链表的第一个未使用的队列控制模块, 这样用户就可以动
//					态的创建队列.可使用的队列个数由宏Q_MAX_QUEUES决定.
// ------------------------------------------------------------------------------------------------------
// 调用模块    :	无
// ------------------------------------------------------------------------------------------------------
// 输入参数    :	无
// ------------------------------------------------------------------------------------------------------
// 返回参数    :	无
// ------------------------------------------------------------------------------------------------------
// 其他        :	
// ------------------------------------------------------------------------------------------------------
#if (Q_EN > 0)
void QListInit (void)
{
#if OS_CRITICAL_METHOD == 3                         
    OS_CPU_SR  cpu_sr;
#endif

#if (Q_MAX_QUEUES > 1)						// 如果数据队列控制模块池的个数大于1, 则需要将数据队列初始化
											// 成一个单向链表
	INT16U  i;

	Q_DCB  *p_q_1;
	Q_DCB  *p_q_2;	

	OS_ENTER_CRITICAL();  //关闭全局中断
	
	// 对数据队列控制模块数组清零
	for (i = 0; i < sizeof(QCBTbl); i++)
	{
		((INT8U *)&QCBTbl[0])[i] = 0;
	}
	
	p_q_1 = &QCBTbl[0];
	p_q_2 = &QCBTbl[1];
	
	// 链接数据队列控制模块池中的成员, 其链接指针为数据队列控制模块域中的->QPtr, 并初始化相关变量
	for (i = 0; i < (Q_MAX_QUEUES - 1); i++)
	{
		p_q_1->QPtr     = p_q_2;

		p_q_1++;
		p_q_2++;
	}
	
	// 数据队列控制模块池中最后一个成员的链接指针为空
	p_q_1->QPtr     = (Q_DCB *)0;
	
	// QFreeList为一个全局变量, 始终指向数据队列控制模块池中的第一个未使用的成员
	QFreeList = &QCBTbl[0];

	OS_EXIT_CRITICAL();   //打开全局中断
#else										// 数据队列控制模块池的个数只有1个
	OS_ENTER_CRITICAL();

	QFreeList           = &QCBTbl[0];
	QFreeList->QPtr     = (Q_DCB *)0;

	OS_EXIT_CRITICAL();
#endif// End of #if (Q_MAX_QUEUES > 1)
}
#endif// End of #if (Q_EN > 0)

// ------------------------------------------------------------------------------------------------------
// 函数名      :	QCreate
// ------------------------------------------------------------------------------------------------------
// 功能描述    :	数据队列创建. 该函数将从数据队列控制模块数组QCBTbl[]中取出一个未使用的成员, 并将其从
//					数据队列链表池中取出. 根据传入的数据缓冲区首指针、大小和数据类型对当前的数据队列控制
//					模块进行初始化. 初始化完毕后, 该队列变成一个非环形队列, 其边界控制及其数据指针均由数据
//					队列控制模块统一管理.
// ------------------------------------------------------------------------------------------------------
// 调用模块    :	无
// ------------------------------------------------------------------------------------------------------
// 输入参数    :	1. uc_q_buf			数据队列数据缓冲区首指针
//					2. ui_q_size		数据队列数据缓冲区大小
// ------------------------------------------------------------------------------------------------------
// 返回参数    :	1. != (Q_DCB *)0	合法数据队列控制模块指针, 创建队列成功
//					2. == (Q_DCB *)0	非法数据队列控制模块指针, 创建队列失败
// ------------------------------------------------------------------------------------------------------
// 其他        :	
// ------------------------------------------------------------------------------------------------------
#if (Q_EN > 0)
Q_DCB *QCreate (INT8U *uc_q_buf, INT16U ui_q_size)
{
#if OS_CRITICAL_METHOD == 3                         
    OS_CPU_SR  cpu_sr;
#endif

    Q_DCB *p_q;
    	
    if (uc_q_buf != (INT8U *)0)											// 数据队列数据缓冲区指针合法性
    {    
		OS_ENTER_CRITICAL();
		
		// 从数据队列控制模块池中取出一个未使用的成员
		p_q = QFreeList;												// 取出数据队列控制模块池中第一个
																		// 未使用的成员
		if (QFreeList != (Q_DCB *)0)									// 不是数据队列控制模块池的最后一
																		// 个成员
		{
			QFreeList = QFreeList->QPtr;								// 调整QFreeList指针指向数据队列
																		// 控制模块池的下一个成员
		}

		OS_EXIT_CRITICAL();

		if (p_q != (Q_DCB *)0)											// 分配的数据队列控制模块指针合法
		{			
			p_q->QMaxData = ui_q_size;									// 初始化数据队列空间大小						
			p_q->QUseData = 0;											// 清零数据队列数据个数变量
						
			p_q->QDataBuf = uc_q_buf;									// 将数据队列数据缓冲区挂入数据队
																		// 列控制模块
			// 初始化数据队列相关控制指针
			p_q->QOut     = p_q->QDataBuf;								
			p_q->QIn      = p_q->QDataBuf;

			p_q->QEnd = p_q->QDataBuf + p_q->QMaxData;
			
			p_q->QPtr = (Q_DCB *)0;										// 断开当前数据队列控制模块与数据
																		// 队列控制模块池的链接

		}// End of if (p_q != (Q_DCB *)0)			      				
    }// End of if (pv_q_buf != (void *)0)

	return p_q;
}
#endif// End of #if (Q_EN > 1)

// ------------------------------------------------------------------------------------------------------
// 函数名      :	QDel
// ------------------------------------------------------------------------------------------------------
// 功能描述    :	数据队列删除. 该函数将需要删除的数据队列控制模块重新放入队列控制模块链表池中.
// ------------------------------------------------------------------------------------------------------
// 调用模块    :	无
// ------------------------------------------------------------------------------------------------------
// 输入参数    :	1. p_q				数据队列控制模块指针
//					2. uc_err			数据队列错误代码指针, 它包含以下值:
//					   Q_NO_ERR			代表调用此函数成功, 数据队列被成功删除
//					   Q_PARA_ERR		代表传入参数错误, 在该函数中表示数据队列控制模块指针是一个非法指针
// ------------------------------------------------------------------------------------------------------
// 返回参数    :	1. != (Q_DCB *)0	合法数据队列控制模块指针, 删除队列失败
//					2. == (Q_DCB *)0	非法数据队列控制模块指针, 删除队列成功
// ------------------------------------------------------------------------------------------------------
// 其他        :	
// ------------------------------------------------------------------------------------------------------
#if (Q_EN > 0) && (Q_EN_DEL > 0)
Q_DCB *QDel (Q_DCB *p_q, INT8U *uc_err)
{
#if OS_CRITICAL_METHOD == 3                         
    OS_CPU_SR  cpu_sr;
#endif

	if (p_q == (Q_DCB *)0)					// 数据队列数据缓冲区指针非性
	{
		*uc_err = QUEUE_PARA_ERR;

		return p_q;
	}

	OS_ENTER_CRITICAL();
	
	// 将当前数据队列控制模块重新放入数据队列控制模块池中
	p_q->QPtr = QFreeList;					
	QFreeList = p_q;

	OS_EXIT_CRITICAL();

	*uc_err = QUEUE_NO_ERR;

	return ((Q_DCB *)0);
}
#endif// End of #if (Q_EN > 1) && (Q_EN_DEL > 0)

// ------------------------------------------------------------------------------------------------------
// 函数名      :	QRead
// ------------------------------------------------------------------------------------------------------
// 功能描述    :	数据队列读取. 该函数根据出入的参数p_q->DataType判断数据队列的数据类型, 然后根据不同的
//					类型调整数据指针, 并读出相应的数据.
// ------------------------------------------------------------------------------------------------------
// 调用模块    :	无
// ------------------------------------------------------------------------------------------------------
// 输入参数    :	1. p_q				数据队列控制模块指针
//					2. uc_q_read_buf	接收数据的变量指针
// ------------------------------------------------------------------------------------------------------
// 返回参数    :	1. Q_NO_ERR			代表调用此函数成功, 数据队列读取数据成功
//					2. Q_EMPTY_ERR		代表调用此函数成功, 但数据队列为空
//					3. Q_PARA_ERR		代表传入参数错误, 在该函数中表示数据队列控制模块指针或者接收数据
//										的变量指针是一个非法指针
// ------------------------------------------------------------------------------------------------------
// 其他        :	
// ------------------------------------------------------------------------------------------------------
#if (Q_EN > 0)
INT8U QRead (Q_DCB *p_q, INT8U *uc_q_read_buf)
{
#if OS_CRITICAL_METHOD == 3                         
    OS_CPU_SR  cpu_sr;
#endif

    INT8U   uc_err;

    if (p_q != (Q_DCB *)0 && uc_q_read_buf != (INT8U *)0)			// 数据队列控制模块和数据读出变量指针
																	// 合法性
    {
		OS_ENTER_CRITICAL();

		
		if (p_q->QUseData > 0)										// 当前数据队列有数据
		{
			*(uc_q_read_buf) = *(p_q->QOut);

			p_q->QOut++;
			if (p_q->QOut >= p_q->QEnd)
			{
				p_q->QOut = p_q->QDataBuf;
			}						
			
			p_q->QUseData--;										// 减少数据队列已有数据个数

			uc_err = QUEUE_NO_ERR;
		}
		else														// 队列没有任何数据
		{
			uc_err = QUEUE_EMPTY_ERR;
		}

		OS_EXIT_CRITICAL();
	}// End of if (p_q != (Q_DCB *)0 && pv_q_read_buf != (void *)0)
	else
	{
		uc_err = QUEUE_PARA_ERR;
	}

    return uc_err;
}
#endif// End of #if (Q_EN > 1)

// ------------------------------------------------------------------------------------------------------
// 函数名      :	QWrite
// ------------------------------------------------------------------------------------------------------
// 功能描述    :	数据队列写入(FIFO方式). 该函数根据出入的参数p_q->DataType判断数据队列的数据类型, 然后
//					根据不同的类型调整数据指针, 并写入相应的数据.
// ------------------------------------------------------------------------------------------------------
// 调用模块    :	无
// ------------------------------------------------------------------------------------------------------
// 输入参数    :	1. p_q				数据队列控制模块指针
//					2. pv_q_write_buf	写入数据的变量指针
// ------------------------------------------------------------------------------------------------------
// 返回参数    :	1. Q_NO_ERR			代表调用此函数成功, 数据队列写入数据成功
//					2. Q_FULL_ERR		代表调用此函数成功, 但数据队列已满
//					3. Q_PARA_ERR		代表传入参数错误, 在该函数中表示数据队列控制模块指针或者写入数据
//										的变量指针是一个非法指针
// ------------------------------------------------------------------------------------------------------
// 其他        :	 一次只能存储一字节的长度
// ------------------------------------------------------------------------------------------------------
#if (Q_EN > 0) && (Q_EN_WRITE > 0)
INT8U QWrite (Q_DCB *p_q, INT8U *uc_q_write_buf)
{
#if OS_CRITICAL_METHOD == 3                         
    OS_CPU_SR  cpu_sr;
#endif

    INT8U   uc_err;

    if (p_q != (Q_DCB *)0 && uc_q_write_buf != (INT8U *)0)			// 数据队列控制模块和数据写入变量指针
																	// 合法性
    {
//		OS_ENTER_CRITICAL();
		if (p_q->QUseData < p_q->QMaxData)							// 数据队列未满
		{		
			*(p_q->QIn) = *(uc_q_write_buf);
				
			p_q->QIn++;
			if (p_q->QIn >= p_q->QEnd)
			{
				p_q->QIn = p_q->QDataBuf;
			}				
			
			p_q->QUseData++;										// 增加数据队列已有数据个数

			uc_err = QUEUE_NO_ERR;
		}// End of if (p_q->QUseData < p_q->QMaxData)
		else														// 数据队列已满
		{
			uc_err = QUEUE_FULL_ERR;
		}       		
//		OS_EXIT_CRITICAL();
    }// End of if (p_q != (Q_DCB *)0 && pv_q_write_buf != (void *)0)
	else
    {
        uc_err = QUEUE_PARA_ERR;
    }

    return uc_err;
}
#endif// End of #if (Q_EN > 1) && (Q_EN_WRITE > 0)

// ------------------------------------------------------------------------------------------------------
// 函数名      :	QWriteFront
// ------------------------------------------------------------------------------------------------------
// 功能描述    :	数据队列写入(LIFO方式). 该函数根据出入的参数p_q->DataType判断数据队列的数据类型, 然后
//					根据不同的类型调整数据指针, 并写入相应的数据.
// ------------------------------------------------------------------------------------------------------
// 调用模块    :	无
// ------------------------------------------------------------------------------------------------------
// 输入参数    :	1. p_q				数据队列控制模块指针
//					2. pv_q_write_buf	写入数据的变量指针
// ------------------------------------------------------------------------------------------------------
// 返回参数    :	1. Q_NO_ERR			代表调用此函数成功, 数据队列写入数据成功
//					2. Q_FULL_ERR		代表调用此函数成功, 但数据队列已满
//					3. Q_PARA_ERR		代表传入参数错误, 在该函数中表示数据队列控制模块指针或者写入数据
//										的变量指针是一个非法指针
// ------------------------------------------------------------------------------------------------------
// 其他        :	
// ------------------------------------------------------------------------------------------------------
#if (Q_EN > 0) && (Q_EN_WRITE_FRONT > 0)
INT8U QWriteFront (Q_DCB *p_q, INT8U *uc_q_write_buf)
{
#if OS_CRITICAL_METHOD == 3                         
    OS_CPU_SR  cpu_sr;
#endif

    INT8U   uc_err;	    

    if (p_q != (Q_DCB *)0 && uc_q_write_buf != (void *)0)           // 数据队列控制模块和数据写入变量指针
																	// 合法性 
    {
		OS_ENTER_CRITICAL();

		if (p_q->QUseData < p_q->QMaxData)							// 数据队列未满
		{
			p_q->QOut--;
			if (p_q->QOut < p_q->QDataBuf)
			{
				p_q->QOut = p_q->QEnd - 1;
			}
				
			*(p_q->QOut) = *(uc_q_write_buf);											
						
			p_q->QUseData++;										// 增加数据队列已有数据个数
			
			uc_err = QUEUE_NO_ERR;
		}// End of if (p_q != (Q_DCB *)0 && pv_q_write_buf != (void *)0)
		else														// 数据队列已满
		{
			uc_err = QUEUE_FULL_ERR;
		}
        
		OS_EXIT_CRITICAL();
		
		uc_err = QUEUE_NO_ERR;
    }// End of if (p_q != (Q_DCB *)0 && pv_q_write_buf != (void *)0)
	else
    {
        uc_err = QUEUE_PARA_ERR;
    }

    return uc_err;
}
#endif// End of #if (Q_EN > 1) && (Q_EN_WRITE_FRONT > 0)

// ------------------------------------------------------------------------------------------------------
// 函数名      :	QGetUseData
// ------------------------------------------------------------------------------------------------------
// 功能描述    :	获得当前队列已有的数据个数. 数据队列控制模块指针中的->QUseData域记录了当前数据队列数
//					据的个数.
// ------------------------------------------------------------------------------------------------------
// 调用模块    :	无
// ------------------------------------------------------------------------------------------------------
// 输入参数    :	1. p_q				数据队列控制模块指针
//					2. pui_q_use_data	读出当前数据队列已有数据个数的变量指针
// ------------------------------------------------------------------------------------------------------
// 返回参数    :	1. Q_NO_ERR			代表调用此函数成功, 读出数据队列已有数据个数成功
//					2. Q_PARA_ERR		代表传入参数错误, 在该函数中表示数据队列控制模块指针或者当前数据
//										队列已有数据个数的变量指针是一个非法指针
// ------------------------------------------------------------------------------------------------------
// 其他        :	
// ------------------------------------------------------------------------------------------------------
#if (Q_EN > 0) && (Q_EN_USE_DATA > 0)
INT8U QGetUseData (Q_DCB *p_q, INT16U *pui_q_use_data)
{   
#if OS_CRITICAL_METHOD == 3                         
    OS_CPU_SR  cpu_sr;
#endif

	INT8U  uc_err;

    if (p_q != (Q_DCB *)0 && pui_q_use_data != (void *)0)			// 数据队列控制模块和数据队列已有数据
																	// 个数变量指针合法性
    {
		OS_ENTER_CRITICAL();

		*pui_q_use_data = p_q->QUseData;							// 从数据队列控制模块中取出记录

		OS_EXIT_CRITICAL();
        
		uc_err = QUEUE_NO_ERR;
    }// End of if (p_q != (Q_DCB *)0 && pui_q_use_data != (void *)0)
	else
    {
        uc_err = QUEUE_PARA_ERR;
    }

    return uc_err;
}
#endif// End of #if (Q_EN > 1) && (Q_EN_USE_DATA > 0)

// ------------------------------------------------------------------------------------------------------
// 函数名      :	QGetMaxData
// ------------------------------------------------------------------------------------------------------
// 功能描述    :	获得当前队列数据空间大小. 数据队列控制模块指针中的->QMaxData域记录了当前数据队列数据
//					空间大小.
// ------------------------------------------------------------------------------------------------------
// 调用模块    :	无
// ------------------------------------------------------------------------------------------------------
// 输入参数    :	1. p_q				数据队列控制模块指针
//					2. pui_q_max_data	读出当前数据队列数据空间大小的变量指针
// ------------------------------------------------------------------------------------------------------
// 返回参数    :	1. Q_NO_ERR			代表调用此函数成功, 读出数据队列数据空间大小成功
//					2. Q_PARA_ERR		代表传入参数错误, 在该函数中表示数据队列控制模块指针或者当前数据
//										队列数据空间大小的变量指针是一个非法指针
// ------------------------------------------------------------------------------------------------------
// 其他        :	
// ------------------------------------------------------------------------------------------------------
#if (Q_EN > 0) && (Q_EN_MAX_DATA > 0)
INT8U QGetMaxData (Q_DCB *p_q, INT16U *pui_q_max_data)
{  
#if OS_CRITICAL_METHOD == 3                         
    OS_CPU_SR  cpu_sr;
#endif

	INT8U  uc_err;

    if (p_q != (Q_DCB *)0 && pui_q_max_data != (void *)0)			// 数据队列控制模块和数据队列数据空间
																	// 大小变量指针合法性
    {
//		OS_ENTER_CRITICAL();

		*pui_q_max_data = p_q->QMaxData;							// 从数据队列控制模块中取出记录

//		OS_EXIT_CRITICAL();

		uc_err = QUEUE_NO_ERR;   
    }// End of if (p_q != (Q_DCB *)0 && pui_q_max_data != (void *)0)
	else
    {
        uc_err = QUEUE_PARA_ERR;
    }

    return uc_err;
}
#endif// End of #if (Q_EN > 1) && (Q_EN_MAX_DATA > 0)

// ------------------------------------------------------------------------------------------------------
// 函数名      :	QFlush
// ------------------------------------------------------------------------------------------------------
// 功能描述    :	清除当前队列数据数据. 该函数调整数据队列控制模块的指针和相关变量为初始化状态.
// ------------------------------------------------------------------------------------------------------
// 调用模块    :	无
// ------------------------------------------------------------------------------------------------------
// 输入参数    :	1. p_q				数据队列控制模块指针
// ------------------------------------------------------------------------------------------------------
// 返回参数    :	1. Q_NO_ERR			代表调用此函数成功, 数据队列清除数据成功
//					2. Q_PARA_ERR		代表传入参数错误, 在该函数中表示数据队列控制模块指针或是一个非法
//										指针
// ------------------------------------------------------------------------------------------------------
// 其他        :	
// ------------------------------------------------------------------------------------------------------
#if (Q_EN > 0) && (Q_EN_FLUSH > 0)
INT8U QFlush (Q_DCB *p_q)
{
#if OS_CRITICAL_METHOD == 3                         
    OS_CPU_SR  cpu_sr;
#endif

    INT8U  uc_err;	
    
    if (p_q != (Q_DCB *)0)											// 数据队列控制模块指针合法性 
    {                                                           
		OS_ENTER_CRITICAL();
		
		// 调整数据队列进出数据指针为数据缓冲区的首地址
		p_q->QOut = p_q->QDataBuf;
		p_q->QIn  = p_q->QDataBuf;
		
		p_q->QUseData = 0;											// 清零数据队列已有数据个数变量 
		
		OS_EXIT_CRITICAL();

		uc_err = QUEUE_NO_ERR;
    }// End of if (p_q != (Q_DCB *)0)
	else
    {
        uc_err = QUEUE_PARA_ERR;
    }

	return uc_err;
}
#endif// End of #if (Q_EN > 1) && (Q_EN_FLUSH > 0)
