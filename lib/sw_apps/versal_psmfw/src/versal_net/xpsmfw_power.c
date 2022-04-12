/******************************************************************************
* Copyright (c) 2018 - 2022 Xilinx, Inc.  All rights reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/

/*****************************************************************************/
/**
*
* @file xpsmfw_power.c
*
* This file contains power handler functions for PS Power islands and FPD
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver	Who	Date		Changes
* ---- ---- -------- ------------------------------
* 1.00	rp	07/13/2018 	Initial release
*
* </pre>
*
* @note
*
******************************************************************************/

#include "xpsmfw_api.h"
#include "xpsmfw_default.h"
#include "xpsmfw_power.h"
#include "xpsmfw_init.h"
#include "apu.h"
#include "psmx_global.h"
#include "rpu.h"
#include "crl.h"
#include "crf.h"
#include "pmc_global.h"
#include <assert.h>
#define CHECK_BIT(reg, mask)	(((reg) & (mask)) == (mask))

/**
 * NOTE: Older PsmToPlmEvent version (0x1U) only consists Event array
 *       while new version (0x2U) adds CpuIdleFlag and ResumeAddress in it.
 */
#define PSM_TO_PLM_EVENT_VERSION		(0x2U)
#define PWR_UP_EVT						(0x1U)
#define PWR_DWN_EVT                     (0x100U)
static volatile struct PsmToPlmEvent_t PsmToPlmEvent = {
	.Version	= PSM_TO_PLM_EVENT_VERSION,
	.Event		= {0x0},
	.CpuIdleFlag 	= {0x0},
	.ResumeAddress 	= {0x0},
};

static u8 ApuClusterState[4U] = {0U};

static struct XPsmFwPwrCtrl_t Acpu0_Core0PwrCtrl = {
	.Id = ACPU_0,
	.ResetCfgAddr = APU_CLUSTER0_RVBARADDR0L,
	.PwrStateMask = PSMX_LOCAL_REG_LOC_PWR_STATE0_APU0_CORE0_MASK,
	.PwrCtrlAddr = PSMX_LOCAL_REG_APU0_CORE0_PWR_CNTRL,
	.PwrStatusAddr = PSMX_LOCAL_REG_APU0_CORE0_PWR_STATUS,
	.ClkCtrlAddr = PSX_CRF_ACPU0_CLK_CTRL,
	.ClkCtrlMask = PSX_CRF_ACPU_CTRL_CLKACT_MASK,
	.ClkPropTime = XPSMFW_ACPU_CTRL_CLK_PROP_TIME,
	.RstAddr = PSX_CRF_RST_APU0,
	.WarmRstMask = PSX_CRF_RST_APU_CORE0_WARM_RST_MASK,
	.ClusterPstate = APU_PCLI_CLUSTER_0_PSTATE,
	.ClusterPstateMask = APU_PCLI_CLUSTER_PSTATE_MASK,
	.ClusterPstateValue = APU_PCLI_CLUSTER_PSTATE_VAL,
	.ClusterPreq = APU_PCLI_CLUSTER_0_PREQ,
	.ClusterPreqMask = APU_PCLI_CLUSTER_PREQ_MASK,
	.CorePstate = APU_PCLI_CORE_0_PSTATE,
	.CorePstateMask = APU_PCLI_CORE_PSTATE_MASK,
	.CorePstateVal = APU_PCLI_CORE_PSTATE_VAL,
	.CorePreq = APU_PCLI_CORE_0_PREQ,
	.CorePreqMask = APU_PCLI_CORE_PREQ_MASK,
	.CorePactive = APU_PCLI_CORE_0_PACTIVE,
	.CorePacceptMask = APU_PCLI_CORE_PACCEPT_MASK,
	.ClusterPactive = APU_PCLI_CLUSTER_0_PACTIVE,
	.ClusterPacceptMask = APU_PCLI_CLUSTER_PACCEPT_MASK,
	.ClusterId = CLUSTER_0,
};

static struct XPsmFwPwrCtrl_t Acpu0_Core1PwrCtrl = {
	.Id = ACPU_1,
	.ResetCfgAddr = APU_CLUSTER0_RVBARADDR1L,
	.PwrStateMask = PSMX_LOCAL_REG_LOC_PWR_STATE0_APU0_CORE1_MASK,
	.PwrCtrlAddr = PSMX_LOCAL_REG_APU0_CORE1_PWR_CNTRL,
	.PwrStatusAddr = PSMX_LOCAL_REG_APU0_CORE1_PWR_STATUS,
	.ClkCtrlAddr = PSX_CRF_ACPU0_CLK_CTRL,
	.ClkCtrlMask = PSX_CRF_ACPU_CTRL_CLKACT_MASK,
	.ClkPropTime = XPSMFW_ACPU_CTRL_CLK_PROP_TIME,
	.RstAddr = PSX_CRF_RST_APU0,
	.WarmRstMask = PSX_CRF_RST_APU_CORE1_WARM_RST_MASK,
	.ClusterPstate = APU_PCLI_CLUSTER_0_PSTATE,
	.ClusterPstateMask = APU_PCLI_CLUSTER_PSTATE_MASK,
	.ClusterPstateValue = APU_PCLI_CLUSTER_PSTATE_VAL,
	.ClusterPreq = APU_PCLI_CLUSTER_0_PREQ,
	.ClusterPreqMask = APU_PCLI_CLUSTER_PREQ_MASK,
	.CorePstate = APU_PCLI_CORE_1_PSTATE,
	.CorePstateMask = APU_PCLI_CORE_PSTATE_MASK,
	.CorePstateVal = APU_PCLI_CORE_PSTATE_VAL,
	.CorePreq = APU_PCLI_CORE_1_PREQ,
	.CorePreqMask = APU_PCLI_CORE_PREQ_MASK,
	.CorePactive = APU_PCLI_CORE_1_PACTIVE,
	.CorePacceptMask = APU_PCLI_CORE_PACCEPT_MASK,
	.ClusterPactive = APU_PCLI_CLUSTER_0_PACTIVE,
	.ClusterPacceptMask = APU_PCLI_CLUSTER_PACCEPT_MASK,
	.ClusterId = CLUSTER_0,
};

static struct XPsmFwPwrCtrl_t Acpu0_Core2PwrCtrl = {
	.Id = ACPU_2,
	.ResetCfgAddr = APU_CLUSTER0_RVBARADDR2L,
	.PwrStateMask = PSMX_LOCAL_REG_LOC_PWR_STATE0_APU0_CORE2_MASK,
	.PwrCtrlAddr = PSMX_LOCAL_REG_APU0_CORE2_PWR_CNTRL,
	.PwrStatusAddr = PSMX_LOCAL_REG_APU0_CORE2_PWR_STATUS,
	.ClkCtrlAddr = PSX_CRF_ACPU0_CLK_CTRL,
	.ClkCtrlMask = PSX_CRF_ACPU_CTRL_CLKACT_MASK,
	.ClkPropTime = XPSMFW_ACPU_CTRL_CLK_PROP_TIME,
	.RstAddr = PSX_CRF_RST_APU0,
	.WarmRstMask = PSX_CRF_RST_APU_CORE2_WARM_RST_MASK,
	.ClusterPstate = APU_PCLI_CLUSTER_0_PSTATE,
	.ClusterPstateMask = APU_PCLI_CLUSTER_PSTATE_MASK,
	.ClusterPstateValue = APU_PCLI_CLUSTER_PSTATE_VAL,
	.ClusterPreq = APU_PCLI_CLUSTER_0_PREQ,
	.ClusterPreqMask = APU_PCLI_CLUSTER_PREQ_MASK,
	.CorePstate = APU_PCLI_CORE_2_PSTATE,
	.CorePstateMask = APU_PCLI_CORE_PSTATE_MASK,
	.CorePstateVal = APU_PCLI_CORE_PSTATE_VAL,
	.CorePreq = APU_PCLI_CORE_2_PREQ,
	.CorePreqMask = APU_PCLI_CORE_PREQ_MASK,
	.CorePactive = APU_PCLI_CORE_2_PACTIVE,
	.CorePacceptMask = APU_PCLI_CORE_PACCEPT_MASK,
	.ClusterPactive = APU_PCLI_CLUSTER_0_PACTIVE,
	.ClusterPacceptMask = APU_PCLI_CLUSTER_PACCEPT_MASK,
	.ClusterId = CLUSTER_0,
};

static struct XPsmFwPwrCtrl_t Acpu0_Core3PwrCtrl = {
	.Id = ACPU_3,
	.ResetCfgAddr = APU_CLUSTER0_RVBARADDR3L,
	.PwrStateMask = PSMX_LOCAL_REG_LOC_PWR_STATE0_APU0_CORE3_MASK,
	.PwrCtrlAddr = PSMX_LOCAL_REG_APU0_CORE3_PWR_CNTRL,
	.PwrStatusAddr = PSMX_LOCAL_REG_APU0_CORE3_PWR_STATUS,
	.ClkCtrlAddr = PSX_CRF_ACPU0_CLK_CTRL,
	.ClkCtrlMask = PSX_CRF_ACPU_CTRL_CLKACT_MASK,
	.ClkPropTime = XPSMFW_ACPU_CTRL_CLK_PROP_TIME,
	.RstAddr = PSX_CRF_RST_APU0,
	.WarmRstMask = PSX_CRF_RST_APU_CORE3_WARM_RST_MASK,
	.ClusterPstate = APU_PCLI_CLUSTER_0_PSTATE,
	.ClusterPstateMask = APU_PCLI_CLUSTER_PSTATE_MASK,
	.ClusterPstateValue = APU_PCLI_CLUSTER_PSTATE_VAL,
	.ClusterPreq = APU_PCLI_CLUSTER_0_PREQ,
	.ClusterPreqMask = APU_PCLI_CLUSTER_PREQ_MASK,
	.CorePstate = APU_PCLI_CORE_3_PSTATE,
	.CorePstateMask = APU_PCLI_CORE_PSTATE_MASK,
	.CorePstateVal = APU_PCLI_CORE_PSTATE_VAL,
	.CorePreq = APU_PCLI_CORE_3_PREQ,
	.CorePreqMask = APU_PCLI_CORE_PREQ_MASK,
	.CorePactive = APU_PCLI_CORE_3_PACTIVE,
	.CorePacceptMask = APU_PCLI_CORE_PACCEPT_MASK,
	.ClusterPactive = APU_PCLI_CLUSTER_0_PACTIVE,
	.ClusterPacceptMask = APU_PCLI_CLUSTER_PACCEPT_MASK,
	.ClusterId = CLUSTER_0,
};

static struct XPsmFwPwrCtrl_t Acpu1_Core0PwrCtrl = {
	.Id = ACPU_4,
	.ResetCfgAddr = APU_CLUSTER1_RVBARADDR0L,
	.PwrStateMask = PSMX_LOCAL_REG_LOC_PWR_STATE0_APU1_CORE0_MASK,
	.PwrCtrlAddr = PSMX_LOCAL_REG_APU1_CORE0_PWR_CNTRL,
	.PwrStatusAddr = PSMX_LOCAL_REG_APU1_CORE0_PWR_STATUS,
	.ClkCtrlAddr = PSX_CRF_ACPU1_CLK_CTRL,
	.ClkCtrlMask = PSX_CRF_ACPU_CTRL_CLKACT_MASK,
	.ClkPropTime = XPSMFW_ACPU_CTRL_CLK_PROP_TIME,
	.RstAddr = PSX_CRF_RST_APU1,
	.WarmRstMask = PSX_CRF_RST_APU_CORE0_WARM_RST_MASK,
	.ClusterPstate = APU_PCLI_CLUSTER_1_PSTATE,
	.ClusterPstateMask = APU_PCLI_CLUSTER_PSTATE_MASK,
	.ClusterPstateValue = APU_PCLI_CLUSTER_PSTATE_VAL,
	.ClusterPreq = APU_PCLI_CLUSTER_1_PREQ,
	.ClusterPreqMask = APU_PCLI_CLUSTER_PREQ_MASK,
	.CorePstate = APU_PCLI_CORE_4_PSTATE,
	.CorePstateMask = APU_PCLI_CORE_PSTATE_MASK,
	.CorePstateVal = APU_PCLI_CORE_PSTATE_VAL,
	.CorePreq = APU_PCLI_CORE_4_PREQ,
	.CorePreqMask = APU_PCLI_CORE_PREQ_MASK,
	.CorePactive = APU_PCLI_CORE_4_PACTIVE,
	.CorePacceptMask = APU_PCLI_CORE_PACCEPT_MASK,
	.ClusterPactive = APU_PCLI_CLUSTER_1_PACTIVE,
	.ClusterPacceptMask = APU_PCLI_CLUSTER_PACCEPT_MASK,
	.ClusterId = CLUSTER_1,
};

static struct XPsmFwPwrCtrl_t Acpu1_Core1PwrCtrl = {
	.Id = ACPU_5,
	.ResetCfgAddr = APU_CLUSTER1_RVBARADDR1L,
	.PwrStateMask = PSMX_LOCAL_REG_LOC_PWR_STATE0_APU1_CORE1_MASK,
	.PwrCtrlAddr = PSMX_LOCAL_REG_APU1_CORE1_PWR_CNTRL,
	.PwrStatusAddr = PSMX_LOCAL_REG_APU1_CORE1_PWR_STATUS,
	.ClkCtrlAddr = PSX_CRF_ACPU1_CLK_CTRL,
	.ClkCtrlMask = PSX_CRF_ACPU_CTRL_CLKACT_MASK,
	.ClkPropTime = XPSMFW_ACPU_CTRL_CLK_PROP_TIME,
	.RstAddr = PSX_CRF_RST_APU1,
	.WarmRstMask = PSX_CRF_RST_APU_CORE1_WARM_RST_MASK,
	.ClusterPstate = APU_PCLI_CLUSTER_1_PSTATE,
	.ClusterPstateMask = APU_PCLI_CLUSTER_PSTATE_MASK,
	.ClusterPstateValue = APU_PCLI_CLUSTER_PSTATE_VAL,
	.ClusterPreq = APU_PCLI_CLUSTER_1_PREQ,
	.ClusterPreqMask = APU_PCLI_CLUSTER_PREQ_MASK,
	.CorePstate = APU_PCLI_CORE_5_PSTATE,
	.CorePstateMask = APU_PCLI_CORE_PSTATE_MASK,
	.CorePstateVal = APU_PCLI_CORE_PSTATE_VAL,
	.CorePreq = APU_PCLI_CORE_5_PREQ,
	.CorePreqMask = APU_PCLI_CORE_PREQ_MASK,
	.CorePactive = APU_PCLI_CORE_5_PACTIVE,
	.CorePacceptMask = APU_PCLI_CORE_PACCEPT_MASK,
	.ClusterPactive = APU_PCLI_CLUSTER_1_PACTIVE,
	.ClusterPacceptMask = APU_PCLI_CLUSTER_PACCEPT_MASK,
	.ClusterId = CLUSTER_1,
};

static struct XPsmFwPwrCtrl_t Acpu1_Core2PwrCtrl = {
	.Id = ACPU_6,
	.ResetCfgAddr = APU_CLUSTER1_RVBARADDR2L,
	.PwrStateMask = PSMX_LOCAL_REG_LOC_PWR_STATE0_APU1_CORE2_MASK,
	.PwrCtrlAddr = PSMX_LOCAL_REG_APU1_CORE2_PWR_CNTRL,
	.PwrStatusAddr = PSMX_LOCAL_REG_APU1_CORE2_PWR_STATUS,
	.ClkCtrlAddr = PSX_CRF_ACPU1_CLK_CTRL,
	.ClkCtrlMask = PSX_CRF_ACPU_CTRL_CLKACT_MASK,
	.ClkPropTime = XPSMFW_ACPU_CTRL_CLK_PROP_TIME,
	.RstAddr = PSX_CRF_RST_APU1,
	.WarmRstMask = PSX_CRF_RST_APU_CORE2_WARM_RST_MASK,
	.ClusterPstate = APU_PCLI_CLUSTER_1_PSTATE,
	.ClusterPstateMask = APU_PCLI_CLUSTER_PSTATE_MASK,
	.ClusterPstateValue = APU_PCLI_CLUSTER_PSTATE_VAL,
	.ClusterPreq = APU_PCLI_CLUSTER_1_PREQ,
	.ClusterPreqMask = APU_PCLI_CLUSTER_PREQ_MASK,
	.CorePstate = APU_PCLI_CORE_6_PSTATE,
	.CorePstateMask = APU_PCLI_CORE_PSTATE_MASK,
	.CorePstateVal = APU_PCLI_CORE_PSTATE_VAL,
	.CorePreq = APU_PCLI_CORE_6_PREQ,
	.CorePreqMask = APU_PCLI_CORE_PREQ_MASK,
	.CorePactive = APU_PCLI_CORE_6_PACTIVE,
	.CorePacceptMask = APU_PCLI_CORE_PACCEPT_MASK,
	.ClusterPactive = APU_PCLI_CLUSTER_1_PACTIVE,
	.ClusterPacceptMask = APU_PCLI_CLUSTER_PACCEPT_MASK,
	.ClusterId = CLUSTER_1,
};

static struct XPsmFwPwrCtrl_t Acpu1_Core3PwrCtrl = {
	.Id = ACPU_7,
	.ResetCfgAddr = APU_CLUSTER1_RVBARADDR3L,
	.PwrStateMask = PSMX_LOCAL_REG_LOC_PWR_STATE0_APU1_CORE3_MASK,
	.PwrCtrlAddr = PSMX_LOCAL_REG_APU1_CORE3_PWR_CNTRL,
	.PwrStatusAddr = PSMX_LOCAL_REG_APU1_CORE3_PWR_STATUS,
	.ClkCtrlAddr = PSX_CRF_ACPU1_CLK_CTRL,
	.ClkCtrlMask = PSX_CRF_ACPU_CTRL_CLKACT_MASK,
	.ClkPropTime = XPSMFW_ACPU_CTRL_CLK_PROP_TIME,
	.RstAddr = PSX_CRF_RST_APU1,
	.WarmRstMask = PSX_CRF_RST_APU_CORE3_WARM_RST_MASK,
	.ClusterPstate = APU_PCLI_CLUSTER_1_PSTATE,
	.ClusterPstateMask = APU_PCLI_CLUSTER_PSTATE_MASK,
	.ClusterPstateValue = APU_PCLI_CLUSTER_PSTATE_VAL,
	.ClusterPreq = APU_PCLI_CLUSTER_1_PREQ,
	.ClusterPreqMask = APU_PCLI_CLUSTER_PREQ_MASK,
	.CorePstate = APU_PCLI_CORE_7_PSTATE,
	.CorePstateMask = APU_PCLI_CORE_PSTATE_MASK,
	.CorePstateVal = APU_PCLI_CORE_PSTATE_VAL,
	.CorePreq = APU_PCLI_CORE_7_PREQ,
	.CorePreqMask = APU_PCLI_CORE_PREQ_MASK,
	.CorePactive = APU_PCLI_CORE_7_PACTIVE,
	.CorePacceptMask = APU_PCLI_CORE_PACCEPT_MASK,
	.ClusterPactive = APU_PCLI_CLUSTER_1_PACTIVE,
	.ClusterPacceptMask = APU_PCLI_CLUSTER_PACCEPT_MASK,
	.ClusterId = CLUSTER_1,
};

static struct XPsmFwPwrCtrl_t Rpu0_Core0PwrCtrl = {
	.Id = RPU0_0,
	.ResetCfgAddr = RPU_RPU0_CORE0_CFG0,
	.PwrStateMask = PSMX_LOCAL_REG_LOC_PWR_STATE0_RPU_A_CORE0_MASK,
	.PwrCtrlAddr = PSMX_LOCAL_REG_RPU_A_CORE0_PWR_CNTRL,
	.PwrStatusAddr = PSMX_LOCAL_REG_RPU_A_CORE0_PWR_STATUS,
	.ClkCtrlAddr = CRL_CPU_R5_CTRL,
	.ClkCtrlMask = CRL_CPU_R5_CTRL_CLKACT_CORE_MASK,
	.ClkPropTime = XPSMFW_RPU_CTRL_CLK_PROP_TIME,
	.RstCtrlMask = PSX_CRL_RST_RPU_CORE0A_MASK,
	.CorePstate = LPD_SLCR_RPU_PCIL_A0_PS,
	.CorePstateMask = LPD_SLCR_RPU_PCIL_A0_PS_PSTATE_MASK,
	.CorePreq = LPD_SLCR_RPU_PCIL_A0_PR,
	.CorePreqMask = LPD_SLCR_RPU_PCIL_A0_PR_PREQ_MASK,
	.CorePactive = LPD_SLCR_RPU_PCIL_A0_PA,
	.CorePactiveMask = LPD_SLCR_RPU_PCIL_A0_PA_PACTIVE_MASK,
	.CorePacceptMask = LPD_SLCR_RPU_PCIL_A0_PA_PACCEPT_MASK,
	.IntrDisableAddr = LPD_SLCR_RPU_PCIL_A0_IDS,
	.IntrDisableMask = LPD_SLCR_RPU_PCIL_A0_IDS_PACTIVE1_MASK,
	.ClusterId = CLUSTER_0,

};

static struct XPsmFwPwrCtrl_t Rpu0_Core1PwrCtrl = {
	.Id = RPU0_1,
	.ResetCfgAddr = RPU_RPU0_CORE1_CFG0,
	.PwrStateMask = PSMX_LOCAL_REG_LOC_PWR_STATE0_RPU_A_CORE1_MASK,
	.PwrCtrlAddr = PSMX_LOCAL_REG_RPU_A_CORE1_PWR_CNTRL,
	.PwrStatusAddr = PSMX_LOCAL_REG_RPU_A_CORE1_PWR_STATUS,
	.ClkCtrlAddr = CRL_CPU_R5_CTRL,
	.ClkCtrlMask = CRL_CPU_R5_CTRL_CLKACT_CORE_MASK,
	.ClkPropTime = XPSMFW_RPU_CTRL_CLK_PROP_TIME,
	.RstCtrlMask = PSX_CRL_RST_RPU_CORE1A_MASK,
	.CorePstate = LPD_SLCR_RPU_PCIL_A1_PS,
	.CorePstateMask = LPD_SLCR_RPU_PCIL_A1_PS_PSTATE_MASK,
	.CorePreq = LPD_SLCR_RPU_PCIL_A1_PR,
	.CorePreqMask = LPD_SLCR_RPU_PCIL_A1_PR_PREQ_MASK,
	.CorePactive = LPD_SLCR_RPU_PCIL_A1_PA,
	.CorePactiveMask = LPD_SLCR_RPU_PCIL_A1_PA_PACTIVE_MASK,
	.CorePacceptMask = LPD_SLCR_RPU_PCIL_A1_PA_PACCEPT_MASK,
	.IntrDisableAddr = LPD_SLCR_RPU_PCIL_A1_IDS,
	.IntrDisableMask = LPD_SLCR_RPU_PCIL_A1_IDS_PACTIVE1_MASK,
	.ClusterId = CLUSTER_0,

};

static struct XPsmFwPwrCtrl_t Rpu1_Core0PwrCtrl = {
	.Id = RPU0_0,
	.ResetCfgAddr = RPU_RPU1_CORE0_CFG0,
	.PwrStateMask = PSMX_LOCAL_REG_LOC_PWR_STATE0_RPU_B_CORE0_MASK,
	.PwrCtrlAddr = PSMX_LOCAL_REG_RPU_B_CORE0_PWR_CNTRL,
	.PwrStatusAddr = PSMX_LOCAL_REG_RPU_B_CORE0_PWR_STATUS,
	.ClkCtrlAddr = CRL_CPU_R5_CTRL,
	.ClkCtrlMask = CRL_CPU_R5_CTRL_CLKACT_CORE_MASK,
	.ClkPropTime = XPSMFW_RPU_CTRL_CLK_PROP_TIME,
	.RstCtrlMask = PSX_CRL_RST_RPU_CORE0B_MASK,
	.CorePstate = LPD_SLCR_RPU_PCIL_B0_PS,
	.CorePstateMask = LPD_SLCR_RPU_PCIL_B0_PS_PSTATE_MASK,
	.CorePreq = LPD_SLCR_RPU_PCIL_B0_PR,
	.CorePreqMask = LPD_SLCR_RPU_PCIL_B0_PR_PREQ_MASK,
	.CorePactive = LPD_SLCR_RPU_PCIL_B0_PA,
	.CorePactiveMask = LPD_SLCR_RPU_PCIL_B0_PA_PACTIVE_MASK,
	.CorePacceptMask = LPD_SLCR_RPU_PCIL_B0_PA_PACCEPT_MASK,
	.IntrDisableAddr = LPD_SLCR_RPU_PCIL_B0_IDS,
	.IntrDisableMask = LPD_SLCR_RPU_PCIL_B0_IDS_PACTIVE1_MASK,
	.ClusterId = CLUSTER_1,

};

static struct XPsmFwPwrCtrl_t Rpu1_Core1PwrCtrl = {
	.Id = RPU0_1,
	.ResetCfgAddr = RPU_RPU1_CORE1_CFG0,
	.PwrStateMask = PSMX_LOCAL_REG_LOC_PWR_STATE0_RPU_B_CORE1_MASK,
	.PwrCtrlAddr = PSMX_LOCAL_REG_RPU_B_CORE1_PWR_CNTRL,
	.PwrStatusAddr = PSMX_LOCAL_REG_RPU_B_CORE1_PWR_STATUS,
	.ClkCtrlAddr = CRL_CPU_R5_CTRL,
	.ClkCtrlMask = CRL_CPU_R5_CTRL_CLKACT_CORE_MASK,
	.ClkPropTime = XPSMFW_RPU_CTRL_CLK_PROP_TIME,
	.RstCtrlMask = PSX_CRL_RST_RPU_CORE1B_MASK,
	.CorePstate = LPD_SLCR_RPU_PCIL_B1_PS,
	.CorePstateMask = LPD_SLCR_RPU_PCIL_B1_PS_PSTATE_MASK,
	.CorePreq = LPD_SLCR_RPU_PCIL_B1_PR,
	.CorePreqMask = LPD_SLCR_RPU_PCIL_B1_PR_PREQ_MASK,
	.CorePactive = LPD_SLCR_RPU_PCIL_B1_PA,
	.CorePactiveMask = LPD_SLCR_RPU_PCIL_B1_PA_PACTIVE_MASK,
	.CorePacceptMask = LPD_SLCR_RPU_PCIL_B1_PA_PACCEPT_MASK,
	.IntrDisableAddr = LPD_SLCR_RPU_PCIL_B1_IDS,
	.IntrDisableMask = LPD_SLCR_RPU_PCIL_B1_IDS_PACTIVE1_MASK,
	.ClusterId = CLUSTER_1,

};

enum XPsmFWPwrUpDwnType {
	XPSMFW_PWR_UPDWN_DIRECT,
	XPSMFW_PWR_UPDWN_REQUEST,
};

static XStatus XPsmFwACPUxDirectPwrUp(struct XPsmFwPwrCtrl_t *Args)
{
	XStatus Status = XST_FAILURE;
	u32 LowAddress, HighAddress;

	if(ApuClusterState[Args->ClusterId] != A78_CLUSTER_CONFIGURED){
		/* APU PSTATE, PREQ configuration */
		XPsmFw_RMW32(Args->ClusterPstate,Args->ClusterPstateMask,Args->ClusterPstateValue);
		XPsmFw_RMW32(Args->ClusterPreq,Args->ClusterPreqMask,Args->ClusterPreqMask);

		/* ACPU clock config */
		XPsmFw_RMW32(Args->ClkCtrlAddr,Args->ClkCtrlMask,Args->ClkCtrlMask);

		/* APU cluster release cold & warm reset */
		XPsmFw_RMW32(Args->RstAddr,ACPU_CLUSTER_COLD_WARM_RST_MASK,0);

		Status = XPsmFw_UtilPollForMask(Args->ClusterPactive,Args->ClusterPacceptMask,ACPU_PACCEPT_TIMEOUT);
		if (Status != XST_SUCCESS) {
			XPsmFw_Printf(DEBUG_ERROR,"A78 Cluster PACCEPT timeout..\n");
			goto done;
		}
		ApuClusterState[Args->ClusterId] = A78_CLUSTER_CONFIGURED;
	}

	/*Enables Power to the Core*/
	XPsmFw_Write32(Args->PwrCtrlAddr,PSMX_LOCAL_REG_APU0_CORE0_PWR_CNTRL_PWR_GATES_MASK);

	/*Removes Isolation to the APU*/
	XPsmFw_RMW32(Args->PwrCtrlAddr,PSMX_LOCAL_REG_APU0_CORE0_PWR_CNTRL_ISOLATION_MASK,
			~PSMX_LOCAL_REG_APU0_CORE0_PWR_CNTRL_ISOLATION_MASK);

	/* Mark ACPUx powered up in LOCAL_PWR_STATUS register */
	XPsmFw_RMW32(PSMX_LOCAL_REG_LOC_PWR_STATE0, Args->PwrStateMask, Args->PwrStateMask);

	/*set start address*/
	LowAddress = (u32)(PsmToPlmEvent.ResumeAddress[Args->Id] & 0xfffffffeULL);
	HighAddress = (u32)(PsmToPlmEvent.ResumeAddress[Args->Id] >> 32ULL);
	XPsmFw_Write32(Args->ResetCfgAddr,LowAddress);
	XPsmFw_Write32(Args->ResetCfgAddr+0x4,HighAddress);
	PsmToPlmEvent.ResumeAddress[Args->Id]=0;

	XPsmFw_RMW32(Args->CorePstate,Args->CorePstateMask,Args->CorePstateVal);
	XPsmFw_RMW32(Args->CorePreq,Args->CorePreqMask,Args->CorePreqMask);

	/* APU core release warm reset */
	XPsmFw_RMW32(Args->RstAddr,Args->WarmRstMask,~Args->WarmRstMask);
	Status = XPsmFw_UtilPollForMask(Args->CorePactive,Args->CorePacceptMask,ACPU_PACCEPT_TIMEOUT);
	if (Status != XST_SUCCESS) {
		XPsmFw_Printf(DEBUG_ERROR,"A78 Cluster PACCEPT timeout..\n");
		goto done;
	}

	/* Disable and clear ACPUx direct wake-up interrupt request */
	XPsmFw_Write32(PSMX_GLOBAL_REG_WAKEUP0_IRQ_STATUS, Args->PwrStateMask);

	/*
	 * Unmask interrupt for all Power-up Requests and Reset Requests that
	 * are triggered but have their interrupt masked.
	 */
	XPsmFw_Write32(PSMX_GLOBAL_REG_REQ_PWRUP0_INT_EN, XPsmFw_Read32(PSMX_GLOBAL_REG_REQ_PWRDWN0_STATUS));
	XPsmFw_Write32(PSMX_GLOBAL_REG_REQ_SWRST_INT_EN, XPsmFw_Read32(PSMX_GLOBAL_REG_REQ_SWRST_STATUS));

done:
	return Status;
}

static XStatus XPsmFwACPUxDirectPwrDwn(struct XPsmFwPwrCtrl_t *Args)
{
	XStatus Status = XST_FAILURE;

	/* poll for power state change */
	Status = XPsmFw_UtilPollForMask(Args->CorePactive,Args->CorePacceptMask,ACPU_PACCEPT_TIMEOUT);
	if (Status != XST_SUCCESS) {
		XPsmFw_Printf(DEBUG_ERROR,"A78 Core PACCEPT timeout..\n");
		goto done;
	}

	XPsmFw_RMW32(Args->RstAddr,Args->WarmRstMask&PSX_CRF_RST_APU_WARM_RST_MASK,
			Args->WarmRstMask);
	/* enable isolation */
	XPsmFw_RMW32(Args->PwrCtrlAddr,PSM_LOCAL_PWR_CTRL_ISO_MASK,PSM_LOCAL_PWR_CTRL_ISO_MASK);

	/* disable power to the core */
	XPsmFw_RMW32(Args->PwrCtrlAddr,PSM_LOCAL_PWR_CTRL_GATES_MASK,~PSM_LOCAL_PWR_CTRL_GATES_MASK);

	/* unmask the wakeup interrupt */
	XPsmFw_Write32(PSMX_GLOBAL_REG_WAKEUP0_IRQ_EN,Args->PwrStateMask);

	/* Unmask the Power Up Interrupt */
	XPsmFw_Write32(PSMX_GLOBAL_REG_PWR_CTRL1_IRQ_EN,Args->PwrStateMask);

	/* Clear the Interrupt */
	XPsmFw_Write32(PSMX_GLOBAL_REG_PWR_CTRL1_IRQ_STATUS,Args->PwrStateMask);

	/*Mark ACPUx powered down in LOCAL_PWR_STATUS register */
	XPsmFw_RMW32(PSMX_LOCAL_REG_LOC_PWR_STATE0,Args->PwrStateMask,~Args->PwrStateMask);

	Status = XST_SUCCESS;

done:
	return Status;
}


static XStatus XPsmFwRPUxDirectPwrUp(struct XPsmFwPwrCtrl_t *Args)
{
	XStatus Status = XST_FAILURE;

	/*TBD:Set the start address */

    XPsmFw_RMW32(Args->ResetCfgAddr,RPU_CORE_CPUHALT_MASK,0);

	/* Mask wake interrupt */
    XPsmFw_RMW32(PSMX_GLOBAL_REG_WAKEUP1_IRQ_EN,Args->PwrStateMask >> 14, 0);

	/* Mask RPU PCIL Interrupts */
    XPsmFw_RMW32(Args->IntrDisableAddr,Args->IntrDisableMask,Args->IntrDisableMask);

	/* Restore Power to Core */
    XPsmFw_RMW32(Args->PwrCtrlAddr,PSMX_LOCAL_REG_RPU_A_CORE0_PWR_CNTRL_PWR_GATES_MASK,
			PSMX_LOCAL_REG_RPU_A_CORE0_PWR_CNTRL_PWR_GATES_MASK);

	/*Remove isolation */
    XPsmFw_RMW32(Args->PwrCtrlAddr,PSMX_LOCAL_REG_RPU_A_CORE0_PWR_CNTRL_ISOLATION_MASK,
		 ~PSMX_LOCAL_REG_RPU_A_CORE0_PWR_CNTRL_ISOLATION_MASK);

	/* Restore Power to the RPU core cache RAMs */
    XPsmFw_RMW32(PSMX_LOCAL_REG_RPU_CACHE_PWR_CNTRL,Args->PwrStateMask >> 16,Args->PwrStateMask >> 16);

	/* Enable the caches */
    XPsmFw_RMW32(PSMX_LOCAL_REG_RPU_CACHE_CE_CNTRL,Args->PwrStateMask >> 16,Args->PwrStateMask >> 16);

	/* set pstate field */
    XPsmFw_RMW32(Args->CorePstate,Args->CorePstateMask,~Args->CorePstateMask);

	/* set preq field to request power state change */
    XPsmFw_RMW32(Args->CorePreq,Args->CorePreqMask,Args->CorePreqMask);

	/* release reset */
    XPsmFw_RMW32(PSX_CRL_RST_RPU,Args->RstCtrlMask,~Args->RstCtrlMask);

	/* TBD: poll for paccept */

	/*Mark RPUx powered up in LOCAL_PWR_STATE register */
    XPsmFw_RMW32(PSMX_LOCAL_REG_LOC_PWR_STATE0,Args->PwrStateMask,Args->PwrStateMask);

	/* Disable and clear RPUx direct wake-up interrupt request */
	XPsmFw_Write32(PSMX_GLOBAL_REG_WAKEUP1_IRQ_STATUS, Args->PwrStateMask >> 14);

	/*
	 * Unmask interrupt for all Power-up Requests and Reset Requests that
	 * are triggered but have their interrupt masked.
	 */
	XPsmFw_Write32(PSMX_GLOBAL_REG_REQ_PWRUP0_INT_EN, XPsmFw_Read32(PSMX_GLOBAL_REG_REQ_PWRDWN0_STATUS));
	XPsmFw_Write32(PSMX_GLOBAL_REG_REQ_SWRST_INT_EN, XPsmFw_Read32(PSMX_GLOBAL_REG_REQ_SWRST_STATUS));

	Status = XST_SUCCESS;

	return Status;
}


static XStatus XPsmFwRPUxDirectPwrDwn(struct XPsmFwPwrCtrl_t *Args)
{
	XStatus Status = XST_FAILURE;
	/*TTBD:poll for pactive[1] bit to go low */

	/* set pstate bit */
	XPsmFw_RMW32(Args->CorePstate,Args->CorePstateMask,Args->CorePstateMask);

	/* set preq field to request power state change */
	XPsmFw_RMW32(Args->CorePreq,Args->CorePreqMask,Args->CorePreqMask);

	/* TBD: poll for paccept bit */

	/* Enable isolation */
	XPsmFw_RMW32(Args->PwrCtrlAddr,PSMX_LOCAL_REG_RPU_A_CORE0_PWR_CNTRL_ISOLATION_MASK,
		PSMX_LOCAL_REG_RPU_A_CORE0_PWR_CNTRL_ISOLATION_MASK);

	/* disable power to rpu core */
	XPsmFw_RMW32(Args->PwrCtrlAddr,PSMX_LOCAL_REG_RPU_A_CORE0_PWR_CNTRL_PWR_GATES_MASK,
		~PSMX_LOCAL_REG_RPU_A_CORE0_PWR_CNTRL_PWR_GATES_MASK);

	/* Disable the RPU core caches */
	XPsmFw_RMW32(PSMX_LOCAL_REG_RPU_CACHE_CE_CNTRL,Args->PwrStateMask >> 16,~(Args->PwrStateMask >> 16));

	/* Power gate the RPU core cache RAMs */
	XPsmFw_RMW32(PSMX_LOCAL_REG_RPU_CACHE_PWR_CNTRL,Args->PwrStateMask >> 16,~(Args->PwrStateMask >> 16));

	/* Unmask the corresponding Wake Interrupt RPU core */
	XPsmFw_RMW32(PSMX_GLOBAL_REG_WAKEUP1_IRQ_EN,Args->PwrStateMask >> 14, ~(Args->PwrStateMask >> 14));

	/* Unmask the RPU PCIL Interrupt */
	XPsmFw_Write32(Args->IntrDisableAddr,Args->IntrDisableMask);

	/*Mark RPUx powered down in LOCAL_PWR_STATE register */
	XPsmFw_RMW32(PSMX_LOCAL_REG_LOC_PWR_STATE0,Args->PwrStateMask,~Args->PwrStateMask);

	Status = XST_SUCCESS;

	return Status;
}

/**
 * XPsmFw_DispatchPwrUpHandler() - Power-up interrupt handler
 *
 * @PwrUpStatus    Power Up status register value
 * @PwrUpIntMask   Power Up interrupt mask register value
 *
 * @return         XST_SUCCESS or error code
 */
XStatus XPsmFw_DispatchPwrUpHandler(u32 PwrUpStatus, u32 PwrUpIntMask)
{
	XStatus Status = XST_FAILURE;
	(void)PwrUpStatus;
	(void)PwrUpIntMask;
	/*TODO: add power up support*/

	return Status;
}

/**
 * XPsmFw_DispatchPwrDwnHandler() - Power-down interrupt handler
 *
 * @PwrDwnStatus   Power Down status register value
 * @pwrDwnIntMask  Power Down interrupt mask register value
 * @PwrUpStatus    Power Up status register value
 * @PwrUpIntMask   Power Up interrupt mask register value
 *
 * @return         XST_SUCCESS or error code
 */
XStatus XPsmFw_DispatchPwrDwnHandler(u32 PwrDwnStatus, u32 pwrDwnIntMask,
		u32 PwrUpStatus, u32 PwrUpIntMask)
{
	XStatus Status = XST_FAILURE;
	(void)PwrDwnStatus;
	(void)pwrDwnIntMask;
	(void)PwrUpStatus;
	(void)PwrUpIntMask;
	/*TODO: add power down support*/

	return Status;
}

/**
 * ACPU0_Core0Wakeup() - Wake up ACPU0_CORE0
 *
 * @return    XST_SUCCESS or error code
 */
static XStatus ACPU0_Core0Wakeup(void)
{
	XStatus Status = XST_FAILURE;

	/* Check for any pending event */
	assert(PsmToPlmEvent.Event[ACPU_0] == 0U);

	if (1U == PsmToPlmEvent.CpuIdleFlag[ACPU_0]) {
		Status = XPsmFwACPUxDirectPwrUp(&Acpu0_Core0PwrCtrl);
		if (XST_SUCCESS != Status) {
			goto done;
		}
	}

	/* Set the event bit for PLM */
	PsmToPlmEvent.Event[ACPU_0] = PWR_UP_EVT;
	Status = XPsmFw_NotifyPlmEvent();

done:
	return Status;
}

/**
 * ACPU0_Core1Wakeup() - Wake up ACPU0_CORE1
 *
 * @return    XST_SUCCESS or error code
 */
static XStatus ACPU0_Core1Wakeup(void)
{
	XStatus Status = XST_FAILURE;

	/* Check for any pending event */
	assert(PsmToPlmEvent.Event[ACPU_1] == 0U);

	if (1U == PsmToPlmEvent.CpuIdleFlag[ACPU_1]) {
		Status = XPsmFwACPUxDirectPwrUp(&Acpu0_Core1PwrCtrl);
		if (XST_SUCCESS != Status) {
			goto done;
		}
	}

	/* Set the event bit for PLM */
	PsmToPlmEvent.Event[ACPU_1] = PWR_UP_EVT;
	Status = XPsmFw_NotifyPlmEvent();

done:
	return Status;
}

/**
 * ACPU0_Core2Wakeup() - Wake up ACPU0_CORE2
 *
 * @return    XST_SUCCESS or error code
 */
static XStatus ACPU0_Core2Wakeup(void)
{
	XStatus Status = XST_FAILURE;

	/* Check for any pending event */
	assert(PsmToPlmEvent.Event[ACPU_2] == 0U);

	if (1U == PsmToPlmEvent.CpuIdleFlag[ACPU_2]) {
		Status = XPsmFwACPUxDirectPwrUp(&Acpu0_Core2PwrCtrl);
		if (XST_SUCCESS != Status) {
			goto done;
		}
	}

	/* Set the event bit for PLM */
	PsmToPlmEvent.Event[ACPU_2] = PWR_UP_EVT;
	Status = XPsmFw_NotifyPlmEvent();

done:
	return Status;
}

/**
 * ACPU0_Core3Wakeup() - Wake up ACPU0_CORE3
 *
 * @return    XST_SUCCESS or error code
 */
static XStatus ACPU0_Core3Wakeup(void)
{
	XStatus Status = XST_FAILURE;

	/* Check for any pending event */
	assert(PsmToPlmEvent.Event[ACPU_3] == 0U);

	if (1U == PsmToPlmEvent.CpuIdleFlag[ACPU_3]) {
		Status = XPsmFwACPUxDirectPwrUp(&Acpu0_Core3PwrCtrl);
		if (XST_SUCCESS != Status) {
			goto done;
		}
	}

	/* Set the event bit for PLM */
	PsmToPlmEvent.Event[ACPU_3] = PWR_UP_EVT;
	Status = XPsmFw_NotifyPlmEvent();

done:
	return Status;
}

/**
 * ACPU1_Core0Wakeup() - Wake up ACPU1_CORE0
 *
 * @return    XST_SUCCESS or error code
 */
static XStatus ACPU1_Core0Wakeup(void)
{
	XStatus Status = XST_FAILURE;

	/* Check for any pending event */
	assert(PsmToPlmEvent.Event[ACPU_4] == 0U);

	if (1U == PsmToPlmEvent.CpuIdleFlag[ACPU_4]) {
		Status = XPsmFwACPUxDirectPwrUp(&Acpu1_Core0PwrCtrl);
		if (XST_SUCCESS != Status) {
			goto done;
		}
	}

	/* Set the event bit for PLM */
	PsmToPlmEvent.Event[ACPU_4] = PWR_UP_EVT;
	Status = XPsmFw_NotifyPlmEvent();

done:
	return Status;
}

/**
 * ACPU1_Core1Wakeup() - Wake up ACPU1_CORE1
 *
 * @return    XST_SUCCESS or error code
 */
static XStatus ACPU1_Core1Wakeup(void)
{
	XStatus Status = XST_FAILURE;

	/* Check for any pending event */
	assert(PsmToPlmEvent.Event[ACPU_5] == 0U);

	if (1U == PsmToPlmEvent.CpuIdleFlag[ACPU_5]) {
		Status = XPsmFwACPUxDirectPwrUp(&Acpu1_Core1PwrCtrl);
		if (XST_SUCCESS != Status) {
			goto done;
		}
	}

	/* Set the event bit for PLM */
	PsmToPlmEvent.Event[ACPU_5] = PWR_UP_EVT;
	Status = XPsmFw_NotifyPlmEvent();

done:
	return Status;
}

/**
 * ACPU1_Core2Wakeup() - Wake up ACPU1_CORE2
 *
 * @return    XST_SUCCESS or error code
 */
static XStatus ACPU1_Core2Wakeup(void)
{
	XStatus Status = XST_FAILURE;

	/* Check for any pending event */
	assert(PsmToPlmEvent.Event[ACPU_6] == 0U);

	if (1U == PsmToPlmEvent.CpuIdleFlag[ACPU_6]) {
		Status = XPsmFwACPUxDirectPwrUp(&Acpu1_Core2PwrCtrl);
		if (XST_SUCCESS != Status) {
			goto done;
		}
	}

	/* Set the event bit for PLM */
	PsmToPlmEvent.Event[ACPU_6] = PWR_UP_EVT;
	Status = XPsmFw_NotifyPlmEvent();

done:
	return Status;
}

/**
 * ACPU1_Core3Wakeup() - Wake up ACPU1_CORE3
 *
 * @return    XST_SUCCESS or error code
 */
static XStatus ACPU1_Core3Wakeup(void)
{
	XStatus Status = XST_FAILURE;

	/* Check for any pending event */
	assert(PsmToPlmEvent.Event[ACPU_7] == 0U);

	if (1U == PsmToPlmEvent.CpuIdleFlag[ACPU_7]) {
		Status = XPsmFwACPUxDirectPwrUp(&Acpu1_Core3PwrCtrl);
		if (XST_SUCCESS != Status) {
			goto done;
		}
	}

	/* Set the event bit for PLM */
	PsmToPlmEvent.Event[ACPU_7] = PWR_UP_EVT;
	Status = XPsmFw_NotifyPlmEvent();

done:
	return Status;
}

/**
 * ACPU0_Core0Sleep() - Direct power down ACPU0 Core0
 *
 * @return    XST_SUCCESS or error code
 */
static XStatus ACPU0_Core0Sleep(void)
{
	XStatus Status = XST_FAILURE;

	/* Check for any pending event */
	assert(PsmToPlmEvent.Event[ACPU_0] == 0U);

	if (1U == PsmToPlmEvent.CpuIdleFlag[ACPU_0]) {
		Status = XPsmFwACPUxDirectPwrDwn(&Acpu0_Core0PwrCtrl);
		if (XST_SUCCESS != Status) {
			goto done;
		}
	}

	/* Set the event bit for PLM */
	PsmToPlmEvent.Event[ACPU_0] = PWR_DWN_EVT;
	Status = XPsmFw_NotifyPlmEvent();

done:
	return Status;
}

/**
 * ACPU0_Core1Sleep() - Direct power down ACPU0 Core1
 *
 * @return    XST_SUCCESS or error code
 */
static XStatus ACPU0_Core1Sleep(void)
{
	XStatus Status = XST_FAILURE;

	/* Check for any pending event */
	assert(PsmToPlmEvent.Event[ACPU_1] == 0U);

	if (1U == PsmToPlmEvent.CpuIdleFlag[ACPU_1]) {
		Status = XPsmFwACPUxDirectPwrDwn(&Acpu0_Core1PwrCtrl);
		if (XST_SUCCESS != Status) {
			goto done;
		}
	}

	/* Set the event bit for PLM */
	PsmToPlmEvent.Event[ACPU_1] = PWR_DWN_EVT;
	Status = XPsmFw_NotifyPlmEvent();

done:
	return Status;
}

/**
 * ACPU0_Core2Sleep() - Direct power down ACPU0 Core2
 *
 * @return    XST_SUCCESS or error code
 */
static XStatus ACPU0_Core2Sleep(void)
{
	XStatus Status = XST_FAILURE;

	/* Check for any pending event */
	assert(PsmToPlmEvent.Event[ACPU_2] == 0U);

	if (1U == PsmToPlmEvent.CpuIdleFlag[ACPU_2]) {
		Status = XPsmFwACPUxDirectPwrDwn(&Acpu0_Core2PwrCtrl);
		if (XST_SUCCESS != Status) {
			goto done;
		}
	}

	/* Set the event bit for PLM */
	PsmToPlmEvent.Event[ACPU_2] = PWR_DWN_EVT;
	Status = XPsmFw_NotifyPlmEvent();

done:
	return Status;
}

/**
 * ACPU0_Core3Sleep() - Direct power down ACPU0 Core3
 *
 * @return    XST_SUCCESS or error code
 */
static XStatus ACPU0_Core3Sleep(void)
{
	XStatus Status = XST_FAILURE;

	/* Check for any pending event */
	assert(PsmToPlmEvent.Event[ACPU_3] == 0U);

	if (1U == PsmToPlmEvent.CpuIdleFlag[ACPU_3]) {
		Status = XPsmFwACPUxDirectPwrDwn(&Acpu0_Core3PwrCtrl);
		if (XST_SUCCESS != Status) {
			goto done;
		}
	}

	/* Set the event bit for PLM */
	PsmToPlmEvent.Event[ACPU_3] = PWR_DWN_EVT;
	Status = XPsmFw_NotifyPlmEvent();

done:
	return Status;
}

/**
 * ACPU1_Core0Sleep() - Direct power down ACPU1 Core0
 *
 * @return    XST_SUCCESS or error code
 */
static XStatus ACPU1_Core0Sleep(void)
{
	XStatus Status = XST_FAILURE;

	/* Check for any pending event */
	assert(PsmToPlmEvent.Event[ACPU_4] == 0U);

	if (1U == PsmToPlmEvent.CpuIdleFlag[ACPU_4]) {
		Status = XPsmFwACPUxDirectPwrDwn(&Acpu1_Core0PwrCtrl);
		if (XST_SUCCESS != Status) {
			goto done;
		}
	}

	/* Set the event bit for PLM */
	PsmToPlmEvent.Event[ACPU_4] = PWR_DWN_EVT;
	Status = XPsmFw_NotifyPlmEvent();

done:
	return Status;
}

/**
 * ACPU1_Core1Sleep() - Direct power down ACPU1 Core1
 *
 * @return    XST_SUCCESS or error code
 */
static XStatus ACPU1_Core1Sleep(void)
{
	XStatus Status = XST_FAILURE;

	/* Check for any pending event */
	assert(PsmToPlmEvent.Event[ACPU_5] == 0U);

	if (1U == PsmToPlmEvent.CpuIdleFlag[ACPU_5]) {
		Status = XPsmFwACPUxDirectPwrDwn(&Acpu1_Core1PwrCtrl);
		if (XST_SUCCESS != Status) {
			goto done;
		}
	}

	/* Set the event bit for PLM */
	PsmToPlmEvent.Event[ACPU_5] = PWR_DWN_EVT;
	Status = XPsmFw_NotifyPlmEvent();

done:
	return Status;
}

/**
 * ACPU1_Core2Sleep() - Direct power down ACPU1 Core2
 *
 * @return    XST_SUCCESS or error code
 */
static XStatus ACPU1_Core2Sleep(void)
{
	XStatus Status = XST_FAILURE;

	/* Check for any pending event */
	assert(PsmToPlmEvent.Event[ACPU_6] == 0U);

	if (1U == PsmToPlmEvent.CpuIdleFlag[ACPU_6]) {
		Status = XPsmFwACPUxDirectPwrDwn(&Acpu1_Core2PwrCtrl);
		if (XST_SUCCESS != Status) {
			goto done;
		}
	}

	/* Set the event bit for PLM */
	PsmToPlmEvent.Event[ACPU_6] = PWR_DWN_EVT;
	Status = XPsmFw_NotifyPlmEvent();

done:
	return Status;
}

/**
 * ACPU1_Core3Sleep() - Direct power down ACPU1 Core3
 *
 * @return    XST_SUCCESS or error code
 */
static XStatus ACPU1_Core3Sleep(void)
{
	XStatus Status = XST_FAILURE;

	/* Check for any pending event */
	assert(PsmToPlmEvent.Event[ACPU_7] == 0U);

	if (1U == PsmToPlmEvent.CpuIdleFlag[ACPU_7]) {
		Status = XPsmFwACPUxDirectPwrDwn(&Acpu1_Core3PwrCtrl);
		if (XST_SUCCESS != Status) {
			goto done;
		}
	}

	/* Set the event bit for PLM */
	PsmToPlmEvent.Event[ACPU_7] = PWR_DWN_EVT;
	Status = XPsmFw_NotifyPlmEvent();

done:
	return Status;
}

/**
 * RPU0_Core0Wakeup() - Wake up Rpu0 Core0
 *
 * @return    XST_SUCCESS or error code
 */
static XStatus RPU0_Core0Wakeup(void)
{
	XStatus Status = XST_FAILURE;

	/* Check for any pending event */
	assert(PsmToPlmEvent.Event[RPU0_0] == 0U);

	if (1U == PsmToPlmEvent.CpuIdleFlag[RPU0_0]) {
		Status = XPsmFwRPUxDirectPwrUp(&Rpu0_Core0PwrCtrl);
		if (XST_SUCCESS != Status) {
			goto done;
		}
	}

	/* Set the event bit for PLM */
	PsmToPlmEvent.Event[RPU0_0] = PWR_UP_EVT;
	Status = XPsmFw_NotifyPlmEvent();

done:
	return Status;
}

/**
 * RPU0_Core1Wakeup() - Wake up Rpu0 Core1
 *
 * @return    XST_SUCCESS or error code
 */
static XStatus RPU0_Core1Wakeup(void)
{
	XStatus Status = XST_FAILURE;

	/* Check for any pending event */
	assert(PsmToPlmEvent.Event[RPU0_1] == 0U);

	if (1U == PsmToPlmEvent.CpuIdleFlag[RPU0_1]) {
		Status = XPsmFwRPUxDirectPwrUp(&Rpu0_Core1PwrCtrl);
		if (XST_SUCCESS != Status) {
			goto done;
		}
	}

	/* Set the event bit for PLM */
	PsmToPlmEvent.Event[RPU0_1] = PWR_UP_EVT;
	Status = XPsmFw_NotifyPlmEvent();

done:
	return Status;
}

/**
 * RPU1_Core0Wakeup() - Wake up Rpu1 Core0
 *
 * @return    XST_SUCCESS or error code
 */
static XStatus RPU1_Core0Wakeup(void)
{
	XStatus Status = XST_FAILURE;

	/* Check for any pending event */
	assert(PsmToPlmEvent.Event[RPU1_0] == 0U);

	if (1U == PsmToPlmEvent.CpuIdleFlag[RPU1_0]) {
		Status = XPsmFwRPUxDirectPwrUp(&Rpu1_Core0PwrCtrl);
		if (XST_SUCCESS != Status) {
			goto done;
		}
	}

	/* Set the event bit for PLM */
	PsmToPlmEvent.Event[RPU1_0] = PWR_UP_EVT;
	Status = XPsmFw_NotifyPlmEvent();

done:
	return Status;
}

/**
 * RPU1_Core1Wakeup() - Wake up Rpu1 Core1
 *
 * @return    XST_SUCCESS or error code
 */
static XStatus RPU1_Core1Wakeup(void)
{
	XStatus Status = XST_FAILURE;

	/* Check for any pending event */
	assert(PsmToPlmEvent.Event[RPU1_1] == 0U);

	if (1U == PsmToPlmEvent.CpuIdleFlag[RPU1_1]) {
		Status = XPsmFwRPUxDirectPwrUp(&Rpu1_Core1PwrCtrl);
		if (XST_SUCCESS != Status) {
			goto done;
		}
	}

	/* Set the event bit for PLM */
	PsmToPlmEvent.Event[RPU1_1] = PWR_UP_EVT;
	Status = XPsmFw_NotifyPlmEvent();

done:
	return Status;
}

/**
 * RPU0_Core0Sleep() - Direct power down Rpu0 Core0
 *
 * @return    XST_SUCCESS or error code
 */
static XStatus RPU0_Core0Sleep(void)
{
	XStatus Status = XST_FAILURE;

	/* Check for any pending event */
	assert(PsmToPlmEvent.Event[RPU0_0] == 0U);

	if (1U == PsmToPlmEvent.CpuIdleFlag[RPU0_0]) {
		Status = XPsmFwRPUxDirectPwrDwn(&Rpu0_Core0PwrCtrl);
		if (XST_SUCCESS != Status) {
			goto done;
		}
	}

	/* Set the event bit for PLM */
	PsmToPlmEvent.Event[RPU0_0] = PWR_DWN_EVT;
	Status = XPsmFw_NotifyPlmEvent();

done:
	return Status;
}

/**
 * RPU0_Core1Sleep() - Direct power down Rpu0 Core1
 *
 * @return    XST_SUCCESS or error code
 */
static XStatus RPU0_Core1Sleep(void)
{
	XStatus Status = XST_FAILURE;

	/* Check for any pending event */
	assert(PsmToPlmEvent.Event[RPU0_1] == 0U);

	if (1U == PsmToPlmEvent.CpuIdleFlag[RPU0_1]) {
		Status = XPsmFwRPUxDirectPwrDwn(&Rpu0_Core1PwrCtrl);
		if (XST_SUCCESS != Status) {
			goto done;
		}
	}

	/* Set the event bit for PLM */
	PsmToPlmEvent.Event[RPU0_1] = PWR_DWN_EVT;
	Status = XPsmFw_NotifyPlmEvent();

done:
	return Status;
}

/**
 * RPU1_Core0Sleep() - Direct power down Rpu1 Core0
 *
 * @return    XST_SUCCESS or error code
 */
static XStatus RPU1_Core0Sleep(void)
{
	XStatus Status = XST_FAILURE;

	/* Check for any pending event */
	assert(PsmToPlmEvent.Event[RPU1_0] == 0U);

	if (1U == PsmToPlmEvent.CpuIdleFlag[RPU1_0]) {
		Status = XPsmFwRPUxDirectPwrDwn(&Rpu1_Core0PwrCtrl);
		if (XST_SUCCESS != Status) {
			goto done;
		}
	}

	/* Set the event bit for PLM */
	PsmToPlmEvent.Event[RPU1_0] = PWR_DWN_EVT;
	Status = XPsmFw_NotifyPlmEvent();

done:
	return Status;
}

/**
 * RPU1_Core1Sleep() - Direct power down Rpu1 Core1
 *
 * @return    XST_SUCCESS or error code
 */
static XStatus RPU1_Core1Sleep(void)
{
	XStatus Status = XST_FAILURE;

	/* Check for any pending event */
	assert(PsmToPlmEvent.Event[RPU1_1] == 0U);

	if (1U == PsmToPlmEvent.CpuIdleFlag[RPU1_1]) {
		Status = XPsmFwRPUxDirectPwrDwn(&Rpu1_Core1PwrCtrl);
		if (XST_SUCCESS != Status) {
			goto done;
		}
	}

	/* Set the event bit for PLM */
	PsmToPlmEvent.Event[RPU1_1] = PWR_DWN_EVT;
	Status = XPsmFw_NotifyPlmEvent();

done:
	return Status;
}

/****************************************************************************/
/**
 * @brief	Direct power down processor
 *
 * @param DeviceId	Device ID of processor
 *
 * @return	XST_SUCCESS or error code
 *
 * @note	None
 *
 ****************************************************************************/
XStatus XPsmFw_DirectPwrDwn(const u32 DeviceId)
{
	XStatus Status = XST_FAILURE;

	switch (DeviceId) {
		case XPSMFW_DEV_CLUSTER0_ACPU_0:
			Status = XPsmFwACPUxDirectPwrDwn(&Acpu0_Core0PwrCtrl);
			break;
		case XPSMFW_DEV_CLUSTER0_ACPU_1:
			Status = XPsmFwACPUxDirectPwrDwn(&Acpu0_Core1PwrCtrl);
			break;
		case XPSMFW_DEV_CLUSTER0_ACPU_2:
			Status = XPsmFwACPUxDirectPwrDwn(&Acpu0_Core2PwrCtrl);
			break;
		case XPSMFW_DEV_CLUSTER0_ACPU_3:
			Status = XPsmFwACPUxDirectPwrDwn(&Acpu0_Core3PwrCtrl);
			break;
		case XPSMFW_DEV_CLUSTER1_ACPU_0:
			Status = XPsmFwACPUxDirectPwrDwn(&Acpu1_Core0PwrCtrl);
			break;
		case XPSMFW_DEV_CLUSTER1_ACPU_1:
			Status = XPsmFwACPUxDirectPwrDwn(&Acpu1_Core1PwrCtrl);
			break;
		case XPSMFW_DEV_CLUSTER1_ACPU_2:
			Status = XPsmFwACPUxDirectPwrDwn(&Acpu1_Core2PwrCtrl);
			break;
		case XPSMFW_DEV_CLUSTER1_ACPU_3:
			Status = XPsmFwACPUxDirectPwrDwn(&Acpu1_Core3PwrCtrl);
			break;
		case XPSMFW_DEV_CLUSTER0_RPU0_0:
			Status = XPsmFwRPUxDirectPwrDwn(&Rpu0_Core0PwrCtrl);
			break;
		case XPSMFW_DEV_CLUSTER0_RPU0_1:
			Status = XPsmFwRPUxDirectPwrDwn(&Rpu0_Core1PwrCtrl);
			break;
		case XPSMFW_DEV_CLUSTER1_RPU0_0:
			Status = XPsmFwRPUxDirectPwrDwn(&Rpu1_Core0PwrCtrl);
			break;
		case XPSMFW_DEV_CLUSTER1_RPU0_1:
			Status = XPsmFwRPUxDirectPwrDwn(&Rpu1_Core1PwrCtrl);
			break;
		default:
			Status = XST_INVALID_PARAM;
			break;
	}

	return Status;
}

/****************************************************************************/
/**
 * @brief	Direct power up processor
 *
 * @param DeviceId	Device ID of processor
 *
 * @return	XST_SUCCESS or error code
 *
 * @note	None
 *
 ****************************************************************************/
XStatus XPsmFw_DirectPwrUp(const u32 DeviceId)
{
	XStatus Status = XST_FAILURE;

	switch (DeviceId) {
		case XPSMFW_DEV_CLUSTER0_ACPU_0:
			Status = XPsmFwACPUxDirectPwrUp(&Acpu0_Core0PwrCtrl);
			break;
		case XPSMFW_DEV_CLUSTER0_ACPU_1:
			Status = XPsmFwACPUxDirectPwrUp(&Acpu0_Core1PwrCtrl);
			break;
		case XPSMFW_DEV_CLUSTER0_ACPU_2:
			Status = XPsmFwACPUxDirectPwrUp(&Acpu0_Core2PwrCtrl);
			break;
		case XPSMFW_DEV_CLUSTER0_ACPU_3:
			Status = XPsmFwACPUxDirectPwrUp(&Acpu0_Core3PwrCtrl);
			break;
		case XPSMFW_DEV_CLUSTER1_ACPU_0:
			Status = XPsmFwACPUxDirectPwrUp(&Acpu1_Core0PwrCtrl);
			break;
		case XPSMFW_DEV_CLUSTER1_ACPU_1:
			Status = XPsmFwACPUxDirectPwrUp(&Acpu1_Core1PwrCtrl);
			break;
		case XPSMFW_DEV_CLUSTER1_ACPU_2:
			Status = XPsmFwACPUxDirectPwrUp(&Acpu1_Core2PwrCtrl);
			break;
		case XPSMFW_DEV_CLUSTER1_ACPU_3:
			Status = XPsmFwACPUxDirectPwrUp(&Acpu1_Core3PwrCtrl);
			break;
		case XPSMFW_DEV_CLUSTER0_RPU0_0:
			Status = XPsmFwRPUxDirectPwrUp(&Rpu0_Core0PwrCtrl);
			break;
		case XPSMFW_DEV_CLUSTER0_RPU0_1:
			Status = XPsmFwRPUxDirectPwrUp(&Rpu0_Core1PwrCtrl);
			break;
		case XPSMFW_DEV_CLUSTER1_RPU0_0:
			Status = XPsmFwRPUxDirectPwrUp(&Rpu1_Core0PwrCtrl);
			break;
		case XPSMFW_DEV_CLUSTER1_RPU0_1:
			Status = XPsmFwRPUxDirectPwrUp(&Rpu1_Core1PwrCtrl);
			break;
		default:
			Status = XST_INVALID_PARAM;
			break;
	}

	return Status;
}

static struct PwrCtlWakeupHandlerTable_t APUWakeupHandlerTable[] = {
	{ PSMX_GLOBAL_REG_WAKEUP0_IRQ_STATUS_APU0_CORE0_MASK, ACPU0_Core0Wakeup},
	{ PSMX_GLOBAL_REG_WAKEUP0_IRQ_STATUS_APU0_CORE1_MASK, ACPU0_Core1Wakeup},
	{ PSMX_GLOBAL_REG_WAKEUP0_IRQ_STATUS_APU0_CORE2_MASK, ACPU0_Core2Wakeup},
	{ PSMX_GLOBAL_REG_WAKEUP0_IRQ_STATUS_APU0_CORE3_MASK, ACPU0_Core3Wakeup},
	{ PSMX_GLOBAL_REG_WAKEUP0_IRQ_STATUS_APU1_CORE0_MASK, ACPU1_Core0Wakeup},
	{ PSMX_GLOBAL_REG_WAKEUP0_IRQ_STATUS_APU1_CORE1_MASK, ACPU1_Core1Wakeup},
	{ PSMX_GLOBAL_REG_WAKEUP0_IRQ_STATUS_APU1_CORE2_MASK, ACPU1_Core2Wakeup},
	{ PSMX_GLOBAL_REG_WAKEUP0_IRQ_STATUS_APU1_CORE3_MASK, ACPU1_Core3Wakeup},
};

static struct PwrCtlWakeupHandlerTable_t RPUWakeupHandlerTable[] = {
	{ PSMX_GLOBAL_REG_WAKEUP1_IRQ_STATUS_RPU_A_CORE0_MASK, RPU0_Core0Wakeup},
	{ PSMX_GLOBAL_REG_WAKEUP1_IRQ_STATUS_RPU_A_CORE1_MASK, RPU0_Core1Wakeup},
	{ PSMX_GLOBAL_REG_WAKEUP1_IRQ_STATUS_RPU_B_CORE0_MASK, RPU1_Core0Wakeup},
	{ PSMX_GLOBAL_REG_WAKEUP1_IRQ_STATUS_RPU_B_CORE1_MASK, RPU1_Core1Wakeup},
};

static struct PwrCtlWakeupHandlerTable_t SleepHandlerTable[] = {
	{ PSMX_GLOBAL_REG_PWR_CTRL1_IRQ_STATUS_APU0_CORE0_PWRDWN_MASK, ACPU0_Core0Sleep},
	{ PSMX_GLOBAL_REG_PWR_CTRL1_IRQ_STATUS_APU0_CORE1_PWRDWN_MASK, ACPU0_Core1Sleep},
	{ PSMX_GLOBAL_REG_PWR_CTRL1_IRQ_STATUS_APU0_CORE2_PWRDWN_MASK, ACPU0_Core2Sleep},
	{ PSMX_GLOBAL_REG_PWR_CTRL1_IRQ_STATUS_APU0_CORE3_PWRDWN_MASK, ACPU0_Core3Sleep},
	{ PSMX_GLOBAL_REG_PWR_CTRL1_IRQ_STATUS_APU1_CORE0_PWRDWN_MASK, ACPU1_Core0Sleep},
	{ PSMX_GLOBAL_REG_PWR_CTRL1_IRQ_STATUS_APU1_CORE1_PWRDWN_MASK, ACPU1_Core1Sleep},
	{ PSMX_GLOBAL_REG_PWR_CTRL1_IRQ_STATUS_APU1_CORE2_PWRDWN_MASK, ACPU1_Core2Sleep},
	{ PSMX_GLOBAL_REG_PWR_CTRL1_IRQ_STATUS_APU1_CORE3_PWRDWN_MASK, ACPU1_Core3Sleep},
	{ PSMX_GLOBAL_REG_PWR_CTRL1_IRQ_MASK_RPU_A_CORE0_PWRDWN_MASK, RPU0_Core0Sleep},
	{ PSMX_GLOBAL_REG_PWR_CTRL1_IRQ_MASK_RPU_A_CORE1_PWRDWN_MASK, RPU0_Core1Sleep},
	{ PSMX_GLOBAL_REG_PWR_CTRL1_IRQ_MASK_RPU_B_CORE0_PWRDWN_MASK, RPU1_Core0Sleep},
	{ PSMX_GLOBAL_REG_PWR_CTRL1_IRQ_MASK_RPU_B_CORE1_PWRDWN_MASK, RPU1_Core1Sleep},
};

/**
 * XPsmFw_DispatchAPUWakeupHandler() - Wakeup up interrupt handler
 *
 * @WakeupStatus    Wake Up status register value
 * @WakeupIntMask   Wake Up interrupt mask register value
 *
 * @return         XST_SUCCESS or error code
 */
XStatus XPsmFw_DispatchAPUWakeupHandler(u32 WakeupStatus, u32 WakeupIntMask)
{
	XStatus Status = XST_FAILURE;
	u32 Index;

	for (Index = 0U; Index < ARRAYSIZE(APUWakeupHandlerTable); Index++) {
		if ((CHECK_BIT(WakeupStatus, APUWakeupHandlerTable[Index].Mask)) &&
		    !(CHECK_BIT(WakeupIntMask, APUWakeupHandlerTable[Index].Mask))) {
			/* Call power up handler */
			Status = APUWakeupHandlerTable[Index].Handler();

			/* Disable wake-up interrupt */
			XPsmFw_Write32(PSMX_GLOBAL_REG_WAKEUP0_IRQ_DIS, APUWakeupHandlerTable[Index].Mask);
		} else {
			Status = XST_SUCCESS;
		}
	}

	return Status;
}

/**
 * XPsmFw_DispatchRPUWakeupHandler() - RPU Wakeup up interrupt handler
 *
 * @WakeupStatus    Wake Up status register value
 * @WakeupIntMask   Wake Up interrupt mask register value
 *
 * @return         XST_SUCCESS or error code
 */
XStatus XPsmFw_DispatchRPUWakeupHandler(u32 WakeupStatus, u32 WakeupIntMask)
{
	XStatus Status = XST_FAILURE;
	u32 Index;
	for (Index = 0U; Index < ARRAYSIZE(RPUWakeupHandlerTable); Index++) {
		if ((CHECK_BIT(WakeupStatus, RPUWakeupHandlerTable[Index].Mask)) &&
		    !(CHECK_BIT(WakeupIntMask, RPUWakeupHandlerTable[Index].Mask))) {
			/* Call power up handler */
			Status = RPUWakeupHandlerTable[Index].Handler();

			/* Disable wake-up interrupt */
			XPsmFw_Write32(PSMX_GLOBAL_REG_WAKEUP1_IRQ_DIS, RPUWakeupHandlerTable[Index].Mask);
		} else {
			Status = XST_SUCCESS;
		}
	}

	return Status;
}

/**
 * XPsmFw_DispatchPwrCtlHandler() - PwrCtl interrupt handler
 *
 * @PwrCtlStatus   Power Down status register value
 * @PwrCtlIntMask  Power Down interrupt mask register value
 *
 * @return         XST_SUCCESS or error code
 */
XStatus XPsmFw_DispatchPwrCtlHandler(u32 PwrCtlStatus, u32 PwrCtlIntMask)
{
	XStatus Status = XST_FAILURE;
	u32 Index;

	for (Index = 0U; Index < ARRAYSIZE(SleepHandlerTable); Index++) {
		if ((CHECK_BIT(PwrCtlStatus, SleepHandlerTable[Index].Mask)) &&
		    !(CHECK_BIT(PwrCtlIntMask, SleepHandlerTable[Index].Mask))) {
			/* Call power up handler */
			Status = SleepHandlerTable[Index].Handler();

			/* Disable direct power-down interrupt */
			XPsmFw_Write32(PSMX_GLOBAL_REG_PWR_CTRL1_IRQ_DIS, SleepHandlerTable[Index].Mask);
		} else {
			Status = XST_SUCCESS;
		}
	}

	return Status;
}

/**
 * XPsmFw_GetPsmToPlmEventAddr() - Provides address of
 * PsmToPlmEvent
 *
 * @EventAddr      Buffer pointer to store PsmToPlmEvent
 */
void XPsmFw_GetPsmToPlmEventAddr(u32 *EventAddr)
{
	*EventAddr = (u32)(&PsmToPlmEvent);
}