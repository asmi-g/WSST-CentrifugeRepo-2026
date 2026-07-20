#include "tip_cleaner.h"

#include "cmsis_os.h"
#include "servo.h"

void TIP_CLEANER_Run(uint16_t servo_instance)
{
    for (uint8_t stroke = 0; stroke < TIP_CLEAN_STROKE_COUNT; stroke++)
    {
        SERVO_MoveTo(servo_instance, TIP_CLEAN_ANGLE);
        osDelay(TIP_CLEAN_DWELL_MS);

        if (stroke + 1U < TIP_CLEAN_STROKE_COUNT)
        {
            SERVO_MoveTo(servo_instance, TIP_CLEAN_RETRACT_ANGLE);
        }
        else
        {
            SERVO_MoveTo(servo_instance, TIP_RETRACTED_ANGLE);
        }

        osDelay(TIP_CLEAN_DWELL_MS);
    }
}
