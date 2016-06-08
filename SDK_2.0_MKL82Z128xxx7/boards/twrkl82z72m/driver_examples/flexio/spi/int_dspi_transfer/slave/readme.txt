Overview
========
The flexio_spi_slave_interrupt_dspi_master example shows how to use flexio spi slave driver in interrupt way:

In this example, a flexio simulated slave connect to a dspi master.

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
- A38-Elevator, B44-Elevator connected
- A37-Elevator, B45-Elevator connected
- B69-Elevator, B46-Elevator connected
- B70-Elevator, B48-Elevator connected

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
DSPI Master interrupt - FLEXIO SPI Slave interrupt example start.
This example use one dspi instance as master and one flexio spi slave on one board.
Master and slave are both use interrupt way.
Please make sure you make the correct line connection. Basically, the connection is:
DSPI_master -- FLEXIO_SPI_slave
   CLK      --    CLK
   PCS      --    PCS
   SOUT     --    SIN
   SIN      --    SOUT
This is FLEXIO SPI slave call back.
DSPI master <-> FLEXIO SPI slave transfer all data matched!
~~~~~~~~~~~~~~~~~~~~~

Customization options
=====================


