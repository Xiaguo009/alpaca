/* time_meas.h */

#include "time_meas.h"  // Modify if needed.


void timer_init()
{
    Timer_A_initContinuousModeParam timerAInitParam = {0};

    timerAInitParam.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    timerAInitParam.clockSourceDivider = TA2_CLOCKDIV;
    timerAInitParam.startTimer = false;
    timerAInitParam.timerClear = TIMER_A_DO_CLEAR;
    timerAInitParam.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;

    Timer_A_initContinuousMode(TIMER_A2_BASE, &timerAInitParam);
}

void timer_start_count(uint16_t *timer_counter_start)
{
    HWREG16(TIMER_A2_BASE + OFS_TAxCTL) &= ~MC_3;
    HWREG16(TIMER_A2_BASE + OFS_TAxCTL) |= TIMER_A_CONTINUOUS_MODE;
    *timer_counter_start = HWREG16(TIMER_A2_BASE + OFS_TAxR);
}

void timer_end_count(uint16_t *timer_counter_finish)
{
    *timer_counter_finish = HWREG16(TIMER_A2_BASE + OFS_TAxR);
    HWREG16(TIMER_A2_BASE + OFS_TAxCTL) &= ~MC_3;
    HWREG16(TIMER_A2_BASE + OFS_TAxCTL) |= TACLR;
}


uint32_t timer_get_cycles(uint16_t cstart, uint16_t cfinish)
{
    uint32_t timer_ticks = (uint32_t)cfinish - (uint32_t)cstart;
    uint32_t cycles;
    switch(TA2_CLOCKDIV)
    {
    case TIMER_A_CLOCKSOURCE_DIVIDER_1:
        cycles = timer_ticks;
        break;
    case TIMER_A_CLOCKSOURCE_DIVIDER_10:
        cycles = timer_ticks * 10;
        break;
    case TIMER_A_CLOCKSOURCE_DIVIDER_64:
        cycles = timer_ticks * 64;
        break;
    }
    return cycles;
}
