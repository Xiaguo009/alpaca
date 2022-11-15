#include <testbench/alpaca.h>
#include <testbench/global_declaration.h>
#include <testbench/testbench_api.h>

__GLOBAL_SCALAR(uint32_t, _v_seed);
__GLOBAL_SCALAR(uint16_t, _v_iter);
__GLOBAL_SCALAR(uint16_t, _v_func);
__GLOBAL_SCALAR(uint16_t, _v_n_0);
__GLOBAL_SCALAR(uint16_t, _v_n_1);
__GLOBAL_SCALAR(uint16_t, _v_n_2);
__GLOBAL_SCALAR(uint16_t, _v_n_3);
__GLOBAL_SCALAR(uint16_t, _v_n_4);
__GLOBAL_SCALAR(uint16_t, _v_n_5);
__GLOBAL_SCALAR(uint16_t, _v_n_6);

//static __nv uint16_t  status = 0;  //cur_task->id
//for test
static __nv uint16_t  status = 0;  //task_id
//count for current bench
static __nv uint16_t bench_task_count = 0; //total execution times for all tasks in a bench
static __nv uint16_t bench_commit = 0; //total pre_commit times in a bench
//count for task[i]
static const uint8_t TASK_NUM = BC_TASK_NUM;
static __nv uint16_t task_count[TASK_NUM] = {0}; //total execution times for task[i]
static __nv uint16_t task_commit[TASK_NUM] = {0}; //total pre_commit times for all execution times of task[i]


//0.declaration
static __nv uint16_t _v_n_0_priv;
static __nv uint16_t _v_n_1_priv;
static __nv uint16_t _v_n_2_priv;
static __nv uint16_t _v_n_3_priv;
static __nv uint16_t _v_n_4_priv;
static __nv uint16_t _v_n_5_priv;
static __nv uint16_t _v_n_6_priv;
static __nv uint16_t _v_func_priv;
static __nv uint32_t _v_seed_priv;
static __nv uint16_t _v_iter_priv;


void alpaca_bc_main()
{

switch(__GET_CURTASK) {

    case 0: goto init;//            false);
    case 1: goto Func_Select;//     true );
    case 2: goto func0;//           true );
    case 3: goto func1;//           true );
    case 4: goto NTBL0;//           true );
    case 5: goto NTBL1;//           true );
    case 6: goto BW_BTBL;//         true );
    case 7: goto AR_BTBL;//         true );
    case 8: goto Bit_Shifter;//     true );

    //WAR_REGIST(11);
}



__TASK(0, init);

__GET(_v_func) = 0;
__GET(_v_n_0) = 0;
__GET(_v_n_1) = 0;
__GET(_v_n_2) = 0;
__GET(_v_n_3) = 0;
__GET(_v_n_4) = 0;
__GET(_v_n_5) = 0;
__GET(_v_n_6) = 0;

__TRANSITION_TO(1,Func_Select);




__TASK(1, Func_Select);

    //1.
    _v_func_priv = __GET(_v_func);


__GET(_v_seed) = (uint32_t)BITCOUNT_SEED;
__GET(_v_iter) = 0;

__PRE_COMMIT(&_v_func_priv, &_v_func, sizeof(_v_func));
switch(_v_func_priv++)
{

case 0:     __TRANSITION_TO(2,func0);
case 1:     __TRANSITION_TO(3,func1);
case 2:     __TRANSITION_TO(4,NTBL0);
case 3:     __TRANSITION_TO(5, NTBL1);
case 4:     __TRANSITION_TO(6,BW_BTBL);
case 5:     __TRANSITION_TO(7, AR_BTBL);
case 6:     __TRANSITION_TO(8, Bit_Shifter);
default:    __TASK_DOWN;  //returnreturn TASK_FINISH;
}



__TASK(2, func0);//bit_count
    //1.
_v_n_0_priv = __GET(_v_n_0);//
//_v_n_1_priv = __GET(_v_n_1);
//_v_n_2_priv = __GET(_v_n_2);
//_v_n_3_priv = __GET(_v_n_3);
//_v_n_4_priv = __GET(_v_n_4);
//_v_n_5_priv = __GET(_v_n_5);
// _v_n_6_priv = __GET(_v_n_6);
// _v_func_priv = __GET(_v_func);
 _v_seed_priv = __GET(_v_seed);//
 _v_iter_priv = __GET(_v_iter);//



uint32_t tmp_seed = _v_seed_priv;
_v_seed_priv = tmp_seed + 13;
uint32_t temp = 0;

if (tmp_seed) do
   temp++;
while ( 0 != (tmp_seed = tmp_seed & (tmp_seed - 1)) );

_v_n_0_priv += temp;
_v_iter_priv++;
if (_v_iter_priv < BITCOUNT_ITER) {
    //3
    __PRE_COMMIT(&_v_n_0_priv, &_v_n_0, sizeof(_v_n_0));
    //__PRE_COMMIT(&_v_n_1_priv, &_v_n_1, sizeof(_v_n_1));
    //__PRE_COMMIT(&_v_n_2_priv, &_v_n_2, sizeof(_v_n_2));
    //__PRE_COMMIT(&_v_n_3_priv, &_v_n_3, sizeof(_v_n_3));
    //__PRE_COMMIT(&_v_n_4_priv, &_v_n_4, sizeof(_v_n_4));
    //__PRE_COMMIT(&_v_n_5_priv, &_v_n_5, sizeof(_v_n_5));
    //__PRE_COMMIT(&_v_n_6_priv, &_v_n_6, sizeof(_v_n_6));
    //__PRE_COMMIT(&_v_func_priv, &_v_func, sizeof(_v_func));
    __PRE_COMMIT(&_v_seed_priv, &_v_seed, sizeof(_v_seed));
    __PRE_COMMIT(&_v_iter_priv, &_v_iter, sizeof(_v_iter));
    __TRANSITION_TO(2,func0);
}
else {
    //3
    __PRE_COMMIT(&_v_n_0_priv, &_v_n_0, sizeof(_v_n_0));
    __PRE_COMMIT(&_v_seed_priv, &_v_seed, sizeof(_v_seed));
    __PRE_COMMIT(&_v_iter_priv, &_v_iter, sizeof(_v_iter));
    __TRANSITION_TO(1,Func_Select);
}


__TASK(3, func1);
// 1.
_v_n_1_priv = __GET(_v_n_1); //
//_v_n_2_priv = __GET(_v_n_2);
//_v_n_3_priv = __GET(_v_n_3);
//_v_n_4_priv = __GET(_v_n_4);
//_v_n_5_priv = __GET(_v_n_5);
//_v_n_6_priv = __GET(_v_n_6);
//_v_func_priv = __GET(_v_func);
_v_seed_priv = __GET(_v_seed);//
_v_iter_priv = __GET(_v_iter);//

//uint32_t
tmp_seed = _v_seed_priv;
_v_seed_priv = tmp_seed + 13;

tmp_seed = ((tmp_seed & 0xAAAAAAAAL) >>  1) + (tmp_seed & 0x55555555L);
tmp_seed = ((tmp_seed & 0xCCCCCCCCL) >>  2) + (tmp_seed & 0x33333333L);
tmp_seed = ((tmp_seed & 0xF0F0F0F0L) >>  4) + (tmp_seed & 0x0F0F0F0FL);
tmp_seed = ((tmp_seed & 0xFF00FF00L) >>  8) + (tmp_seed & 0x00FF00FFL);
tmp_seed = ((tmp_seed & 0xFFFF0000L) >> 16) + (tmp_seed & 0x0000FFFFL);

_v_n_1_priv += (int)tmp_seed;
_v_iter_priv++;
if (_v_iter_priv < BITCOUNT_ITER) {
    //3    
    __PRE_COMMIT(&_v_n_1_priv, &_v_n_1, sizeof(_v_n_1));
    //__PRE_COMMIT(&_v_n_2_priv, &_v_n_2, sizeof(_v_n_2));
    //__PRE_COMMIT(&_v_n_3_priv, &_v_n_3, sizeof(_v_n_3));
    //__PRE_COMMIT(&_v_n_4_priv, &_v_n_4, sizeof(_v_n_4));
    //__PRE_COMMIT(&_v_n_5_priv, &_v_n_5, sizeof(_v_n_5));
    //__PRE_COMMIT(&_v_n_6_priv, &_v_n_6, sizeof(_v_n_6));
    //__PRE_COMMIT(&_v_func_priv, &_v_func, sizeof(_v_func));
    __PRE_COMMIT(&_v_seed_priv, &_v_seed, sizeof(_v_seed));
    __PRE_COMMIT(&_v_iter_priv, &_v_iter, sizeof(_v_iter));
    __TRANSITION_TO(3,func1);
}
else {
    //3    
    __PRE_COMMIT(&_v_n_1_priv, &_v_n_1, sizeof(_v_n_1));
    __PRE_COMMIT(&_v_seed_priv, &_v_seed, sizeof(_v_seed));
    __PRE_COMMIT(&_v_iter_priv, &_v_iter, sizeof(_v_iter));
    __TRANSITION_TO(1,Func_Select);
}



__TASK(4, NTBL0);
// 1.
_v_n_2_priv = __GET(_v_n_2); //
//_v_n_3_priv = __GET(_v_n_3);
//_v_n_4_priv = __GET(_v_n_4);
//_v_n_5_priv = __GET(_v_n_5);
//_v_n_6_priv = __GET(_v_n_6);
//_v_func_priv = __GET(_v_func);
_v_seed_priv = __GET(_v_seed);//
_v_iter_priv = __GET(_v_iter);

//uint32_t
tmp_seed = _v_seed_priv;
_v_n_2_priv += Bitcount_NonRecursiveCnt(tmp_seed);
_v_seed_priv = tmp_seed + 13;

_v_iter_priv++;
if (_v_iter_priv < BITCOUNT_ITER) {
    //3    

    __PRE_COMMIT(&_v_n_2_priv, &_v_n_2, sizeof(_v_n_2));
 /*   __PRE_COMMIT(&_v_n_3_priv, &_v_n_3, sizeof(_v_n_3));
    __PRE_COMMIT(&_v_n_4_priv, &_v_n_4, sizeof(_v_n_4));
    __PRE_COMMIT(&_v_n_5_priv, &_v_n_5, sizeof(_v_n_5));
    __PRE_COMMIT(&_v_n_6_priv, &_v_n_6, sizeof(_v_n_6));
    __PRE_COMMIT(&_v_func_priv, &_v_func, sizeof(_v_func));*/
    __PRE_COMMIT(&_v_seed_priv, &_v_seed, sizeof(_v_seed));
    __PRE_COMMIT(&_v_iter_priv, &_v_iter, sizeof(_v_iter));
    __TRANSITION_TO(4,NTBL0);
}
else {
    //3    

    __PRE_COMMIT(&_v_n_2_priv, &_v_n_2, sizeof(_v_n_2));
    __PRE_COMMIT(&_v_seed_priv, &_v_seed, sizeof(_v_seed));
    __PRE_COMMIT(&_v_iter_priv, &_v_iter, sizeof(_v_iter));
    __TRANSITION_TO(1,Func_Select);
}


__TASK(5, NTBL1);
// 1.
_v_n_3_priv = __GET(_v_n_3);
//_v_n_4_priv = __GET(_v_n_4);
//_v_n_5_priv = __GET(_v_n_5);
//_v_n_6_priv = __GET(_v_n_6);
//_v_func_priv = __GET(_v_func);
_v_seed_priv = __GET(_v_seed);
_v_iter_priv = __GET(_v_iter);

uint16_t __cry = _v_seed_priv;

_v_n_3_priv += bc_bits[ (int) (__cry & 0x0000000FUL)] +
        bc_bits[ (int)((__cry & 0x000000F0UL) >> 4) ] +
        bc_bits[ (int)((__cry & 0x00000F00UL) >> 8) ] +
        bc_bits[ (int)((__cry & 0x0000F000UL) >> 12)] +
        bc_bits[ (int)((__cry & 0x000F0000UL) >> 16)] +
        bc_bits[ (int)((__cry & 0x00F00000UL) >> 20)] +
        bc_bits[ (int)((__cry & 0x0F000000UL) >> 24)] +
        bc_bits[ (int)((__cry & 0xF0000000UL) >> 28)] ;

//uint32_t
tmp_seed = _v_seed_priv;
_v_seed_priv = tmp_seed + 13;
_v_iter_priv++;
if (_v_iter_priv < BITCOUNT_ITER) {
    //3    
    __PRE_COMMIT(&_v_n_3_priv, &_v_n_3, sizeof(_v_n_3));
 /*   __PRE_COMMIT(&_v_n_4_priv, &_v_n_4, sizeof(_v_n_4));
    __PRE_COMMIT(&_v_n_5_priv, &_v_n_5, sizeof(_v_n_5));
    __PRE_COMMIT(&_v_n_6_priv, &_v_n_6, sizeof(_v_n_6));
    __PRE_COMMIT(&_v_func_priv, &_v_func, sizeof(_v_func));*/
    __PRE_COMMIT(&_v_seed_priv, &_v_seed, sizeof(_v_seed));
    __PRE_COMMIT(&_v_iter_priv, &_v_iter, sizeof(_v_iter));
    __TRANSITION_TO(5, NTBL1);
}
else {
    //3    

    __PRE_COMMIT(&_v_n_3_priv, &_v_n_3, sizeof(_v_n_3));
    __PRE_COMMIT(&_v_seed_priv, &_v_seed, sizeof(_v_seed));
    __PRE_COMMIT(&_v_iter_priv, &_v_iter, sizeof(_v_iter));
    __TRANSITION_TO(1,Func_Select);
}


__TASK(6, BW_BTBL);
// 1.
_v_n_4_priv = __GET(_v_n_4);
//_v_n_5_priv = __GET(_v_n_5);
//_v_n_6_priv = __GET(_v_n_6);
//_v_func_priv = __GET(_v_func);
_v_seed_priv = __GET(_v_seed);
_v_iter_priv = __GET(_v_iter);

union {
   unsigned char ch[4];
   long y;
} U;

U.y = _v_seed_priv;
_v_n_4_priv += bc_bits[ U.ch[0] ] + bc_bits[ U.ch[1] ] +
        bc_bits[ U.ch[3] ] + bc_bits[ U.ch[2] ];

//uint32_t
tmp_seed = _v_seed_priv;
_v_seed_priv = tmp_seed + 13;
_v_iter_priv++;
if (_v_iter_priv < BITCOUNT_ITER) {
    //3    
    __PRE_COMMIT(&_v_n_4_priv, &_v_n_4, sizeof(_v_n_4));
    //__PRE_COMMIT(&_v_n_5_priv, &_v_n_5, sizeof(_v_n_5));
    //__PRE_COMMIT(&_v_n_6_priv, &_v_n_6, sizeof(_v_n_6));
    //__PRE_COMMIT(&_v_func_priv, &_v_func, sizeof(_v_func));
    __PRE_COMMIT(&_v_seed_priv, &_v_seed, sizeof(_v_seed));
    __PRE_COMMIT(&_v_iter_priv, &_v_iter, sizeof(_v_iter));
    __TRANSITION_TO(6,BW_BTBL);
}
else {
    //3    
    __PRE_COMMIT(&_v_n_4_priv, &_v_n_4, sizeof(_v_n_4));
    __PRE_COMMIT(&_v_seed_priv, &_v_seed, sizeof(_v_seed));
    __PRE_COMMIT(&_v_iter_priv, &_v_iter, sizeof(_v_iter));
    __TRANSITION_TO(1,Func_Select);
}


__TASK(7, AR_BTBL);
// 1.
_v_n_5_priv = __GET(_v_n_5);
//    _v_n_6_priv = __GET(_v_n_6);
//_v_func_priv = __GET(_v_func);
_v_seed_priv = __GET(_v_seed);
_v_iter_priv = __GET(_v_iter);

unsigned char * Ptr = (unsigned char *) &_v_seed_priv;
int Accu  = bc_bits[ *Ptr++ ];
Accu += bc_bits[ *Ptr++ ];
Accu += bc_bits[ *Ptr++ ];
Accu += bc_bits[ *Ptr ];
_v_n_5_priv += Accu;

//uint32_t
tmp_seed = __GET(_v_seed);
_v_seed_priv = tmp_seed + 13;
_v_iter_priv++;
if (_v_iter_priv < BITCOUNT_ITER) {
    //3    
    
    __PRE_COMMIT(&_v_n_5_priv, &_v_n_5, sizeof(_v_n_5));
    //__PRE_COMMIT(&_v_n_6_priv, &_v_n_6, sizeof(_v_n_6));
    //__PRE_COMMIT(&_v_func_priv, &_v_func, sizeof(_v_func));
    __PRE_COMMIT(&_v_seed_priv, &_v_seed, sizeof(_v_seed));
    __PRE_COMMIT(&_v_iter_priv, &_v_iter, sizeof(_v_iter));
    __TRANSITION_TO(7, AR_BTBL);
}
else {
    //3    

    __PRE_COMMIT(&_v_n_5_priv, &_v_n_5, sizeof(_v_n_5));
    __PRE_COMMIT(&_v_seed_priv, &_v_seed, sizeof(_v_seed));
    __PRE_COMMIT(&_v_iter_priv, &_v_iter, sizeof(_v_iter));
    __TRANSITION_TO(1,Func_Select);
}


__TASK(8, Bit_Shifter);
// 1.
_v_n_6_priv = __GET(_v_n_6);
//_v_func_priv = __GET(_v_func);
_v_seed_priv = __GET(_v_seed);
_v_iter_priv = __GET(_v_iter);



//uint32_t
tmp_seed = _v_seed_priv;
unsigned i, nn;
for (i = nn = 0;
       tmp_seed && (i < (sizeof(long) * BITCOUNT_CHARBIT));
       ++i, tmp_seed >>= 1)
{
   nn += (unsigned)(tmp_seed & 1L);
}

_v_n_6_priv += nn;

tmp_seed = __GET(_v_seed);
_v_seed_priv = tmp_seed + 13;
_v_iter_priv++;
if (_v_iter_priv < BITCOUNT_ITER) {
    //3    

    __PRE_COMMIT(&_v_n_6_priv, &_v_n_6, sizeof(_v_n_6));
    //__PRE_COMMIT(&_v_func_priv, &_v_func, sizeof(_v_func));
    __PRE_COMMIT(&_v_seed_priv, &_v_seed, sizeof(_v_seed));
    __PRE_COMMIT(&_v_iter_priv, &_v_iter, sizeof(_v_iter));
    __TRANSITION_TO(8, Bit_Shifter);
}
else {
    //3    

    __PRE_COMMIT(&_v_n_6_priv, &_v_n_6, sizeof(_v_n_6));
    __PRE_COMMIT(&_v_seed_priv, &_v_seed, sizeof(_v_seed));
    __PRE_COMMIT(&_v_iter_priv, &_v_iter, sizeof(_v_iter));
    __TRANSITION_TO(1,Func_Select);
}


} //main
