
#ifndef SCHEDULING_SCHEDULER_BASED_TASK_H_
#define SCHEDULING_SCHEDULER_BASED_TASK_H_

#include <stdint.h>

typedef uint8_t TASKID;
typedef TASKID (*TASKFUNC)();

#define __SHARED_VAR(...) \
        typedef struct { __VA_ARGS__ } FRAM_data_t  __attribute__ ((aligned (2))); \
        static __nv FRAM_data_t __persistent_vars; \

#define __GET(item) __persistent_vars.item

#define __TASK_ENTRY(name, ...) \
    static TASKID name() { \
        __VA_ARGS__ \
    }

#define __TASK(id, name, ...) \
    static TASKID name() { \
        __VA_ARGS__ \
    }

#define TASK_FINISH  (TASKID)0


#endif /* SCHEDULING_SCHEDULER_BASED_TASK_H_ */
