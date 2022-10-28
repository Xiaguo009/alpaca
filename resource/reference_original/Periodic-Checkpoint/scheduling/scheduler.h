
#ifndef SCHEDULING_SCHEDULER_H_
#define SCHEDULING_SCHEDULER_H_

#include <board/nv.h>
#include <scheduling/scheduler_based_task.h>
#include <profile.h>
#include <stdbool.h>

#define TASK_QUE_SIZE       12
#define BACKUP_BUF_SIZE     250

#define BACKUP_ENABLE       PROFILE_BACKUP_ENABLE

#if   defined(PROFILE_BACKUP_USE_DMA)
#define BACKUP_USE_DMA
#elif defined(PROFILE_BACKUP_USE_CPU)
#define BACKUP_USE_CPU
#endif

void task_regist(TASKID id, TASKFUNC name, bool backup);
void war_regist(void* src, uint16_t size);

#define WAR_REGIST(size) war_regist((void*)&__persistent_vars, size)

bool scheduler_main();

#endif /* SCHEDULING_SCHEDULER_H_ */
