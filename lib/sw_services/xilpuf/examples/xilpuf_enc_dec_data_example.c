/******************************************************************************
* Copyright (c) 2020 - 2023 Xilinx, Inc.  All rights reserved.
* Copyright (c) 2022 - 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
 *****************************************************************************/

 /*****************************************************************************/
 /**
 *
 * @file xilpuf_enc_dec_data_example.c
 *
 * This file illustrates encryption and decryption of user data using PUF KEY.
 * The key can be generated using either PUF registration or PUF on demand
 * regeneration.
 * To build this application, xilmailbox library must be included in BSP and
 * xilsecure must be in client mode and xilpuf in server mode
 *
 * This example is supported for Versal and Versal Net devices.
 *
 * <pre>
 * MODIFICATION HISTORY:
 *
 * Ver   Who   Date     Changes
 * ----- ---  -------- -------------------------------------------------------
 * 1.0   har  01/30/20 Initial release
 * 1.1   har  01/30/20 Updated file version to 1.1 to sync with library version
 *       har  03/08/20 Added function to print array
 *                     Corrected endianness of PUF helper data
 * 1.2   har  07/03/20 Corrected the length of PUF ID passed in XPuf_ShowData
 *       am   08/14/20 Replacing function prototype and local status variable
 *                     from u32 and s32 to int.
 *       har  09/30/20 Replaced XPuf_printf with xil_printf
 * 1.3   har  01/04/21 Added check and updated comments for
 *                     XPUF_DATA_LEN_IN_BYTES
 *       har  04/14/21 Modified code to use client side APIs of Xilsecure
 * 1.4   kpt  12/02/21 Replaced standard library utility functions with
 *                     xilinx maintained functions
 *       har  01/20/22 Removed inclusion of xil_mem.h
 *       har  03/04/22 Added comment to specify mode of libraries
 *                     Added shared memory allocation for client APIs
 *       kpt  03/18/22 Removed IPI related code and added mailbox support
 *       har  03/31/22 Updated default data and data size
 * 2.1   am   04/13/23 Fix PUF auxiliary convergence error
 * 2.2   am   05/03/23 Added KAT before crypto usage
 * 	 vss  11/22/23 Added header file required for SDT flow
 *
 * @note
 *
 * User configurable parameters for PUF
 *------------------------------------------------------------------------------
 * #define XPUF_DATA				"0123456789ABCDEF0123456789ABCDEF"
 * Data to be encrypted by PUF KEY should be provided in string format.
 *
 * #define XPUF_DATA_LEN_IN_BYTES		(16U)
 * Length of data to be encrypted should be provided in bytes, where number of
 * bytes must be a multiple of 4.
 *
 * #define XPUF_IV				"000000000000000000000000"
 * IV should be provided in string format. It should be 24 characters long, valid
 * characters are 0-9, a-f, A-F. Any other character is considered as invalid
 * string. The value mentioned here will be converted to hex buffer. It is used
 * with the AES-GCM cryptographic hardware in order to encrypt user data.
 *
 * #define XPUF_KEY_GENERATE_OPTION		(XPUF_REGISTRATION)
 *							(or)
 *						(XPUF_REGEN_ON_DEMAND)
 * PUF helper data can be generated by PUF registration or PUF on-demand
 * regeneration. The user can configure XPUF_KEY_GENERATE_OPTION as either
 * XPUF_REGISTRATION or XPUF_REGEN_ON_DEMAND to select the mode of PUF operation
 * to generate helper data
 *
 * #define XPUF_READ_HD_OPTION			(XPUF_READ_FROM_RAM)
 *							(or)
 *						(XPUF_READ_FROM_EFUSE_CACHE)
 * This selects the location from where the helper data must be read by the
 * application. This option must be configured if XPUF_KEY_GENERATE_OPTION
 * is configured as XPUF_REGEN_ON_DEMAND.
 *
 * #define XPUF_CHASH				(0x00000000)
 * The length of CHASH should be 24 bits. It is valid only for PUF regeneration
 * and invalid for PUF registration. CHASH value should be supplied if
 * XPUF_READ_HD_OPTION is configured as XPUF_READ_FROM_RAM.
 *
 * #define XPUF_AUX				(0x00000000)
 * The length of AUX should be 32 bits. It is valid only for PUF regeneration
 * and invalid for PUF registration. AUX value should be supplied if
 * XPUF_READ_HD_OPTION is configured as XPUF_READ_FROM_RAM.
 *
 * #define XPUF_SYN_DATA_ADDRESS		(0x00000000)
 * Address of syndrome data should be supplied if XPUF_READ_HD_OPTION is
 * configured as XPUF_READ_FROM_RAM.
 *
 * #define XPUF_GLBL_VAR_FLTR_OPTION	(TRUE)
 * It is recommended to always enable this option to ensure entropy. It can
 * be configured as FALSE to disable Global Variation Filter.
 *
 *****************************************************************************/
/***************************** Include Files *********************************/
#ifdef SDT
#include "xpuf_bsp_config.h"
#endif
#include "xpuf.h"
#include "xsecure_aesclient.h"
#include "xsecure_katclient.h"
#include "xil_util.h"
#include "xil_cache.h"

/************************** Constant Definitions ****************************/
/* User configurable parameters start*/
#define XPUF_DATA 			"0123456789ABCDEF0123456789ABCDEF"
#define XPUF_DATA_LEN_IN_BYTES		(16U)
					/* Data length in Bytes */
#define XPUF_IV				"000000000000000000000000"

#define XPUF_KEY_GENERATE_OPTION	(XPUF_REGISTRATION)

#if (XPUF_KEY_GENERATE_OPTION == XPUF_REGISTRATION)
#define XPUF_WRITE_HD_OPTION 		(XPUF_WRITE_ON_UART)
#endif

#if (XPUF_KEY_GENERATE_OPTION == XPUF_REGEN_ON_DEMAND)
#define XPUF_READ_HD_OPTION		(XPUF_READ_FROM_RAM)
#define XPUF_CHASH			(0x00000000U)
#define XPUF_AUX			(0x00000000U)
#define XPUF_SYN_DATA_ADDRESS		(0x00000000U)
#endif

#define XPUF_GLBL_VAR_FLTR_OPTION	(TRUE)

#if defined (VERSAL_NET)
#define PUF_RO_SWAP			(0x00000000)
#endif

/*User configurable parameters end */

#define XPUF_IV_LEN_IN_BYTES		(12U)
					/* IV Length in bytes */
#define XPUF_DATA_LEN_IN_BITS		(XPUF_DATA_LEN_IN_BYTES * 8U)
					/* Data length in Bits */
#define XPUF_IV_LEN_IN_BITS		(XPUF_IV_LEN_IN_BYTES * 8U)
					/* IV length in Bits */
#define XPUF_GCM_TAG_SIZE		(16U)
					/* GCM tag Length in bytes */
#define XPUF_HD_LEN_IN_WORDS		(386U)
#define XPUF_ID_LEN_IN_BYTES		(XPUF_ID_LEN_IN_WORDS * XPUF_WORD_LENGTH)

/*
 * Below macro values should match with enum XSecure_AesKeySize.
 * As preprocessor can't handle enums at preprocessing stage of compilation,
 * these macros are defined.
 */
#define XPUF_RED_KEY_SIZE_128		(0U)
#define XPUF_RED_KEY_SIZE_256		(2U)

/************************** Type Definitions *********************************/
static XPuf_Data PufData;

/* shared memory allocation */
static u8 SharedMem[XPUF_IV_LEN_IN_BYTES + XSECURE_SHARED_MEM_SIZE] __attribute__((aligned(64U)))
						__attribute__ ((section (".data.Data")));

#if defined (__GNUC__)
static u8 Data[XPUF_DATA_LEN_IN_BYTES]__attribute__ ((aligned (64)))
				__attribute__ ((section (".data.Data")));
static u8 DecData[XPUF_DATA_LEN_IN_BYTES]__attribute__ ((aligned (64)))
				__attribute__ ((section (".data.DecData")));
static u8 EncData[XPUF_DATA_LEN_IN_BYTES]__attribute__ ((aligned (64)))
				__attribute__ ((section (".data.EncData")));
static u8 GcmTag[XPUF_GCM_TAG_SIZE]__attribute__ ((aligned (64)))
				__attribute__ ((section (".data.GcmTag")));
#elif defined (__ICCARM__)
#pragma data_alignment = 64
static u8 Data[XPUF_DATA_LEN_IN_BYTES];
#pragma data_alignment = 64
static u8 DecData[XPUF_DATA_LEN_IN_BYTES];
#pragma data_alignment = 64
static u8 EncData[XPUF_DATA_LEN_IN_BYTES];
#pragma data_alignment = 64
static u8 GcmTag[XPUF_GCM_TAG_SIZE];
#endif

/************************** Function Prototypes ******************************/
static int XPuf_GenerateKey(void);
static int XPuf_VerifyDataEncDec(void);
static void XPuf_ShowData(const u8* Data, u32 Len);

/************************** Function Definitions *****************************/
int main(void)
{
	int Status = XST_FAILURE;

	#ifdef XPUF_CACHE_DISABLE
		Xil_DCacheDisable();
	#endif

	/* Generate PUF KEY
	 */
	Status = XPuf_GenerateKey();
	if (Status == XST_SUCCESS) {
		xil_printf("Successfully generated PUF KEY %x\r\n", Status);
	}
	else {
		xil_printf("PUF KEY generation failed %x\r\n", Status);
		goto END;
	}

	/* Encryption using PUF KEY followed by decryption and then comparing
	 * decrypted data with original data */
	Status = XPuf_VerifyDataEncDec();
	if (Status == XST_SUCCESS) {
		xil_printf("Successfully encrypted and decrypted user data %x\r\n", Status);
	}
	else {
		xil_printf("Encryption/Decryption failed %x\r\n", Status);
	}

	if (Status == XST_SUCCESS) {
		xil_printf("Successfully ran Xilpuf enc dec data example\r\n");
	}

END:
	return Status;
}

/******************************************************************************/
/**
 *
 * @brief	This function generates PUF KEY by PUF registration or
 * 		PUF on demand regeneration as per the user provided inputs.
 *
 * @param	None.
 *
 * @return
 *		XST_SUCCESS - If PUF_KEY generation was successful.
 * 		XPUF_ERROR_INVALID_PARAM - PufData is NULL.
 *		XPUF_ERROR_INVALID_SYNDROME_MODE  - Incorrect Registration mode.
 *		XPUF_ERROR_SYNDROME_WORD_WAIT_TIMEOUT - Timeout occurred while
 *			 waiting for PUF Syndrome data.
 *		XPUF_ERROR_SYNDROME_DATA_OVERFLOW - Syndrome data overflow
 *			reported by PUF controller or more than required data
 *			 is provided by PUF controller.
 *		XPUF_ERROR_SYNDROME_DATA_UNDERFLOW - Number of syndrome data words
 *			words are less than expected number of words.
 *		XPUF_ERROR_INVALID_REGENERATION_TYPE - Selection of invalid
 *			regeneration type.
 *		XPUF_ERROR_CHASH_NOT_PROGRAMMED - Helper data not provided.
 *		XPUF_ERROR_PUF_STATUS_DONE_TIMEOUT - Timeout before Status was done.
 *		XST_FAILURE - PUF KEY generation failed.
 *
 ******************************************************************************/
static int XPuf_GenerateKey(void)
{
	int Status = XST_FAILURE;
#if (XPUF_KEY_GENERATE_OPTION == XPUF_REGISTRATION)
	u32 PUF_HelperData[XPUF_HD_LEN_IN_WORDS] = {0U};
#endif

	PufData.ShutterValue = XPUF_SHUTTER_VALUE;
	PufData.GlobalVarFilter = XPUF_GLBL_VAR_FLTR_OPTION;
#if defined (VERSAL_NET)
	PufData.RoSwapVal = PUF_RO_SWAP;
#endif

	PufData.PufOperation = XPUF_KEY_GENERATE_OPTION;
#if (XPUF_KEY_GENERATE_OPTION == XPUF_REGISTRATION)
	Status = XPuf_Registration(&PufData);
	if (Status != XST_SUCCESS) {
		goto END;
	}
	xil_printf("Provided PUF helper on UART\r\n");
	xil_printf("PUF Helper data Start\r\n");
	Status = Xil_SMemCpy(PUF_HelperData, XPUF_4K_PUF_SYN_LEN_IN_BYTES,
			PufData.SyndromeData, XPUF_4K_PUF_SYN_LEN_IN_BYTES,
			XPUF_4K_PUF_SYN_LEN_IN_BYTES);
	if (Status != XST_SUCCESS) {
		goto END;
	}
	PUF_HelperData[XPUF_HD_LEN_IN_WORDS - 2U] = PufData.Chash;
	/** PLM left shifts the the AUX value by 4-bits for NVM provisioning. During regeneration
	 *  from BootHeader, the ROM expects PUF AUX value in below format:
	 *
	 *	-------------------------------------------------------------------------
	 *	|    0x00    |              AUX (23:4)                   | AUX_EN (3:0) |
	 *	-------------------------------------------------------------------------
	 *
	 *  Any non-zero value in AUX_EN is causing PUF convergence error. Hence left shifting the
	 *  AUX value by 4-bits.
	 */
	PUF_HelperData[XPUF_HD_LEN_IN_WORDS - 1U] = (PufData.Aux << XIL_SIZE_OF_NIBBLE_IN_BITS);
	XPuf_ShowData((u8*)PUF_HelperData, XPUF_HD_LEN_IN_WORDS * XPUF_WORD_LENGTH);
	xil_printf("Chash: %02x \r\n", PufData.Chash);
	xil_printf("Aux: %02x \r\n", PufData.Aux);
	xil_printf("PUF Helper data End\r\n");
	xil_printf("PUF ID : ");
	XPuf_ShowData((u8*)PufData.PufID, XPUF_ID_LEN_IN_BYTES);

#elif (XPUF_KEY_GENERATE_OPTION == XPUF_REGEN_ON_DEMAND)
	PufData.ReadOption = XPUF_READ_HD_OPTION;
	if (PufData.ReadOption == XPUF_READ_FROM_RAM) {
		PufData.Chash = XPUF_CHASH;
		PufData.Aux = XPUF_AUX;
		PufData.SyndromeAddr = XPUF_SYN_DATA_ADDRESS;
		xil_printf("Reading helper data from provided address\r\n");
	}
	else if (PufData.ReadOption == XPUF_READ_FROM_EFUSE_CACHE) {
		xil_printf("Reading helper data from eFUSE\r\n");
	}
	else {
		xil_printf("Invalid read option for reading helper data\r\n");
	}
	Status = XPuf_Regeneration(&PufData);
	if (Status != XST_SUCCESS) {
		xil_printf("Puf Regeneration failed with error:%x\r\n", Status);
		goto END;
	}
	xil_printf("PUF ID : ");
	XPuf_ShowData((u8*)PufData.PufID, XPUF_ID_LEN_IN_BYTES);
#else
	#error "Invalid option selected for generating PUF KEY. Only Puf \
registration and on demand regeneration are allowed"
#endif

END:
	return Status;
}

/******************************************************************************/
/**
 *
 * @brief	This function verifies the encryption and decryption of the data
 *		blob using PUF key.
 *
 * @param	None.
 *
 * @return
 *		XST_SUCCESS - When the encryption and decryption of data is
 *			successfully verified
 *  		XNVM_EFUSE_ERR_INVALID_PARAM - On Invalid Parameter.
 *		XST_FAILURE - On failure of AES Encrypt Initialization,
 *			AES Encrypt data, format AES key and AES decrypt
 *			Initialization and AES decrypt data.
 *
 ******************************************************************************/
static int XPuf_VerifyDataEncDec(void)
{
	int Status = XST_FAILURE;
	u32 Index;
	u8 *Iv = &SharedMem[0U];
	XSecure_ClientInstance SecureClientInstance;
	XMailbox MailboxInstance;

	Status = XMailbox_Initialize(&MailboxInstance, 0U);
	if (Status != XST_SUCCESS) {
		xil_printf("Mailbox initialization failed %x\r\n", Status);
		goto END;
	}

	Status = XSecure_ClientInit(&SecureClientInstance, &MailboxInstance);
	if (Status != XST_SUCCESS) {
		xil_printf("Client initialization failed %x\r\n", Status);
	}

	/* Set shared memory */
	Status = XMailbox_SetSharedMem(&MailboxInstance, (u64)(UINTPTR)(SharedMem + XPUF_IV_LEN_IN_BYTES),
			XSECURE_SHARED_MEM_SIZE);
	if (Status != XST_SUCCESS) {
		xil_printf("\r\n shared memory initialization failed");
		goto END;
	}

	if (Xil_Strnlen(XPUF_IV, (XPUF_IV_LEN_IN_BYTES * 2U)) ==
				(XPUF_IV_LEN_IN_BYTES * 2U)) {
		Status = Xil_ConvertStringToHexBE((const char *)(XPUF_IV), Iv,
						XPUF_IV_LEN_IN_BITS);
		if (Status != XST_SUCCESS) {
			xil_printf("String Conversion error (IV):%08x !!!\r\n",
				Status);
			goto END;
		}
	}
	else {
		xil_printf("Provided IV length is wrong\r\n");
		goto END;
	}

	if (Xil_Strnlen(XPUF_DATA, ((XPUF_DATA_LEN_IN_BYTES * 2U) + 1U)) ==
				(XPUF_DATA_LEN_IN_BYTES * 2U)) {
		Status = Xil_ConvertStringToHexBE(
			(const char *) (XPUF_DATA),
			Data, XPUF_DATA_LEN_IN_BITS);
		if (Status != XST_SUCCESS) {
			xil_printf("String Conversion error (Data):%08x !!!\r\n",
				Status);
			goto END;
		}
	}
	else {
		Status = XST_FAILURE;
		xil_printf("Provided XPUF_DATA_LEN is wrong\r\n");
		goto END;
	}

	if (XPUF_DATA_LEN_IN_BYTES == 0U) {
		Status = XST_FAILURE;
		xil_printf("Zero data length is not accepted\r\n");
		goto END;
	}

	if (XPUF_DATA_LEN_IN_BYTES % XPUF_WORD_LENGTH != 0U) {
		Status = XST_FAILURE;
		xil_printf("Provided XPUF_DATA_LEN is not multiple of 4\r\n");
		goto END;
	}

	xil_printf("Data to be encrypted: \n\r");
	XPuf_ShowData((u8*)Data, XPUF_DATA_LEN_IN_BYTES);

	xil_printf("IV: \n\r");
	XPuf_ShowData((u8*)Iv, XPUF_IV_LEN_IN_BYTES);

	Xil_DCacheFlushRange((UINTPTR)Iv, XPUF_IV_LEN_IN_BYTES);
	Xil_DCacheFlushRange((UINTPTR)Data, XPUF_DATA_LEN_IN_BYTES);

	/* Run KAT for the Aes driver so that it's ready to use */
	Status = XSecure_AesEncryptKat(&SecureClientInstance);
	if (Status != XST_SUCCESS) {
		xil_printf("Aes Encrypt KAT failed %x\n\r", Status);
		goto END;
	}

	Xil_DCacheInvalidateRange((UINTPTR)EncData, XPUF_DATA_LEN_IN_BYTES);
	Xil_DCacheInvalidateRange((UINTPTR)GcmTag, XPUF_GCM_TAG_SIZE);

	/* Encryption of Data */
	Status = XSecure_AesEncryptData(&SecureClientInstance, XSECURE_AES_PUF_KEY,
		XPUF_RED_KEY_SIZE_256, (UINTPTR)Iv, (UINTPTR)Data,
		(UINTPTR)EncData, XPUF_DATA_LEN_IN_BYTES, (UINTPTR)GcmTag);
	if (Status != XST_SUCCESS) {
		xil_printf("Data encryption failed %x\n\r", Status);
		goto END;
	}

	Xil_DCacheInvalidateRange((UINTPTR)EncData, XPUF_DATA_LEN_IN_BYTES);
	Xil_DCacheInvalidateRange((UINTPTR)GcmTag, XPUF_GCM_TAG_SIZE);

	xil_printf("Encrypted data: \n\r");
	XPuf_ShowData((u8*)EncData, XPUF_DATA_LEN_IN_BYTES);

	xil_printf("GCM tag: \n\r");
	XPuf_ShowData((u8*)GcmTag, XPUF_GCM_TAG_SIZE);

	/* Run KAT for the Aes driver so that it's ready to use */
	Status = XSecure_AesDecryptKat(&SecureClientInstance);
	if (Status != XST_SUCCESS) {
		xil_printf("Aes Decrypt KAT failed %x\n\r", Status);
		goto END;
	}

	Xil_DCacheInvalidateRange((UINTPTR)DecData, XPUF_DATA_LEN_IN_BYTES);

	/* Decryption of Data */
	Status = XSecure_AesDecryptData(&SecureClientInstance, XSECURE_AES_PUF_KEY,
		XPUF_RED_KEY_SIZE_256, (UINTPTR)Iv, (UINTPTR)EncData,
		(UINTPTR)DecData, XPUF_DATA_LEN_IN_BYTES, (UINTPTR)GcmTag);
	if (Status != XST_SUCCESS) {
		xil_printf("Data encryption failed %x\n\r", Status);
		goto END;
	}

	Xil_DCacheInvalidateRange((UINTPTR)DecData, XPUF_DATA_LEN_IN_BYTES);

	xil_printf("Decrypted data \n\r ");
	XPuf_ShowData((u8*)DecData, XPUF_DATA_LEN_IN_BYTES);

	/* Comparison of Decrypted Data with original data */
	for(Index = 0U; Index < XPUF_DATA_LEN_IN_BYTES; Index++) {
		if (Data[Index] != DecData[Index]) {
			xil_printf("Failure during comparison of the data\n\r");
			Status = XST_FAILURE;
		}
	}
END:
	Status |= XMailbox_ReleaseSharedMem(&MailboxInstance);
	return Status;
}

/******************************************************************************/
/**
 *
 * @brief	This function prints the data array.
 *
 * @param	Data - Pointer to the data to be printed.
 * @param	Len  - Length of the data in bytes.
 *
 * @return	None.
 *
 ******************************************************************************/
static void XPuf_ShowData(const u8* Data, u32 Len)
{
	u32 Index;

	for (Index = 0U; Index < Len; Index++) {
		xil_printf("%02x", Data[Index]);
	}
	xil_printf("\r\n");
}
