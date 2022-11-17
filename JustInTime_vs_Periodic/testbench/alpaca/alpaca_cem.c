#include <testbench/alpaca.h>
#include <testbench/global_declaration.h>
#include <testbench/testbench_api.h>nclude <scheduling/scheduler_based_task.h>

__GLOBAL_SCALAR(uint16_t,       _v_letter_idx);
__GLOBAL_SCALAR(cem_sample_t,   _v_prev_sample);
__GLOBAL_SCALAR(cem_index_t,    _v_sample_count);
__GLOBAL_SCALAR(cem_index_t,    _v_sibling);
__GLOBAL_SCALAR(cem_index_t,    _v_node_count);
__GLOBAL_SCALAR(cem_index_t,    _v_out_len);

__GLOBAL_SCALAR(cem_letter_t,   _v_letter);
__GLOBAL_SCALAR(cem_sample_t,   _v_sample);
__GLOBAL_SCALAR(cem_index_t,    _v_child);
__GLOBAL_SCALAR(cem_index_t,    _v_parent);
__GLOBAL_SCALAR(cem_index_t,    _v_parent_next);
__GLOBAL_SCALAR(cem_node_t,     _v_parent_node);
__GLOBAL_SCALAR(cem_node_t,     _v_sibling_node);
__GLOBAL_SCALAR(cem_index_t,    _v_symbol);

__GLOBAL_ARRAY(cem_node_t,     _v_compressed_data,CEM_BLOCK_SIZE);
__GLOBAL_ARRAY(cem_node_t,     _v_dict,CEM_DICT_SIZE);

static __nv uint16_t _v_letter_idx_priv;  
static __nv cem_sample_t _v_prev_sample_priv;
static __nv cem_index_t _v_out_len_priv;
static __nv cem_index_t _v_node_count_priv;
static __nv cem_index_t _v_sample_count_priv;
static __nv cem_index_t _v_sibling_priv;

static __nv uint16_t  status = 0;

void alpaca_cem_main()
{

switch(__GET_CURTASK) {

    case 0: goto  init;//               false);
    case 1: goto  Dict_Init;//          false);
    case 2: goto  Sample;//             true );
    case 3: goto  Measure_Temp;//       true );
    case 4: goto  Letterize;//          false); 
    case 5: goto  Compress;//           true );
    case 6: goto  Find_Sibling;//       true );
    case 7: goto  Add_Node;//           true );
    case 8: goto  Add_Insert;//         true );
    case 9: goto  AppendCompressed;//   true ); 

}




__TASK(0, init);


__GET(_v_parent_next) = 0;
__GET(_v_out_len) = 0;
__GET(_v_letter) = 0;
__GET(_v_prev_sample) = 0;
__GET(_v_letter_idx) = 0;
__GET(_v_sample_count) = 1;

__TRANSITION_TO(1, Dict_Init);

__TASK(1, Dict_Init);

for (uint16_t i = 0; i < CEM_NUM_LETTERS; ++i) 
{
   __GET(_v_dict[i].letter) = i;
   __GET(_v_dict[i].sibling) = CEM_NIL;
   __GET(_v_dict[i].child) = CEM_NIL;
}

__GET(_v_letter) = CEM_NUM_LETTERS + 1;
__GET(_v_node_count) = CEM_NUM_LETTERS;


__TRANSITION_TO(2, Sample);



__TASK(2, Sample);

_v_letter_idx_priv = __GET(_v_letter_idx);

uint16_t next_letter_idx = _v_letter_idx_priv + 1;
if (next_letter_idx == CEM_NUM_LETTERS_IN_SAMPLE)
   next_letter_idx = 0;

if (_v_letter_idx_priv == 0)
{
   _v_letter_idx_priv = next_letter_idx;

   __PRE_COMMIT(&_v_letter_idx_priv, &_v_letter_idx, sizeof(_v_letter_idx));
   __TRANSITION_TO(3, Measure_Temp);
}
else
{
    _v_letter_idx_priv = next_letter_idx;
   //3    
   __PRE_COMMIT(&_v_letter_idx_priv, &_v_letter_idx, sizeof(_v_letter_idx));
   __TRANSITION_TO(4, Letterize);
}



__TASK(3, Measure_Temp);
// 1.
_v_prev_sample_priv = __GET(_v_prev_sample); 

cem_sample_t prev_sample = _v_prev_sample_priv;
cem_sample_t sample = CEM_AcquireSample(prev_sample);
prev_sample = sample;
_v_prev_sample_priv = prev_sample;
__GET(_v_sample) = sample;

//3
__PRE_COMMIT(&_v_prev_sample_priv, &_v_prev_sample, sizeof(_v_prev_sample));

__TRANSITION_TO(4, Letterize);



__TASK(4, Letterize);

uint16_t letter_idx = __GET(_v_letter_idx);
if (letter_idx == 0)
   letter_idx = CEM_NUM_LETTERS_IN_SAMPLE;
else
   letter_idx--;

uint16_t letter_shift = CEM_LETTER_SIZE_BITS * letter_idx;
cem_letter_t letter =
        ( __GET(_v_sample) & (CEM_LETTER_MASK << letter_shift) ) >> letter_shift;
__GET(_v_letter) = letter;

__TRANSITION_TO(5, Compress);

__TASK(5, Compress);

_v_sample_count_priv = __GET(_v_sample_count);

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
__PRE_COMMIT(&_v_sample_count_priv, &_v_sample_count, sizeof(_v_sample_count));
__TRANSITION_TO(6, Find_Sibling);



__TASK(6, Find_Sibling);

_v_sibling_priv = __GET(_v_sibling); 

if (_v_sibling_priv != CEM_NIL)
{
   int16_t idx = _v_sibling_priv;
   if(__GET(_v_dict[idx].letter) == __GET(_v_letter))
   {
       __GET(_v_parent_next) = _v_sibling_priv;

       __PRE_COMMIT(&_v_sibling_priv, &_v_sibling, sizeof(_v_sibling));

       __TRANSITION_TO(4, Letterize);
   }
   else
   {
       if (__GET(_v_dict[idx].sibling) != 0)
       {
           _v_sibling_priv = __GET(_v_dict[idx].sibling);
           //3
           __PRE_COMMIT(&_v_sibling_priv, &_v_sibling, sizeof(_v_sibling));

           __TRANSITION_TO(6, Find_Sibling);
       }
   }
}

cem_index_t starting_node_idx = (cem_index_t)__GET(_v_letter);
__GET(_v_parent_next) = starting_node_idx;
if (__GET(_v_child) == CEM_NIL) {
    //3
    __PRE_COMMIT(&_v_sibling_priv, &_v_sibling, sizeof(_v_sibling));
    __TRANSITION_TO(8, Add_Insert);
}
else {
    //3
    __PRE_COMMIT(&_v_sibling_priv, &_v_sibling, sizeof(_v_sibling));
    __TRANSITION_TO(7, Add_Node);
}


__TASK(7, Add_Node); 
//war _v_sibling
_v_sibling_priv = __GET(_v_sibling);

uint16_t i = _v_sibling_priv;
if (__GET(_v_dict[i].sibling) != CEM_NIL)
{
   cem_index_t next_sibling = __GET(_v_dict[i].sibling);
   _v_sibling_priv = next_sibling;
   __PRE_COMMIT(&_v_sibling_priv, &_v_sibling, sizeof(_v_sibling));
   __TRANSITION_TO(7, Add_Node);
}
else
{
   __GET(_v_sibling_node.letter) = __GET(_v_dict[i].letter);
   __GET(_v_sibling_node.sibling) = __GET(_v_dict[i].sibling);
   __GET(_v_sibling_node.child) = __GET(_v_dict[i].child);
   __PRE_COMMIT(&_v_sibling_priv, &_v_sibling, sizeof(_v_sibling));
   __TRANSITION_TO(8, Add_Insert);
}



__TASK(8, Add_Insert);
// 1.
_v_node_count_priv = __GET(_v_node_count);


if (_v_node_count_priv == CEM_DICT_SIZE)
   while(1){}

cem_index_t child = _v_node_count_priv;
if (__GET(_v_parent_node.child) == CEM_NIL)
{
    i = __GET(_v_parent);
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
__PRE_COMMIT(&_v_node_count_priv, &_v_node_count, sizeof(_v_node_count));

__TRANSITION_TO(9, AppendCompressed);

__TASK(9, AppendCompressed);

    //1.
    _v_out_len_priv = __GET(_v_out_len);//196

__GET(_v_compressed_data[__GET(_v_out_len)].letter) = __GET(_v_symbol);
_v_out_len_priv++;
if(_v_out_len_priv == CEM_BLOCK_SIZE) {
    //3
    __PRE_COMMIT(&_v_out_len_priv, &_v_out_len, sizeof(_v_out_len));

    __TASK_DOWN; 
}
else {
    //3
    __PRE_COMMIT(&_v_out_len_priv, &_v_out_len, sizeof(_v_out_len));
    
    __TRANSITION_TO(2, Sample);
}

}//main



