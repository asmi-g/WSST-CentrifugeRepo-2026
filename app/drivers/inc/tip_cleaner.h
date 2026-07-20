#ifndef TIP_CLEANER_H
#define TIP_CLEANER_H

#include <stdint.h>

/*
 * Mechanism positions and timing.
 * TIP_CLEAN_ANGLE and TIP_CLEAN_DWELL_MS are expected to be tuned during
 * hardware testing.
 */
#define TIP_RETRACTED_ANGLE       70.0f
#define TIP_SOLDER_ANGLE          10.0f
#define TIP_CLEAN_ANGLE           20.0f
#define TIP_CLEAN_RETRACT_ANGLE   30.0f
#define TIP_CLEAN_DWELL_MS       400U
#define TIP_CLEAN_STROKE_COUNT     3U

void TIP_CLEANER_Run(uint16_t servo_instance);

#endif
