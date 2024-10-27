/******************************************************************************
* Copyright (C) 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/
/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include <unistd.h>
#include "platform.h"
#include "xil_printf.h"

int main()
{
    init_platform();

    print("Hello World\n\r");
    print("Successfully ran Hello World application");

#ifdef AXI_TEST
	volatile int* gpioaddr = (volatile int*)0x41200000;
    *gpioaddr = 0xffffffff;
    print("AXI GPIO set.\n\r");
    while (1) {
    	usleep(1000000);
    	*gpioaddr = 0x5;
    	usleep(1000000);
    	*gpioaddr = 0xa;
    	printf("Hello\n");
    }
#endif

    cleanup_platform();
    return 0;
}
