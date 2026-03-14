# Actuation System Architecture

<div align="center">
    <img src="assets/actuator-subsystem-block-diagram-v1.png" alt="Actuator Subsystem Block Diagram"/>
</div>
<br>

Actuation subsystem from the software side:

* Actuation modules – Responsible for all mechanical motion in the system, including shaft indexing, iron in/out engagement, and solder wire feeding. Actuators are driven using a combination of stepper motors (STEP/DIR) and servo motors (PWM).
* Feedback sensing – Limit switches and optional encoders provide homing, travel bounds, and motion verification for relevant axes.
* Interlocks with heating – Wire feed is explicitly gated by the heating subsystem and is only enabled when the corresponding iron is in a "READY" heating state.
* From the STM32 side – FreeRTOS threads handle high-level actuation sequencing, per-axis state management, low-level motor control, safety monitoring (E-stop and fault handling), and logging/telemetry.
