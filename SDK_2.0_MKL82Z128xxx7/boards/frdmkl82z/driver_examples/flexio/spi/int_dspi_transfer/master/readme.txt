Overview
========
The flexio_spi_master_interrupt_dspi_slave example shows how to use flexio spi master driver in interrupt way:

In this example, a flexio simulated master connect to a dspi slave .

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
- Personal Computer

Board settings
==============
The example is requires connecting between FlexIO pins with DSPI pins
The connection should be set as following:
- J2-8, J2-20 connected
- J2-10, J2-18 connected
- J2-6, J4-12 connected
- J2-12, J4-10 connected

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
FLEXIO Master - DSPI Slave interrupt example start.
This example use one flexio spi as master and one dspi instance as slave on one board.
Master and slave are both use interrupt way.
Please make sure you make the correct line connection. Basically, the connection is:
FLEXI_SPI_master -- DSPI_slave
   CLK      --    CLK
   PCS      --    PCS
   SOUT     --    SIN
   SIN      --    SOUT
This is DSPI slave call back.
FLEXIO SPI master <-> DSPI slave transfer all data matched!
~~~~~~~~~~~~~~~~~~~~~

Customization options
=====================


