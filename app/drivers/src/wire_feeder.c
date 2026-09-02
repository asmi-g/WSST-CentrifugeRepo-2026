#include "wire_feeder.h"

void WIRE_FEEDER_Run(stepper_t *feeder_1, stepper_t *feeder_2)
{
    STEPPER_StepTwo(
        feeder_1,
        feeder_2,
        WIRE_FEED_STEPS,
        WIRE_FEED_PULSE_US,
        WIRE_FEED_STEP_DELAY_US
    );
}
