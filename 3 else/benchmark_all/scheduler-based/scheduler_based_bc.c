
#include <app/app_api.h>
#include <app/app_global.h>
#include <scheduling/scheduler.h>
#include <scheduling/scheduler_based_task.h>

__SHARED_VAR(
uint32_t _v_seed;
uint16_t _v_iter;
uint16_t _v_func;
uint16_t _v_n_0;
uint16_t _v_n_1;
uint16_t _v_n_2;
uint16_t _v_n_3;
uint16_t _v_n_4;
uint16_t _v_n_5;
uint16_t _v_n_6;
)

//0.declaration
__nv uint16_t _v_n_0_priv;
__nv uint16_t _v_n_1_priv;
__nv uint16_t _v_n_2_priv;
__nv uint16_t _v_n_3_priv;
__nv uint16_t _v_n_4_priv;
__nv uint16_t _v_n_5_priv;
__nv uint16_t _v_n_6_priv;
//__nv uint16_t _v_func_priv;
__nv uint32_t _v_seed_priv;
__nv uint16_t _v_iter_priv;

__TASK_ENTRY(init,

__GET(_v_func) = 0;
__GET(_v_n_0) = 0;
__GET(_v_n_1) = 0;
__GET(_v_n_2) = 0;
__GET(_v_n_3) = 0;
__GET(_v_n_4) = 0;
__GET(_v_n_5) = 0;
__GET(_v_n_6) = 0;

return 1;

)


__TASK(1, Func_Select,

    //1.
    _v_func_priv = __GET(_v_func)


__GET(_v_seed) = (uint32_t)BITCOUNT_SEED;
__GET(_v_iter) = 0;
switch(__GET(_v_func)++)
{
case 0:     return 2;
case 1:     return 3;
case 2:     return 4;
case 3:     return 5;
case 4:     return 6;
case 5:     return 7;
case 6:     return 8;
default:    return TASK_FINISH;
}

)

__TASK(2, func0,//bit_count
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
    write_to_gbuf(&_v_n_0_priv, &_v_n_0, sizeof(_v_n_0));
    //write_to_gbuf(&_v_n_1_priv, &_v_n_1, sizeof(_v_n_1));
    //write_to_gbuf(&_v_n_2_priv, &_v_n_2, sizeof(_v_n_2));
    //write_to_gbuf(&_v_n_3_priv, &_v_n_3, sizeof(_v_n_3));
    //write_to_gbuf(&_v_n_4_priv, &_v_n_4, sizeof(_v_n_4));
    //write_to_gbuf(&_v_n_5_priv, &_v_n_5, sizeof(_v_n_5));
    //write_to_gbuf(&_v_n_6_priv, &_v_n_6, sizeof(_v_n_6));
    //write_to_gbuf(&_v_func_priv, &_v_func, sizeof(_v_func));
    write_to_gbuf(&_v_seed_priv, &_v_seed, sizeof(_v_seed));
    write_to_gbuf(&_v_iter_priv, &_v_iter, sizeof(_v_iter));
    return 2;
}
else {
    //3
    write_to_gbuf(&_v_n_0_priv, &_v_n_0, sizeof(_v_n_0));
    write_to_gbuf(&_v_seed_priv, &_v_seed, sizeof(_v_seed));
    write_to_gbuf(&_v_iter_priv, &_v_iter, sizeof(_v_iter));
    return 1;
}
)

__TASK(3, func1,
    //1.
    _v_n_1_priv = __GET(_v_n_1);//
//_v_n_2_priv = __GET(_v_n_2);
//_v_n_3_priv = __GET(_v_n_3);
//_v_n_4_priv = __GET(_v_n_4);
//_v_n_5_priv = __GET(_v_n_5);
//_v_n_6_priv = __GET(_v_n_6);
//_v_func_priv = __GET(_v_func);
_v_seed_priv = __GET(_v_seed);//
_v_iter_priv = __GET(_v_iter);//

uint32_t tmp_seed = _v_seed_priv;
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
    write_to_gbuf(&_v_n_1_priv, &_v_n_1, sizeof(_v_n_1));
    //write_to_gbuf(&_v_n_2_priv, &_v_n_2, sizeof(_v_n_2));
    //write_to_gbuf(&_v_n_3_priv, &_v_n_3, sizeof(_v_n_3));
    //write_to_gbuf(&_v_n_4_priv, &_v_n_4, sizeof(_v_n_4));
    //write_to_gbuf(&_v_n_5_priv, &_v_n_5, sizeof(_v_n_5));
    //write_to_gbuf(&_v_n_6_priv, &_v_n_6, sizeof(_v_n_6));
    //write_to_gbuf(&_v_func_priv, &_v_func, sizeof(_v_func));
    write_to_gbuf(&_v_seed_priv, &_v_seed, sizeof(_v_seed));
    write_to_gbuf(&_v_iter_priv, &_v_iter, sizeof(_v_iter));
    return 3;
}
else {
    //3    
    write_to_gbuf(&_v_n_1_priv, &_v_n_1, sizeof(_v_n_1));
    write_to_gbuf(&_v_seed_priv, &_v_seed, sizeof(_v_seed));
    write_to_gbuf(&_v_iter_priv, &_v_iter, sizeof(_v_iter));
    return 1;
}

)

__TASK(4, NTBL0,
    //1.
    _v_n_2_priv = __GET(_v_n_2);//
//_v_n_3_priv = __GET(_v_n_3);
//_v_n_4_priv = __GET(_v_n_4);
//_v_n_5_priv = __GET(_v_n_5);
//_v_n_6_priv = __GET(_v_n_6);
//_v_func_priv = __GET(_v_func);
_v_seed_priv = __GET(_v_seed);//
_v_iter_priv = __GET(_v_iter);

uint32_t tmp_seed = _v_seed_priv;
_v_n_2_priv += Bitcount_NonRecursiveCnt(tmp_seed);
_v_seed_priv = tmp_seed + 13;

_v_iter_priv++;
if (_v_iter_priv < BITCOUNT_ITER) {
    //3    

    write_to_gbuf(&_v_n_2_priv, &_v_n_2, sizeof(_v_n_2));
 /*   write_to_gbuf(&_v_n_3_priv, &_v_n_3, sizeof(_v_n_3));
    write_to_gbuf(&_v_n_4_priv, &_v_n_4, sizeof(_v_n_4));
    write_to_gbuf(&_v_n_5_priv, &_v_n_5, sizeof(_v_n_5));
    write_to_gbuf(&_v_n_6_priv, &_v_n_6, sizeof(_v_n_6));
    write_to_gbuf(&_v_func_priv, &_v_func, sizeof(_v_func));*/
    write_to_gbuf(&_v_seed_priv, &_v_seed, sizeof(_v_seed));
    write_to_gbuf(&_v_iter_priv, &_v_iter, sizeof(_v_iter));
    return 4;
}
else {
    //3    

    write_to_gbuf(&_v_n_2_priv, &_v_n_2, sizeof(_v_n_2));
    write_to_gbuf(&_v_seed_priv, &_v_seed, sizeof(_v_seed));
    write_to_gbuf(&_v_iter_priv, &_v_iter, sizeof(_v_iter));
    return 1;
}
)

__TASK(5, NTBL1,
    //1.
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

uint32_t tmp_seed = _v_seed_priv;
_v_seed_priv = tmp_seed + 13;
_v_iter_priv++;
if (_v_iter_priv < BITCOUNT_ITER) {
    //3    
    write_to_gbuf(&_v_n_3_priv, &_v_n_3, sizeof(_v_n_3));
 /*   write_to_gbuf(&_v_n_4_priv, &_v_n_4, sizeof(_v_n_4));
    write_to_gbuf(&_v_n_5_priv, &_v_n_5, sizeof(_v_n_5));
    write_to_gbuf(&_v_n_6_priv, &_v_n_6, sizeof(_v_n_6));
    write_to_gbuf(&_v_func_priv, &_v_func, sizeof(_v_func));*/
    write_to_gbuf(&_v_seed_priv, &_v_seed, sizeof(_v_seed));
    write_to_gbuf(&_v_iter_priv, &_v_iter, sizeof(_v_iter));
    return 5;
}
else {
    //3    

    write_to_gbuf(&_v_n_3_priv, &_v_n_3, sizeof(_v_n_3));
    write_to_gbuf(&_v_seed_priv, &_v_seed, sizeof(_v_seed));
    write_to_gbuf(&_v_iter_priv, &_v_iter, sizeof(_v_iter));
    return 1;
}
)

__TASK(6, BW_BTBL,
//1.
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

uint32_t tmp_seed = _v_seed_priv;
_v_seed_priv = tmp_seed + 13;
_v_iter_priv++;
if (_v_iter_priv < BITCOUNT_ITER) {
    //3    
    write_to_gbuf(&_v_n_4_priv, &_v_n_4, sizeof(_v_n_4));
    //write_to_gbuf(&_v_n_5_priv, &_v_n_5, sizeof(_v_n_5));
    //write_to_gbuf(&_v_n_6_priv, &_v_n_6, sizeof(_v_n_6));
    //write_to_gbuf(&_v_func_priv, &_v_func, sizeof(_v_func));
    write_to_gbuf(&_v_seed_priv, &_v_seed, sizeof(_v_seed));
    write_to_gbuf(&_v_iter_priv, &_v_iter, sizeof(_v_iter));
    return 6;
}
else {
    //3    
    write_to_gbuf(&_v_n_4_priv, &_v_n_4, sizeof(_v_n_4));
    write_to_gbuf(&_v_seed_priv, &_v_seed, sizeof(_v_seed));
    write_to_gbuf(&_v_iter_priv, &_v_iter, sizeof(_v_iter));
    return 1;
}
)

__TASK(7, AR_BTBL,
    //1.
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

uint32_t tmp_seed = __GET(_v_seed);
_v_seed_priv = tmp_seed + 13;
_v_iter_priv++;
if (_v_iter_priv < BITCOUNT_ITER) {
    //3    
    
    write_to_gbuf(&_v_n_5_priv, &_v_n_5, sizeof(_v_n_5));
    //write_to_gbuf(&_v_n_6_priv, &_v_n_6, sizeof(_v_n_6));
    //write_to_gbuf(&_v_func_priv, &_v_func, sizeof(_v_func));
    write_to_gbuf(&_v_seed_priv, &_v_seed, sizeof(_v_seed));
    write_to_gbuf(&_v_iter_priv, &_v_iter, sizeof(_v_iter));
    return 7;
}
else {
    //3    

    write_to_gbuf(&_v_n_5_priv, &_v_n_5, sizeof(_v_n_5));
    write_to_gbuf(&_v_seed_priv, &_v_seed, sizeof(_v_seed));
    write_to_gbuf(&_v_iter_priv, &_v_iter, sizeof(_v_iter));
    return 1;
}
)

__TASK(8, Bit_Shifter,
    //1.
    _v_n_6_priv = __GET(_v_n_6);
    //_v_func_priv = __GET(_v_func);
_v_seed_priv = __GET(_v_seed);
_v_iter_priv = __GET(_v_iter);



uint32_t tmp_seed = _v_seed_priv;
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

    write_to_gbuf(&_v_n_6_priv, &_v_n_6, sizeof(_v_n_6));
    //write_to_gbuf(&_v_func_priv, &_v_func, sizeof(_v_func));
    write_to_gbuf(&_v_seed_priv, &_v_seed, sizeof(_v_seed));
    write_to_gbuf(&_v_iter_priv, &_v_iter, sizeof(_v_iter));
    return 8;
}
else {
    //3    

    write_to_gbuf(&_v_n_6_priv, &_v_n_6, sizeof(_v_n_6));
    write_to_gbuf(&_v_seed_priv, &_v_seed, sizeof(_v_seed));
    write_to_gbuf(&_v_iter_priv, &_v_iter, sizeof(_v_iter));
    return 1;
}
)


void bc_regist()
{
    task_regist(0, init,            false);
    task_regist(1, Func_Select,     true );
    task_regist(2, func0,           true );
    task_regist(3, func1,           true );
    task_regist(4, NTBL0,           true );
    task_regist(5, NTBL1,           true );
    task_regist(6, BW_BTBL,         true );
    task_regist(7, AR_BTBL,         true );
    task_regist(8, Bit_Shifter,     true );

    WAR_REGIST(11);
}
