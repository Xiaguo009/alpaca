
#include <app/app_api.h>
#include <app/app_global.h>
#include <scheduling/scheduler.h>
#include <scheduling/scheduler_based_task.h>

__SHARED_VAR(
uint16_t sorted[SORT_LENGTH];
uint16_t inner_index;
uint16_t outer_index;
)

__TASK_ENTRY(init,

__GET(outer_index) = 0;
__GET(inner_index) = 1;
for (uint16_t temp = 0; temp < SORT_LENGTH; ++temp)
    __GET(sorted[temp]) = raw[temp];
return 1;

)

__TASK(1, inner_loop,

uint16_t temp;
uint16_t val_outer = __GET(sorted[__GET(outer_index)]);
uint16_t val_inner = __GET(sorted[__GET(inner_index)]);
if (val_outer > val_inner)
{
   temp = val_outer;
   val_outer = val_inner;
   val_inner = temp;
}
__GET(sorted[__GET(outer_index)]) = val_outer;
__GET(sorted[__GET(inner_index)]) = val_inner;

++__GET(inner_index);
if (__GET(inner_index) < SORT_LENGTH)
    return 1;
else
    return 2;

)

__TASK(2, outer_loop,

++__GET(outer_index);
__GET(inner_index) = __GET(outer_index) + 1;
if (__GET(outer_index) < SORT_LENGTH - 1)
    return 1;
else {
    return TASK_FINISH;
}

)


void sort_regist()
{
    task_regist(0, init,       false);
    task_regist(1, inner_loop, true );
    task_regist(2, outer_loop, true );

    WAR_REGIST(SORT_LENGTH + 2);
}
