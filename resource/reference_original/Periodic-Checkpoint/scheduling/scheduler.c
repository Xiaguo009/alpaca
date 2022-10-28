
#include <board/driverlib_include.h>
#include <scheduling/scheduler.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define GET_TASK(id)            scheduler_task_queue[id]
#define GET_TASK_BCTYPE(id)     scheduler_task_bakcup[id]
#define GET_TASK_PTR(id)        scheduler_task_funcque[id]

__nv uint16_t scheduler_backbuf[BACKUP_BUF_SIZE];
__nv void* scheduler_backup_src = 0;
__nv uint16_t scheduler_backup_size = 0;

__nv TASKFUNC scheduler_task_funcque[TASK_QUE_SIZE];
__nv bool scheduler_task_bakcup[TASK_QUE_SIZE];
__nv TASKID cur_task_id = 0;
__nv TASKID next_task_id = 0;
__nv bool task_running = 0;

__nv bool is_first_run = true;

static void undo()
{
#if (defined(__MSP430FR5969__) || defined(__MSP430FR5994__)) && defined(BACKUP_USE_DMA)
    HWREG16(DMA_BASE + DMA_CHANNEL_0 + OFS_DMA0CTL) &= ~DMAEN;
    __data16_write_addr((unsigned short)(DMA_BASE + DMA_CHANNEL_0 + OFS_DMA0SA), (unsigned long)scheduler_backbuf);
    __data16_write_addr((unsigned short)(DMA_BASE + DMA_CHANNEL_0 + OFS_DMA0DA), (unsigned long)scheduler_backup_src);
    HWREG16(DMA_BASE + DMA_CHANNEL_0 + OFS_DMA0SZ) = scheduler_backup_size;
    HWREG16(DMA_BASE + DMA_CHANNEL_0 + OFS_DMA0CTL) |= DMAEN;
    HWREG16(DMA_BASE + DMA_CHANNEL_0 + OFS_DMA0CTL) |= DMAREQ;
#elif defined(__MSP430FR2433__) && defined(BACKUP_USE_DMA)
#error  "MSP430FR2433 DOES NOT HAVE A DMA."
#elif defined(BACKUP_USE_CPU)
    uint16_t *dataPtr = scheduler_backbuf;
    uint16_t *framPtr = scheduler_backup_src;
    uint16_t numberOfWords = scheduler_backup_size;
    while (numberOfWords > 0) {
        *framPtr++ = *dataPtr++;
        numberOfWords--;
    }
#else
#error "ERROR: UNRECOGNIZED UNDO SCHEME!"
#endif
}

bool scheduler_main()
{
    if (is_first_run == false) {
        if (cur_task_id != next_task_id) {
            cur_task_id = next_task_id;
        }
        else if ( task_running == true && GET_TASK_BCTYPE(cur_task_id) == true ) {
            undo();
        }
    } else {
        cur_task_id = (* GET_TASK_PTR(0) )();
        next_task_id = cur_task_id;
        is_first_run = false;
    }

#if (defined(__MSP430FR5969__) || defined(__MSP430FR5994__)) && defined(BACKUP_USE_DMA)
    HWREG16(DMA_BASE + DMA_CHANNEL_0 + OFS_DMA0CTL) &= ~DMAEN;
    __data16_write_addr((unsigned short)(DMA_BASE + DMA_CHANNEL_0 + OFS_DMA0SA), (unsigned long)scheduler_backup_src);
    __data16_write_addr((unsigned short)(DMA_BASE + DMA_CHANNEL_0 + OFS_DMA0DA), (unsigned long)scheduler_backbuf);
    HWREG16(DMA_BASE + DMA_CHANNEL_0 + OFS_DMA0SZ) = scheduler_backup_size;
    HWREG16(DMA_BASE + DMA_CHANNEL_0 + OFS_DMA0CTL) |= DMAEN;
#endif

    // Scheduler
    while (cur_task_id != TASK_FINISH) {

#if BACKUP_ENABLE
        if (task_running == false && GET_TASK_BCTYPE(cur_task_id) == true) {

#if (defined(__MSP430FR5969__) || defined(__MSP430FR5994__)) && defined(BACKUP_USE_DMA)
            HWREG16(DMA_BASE + DMA_CHANNEL_0 + OFS_DMA0CTL) |= DMAREQ;
#elif defined(__MSP430FR2433__) && defined(BACKUP_USE_DMA)
#error  "MSP430FR2433 DOES NOT HAVE A DMA."
#elif defined(BACKUP_USE_CPU)
            uint16_t *dataPtr = scheduler_backup_src;
            uint16_t *framPtr = scheduler_backbuf;
            uint16_t numberOfWords = scheduler_backup_size;
            while (numberOfWords > 0)
            {
                *framPtr++ = *dataPtr++;
                numberOfWords--;
            }
#else
#error "ERROR: UNRECOGNIZED BACKUP SCHEME!"
#endif
        }
#endif

        task_running = true;
        next_task_id = (* GET_TASK_PTR(cur_task_id) )();
        task_running = false;
        cur_task_id = next_task_id;
    }

    is_first_run = true;
    return true;
}


void task_regist(TASKID id, TASKFUNC name, bool backup)
{
    GET_TASK_PTR(id) = name;
    GET_TASK_BCTYPE(id) = backup;
}

void war_regist(void* src, uint16_t size)
{
    scheduler_backup_src = src;
    scheduler_backup_size = size;
}
