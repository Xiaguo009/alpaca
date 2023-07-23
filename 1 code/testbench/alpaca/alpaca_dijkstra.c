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

static __nv uint16_t  status = 0;  //cur_task->id


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
    //1.
    //src_node_priv = __GET(src_node);

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

//3
//write_to_gbuf(&src_node_priv, &src_node, sizeof(src_node));
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
   write_to_gbuf(&deq_idx_priv, &deq_idx, sizeof(deq_idx));
   __TRANSITION_TO(2, Shorter_Path_Find);
}
else {
    //3 unnecessary
    write_to_gbuf(&deq_idx_priv, &deq_idx, sizeof(deq_idx));
    __TASK_DOWN;  //return__TRANSITION_TO( TASK_FINISH;
}




__TASK(2, Shorter_Path_Find);
// 1.
enq_idx_priv = __GET(enq_idx);//
node_idx_priv = __GET(node_idx);//
//nearest_node_priv = __GET(nearest_node);

//2.
uint16_t node = __GET(nearest_node.node); 
//uint16_t node = nearest_node_priv.node;  //?
//uint16_t
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
    write_to_gbuf(&enq_idx_priv, &enq_idx, sizeof(enq_idx));
    write_to_gbuf(&node_idx_priv, &node_idx, sizeof(node_idx));
    //write_to_gbuf(&nearest_node_priv, &nearest_node, sizeof(nearest_node));
    __TRANSITION_TO(2, Shorter_Path_Find);
}
else {
    //3
    write_to_gbuf(&enq_idx_priv, &enq_idx, sizeof(enq_idx));
    write_to_gbuf(&node_idx_priv, &node_idx, sizeof(node_idx));
    //write_to_gbuf(&nearest_node_priv, &nearest_node, sizeof(nearest_node));
    __TRANSITION_TO(1, Nearest_Node_Sel);
}

}//main

