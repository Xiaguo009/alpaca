
#include <main.hpp>
#include <stdio.h>
#include <msp430.h>

__nv uint8_t task_success_flag = false;
__nv uint8_t running_flag = 0;

__nv uint16_t current_testbench = 0;
__nv uint16_t state = TESTBENCH_READY;

extern __nv uint8_t backup_error_flag;
extern __nv uint8_t recovery_needed;
extern __nv uint8_t system_in_lpm;

//__nv uint8_t task_count;

void alpaca();
void Periodic_only();
//void scope();

int main()
{
    ref_volt_init();
    adc_timer_init(VOLT_MONITOR_INTERVAL);
    adc_init();
    power_on_init();
    clock_sys_init();
    dma_init();
    uart2target_init();
    timer_init();

    if (running_flag == 0) {
        turn_on_green_led;
        left_button_init;
        while (!check_button_press) {/* empty */}
        left_button_disable;
        turn_off_green_led;
        running_flag = 1;
    }

    if (backup_error_flag == true) {
        turn_on_red_led;
        LPM1;
    }

    if (task_success_flag == true) { /* TODO: do something after success. */ }

    Alpaca_only();
    //Periodic_only();
}


void Alpaca_only() {
    do {
        while (state != TESTBENCH_FINISH) {
            printf("current_testbench:%d,",current_testbench);
            alpaca_run_testbench(current_testbench, &state);
        }
        current_testbench++;
        if (current_testbench >= TESTBENCH_LIST_SIZE) {current_testbench = 0;return;}
        state = TESTBENCH_READY;
    } while (1);

    // task_success_flag = 1;
}

void Periodic_only() {
    do {
        while (state != TESTBENCH_FINISH) {
            printf("current_testbench:%d,",current_testbench);
            periodic_run_testbench(current_testbench, &state);
        }
        current_testbench++;
        if (current_testbench >= TESTBENCH_LIST_SIZE) {current_testbench = 0;return;} //end condition
        state = TESTBENCH_READY;
    } while (1);

    // task_success_flag = 1;
}
