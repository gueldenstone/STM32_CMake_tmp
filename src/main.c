/**
 * @file: main.c
 *
 * @date: 2020-03-30
 *
 * @author: Lukas Güldenstein
 *
 * @brief: simple blinky project
 *
 * @public functions: none
 *
 * @license: © Lukas Güldenstein
 */

/* -------------------------------- includes -------------------------------- */
#include "main.h"

#include "blink.h"

/* --------------------------- private variables --------------------------- */

/* --------------------------- imported variables --------------------------- */

/* --------------------------- exported variables --------------------------- */

/* -------------------------------- functions ------------------------------- */
int main(void) {
  blinkInit();
  while (1) {
    blink();
  }
}

/* EOF */