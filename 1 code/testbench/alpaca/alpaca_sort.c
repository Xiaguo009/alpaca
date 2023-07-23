
#include <testbench/alpaca.h>
#include <testbench/global_declaration.h>
#include <testbench/testbench_api.h>

//shared vars
__GLOBAL_SCALAR(uint16_t, inner_index);
__GLOBAL_SCALAR(uint16_t, outer_index);
__GLOBAL_ARRAY(uint16_t, sorted, SORT_LENGTH);

//3 vars
//
static __nv uint16_t  status = 0;  //cur_task->id


//1 for vbm
//
// scalar: declaration of the buffer 
static __nv uint16_t inner_index_priv;
static __nv uint16_t outer_index_priv;
//
// vector: declaration of the buffer and vbm
static __nv uint16_t sorted_priv[SORT_LENGTH];
static __nv uint16_t sorted_vbm[SORT_LENGTH];
//
//1 for vbm


void alpaca_sort_main()
{

    switch(__GET_CURTASK) {
    case 0: goto init;
    case 1: goto inner_loop;
    case 2: goto outer_loop;
    }

    __TASK(0, init);

    __GET(outer_index) = 0;
    __GET(inner_index) = 1;
    for (uint16_t temp = 0; temp < SORT_LENGTH; ++temp)
        __GET(sorted[temp]) = raw[temp];

    __TRANSITION_TO(1, inner_loop);



    __TASK(1, inner_loop);

    // scalar: initialize the buffer
    inner_index_priv = __GET(inner_index);

    uint16_t temp;

    // scalar: redirect the data access to the buffer
    // vector: initialize if read before the first write
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
    //wt
    sorted_priv[__GET(outer_index)] = val_outer;
    // vector: pre_commit after the first write
    if (!vbm_test(sorted_vbm[__GET(outer_index)])) {
        vbm_set(sorted_vbm[__GET(outer_index)]);
        write_to_gbuf(&sorted_priv[__GET(outer_index)], &sorted[__GET(outer_index)], sizeof(sorted[__GET(outer_index)]));
    }

    sorted_priv[inner_index_priv] = val_inner;
    if (!vbm_test(sorted_vbm[inner_index_priv])) {
        vbm_set(sorted_vbm[inner_index_priv]);
        write_to_gbuf(&sorted_priv[inner_index_priv], &sorted[inner_index_priv], sizeof(sorted[inner_index_priv]));
    }



    // scalar: redirect
    ++inner_index_priv;
    if (inner_index_priv < SORT_LENGTH) {

        // scalar: pre_commit before transition_to=
        write_to_gbuf(&inner_index_priv, &inner_index, sizeof(inner_index));
        __TRANSITION_TO(1, inner_loop);
    }
    else {
        write_to_gbuf(&inner_index_priv, &inner_index, sizeof(inner_index));
        __TRANSITION_TO(2, outer_loop);
    }


    __TASK(2, outer_loop);
    
    outer_index_priv = __GET(outer_index);

    ++outer_index_priv;
    __GET(inner_index) = outer_index_priv + 1;

    if (outer_index_priv < SORT_LENGTH - 1){
        write_to_gbuf(&outer_index_priv, &outer_index, sizeof(outer_index));
        __TRANSITION_TO(1, inner_loop);}
    else {  
        write_to_gbuf(&outer_index_priv, &outer_index, sizeof(outer_index));
        __TASK_DOWN;  //return
    }


} //for main

