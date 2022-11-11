
#ifndef MAIN_HPP_
#define MAIN_HPP_

#include <driver/backup_and_recovery.h>
#include <driver/driverlib_include.h>
#include <driver/led_button.h>
#include <driver/mem_write.h>
#include <driver/nv.h>
#include <driver/target.h>
#include <driver/uart2target.h>
#include <driver/volt_monit.h>
#include <driver/time_meas.h>
#include <testbench/testbench_api.h>
#include <global_config.h>
#include <stdint.h>


extern __nv uint8_t backup_error_flag;
extern __nv uint8_t recovery_needed;
extern __nv uint8_t system_in_lpm;

#define JIT_SYSTEM_INTO_SLEEP  system_in_lpm = 1; __bis_SR_register(GIE | LPM1_bits)

enum {
    TESTBENCH_READY = 0x02,
    TESTBENCH_RUNNING = 0x01,
    TESTBENCH_FINISH = 0x00
};


#define TESTBENCH_LIST_SIZE     9//8

const uint8_t status_code[TESTBENCH_LIST_SIZE][2] = {
    {0, 15}, {51, 60}, {85, 90}, {102, 105},
    {150, 153}, {165, 170}, {195, 204}, {240, 255}
};

const FUNCPTR JIT_func[TESTBENCH_LIST_SIZE] = {
    jit_ar_main, jit_bc_main, jit_cem_main, jit_crc_main,
    jit_cuckoo_main, jit_dijkstra_main, jit_rsa_main, jit_sort_main
};

const FUNCPTR PC_func[TESTBENCH_LIST_SIZE] = {
    pc_ar_main, pc_bc_main, pc_blowfish_main,pc_cem_main, pc_crc_main,
    pc_cuckoo_main, pc_dijkstra_main, pc_rsa_main, pc_sort_main
}; //pc_blowfish_main

const FUNCPTR ALPACA_func[TESTBENCH_LIST_SIZE] = {
    alpaca_ar_main, alpaca_bc_main, alpaca_blowfish_main ,alpaca_cem_main, alpaca_crc_main,
    alpaca_cuckoo_main, alpaca_dijkstra_main, alpaca_rsa_main, alpaca_sort_main
};



inline void alpaca_run_testbench(uint16_t tbid, uint16_t* state) {
    switch(*state) {
    case TESTBENCH_READY:
        *state = TESTBENCH_RUNNING;
#if SEND_SINGNAL_AT_START_AND_END
        EUSCI_A_UART_transmitData(UART_BASEADDR, status_code[tbid][0]);
#endif
        /* NO BREAK */
    case TESTBENCH_RUNNING:
        (* (ALPACA_func[tbid]))(); //call bench_main()
        *state = TESTBENCH_FINISH;
#if SEND_SINGNAL_AT_START_AND_END
        EUSCI_A_UART_transmitData(UART_BASEADDR, status_code[tbid][1]);
#endif
        /* NO BREAK */
    case TESTBENCH_FINISH:
        return;
    }
}

inline void periodic_run_testbench(uint16_t tbid, uint16_t* state) {
    switch(*state) {
    case TESTBENCH_READY:
        *state = TESTBENCH_RUNNING;
#if SEND_SINGNAL_AT_START_AND_END
        EUSCI_A_UART_transmitData(UART_BASEADDR, status_code[tbid][0]);
#endif
        /* NO BREAK */
    case TESTBENCH_RUNNING:
        (* (PC_func[tbid]))();
        *state = TESTBENCH_FINISH;
#if SEND_SINGNAL_AT_START_AND_END
        EUSCI_A_UART_transmitData(UART_BASEADDR, status_code[tbid][1]);
#endif
        /* NO BREAK */
    case TESTBENCH_FINISH:
        return;
    }
}


#endif /* MAIN_HPP_ */
