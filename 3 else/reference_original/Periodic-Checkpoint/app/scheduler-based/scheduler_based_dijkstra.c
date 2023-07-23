
#include <app/app_api.h>
#include <app/app_global.h>
#include <scheduling/scheduler.h>
#include <scheduling/scheduler_based_task.h>

__SHARED_VAR(
uint16_t            deq_idx;
uint16_t            enq_idx;
uint16_t            node_idx;
dijkstra_node_t     node_list[DIJKSTRA_NNODES];

dijkstra_queue_t    queue[DIJKSTRA_QSIZE];
uint16_t            src_node;
dijkstra_queue_t    nearest_node;
)


__TASK_ENTRY(init,


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

return 1;

)


__TASK(1, Nearest_Node_Sel,


uint16_t i = __GET(deq_idx);
if (__GET(enq_idx) != i)
{
   __GET(nearest_node.node) = __GET(queue[i].node);
   __GET(nearest_node.dist) = __GET(queue[i].dist);
   __GET(nearest_node.prev) = __GET(queue[i].prev);

   ++i;
   if (i < DIJKSTRA_QSIZE)
       __GET(deq_idx) = i;
   else
       __GET(deq_idx) = 0;

   __GET(node_idx) = 0;
   return 2;
}
else {
    return TASK_FINISH;
}

)


__TASK(2, Shorter_Path_Find,

uint16_t node = __GET(nearest_node.node);
uint16_t i = __GET(node_idx);
uint16_t cost = adj_matrix[node][i];

if (cost != DIJKSTRA_INFINITY)
{
   uint16_t nearest_dist = __GET(nearest_node.dist);
   uint16_t dist = __GET(node_list[i].dist);
   if (dist == DIJKSTRA_INFINITY || dist > (cost + nearest_dist))
   {
       __GET(node_list[i].dist) = nearest_dist + cost;
       __GET(node_list[i].prev) = node;

       uint16_t j = __GET(enq_idx);
       __GET(queue[j].node) = i;
       __GET(queue[j].dist) = nearest_dist + cost;
       __GET(queue[j].prev) = node;

       j++;
       if (j < DIJKSTRA_QSIZE)
           __GET(enq_idx) = j;
       else
           __GET(enq_idx) = 0;

   }
}

if(++__GET(node_idx) < DIJKSTRA_NNODES)
   return 2;
else
   return 1;


)


void dijkstra_regist()
{
    task_regist(0, init,                false);
    task_regist(1, Nearest_Node_Sel,    true );
    task_regist(2, Shorter_Path_Find,   true );

    WAR_REGIST(2 * DIJKSTRA_NNODES + 3);
}
