# Zephyr Example Application

This repository contains an example Zephyr application that is modified from 
the [Zephyr example application][example]. The main purpose of this repository
is to illustrate a typical structure of a Zephyr-based application that uses
a custom module.

This example application runs on the micro:bit v2 board. When flashed onto the
board without change, it detects changes in the direction of gravity and
illuminate a column/row of LED of the 5x5 LED matrix that corresponds with the
change:
- When the board is placed flat or not directly on one of its sides (looking
from the side of the LED matrix), no LED is illuminated.
- When the board is placed perpendicularly on one of its sides (left, right,
top, or bottom), the column/row closest to the down side of the board is
illuminated. For example, if the board has its top side (looking from the side
of the LED matrix) straight up, the bottow row of LED will illuminate.

This example demonstrates several functionalities of the Zephyr OS:
- A basic custom libarary (accel) that can be enabled/disabled with Kconfig.
The library provides support of using the 3-axis sensor.
- A custom device tree binding and a custom ledmatrix driver that provides
support of lighting up the top row, bottom row, left column, or right column
LEDs, as well as to turn the LED matrix off. In combination they illustrate
the connection from the device tree to the driver API, then to the driver API
implementation.
- Layered Kconfig configurations for the custom lib and driver, including config
to enable/disable them.
- Logging of modules that are individually configurable.

This application is tested with the 4.0.99 development version of Zephyr OS as
of December 2024.

##How to Use this Repository

First, the Zephyr main repository and Zephyr SDK should be set up. The simplest
way to achieve this is to follow the [Zephyr Getting Started Guide][started] and
complete the blinky example on the micro:bit V2 board.

Next, this repository should be cloned into a sub-directory within the
zephyrproject folder (~/zephyrproject if you have followed the guide). In other
words, it is intended to be used as a "workspace" type of application as
mentioned in the [Zephyr Application Developement][development] documentation
example, in ~/zephyrproject/applications.

Then, the default configurations should allow for a seemless compilation:
```shell
cd ~zephyrproject/applications
west build -b bbc_microbit_v2 app -p
```

Flash to the board with command:
```shell
west flash
```
Debugging on the board with command:
```shell
west debug
```
##Helpful Links

- [Zephyr GitHub Repository][github]
- [Getting Started Guide][started]
- [Application Developement][development]
- [Devicetree HOWTOs][howtos]
- [Device Driver Model][model]
- [micro:bit v2 Board][board]
- [The West Tool][west]

- Basic [Zephyr application][app_dev] skeleton
- [Zephyr workspace applications][workspace_app]
- [Zephyr modules][modules]
- [West T2 topology][west_t2]
- [Custom boards][board_porting]
- Custom [devicetree bindings][bindings]
- Out-of-tree [drivers][drivers]
- Out-of-tree libraries
- Example CI configuration (using GitHub Actions)
- Custom [west extension][west_ext]
- Doxygen and Sphinx documentation boilerplate

This repository is versioned together with the [Zephyr main tree][zephyr]. This
means that every time that Zephyr is tagged, this repository is tagged as well
with the same version number, and the [manifest](west.yml) entry for `zephyr`
will point to the corresponding Zephyr tag. For example, the `example-application`
v2.6.0 will point to Zephyr v2.6.0. Note that the `main` branch always
points to the development branch of Zephyr, also `main`.

[github]: https://github.com/zephyrproject-rtos/zephyr
[example]: https://github.com/zephyrproject-rtos/example-application/tree/main
[started]: https://docs.zephyrproject.org/latest/develop/getting_started/index.html
[development]: https://docs.zephyrproject.org/latest/develop/application/index.html
[howtos]: https://docs.zephyrproject.org/latest/build/dts/howtos.html
[model]: https://docs.zephyrproject.org/latest/kernel/drivers/index.html
[board]: https://docs.zephyrproject.org/latest/boards/bbc/microbit_v2/doc/index.html
[west]: https://docs.zephyrproject.org/latest/develop/west/index.html

