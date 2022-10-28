
#include <app/app_api.h>
#include <app/app_global.h>
#include <scheduling/scheduler.h>
#include <scheduling/scheduler_based_task.h>

__SHARED_VAR(
cuckoo_value_t         _v_key;

cuckoo_value_t         _v_insert_count;
cuckoo_value_t         _v_inserted_count;

cuckoo_value_t         _v_lookup_count;
cuckoo_value_t         _v_member_count;

cuckoo_fingerprint_t   _v_filter[CUCKOO_NUM_BUCKETS];
cuckoo_value_t         _v_index1;
cuckoo_value_t         _v_index2;
cuckoo_fingerprint_t   _v_fingerprint;
cuckoo_value_t         _v_relocation_count;

uint16_t               _v_index;
TaskName               _v_next_task;
uint16_t               _v_success;
uint16_t               _v_member;
)


__TASK_ENTRY(init,

for (uint16_t i = 0; i < CUCKOO_NUM_BUCKETS; ++i)
    __GET(_v_filter[i]) = 0;

__GET(_v_insert_count) = 0;
__GET(_v_lookup_count) = 0;
__GET(_v_inserted_count) = 0;
__GET(_v_member_count) = 0;
__GET(_v_key) = cuckoo_init_key;
__GET(_v_next_task) = CUCKOO_Insert;

return 1;

)

__TASK(1, KeyGenerate,

__GET(_v_key) = (__GET(_v_key) + 1) * 17;
if (__GET(_v_next_task) == CUCKOO_Insert)
   return 3;
else if (__GET(_v_next_task) == CUCKOO_Lookup)
   return 4;
return 2;
)

__TASK(2, shared_calc_index,

__GET(_v_fingerprint) = CUCKOO_Hash2Fingerprint(__GET(_v_key));

__GET(_v_index1) = CUCKOO_Hash2Index(__GET(_v_key));

cuckoo_index_t fp_hash = CUCKOO_Hash2Index(__GET(_v_fingerprint));
__GET(_v_index2) = __GET(_v_index1) ^ fp_hash;

if (__GET(_v_next_task) == CUCKOO_Add)
   return 5;
else if (__GET(_v_next_task) == CUCKOO_Lookup_Search)
   return 8;
return 3;
)

__TASK(3, Insert,

__GET(_v_next_task) = CUCKOO_Add;
return 2;
)

__TASK(4, Lookup,

__GET(_v_next_task) = CUCKOO_Lookup_Search;
return 2;
)

__TASK(5, Add,

       uint16_t __cry_idx = __GET(_v_index1);
       uint16_t __cry_idx2 = __GET(_v_index2);

       if (!__GET(_v_filter[__cry_idx]))
       {
           __GET(_v_success) = 1;
           __GET(_v_filter[__cry_idx]) = __GET(_v_fingerprint);
           return 7;
       }
       else
       {
           if (!__GET(_v_filter[__cry_idx2]))
           {
               __GET(_v_success) = 1;
               __GET(_v_filter[__cry_idx2]) = __GET(_v_fingerprint);
               return 7;
           }
           else
           {
               cuckoo_fingerprint_t fp_victim;
               cuckoo_index_t index_victim;
               if (__GET(_v_key) % 2)   //! WARNNING: replace rand() with __GET(_v_key).
               {
                   index_victim = __cry_idx;
                   fp_victim = __GET(_v_filter[__cry_idx]);
               }
               else
               {
                   index_victim = __cry_idx2;
                   fp_victim = __GET(_v_filter[__cry_idx2]);
               }
               __GET(_v_filter[index_victim]) = __GET(_v_fingerprint);
               __GET(_v_index1) = index_victim;
               __GET(_v_fingerprint) = fp_victim;
               __GET(_v_relocation_count) = 0;
           }
       }
       return 6;
)

__TASK(6, Relocate,

cuckoo_fingerprint_t fp_victim = __GET(_v_fingerprint);
cuckoo_index_t fp_hash_victim = CUCKOO_Hash2Index(fp_victim);
cuckoo_index_t index2_victim = __GET(_v_index1) ^ fp_hash_victim;

if (!__GET(_v_filter[index2_victim]))
{
   __GET(_v_success) = 1;
   __GET(_v_filter[index2_victim]) = fp_victim;
   return 7;
}
else
{
   if (__GET(_v_relocation_count) >= CUCKOO_MAX_RELOCATIONS)
   {
       __GET(_v_success) = 0;
       return 7;
   }

   __GET(_v_relocation_count)++;
   __GET(_v_index1) = index2_victim;
   __GET(_v_fingerprint) = __GET(_v_filter[index2_victim]);
   __GET(_v_filter[index2_victim]) = fp_victim;
   return 6;
}
)

__TASK(7, Insert_Done,

__GET(_v_insert_count)++;
uint16_t __cry = __GET(_v_inserted_count);
__cry += __GET(_v_success);
__GET(_v_inserted_count) = __cry;

if (__GET(_v_insert_count) < CUCKOO_NUM_INSERTS)
{
   __GET(_v_next_task) = CUCKOO_Insert;
   return 1;
}
else
{
   __GET(_v_next_task) = CUCKOO_Lookup;
   __GET(_v_key) = cuckoo_init_key;
   return 1;
}
)

__TASK(8, Lookup_Search,


if (__GET(_v_filter[__GET(_v_index1)]) == __GET(_v_fingerprint))
       __GET(_v_member) = 1;
else if (__GET(_v_filter[__GET(_v_index2)]) == __GET(_v_fingerprint))
   __GET(_v_member) = 1;
else
   __GET(_v_member) = 0;

__GET(_v_lookup_count)++;
uint16_t __cry = __GET(_v_member_count);
__cry += __GET(_v_member);
__GET(_v_member_count) = __cry;

if (__GET(_v_lookup_count) < CUCKOO_NUM_LOOKUPS)
{
   __GET(_v_next_task) = CUCKOO_Lookup;
   return 1;
}
else
{
    return TASK_FINISH;
}

)


void cuckoo_regist()
{
    task_regist(0, init,                false);
    task_regist(1, KeyGenerate,         true );
    task_regist(2, shared_calc_index,   false);
    task_regist(3, Insert,              false);
    task_regist(4, Lookup,              false);
    task_regist(5, Add,                 true );
    task_regist(6, Relocate,            true );
    task_regist(7, Insert_Done,         true );
    task_regist(8, Lookup_Search,       true );
    WAR_REGIST(CUCKOO_NUM_BUCKETS + 9);
}
