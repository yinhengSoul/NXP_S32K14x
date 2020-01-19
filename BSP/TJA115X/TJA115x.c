/*
 * Copyright (c) 2007 - 2018, NXP Semiconductors B.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * o Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in
 *   the documentation and/or other materials provided with the
 *   distribution.
 *
 * o Neither the name of NXP Semiconductors B.V. nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*_____ I N C L U D E S ____________________________________________________ */

#include <stdlib.h>
#include "TJA115x.h"
#include "TJA_CanDrv.h"
#include "osif.h"
#include "TJA115X_Cfg.h"


#define delay_ms(x)   OSIF_TimeDelay(x);

/*_____ L O C A L  F U N C T I O N S _______________________________________ */

/**
 * @brief Transmit message to update command register
 * @param canid Configuration message CAN ID
 * @param byte Command byte value
 * @param reg Command register value
 * @param len Command register len in bytes
 * @return TJA115x_NOERROR on success or TJA115x_ERR_COMMAND on
 * verification failure
 */
static TJA115x_ErrorCode_t  tja115x_update_command(uint32_t canid,
                                                   uint8_t byte,
                                                   uint32_t reg,
                                                   uint8_t len) {
  TJA115x_ErrorCode_t   retval = TJA115x_NOERROR;
  uint8_t   i;
  uint8_t   payload[8];

  /* Prepare command payload: command byte and register parameters */
  payload[0] = byte;
  for (i=1; i<=len; i++) {
    payload[i] = (uint8_t)(0xFF & (reg >> (8*(len-i))));
  }
    
  TJACAN_transmit_extended(canid, 1+len, payload);
  if (!CAN_acknowledged()) {
    retval = TJA115x_ERR_COMMAND;
  }
  return retval;
}

/**
 * @brief Transmit message command to leave configuration mode
 * @param canid Configuration message CAN ID
 * @param mode Leave configuration mode (@see TJA115x_LeaveMode)
 * @return TJA115x_NOERROR on success or TJA115x_ERR_LEAVE on command
 * ACK failure
 */
static TJA115x_ErrorCode_t  tja115x_leave(uint32_t canid,
                                          uint8_t mode) {
  TJA115x_ErrorCode_t   retval = TJA115x_NOERROR;
  uint8_t payload[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

  /* Apply command to close configuration window (if present) */
  payload[1] |= mode & 0x80;
  payload[0] = mode & 0x7F;
  
  TJACAN_transmit_extended(canid, 8, payload);
  if (!CAN_acknowledged()) {
    retval = TJA115x_ERR_LEAVE;
  }
  return retval;
}

/**
 * @brief Transmit provided configuration description
 * @param cfg Pointer to allocated and configured configuration description
 * @param canid CAN ID of the configuration message command
 * @return TJA115x_NOERROR on success or another TJA115x_ErrorCode
 */
static TJA115x_ErrorCode_t  tja115x_configure(TJA115x_ConfigDesc_t *cfg,
                                              uint32_t canid,
                                              uint8_t len) {
  TJA115x_ErrorCode_t retval = TJA115x_NOERROR;
  uint8_t   i, payload[8];

  for (i=0; i<len && i<TJA115x_ELEMENTS_COUNT && retval==TJA115x_NOERROR; i++) {
    /* Prepare command payload: command byte and register parameters */
    payload[0] = (uint8_t)0x10;
    payload[1] = (uint8_t)i;
    payload[2] = (uint8_t)(0xFF & (cfg->elements[i] >> 24));
    payload[3] = (uint8_t)(0xFF & (cfg->elements[i] >> 16));
    payload[4] = (uint8_t)(0xFF & (cfg->elements[i] >> 8));
    payload[5] = (uint8_t)(0xFF & (cfg->elements[i] >> 0));
    TJACAN_transmit_extended(canid, 6, payload);

    /* Use first element to check TJA115x configuation mode */
    if (i==0) {
      payload[0] |= 0x80;           /* Verification with
                                       forced/expected failure */
      payload[2] ^= (uint8_t)0xFF;  /* Force failure with unexpected value */
      payload[3] ^= (uint8_t)0xFF;  /* Force failure with unexpected value */
      TJACAN_transmit_extended(canid, 6, payload);
      if (CAN_acknowledged()) {
        retval = TJA115x_ERR_MODE;
        break;
      }
      /* Restore expected value for following sucessful verification */
      payload[2] ^= (uint8_t)0xFF;
      payload[3] ^= (uint8_t)0xFF;
    }
    
    payload[0] |= 0x80; /* Register verification */
    TJACAN_transmit_extended(canid, 6, payload);
    if (!CAN_acknowledged()) {
      retval = TJA115x_ERR_ELEMENT;
    }
  }

  /* Update command 0x40 register */
  if (retval==TJA115x_NOERROR) {
    retval = tja115x_update_command(canid, 0x40, cfg->cmd40, 1);
  }
  
  /* Update command 0x50 register */
  if (retval==TJA115x_NOERROR) {
    retval = tja115x_update_command(canid, 0x50, cfg->cmd50, 2);
  }

  /* Update command 0x60 register */
  if (retval==TJA115x_NOERROR) {
    retval = tja115x_update_command(canid, 0x60, cfg->cmd60, 4);
  }
  
  return retval;
}

/*_____ P U B L I C  F U N C T I O N S _____________________________________ */

/* See TJA115x.h for details */
TJA115x_ErrorCode_t TJA115x_init(TJA115x_ConfigDesc_t *cfg) {
  uint8_t   i;

  /* Disable all filters */
  for (i=0; i<TJA115x_ELEMENTS_COUNT; i++) {
    cfg->elements[i] = 0u;
  }

  /* Align to defaults */
  cfg->cmd40 =                                                  \
    TJA115x_CMD40_TMPE(TJA115x_CMD40_TMPE_DIS) |                \
    TJA115x_CMD40_FLDTH(TJA115x_CMD40_FLDTH_DEFAULT) |          \
    TJA115x_CMD40_FLDMAX(TJA115x_CMD40_FLDMAX_DEFAULT) |        \
    TJA115x_CMD40_FLDL(TJA115x_CMD40_FLDL_DEFAULT);
  
  /* Align to defaults */
  cfg->cmd50 =                                                          \
    TJA115x_CMD50_CONFIG_EN_TO(TJA115x_CMD50_CONFIG_EN_TO_DEFAULT) |    \
    TJA115x_CMD50_BBLM(TJA115x_CMD50_BBLM_DEFAULT) |                    \
    TJA115x_CMD50_LGEF(TJA115x_CMD50_LGEF_DEFAULT) |                    \
    TJA115x_CMD50_SEC_TO(TJA115x_CMD50_SEC_TO_DEFAULT) |                \
    TJA115x_CMD50_CAN_SP(TJA115x_CMD50_CAN_SP_DEFAULT) |                \
    TJA115x_CMD50_CANFD_SP(TJA115x_CMD50_CANFD_SP_DEFAULT) |            \
    TJA115x_CMD50_CANCANFD_BR(TJA115x_CMD50_CANCANFD_BR_DEFAULT);

  /* Align to defaults */
  cfg->cmd60 =                                                  \
    TJA115x_CMD60_LCLREMn(TJA115x_CMD60_LCLREMn_DEFAULT) |      \
    TJA115x_CMD60_CONFIG_ID(TJA115x_CMD60_CONFIG_ID_DEFAULT);

  return TJA115x_NOERROR;
}

/* See TJA115x.h for details */
TJA115x_ErrorCode_t TJA115x_set_sfid1(TJA115x_ConfigDesc_t *cfg,
                                      uint8_t n,
                                      uint8_t twl,
                                      uint8_t bbl,
                                      uint16_t id) {
  TJA115x_ErrorCode_t   retval = TJA115x_ERR_INVALID;

  if (n < TJA115x_ELEMENTS_COUNT) {
    cfg->elements[n] &= ~TJA115x_CMD10_SFE_MASK1;
    cfg->elements[n] |=                           \
      TJA115x_CMD10_FET(TJA115x_CMD10_FET_STD) |  \
      TJA115x_CMD10_SFC(TJA115x_CMD10_SFC_DUAL) | \
      TJA115x_CMD10_TWL1(twl) |                   \
      TJA115x_CMD10_BBL1(bbl) |                   \
      TJA115x_CMD10_SFID1(id);

    retval = TJA115x_NOERROR;
  }
  return retval;
}

/* See TJA115x.h for details */
TJA115x_ErrorCode_t TJA115x_set_sfid2(TJA115x_ConfigDesc_t *cfg,
                                      uint8_t n,
                                      uint8_t twl,
                                      uint8_t bbl,
                                      uint16_t id) {
  TJA115x_ErrorCode_t   retval = TJA115x_ERR_INVALID;
  if (n < TJA115x_ELEMENTS_COUNT) {
    cfg->elements[n] &= ~TJA115x_CMD10_SFE_MASK2;
    cfg->elements[n] |=                           \
      TJA115x_CMD10_FET(TJA115x_CMD10_FET_STD) |  \
      TJA115x_CMD10_SFC(TJA115x_CMD10_SFC_DUAL) | \
      TJA115x_CMD10_TWL2(twl) |                   \
      TJA115x_CMD10_BBL2(bbl) |                   \
      TJA115x_CMD10_SFID2(id);

    retval = TJA115x_NOERROR;
  }
  return retval;
}

/* See TJA115x.h for details */
TJA115x_ErrorCode_t TJA115x_set_sfmask(TJA115x_ConfigDesc_t *cfg,
                                       uint8_t n,
                                       uint16_t mask) {
  TJA115x_ErrorCode_t   retval = TJA115x_ERR_INVALID;
  if (n < TJA115x_ELEMENTS_COUNT) {
    cfg->elements[n] &= ~TJA115x_CMD10_SFE_MASK2;
    cfg->elements[n] |=                              \
      TJA115x_CMD10_SFC(TJA115x_CMD10_SFC_CLASSIC) | \
      TJA115x_CMD10_SFID2(mask);
    retval = TJA115x_NOERROR;
  }
  return retval;
}

/* See TJA115x.h for details */
TJA115x_ErrorCode_t TJA115x_set_efid(TJA115x_ConfigDesc_t *cfg,
                                     uint8_t n,
                                     uint8_t efc,
                                     uint32_t id) {
  TJA115x_ErrorCode_t   retval = TJA115x_ERR_INVALID;
  if (n < TJA115x_ELEMENTS_COUNT) {
    cfg->elements[n] &= ~TJA115x_CMD10_SFE_MASK2;
    cfg->elements[n] |=                             \
      TJA115x_CMD10_FET(TJA115x_CMD10_FET_EXT) |    \
      TJA115x_CMD10_EFC(efc) |                      \
      TJA115x_CMD10_EFID(id);

    retval = TJA115x_NOERROR;
  }
  return retval;
}

/* See TJA115x.h for details */
TJA115x_ErrorCode_t TJA115x_set_efmask(TJA115x_ConfigDesc_t *cfg,
                                       uint8_t n,
                                       uint32_t mask) {
  TJA115x_ErrorCode_t   retval = TJA115x_ERR_INVALID;
  if (n < TJA115x_ELEMENTS_COUNT) {
    cfg->elements[n] &= ~TJA115x_CMD10_SFE_MASK2;
    cfg->elements[n] |=                     \
      TJA115x_CMD10_FET(TJA115x_CMD10_FET_EXT) |        \
      TJA115x_CMD10_EFC(TJA115x_CMD10_EFC_MASK) |       \
      TJA115x_CMD10_EFID(mask);

    retval = TJA115x_NOERROR;
  }
  return retval;
}

/* See TJA115x.h for details */
TJA115x_ErrorCode_t TJA115x_set_tmpe(TJA115x_ConfigDesc_t *cfg,
                                     uint8_t tmpe) {
  cfg->cmd40 &= ~TJA115x_CMD40_TMPE(0xFFu);
  cfg->cmd40 |= TJA115x_CMD40_TMPE(tmpe);
  return TJA115x_NOERROR;
}

/* See TJA115x.h for details */
TJA115x_ErrorCode_t TJA115x_set_flooding(TJA115x_ConfigDesc_t *cfg,
                                         uint8_t fldl,
                                         uint8_t fldmax,
                                         uint8_t fldth) {
  cfg->cmd40 &= ~(TJA115x_CMD40_FLDTH(0xFFu) |
                  TJA115x_CMD40_FLDMAX(0xFFu) |
                  TJA115x_CMD40_FLDL(0xFFu));
  cfg->cmd40 |= TJA115x_CMD40_FLDL(fldl);
  cfg->cmd40 |= TJA115x_CMD40_FLDMAX(fldmax);
  cfg->cmd40 |= TJA115x_CMD40_FLDTH(fldth);
  return TJA115x_NOERROR;
}

/* See TJA115x.h for details */
TJA115x_ErrorCode_t TJA115x_set_config_en_to(TJA115x_ConfigDesc_t *cfg,
                                             uint8_t config_en_to) {
  cfg->cmd50 &= ~TJA115x_CMD50_CONFIG_EN_TO(0xFFu);
  cfg->cmd50 |= TJA115x_CMD50_CONFIG_EN_TO(config_en_to);
  return TJA115x_NOERROR;
}

/* See TJA115x.h for details */
TJA115x_ErrorCode_t TJA115x_set_reporting(TJA115x_ConfigDesc_t *cfg,
                                          uint8_t bblm,
                                          uint8_t lgef) {
  cfg->cmd50 &= ~(TJA115x_CMD50_BBLM(0xFFu) |
                  TJA115x_CMD50_LGEF(0xFFu));
  cfg->cmd50 |= TJA115x_CMD50_BBLM(bblm);
  cfg->cmd50 |= TJA115x_CMD50_LGEF(lgef);
  return TJA115x_NOERROR;
}

/* See TJA115x.h for details */
TJA115x_ErrorCode_t TJA115x_set_sec_to(TJA115x_ConfigDesc_t *cfg,
                                       uint8_t sec_to) {
  cfg->cmd50 &= ~TJA115x_CMD50_SEC_TO(0xFFu);
  cfg->cmd50 |= TJA115x_CMD50_SEC_TO(sec_to);
  return TJA115x_NOERROR;
}

/* See TJA115x.h for details */
TJA115x_ErrorCode_t TJA115x_set_bit_timing(TJA115x_ConfigDesc_t *cfg,
                                           uint8_t can_sp,
                                           uint8_t canfd_sp,
                                           uint8_t canfd_br) {
  cfg->cmd50 &= ~(TJA115x_CMD50_CAN_SP(0xFFu) |
                  TJA115x_CMD50_CANFD_SP(0xFFu) |
                  TJA115x_CMD50_CANCANFD_BR(0xFFu));
  cfg->cmd50 |= TJA115x_CMD50_CAN_SP(can_sp);
  cfg->cmd50 |= TJA115x_CMD50_CANFD_SP(canfd_sp);
  cfg->cmd50 |= TJA115x_CMD50_CANCANFD_BR(canfd_br);
  return TJA115x_NOERROR;
}

/* See TJA115x.h for details */
TJA115x_ErrorCode_t TJA115x_set_lclremn(TJA115x_ConfigDesc_t *cfg,
                                        uint8_t lclremn) {
  cfg->cmd60 &= ~TJA115x_CMD60_LCLREMn(0xFFu);
  cfg->cmd60 |= TJA115x_CMD60_LCLREMn(lclremn);
  return TJA115x_NOERROR;
}

/* See TJA115x.h for details */
TJA115x_ErrorCode_t TJA115x_set_config_id(TJA115x_ConfigDesc_t *cfg,
                                          uint32_t config_id) {
  cfg->cmd60 &= ~TJA115x_CMD60_CONFIG_ID(0xFFFFFFFFu);
  cfg->cmd60 |= TJA115x_CMD60_CONFIG_ID(config_id);
  return TJA115x_NOERROR;
}

/* See TJA115x.h for details */
TJA115x_ErrorCode_t TJA115x_reconfigure_local(TJA115x_ConfigDesc_t *cfg,
                                              uint32_t canid,
                                              uint8_t len,
                                              TJA115x_LeaveMode_t leavemode) {
  TJA115x_ErrorCode_t   retval = TJA115x_NOERROR;

  /* Request local configuration: enter configuration mode */
  TJACAN_transmit_extended(canid, 0, NULL);

  /* Perform the configuration */
  retval = tja115x_configure(cfg, canid, len);

  /* Leave configuration mode */
  if (retval==TJA115x_NOERROR) {
    retval = tja115x_leave(canid, leavemode);
  }
  return retval;
}

/* See TJA115x.h for details */
TJA115x_ErrorCode_t TJA115x_reconfigure_remote(TJA115x_ConfigDesc_t *cfg,
                                               uint32_t canid,
                                               uint8_t len,
                                               TJA115x_LeaveMode_t leavemode) {
  TJA115x_ErrorCode_t   retval = TJA115x_NOERROR;

  /* Request remote configuration: enter configuration mode */
  TJACAN_transmit_extended(canid, 0, NULL);

  /* Check if canid is permitted, hence part of TWL? */
  if (CAN_acknowledged()) {
    /* Wait required delay... for confirmation */
    delay_ms(110); /* Upper datasheet limit +10% */

    /* Confirm remote configuration */
    TJACAN_transmit_extended(canid, 0, NULL);

    /* Check if canid is permitted on the bus, hence not part of a
     * BBL? */
    if (CAN_acknowledged()) {
      /* Perform configuration */
      retval = tja115x_configure(cfg, canid, len);

      /* Leave configuration mode */
      if (retval==TJA115x_NOERROR) {
        retval = tja115x_leave(canid, leavemode);
      }
    } else {
      retval = TJA115x_ERR_REMOTE2;
    }
  } else {
    retval = TJA115x_ERR_REMOTE1;
  }
  return retval;
}

/* See TJA115x.h for details */
TJA115x_ErrorCode_t TJA115x_configure_vanilla(TJA115x_ConfigDesc_t *cfg,
                                              uint32_t canid,
                                              uint8_t len,
                                              TJA115x_LeaveMode_t leavemode) {
  TJA115x_ErrorCode_t   retval = TJA115x_ERR_BAUDRATE;
  uint8_t   i;

  /* Baudrate auto-detection */
  for (i=0; i<2 && retval != TJA115x_NOERROR; i++) {
	  TJACAN_transmit(0x555, 0, NULL);
    if (CAN_acknowledged()) {
      retval = TJA115x_NOERROR;
    }
  }

  /* Perform configuration */
  if (retval==TJA115x_NOERROR) {
    retval = tja115x_configure(cfg, canid, len);
  }

  /* Leave configuration mode */
  if (retval==TJA115x_NOERROR) {
    retval = tja115x_leave(canid, leavemode);
  }
  return retval;
}




//static TJA115x_ErrorCode_t RullerTableSetElement(uint8 id,)
/*
*********************************************************************************************************
*                                      
* Description:  User  function
*
* Arguments  : 
*              
*             
* Returns    :  USE ELEMENT Num
*
* Notes: Must Start RTOS
*             
*********************************************************************************************************
*/
TJA115x_ErrorCode_t ConfigTJA115X()
{
	TJA115X_RulerTableType RulerTable[Table_Ruler_Num]={WhiteAnBlakeTable};
	TJA115x_ConfigDesc_t Tja115x_Cfg;
	TJA115x_ErrorCode_t TRet;
	uint8 i;
	uint8 ElementNum=0;
	uint8 TWL;
	uint8 TBL;
	uint32 ID;
	uint8 sel_levmode = 0;
	TJA115x_LeaveMode_t LevMode;
	TRet = TJA115x_init(&Tja115x_Cfg);
	//Config Table
	for(i=0;i<Table_Ruler_Num;i++)
	{
		if(RulerTable[i].FrameFormat == 0)
		{
			TWL = RulerTable[i].TWBL&0x1;
			TBL = (RulerTable[i].TWBL>>1)&0x1;
			ID =  RulerTable[i].CANID;
			TRet = TJA115x_set_sfid1(&Tja115x_Cfg,ElementNum,TWL,TBL,(uint16)ID);
			if(TRet != TJA115x_NOERROR)
				break;
			if((RulerTable[i].STD2Mask&0xf)==1)
			{
				ID = RulerTable[i].CanMaskID;
				TRet = TJA115x_set_sfmask(&Tja115x_Cfg,ElementNum,(uint16)ID);
				if(TRet != TJA115x_NOERROR)
					break;
			}
			else if((RulerTable[i].STD2Mask&0x30)!=0)
			{
				TWL = (RulerTable[i].STD2Mask>>4)&0x1;
				TBL = (RulerTable[i].STD2Mask>>5)&0x1;
				ID =  RulerTable[i].CanMaskID;
				TRet = TJA115x_set_sfid2(&Tja115x_Cfg,ElementNum,TWL,TBL,(uint16)ID);
				if(TRet != TJA115x_NOERROR)
					break;
			}
			ElementNum++;
		}
		else if(RulerTable[i].FrameFormat == 1)
		{
			if(RulerTable[i].STD2Mask == 1)
			{
				ID = RulerTable[i].CanMaskID;
				TRet = TJA115x_set_efmask(&Tja115x_Cfg,ElementNum,ID);
				if(TRet != TJA115x_NOERROR)
					break;
				ElementNum++;
				ID = RulerTable[i].CANID;
				if(RulerTable[i].TWBL == 0)
				{
					TRet = TJA115x_ERR_ELEMENT;
						break;
				}
				TRet = TJA115x_set_efid(&Tja115x_Cfg,ElementNum,RulerTable[i].TWBL,ID);
				if(TRet != TJA115x_NOERROR)
					break;
				ElementNum++;
			}
			else
			{
				ID = RulerTable[i].CANID;
				TRet = TJA115x_set_efid(&Tja115x_Cfg,ElementNum,RulerTable[i].TWBL,ID);
				if(TRet != TJA115x_NOERROR)
					break;
				ElementNum++;
			}
		}
	}
	//ConfigSetting
	if(TRet == TJA115x_NOERROR)
	{
		TRet = TJA115x_set_tmpe(&Tja115x_Cfg,TMPE);
#if FloodProtectionEn		
		TRet = TJA115x_set_flooding(&Tja115x_Cfg,FLDTH,FLDMAX,FLDL);
#endif
		TRet = TJA115x_set_config_en_to(&Tja115x_Cfg,CONFIG_EN_TO);
		TRet = TJA115x_set_reporting(&Tja115x_Cfg,BBLM,LGEF);
		TRet = TJA115x_set_sec_to(&Tja115x_Cfg,SEC_TO);
		TRet = TJA115x_set_bit_timing(&Tja115x_Cfg,CAN_SP,CANFD_SP,CAN_CANFD_BR);
		TRet = TJA115x_set_config_id(&Tja115x_Cfg,CONFIGID);
		if(TRet == TJA115x_NOERROR)
		{
			sel_levmode = LeavingConfigMode;
			if(sel_levmode == 0)
				LevMode = TJA115x_LEAVE_VOLATILE;
			else if(sel_levmode == 1)
				LevMode = TJA115x_LEAVE_OPEN_LOCK;
//			else if(sel_levmode == 2)
//				LevMode = TJA115x_LEAVE_FULL_LOCK;

			{
				TRet = TJA115x_configure_vanilla(&Tja115x_Cfg,CONFIGID,ElementNum,LevMode);
				if(TRet == TJA115x_ERR_BAUDRATE)
					TRet = TJA115x_reconfigure_local(&Tja115x_Cfg,CONFIGID,ElementNum,LevMode);
			}
		}
	}
	return 	TRet;
}






/* End of file: TJA115x.c */
