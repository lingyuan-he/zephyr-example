#-------------------------------------------------------------------------------
# Zephyr Example Application
#
# Copyright (c) 2021 Nordic Semiconductor ASA
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)

# Define the custom module for library/Kconfig/Device tree.
# Must be done before finding the Zephyr package.
list(APPEND EXTRA_ZEPHYR_MODULES
  ${CMAKE_CURRENT_SOURCE_DIR}/../custom-module
)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(app LANGUAGES C)

target_sources(app PRIVATE src/main.c)
