/* time_meas.h */

#ifndef IMC_ANALYSIS_TIME_MEAS_H_
#define IMC_ANALYSIS_TIME_MEAS_H_

#include <stdint.h>
#include <lib/driverlib_MSP430FR5xx_6xx/timer_a.h>
#include <stdbool.h>


#define TA2_CLOCKDIV        TIMER_A_CLOCKSOURCE_DIVIDER_64
//!                         TIMER_A_CLOCKSOURCE_DIVIDER_1
//!                         TIMER_A_CLOCKSOURCE_DIVIDER_10
//!                         TIMER_A_CLOCKSOURCE_DIVIDER_64

void timer_init();

void timer_start_count(uint16_t *timer_counter_start);
void timer_end_count(uint16_t *timer_counter_finish);
uint32_t timer_get_cycles(uint16_t cstart, uint16_t cfinish);

#endif /* IMC_ANALYSIS_TIME_MEAS_H_ */
