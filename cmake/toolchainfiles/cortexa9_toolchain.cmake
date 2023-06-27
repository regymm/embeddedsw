# Copyright (C) 2023 Advanced Micro Devices, Inc.  All rights reserved.
# SPDX-License-Identifier: MIT
set( CMAKE_EXPORT_COMPILE_COMMANDS ON)
set( CMAKE_INSTALL_MESSAGE LAZY)
set( CMAKE_C_COMPILER arm-none-eabi-gcc )
set( CMAKE_CXX_COMPILER arm-none-eabi-g++ )
set( CMAKE_C_COMPILER_LAUNCHER  )
set( CMAKE_CXX_COMPILER_LAUNCHER  )
set( CMAKE_ASM_COMPILER arm-none-eabi-gcc )
set( CMAKE_AR arm-none-eabi-ar CACHE FILEPATH "Archiver" )
set( CMAKE_SIZE arm-none-eabi-size CACHE FILEPATH "Size" )
set( CMAKE_SYSTEM_PROCESSOR "cortexa9" )
set( CMAKE_MACHINE "Zynq" )
set( CMAKE_SYSTEM_NAME "Generic" )
set( CMAKE_SPECS_FILE "$ENV{ESW_REPO}/scripts/specs/arm/Xilinx.spec" CACHE STRING "Specs file path for using CMAKE toolchain files" )
set( TOOLCHAIN_C_FLAGS " -O2 -DSDT" CACHE STRING "CFLAGS" )
set( TOOLCHAIN_CXX_FLAGS " -O2 -DSDT" CACHE STRING "CXXFLAGS" )
set( TOOLCHAIN_ASM_FLAGS " -O2 -DSDT" CACHE STRING "ASM FLAGS" )
set( TOOLCHAIN_DEP_FLAGS " -MMD -MP" CACHE STRING "Flags used by compiler to generate dependency files")
set( TOOLCHAIN_EXTRA_C_FLAGS " -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -g -Wall -Wextra -fno-tree-loop-distribute-patterns" CACHE STRING "Extra CFLAGS")
set( CMAKE_C_FLAGS_RELEASE "-DNDEBUG" CACHE STRING "Additional CFLAGS for release" )
set( CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG" CACHE STRING "Additional CXXFLAGS for release" )
set( CMAKE_ASM_FLAGS_RELEASE "-DNDEBUG" CACHE STRING "Additional ASM FLAGS for release" )
