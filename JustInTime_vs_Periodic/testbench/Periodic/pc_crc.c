
#include <testbench/global_declaration.h>
#include <testbench/Periodic_scheduler.h>
#include <testbench/testbench_api.h>

__SHARED_VAR(
uint16_t SW_Results;
uint16_t cnt;
)

static __nv bool      first_run = 1;
static __nv uint16_t  status = 0;
static const uint16_t global_war_size = 2;
static __nv uint16_t  backup_buf[2] = {};

static const bool backup_needed[] = { false, true };

//for test
static __nv uint16_t task_id = 0;  //task_id
//count for current bench
static __nv uint16_t bench_task_count = 0; //total execution times for all tasks in a bench
static __nv uint32_t bench_commit = 0; //total pre_commit size in a bench
//count for task[i]
static const uint8_t TASK_NUM = CRC_TASK_NUM;
static __nv uint16_t task_count[TASK_NUM] = {0}; // total execution times for task[i]
static __nv uint32_t task_commit[TASK_NUM] = {0}; // total pre_commit size for all execution times of task[i]


void pc_crc_main()
{
    // Buildin scheduler

    if (first_run == 1) { status = 0; first_run = 0;}
    else {
        if (__IS_TASK_RUNNING && backup_needed[__GET_CURTASK] == true) {
            BUILDIN_UNDO;
        }
    }

    PREPARE_FOR_BACKUP;

    switch(__GET_CURTASK) {
    case 0: goto init;
    case 1: goto main;
    }

    // Tasks
    // =================================================================
    __BUILDIN_TASK_BOUNDARY(0, init);
    __GET(cnt) = 0;
    __GET(SW_Results) = CRC_INIT;
    __NEXT(1, main);

    // =================================================================
    __BUILDIN_TASK_BOUNDARY(1, main);

    __GET(SW_Results) = CRCheck_CCITT_Update(__GET(SW_Results), CRC_Input[__GET(cnt)] & 0xFF);
    __GET(SW_Results) = CRCheck_CCITT_Update(__GET(SW_Results), (CRC_Input[__GET(cnt)] >> 8) & 0xFF);
    ++__GET(cnt);

    if(__GET(cnt) < CRC_LENGTH) {
        __NEXT(1, main);
    }
    else {
        __FINISH;
    }

}
