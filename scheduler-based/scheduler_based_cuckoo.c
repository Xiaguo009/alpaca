
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

// //to .h
// #define vbm_test(v) v == cur_version
// #define vbm_set(v) v = cur_version


//0. all
__nv cuckoo_fingerprint_t   _v_filter_priv[CUCKOO_NUM_BUCKETS];
__nv cuckoo_fingerprint_t   _v_filter_vbm[CUCKOO_NUM_BUCKETS];

//__nv uint16_t               _v_index_priv;

__nv cuckoo_value_t         _v_key_priv;

//__nv TaskName               _v_next_task_priv;
__nv cuckoo_fingerprint_t   _v_fingerprint_priv;
__nv cuckoo_value_t         _v_index1_priv;
//__nv cuckoo_value_t         _v_index2_priv;
__nv cuckoo_value_t         _v_relocation_count_priv;
__nv cuckoo_value_t         _v_insert_count_priv;

__nv cuckoo_value_t         _v_inserted_count_priv;
__nv cuckoo_value_t         _v_lookup_count_priv;

__nv cuckoo_value_t         _v_member_count_priv;
//__nv uint16_t               _v_success_priv;
//__nv uint16_t               _v_member_priv;


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
    //1.
    _v_key_priv = __GET(_v_key);


__GET(_v_key) = (__GET(_v_key) + 1) * 17;
if (__GET(_v_next_task) == CUCKOO_Insert) {
    //3
    write_to_gbuf(&_v_key_priv, &_v_key, sizeof(_v_key));
    return 3;
}
else if (__GET(_v_next_task) == CUCKOO_Lookup) {
    //3
    write_to_gbuf(&_v_key_priv, &_v_key, sizeof(_v_key));
    return 4;
}
//3
write_to_gbuf(&_v_key_priv, &_v_key, sizeof(_v_key));
return 2;
)

__TASK(2, shared_calc_index,

    //1. war in funcion
    _v_key_priv = __GET(_v_key);
_v_fingerprint_priv = __GET(_v_fingerprint);
    

_v_fingerprint_priv = CUCKOO_Hash2Fingerprint(_v_key_priv);

__GET(_v_index1) = CUCKOO_Hash2Index(_v_key_priv);

cuckoo_index_t fp_hash = CUCKOO_Hash2Index(_v_fingerprint_priv);
__GET(_v_index2) = __GET(_v_index1) ^ fp_hash;

if (__GET(_v_next_task) == CUCKOO_Add) {
    //3
    write_to_gbuf(&_v_key_priv, &_v_key, sizeof(_v_key));
    write_to_gbuf(&_v_fingerprint_priv, &_v_fingerprint, sizeof(_v_fingerprint));
    return 5;
}
else if (__GET(_v_next_task) == CUCKOO_Lookup_Search) {
    //3
    write_to_gbuf(&_v_key_priv, &_v_key, sizeof(_v_key));
    write_to_gbuf(&_v_fingerprint_priv, &_v_fingerprint, sizeof(_v_fingerprint));
    return 8;
}
//3
write_to_gbuf(&_v_key_priv, &_v_key, sizeof(_v_key));
write_to_gbuf(&_v_fingerprint_priv, &_v_fingerprint, sizeof(_v_fingerprint));
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

__TASK(5, Add,  //_v_filter[]
    // war?  _v_fingerprint _v_index1
    //1. 
    _v_index1_priv = __GET(_v_index1);
_v_fingerprint_priv = __GET(_v_fingerprint);


       uint16_t __cry_idx = _v_index1_priv;
       uint16_t __cry_idx2 = __GET(_v_index2);

       //read
       if (!vbm_test(_v_filter_vbm[__cry_idx])) {
           _v_filter_priv[__cry_idx] = __GET(_v_filter[__cry_idx]);
       }

       if (!__GET(_v_filter_priv[__cry_idx]))
       {
           __GET(_v_success) = 1;
           __GET(_v_filter_priv[__cry_idx]) = _v_fingerprint_priv;

           //wt
           if (!vbm_test(_v_filter_vbm[__cry_idx])) {
               vbm_set(_v_filter_vbm[__cry_idx]);
               write_to_gbuf(&_v_filter_priv[__cry_idx], &_v_filter[__cry_idx], sizeof(_v_filter[__cry_idx]));
           }

           //3
           write_to_gbuf(&_v_index1_priv, &_v_index1, sizeof(_v_index1));
           write_to_gbuf(&_v_fingerprint_priv, &_v_fingerprint, sizeof(_v_fingerprint));
           return 7;
       }
       else
       {
           //rd
           if (!vbm_test(_v_filter_vbm[__cry_idx2])) {
               _v_filter_priv[__cry_idx2] = __GET(_v_filter[__cry_idx2]);
           }

           if (!__GET(_v_filter_priv[__cry_idx2]))
           {
               __GET(_v_success) = 1;
               __GET(_v_filter_priv[__cry_idx2]) = _v_fingerprint_priv;

               //wt
               if (!vbm_test(_v_filter_vbm[__cry_idx2])) {
                   vbm_set(_v_filter_vbm[__cry_idx2]);
                   write_to_gbuf(&_v_filter_priv[__cry_idx2], &_v_filter[__cry_idx2], sizeof(_v_filter[__cry_idx2]));
               }

               //3
               write_to_gbuf(&_v_index1_priv, &_v_index1, sizeof(_v_index1));
               write_to_gbuf(&_v_fingerprint_priv, &_v_fingerprint, sizeof(_v_fingerprint));
               return 7;
           }
           else
           {
               cuckoo_fingerprint_t fp_victim;
               cuckoo_index_t index_victim;
               if (__GET(_v_key) % 2)   //! WARNNING: replace rand() with __GET(_v_key).
               {
                   index_victim = __cry_idx;

                   //rd
                   if (!vbm_test(_v_filter_vbm[__cry_idx])) {
                       _v_filter_priv[__cry_idx] = __GET(_v_filter[__cry_idx]);
                   }

                   fp_victim = __GET(_v_filter_priv[__cry_idx]);
               }
               else
               {
                   index_victim = __cry_idx2;

                   //rd
                   if (!vbm_test(_v_filter_vbm[__cry_idx2])) {
                       _v_filter_priv[__cry_idx2] = __GET(_v_filter[__cry_idx2]);
                   }

                   fp_victim = __GET(_v_filter_priv[__cry_idx2]);
               }
               __GET(_v_filter_priv[index_victim]) = _v_fingerprint_priv;
               //wt
               if (!vbm_test(_v_filter_vbm[index_victim])) {
                   vbm_set(_v_filter_vbm[index_victim]);
                   write_to_gbuf(&_v_filter_priv[index_victim], &_v_filter[index_victim], sizeof(_v_filter[index_victim]));
               }

               _v_index1_priv = index_victim;
               _v_fingerprint_priv = fp_victim;
               __GET(_v_relocation_count) = 0;
           }
       }
       //3
       write_to_gbuf(&_v_index1_priv, &_v_index1, sizeof(_v_index1));
       write_to_gbuf(&_v_fingerprint_priv, &_v_fingerprint, sizeof(_v_fingerprint));
       return 6;
)

__TASK(6, Relocate,
    //vector _v_filter[] 
    //1. war - _v_index1  _v_relocation_count  
    //1. war in function  -_v_fingerprint 
    _v_index1_priv = __GET(_v_index1);
_v_relocation_count_priv = __GET(_v_relocation_count);
_v_fingerprint_priv = __GET(_v_fingerprint);


cuckoo_fingerprint_t fp_victim = __GET(_v_fingerprint);
cuckoo_index_t fp_hash_victim = CUCKOO_Hash2Index(fp_victim);
cuckoo_index_t index2_victim = __GET(_v_index1) ^ fp_hash_victim;

__GET(_v_index1) ^ CUCKOO_Hash2Index(__GET(_v_fingerprint))  //??

//rd
if (!vbm_test(_v_filter_vbm[index2_victim])) {
    _v_filter_priv[index2_victim] = __GET(_v_filter[index2_victim]);
}

if (!__GET(_v_filter_priv[index2_victim]))
{
   __GET(_v_success) = 1;
   __GET(_v_filter_priv[index2_victim]) = fp_victim;
   //wt
   if (!vbm_test(_v_filter_vbm[index2_victim])) {
   vbm_set(_v_filter_vbm[index2_victim]);
   write_to_gbuf(&_v_filter_priv[index2_victim], &_v_filter[index2_victim], sizeof(_v_filter[index2_victim]));
}

   //3
   write_to_gbuf(&_v_index1_priv, &_v_index1, sizeof(_v_index1));
   write_to_gbuf(&_v_relocation_count_priv, &_v_relocation_count, sizeof(_v_relocation_count));
   write_to_gbuf(&_v_fingerprint_priv, &_v_fingerprint, sizeof(_v_fingerprint));

   return 7;
}
else
{
   if (__GET(_v_relocation_count) >= CUCKOO_MAX_RELOCATIONS)
   {
       __GET(_v_success) = 0;
       //3
       write_to_gbuf(&_v_index1_priv, &_v_index1, sizeof(_v_index1));
       write_to_gbuf(&_v_relocation_count_priv, &_v_relocation_count, sizeof(_v_relocation_count));
       write_to_gbuf(&_v_fingerprint_priv, &_v_fingerprint, sizeof(_v_fingerprint));

       return 7;
   }

   __GET(_v_relocation_count)++;
   __GET(_v_index1) = index2_victim;

   //rd
   if (!vbm_test(_v_filter_vbm[index2_victim])) {
       _v_filter_priv[index2_victim] = __GET(_v_filter[index2_victim]);
   }
   __GET(_v_fingerprint) = __GET(_v_filter_priv[index2_victim]);
   __GET(_v_filter_priv[index2_victim]) = fp_victim;
   //wt
   if (!vbm_test(_v_filter_vbm[index2_victim])) {
       vbm_set(_v_filter_vbm[index2_victim]);
       write_to_gbuf(&_v_filter_priv[index2_victim], &_v_filter[index2_victim], sizeof(_v_filter[index2_victim]));
   }

   //3
   write_to_gbuf(&_v_index1_priv, &_v_index1, sizeof(_v_index1));
   write_to_gbuf(&_v_relocation_count_priv, &_v_relocation_count, sizeof(_v_relocation_count));
   write_to_gbuf(&_v_fingerprint_priv, &_v_fingerprint, sizeof(_v_fingerprint));

   return 6;
}
)

__TASK(7, Insert_Done,

    //1. vector
//_v_filter_priv[CUCKOO_NUM_BUCKETS] = ;
//1.
//_v_index_priv = __GET(_v_index);

  //_v_key_priv = __GET(_v_key);

  //_v_next_task_priv = __GET(_v_next_task);
  //_v_fingerprint_priv = __GET(_v_fingerprint);
 /* _v_index1_priv = __GET(_v_index1);
  _v_index2_priv = __GET(_v_index2);
  _v_relocation_count_priv = __GET(_v_relocation_count);*/
  _v_insert_count_priv = __GET(_v_insert_count);//

  _v_inserted_count_priv = __GET(_v_insert_count); //+_v_insert_count
  


__GET(_v_insert_count)++;
uint16_t __cry = __GET(_v_inserted_count);
__cry += __GET(_v_success);
__GET(_v_inserted_count) = __cry;

if (__GET(_v_insert_count) < CUCKOO_NUM_INSERTS)
{
   __GET(_v_next_task) = CUCKOO_Insert;
   //3
   /*write_to_gbuf(&_v_index_priv, &_v_index, sizeof(_v_index));
   write_to_gbuf(&_v_key_priv, &_v_key, sizeof(_v_key));
   write_to_gbuf(&_v_next_task_priv, &_v_next_task, sizeof(_v_next_task));
   write_to_gbuf(&_v_fingerprint_priv, &_v_fingerprint, sizeof(_v_fingerprint));
   write_to_gbuf(&_v_index1_priv, &_v_index1, sizeof(_v_index1));
   write_to_gbuf(&_v_index2_priv, &_v_index2, sizeof(_v_index2));
   write_to_gbuf(&_v_relocation_count_priv, &_v_relocation_count, sizeof(_v_relocation_count));*/
   write_to_gbuf(&_v_insert_count_priv, &_v_insert_count, sizeof(_v_insert_count));
   write_to_gbuf(&_v_inserted_count_priv, &_v_inserted_count, sizeof(_v_inserted_count));

   return 1;
}
else
{
   __GET(_v_next_task) = CUCKOO_Lookup;
   __GET(_v_key) = cuckoo_init_key;
   //3
   write_to_gbuf(&_v_insert_count_priv, &_v_insert_count, sizeof(_v_insert_count));
   write_to_gbuf(&_v_inserted_count_priv, &_v_inserted_count, sizeof(_v_inserted_count));

   return 1;
}
)

__TASK(8, Lookup_Search, //+lookup_done  

    //1.

 _v_lookup_count_priv = __GET(_v_lookup_count);  //+_v_lookup_count

 _v_member_count_priv = __GET(_v_member_count);//
 //_v_success_priv = __GET(_v_success);
 //_v_member_priv = __GET(_v_member);
 


 if (__GET(_v_filter[__GET(_v_index1)]) == __GET(_v_fingerprint)) {
     __GET(_v_member) = 1;
 }
 else if (__GET(_v_filter[__GET(_v_index2)]) == __GET(_v_fingerprint)) {
     __GET(_v_member) = 1;
 }
 else {
     __GET(_v_member) = 0;
 }

__GET(_v_lookup_count)++;
uint16_t __cry = __GET(_v_member_count);
__cry += __GET(_v_member);
__GET(_v_member_count) = __cry;

if (__GET(_v_lookup_count) < CUCKOO_NUM_LOOKUPS)
{
   __GET(_v_next_task) = CUCKOO_Lookup;
   //3
   write_to_gbuf(&_v_lookup_count_priv, &_v_lookup_count, sizeof(_v_lookup_count));
   write_to_gbuf(&_v_member_count_priv, &_v_member_count, sizeof(_v_member_count));
  /* write_to_gbuf(&_v_success_priv, &_v_success, sizeof(_v_success));
   write_to_gbuf(&_v_member_priv, &_v_member, sizeof(_v_member));*/
   return 1;
}
else
{ 
    //3
    write_to_gbuf(&_v_lookup_count_priv, &_v_lookup_count, sizeof(_v_lookup_count));
    write_to_gbuf(&_v_member_count_priv, &_v_member_count, sizeof(_v_member_count));
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
