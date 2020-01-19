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
#include "ComTo4G_TP.h"

#define Debug 1


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
typedef struct 
{
	uint8 TpTranSts; // 0:idle/Response Success 1:busy 2:WaitingForResp 3:Response OverTime 
	uint8 TpTranReq;
	uint8 RespTranReq;
	PDU_FrameType *DataPDU;
	PDU_FrameType *RespPDU;
	uint8* SendDataStream;
	uint16 StreamDataLen;
	uint32 SendDataNum;
	uint32 SendRespNum;
	uint32 SendSuceesNum;
}TpFrameTranCtlType;

typedef struct 
{
	uint8  DataFrameRecSts;  //0:no Frame  1: Receved 
	uint8  RespFrameRecSts;  //0:no Frame  1:Receved 
	PDU_FrameType* RespPDU;  
	PDU_FrameType* DataPDU;
	uint8* RecDataStream;
	uint32 RecDataNum;
	uint32 RecRespNum;
	uint32 RecDataNum_Suc;
}TpFrameRecCtlType;


typedef enum 
{
	T_Idle =     0,
	T_DataTx =  1,
	T_WaitingResponse = 2,
	T_OverTime = 3,
	T_RespTx =4
		
}TP_TranStsType;

typedef enum 
{
	R_Idle = 0,
	R_Receving  = 1,
	R_RecDataFrame =2,
	R_RecRespFrame =3,
	R_RecOverTime=4
}TP_RecStsType;





/*****************************************************************************
 Imported global variables and functions (from other files)
*****************************************************************************/


/*****************************************************************************
 Exported global variables and functions (to be accessed by other files)
*****************************************************************************/


/*****************************************************************************
 Private variables and functions
*****************************************************************************/

//For TpTx
static TpFrameTranCtlType TpFrameTranWord;
static TP_TranStsType TP_TranMainSts = T_Idle;


//For TpRx
static TpFrameRecCtlType  TpFrameRecWord;

//static Q_DCB* TpRxQueue ;


static uint16 CalcDataStreamLenFromPdu(PDU_FrameType pdu);
static uint16 CalcDataStreamLenFromRecDataStream(uint8* data);

static uint8 CalcCheckSumFromDataPdu(PDU_FrameType pdu);
static uint8 CalcCheckSumFromRespPdu(PDU_FrameType pdu);
static uint8 CalcCheckSumFromDataStream(uint8* data,uint32 len);

static void GetRespStream_8BitFromPdu(PDU_FrameType pdu,uint8* DataStream);
static void GetDataStream_8BitFromPdu(PDU_FrameType pdu,uint8* DataStream,uint16 *DataStreamlen);
static void GetDataPduFromStream(PDU_FrameType *pdu,uint8* DataStream);
static void GetRespPduFromStream(PDU_FrameType *pdu,uint8* DataStream);
static uint8 CheckDataStream(uint8* DataStream,uint16 len);
static uint8 JudgeRecHeader(uint8 PreDATA,uint8 CurData);



void ClearTranStatics()
{
	TpFrameTranWord.SendDataNum =0;
	TpFrameTranWord.SendRespNum = 0;
	TpFrameTranWord.SendSuceesNum =0;

}

void ClearRecStatics()
{
	TpFrameRecWord.RecDataNum = 0;
	TpFrameRecWord.RecRespNum = 0;
	TpFrameRecWord.RecDataNum_Suc = 0;
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
void ComTo4GTp_GetVersionInfo(Std_VersionInfoType *VersionInfo)
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
void ComTo4GTpModuleInit()
{
	//for Tp_Tx
	static PDU_FrameType Tran_DataPDU;
	static PDU_FrameType Tran_RespPDU;
	static uint8 sendDataStream[TpSendDataStreamLenMax];

	//for Tp_Rx
	static PDU_FrameType Rec_DataPDU;
	static PDU_FrameType Rec_RespPDU;
	static uint8 recDataField[TpRecDataStreamLenMax];
	static uint8 recDataStream[TpRecDataStreamLenMax];
	//tx
	TpFrameTranWord.TpTranReq = 0;
	TpFrameTranWord.RespTranReq =0;
	TpFrameTranWord.TpTranSts = 0;
	TpFrameTranWord.SendDataStream = sendDataStream;
	TpFrameTranWord.DataPDU = &Tran_DataPDU;
	TpFrameTranWord.RespPDU = &Tran_RespPDU;
	TpFrameTranWord.DataPDU[0].RollingCounter = 1;
	ClearTranStatics();
	
	//rx
	TpFrameRecWord.RecDataStream = recDataStream;
	TpFrameRecWord.DataPDU = &Rec_DataPDU;
	TpFrameRecWord.DataPDU[0].Datafield = recDataField;
	TpFrameRecWord.RespPDU = &Rec_RespPDU;
	ClearRecStatics();
}

void ComTo4GTpRestart()
{
	//Tp_tx
	TpFrameTranWord.TpTranReq = 0;
	TpFrameTranWord.RespTranReq =0;
	TpFrameTranWord.TpTranSts = 0;
	TP_TranMainSts = T_Idle;
	TpFrameTranWord.DataPDU[0].RollingCounter = 1;
	ComTo4GLinkAbortTraning();
	ClearTranStatics();
	
	//Tp_rx
	ClearRecStatics();
}


/*
*********************************************************************************************************
*
* Description:
*
* Arguments  :
*
* Returns    :0:idele/Response Success 1:busy 2:Tran complete 3:Response OverTime 
*
* Notes:
*
*********************************************************************************************************
*/
uint8 ComTo4GTpGetTranSts()
{
	return (TpFrameTranWord.TpTranSts);
}

void ComTo4GTpClearTranSts()
{
	TpFrameTranWord.TpTranSts = 0;
	TP_TranMainSts = R_Idle;
}



/*
*********************************************************************************************************
*
* Description:
*
* Arguments  :
*
* Returns    :0:Req Success 1: busy   2:WaitingForResp  3:Response OverTime 
*
* Notes: Rec Data from AppLayer
*
*********************************************************************************************************
*/
uint8 ComTo4GTpTranReq(uint8* data,uint16 len)
{
	if(TpFrameTranWord.TpTranSts == 0)
	{
		if(len>0)
		{
			TpFrameTranWord.DataPDU[0].StartSymbol[0] = StartSymbolValue1;
			TpFrameTranWord.DataPDU[0].StartSymbol[1] = StartSymbolValue2;
			TpFrameTranWord.DataPDU[0].FrameType = 0x5a;
			TpFrameTranWord.DataPDU[0].DataEncrptFormat = 1;
			TpFrameTranWord.DataPDU[0].DataFiledLen = len;
			TpFrameTranWord.DataPDU[0].Datafield = data;
			TpFrameTranWord.DataPDU[0].CheckSum = CalcCheckSumFromDataPdu(TpFrameTranWord.DataPDU[0]);
		}
		TpFrameTranWord.TpTranSts = 1;
		TpFrameTranWord.TpTranReq = 1;
		return 0;
	}
	else
		return 1;
}



/*
*********************************************************************************************************
*
* Description:
*
* Arguments  :
*
* Returns    :0:empty  1: success 
*
* Notes: Rec Data from AppLayer
*
*********************************************************************************************************
*/
uint8 ComTo4GTpRecDataStream(uint8* recData,uint16* len)
{
	uint16 i;
	if(TpFrameRecWord.DataFrameRecSts)
	{
		TpFrameRecWord.DataFrameRecSts = 0;
		len[0] = TpFrameRecWord.DataPDU[0].DataFiledLen;
		for(i=0;i<len[0];i++)
			recData[i] =  TpFrameRecWord.DataPDU[0].Datafield[i];
		return 1;
	}
	return 0;

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
* Notes: 2ms Clk
*
*********************************************************************************************************
*/
void ComTo4GTranTpMain()
{
	static uint8 FirstInFlag = 1;
	static uint16 TaskCount;
	static TP_TranStsType HisTranSts;
	uint8 LintTranSts;
	uint16 Templen;
	uint32 LeftDataLen;
	switch(TP_TranMainSts)
	{
		case T_Idle:
			if(TpFrameTranWord.RespTranReq == 1)
			{
				TpFrameTranWord.RespTranReq =0;
				TP_TranMainSts = T_RespTx;
				FirstInFlag = 1;
				HisTranSts = T_Idle;
			}
			else if(TpFrameTranWord.TpTranReq == 1)
			{
				TpFrameTranWord.TpTranReq = 0;
				TP_TranMainSts = T_DataTx;
				FirstInFlag = 1;
				GetDataStream_8BitFromPdu(TpFrameTranWord.DataPDU[0],TpFrameTranWord.SendDataStream,&Templen);
				ComTo4GLinkSendData(TpFrameTranWord.SendDataStream,Templen);
				if(TpFrameTranWord.SendDataNum <=0xffffffff)
					TpFrameTranWord.SendDataNum++;
			}
			break;
		case T_RespTx:
			if(FirstInFlag)
			{
				GetRespStream_8BitFromPdu(TpFrameTranWord.RespPDU[0],TpFrameTranWord.SendDataStream);
				ComTo4GLinkSendData(TpFrameTranWord.SendDataStream,5);	
				if(TpFrameTranWord.SendRespNum <=0xffffffff)
					TpFrameTranWord.SendRespNum++;
				FirstInFlag = 0;
			}
			LintTranSts = ComTo4GLinkGetTranSts(&LeftDataLen);
			if(LintTranSts == 0)
			{
				TP_TranMainSts = HisTranSts;
			}		
			break;
		case T_DataTx:
			LintTranSts = ComTo4GLinkGetTranSts(&LeftDataLen);
			if(LintTranSts == 0)
			{
				TP_TranMainSts = T_WaitingResponse;
				FirstInFlag = 1;
			}
			break;
		case T_WaitingResponse:
			if(FirstInFlag)
			{
				FirstInFlag = 0;
				TaskCount = 0;	
				TpFrameTranWord.TpTranSts = 2;
#if APU_Asc
				static uint8 huanhang[2];
				huanhang[0] = '\r';
				huanhang[1] = '\n';
				ComTo4GLinkSendData(huanhang,2);
#endif
			}
			TaskCount++;
			if(TpFrameTranWord.RespTranReq == 1)
			{
				TpFrameTranWord.RespTranReq =0;
				TP_TranMainSts = T_RespTx;
				FirstInFlag = 1;
				HisTranSts = T_WaitingResponse;		
			}
			if(TaskCount>TxWaitingOverTimeSet)
			{
				TP_TranMainSts = T_OverTime;
				FirstInFlag = 1;
			}
			if(TpFrameRecWord.RespFrameRecSts)
			{
				TpFrameRecWord.RespFrameRecSts = 0;
				if(TpFrameRecWord.RespPDU[0].RollingCounter == TpFrameTranWord.DataPDU[0].RollingCounter)
				{
					TpFrameTranWord.TpTranSts = 0;
					if(TpFrameTranWord.SendSuceesNum<=0xffffffff)
						TpFrameTranWord.SendSuceesNum++;
					if(TpFrameTranWord.DataPDU[0].RollingCounter <255)
						TpFrameTranWord.DataPDU[0].RollingCounter++;
					else
						TpFrameTranWord.DataPDU[0].RollingCounter = 0;
					TP_TranMainSts = T_Idle;
					FirstInFlag = 1;
				}
			}
			break;
		case T_OverTime:			
			if(FirstInFlag)
			{
				FirstInFlag = 0;
				TpFrameTranWord.TpTranSts = 3;
			}
			if(TpFrameTranWord.RespTranReq == 1)
			{
				TpFrameTranWord.RespTranReq =0;
				TP_TranMainSts = T_RespTx;
				FirstInFlag = 1;
				HisTranSts = T_OverTime;	
			}
#if Debug
			TP_TranMainSts = T_Idle;
			TpFrameTranWord.TpTranSts = 0;
#else
			if(TpFrameTranWord.TpTranSts ==0)
				TP_TranMainSts = T_Idle;
#endif
			break;
		default:	
			break;
	}
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
* Notes: 1ms Clk
*
*********************************************************************************************************
*/
void ComTo4GRecTpMain()
{
	static TP_RecStsType TP_RecMainSts = R_Idle;
	static uint8 FirstInFlag = 1;
	static uint8 InitData;
	static uint16 TaskCount;
	static uint8 RecData;
	static uint16 RecPduDataNum;
	static uint16 CalcPduDataNum;
	uint16 RecDataNum;
	uint8 retValue;
	
	switch(TP_RecMainSts)
	{
		case R_Idle:
			if(FirstInFlag)
			{
				FirstInFlag = 0;
			}
			RecDataNum = ComTo4GLinkRecData(&RecData,1);
			if(RecDataNum>0)
			{	
				TP_RecMainSts = R_Receving;
				FirstInFlag = 1;
			}
			break;
		case R_Receving:
			if(FirstInFlag)
			{
				FirstInFlag = 0;
				TaskCount = 0;
				InitData = RecData;
				RecPduDataNum=0;
			}
			TaskCount++;
			RecDataNum = ComTo4GLinkRecData(&RecData,1);
			if(RecDataNum>0)
			{
				TaskCount = 0;
				do
				{	
					if(RecPduDataNum==0)
					{
						retValue = JudgeRecHeader(InitData,RecData);
						InitData = RecData;
						if(retValue == TRUE)
							RecPduDataNum = 2;
					}
					else 
					{
						if(RecData == 0x5a) //RecDataFrame
						{
							TP_RecMainSts = R_RecDataFrame;
							FirstInFlag = 1;
							return;
						}
						else if(RecData == 0xaa) //RespFrame
						{
							TP_RecMainSts = R_RecRespFrame;
							FirstInFlag = 1;
							return;
						}
						else
						{
							RecPduDataNum = 0;
						}
					}
					RecDataNum = ComTo4GLinkRecData(&RecData,1);
				}while(RecDataNum>0);
			}		
			if(TaskCount>RecOverTimeSet)
			{
				TP_RecMainSts = R_RecOverTime;
				FirstInFlag = 1;
			}			
			break;
		case R_RecRespFrame:
			if(FirstInFlag)
			{
				FirstInFlag = 0;
				TpFrameRecWord.RecDataStream[0] = StartSymbolValue1;
				TpFrameRecWord.RecDataStream[1] = StartSymbolValue2;
				TpFrameRecWord.RecDataStream[2] = 0xaa;
				RecPduDataNum = 3;
				TaskCount = 0;
			}
			TaskCount++;
			RecDataNum = ComTo4GLinkRecData(&RecData,1);
			if(RecDataNum>0)
			{
				TaskCount = 0;
				do
				{	
					TpFrameRecWord.RecDataStream[RecPduDataNum] = RecData;
					RecPduDataNum ++;
					if(RecPduDataNum ==5)
					{
						retValue = CheckDataStream(TpFrameRecWord.RecDataStream,5);
						if(retValue == TRUE)
						{
							if(TpFrameRecWord.RecRespNum<=0xffffffff)
								TpFrameRecWord.RecRespNum++;
							GetRespPduFromStream(TpFrameRecWord.RespPDU,TpFrameRecWord.RecDataStream);
							TpFrameRecWord.RespFrameRecSts = 1;
						}
						TP_RecMainSts = R_Idle;
						FirstInFlag = 1;
						return;
					}
					RecDataNum = ComTo4GLinkRecData(&RecData,1);
				}while(RecDataNum>0);
			}				
			if(TaskCount>RecOverTimeSet)
			{
				TP_RecMainSts = R_RecOverTime;
				FirstInFlag = 1;
			}
			break;
		case R_RecDataFrame:
			if(FirstInFlag)
			{
				FirstInFlag = 0;
				TpFrameRecWord.RecDataStream[0] = StartSymbolValue1;
				TpFrameRecWord.RecDataStream[1] = StartSymbolValue2;
				TpFrameRecWord.RecDataStream[2] = 0x5a;
				RecPduDataNum = 3;
				TaskCount = 0;
				CalcPduDataNum =0;
				if(TpFrameRecWord.RecDataNum<=0xffffffff)
					TpFrameRecWord.RecDataNum++;
			}
			TaskCount++;
			RecDataNum = ComTo4GLinkRecData(&RecData,1);
			if(RecDataNum>0)
			{
				TaskCount = 0;
				do
				{	
					TpFrameRecWord.RecDataStream[RecPduDataNum] = RecData;
					RecPduDataNum ++;

					if(CalcPduDataNum>0)
					{
						if(RecPduDataNum == CalcPduDataNum)	
						{
							retValue = CheckDataStream(TpFrameRecWord.RecDataStream,RecPduDataNum);
							if(retValue == TRUE)
							{
								if(TpFrameRecWord.RecDataNum_Suc<=0xffffffff)
									TpFrameRecWord.RecDataNum_Suc++;
								//modify
								GetDataPduFromStream(TpFrameRecWord.DataPDU,TpFrameRecWord.RecDataStream);
								
								TpFrameRecWord.DataFrameRecSts = 1;
								
								//Req Resp Frame								
								TpFrameTranWord.RespPDU[0].StartSymbol[0] = StartSymbolValue1;
								TpFrameTranWord.RespPDU[0].StartSymbol[1] = StartSymbolValue2;
								TpFrameTranWord.RespPDU[0].FrameType = 0xaa;
								TpFrameTranWord.RespPDU[0].RollingCounter = TpFrameRecWord.DataPDU[0].RollingCounter;
								TpFrameTranWord.RespPDU[0].CheckSum = CalcCheckSumFromRespPdu(TpFrameTranWord.RespPDU[0]);
								TpFrameTranWord.RespTranReq= 1;
							}

							TP_RecMainSts = R_Idle;
							FirstInFlag = 1;
							return;
						}
					}
					else 
					{
						if(RecPduDataNum>=7)
							CalcPduDataNum = CalcDataStreamLenFromRecDataStream(TpFrameRecWord.RecDataStream);
					}
					RecDataNum = ComTo4GLinkRecData(&RecData,1);
				}while(RecDataNum>0);
			}				
			if(TaskCount>RecOverTimeSet)
			{
				TP_RecMainSts = R_RecOverTime;
				FirstInFlag = 1;
			}
			break;
		case R_RecOverTime:
				TP_RecMainSts = R_Idle;
				FirstInFlag = 1;
			break;
		default:
			break;

	}
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
static uint16 CalcDataStreamLenFromPdu(PDU_FrameType pdu)
{
	return (StartSymbolNum + 5 + pdu.DataFiledLen + 1);
}

static void GetDataStream_8BitFromPdu(PDU_FrameType pdu,uint8* DataStream,uint16 *DataStreamlen)
{
	uint16 Templen;
	uint16 i;
	uint16 Datalen = 0;
	Templen = CalcDataStreamLenFromPdu(pdu);
	*DataStreamlen = Templen;
	
	for(i=0;i<StartSymbolNum;i++)
	{
		*(DataStream+Datalen) = pdu.StartSymbol[i];
		Datalen++;
	}
	*(DataStream+Datalen) = pdu.FrameType;
	Datalen++;
	*(DataStream+Datalen) = pdu.RollingCounter;
	Datalen++;
	*(DataStream+Datalen) = pdu.DataEncrptFormat;
	Datalen++;
	*(DataStream+Datalen) = pdu.DataFiledLen>>8;
	Datalen++;
	*(DataStream+Datalen) = pdu.DataFiledLen&0xff;
	Datalen++;
	for(i=0;i<pdu.DataFiledLen;i++)
	{
		*(DataStream+Datalen) = pdu.Datafield[i];
		Datalen++;
	}
	*(DataStream+Datalen) = pdu.CheckSum;
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
static void GetRespStream_8BitFromPdu(PDU_FrameType pdu,uint8* DataStream)
{

	uint16 i;
	uint16 Datalen = 0;
	
	for(i=0;i<StartSymbolNum;i++)
	{
		*(DataStream+Datalen) = pdu.StartSymbol[i];
		Datalen++;
	}
	*(DataStream+Datalen) = pdu.FrameType;
	Datalen++;
	*(DataStream+Datalen) = pdu.RollingCounter;
	Datalen++;
	*(DataStream+Datalen) = pdu.CheckSum;
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
static uint16 CalcDataStreamLenFromRecDataStream(uint8* data)
{
	uint16 datalen = data[5];
	datalen = (datalen<<8)+data[6];
	return (2+1+1+1+2+datalen+1);
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
static uint8 CalcCheckSumFromDataPdu(PDU_FrameType pdu)
{
	uint8 Result = pdu.RollingCounter;
	uint16 i;
	Result ^= pdu.FrameType;
	Result ^= pdu.DataEncrptFormat;
	Result ^= pdu.DataFiledLen>>8;
	Result ^= pdu.DataFiledLen&0xff;
	for(i=0;i<pdu.DataFiledLen;i++)
		Result ^= pdu.Datafield[i];
	return Result;
}

static uint8 CalcCheckSumFromRespPdu(PDU_FrameType pdu)
{
	uint8 Result = pdu.RollingCounter;
	Result ^= pdu.FrameType;
	return Result;
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
static uint8 CalcCheckSumFromDataStream(uint8* data,uint32 len)
{
	uint32 i;
	uint8 Result = 0;
	for(i=0;i<len-3;i++)
		Result ^= data[i+2];
	return Result;
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
static void GetDataPduFromStream(PDU_FrameType *pdu,uint8* DataStream)
{
	uint16 dataNum=0;
	uint16 i;
	uint16 Temp16;
	for(i=0;i<StartSymbolNum;i++)
	{
		pdu->StartSymbol[i] = DataStream[dataNum];
		dataNum++;
	}
	
	pdu->FrameType = DataStream[dataNum];
	dataNum++;
	pdu->RollingCounter = DataStream[dataNum];
	dataNum++;
	pdu->DataEncrptFormat = DataStream[dataNum];
	dataNum++;
	Temp16 = DataStream[dataNum];
	dataNum++;
	Temp16 = (Temp16<<8)+DataStream[dataNum];
	dataNum++;
	pdu->DataFiledLen = Temp16;
	for(i=0;i<Temp16;i++)
	{
		pdu->Datafield[i] = DataStream[dataNum];
		dataNum++;
	}
	pdu->CheckSum = DataStream[dataNum];
}

static void GetRespPduFromStream(PDU_FrameType *pdu,uint8* DataStream)
{
	uint16 dataNum=0;
	uint16 i;
	for(i=0;i<StartSymbolNum;i++)
	{
		pdu->StartSymbol[i] = DataStream[dataNum];
		dataNum++;
	}
	pdu->FrameType = DataStream[dataNum];
	dataNum++;
	pdu->RollingCounter = DataStream[dataNum];
	dataNum++;
	pdu->CheckSum = DataStream[dataNum];
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
static uint8 CheckDataStream(uint8* DataStream,uint16 len)
{
	uint8 CheckSum = CalcCheckSumFromDataStream(DataStream,len);
	if(CheckSum == DataStream[len-1])
		return TRUE;
	else
		return FALSE;
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
static uint8 JudgeRecHeader(uint8 PreDATA,uint8 CurData)
{
	if((PreDATA == StartSymbolValue1)&&(CurData ==StartSymbolValue2))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}



