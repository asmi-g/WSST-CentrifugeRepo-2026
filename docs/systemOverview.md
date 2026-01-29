# System Architecture

<div align="center">
    <img src="assets/WSST-2026.png" alt="High Level Software Interfacing"/>
</div>

The software subsystem can be divided into 3 components:
* High Level Functionality - interfacing the microcontroller for the mission specialists' use
* Actuation - controlling movement of components within the soldering mechanism (stepper and servos)
* Heating - controlling the heating of the soldering irons

Tool stack includes C/C++, cmake, OpenOCD, gnu embedded toolchain, and python for the GUI.