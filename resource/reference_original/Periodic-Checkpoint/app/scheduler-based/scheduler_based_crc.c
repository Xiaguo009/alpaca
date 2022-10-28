
#include <app/app_api.h>
#include <app/app_global.h>
#include <scheduling/scheduler.h>
#include <scheduling/scheduler_based_task.h>

__SHARED_VAR(
uint16_t SW_Results;
uint16_t cnt;
)


__TASK_ENTRY(init,

__GET(cnt) = 0;
__GET(SW_Results) = CRC_INIT;
return 1;

)


__TASK(1, main,

__GET(SW_Results) = CRCheck_CCITT_Update(__GET(SW_Results), CRC_Input[__GET(cnt)] & 0xFF);
__GET(SW_Results) = CRCheck_CCITT_Update(__GET(SW_Results), (CRC_Input[__GET(cnt)] >> 8) & 0xFF);
++__GET(cnt);

if(__GET(cnt) < CRC_LENGTH)
   return 1;
else {
    return TASK_FINISH;
}

)


void crc_regist()
{
    task_regist(0, init,       false);
    task_regist(1, main,       true );

    WAR_REGIST(2);
}
