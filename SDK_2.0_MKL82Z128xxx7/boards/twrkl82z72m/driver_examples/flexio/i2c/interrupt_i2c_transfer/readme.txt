Overview
========
The flexio_i2c_interrupt example shows how to use flexio i2c master driver in interrupt way:

In this example, a flexio simulated i2c master connect to an I2C slave.

Toolchain supported
===================
- IAR embedded Workbench 7.60.1
- Keil MDK 5.18a
- GCC ARM Embedded 2015-4.9-q3
- Kinetis Development Studio IDE 3.2.0
- Atollic TrueSTUDIO 5.4.2

Hardware requirements
=====================
- Mini/micro USB cable
- TWR-KL82Z72M board
- Primary Elevator
- Personal Computer

Board settings
==============
The example is requires connecting the FLEXIO pins with the USB2COM pins
Insert TWR-KL82Z72M board into Primary Elevator. The connection should be set as following:
- B69-Elevator, A7-Elevator connected
- B70-Elevator, A8-Elevator connected

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:
~~~~~~~~~~~~~~~~~~~~~
FlexIO I2C interrupt - I2C interrupt
Master will send data :
0x00  0x01  0x02  0x03  0x04  0x05  0x06  0x07
0x08  0x09  0x0A  0x0B  0x0C
Slave received data :
0x00  0x01  0x02  0x03  0x04  0x05  0x06  0x07
0x08  0x09  0x0A  0x0B  0x0C
~~~~~~~~~~~~~~~~~~~~~

Customization options
=====================


