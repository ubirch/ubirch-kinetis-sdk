Overview
========
The TPM project is a simple demonstration program of the SDK TPM driver. It sets up the TPM
hardware block to output a center-aligned PWM signal. The PWM dutycycle is periodically updated.
On boards that have an LED connected to the TPM pins, the user will see a change in LED brightness.

Toolchain supported
===================
- IAR embedded Workbench 7.70.1
- Keil MDK 5.20
- GCC ARM Embedded 2015-4.9-q3
- Kinetis Development Studio IDE 3.2.0
- Atollic TrueSTUDIO 5.5.2

Hardware requirements
=====================
- Mini/micro USB cable
- TWR-K80F150M board
- Personal Computer
- Elevator Tower
- Oscilloscope

Board settings
==============
No special settings are required.

Prepare the Demo
================
1. Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, you can see information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TPM example to output center-aligned PWM signal

You will see a change in LED brightness if an LED is connected to the TPM pin
If no LED is connected to the TPM pin, then probe the signal using an oscilloscope
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
At this example, you need use oscillooscope to measure and observe the format of output.
Output is PTB18 of Primary Elevator board.
Customization options
=====================

