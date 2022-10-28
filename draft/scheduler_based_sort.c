
// #include <app/app_api.h>
// #include <app/app_global.h>
// #include <scheduling/scheduler.h>
// #include <scheduling/scheduler_based_task.h>
#include "alpaca.h"

__SHARED_VAR(
uint16_t sorted[SORT_LENGTH];
uint16_t inner_index;
uint16_t outer_index;
)

//3 vars
//
static __nv uint16_t  status = 0;  //cur_task->id


//1 for vbm
//
// scalar: declaration of the buffer 
__nv uint16_t inner_index_priv;
__nv uint16_t outer_index_priv;
// vector: declaration of the buffer and vbm
__nv uint16_t sorted_priv[SORT_LENGTH];
__nv uint16_t sorted_vbm[SORT_LENGTH];
//
//1 for vbm


//2 define in alpaca.h
//
// #define vbm_test(v) v == _numBoots //cur_version
// #define vbm_set(v) v = _numBoots
#define __COMMIT commit() //inline or call? like cpu_wirte()

#define __TASK(id, name) \
    name:                \
    ++_numBoots;         \
    __COMMIT;   //inline?             
    //status |= 0xF000  //?

#define __TRANSITION_TO(id, name) \
    needCommit = 1;                             \
    status = id;                  \
    goto name

#define __TASK_DOWN return

#define __GET_CURTASK  (status & 0x0FFF) 
// 
//2 define in alpaca.h

void pc_sort_main()
{

    switch(__GET_CURTASK) {
    case 0: goto init;
    case 1: goto inner_loop;
    case 2: goto outer_loop;
    }


    //__TASK_ENTRY(init,
    __TASK(0, init);

    __GET(outer_index) = 0;
    __GET(inner_index) = 1;
    for (uint16_t temp = 0; temp < SORT_LENGTH; ++temp)
        __GET(sorted[temp]) = raw[temp];
    //return 1;

    __TRANSITION_TO(1, inner_loop);

    //)


    //__TASK(1, inner_loop,
    __TASK(1, inner_loop);

    // scalar: initialize the buffer
    //outer_index_priv = __GET(outer_index);
    inner_index_priv = __GET(inner_index);

    uint16_t temp;

    // scalar: redirect the data access to the buffer
    // vector: initialize if read before the first write
    // 
    //uint16_t val_outer = __GET(sorted[__GET(outer_index)]);
    //uint16_t val_inner = __GET(sorted[__GET(inner_index)]);
    if (!vbm_test(sorted_vbm[__GET(outer_index)])) {
        sorted_priv[__GET(outer_index)] = __GET(sorted[__GET(outer_index)]);
    }
    if (!vbm_test(sorted_vbm[inner_index_priv])) {
        sorted_priv[inner_index_priv] = __GET(sorted[inner_index_priv]);
    }
    //vector: redirect
    uint16_t val_outer = sorted_priv[__GET(outer_index)];
    uint16_t val_inner = sorted_priv[inner_index_priv];

    if (val_outer > val_inner)
    {
        temp = val_outer;
        val_outer = val_inner;
        val_inner = temp;
    }

    // scalar and vector: redirect
    // 
    //__GET(sorted[__GET(outer_index)]) = val_outer;
    //__GET(sorted[__GET(inner_index)]) = val_inner;
    //wt
    sorted_priv[__GET(outer_index)] = val_outer;
    // vector: pre_commit after the first write
    if (!vbm_test(sorted_vbm[__GET(outer_index)]))) {
        vbm_set(sorted_vbm[__GET(outer_index)]);
        write_to_gbuf(&sorted_priv[__GET(outer_index)], &sorted[__GET(outer_index)], sizeof(sorted[__GET(outer_index)]));
    }

    sorted_priv[inner_index_priv] = val_inner;
    if (!vbm_test(sorted_vbm[inner_index_priv])) {
        vbm_set(sorted_vbm[inner_index_priv]);
        write_to_gbuf(&sorted_priv[inner_index_priv], &sorted[inner_index_priv], sizeof(sorted[inner_index_priv]));
    }



    // scalar: redirect
    // 
    //++__GET(inner_index);
    //if (__GET(inner_index) < SORT_LENGTH)
    ++inner_index_priv;
    if (inner_index_priv < SORT_LENGTH) {

        // scalar: pre_commit before transition_to
        //pre_commit(&src, &dest, sozeof(&dest))
        write_to_gbuf(&inner_index_priv, &inner_index, sizeof(inner_index));
        //write_to_gbuf(&outer_index_priv, &outer_index, sizeof(outer_index));
        //return 1;
        __TRANSITION_TO(1, inner_loop);
    }
    else {
        // scalar: pre_commit
        write_to_gbuf(&inner_index_priv, &inner_index, sizeof(inner_index));
        //write_to_gbuf(&outer_index_priv, &outer_index, sizeof(outer_index));
        //return 2;
        __TRANSITION_TO(2, outer_loop);
    }

    //)

    __TASK(2, outer_loop);
    
    outer_index_priv = __GET(outer_index);
    //
    ++outer_index_priv;
    __GET(inner_index) = outer_index_priv + 1;
    if (outer_index_priv < SORT_LENGTH - 1)
        //return 1;
        write_to_gbuf(&outer_index_priv, &outer_index, sizeof(outer_index));
        __TRANSITION_TO(1, inner_loop);
    else {
        //return TASK_FINISH;
        //__FINISH;
        write_to_gbuf(&outer_index_priv, &outer_index, sizeof(outer_index));
        __TASK_DOWN;
    }

    //)

} //for main

