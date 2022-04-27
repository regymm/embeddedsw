/******************************************************************************
* Copyright (c) 2022 Xilinx, Inc.  All rights reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/

/*
 * This is an automatically generated file from script.
 * Please do not modify this!
 */
#ifndef XPM_NODE_H_
#define XPM_NODE_H_

#include <xil_types.h>
#include <xstatus.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct XPm_Node XPm_Node;
/**
 * The node class.  This is the base class for all the power, clock, pin and
 * reset node classes.
 */
struct XPm_Node {
        u32 Id; /**< Node ID: For XilPM clock and pin APIs */
        u32 BaseAddress; /**< Base address: Specify to node type */
        u16 LatencyMarg; /**< lowest latency requirement - powerup latency */
        u8  State; /**< Node state: Specific to node type */
        u8  Flags;
};

/************************** Function Prototypes ******************************/
void XPmNode_Init(XPm_Node *Node, u32 Id, u8 State, u32 BaseAddress);

#define NODE_CLASS_SHIFT        26U
#define NODE_SUBCLASS_SHIFT     20U
#define NODE_TYPE_SHIFT         14U
#define NODE_INDEX_SHIFT        0U
#define NODE_CLASS_MASK_BITS    0x3FU
#define NODE_SUBCLASS_MASK_BITS 0x3FU
#define NODE_TYPE_MASK_BITS     0x3FU
#define NODE_INDEX_MASK_BITS    0x3FFFU
#define NODE_CLASS_MASK         ((u32)NODE_CLASS_MASK_BITS << NODE_CLASS_SHIFT)
#define NODE_SUBCLASS_MASK      ((u32)NODE_SUBCLASS_MASK_BITS << NODE_SUBCLASS_SHIFT)
#define NODE_TYPE_MASK          ((u32)NODE_TYPE_MASK_BITS << NODE_TYPE_SHIFT)
#define NODE_INDEX_MASK         ((u32)NODE_INDEX_MASK_BITS << NODE_INDEX_SHIFT)

#define NODEID(CLASS, SUBCLASS, TYPE, INDEX)    \
        ((((CLASS) & NODE_CLASS_MASK_BITS) << NODE_CLASS_SHIFT) | \
        (((SUBCLASS) & NODE_SUBCLASS_MASK_BITS) << NODE_SUBCLASS_SHIFT) | \
        (((TYPE) & NODE_TYPE_MASK_BITS) << NODE_TYPE_SHIFT) | \
        (((INDEX) & NODE_INDEX_MASK_BITS) << NODE_INDEX_SHIFT))

#define NODECLASS(ID)           (((ID) & NODE_CLASS_MASK) >> NODE_CLASS_SHIFT)
#define NODESUBCLASS(ID)        (((ID) & NODE_SUBCLASS_MASK) >> NODE_SUBCLASS_SHIFT)
#define NODETYPE(ID)            (((ID) & NODE_TYPE_MASK) >> NODE_TYPE_SHIFT)
#define NODEINDEX(ID)           (((ID) & NODE_INDEX_MASK) >> NODE_INDEX_SHIFT)

/* Node class types */
typedef enum {
	XPM_NODECLASS_MIN,	/* 0X0 */
	XPM_NODECLASS_POWER,	/* 0X1 */
	XPM_NODECLASS_CLOCK,	/* 0X2 */
	XPM_NODECLASS_RESET,	/* 0X3 */
	XPM_NODECLASS_MEMIC,	/* 0X4 */
	XPM_NODECLASS_STMIC,	/* 0X5 */
	XPM_NODECLASS_DEVICE,	/* 0X6 */
	XPM_NODECLASS_SUBSYSTEM,	/* 0X7 */
	XPM_NODECLASS_ISOLATION,	/* 0X8 */
	XPM_NODECLASS_PROTECTION,	/* 0X9 */
	XPM_NODECLASS_EVENT,	/* 0XA */
	XPM_NODECLASS_MONITOR,	/* 0XB */
	XPM_NODECLASS_REGNODE,	/* 0XC */
	XPM_NODECLASS_MISC,	/* 0XD */
	XPM_NODECLASS_MAX	/* 0XE */
} XPm_NodeClass;


/* Node subclass types */
/* Power nodes */
typedef enum {
	XPM_NODESUBCL_POWER_MIN,	/* 0X0 */
	XPM_NODESUBCL_POWER_ISLAND,	/* 0X1 */
	XPM_NODESUBCL_POWER_DOMAIN,	/* 0X2 */
} XPm_PowerNodeSubclass;

/* Clock nodes */
typedef enum {
	XPM_NODESUBCL_CLOCK_MIN,	/* 0X0 */
	XPM_NODESUBCL_CLOCK_PLL,	/* 0X1 */
	XPM_NODESUBCL_CLOCK_OUT,	/* 0X2 */
	XPM_NODESUBCL_CLOCK_REF,	/* 0X3 */
} XPm_ClockNodeSubclass;

/* Reset nodes */
typedef enum {
	XPM_NODESUBCL_RESET_MIN,	/* 0X0 */
	XPM_NODESUBCL_RESET_PERIPHERAL,	/* 0X1 */
	XPM_NODESUBCL_RESET_DBG,	/* 0X2 */
	XPM_NODESUBCL_RESET_POR,	/* 0X3 */
	XPM_NODESUBCL_RESET_SOFT,	/* 0X4 */
	XPM_NODESUBCL_RESET_WARM,	/* 0X5 */
	XPM_NODESUBCL_RESET_COLD,	/* 0X6 */
} XPm_ResetNodeSubclass;

/* Device nodes */
typedef enum {
	XPM_NODESUBCL_DEV_MIN,	/* 0X0 */
	XPM_NODESUBCL_DEV_CORE,	/* 0X1 */
	XPM_NODESUBCL_DEV_PERIPH,	/* 0X2 */
	XPM_NODESUBCL_DEV_MEM,	/* 0X3 */
	XPM_NODESUBCL_DEV_SOC,	/* 0X4 */
	XPM_NODESUBCL_DEV_PL = 7,
	XPM_NODESUBCL_DEV_AIE,
} XPm_DeviceNodeSubclass;

/* isolation subclasses */
typedef enum {
	XPM_NODESUBCL_ISOLATION,	/* 0X0 */
} XPm_IsolationSubclass;

/* subsyste, subclasses */
typedef enum {
	XPM_NODESUBCL_SUBSYSTEM,	/* 0X0 */
} XPm_SubsystemSubclass;


/* Node types */
/* Power nodes */
typedef enum {
	XPM_NODETYPE_POWER_MIN,	/* 0X0 */
	XPM_NODETYPE_POWER_ISLAND,	/* 0X1 */
	XPM_NODETYPE_POWER_DOMAIN_PMC,	/* 0X2 */
	XPM_NODETYPE_POWER_DOMAIN_PS_FULL,	/* 0X3 */
	XPM_NODETYPE_POWER_DOMAIN_PS_LOW,	/* 0X4 */
	XPM_NODETYPE_POWER_DOMAIN_NOC,	/* 0X5 */
	XPM_NODETYPE_POWER_DOMAIN_CPM,	/* 0X6 */
	XPM_NODETYPE_POWER_DOMAIN_ME,	/* 0X7 */
	XPM_NODETYPE_POWER_DOMAIN_PL,	/* 0X8 */
} XPm_PowerNodeType;

/* Clock nodes */
typedef enum {
	XPM_NODETYPE_CLOCK_MIN,	/* 0X0 */
	XPM_NODETYPE_CLOCK_PLL,	/* 0X1 */
	XPM_NODETYPE_CLOCK_OUT,	/* 0X2 */
	XPM_NODETYPE_CLOCK_REF,	/* 0X3 */
	XPM_NODETYPE_CLOCK_SUBNODE,	/* 0X4 */
} XPm_ClockNodeType;

/* Reset nodes */
typedef enum {
	XPM_NODETYPE_RESET_MIN,	/* 0X0 */
	XPM_NODETYPE_RESET_PERIPHERAL,	/* 0X1 */
	XPM_NODETYPE_RESET_POR,	/* 0X2 */
	XPM_NODETYPE_RESET_DBG,	/* 0X3 */
	XPM_NODETYPE_RESET_SOFT,	/* 0X4 */
	XPM_NODETYPE_RESET_WARM,	/* 0X5 */
	XPM_NODETYPE_RESET_COLD,	/* 0X6 */
} XPm_ResetNodeType;

/* Device nodes */
typedef enum {
	XPM_NODETYPE_DEV_MIN,	/* 0X0 */
	XPM_NODETYPE_DEV_CORE_PMC,	/* 0X1 */
	XPM_NODETYPE_DEV_CORE_PSM,	/* 0X2 */
	XPM_NODETYPE_DEV_CORE_APU,	/* 0X3 */
	XPM_NODETYPE_DEV_CORE_RPU,	/* 0X4 */
	XPM_NODETYPE_DEV_OCM,	/* 0X5 */
	XPM_NODETYPE_DEV_TCM,	/* 0X6 */
	XPM_NODETYPE_DEV_PERIPH,	/* 0X7 */
	XPM_NODETYPE_DEV_DDR,	/* 0X8 */
	XPM_NODETYPE_DEV_SOC,	/* 0X9 */
} XPm_DeviceNodeType;

/* isolation subclasses */
typedef enum {
	XPM_NODETYPE_ISOLATION,	/* 0X0 */
} XPm_IsolationType;

/* subsystem type */
typedef enum {
	XPM_NODETYPE_SUBSYSTEM,	/* 0X0 */
} XPm_SubsystemType;


/* Node index */
/* Power nodes */
typedef enum {
	XPM_NODEIDX_POWER_MIN,	/* 0X0 */
	XPM_NODEIDX_POWER_PMC,	/* 0X1 */
	XPM_NODEIDX_POWER_LPD,	/* 0X2 */
	XPM_NODEIDX_POWER_NOC,	/* 0X3 */
	XPM_NODEIDX_POWER_PLD,	/* 0X4 */
	XPM_NODEIDX_POWER_CPM5N,	/* 0X5 */
	XPM_NODEIDX_POWER_FPD,	/* 0X6 */
	XPM_NODEIDX_POWER_ACPU_0_0,	/* 0X7 */
	XPM_NODEIDX_POWER_ACPU_0_1,	/* 0X8 */
	XPM_NODEIDX_POWER_ACPU_0_2,	/* 0X9 */
	XPM_NODEIDX_POWER_ACPU_0_3,	/* 0XA */
	XPM_NODEIDX_POWER_ACPU_1_0,	/* 0XB */
	XPM_NODEIDX_POWER_ACPU_1_1,	/* 0XC */
	XPM_NODEIDX_POWER_ACPU_1_2,	/* 0XD */
	XPM_NODEIDX_POWER_ACPU_1_3,	/* 0XE */
	XPM_NODEIDX_POWER_ACPU_2_0,	/* 0XF */
	XPM_NODEIDX_POWER_ACPU_2_1,	/* 0X10 */
	XPM_NODEIDX_POWER_ACPU_2_2,	/* 0X11 */
	XPM_NODEIDX_POWER_ACPU_2_3,	/* 0X12 */
	XPM_NODEIDX_POWER_ACPU_3_0,	/* 0X13 */
	XPM_NODEIDX_POWER_ACPU_3_1,	/* 0X14 */
	XPM_NODEIDX_POWER_ACPU_3_2,	/* 0X15 */
	XPM_NODEIDX_POWER_ACPU_3_3,	/* 0X16 */
	XPM_NODEIDX_POWER_RPU_A_0,	/* 0X17 */
	XPM_NODEIDX_POWER_RPU_A_1,	/* 0X18 */
	XPM_NODEIDX_POWER_RPU_B_0,	/* 0X19 */
	XPM_NODEIDX_POWER_RPU_B_1,	/* 0X1A */
	XPM_NODEIDX_POWER_MAX	/* 0X1B */
} XPm_PowerNodeIdx;

/* Clock nodes */
typedef enum {
	XPM_NODEIDX_CLK_MIN,	/* 0X0 */
	XPM_NODEIDX_CLK_CPLL_PRE_SRC,	/* 0X1 */
	XPM_NODEIDX_CLK_CPLL_POST_SRC,	/* 0X2 */
	XPM_NODEIDX_CLK_CPLL_OUT,	/* 0X3 */
	XPM_NODEIDX_CLK_CPLL_CLK,	/* 0X4 */
	XPM_NODEIDX_CLK_NOCPLL_PRE_SRC,	/* 0X5 */
	XPM_NODEIDX_CLK_NOCPLL_POST_SRC,	/* 0X6 */
	XPM_NODEIDX_CLK_NOCPLL_OUT,	/* 0X7 */
	XPM_NODEIDX_CLK_NOCPLL_CLK,	/* 0X8 */
	XPM_NODEIDX_CLK_PMCPLL_PRE_SRC,	/* 0X9 */
	XPM_NODEIDX_CLK_PMCPLL_POST_SRC,	/* 0XA */
	XPM_NODEIDX_CLK_PMCPLL_OUT,	/* 0XB */
	XPM_NODEIDX_CLK_PMCPLL_CLK,	/* 0XC */
	XPM_NODEIDX_CLK_APLL1_PRE_SRC,	/* 0XD */
	XPM_NODEIDX_CLK_APLL1_POST_SRC,	/* 0XE */
	XPM_NODEIDX_CLK_APLL1_OUT,	/* 0XF */
	XPM_NODEIDX_CLK_APLL1_CLK,	/* 0X10 */
	XPM_NODEIDX_CLK_APLL2_PRE_SRC,	/* 0X11 */
	XPM_NODEIDX_CLK_APLL2_POST_SRC,	/* 0X12 */
	XPM_NODEIDX_CLK_APLL2_OUT,	/* 0X13 */
	XPM_NODEIDX_CLK_APLL2_CLK,	/* 0X14 */
	XPM_NODEIDX_CLK_RPLL_PRE_SRC,	/* 0X15 */
	XPM_NODEIDX_CLK_RPLL_POST_SRC,	/* 0X16 */
	XPM_NODEIDX_CLK_RPLL_OUT,	/* 0X17 */
	XPM_NODEIDX_CLK_RPLL_CLK,	/* 0X18 */
	XPM_NODEIDX_CLK_FLXPLL_PRE_SRC,	/* 0X19 */
	XPM_NODEIDX_CLK_FLXPLL_POST_SRC,	/* 0X1A */
	XPM_NODEIDX_CLK_FLXPLL_OUT,	/* 0X1B */
	XPM_NODEIDX_CLK_FLXPLL_CLK,	/* 0X1C */
	XPM_NODEIDX_CLK_REF_CLK,	/* 0X1D */
	XPM_NODEIDX_CLK_IRO,	/* 0X1E */
	XPM_NODEIDX_CLK_IN_REF,	/* 0X1F */
	XPM_NODEIDX_CLK_OSCCLK_DIV2,	/* 0X20 */
	XPM_NODEIDX_CLK_PL_ALT_REF_CLK,	/* 0X21 */
	XPM_NODEIDX_CLK_CPM5N_DBG_REF,	/* 0X22 */
	XPM_NODEIDX_CLK_CPM5N_AUX1_REF,	/* 0X23 */
	XPM_NODEIDX_CLK_CPM5N_TOPSW_REF,	/* 0X24 */
	XPM_NODEIDX_CLK_CPM5N_AUX0_REF,	/* 0X25 */
	XPM_NODEIDX_CLK_CPM5N_DMA_ALT,	/* 0X26 */
	XPM_NODEIDX_CLK_CPM5N_AUX2_REF,	/* 0X27 */
	XPM_NODEIDX_CLK_CPM5N_CORE_REF,	/* 0X28 */
	XPM_NODEIDX_CLK_PMC_PL2_REF,	/* 0X29 */
	XPM_NODEIDX_CLK_DFT_OSC_REF,	/* 0X2A */
	XPM_NODEIDX_CLK_PMC_RCLK,	/* 0X2B */
	XPM_NODEIDX_CLK_OSPI_REF,	/* 0X2C */
	XPM_NODEIDX_CLK_USB_SUSPEND,	/* 0X2D */
	XPM_NODEIDX_CLK_QSPI_REF,	/* 0X2E */
	XPM_NODEIDX_CLK_PMC_PL3_REF,	/* 0X2F */
	XPM_NODEIDX_CLK_SD_DLL_REF,	/* 0X30 */
	XPM_NODEIDX_CLK_NPLL_TO_XPD,	/* 0X31 */
	XPM_NODEIDX_CLK_HSM0_REF,	/* 0X32 */
	XPM_NODEIDX_CLK_HSM1_REF,	/* 0X33 */
	XPM_NODEIDX_CLK_PMC_PL0_REF,	/* 0X34 */
	XPM_NODEIDX_CLK_PMC_LSBUS_REF,	/* 0X35 */
	XPM_NODEIDX_CLK_EFUSE_REF,	/* 0X36 */
	XPM_NODEIDX_CLK_TEST_PATTERN_REF,	/* 0X37 */
	XPM_NODEIDX_CLK_SDIO0_REF,	/* 0X38 */
	XPM_NODEIDX_CLK_PMC_PL1_REF,	/* 0X39 */
	XPM_NODEIDX_CLK_SYSMON_REF,	/* 0X3A */
	XPM_NODEIDX_CLK_CFU_REF,	/* 0X3B */
	XPM_NODEIDX_CLK_SWITCH_TIMEOUT,	/* 0X3C */
	XPM_NODEIDX_CLK_SPARE_REF,	/* 0X3D */
	XPM_NODEIDX_CLK_NPI_REF,	/* 0X3E */
	XPM_NODEIDX_CLK_PPLL_TO_XPD,	/* 0X3F */
	XPM_NODEIDX_CLK_I2C_REF,	/* 0X40 */
	XPM_NODEIDX_CLK_IRO_SUSPEND_REF,	/* 0X41 */
	XPM_NODEIDX_CLK_SDIO1_REF,	/* 0X42 */
	XPM_NODEIDX_CLK_FPD_RCLK,	/* 0X43 */
	XPM_NODEIDX_CLK_DBG_TRACE,	/* 0X44 */
	XPM_NODEIDX_CLK_FPD_WWDT_PLL,	/* 0X45 */
	XPM_NODEIDX_CLK_FPX_TOP_SWITCH,	/* 0X46 */
	XPM_NODEIDX_CLK_FPX_LSBUS,	/* 0X47 */
	XPM_NODEIDX_CLK_PERIPH,	/* 0X48 */
	XPM_NODEIDX_CLK_DBG_FPX,	/* 0X49 */
	XPM_NODEIDX_CLK_ACPU0,	/* 0X4A */
	XPM_NODEIDX_CLK_ACPU3,	/* 0X4B */
	XPM_NODEIDX_CLK_ACPU2,	/* 0X4C */
	XPM_NODEIDX_CLK_ACPU1,	/* 0X4D */
	XPM_NODEIDX_CLK_FPX_PKI_DIV,	/* 0X4E */
	XPM_NODEIDX_CLK_SPI0_REF,	/* 0X4F */
	XPM_NODEIDX_CLK_I3C0_REF,	/* 0X50 */
	XPM_NODEIDX_CLK_UART0_REF,	/* 0X51 */
	XPM_NODEIDX_CLK_LPX_LSBUS,	/* 0X52 */
	XPM_NODEIDX_CLK_CAN0_REF_2X,	/* 0X53 */
	XPM_NODEIDX_CLK_I3C1_REF,	/* 0X54 */
	XPM_NODEIDX_CLK_DBG_TSTMP,	/* 0X55 */
	XPM_NODEIDX_CLK_IOU_SWITCH,	/* 0X56 */
	XPM_NODEIDX_CLK_USB0_BUS_REF,	/* 0X57 */
	XPM_NODEIDX_CLK_LPD_RCLK,	/* 0X58 */
	XPM_NODEIDX_CLK_SPI1_REF,	/* 0X59 */
	XPM_NODEIDX_CLK_GEM1_REF,	/* 0X5A */
	XPM_NODEIDX_CLK_GEM1_REF_TX,	/* 0X5B */
	XPM_NODEIDX_CLK_GEM1_REF_RX,	/* 0X5C */
	XPM_NODEIDX_CLK_RPU,	/* 0X5D */
	XPM_NODEIDX_CLK_RPU_OCM,	/* 0X5E */
	XPM_NODEIDX_CLK_RPU_XRAM,	/* 0X5F */
	XPM_NODEIDX_CLK_RPU_CLUSTERB,	/* 0X60 */
	XPM_NODEIDX_CLK_RPU_CLUSTERA,	/* 0X61 */
	XPM_NODEIDX_CLK_GEM0_REF,	/* 0X62 */
	XPM_NODEIDX_CLK_GEM0_REF_TX,	/* 0X63 */
	XPM_NODEIDX_CLK_GEM0_REF_RX,	/* 0X64 */
	XPM_NODEIDX_CLK_PSM,	/* 0X65 */
	XPM_NODEIDX_CLK_CPM_TOPSW,	/* 0X66 */
	XPM_NODEIDX_CLK_TIMESTAMP_REF,	/* 0X67 */
	XPM_NODEIDX_CLK_GEM_TSU_REF,	/* 0X68 */
	XPM_NODEIDX_CLK_CAN1_REF_2X,	/* 0X69 */
	XPM_NODEIDX_CLK_RPLL_TO_XPD,	/* 0X6A */
	XPM_NODEIDX_CLK_LPD_WWDT_PLL,	/* 0X6B */
	XPM_NODEIDX_CLK_USB1_BUS_REF,	/* 0X6C */
	XPM_NODEIDX_CLK_LPX_TOP_SWITCH,	/* 0X6D */
	XPM_NODEIDX_CLK_LPX_TOP_SWITCH_ADMA,	/* 0X6E */
	XPM_NODEIDX_CLK_DBG_LPX,	/* 0X6F */
	XPM_NODEIDX_CLK_UART1_REF,	/* 0X70 */
	XPM_NODEIDX_CLK_CPLL,	/* 0X71 */
	XPM_NODEIDX_CLK_NOCPLL,	/* 0X72 */
	XPM_NODEIDX_CLK_PMCPLL,	/* 0X73 */
	XPM_NODEIDX_CLK_APLL1,	/* 0X74 */
	XPM_NODEIDX_CLK_APLL2,	/* 0X75 */
	XPM_NODEIDX_CLK_RPLL,	/* 0X76 */
	XPM_NODEIDX_CLK_FLXPLL,	/* 0X77 */
	XPM_NODEIDX_CLK_MAX	/* 0X78 */
} XPm_ClockNodeIdx;

/* Reset nodes */
typedef enum {
	XPM_NODEIDX_RST_MIN,	/* 0X0 */
	XPM_NODEIDX_RST_GTY1,	/* 0X1 */
	XPM_NODEIDX_RST_GTY3,	/* 0X2 */
	XPM_NODEIDX_RST_GTY2,	/* 0X3 */
	XPM_NODEIDX_RST_DMA_CONFIG_POR,	/* 0X4 */
	XPM_NODEIDX_RST_CDX,	/* 0X5 */
	XPM_NODEIDX_RST_PCIE0_CONFIG,	/* 0X6 */
	XPM_NODEIDX_RST_PCIE_CONFIG_POR,	/* 0X7 */
	XPM_NODEIDX_RST_DPU,	/* 0X8 */
	XPM_NODEIDX_RST_FPD_SYSMON_CFG,	/* 0X9 */
	XPM_NODEIDX_RST_FPD_SYSMON_SEQ,	/* 0XA */
	XPM_NODEIDX_RST_CDX_CONFIG,	/* 0XB */
	XPM_NODEIDX_RST_PCIE1,	/* 0XC */
	XPM_NODEIDX_RST_PCIE0,	/* 0XD */
	XPM_NODEIDX_RST_PCIE3,	/* 0XE */
	XPM_NODEIDX_RST_PCIE2,	/* 0XF */
	XPM_NODEIDX_RST_PCIE1_CONFIG,	/* 0X10 */
	XPM_NODEIDX_RST_PCIE3_CONFIG,	/* 0X11 */
	XPM_NODEIDX_RST_DPU_CONFIG,	/* 0X12 */
	XPM_NODEIDX_RST_PCIE2_CONFIG,	/* 0X13 */
	XPM_NODEIDX_RST_DBG,	/* 0X14 */
	XPM_NODEIDX_RST_RAM0,	/* 0X15 */
	XPM_NODEIDX_RST_RAM1,	/* 0X16 */
	XPM_NODEIDX_RST_ECO_2_R1_SECO,	/* 0X17 */
	XPM_NODEIDX_RST_ECO_1_R0_SECO,	/* 0X18 */
	XPM_NODEIDX_RST_XPD_SOFT_CLK0,	/* 0X19 */
	XPM_NODEIDX_RST_ECO_0_R0_SECO,	/* 0X1A */
	XPM_NODEIDX_RST_USB1_PHY,	/* 0X1B */
	XPM_NODEIDX_RST_WWDT,	/* 0X1C */
	XPM_NODEIDX_RST_NONPS_NPI,	/* 0X1D */
	XPM_NODEIDX_RST_NONPS_SYS_1,	/* 0X1E */
	XPM_NODEIDX_RST_NONPS_NOC_POR,	/* 0X1F */
	XPM_NODEIDX_RST_NONPS_SYS_3,	/* 0X20 */
	XPM_NODEIDX_RST_NONPS_SYS_2,	/* 0X21 */
	XPM_NODEIDX_RST_NONPS_NOC,	/* 0X22 */
	XPM_NODEIDX_RST_OSPI,	/* 0X23 */
	XPM_NODEIDX_RST_PS_PL_POR,	/* 0X24 */
	XPM_NODEIDX_RST_PS_PS_SRST,	/* 0X25 */
	XPM_NODEIDX_RST_PS_PMC_SRST,	/* 0X26 */
	XPM_NODEIDX_RST_PS_PL_SRST,	/* 0X27 */
	XPM_NODEIDX_RST_PS_PMC_POR,	/* 0X28 */
	XPM_NODEIDX_RST_PS_PS_POR,	/* 0X29 */
	XPM_NODEIDX_RST_GPIO,	/* 0X2A */
	XPM_NODEIDX_RST_SBI,	/* 0X2B */
	XPM_NODEIDX_RST_SDIO1,	/* 0X2C */
	XPM_NODEIDX_RST_PL1,	/* 0X2D */
	XPM_NODEIDX_RST_PL0,	/* 0X2E */
	XPM_NODEIDX_RST_PL3,	/* 0X2F */
	XPM_NODEIDX_RST_PL2,	/* 0X30 */
	XPM_NODEIDX_RST_USB0_PHY,	/* 0X31 */
	XPM_NODEIDX_RST_QSPI,	/* 0X32 */
	XPM_NODEIDX_RST_SDIO0,	/* 0X33 */
	XPM_NODEIDX_RST_PDMA1,	/* 0X34 */
	XPM_NODEIDX_RST_PDMA0,	/* 0X35 */
	XPM_NODEIDX_RST_I2C,	/* 0X36 */
	XPM_NODEIDX_RST_DBG_DPC,	/* 0X37 */
	XPM_NODEIDX_RST_PKI,	/* 0X38 */
	XPM_NODEIDX_RST_DBG_FPX,	/* 0X39 */
	XPM_NODEIDX_RST_FPX_SWDT2,	/* 0X3A */
	XPM_NODEIDX_RST_FMU,	/* 0X3B */
	XPM_NODEIDX_RST_FPX_SWDT1,	/* 0X3C */
	XPM_NODEIDX_RST_MMU_TBU3,	/* 0X3D */
	XPM_NODEIDX_RST_MMU_GLOBAL,	/* 0X3E */
	XPM_NODEIDX_RST_MMU_TBU4,	/* 0X3F */
	XPM_NODEIDX_RST_MMU_TBU9,	/* 0X40 */
	XPM_NODEIDX_RST_MMU_TBU5,	/* 0X41 */
	XPM_NODEIDX_RST_MMU_TBU0,	/* 0X42 */
	XPM_NODEIDX_RST_MMU_TBU7,	/* 0X43 */
	XPM_NODEIDX_RST_MMU_TBU1,	/* 0X44 */
	XPM_NODEIDX_RST_MMU_TBU8,	/* 0X45 */
	XPM_NODEIDX_RST_MMU_TBU6,	/* 0X46 */
	XPM_NODEIDX_RST_MMU_TBU10,	/* 0X47 */
	XPM_NODEIDX_RST_MMU_TBU2,	/* 0X48 */
	XPM_NODEIDX_RST_APU3_CORE1_WARM,	/* 0X49 */
	XPM_NODEIDX_RST_APU3_CORE3_COLD,	/* 0X4A */
	XPM_NODEIDX_RST_APU3_CORE0_COLD,	/* 0X4B */
	XPM_NODEIDX_RST_APU3_CORE1_COLD,	/* 0X4C */
	XPM_NODEIDX_RST_APU3_CLUSTER_COLD,	/* 0X4D */
	XPM_NODEIDX_RST_APU3_CORE0_WARM,	/* 0X4E */
	XPM_NODEIDX_RST_APU3_CORE2_COLD,	/* 0X4F */
	XPM_NODEIDX_RST_APU3_CORE2_WARM,	/* 0X50 */
	XPM_NODEIDX_RST_APU3_CORE3_WARM,	/* 0X51 */
	XPM_NODEIDX_RST_APU3_CLUSTER_WARM,	/* 0X52 */
	XPM_NODEIDX_RST_FPX_SWDT3,	/* 0X53 */
	XPM_NODEIDX_RST_APU1_CORE1_WARM,	/* 0X54 */
	XPM_NODEIDX_RST_APU1_CORE3_COLD,	/* 0X55 */
	XPM_NODEIDX_RST_APU1_CORE0_COLD,	/* 0X56 */
	XPM_NODEIDX_RST_APU1_CORE1_COLD,	/* 0X57 */
	XPM_NODEIDX_RST_APU1_CLUSTER_COLD,	/* 0X58 */
	XPM_NODEIDX_RST_APU1_CORE0_WARM,	/* 0X59 */
	XPM_NODEIDX_RST_APU1_CORE2_COLD,	/* 0X5A */
	XPM_NODEIDX_RST_APU1_CORE2_WARM,	/* 0X5B */
	XPM_NODEIDX_RST_APU1_CORE3_WARM,	/* 0X5C */
	XPM_NODEIDX_RST_APU1_CLUSTER_WARM,	/* 0X5D */
	XPM_NODEIDX_RST_CPI,	/* 0X5E */
	XPM_NODEIDX_RST_APU0_CORE1_WARM,	/* 0X5F */
	XPM_NODEIDX_RST_APU0_CORE3_COLD,	/* 0X60 */
	XPM_NODEIDX_RST_APU0_CORE0_COLD,	/* 0X61 */
	XPM_NODEIDX_RST_APU0_CORE1_COLD,	/* 0X62 */
	XPM_NODEIDX_RST_APU0_CLUSTER_COLD,	/* 0X63 */
	XPM_NODEIDX_RST_APU0_CORE0_WARM,	/* 0X64 */
	XPM_NODEIDX_RST_APU0_CORE2_COLD,	/* 0X65 */
	XPM_NODEIDX_RST_APU0_CORE2_WARM,	/* 0X66 */
	XPM_NODEIDX_RST_APU0_CORE3_WARM,	/* 0X67 */
	XPM_NODEIDX_RST_APU0_CLUSTER_WARM,	/* 0X68 */
	XPM_NODEIDX_RST_TIMESTAMP,	/* 0X69 */
	XPM_NODEIDX_RST_FPX_SWDT0,	/* 0X6A */
	XPM_NODEIDX_RST_GIC,	/* 0X6B */
	XPM_NODEIDX_RST_APU2_CORE1_WARM,	/* 0X6C */
	XPM_NODEIDX_RST_APU2_CORE3_COLD,	/* 0X6D */
	XPM_NODEIDX_RST_APU2_CORE0_COLD,	/* 0X6E */
	XPM_NODEIDX_RST_APU2_CORE1_COLD,	/* 0X6F */
	XPM_NODEIDX_RST_APU2_CLUSTER_COLD,	/* 0X70 */
	XPM_NODEIDX_RST_APU2_CORE0_WARM,	/* 0X71 */
	XPM_NODEIDX_RST_APU2_CORE2_COLD,	/* 0X72 */
	XPM_NODEIDX_RST_APU2_CORE2_WARM,	/* 0X73 */
	XPM_NODEIDX_RST_APU2_CORE3_WARM,	/* 0X74 */
	XPM_NODEIDX_RST_APU2_CLUSTER_WARM,	/* 0X75 */
	XPM_NODEIDX_RST_CMN,	/* 0X76 */
	XPM_NODEIDX_RST_CMN_CXS,	/* 0X77 */
	XPM_NODEIDX_RST_CMN_CGL,	/* 0X78 */
	XPM_NODEIDX_RST_USB0,	/* 0X79 */
	XPM_NODEIDX_RST_USB1,	/* 0X7A */
	XPM_NODEIDX_RST_IPI,	/* 0X7B */
	XPM_NODEIDX_RST_SWDT1,	/* 0X7C */
	XPM_NODEIDX_RST_SWDT0,	/* 0X7D */
	XPM_NODEIDX_RST_OCM,	/* 0X7E */
	XPM_NODEIDX_RST_TTC_TTC1,	/* 0X7F */
	XPM_NODEIDX_RST_TTC_TTC0,	/* 0X80 */
	XPM_NODEIDX_RST_TTC_TTC3,	/* 0X81 */
	XPM_NODEIDX_RST_TTC_TTC2,	/* 0X82 */
	XPM_NODEIDX_RST_RPU_A_GD,	/* 0X83 */
	XPM_NODEIDX_RST_RPU_B_GD,	/* 0X84 */
	XPM_NODEIDX_RST_RPU_CORE0A,	/* 0X85 */
	XPM_NODEIDX_RST_RPU_CORE0A_POR,	/* 0X86 */
	XPM_NODEIDX_RST_RPU_CORE0B_POR,	/* 0X87 */
	XPM_NODEIDX_RST_RPU_A_GD_TOP,	/* 0X88 */
	XPM_NODEIDX_RST_RPU_CORE1B,	/* 0X89 */
	XPM_NODEIDX_RST_RPU_B_TOPRESET,	/* 0X8A */
	XPM_NODEIDX_RST_RPU_CORE1B_POR,	/* 0X8B */
	XPM_NODEIDX_RST_RPU_CORE1A,	/* 0X8C */
	XPM_NODEIDX_RST_RPU_B_GD_TOP,	/* 0X8D */
	XPM_NODEIDX_RST_RPU_A_TOPRESET,	/* 0X8E */
	XPM_NODEIDX_RST_RPU_B_DBGRST,	/* 0X8F */
	XPM_NODEIDX_RST_RPU_A_DCLS_TOPRESET,	/* 0X90 */
	XPM_NODEIDX_RST_RPU_CORE1A_POR,	/* 0X91 */
	XPM_NODEIDX_RST_RPU_B_DCLS_TOPRESET,	/* 0X92 */
	XPM_NODEIDX_RST_RPU_A_DBGRST,	/* 0X93 */
	XPM_NODEIDX_RST_RPU_CORE0B,	/* 0X94 */
	XPM_NODEIDX_RST_CPM_POR,	/* 0X95 */
	XPM_NODEIDX_RST_SPARE,	/* 0X96 */
	XPM_NODEIDX_RST_ADMA,	/* 0X97 */
	XPM_NODEIDX_RST_I3C1,	/* 0X98 */
	XPM_NODEIDX_RST_I3C0,	/* 0X99 */
	XPM_NODEIDX_RST_FPX_POR,	/* 0X9A */
	XPM_NODEIDX_RST_FPX_SRST,	/* 0X9B */
	XPM_NODEIDX_RST_DBG_LPX,	/* 0X9C */
	XPM_NODEIDX_RST_DBG_LPX_HSDP,	/* 0X9D */
	XPM_NODEIDX_RST_PSM_MODE_WAKEUP,	/* 0X9E */
	XPM_NODEIDX_RST_PSM_MODE_MODE,	/* 0X9F */
	XPM_NODEIDX_RST_SPI0,	/* 0XA0 */
	XPM_NODEIDX_RST_SPI1,	/* 0XA1 */
	XPM_NODEIDX_RST_UART1,	/* 0XA2 */
	XPM_NODEIDX_RST_UART0,	/* 0XA3 */
	XPM_NODEIDX_RST_CAN0,	/* 0XA4 */
	XPM_NODEIDX_RST_CAN1,	/* 0XA5 */
	XPM_NODEIDX_RST_GEM1,	/* 0XA6 */
	XPM_NODEIDX_RST_GEM0,	/* 0XA7 */
	XPM_NODEIDX_RST_CPM5N_SYSMON_CFG,	/* 0XA8 */
	XPM_NODEIDX_RST_PMC_SYSMON_CFG,	/* 0XA9 */
	XPM_NODEIDX_RST_CPM5N_SYSMON_SEQ,	/* 0XAA */
	XPM_NODEIDX_RST_PMC_SYSMON_SEQ,	/* 0XAB */
	XPM_NODEIDX_RST_CPM5N_DBG,	/* 0XAC */
	XPM_NODEIDX_RST_PMC_DBG,	/* 0XAD */
	XPM_NODEIDX_RST_LPD_SYSMON_CFG,	/* 0XAE */
	XPM_NODEIDX_RST_LPD_SYSMON_SEQ,	/* 0XAF */
	XPM_NODEIDX_RST_PMC_GPIO,	/* 0XB0 */
	XPM_NODEIDX_RST_LPD_GPIO,	/* 0XB1 */
	XPM_NODEIDX_RST_FPD_TIMESTAMP,	/* 0XB2 */
	XPM_NODEIDX_RST_LPD_TIMESTAMP,	/* 0XB3 */
	XPM_NODEIDX_RST_MAX	/* 0XB4 */
} XPm_ResetNodeIdx;

/* Device nodes */
typedef enum {
	XPM_NODEIDX_DEV_MIN,	/* 0X0 */
	XPM_NODEIDX_DEV_PMC_PROC,	/* 0X1 */
	XPM_NODEIDX_DEV_PSM_PROC,	/* 0X2 */
	XPM_NODEIDX_DEV_ACPU_0_0,	/* 0X3 */
	XPM_NODEIDX_DEV_ACPU_0_1,	/* 0X4 */
	XPM_NODEIDX_DEV_ACPU_0_2,	/* 0X5 */
	XPM_NODEIDX_DEV_ACPU_0_3,	/* 0X6 */
	XPM_NODEIDX_DEV_ACPU_1_0,	/* 0X7 */
	XPM_NODEIDX_DEV_ACPU_1_1,	/* 0X8 */
	XPM_NODEIDX_DEV_ACPU_1_2,	/* 0X9 */
	XPM_NODEIDX_DEV_ACPU_1_3,	/* 0XA */
	XPM_NODEIDX_DEV_ACPU_2_0,	/* 0XB */
	XPM_NODEIDX_DEV_ACPU_2_1,	/* 0XC */
	XPM_NODEIDX_DEV_ACPU_2_2,	/* 0XD */
	XPM_NODEIDX_DEV_ACPU_2_3,	/* 0XE */
	XPM_NODEIDX_DEV_ACPU_3_0,	/* 0XF */
	XPM_NODEIDX_DEV_ACPU_3_1,	/* 0X10 */
	XPM_NODEIDX_DEV_ACPU_3_2,	/* 0X11 */
	XPM_NODEIDX_DEV_ACPU_3_3,	/* 0X12 */
	XPM_NODEIDX_DEV_RPU_A_0,	/* 0X13 */
	XPM_NODEIDX_DEV_RPU_A_1,	/* 0X14 */
	XPM_NODEIDX_DEV_RPU_B_0,	/* 0X15 */
	XPM_NODEIDX_DEV_RPU_B_1,	/* 0X16 */
	XPM_NODEIDX_DEV_OCM_0_0,	/* 0X17 */
	XPM_NODEIDX_DEV_OCM_0_1,	/* 0X18 */
	XPM_NODEIDX_DEV_OCM_0_2,	/* 0X19 */
	XPM_NODEIDX_DEV_OCM_0_3,	/* 0X1A */
	XPM_NODEIDX_DEV_OCM_1_0,	/* 0X1B */
	XPM_NODEIDX_DEV_OCM_1_1,	/* 0X1C */
	XPM_NODEIDX_DEV_OCM_1_2,	/* 0X1D */
	XPM_NODEIDX_DEV_OCM_1_3,	/* 0X1E */
	XPM_NODEIDX_DEV_TCM_A_0A,	/* 0X1F */
	XPM_NODEIDX_DEV_TCM_A_0B,	/* 0X20 */
	XPM_NODEIDX_DEV_TCM_A_0C,	/* 0X21 */
	XPM_NODEIDX_DEV_TCM_A_1A,	/* 0X22 */
	XPM_NODEIDX_DEV_TCM_A_1B,	/* 0X23 */
	XPM_NODEIDX_DEV_TCM_A_1C,	/* 0X24 */
	XPM_NODEIDX_DEV_TCM_B_0A,	/* 0X25 */
	XPM_NODEIDX_DEV_TCM_B_0B,	/* 0X26 */
	XPM_NODEIDX_DEV_TCM_B_0C,	/* 0X27 */
	XPM_NODEIDX_DEV_TCM_B_1A,	/* 0X28 */
	XPM_NODEIDX_DEV_TCM_B_1B,	/* 0X29 */
	XPM_NODEIDX_DEV_TCM_B_1C,	/* 0X2A */
	XPM_NODEIDX_DEV_UART_0,	/* 0X2B */
	XPM_NODEIDX_DEV_UART_1,	/* 0X2C */
	XPM_NODEIDX_DEV_SPI_0,	/* 0X2D */
	XPM_NODEIDX_DEV_SPI_1,	/* 0X2E */
	XPM_NODEIDX_DEV_QSPI,	/* 0X2F */
	XPM_NODEIDX_DEV_OSPI,	/* 0X30 */
	XPM_NODEIDX_DEV_SDIO_0,	/* 0X31 */
	XPM_NODEIDX_DEV_USB_0,	/* 0X32 */
	XPM_NODEIDX_DEV_USB_1,	/* 0X33 */
	XPM_NODEIDX_DEV_DDR_0,	/* 0X34 */
	XPM_NODEIDX_DEV_PMC_WWDT,	/* 0X35 */
	XPM_NODEIDX_DEV_IPI_0,	/* 0X36 */
	XPM_NODEIDX_DEV_IPI_1,	/* 0X37 */
	XPM_NODEIDX_DEV_IPI_2,	/* 0X38 */
	XPM_NODEIDX_DEV_IPI_3,	/* 0X39 */
	XPM_NODEIDX_DEV_IPI_4,	/* 0X3A */
	XPM_NODEIDX_DEV_IPI_5,	/* 0X3B */
	XPM_NODEIDX_DEV_IPI_6,	/* 0X3C */
	XPM_NODEIDX_DEV_SOC,	/* 0X3D */
	XPM_NODEIDX_DEV_MAX	/* 0X3E */
} XPm_DeviceNodeIdx;

/* isolation subclasses */
typedef enum {
	XPM_NODEIDX_ISO_PL_MISC_N,	/* 0X0 */
	XPM_NODEIDX_ISO_PL_TEST_N,	/* 0X1 */
	XPM_NODEIDX_ISO_TNOC_AXI_N,	/* 0X2 */
	XPM_NODEIDX_ISO_PL_AXIST0_N,	/* 0X3 */
	XPM_NODEIDX_ISO_PL_AXIST1_N,	/* 0X4 */
	XPM_NODEIDX_ISO_PMC_PL,	/* 0X5 */
	XPM_NODEIDX_ISO_LPD_SOC,	/* 0X6 */
	XPM_NODEIDX_ISO_VCCAUX_SOC,	/* 0X7 */
	XPM_NODEIDX_ISO_LPD_CPM_DFX,	/* 0X8 */
	XPM_NODEIDX_ISO_PMC_PL_CFRAME,	/* 0X9 */
	XPM_NODEIDX_ISO_PMC_SOC,	/* 0XA */
	XPM_NODEIDX_ISO_LPD_CPM,	/* 0XB */
	XPM_NODEIDX_ISO_PL_SOC,	/* 0XC */
	XPM_NODEIDX_ISO_FPD_PL_TEST,	/* 0XD */
	XPM_NODEIDX_ISO_FPD_SOC,	/* 0XE */
	XPM_NODEIDX_ISO_VCCRAM_SOC,	/* 0XF */
	XPM_NODEIDX_ISO_LPD_PL,	/* 0X10 */
	XPM_NODEIDX_ISO_PMC_PL_TEST,	/* 0X11 */
	XPM_NODEIDX_ISO_PMC_LPD_DFX,	/* 0X12 */
	XPM_NODEIDX_ISO_PMC_SOC_NPI,	/* 0X13 */
	XPM_NODEIDX_ISO_FPD_PL,	/* 0X14 */
	XPM_NODEIDX_ISO_VCCAUX_VCCRAM,	/* 0X15 */
	XPM_NODEIDX_ISO_LPD_PL_TEST,	/* 0X16 */
	XPM_NODEIDX_ISO_PMC_LPD,	/* 0X17 */
	XPM_NODEIDX_ISO_CPM5_PL,	/* 0X18 */
	XPM_NODEIDX_ISO_FPD_CPM5,	/* 0X19 */
	XPM_NODEIDX_ISO_LPD_FPD_DFX,	/* 0X1A */
	XPM_NODEIDX_ISO_LPD_FPD,	/* 0X1B */
	XPM_NODEIDX_ISO_CPM5_LPD_DFX,	/* 0X1C */
	XPM_NODEIDX_ISO_FPD_CPM5_DFX,	/* 0X1D */
	XPM_NODEIDX_ISO_LPD_OCM2_DFX,	/* 0X1E */
	XPM_NODEIDX_ISO_CPM5_GT_DFX,	/* 0X1F */
	XPM_NODEIDX_ISO_LPD_OCM2,	/* 0X20 */
	XPM_NODEIDX_ISO_CPM5_LPD,	/* 0X21 */
	XPM_NODEIDX_ISO_CPM5_GT,	/* 0X22 */
	XPM_NODEIDX_ISO_CPM5_PL_DFX,	/* 0X23 */
	XPM_NODEIDX_ISO_MAX	/* 0X24 */
} XPm_IsolationIdx;

/* subsystem index */
typedef enum {
	XPM_NODEIDX_SUBSYS_DEFAULT,	/* 0X0 */
	XPM_NODEIDX_SUBSYS_PMC,	/* 0X1 */
	XPM_NODEIDX_SUBSYS_MAX	/* 0X2 */
} XPm_SubsystemIdx;
#endif /* XPM_NODE_H_ */
