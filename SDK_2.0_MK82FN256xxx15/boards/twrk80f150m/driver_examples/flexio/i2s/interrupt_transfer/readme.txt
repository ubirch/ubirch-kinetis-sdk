Overview
========
The flexio_i2s_interrupt example shows how to use flexio_i2s driver with interrupt:

In this example, flexio acts as I2S module to playbacks the audio data stored in flash using interrupt.

Toolchain supported
===================
- IAR embedded Workbench 7.50.1
- Keil MDK 5.17
- GCC ARM Embedded 2015-4.9-q3
- Kinetis Development Studio IDE 3.0.0
- Atollic TrueSTUDIO 5.4.0

Hardware requirements
=====================
- Mini/micro USB cable
- TWR-K80F150M board
- Primary Elevator
- TWR-SGTL5000
- Headphone
- Personal Computer

Board settings
==============
The flexio_i2s_interrupt example is requires connecting the FLEXIO pins with the SAI pins so that
the I2S signals can route to the TWR-SGTL5000 Tower System module. Insert all boards into TWR-Elevator
The connection should be set as following:
- B70-Elevator, A22-Elevator connected
- B69-Elevator, A23-Elevator connected
- A37-Elevator, A25-Elevator connected
- J7-SGTL5000, headphone connected

Prepare the Demo
================
1.  Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the log would be seen on the OpenSDA terminal like as below,
You can hear a piece of sound which is a sin wave
~~~~~~~~~~~~~~~~~~~~~
FLEXIO_I2S interrupt example started!
FLEXIO_I2S interrupt example finished!
~~~~~~~~~~~~~~~~~~~~~

Customization options
=====================


