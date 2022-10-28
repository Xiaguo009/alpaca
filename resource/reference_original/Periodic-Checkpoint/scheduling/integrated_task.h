#ifndef SCHEDULING_INTEGRATED_TASK_H_
#define SCHEDULING_INTEGRATED_TASK_H_

#include <profile.h>
#include <stdint.h>

#define     INTEGRATED_BACKUP_ENABLE   PROFILE_BACKUP_ENABLE
#if   defined(PROFILE_BACKUP_USE_DMA)
#define     INTEGRATED_BACKUP_USE_DMA
#elif defined(PROFILE_BACKUP_USE_CPU)
#define     INTEGRATED_BACKUP_USE_CPU
#endif

void cpu_copy(uint16_t* from, uint16_t* to, uint16_t size);
void dma_prepare(unsigned long from, unsigned long to, uint16_t size);

#if INTEGRATED_BACKUP_ENABLE && (defined(__MSP430FR5969__) || defined(__MSP430FR5994__)) && defined(INTEGRATED_BACKUP_USE_DMA)
#define PREPARE_FOR_BACKUP  dma_prepare((unsigned long)&__persistent_vars, (unsigned long)backup_buf, global_war_size)
#else
#define PREPARE_FOR_BACKUP  ((void)0)
#endif


#if INTEGRATED_BACKUP_ENABLE
#if defined(INTEGRATED_BACKUP_USE_DMA)
#define BUILDIN_UNDO \
        dma_prepare((unsigned long)backup_buf, (unsigned long)&__persistent_vars, global_war_size); \
        HWREG16(DMA_BASE + DMA_CHANNEL_0 + OFS_DMA0CTL) |= DMAREQ
#elif defined(INTEGRATED_BACKUP_USE_CPU)
#define BUILDIN_UNDO        cpu_copy(backup_buf, (uint16_t*)&__persistent_vars, global_war_size)
#endif
#else
#define BUILDIN_UNDO        ((void)0)
#endif


#if INTEGRATED_BACKUP_ENABLE
#if defined(INTEGRATED_BACKUP_USE_DMA)
#define BUILDIN_BACKUP      HWREG16(DMA_BASE + DMA_CHANNEL_0 + OFS_DMA0CTL) |= DMAREQ
#elif defined(INTEGRATED_BACKUP_USE_CPU)
#define BUILDIN_BACKUP      cpu_copy((uint16_t*)&__persistent_vars, backup_buf, global_war_size)
#endif
#else
#define BUILDIN_BACKUP      ((void)0)
#endif

#define __GET_CURTASK           (status & 0x0FFF)
#define __IS_TASK_RUNNING       ((status & 0xF000) != 0)

#define __NEXT(id, name)        status = id; goto name
#define __FINISH                first_run = 1; return

#define __SHARED_VAR(...) \
        typedef struct { __VA_ARGS__ } FRAM_data_t  __attribute__ ((aligned (2))); \
        static __nv FRAM_data_t __persistent_vars; \

#define __GET(item) __persistent_vars.item

#if INTEGRATED_BACKUP_ENABLE
#define __BUILDIN_TASK_BOUNDARY(id, name) \
    name: \
    if (backup_needed[__GET_CURTASK] == true && !__IS_TASK_RUNNING) { \
        BUILDIN_BACKUP; \
    } \
    status |= 0xF000
#elif DO_ANALYSIS
#define __BUILDIN_TASK_BOUNDARY(id, name) \
    name: \
    if (backup_needed[__GET_CURTASK] == true) analysis_runtask(__GET_CURTASK, global_war_size); \
    else analysis_runtask(__GET_CURTASK, 0); \
    status |= 0xF000
#else
#define __BUILDIN_TASK_BOUNDARY(id, name) \
    name: \
    status |= 0xF000
#endif


#endif /* SCHEDULING_INTEGRATED_TASK_H_ */
