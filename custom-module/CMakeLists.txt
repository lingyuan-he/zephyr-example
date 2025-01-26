# Copyright (c) 2021 Nordic Semiconductor ASA
# SPDX-License-Identifier: Apache-2.0
#
# This CMake file is picked by the Zephyr build system because it is defined
# as the module CMake entry point (see zephyr/module.yml).

zephyr_include_directories(include)

# This is needed so that custom ledmatrix driver class can define system calls.
# See include/app/drivers/ledmatrix.h.
zephyr_syscall_include_directories(include)

add_subdirectory(drivers)
add_subdirectory(lib)
