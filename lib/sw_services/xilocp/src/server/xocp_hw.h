/******************************************************************************
* Copyright (c) 2022 Xilinx, Inc.  All rights reserved.
* Copyright (C) 2022 - 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
*******************************************************************************/

/*****************************************************************************/
/**
*
* @file xocp_dme.h
* @addtogroup xil_ocpapis DME APIs
* @{
*
* @cond xocp_internal
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date       Changes
* ----- ---- --------   -------------------------------------------------------
* 1.0   vns  06/27/2022 Initial release
*
* </pre>
*
* @note
* @endcond
*
******************************************************************************/
#ifndef XOCP_HW_H
#define XOCP_HW_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "xil_io.h"
#include "xplmi_hw.h"
#include "xil_types.h"

/************************** Constant Definitions *****************************/
/**
 * Register: XOCP_PMC_GLOBAL_PMC_FW_AUTH_HASH_0
 */
#define XOCP_PMC_GLOBAL_PMC_FW_AUTH_HASH_0	(PMC_GLOBAL_BASEADDR + 0x00000750U)

/**
 * Register: XOCP_PMC_GLOBAL_PCR_0_0
 */
#define XOCP_PMC_GLOBAL_PCR_0_0			(PMC_GLOBAL_BASEADDR + 0x00005200U)
/**
 * Register: XOCP_PMC_GLOBAL_PCR_0_0
 */
#define XOCP_PMC_GLOBAL_PCR_7_0			(PMC_GLOBAL_BASEADDR + 0x00005350U)
/**
 * Register: XOCP_PMC_GLOBAL_DME_KEY_PUBLIC_X_0
 */
#define XOCP_PMC_GLOBAL_DME_KEY_PUBLIC_X_0	(PMC_GLOBAL_BASEADDR+ 0x00005400U)

/**
 * Register: XOCP_PMC_GLOBAL_DME_KEY_PUBLIC_Y_0
 */
#define XOCP_PMC_GLOBAL_DME_KEY_PUBLIC_Y_0	(PMC_GLOBAL_BASEADDR + 0x00005430U)

/**
 * Register: XOCP_PMC_GLOBAL_DME_PUBLIC_KEY_X_PARITY
 */
#define XOCP_PMC_GLOBAL_DME_PUBLIC_KEY_X_PARITY	(PMC_GLOBAL_BASEADDR + 0x00005460U)
#define XOCP_PMC_GLOBAL_DME_PUBLIC_KEY_X_PARITY_ERROR_MASK	(0x00001000U)
#define XOCP_PMC_GLOBAL_DME_PUBLIC_KEY_X_PARITY_VAL_MASK	(0x00000FFFU)

/**
 * Register: XOCP_PMC_GLOBAL_DME_PUBLIC_KEY_Y_PARITY
 */
#define XOCP_PMC_GLOBAL_DME_PUBLIC_KEY_Y_PARITY	(PMC_GLOBAL_BASEADDR + 0x00005464U)
#define XOCP_PMC_GLOBAL_DME_PUBLIC_KEY_Y_PARITY_ERROR_MASK	(0x00001000U)
#define XOCP_PMC_GLOBAL_DME_PUBLIC_KEY_Y_PARITY_VAL_MASK	(0x00000FFFU)

/**
 * Register: XOCP_PMC_GLOBAL_DME_CHALLENGE_SIGNATURE_R_0
 */
#define XOCP_PMC_GLOBAL_DME_CHALLENGE_SIGNATURE_R_0	(PMC_GLOBAL_BASEADDR + 0x00005468U)

/**
 * Register: XOCP_PMC_GLOBAL_DME_CHALLENGE_SIGNATURE_S_0
 */
#define XOCP_PMC_GLOBAL_DME_CHALLENGE_SIGNATURE_S_0	(PMC_GLOBAL_BASEADDR + 0x00005498U)

/**
 * Register: XOCP_PMC_GLOBAL_PCR_OP
 */
#define XOCP_PMC_GLOBAL_PCR_OP    		(PMC_GLOBAL_BASEADDR + 0x00011000U)
#define XOCP_PMC_GLOBAL_PCR_OP_IDX_SHIFT	(3U)

/**
 * Register: XOCP_PMC_GLOBAL_PCR_EXTEND_INPUT_0
 */
#define XOCP_PMC_GLOBAL_PCR_EXTEND_INPUT_0	(PMC_GLOBAL_BASEADDR + 0x00011004U)

/**
 * Register: XOCP_PMC_GLOBAL_PCR_OP_STATUS
 */
#define XOCP_PMC_GLOBAL_PCR_OP_STATUS		(PMC_GLOBAL_BASEADDR + 0x00011034U)
#define XOCP_PMC_GLOBAL_PCR_OP_STATUS_ERROR_MASK	(0x000003FCU)
#define XOCP_PMC_GLOBAL_PCR_OP_STATUS_PASS_MASK		(0x00000002U)
#define XOCP_PMC_GLOBAL_PCR_OP_STATUS_DONE_MASK		(0x00000001U)

/**
 * Register: XOCP_PMC_GLOBAL_DICE_CDI_SEED_0
 */
#define XOCP_PMC_GLOBAL_DICE_CDI_SEED_0		(PMC_GLOBAL_BASEADDR + 0x00011100U)

/**
 * Register: XOCP_PMC_GLOBAL_DICE_CDI_SEED_VALID
 */
#define XOCP_PMC_GLOBAL_DICE_CDI_SEED_VALID	(PMC_GLOBAL_BASEADDR + 0x00011130U)
#define XOCP_PMC_GLOBAL_DICE_CDI_SEED_VALID_VAL_MASK	(0x00000001U)

/**
 * Register: XOCP_PMC_GLOBAL_DICE_CDI_SEED_PARITY
 */
#define XOCP_PMC_GLOBAL_DICE_CDI_SEED_PARITY	(PMC_GLOBAL_BASEADDR + 0x00011134U)
#define XOCP_PMC_GLOBAL_DICE_CDI_SEED_PARITY_ERROR_SHIFT	(12U)
#define XOCP_PMC_GLOBAL_DICE_CDI_SEED_PARITY_ERROR_MASK		(0x00001000U)
#define XOCP_PMC_GLOBAL_DICE_CDI_SEED_PARITY_VAL_WIDTH   	(12U)
#define XOCP_PMC_GLOBAL_DICE_CDI_SEED_PARITY_VAL_MASK		(0x00000FFFU)

/**
 * Register: XOCP_PMC_GLOBAL_DICE_CDI_SEED_ZEROIZE_CTRL
 */
#define XOCP_PMC_GLOBAL_DICE_CDI_SEED_ZEROIZE_CTRL	(PMC_GLOBAL_BASEADDR + 0x00011138U)
#define XOCP_PMC_GLOBAL_DICE_CDI_SEED_ZEROIZE_CTRL_ZEROIZE_MASK	(0x00000001U)

/**
 * Register: XOCP_PMC_GLOBAL_DICE_CDI_SEED_ZEROIZE_STATUS
 */
#define XOCP_PMC_GLOBAL_DICE_CDI_SEED_ZEROIZE_STATUS    (PMC_GLOBAL_BASEADDR + 0x0001113CU)
#define XOCP_PMC_GLOBAL_DICE_CDI_SEED_ZEROIZE_STATUS_PASS_MASK    (0x00000002U)
#define XOCP_PMC_GLOBAL_DICE_CDI_SEED_ZEROIZE_STATUS_DONE_MASK    (0x00000001U)

/**
 * Register: XOCP_PMC_GLOBAL_DEV_IK_PRIVATE_0
 */
#define XOCP_PMC_GLOBAL_DEV_IK_PRIVATE_0    (PMC_GLOBAL_BASEADDR + 0x00011200U)

/**
 * Register: XOCP_PMC_GLOBAL_DEV_IK_PUBLIC_X_0
 */
#define XOCP_PMC_GLOBAL_DEV_IK_PUBLIC_X_0    (PMC_GLOBAL_BASEADDR + 0x00011230U)

/**
 * Register: XOCP_PMC_GLOBAL_DEV_IK_PUBLIC_Y_0
 */
#define XOCP_PMC_GLOBAL_DEV_IK_PUBLIC_Y_0    (PMC_GLOBAL_BASEADDR + 0x00011260U)

/**
 * Register: XOCP_PMC_GLOBAL_DEV_IK_PRIVATE_PARITY
 */
#define XOCP_PMC_GLOBAL_DEV_IK_PRIVATE_PARITY    (PMC_GLOBAL_BASEADDR + 0x00011290U)
#define XOCP_PMC_GLOBAL_DEV_IK_PRIVATE_PARITY_ERROR_SHIFT   (12U)
#define XOCP_PMC_GLOBAL_DEV_IK_PRIVATE_PARITY_ERROR_MASK    (0x00001000U)
#define XOCP_PMC_GLOBAL_DEV_IK_PRIVATE_PARITY_VAL_SHIFT   (0U)
#define XOCP_PMC_GLOBAL_DEV_IK_PRIVATE_PARITY_VAL_MASK    (0x00000FFFU)

/**
 * Register: XOCP_PMC_GLOBAL_DEV_IK_PRIVATE_ZEROIZE_CTRL
 */
#define XOCP_PMC_GLOBAL_DEV_IK_PRIVATE_ZEROIZE_CTRL    (PMC_GLOBAL_BASEADDR + 0x0001129CU)

/**
 * Register: XOCP_PMC_GLOBAL_DEV_IK_PRIVATE_ZEROIZE_STATUS
 */
#define XOCP_PMC_GLOBAL_DEV_IK_PRIVATE_ZEROIZE_STATUS    (PMC_GLOBAL_BASEADDR + 0x000112A0U)

/**
 * Register: XOCP_PMC_GLOBAL_NEXTSW_CDI_SEED_0
 */
#define XOCP_PMC_GLOBAL_NEXTSW_CDI_SEED_0    (PMC_GLOBAL_BASEADDR + 0x00011300U)

/**
 * Register: XOCP_PMC_GLOBAL_NEXTSW_CDI_SEED_PARITY
 */
#define XOCP_PMC_GLOBAL_NEXTSW_CDI_SEED_PARITY    (PMC_GLOBAL_BASEADDR + 0x00011330U)
#define XOCP_PMC_GLOBAL_NEXTSW_CDI_SEED_PARITY_ERROR_SHIFT   (12U)
#define XOCP_PMC_GLOBAL_NEXTSW_CDI_SEED_PARITY_ERROR_MASK    (0x00001000U)
#define XOCP_PMC_GLOBAL_NEXTSW_CDI_SEED_PARITY_VAL_WIDTH   (12U)
#define XOCP_PMC_GLOBAL_NEXTSW_CDI_SEED_PARITY_VAL_MASK    (0x00000FFFU)

/**
 * Register: XOCP_PMC_GLOBAL_NEXTSW_CDI_SEED_ZEROIZE_CTRL
 */
#define XOCP_PMC_GLOBAL_NEXTSW_CDI_SEED_ZEROIZE_CTRL    (PMC_GLOBAL_BASEADDR + 0x00011334U)
#define XOCP_PMC_GLOBAL_NEXTSW_CDI_SEED_ZEROIZE_CTRL_ZEROIZE_MASK    (0x00000001U)

/**
 * Register: XOCP_PMC_GLOBAL_NEXTSW_CDI_SEED_ZEROIZE_STATUS
 */
#define XOCP_PMC_GLOBAL_NEXTSW_CDI_SEED_ZEROIZE_STATUS    (PMC_GLOBAL_BASEADDR + 0x00011338U)
#define XOCP_PMC_GLOBAL_NEXTSW_CDI_SEED_ZEROIZE_STATUS_PASS_MASK    (0x00000002U)
#define XOCP_PMC_GLOBAL_NEXTSW_CDI_SEED_ZEROIZE_STATUS_DONE_MASK    (0x00000001U)

/**
 * Register: XOCP_PMC_GLOBAL_DEV_AK_PRIVATE_0
 */
#define XOCP_PMC_GLOBAL_DEV_AK_PRIVATE_0    (PMC_GLOBAL_BASEADDR + 0x00011340U)

/**
 * Register: XOCP_PMC_GLOBAL_DEV_AK_PUBLIC_X_0
 */
#define XOCP_PMC_GLOBAL_DEV_AK_PUBLIC_X_0    (PMC_GLOBAL_BASEADDR + 0x00011370U)

/**
 * Register: XOCP_PMC_GLOBAL_DEV_AK_PUBLIC_Y_0
 */
#define XOCP_PMC_GLOBAL_DEV_AK_PUBLIC_Y_0    (PMC_GLOBAL_BASEADDR + 0x000113A0U)

/**
 * Register: XOCP_PMC_GLOBAL_DEV_AK_PRIVATE_PARITY
 */
#define XOCP_PMC_GLOBAL_DEV_AK_PRIVATE_PARITY    (PMC_GLOBAL_BASEADDR + 0x000113D0U)
#define XOCP_PMC_GLOBAL_DEV_AK_PRIVATE_PARITY_ERROR_SHIFT   (12U)
#define XOCP_PMC_GLOBAL_DEV_AK_PRIVATE_PARITY_ERROR_MASK    (0x00001000U)
#define XOCP_PMC_GLOBAL_DEV_AK_PRIVATE_PARITY_VAL_MASK    (0x00000FFFU)

/**
 * Register: XOCP_PMC_GLOBAL_DEV_AK_PRIVATE_ZEROIZE_CTRL
 */
#define XOCP_PMC_GLOBAL_DEV_AK_PRIVATE_ZEROIZE_CTRL    (PMC_GLOBAL_BASEADDR + 0x000113DCU)
#define XOCP_PMC_GLOBAL_DEV_AK_PRIVATE_ZEROIZE_CTRL_ZEROIZE_MASK    (0x00000001U)


#define XOCP_PMC_GLOBAL_DEV_AK_PRIVATE_ZEROIZE_STATUS	(PMC_GLOBAL_BASEADDR + 0x000113E0U)

/* XPPU configurations required for DME */
#define PMC_XPPU_CTRL    		(PMC_XPPU_BASEADDR + 0x00000000U)
#define PMC_XPPU_CTRL_ENABLE_MASK    	(0x00000001U)
#define PMC_XPPU_CTRL_DEFVAL		(0U)

#define PMC_XPPU_DYNAMIC_RECONFIG_EN    (PMC_XPPU_BASEADDR + 0x000000FCU)
#define PMC_XPPU_DYNAMIC_RECONFIG_EN_DEFVAL   (0x0U)
#define PMC_XPPU_MASTER_ID00    	(PMC_XPPU_BASEADDR + 0x00000100U)
#define PMC_XPPU_MASTER_ID01    	(PMC_XPPU_BASEADDR + 0x00000104U)

#define PMC_XPPU_DYNAMIC_RECONFIG_APER_ADDR    (PMC_XPPU_BASEADDR + 0x00000150U)
#define PMC_XPPU_DYNAMIC_RECONFIG_APER_PERM    (PMC_XPPU_BASEADDR + 0x00000154U)


#define PMC_XPPU_APERPERM_017		(PMC_XPPU_BASEADDR + 0x00001044U)
#define PMC_XPPU_APERPERM_018		(PMC_XPPU_BASEADDR + 0x00001048U)
#define PMC_XPPU_APERPERM_019		(PMC_XPPU_BASEADDR + 0x0000104CU)
#define PMC_XPPU_APERPERM_020		(PMC_XPPU_BASEADDR + 0x00001050U)
#define PMC_XPPU_APERPERM_021		(PMC_XPPU_BASEADDR + 0x00001054U)
#define PMC_XPPU_APERPERM_026		(PMC_XPPU_BASEADDR + 0x00001068U)
#define PMC_XPPU_APERPERM_027		(PMC_XPPU_BASEADDR + 0x0000106CU)
#define PMC_XPPU_APERPERM_028		(PMC_XPPU_BASEADDR + 0x00001070U)
#define PMC_XPPU_APERPERM_030		(PMC_XPPU_BASEADDR + 0x00001078U)
#define PMC_XPPU_APERPERM_033		(PMC_XPPU_BASEADDR + 0x00001084U)
#define PMC_XPPU_APERPERM_032		(PMC_XPPU_BASEADDR + 0x00001080U)
#define PMC_XPPU_APERPERM_035		(PMC_XPPU_BASEADDR + 0x0000108CU)
#define PMC_XPPU_APERPERM_037		(PMC_XPPU_BASEADDR + 0x00001094U)
#define PMC_XPPU_APERPERM_038		(PMC_XPPU_BASEADDR + 0x00001098U)
#define PMC_XPPU_APERPERM_049		(PMC_XPPU_BASEADDR + 0x000010C4U)
#define PMC_XPPU_APERPERM_146		(PMC_XPPU_BASEADDR + 0x00001248U)
#define PMC_XPPU_APERPERM_147		(PMC_XPPU_BASEADDR + 0x0000124CU)
#define PMC_XPPU_APERPERM_386		(PMC_XPPU_BASEADDR + 0x00001608U)

#define PMC_XPPU_LOCK			(PMC_XPPU_BASEADDR + 0x00000020U)
#define PMC_XPPU_LOCK_DEFVAL		(0U)

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

#ifdef __cplusplus
}
#endif

#endif  /* XOCP_HW_H */

/* @} */
