#include <testbench/alpaca.h>
#include <testbench/global_declaration.h>
#include <testbench/testbench_api.h>

__GLOBAL_SCALAR(uint16_t,           deq_idx);
__GLOBAL_SCALAR(uint16_t,           enq_idx);
__GLOBAL_SCALAR(uint16_t,           node_idx);

__GLOBAL_ARRAY(dijkstra_node_t,    node_list,DIJKSTRA_NNODES);

__GLOBAL_ARRAY(dijkstra_queue_t,   queue,DIJKSTRA_QSIZE);

__GLOBAL_SCALAR(uint16_t,           src_node);
__GLOBAL_SCALAR(dijkstra_queue_t,   nearest_node);

//static __nv uint16_t  status = 0;  //cur_task->id
//for test
static __nv uint16_t  status = 0;  //task_id
//count for current bench
static __nv uint16_t bench_task_count = 0; //total execution times for all tasks in a bench
static __nv uint16_t bench_commit = 0; //total pre_commit size in a bench
//count for task[i]
static const uint8_t TASK_NUM = DIJKSTRA_TASK_NUM;
static __nv uint16_t task_count[TASK_NUM] = {0}; //total execution times for task[i]
static __nv uint16_t task_commit[TASK_NUM] = {0}; //total pre_commit size for all execution times of task[i]


// 0.
//static __nv uint16_t            src_node_priv;

static __nv uint16_t            deq_idx_priv;

static __nv uint16_t            enq_idx_priv;
static __nv uint16_t            node_idx_priv;
//static __nv dijkstra_queue_t    nearest_node_priv;



void alpaca_dijkstra_main()
{

    switch(__GET_CURTASK) {

    case 0:
        goto init;
    case 1:
        goto Nearest_Node_Sel;
    case 2:
        goto Shorter_Path_Find;

}



__TASK(0, init);


__GET(src_node) = 0;

__GET(queue[0].node) = __GET(src_node);
__GET(queue[0].dist) = 0;
__GET(queue[0].prev) = DIJKSTRA_UNDEFINED;
__GET(deq_idx) = 0;
__GET(enq_idx) = 1;

for(uint16_t i = 0; i < DIJKSTRA_NNODES; ++i)
{
     __GET(node_list[i].dist) = DIJKSTRA_INFINITY;
     __GET(node_list[i].prev) = DIJKSTRA_UNDEFINED;
}

__GET(node_list[__GET(src_node)].dist) = 0;
__GET(node_list[__GET(src_node)].prev) = DIJKSTRA_UNDEFINED;

__TRANSITION_TO(1,Nearest_Node_Sel);




__TASK(1, Nearest_Node_Sel);
// 1.
deq_idx_priv = __GET(deq_idx);


uint16_t i = deq_idx_priv;
if (__GET(enq_idx) != i)
{
   __GET(nearest_node.node) = __GET(queue[i].node);
   __GET(nearest_node.dist) = __GET(queue[i].dist);
   __GET(nearest_node.prev) = __GET(queue[i].prev);

   ++i;
   if (i < DIJKSTRA_QSIZE)
       deq_idx_priv = i;
   else
       deq_idx_priv = 0;

   __GET(node_idx) = 0;
   //3
   __PRE_COMMIT(&deq_idx_priv, &deq_idx, sizeof(deq_idx));
   __TRANSITION_TO(2, Shorter_Path_Find);
}
else {
    //3 unnecessary
    __PRE_COMMIT(&deq_idx_priv, &deq_idx, sizeof(deq_idx));
    __TASK_DOWN;  
}




__TASK(2, Shorter_Path_Find);
// 1.
enq_idx_priv = __GET(enq_idx);//
node_idx_priv = __GET(node_idx);//


//2.
uint16_t node = __GET(nearest_node.node); 
i = node_idx_priv;
uint16_t cost = adj_matrix[node][i];

if (cost != DIJKSTRA_INFINITY)
{
   uint16_t nearest_dist = __GET(nearest_node.dist);
   uint16_t dist = __GET(node_list[i].dist);
   if (dist == DIJKSTRA_INFINITY || dist > (cost + nearest_dist))
   {
       __GET(node_list[i].dist) = nearest_dist + cost;
       __GET(node_list[i].prev) = node;

       uint16_t j = enq_idx_priv;
       __GET(queue[j].node) = i;
       __GET(queue[j].dist) = nearest_dist + cost;
       __GET(queue[j].prev) = node;

       j++;
       if (j < DIJKSTRA_QSIZE)
           enq_idx_priv = j;
       else
           enq_idx_priv = 0;

   }
}

if (++node_idx_priv < DIJKSTRA_NNODES) {
    //3
    __PRE_COMMIT(&enq_idx_priv, &enq_idx, sizeof(enq_idx));
    __PRE_COMMIT(&node_idx_priv, &node_idx, sizeof(node_idx));
    __TRANSITION_TO(2, Shorter_Path_Find);
}
else {
    //3
    __PRE_COMMIT(&enq_idx_priv, &enq_idx, sizeof(enq_idx));
    __PRE_COMMIT(&node_idx_priv, &node_idx, sizeof(node_idx));
    __TRANSITION_TO(1, Nearest_Node_Sel);
}

}//main

