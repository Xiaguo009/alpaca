
#include <app/app_api.h>
#include <app/app_global.h>
#include <scheduling/scheduler.h>
#include <scheduling/scheduler_based_task.h>

__SHARED_VAR(
uint16_t       _v_letter_idx;
cem_sample_t   _v_prev_sample;
cem_index_t    _v_sample_count;
cem_index_t    _v_sibling;
cem_index_t    _v_node_count;
cem_index_t    _v_out_len;

cem_letter_t   _v_letter;
cem_sample_t   _v_sample;
cem_index_t    _v_child;
cem_index_t    _v_parent;
cem_index_t    _v_parent_next;
cem_node_t     _v_parent_node;
cem_node_t     _v_sibling_node;
cem_index_t    _v_symbol;

cem_node_t     _v_compressed_data[CEM_BLOCK_SIZE];
cem_node_t     _v_dict[CEM_DICT_SIZE];
)

// 0.
//node_t _v_compressed_data[BLOCK_SIZE];//192
__nv cem_letter_t _v_letter_priv;//193
//__nv uint16_t _v_letter_idx_priv;  //194

__nv cem_sample_t _v_prev_sample_priv;//195

__nv cem_index_t _v_out_len_priv;//196

__nv cem_index_t _v_node_count_priv;

//__nv cem_sample_t _v_sample_priv;//198

__nv cem_index_t _v_sample_count_priv;//199

__nv cem_index_t _v_sibling_priv;



__TASK_ENTRY(init,


__GET(_v_parent_next) = 0;
__GET(_v_out_len) = 0;
__GET(_v_letter) = 0;
__GET(_v_prev_sample) = 0;
__GET(_v_letter_idx) = 0;
__GET(_v_sample_count) = 1;

return 1;

)

__TASK(1, Dict_Init,

    //1.
    //_v_letter_priv = __GET(_v_letter);//193

for (uint16_t i = 0; i < CEM_NUM_LETTERS; ++i)
{
   __GET(_v_dict[i].letter) = i;
   __GET(_v_dict[i].sibling) = CEM_NIL;
   __GET(_v_dict[i].child) = CEM_NIL;
}

__GET(_v_letter) = CEM_NUM_LETTERS + 1;
__GET(_v_node_count) = CEM_NUM_LETTERS;

//3
//write_to_gbuf(&_v_letter_priv, &_v_letter, sizeof(_v_letter));
return 2;

)

__TASK(2, Sample,
    //1.
_v_letter_priv = __GET(_v_letter);//193 //
 //_v_letter_idx_priv = __GET(_v_letter);  //194

 //_v_prev_sample_priv = __GET(_v_prev_sample);//195

 //_v_out_len_priv = __GET(_v_out_len);//196

 //_v_node_count_priv = __GET(_v_node_count);

 //_v_sample_priv = __GET(_v_sample);//198

 //_v_sample_count_priv = __GET(_v_sample_count);//199
 

uint16_t next_letter_idx = _v_letter_priv + 1;
if (next_letter_idx == CEM_NUM_LETTERS_IN_SAMPLE)
   next_letter_idx = 0;

if (_v_letter_priv == 0)
{
   __v_letter_priv = next_letter_idx;

       //3    
       write_to_gbuf(&_v_letter_priv, &_v_letter, sizeof(_v_letter));
       //write_to_gbuf(&_v_letter_idx_priv, &_v_letter, sizeof(_v_letter));
       //write_to_gbuf(&_v_prev_sample_priv, &_v_prev_sample, sizeof(_v_prev_sample));
       //write_to_gbuf(&_v_out_len_priv, &_v_out_len, sizeof(_v_out_len));
       //write_to_gbuf(&_v_node_count_priv, &_v_node_count, sizeof(_v_node_count));
       //write_to_gbuf(&_v_sample_priv, &_v_sample, sizeof(_v_sample));
       //write_to_gbuf(&_v_sample_count_priv, &_v_sample_count, sizeof(_v_sample_count));
   return 3;
}
else
{
    _v_letter_priv = next_letter_idx;
   //3    
   write_to_gbuf(&_v_letter_priv, &_v_letter, sizeof(_v_letter));

   return 4;
}

)

__TASK(3, Measure_Temp,
    //1.
    _v_prev_sample_priv = __GET(_v_prev_sample);//195

cem_sample_t prev_sample = _v_prev_sample_priv;
cem_sample_t sample = CEM_AcquireSample(prev_sample);
prev_sample = sample;
_v_prev_sample_priv = prev_sample;
__GET(_v_sample) = sample;

//3
write_to_gbuf(&_v_prev_sample_priv, &_v_prev_sample, sizeof(_v_prev_sample));

return 4;

)

__TASK(4, Letterize,

uint16_t letter_idx = __GET(_v_letter_idx);
if (letter_idx == 0)
   letter_idx = CEM_NUM_LETTERS_IN_SAMPLE;
else
   letter_idx--;

uint16_t letter_shift = CEM_LETTER_SIZE_BITS * letter_idx;
cem_letter_t letter =
        ( __GET(_v_sample) & (CEM_LETTER_MASK << letter_shift) ) >> letter_shift;
__GET(_v_letter) = letter;

return 5;

)

__TASK(5, Compress,

    //1.
    _v_sample_count_priv = __GET(_v_sample_count);//199

cem_index_t parent = __GET(_v_parent_next);
__GET(_v_parent_node.letter) = __GET(_v_dict[parent].letter);
__GET(_v_parent_node.sibling) = __GET(_v_dict[parent].sibling);
__GET(_v_parent_node.child) = __GET(_v_dict[parent].child);
__GET(_v_sibling) = __GET(_v_dict[parent].child);
__GET(_v_parent) = parent;
__GET(_v_child) = __GET(_v_dict[parent].child);
//2.
_v_sample_count_priv++;

//3
write_to_gbuf(&_v_sample_count_priv, &_v_sample_count, sizeof(_v_sample_count));
return 6;

)

__TASK(6, Find_Sibling,
    //1.
    _v_sibling_priv = __GET(_v_sibling);//198


if (_v_sibling_priv != CEM_NIL)
{
   int16_t idx = _v_sibling_priv;
   if(__GET(_v_dict[idx].letter) == __GET(_v_letter))
   {
       __GET(_v_parent_next) = _v_sibling_priv;

        //3 unnecessary?
       write_to_gbuf(&_v_sibling_priv, &_v_sibling, sizeof(_v_sibling));

       return 4;
   }
   else
   {
       if (__GET(_v_dict[idx].sibling) != 0)
       {
           _v_sibling_priv = __GET(_v_dict[idx].sibling);
           //3
           write_to_gbuf(&_v_sibling_priv, &_v_sibling, sizeof(_v_sibling));

           return 6;
       }
   }
}

cem_index_t starting_node_idx = (cem_index_t)__GET(_v_letter);
__GET(_v_parent_next) = starting_node_idx;
if (__GET(_v_child) == CEM_NIL) {
    //3
    write_to_gbuf(&_v_sibling_priv, &_v_sibling, sizeof(_v_sibling));
    return 8;
}
else {
    //3
    write_to_gbuf(&_v_sibling_priv, &_v_sibling, sizeof(_v_sibling));
    return 7;
}
)

__TASK(7, Add_Node,

uint16_t i = __GET(_v_sibling);
if (__GET(_v_dict[i].sibling) != CEM_NIL)
{
   cem_index_t next_sibling = __GET(_v_dict[i].sibling);
   __GET(_v_sibling) = next_sibling;
   return 7;
}
else
{
   __GET(_v_sibling_node.letter) = __GET(_v_dict[i].letter);
   __GET(_v_sibling_node.sibling) = __GET(_v_dict[i].sibling);
   __GET(_v_sibling_node.child) = __GET(_v_dict[i].child);
   return 8;
}

)

__TASK(8, Add_Insert,
    //1.
    _v_node_count_priv = __GET(_v_node_count);


if (_v_node_count_priv == CEM_DICT_SIZE)
   while(1){}

cem_index_t child = _v_node_count_priv;
if (__GET(_v_parent_node.child) == CEM_NIL)
{
   int16_t i = __GET(_v_parent);
   __GET(_v_dict[i].letter) = __GET(_v_parent_node.letter);
   __GET(_v_dict[i].sibling) = __GET(_v_parent_node.sibling);
   __GET(_v_dict[i].child) = child;
}
else
{
   cem_index_t last_sibling = __GET(_v_sibling);
   __GET(_v_dict[last_sibling].letter) = __GET(_v_sibling_node.letter);
   __GET(_v_dict[last_sibling].sibling) = child;
   __GET(_v_dict[last_sibling].child) = __GET(_v_sibling_node.child);
}

__GET(_v_dict[child].letter) = __GET(_v_letter);
__GET(_v_dict[child].sibling) = CEM_NIL;
__GET(_v_dict[child].child) = CEM_NIL;
__GET(_v_symbol) = __GET(_v_parent);
_v_node_count_priv++;

//3
write_to_gbuf(&_v_node_count_priv, &_v_node_count, sizeof(_v_node_count));

return 9;

)


__TASK(9, AppendCompressed,

    //1.
    _v_out_len_priv = __GET(_v_out_len);//196

__GET(_v_compressed_data[__GET(_v_out_len)].letter) = __GET(_v_symbol);
_v_out_len_priv++;
if(_v_out_len_priv == CEM_BLOCK_SIZE) {
    //3
    write_to_gbuf(&_v_out_len_priv, &_v_out_len, sizeof(_v_out_len));

    return TASK_FINISH;
}
else {
    //3
    write_to_gbuf(&_v_out_len_priv, &_v_out_len, sizeof(_v_out_len));
    
    return 2;
}

)


void cem_regist()
{
    task_regist(0,  init,                false);
    task_regist(1,  Dict_Init,           false);
    task_regist(2,  Sample,              true );
    task_regist(3,  Measure_Temp,        true );
    task_regist(4,  Letterize,           false);
    task_regist(5,  Compress,            true );
    task_regist(6,  Find_Sibling,        true );
    task_regist(7,  Add_Node,            true );
    task_regist(8,  Add_Insert,          true );
    task_regist(9,  AppendCompressed,    true );

    WAR_REGIST(6);
}
