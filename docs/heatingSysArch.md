# Heating System Architecture

<div align="center">
    <img src="assets/heating-subsystem-block-diagram-v1.png" alt="Heating Subsystem Block Diagram"/>
</div>
<br>

Heating subsystem from the software side:
* Heating Modules (x2): Pinecil v2 + Breakout Board (for development only) - Two Pinecil irons will be connected to breakout boards so the STM32 can communicate with the Pinecil’s microcontrollers over UART.
* IronOS - Running on the Pinecils is IronOS, an open-source firmware for smart soldering irons. What we'll be using to help us set iron temperature and monitor the iron’s state (has couple of its own FreeRTOS thread implementations already, e.g. PIDTask, POWTask).
* External thermocouples (x2) - Intend on using external thermocouples interfaced with the STM32 to capture continuous, external temperature readings at the iron’s tips.
* From the STM32 side - Threads for communicating with actuator subsystem, GUI/logging, external temp measurements, and polling/commands to Pinecils