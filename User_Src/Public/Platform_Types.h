/*****************************************************************************/
/* Copyright (C) Yogomo Motor Corp, 2016                                     */
/*****************************************************************************/
/* File name:       Platform_Types.h                                         */
/* Discreption:                                                              */
/* Current version: V1.0                                                     */
/* Data:            16th July, 2016                                          */
/* Comment:                                                                  */
/*****************************************************************************/
/*                             Change Record                                 */
/*---------------------------------------------------------------------------*/
/*  No.   Author    Data             Inf.                                    */
/*---------------------------------------------------------------------------*/
/*  1.0   WangYM    7th July, 2016   Initial Release.                        */
/*****************************************************************************/

#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

#define CPU_TYPE_8          8
#define CPU_TYPE_16         16
#define CPU_TYPE_32         32

#define MSB_FIRST           0
#define LSB_FIRST           1

#define HIGH_BYTE_FIRST     0
#define LOW_BYTE_FIRST      1

/* Symbols:                      */
#define CPU_TYPE            CPU_TYPE_32
#define CPU_BIT_ORDER       LSB_FIRST
#define CPU_BYTE_ORDER      LOW_BYTE_FIRST

/* Standard Autosar type define. */
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned long       uint32;
typedef unsigned long long  uint64;

typedef signed char         sint8;
typedef signed short        sint16;
typedef signed long         sint32;
typedef signed long long    sint64;

typedef unsigned long       uint8_least;
typedef unsigned long       uint16_least;
typedef unsigned long       uint32_least;

typedef signed long         sint8_least;
typedef signed long         sint16_least;
typedef signed long         sint32_least;

typedef float               float32;
typedef double              float64;

typedef unsigned char       boolean;

typedef unsigned char    INT8U;                    /* Unsigned  8 bit quantity                           */
typedef signed   char    INT8S;                    /* Signed    8 bit quantity                           */
typedef unsigned short   INT16U;                   /* Unsigned 16 bit quantity                           */
typedef signed   short   INT16S;                   /* Signed   16 bit quantity                           */
typedef unsigned long    INT32U;                   /* Unsigned 32 bit quantity                           */
typedef signed   long    INT32S;                   /* Signed   32 bit quantity                           */
typedef float            FP32;                     /* Single precision floating point                    */
typedef double           FP64;                     /* Double precision floating point                    */

typedef unsigned char   ubyte;    // 1 byte unsigned; prefix: ub 
typedef signed char     sbyte;    // 1 byte signed;   prefix: sb 
typedef unsigned short  uword;    // 2 byte unsigned; prefix: uw 
typedef signed   short  sword;    // 2 byte signed;   prefix: sw 
typedef unsigned long   ulong;    // 4 byte unsigned; prefix: ul 
typedef signed long     slong;    // 4 byte signed;   prefix: sl 
typedef unsigned short  ushort;   // 2 byte unsigned; prefix: us 
typedef signed short    sshort;   // 2 byte signed;   prefix: ss 

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE  1


#endif

#endif /* PLATFORM_TYPES_H */
