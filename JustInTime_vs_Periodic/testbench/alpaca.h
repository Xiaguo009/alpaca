#ifndef ALPACA_H
#define ALPACA_H

#include <driver/nv.h>
#include <stddef.h>
#include <stdint.h>


//1.for vbm
//
//cur_version ,for vbm. ++ when reboot and transition_to
extern volatile unsigned _numBoots;
//test and set for wt and rd of war array
#define vbm_test(v) v == _numBoots //cur_version
#define vbm_set(v) v = _numBoots


//2.pre_commit
extern unsigned long data_src_base[100];
extern unsigned long data_dest_base[100];
extern unsigned long data_size_base[100];

extern volatile unsigned num_dirty_gv;
void write_to_gbuf(unsigned long data_src, unsigned long data_dest, size_t var_size);

//3.commit --task_prologue
extern volatile uint8_t needCommit;
extern volatile unsigned gv_index;
void commit();


//4. for task
//
#define __COMMIT commit() //call

//for count
#define __PRE_COMMIT(data_src, data_dest, var_size)                                 \
    write_to_gbuf((unsigned long)(data_src), (unsigned long)(data_dest), var_size); \
    bench_commit += var_size;                                                                 \
    task_commit[status] += var_size//;\
    //printf("__PRE_COMMIT of task_i:%d,.bench_commit:%d,task_commit:%d.\n",status,bench_commit,task_commit[status])

//TODO: add [task_array_commit + bench_array_commit] to all bench + VBM_SET
//to count the total pre_commit size of array in a single task
#define VBM_SET(x)                          \
    task_array_commit[status] += sizeof(x); \
    bench_array_commit += sizeof(x)

#define __TASK(id, name) \
    name:                \
    ++_numBoots;         \
    __COMMIT;            \
    ++task_count[id];    \
    ++bench_task_count//;\
    //printf("__TASK. status: %d. task count: %d. bench_task_count:%d. \n",status,task_count[id],bench_task_count)
// for testing
// printf("id: %d. task count: %d. \n",id,++task_count)
// status: to be consistent with latics
//    status |= 0xF000;    \


#define __TRANSITION_TO(id, name) \
    needCommit = 1;                             \
    status = id;                  \
    goto name

//TODO: to count the scalar_commit size by : [task/bench]_commit - array_commit
#define __TASK_DOWN                           \
    printf("bench_task_count:%d,bench_commit_size(bytes):%d.\n",bench_task_count, bench_commit);   \
    for (uint16_t i = 0; i < TASK_NUM; i++) \
    {                                         \
        printf("task_id:%d,task_count:%d,task_commit_size(bytes):%d.\n", i, task_count[i], task_commit[i]); \
    }                                         \
    return

#define __GET_CURTASK  (status & 0x0FFF) 


// SHARED->
#define __GLOBAL_SCALAR(type, name)  static __nv type name
#define __GLOBAL_ARRAY(type, name, size)  static __nv type name[size]


#define __GET(item) item


#endif // ALPACA_H
