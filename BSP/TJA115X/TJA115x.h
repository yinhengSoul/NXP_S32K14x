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

/**
 * @file TJA115x.h
 *
 *  Created on: Nov 1, 2018
 *     @author: Th. Walrant
 *
 *     TWa,  2019-06-20:    Add TJA115x_VERSION_xxx
 */

#ifndef _TJA115x_H_
#define _TJA115x_H_

/*_____ I N C L U D E S ____________________________________________________ */

#include <stdint.h>
#include "TJA115x_regs.h"

/*_____ D E F I N I T I O N S ______________________________________________ */

#define	TJA115x_VERSION_MAJOR	1
#define	TJA115x_VERSION_MINOR	1
#define	TJA115x_VERSION_STR		"1.01"

#define TJA115x_CMD10_SFE_MASK1 (TJA115x_CMD10_SFC_RMASK |  \
                 TJA115x_CMD10_TWL1_RMASK | \
                 TJA115x_CMD10_BBL1_RMASK | \
                 TJA115x_CMD10_SFID1_RMASK)
#define TJA115x_CMD10_SFE_MASK2 (TJA115x_CMD10_SFC_RMASK |  \
                 TJA115x_CMD10_TWL2_RMASK | \
                 TJA115x_CMD10_BBL2_RMASK | \
                 TJA115x_CMD10_SFID2_RMASK)

/*_____ T Y P E D E F S ____________________________________________________ */

/**
 * @brief Error codes
 */
typedef enum TJA115x_ErrorCode {
  TJA115x_NOERROR = 0,  /*! No error - successful operation */ 
  TJA115x_ERR_BASE = -128,  /*! Error code base - unused */
  TJA115x_ERR_INVALID,  /*! Invalid parameter(s) */
  TJA115x_ERR_BAUDRATE, /*! Failure during vanilla baudrate
                          detection */
  TJA115x_ERR_REMOTE1,  /*! Failure to perform or initiate remote
                          configuration */ 
  TJA115x_ERR_REMOTE2,  /*! Failure to confirm remote configuration
                          request */ 
  TJA115x_ERR_MODE,     /*! Failure to confirm configuration mode,
                          i.e.: TJA115x is not in expected mode */
  TJA115x_ERR_ELEMENT,  /*! Filter element register verification
                          failure */
  TJA115x_ERR_COMMAND,  /*! Command (non-FET) register verification
                          failure */
  TJA115x_ERR_LEAVE     /*! Failure to leave configuration mode */
} TJA115x_ErrorCode_t;

/**
 * @brief Leave configuration modes
 */
typedef enum TJA115x_LeaveMode {
  TJA115x_LEAVE_VOLATILE = 0x71,    /*! Development mode without lock,
                                      NO MTP programming. */
  TJA115x_LEAVE_OPEN_LOCK = 0x72,   /*! Programming configuration into
                                      nonvolatile and set "open lock
                                      bit" */
  TJA115x_LEAVE_FULL_LOCK = 0x74,   /*! Programming configuration into
                                      nonvolatile and set "full lock
                                      bit" */
  TJA115x_CLOSE_VOLATILE = 0xF1,    /*! Same as TJA115x_LEAVE_VOLATILE
                                      with immediate close of
                                      configuration window */
  TJA115x_CLOSE_OPEN_LOCK = 0xF2,   /*! Same as
                                      TJA115x_LEAVE_OPEN_LOCK with
                                      immediate close of configuration
                                      window */
  TJA115x_CLOSE_FULL_LOCK = 0xF4    /*! Same as
                                      TJA115x_LEAVE_FULL_LOCK with
                                      immediate close of configuration
                                      window */
} TJA115x_LeaveMode_t;

/**
 * @brief Configuration description
 */
typedef struct TJA115x_ConfigDesc {
  uint32_t  elements[TJA115x_ELEMENTS_COUNT];
  uint32_t  cmd40;
  uint32_t  cmd50;
  uint32_t  cmd60;
  
} TJA115x_ConfigDesc_t;



/*_____ P U B L I C  F U N C T I O N S _____________________________________ */

/**
 * @brief Initialize configuration description with default values.
 * @param cfg Pointer to allocated configuration description
 * @return TJA115x_NOERROR
 */
TJA115x_ErrorCode_t TJA115x_init(TJA115x_ConfigDesc_t *cfg);

/**
 * @brief Set 1st ID for standard dual or classical filter element
 * @param cfg Pointer to allocated configuration description
 * @param n Filter element number for command 0x10
 * @param twl Standard filter TWL configuration for command 0x10
 * @param bbl Standard filter BBL configuration for command 0x10
 * @param id Standard filter element CAN ID for command 0x10
 * @return TJA115x_NOERROR on success or TJA115x_ERR_INVALID is wrong
 * element number
 * @caution Void any previous call to TJA115x_set_sfmask() on same
 * element
 */
TJA115x_ErrorCode_t TJA115x_set_sfid1(TJA115x_ConfigDesc_t *cfg,
                                      uint8_t n,
                                      uint8_t twl,
                                      uint8_t bbl,
                                      uint16_t id);

/**
 * @brief Set 2nd ID for standard dual filter element
 * @param cfg Pointer to allocated configuration description
 * @param n Filter element number for command 0x10
 * @param twl Standard filter TWL configuration for command 0x10
 * @param bbl Standard filter BBL configuration for command 0x10
 * @param id Standard filter element CAN ID for command 0x10
 * @return TJA115x_NOERROR on success or TJA115x_ERR_INVALID is wrong
 * element number
 * @caution Void any previous call to TJA115x_set_sfmask() on same
 * element
 */
TJA115x_ErrorCode_t TJA115x_set_sfid2(TJA115x_ConfigDesc_t *cfg,
                                      uint8_t n,
                                      uint8_t twl,
                                      uint8_t bbl,
                                      uint16_t id);

/**
 * @brief Set mask for standard classical filter element
 * @param cfg Pointer to allocated configuration description
 * @param n Filter element number for command 0x10
 * @param mask Standard filter element mask for command 0x10
 * @return TJA115x_NOERROR on success or TJA115x_ERR_INVALID is wrong
 * element number
 * @caution Must be called after TJA115x_set_sfid1() on same element
 */
TJA115x_ErrorCode_t TJA115x_set_sfmask(TJA115x_ConfigDesc_t *cfg,
                                       uint8_t n,
                                       uint16_t mask);

/**
 * @brief Set ID for extended filter element
 * @param cfg Pointer to allocated configuration description
 * @param n Filter element number for command 0x10
 * @param efc Extended filter configuration for command 0x10
 * @param id Extended filter element CAN ID for command 0x10
 *
 * @return TJA115x_NOERROR on success or TJA115x_ERR_INVALID is wrong
 * element number
 */
TJA115x_ErrorCode_t TJA115x_set_efid(TJA115x_ConfigDesc_t *cfg,
                                     uint8_t n,
                                     uint8_t efc,
                                     uint32_t id);

/**
 * @brief Set mask for extended filter element
 * @param cfg Pointer to allocated configuration description
 * @param n Filter element number for command 0x10
 * @param mask Extended filter element mask for command 0x10
 *
 * @return TJA115x_NOERROR on success or TJA115x_ERR_INVALID is wrong
 * element number
 */
TJA115x_ErrorCode_t TJA115x_set_efmask(TJA115x_ConfigDesc_t *cfg,
                                       uint8_t n,
                                       uint32_t mask);

/**
 * @brief Set the tampering protection (part of command 0x40) in the
 * config description
 *
 * @param cfg Pointer to allocated configuration description
 * @param tmpe Tamper protectin enable for command 0x40
 * @return TJA115x_NOERROR
 */
TJA115x_ErrorCode_t TJA115x_set_tmpe(TJA115x_ConfigDesc_t *cfg,
                                     uint8_t tmpe);

/**
 * @brief Set the Flooding parameters (part of command 0x40) in the
 * config description
 *
 * @param cfg Pointer to allocated configuration description
 * @param fldl Flooding load for command 0x40
 * @param fldmax Maximum Flooding bucket limit for command 0x40
 * @param fldth Flooding threshold limit for command 0x40
 * @return TJA115x_NOERROR
 */
TJA115x_ErrorCode_t TJA115x_set_flooding(TJA115x_ConfigDesc_t *cfg,
                                         uint8_t fldl,
                                         uint8_t fldmax,
                                         uint8_t fldth);

/**
 * @brief Set the Configuration Enable Timeout (part of command 0x50)
 * in the config description
 *
 * @param cfg Pointer to allocated configuration description
 * @param config_en_to CONFIG_EN_TO for command 0x50
 * @return TJA115x_NOERROR
 */
TJA115x_ErrorCode_t TJA115x_set_config_en_to(TJA115x_ConfigDesc_t *cfg,
                                             uint8_t config_en_to);

/**
 * @brief Set the IDS/Reporting parameters (part of command 0x50) in
 * the config description
 *
 * @param cfg Pointer to allocated configuration description
 * @param bblm Bus Black List mask enable, allowing local CAN node to
 * receive the original blacklisted ID
 * @param lgef Selection of the Error Frame length for secure functions
 * @return TJA115x_NOERROR
 */
TJA115x_ErrorCode_t TJA115x_set_reporting(TJA115x_ConfigDesc_t *cfg,
                                          uint8_t bblm,
                                          uint8_t lgef);

/**
 * @brief Set the Secure Timeout (part of command 0x50) in the config
 * description
 *
 * @param cfg Pointer to allocated configuration description
 * @param sec_to Secure timeout for command 0x50
 * @return TJA115x_NOERROR
 */
TJA115x_ErrorCode_t TJA115x_set_sec_to(TJA115x_ConfigDesc_t *cfg,
                                       uint8_t sec_to);

/**
 * @brief Set CAN FD bit timing (part of command 0x50) in the config
 * description
 *
 * @param cfg Pointer to allocated configuration description
 * @param can_sp Sample point Classical CAN / CAN FD arbitration
 * @param canfd_sp Sample point DATA PHASE
 * @param canfd_br CAN/CANFD bitrate
 * @return TJA115x_NOERROR
 */
TJA115x_ErrorCode_t TJA115x_set_bit_timing(TJA115x_ConfigDesc_t *cfg,
                                           uint8_t can_sp,
                                           uint8_t canfd_sp,
                                           uint8_t canfd_br);

/**
 * @brief Set the Local or Remote reconfiguration permission (part of
 * command 0x60) in the config description
 *
 * @param cfg Pointer to allocated configuration description
 * @param lclremn LCL/REMn value for command 0x60
 * @return TJA115x_NOERROR
 */
TJA115x_ErrorCode_t TJA115x_set_lclremn(TJA115x_ConfigDesc_t *cfg,
                                        uint8_t lclremn);

/**
 * @brief Set the CONFIG_ID (part of command 0x60) in the config
 * description
 *
 * @param cfg Pointer to allocated configuration description
 * @param config_id CONFIG_ID for command 0x60
 * @return TJA115x_NOERROR
 */
TJA115x_ErrorCode_t TJA115x_set_config_id(TJA115x_ConfigDesc_t *cfg,
                                          uint32_t config_id);

/**
 * @brief Perform initial vanilla configuration
 *
 * @param cfg Pointer to allocated and configured configuration description
 * @param canid CAN ID of the configuration message command
 * @param len Number of filter elememts to configure
 * @param leavemode Specifies how to leave configuration mode (@see
 * TJA115x_LeaveMode)
 * @return TJA115x_NOERROR on success or another TJA115x_ErrorCode
 */
TJA115x_ErrorCode_t TJA115x_configure_vanilla(TJA115x_ConfigDesc_t *cfg,
                                              uint32_t canid,
                                              uint8_t len,
                                              TJA115x_LeaveMode_t leavemode);

/**
 * @brief Perform remote reconfiguration
 * @param cfg Pointer to allocated and configured configuration description
 * @param canid CAN ID of the configuration message command
 * @param len Number of filter elememts to configure
 * @param leavemode Specifies how to leave configuration mode (@see
 * TJA115x_LeaveMode)
 * @return TJA115x_NOERROR on success or another TJA115x_ErrorCode
 */
TJA115x_ErrorCode_t TJA115x_reconfigure_remote(TJA115x_ConfigDesc_t *cfg,
                                               uint32_t canid,
                                               uint8_t len,
                                               TJA115x_LeaveMode_t leavemode);

/**
 * @brief Perform local reconfiguration
 * @param cfg Pointer to allocated and configured configuration description
 * @param canid CAN ID of the configuration message command
 * @param len Number of filter elememts to configure
 * @param leavemode Specifies how to leave configuration mode (@see
 * TJA115x_LeaveMode)
 * @return TJA115x_NOERROR on success or another TJA115x_ErrorCode
 */
TJA115x_ErrorCode_t TJA115x_reconfigure_local(TJA115x_ConfigDesc_t *cfg,
                                              uint32_t canid,
                                              uint8_t len,
                                              TJA115x_LeaveMode_t leavemode);

TJA115x_ErrorCode_t ConfigTJA115X();


#endif /* _TJA115x_H_ */
/* End of file: TJA115x.h */
