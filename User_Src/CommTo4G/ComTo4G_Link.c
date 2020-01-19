/**
* \file    c
* \brief   functionality
*
* \version V1.0
* \copyright Copyright (c) 2016 YOGOMO Motor. All rights reserved.
* \by  yinheng 
*/
/* 
 	notice:
*/
/*****************************************************************************
 Includes   <System Includes> , "Project Includes"
 *****************************************************************************/
#include "ComTo4G_Link.h"

//void UART_RxCallbackHook(void *driverState, uart_event_t event, void *userData);
//****************************************************************************
// @Macros
//****************************************************************************
#define  VendorID   0
#define  ModuleID 	0
	
#define  Sw_major_version	0
#define  Sw_minor_version   0
#define  Sw_path_version	1

/*****************************************************************************
 Typedef definitions
*****************************************************************************/



/*****************************************************************************
 Imported global variables and functions (from other files)
*****************************************************************************/
static inline uint8 ASCII_To_Hex(uint8 ASCII_Data);

/*****************************************************************************
 Exported global variables and functions (to be accessed by other files)
*****************************************************************************/


/*****************************************************************************
 Private variables and functions
*****************************************************************************/
static Q_DCB* LinkRxQueue ;

static uint8 SetFormat ;  //0:hex 1:asc

void SetComFormatASCII()
{
	SetFormat = 1;
}
void SetComFormatHex()
{
	SetFormat = 0;
}

/*
*********************************************************************************************************
*                                      
* Description: 
*
* Arguments  : 
*             
* Returns    : 
*
* Notes:
*             
*********************************************************************************************************
*/	
void ComTo4GLink_GetVersionInfo(Std_VersionInfoType *VersionInfo)
{
	VersionInfo->vendorID = VendorID;
	VersionInfo->moduleID = ModuleID;
	VersionInfo->sw_major_version = Sw_major_version;
	VersionInfo->sw_minor_version = Sw_minor_version;
	VersionInfo->sw_path_version  = Sw_path_version;
}

/*
*********************************************************************************************************
*
* Description: 
*
* Arguments  :
*
* Returns    :
*
* Notes:
*
*********************************************************************************************************
*/
static uint8 LinkRecData[LinkRecDataLenMax-1];
void ComTo4GLinkModuleInit()
{
	QListInit();
	SetFormat = 0;
	LinkRxQueue = QCreate(LinkRecData,LinkRecDataLenMax);
}

int HexToAscii(unsigned char *pHexStr,unsigned char *pAscStr,int Len)
{
	char Nibble[2];
	unsigned char Buffer[2048];
	int i = 0;
	int j = 0;

	for(i=0;i<Len;i++)
	{
		Nibble[0]=pHexStr[i] >> 4 & 0X0F;
		Nibble[1]=pHexStr[i] & 0x0F;
		for(j=0;j<2;j++)
		{
			if(Nibble[j]<10)
			{
				Nibble[j]=Nibble[j]+0x30;
			}
			else if(Nibble[j]<16)
			{
				Nibble[j]=Nibble[j]-10+'A';
			}
			else
			{
				return 0;
			}
		}
		memcpy(Buffer+i*2,Nibble,2);
	}
	Buffer[2*Len]=0x00;
	memcpy(pAscStr,Buffer,2*Len);
	pAscStr[2*Len]=0x00;
	return 1;
}

static inline uint8 ASCII_To_Hex(uint8 ASCII_Data)
{
	if((ASCII_Data>='0')&&(ASCII_Data<='9'))
		return(ASCII_Data-'0');
	else if((ASCII_Data>='A')&&(ASCII_Data<='F'))
		return(ASCII_Data-'A'+10);

	if(ASCII_Data=='S')
		return 0;
	else
		return 0xF;	//error
}


/*
*********************************************************************************************************
*
* Description: 
*
* Arguments  :
*
* Returns    : 0 :ok  1: error
*
* Notes:
*
*********************************************************************************************************
*/
uint8 ComTo4GLinkSendData(uint8* tranData ,uint16 len)
{		
	status_t Ret;
//#if COM_Asc
	if(SetFormat)
	{
		static uint8 TranDataStream_ASC[200];
		HexToAscii(tranData,TranDataStream_ASC,len);
		Ret = UART_SendData(&uart_pal1_instance,TranDataStream_ASC,len<<1);
	}
	else
//#else
		Ret = UART_SendData(&uart_pal1_instance,tranData,len);

//#endif
	return ((uint8)Ret);
}


/*
*********************************************************************************************************
*
* Description: 
*
* Arguments  :
*
* Returns    : Actual Rec Number
*
* Notes:
*
*********************************************************************************************************
*/
uint16 ComTo4GLinkRecData(uint8 *recData,uint16 len)
{
	uint16 RecDataNum = 0;  
	uint16 i;
	QGetUseData(LinkRxQueue,&RecDataNum);
//#if COM_Asc
	if(SetFormat)
	{
		uint8 recdata_h;
		uint8 recdata_l;
		if(RecDataNum>1)
		{
			if((RecDataNum>>1)<=len)
			{
				for(i=0;i<(RecDataNum>>1);i++)
				{
					QRead(LinkRxQueue,&recdata_h);
					recdata_h = ASCII_To_Hex(recdata_h);
					QRead(LinkRxQueue,&recdata_l);
					recdata_l = ASCII_To_Hex(recdata_l);
					recData[i] = (recdata_h<<4)+recdata_l;
				}
				return (RecDataNum>>1);
			}
			else
			{
				for(i=0;i<len;i++)
				{
					QRead(LinkRxQueue,&recdata_h);
					recdata_h = ASCII_To_Hex(recdata_h);
					QRead(LinkRxQueue,&recdata_l);
					recdata_l = ASCII_To_Hex(recdata_l);
					recData[i] = (recdata_h<<4)+recdata_l;
				}
				return len;
			}
		}
	}
//#else
	else
	{
		if(RecDataNum>0)
		{
			if(RecDataNum<=len)
			{
				for(i=0;i<RecDataNum;i++)
					QRead(LinkRxQueue,&(recData[i]));
				return RecDataNum;
			}
			else
			{
				for(i=0;i<len;i++)
					QRead(LinkRxQueue,&(recData[i]));
				return len;
			}
		}
	}
//#endif
	return 0;
}



/*
*********************************************************************************************************
*
* Description: 
*
* Arguments  :
*
* Returns    : 0:No Err  1:Fifo Fulled
*
* Notes: this function put into Rec Int;
*
*********************************************************************************************************
*/
uint8 ComTo4GDriverWriteData(uint8* data ,uint16 len)
{
	uint16 i;
	uint16 Max_ReserveData=0;
	QGetMaxData(LinkRxQueue,&Max_ReserveData);
	if(Max_ReserveData<len)
		return 1;
	else
	{
		for(i=0;i<len;i++)
			QWrite(LinkRxQueue,&(data[i]));
		return 0;
	}
}

void ComTo4GLinkAbortTraning()
{
	UART_AbortSendingData(&uart_pal1_instance);
}


/*
*********************************************************************************************************
*
* Description:
*
* Arguments  :
*
* Returns    :0: SUCCESS   1:BUSY  2:ABORTED  3:ERROR
*
* Notes:
*
*********************************************************************************************************
*/
uint8 ComTo4GLinkGetTranSts(uint32* bytesRemaining)
{	
	status_t Ret;
	Ret = UART_GetTransmitStatus(&uart_pal1_instance,bytesRemaining);
	if(Ret == STATUS_SUCCESS)
		return 0;
	else if(Ret == STATUS_BUSY)
		return 1;
	else if(Ret == STATUS_UART_ABORTED)
		return 2;
	else if(Ret == STATUS_ERROR)
		return 3;
	return 3;
}

void ComTo4GLinkClearRecBuffer()
{
	QFlush(LinkRxQueue);
}




