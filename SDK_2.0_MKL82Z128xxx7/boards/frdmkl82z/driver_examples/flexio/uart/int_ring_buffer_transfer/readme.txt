Overview
========
The flexio_uart_interrupt_ring_buffer example shows how to use flexio uart driver in interrupt way with
RX ring buffer enabled:

In this example, a flexio simulated uart connect to PC through USB-Serial, the board will
send back all characters that PC send to the board.

Note: The example echo every 8 characters, so input 8 characters every time.

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
- FRDM-KL82Z board
- USB-to-COM external
- Personal Computer

Board settings
==============
The example is requires connecting the FLEXIO pins with the USB2COM pins
The connection should be set as following:
- J4-12, TX of USB2COM connected
- J4-10, RX of USB2COM connected
- J3-14, Ground of USB2COM connected

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
When the example runs successfully, the log would be seen on the UART Terminal port which connected to the USB2COM like:
~~~~~~~~~~~~~~~~~~~~~
FLEXIO UART RX ring buffer example
Send back received data
Echo every 8 bytes
~~~~~~~~~~~~~~~~~~~~~

Customization options
=====================


