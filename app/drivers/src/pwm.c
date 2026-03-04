#include "pwm.h"

static uint32_t clamp_u32(uint32_t x, uint32_t lo, uint32_t hi)
{
  if (x < lo) return lo;
  if (x > hi) return hi;
  return x;
}

HAL_StatusTypeDef pwm_init(pwm_t *p, TIM_HandleTypeDef *htim, uint32_t channel)
{
  if (!p || !htim) return HAL_ERROR;

  p->htim = htim;
  p->channel = channel;
  p->arr = __HAL_TIM_GET_AUTORELOAD(htim);

  return HAL_OK;
}

HAL_StatusTypeDef pwm_start(pwm_t *p)
{
  if (!p || !p->htim) return HAL_ERROR;
  return HAL_TIM_PWM_Start(p->htim, p->channel);
}

HAL_StatusTypeDef pwm_set_u8(pwm_t *p, uint8_t duty_u8)
{
  if (!p || !p->htim) return HAL_ERROR;

  uint32_t arr = p->arr;
  uint32_t ccr = (uint32_t)(((uint64_t)duty_u8 * (uint64_t)(arr + 1)) / 255ULL);
  ccr = clamp_u32(ccr, 0, arr);

  __HAL_TIM_SET_COMPARE(p->htim, p->channel, ccr);
  return HAL_OK;
}