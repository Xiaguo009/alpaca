#include <testbench/alpaca.h>
#include <testbench/global_declaration.h>
#include <testbench/testbench_api.h>

__GLOBAL_SCALAR(cuckoo_value_t,        _v_key);

__GLOBAL_SCALAR(cuckoo_value_t,        _v_insert_count);
__GLOBAL_SCALAR(cuckoo_value_t,        _v_inserted_count);

__GLOBAL_SCALAR(cuckoo_value_t,        _v_lookup_count);
__GLOBAL_SCALAR(cuckoo_value_t,        _v_member_count);

__GLOBAL_ARRAY(cuckoo_fingerprint_t,  _v_filter,CUCKOO_NUM_BUCKETS);

__GLOBAL_SCALAR(cuckoo_value_t,        _v_index1);
__GLOBAL_SCALAR(cuckoo_value_t,        _v_index2);
__GLOBAL_SCALAR(cuckoo_fingerprint_t,  _v_fingerprint);
__GLOBAL_SCALAR(cuckoo_value_t,        _v_relocation_count);

__GLOBAL_SCALAR(uint16_t,               _v_index);
__GLOBAL_SCALAR(TaskName,               _v_next_task);
__GLOBAL_SCALAR(uint16_t,               _v_success);
__GLOBAL_SCALAR(uint16_t,               _v_member);

//static __nv uint16_t  status = 0;  //cur_task->id
//for test
static __nv uint16_t  status = 0;  //task_id
//count for current bench
static __nv uint16_t bench_task_count = 0; //total execution times for all tasks in a bench
static __nv uint16_t bench_commit = 0; //total pre_commit size in a bench
//count for task[i]
static const uint8_t TASK_NUM = CUCKOO_TASK_NUM;
static __nv uint16_t task_count[TASK_NUM] = {0}; //total execution times for task[i]
static __nv uint16_t task_commit[TASK_NUM] = {0}; //total pre_commit size for all execution times of task[i]


//0. all
static __nv cuckoo_fingerprint_t   _v_filter_priv[CUCKOO_NUM_BUCKETS];
static __nv uint16_t   _v_filter_vbm[CUCKOO_NUM_BUCKETS];
static __nv cuckoo_value_t         _v_key_priv;
static __nv cuckoo_fingerprint_t   _v_fingerprint_priv;
static __nv cuckoo_value_t         _v_index1_priv;
static __nv cuckoo_value_t         _v_relocation_count_priv;
static __nv cuckoo_value_t         _v_insert_count_priv;
static __nv cuckoo_value_t         _v_inserted_count_priv;
static __nv cuckoo_value_t         _v_lookup_count_priv;
static __nv cuckoo_value_t         _v_member_count_priv;



void alpaca_cuckoo_main()
{
switch(__GET_CURTASK) {


    case 0:
        goto init;
    case 1:
        goto KeyGenerate;
    case 2:
        goto shared_calc_index;
    case 3:
        goto Add;
    case 4:
        goto Relocate;
    case 5:
        goto Insert_Done;
    case 6:
        goto Lookup_Search;
    }

__TASK(0, init);

for (uint16_t i = 0; i < CUCKOO_NUM_BUCKETS; ++i)
    __GET(_v_filter[i]) = 0;

__GET(_v_insert_count) = 0;
__GET(_v_lookup_count) = 0;
__GET(_v_inserted_count) = 0;
__GET(_v_member_count) = 0;
__GET(_v_key) = cuckoo_init_key;
__GET(_v_next_task) = CUCKOO_Insert;

__TRANSITION_TO( 1,KeyGenerate);



__TASK(1, KeyGenerate);
// 1.
_v_key_priv = __GET(_v_key);


_v_key_priv = (_v_key_priv + 1) * 17;
if (__GET(_v_next_task) == CUCKOO_Insert) {
    __GET(_v_next_task) = CUCKOO_Add;  

    __PRE_COMMIT(&_v_key_priv, &_v_key, sizeof(_v_key));
    __TRANSITION_TO(2, shared_calc_index);
}
else if (__GET(_v_next_task) == CUCKOO_Lookup) {
    __GET(_v_next_task) = CUCKOO_Lookup_Search;
    
    __PRE_COMMIT(&_v_key_priv, &_v_key, sizeof(_v_key));
    __TRANSITION_TO(2, shared_calc_index);
}
//3
__PRE_COMMIT(&_v_key_priv, &_v_key, sizeof(_v_key));
__TRANSITION_TO(2,shared_calc_index);


__TASK(2, shared_calc_index);

    //1. war in funcion
    _v_key_priv = __GET(_v_key);
_v_fingerprint_priv = __GET(_v_fingerprint);
    

_v_fingerprint_priv = CUCKOO_Hash2Fingerprint(_v_key_priv);

__GET(_v_index1) = CUCKOO_Hash2Index(_v_key_priv);

cuckoo_index_t fp_hash = CUCKOO_Hash2Index(_v_fingerprint_priv);
__GET(_v_index2) = __GET(_v_index1) ^ fp_hash;

if (__GET(_v_next_task) == CUCKOO_Add) {
    //3
    __PRE_COMMIT(&_v_key_priv, &_v_key, sizeof(_v_key));
    __PRE_COMMIT(&_v_fingerprint_priv, &_v_fingerprint, sizeof(_v_fingerprint));
    __TRANSITION_TO(3, Add);
}
else if (__GET(_v_next_task) == CUCKOO_Lookup_Search) {
    //3
    __PRE_COMMIT(&_v_key_priv, &_v_key, sizeof(_v_key));
    __PRE_COMMIT(&_v_fingerprint_priv, &_v_fingerprint, sizeof(_v_fingerprint));
    __TRANSITION_TO( 6,Lookup_Search);
}

    __GET(_v_next_task) = CUCKOO_Add;
    __TRANSITION_TO(2, shared_calc_index);


__TASK(3, Add);  //_v_filter[]
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
               __PRE_COMMIT(&_v_filter_priv[__cry_idx], &_v_filter[__cry_idx], sizeof(_v_filter[__cry_idx]));
           }

           //3
           __PRE_COMMIT(&_v_index1_priv, &_v_index1, sizeof(_v_index1));
           __PRE_COMMIT(&_v_fingerprint_priv, &_v_fingerprint, sizeof(_v_fingerprint));
           __TRANSITION_TO(5, Insert_Done);
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
                   __PRE_COMMIT(&_v_filter_priv[__cry_idx2], &_v_filter[__cry_idx2], sizeof(_v_filter[__cry_idx2]));
               }

               //3
               __PRE_COMMIT(&_v_index1_priv, &_v_index1, sizeof(_v_index1));
               __PRE_COMMIT(&_v_fingerprint_priv, &_v_fingerprint, sizeof(_v_fingerprint));
               __TRANSITION_TO(5, Insert_Done);
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
                   __PRE_COMMIT(&_v_filter_priv[index_victim], &_v_filter[index_victim], sizeof(_v_filter[index_victim]));
               }

               _v_index1_priv = index_victim;
               _v_fingerprint_priv = fp_victim;
               __GET(_v_relocation_count) = 0;
           }
       }
       //3
       __PRE_COMMIT(&_v_index1_priv, &_v_index1, sizeof(_v_index1));
       __PRE_COMMIT(&_v_fingerprint_priv, &_v_fingerprint, sizeof(_v_fingerprint));
       __TRANSITION_TO(4,Relocate);


__TASK(4, Relocate);
// vector _v_filter[]
// 1. war - _v_index1  _v_relocation_count
// 1. war in function  -_v_fingerprint
_v_index1_priv = __GET(_v_index1);
_v_relocation_count_priv = __GET(_v_relocation_count);
_v_fingerprint_priv = __GET(_v_fingerprint);


cuckoo_fingerprint_t fp_victim = _v_fingerprint_priv;
cuckoo_index_t fp_hash_victim = CUCKOO_Hash2Index(fp_victim);
cuckoo_index_t index2_victim = _v_index1_priv ^ fp_hash_victim;

//__GET(_v_index1) ^ CUCKOO_Hash2Index(__GET(_v_fingerprint))  //??

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
   __PRE_COMMIT(&_v_filter_priv[index2_victim], &_v_filter[index2_victim], sizeof(_v_filter[index2_victim]));
}

   //3
   __PRE_COMMIT(&_v_index1_priv, &_v_index1, sizeof(_v_index1));
   __PRE_COMMIT(&_v_relocation_count_priv, &_v_relocation_count, sizeof(_v_relocation_count));
   __PRE_COMMIT(&_v_fingerprint_priv, &_v_fingerprint, sizeof(_v_fingerprint));

   __TRANSITION_TO(5, Insert_Done);
}
else
{
   if (_v_relocation_count_priv >= CUCKOO_MAX_RELOCATIONS)
   {
       __GET(_v_success) = 0;
       //3
       __PRE_COMMIT(&_v_index1_priv, &_v_index1, sizeof(_v_index1));
       __PRE_COMMIT(&_v_relocation_count_priv, &_v_relocation_count, sizeof(_v_relocation_count));
       __PRE_COMMIT(&_v_fingerprint_priv, &_v_fingerprint, sizeof(_v_fingerprint));

       __TRANSITION_TO(5, Insert_Done);
   }

   _v_relocation_count_priv++;
   _v_index1_priv = index2_victim;

   //rd
   if (!vbm_test(_v_filter_vbm[index2_victim])) {
       _v_filter_priv[index2_victim] = __GET(_v_filter[index2_victim]);
   }
   _v_fingerprint_priv = __GET(_v_filter_priv[index2_victim]);
   __GET(_v_filter_priv[index2_victim]) = fp_victim;
   //wt
   if (!vbm_test(_v_filter_vbm[index2_victim])) {
       vbm_set(_v_filter_vbm[index2_victim]);
       __PRE_COMMIT(&_v_filter_priv[index2_victim], &_v_filter[index2_victim], sizeof(_v_filter[index2_victim]));
   }

   //3
   __PRE_COMMIT(&_v_index1_priv, &_v_index1, sizeof(_v_index1));
   __PRE_COMMIT(&_v_relocation_count_priv, &_v_relocation_count, sizeof(_v_relocation_count));
   __PRE_COMMIT(&_v_fingerprint_priv, &_v_fingerprint, sizeof(_v_fingerprint));

   __TRANSITION_TO(4,Relocate);
}

__TASK(5, Insert_Done);

  _v_insert_count_priv = __GET(_v_insert_count);

  _v_inserted_count_priv = __GET(_v_inserted_count);
  


_v_insert_count_priv++;
uint16_t __cry = _v_inserted_count_priv;
__cry += __GET(_v_success);
_v_inserted_count_priv = __cry;

if (_v_insert_count_priv < CUCKOO_NUM_INSERTS)
{
   __GET(_v_next_task) = CUCKOO_Insert;
   //3
   __PRE_COMMIT(&_v_insert_count_priv, &_v_insert_count, sizeof(_v_insert_count));
   __PRE_COMMIT(&_v_inserted_count_priv, &_v_inserted_count, sizeof(_v_inserted_count));

   __TRANSITION_TO( 1,KeyGenerate);
}
else
{
   __GET(_v_next_task) = CUCKOO_Lookup;
   __GET(_v_key) = cuckoo_init_key;
   //3
   __PRE_COMMIT(&_v_insert_count_priv, &_v_insert_count, sizeof(_v_insert_count));
   __PRE_COMMIT(&_v_inserted_count_priv, &_v_inserted_count, sizeof(_v_inserted_count));

   __TRANSITION_TO( 1,KeyGenerate);
}


__TASK(6, Lookup_Search); //+lookup_done

    //1.

 _v_lookup_count_priv = __GET(_v_lookup_count); 

 _v_member_count_priv = __GET(_v_member_count);
 


 if (__GET(_v_filter[__GET(_v_index1)]) == __GET(_v_fingerprint)) {
     __GET(_v_member) = 1;
 }
 else if (__GET(_v_filter[__GET(_v_index2)]) == __GET(_v_fingerprint)) {
     __GET(_v_member) = 1;
 }
 else {
     __GET(_v_member) = 0;
 }

_v_lookup_count_priv++;
//uint16_t
__cry = _v_member_count_priv;
__cry += __GET(_v_member);
_v_member_count_priv = __cry;

if (_v_lookup_count_priv < CUCKOO_NUM_LOOKUPS)
{
   __GET(_v_next_task) = CUCKOO_Lookup;
   //3
   __PRE_COMMIT(&_v_lookup_count_priv, &_v_lookup_count, sizeof(_v_lookup_count));
   __PRE_COMMIT(&_v_member_count_priv, &_v_member_count, sizeof(_v_member_count));
   __TRANSITION_TO( 1,KeyGenerate);
}
else
{ 
    //3
    __PRE_COMMIT(&_v_lookup_count_priv, &_v_lookup_count, sizeof(_v_lookup_count));
    __PRE_COMMIT(&_v_member_count_priv, &_v_member_count, sizeof(_v_member_count));
    __TASK_DOWN;  //return__TRANSITION_TO( TASK_FINISH;
}

}//main
