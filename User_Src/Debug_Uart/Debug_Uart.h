/**
 * \file .h
 * \brief  functionality
 * \ingroup 
 *
 * \version V1.0
 * \copyright Copyright (c) 2016 YOGOMO Motor. All rights reserved.
 *
 *
 *                                 IMPORTANT NOTICE
 *
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *
 */

#ifndef Debug_Uart_H
#define Debug_Uart_H
//****************************************************************************
// @Project Includes
//****************************************************************************
#include "Std_Types.h"
#include "uart_pal.h"


#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>


//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// @Typedefs emum
//****************************************************************************



//****************************************************************************
// @Typedefs Structures
//****************************************************************************




/******************************************************************************/
/*-------------------------Global Variable Prototypes-------------------------*/
/******************************************************************************/ 
 

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/
void print(const char *sourceStr);
status_t StartReceive(uint8* buffer,uint32 len, uint32_t timeout);

void DataProceess(uint8 *data,uint8* OutOpion);



#endif
