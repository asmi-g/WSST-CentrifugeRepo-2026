#ifndef WIRE_FEEDER_H
#define WIRE_FEEDER_H

#include "stepper.h"

#define WIRE_FEED_STEPS          500U
#define WIRE_FEED_SETTLE_MS      1000U
#define WIRE_FEED_PULSE_US       5U
#define WIRE_FEED_STEP_DELAY_US  2000U

void WIRE_FEEDER_Run(stepper_t *feeder_1, stepper_t *feeder_2);

#endif
