/**
 * @file: blink.c
 *
 * @date: 2020-03-31
 *
 * @author: Lukas Güldenstein
 *
 * @brief: simple blinky function
 *
 * @public functions: void blink(void)
 *
 * @license: © Lukas Güldenstein
 */

/* -------------------------------- includes -------------------------------- */
#include "blink.h"

#include "stm32f3xx.h"

/* --------------------------- private variables --------------------------- */

/* --------------------------- imported variables --------------------------- */

/* --------------------------- exported variables --------------------------- */

/* -------------------------------- functions ------------------------------- */

void blinkInit(void) {
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
  GPIOA->MODER |= GPIO_MODER_MODER5_0;
}
void blink(void) {
  volatile unsigned int i = 65536;
  while (i--)
    ;

  GPIOA->ODR ^= GPIO_ODR_5;
}

/* EOF */