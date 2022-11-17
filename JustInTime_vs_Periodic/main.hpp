
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

enum {
    TESTBENCH_READY = 0x02,
    TESTBENCH_RUNNING = 0x01,
    TESTBENCH_FINISH = 0x00
};



#define TESTBENCH_LIST_SIZE     6//change it if need.

const uint8_t status_code[TESTBENCH_LIST_SIZE][2] = {
    {0, 15}, {51, 60}, {85, 90}, {102, 105},
    {150, 153}, {165, 170}, {195, 204}, {240, 255}
};


const FUNCPTR PC_func[TESTBENCH_LIST_SIZE] = {
    pc_ar_main, pc_bc_main, pc_crc_main,
    pc_cuckoo_main, pc_dijkstra_main, pc_sort_main
}; //, pc_sha256_main, pc_aes_main

const FUNCPTR ALPACA_func[TESTBENCH_LIST_SIZE] = {
    alpaca_ar_main, alpaca_bc_main , alpaca_crc_main,
    alpaca_cuckoo_main, alpaca_dijkstra_main, alpaca_sort_main
}; //, alpaca_sha256_main, alpaca_aes_main



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
