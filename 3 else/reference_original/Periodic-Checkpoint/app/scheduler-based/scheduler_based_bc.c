
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

__TASK(2, func0,

uint32_t tmp_seed = __GET(_v_seed);
__GET(_v_seed) = tmp_seed + 13;
uint32_t temp = 0;

if (tmp_seed) do
   temp++;
while ( 0 != (tmp_seed = tmp_seed & (tmp_seed - 1)) );

__GET(_v_n_0) += temp;
__GET(_v_iter)++;
if (__GET(_v_iter) < BITCOUNT_ITER)
   return 2;
else
   return 1;
)

__TASK(3, func1,

uint32_t tmp_seed = __GET(_v_seed);
__GET(_v_seed) = tmp_seed + 13;

tmp_seed = ((tmp_seed & 0xAAAAAAAAL) >>  1) + (tmp_seed & 0x55555555L);
tmp_seed = ((tmp_seed & 0xCCCCCCCCL) >>  2) + (tmp_seed & 0x33333333L);
tmp_seed = ((tmp_seed & 0xF0F0F0F0L) >>  4) + (tmp_seed & 0x0F0F0F0FL);
tmp_seed = ((tmp_seed & 0xFF00FF00L) >>  8) + (tmp_seed & 0x00FF00FFL);
tmp_seed = ((tmp_seed & 0xFFFF0000L) >> 16) + (tmp_seed & 0x0000FFFFL);

__GET(_v_n_1) += (int)tmp_seed;
__GET(_v_iter)++;
if (__GET(_v_iter) < BITCOUNT_ITER)
   return 3;
else
   return 1;

)

__TASK(4, NTBL0,

uint32_t tmp_seed = __GET(_v_seed);
__GET(_v_n_2) += Bitcount_NonRecursiveCnt(tmp_seed);
__GET(_v_seed) = tmp_seed + 13;

__GET(_v_iter)++;
if(__GET(_v_iter) < BITCOUNT_ITER)
   return 4;
else
   return 1;
)

__TASK(5, NTBL1,

uint16_t __cry = __GET(_v_seed);

__GET(_v_n_3) += bc_bits[ (int) (__cry & 0x0000000FUL)] +
        bc_bits[ (int)((__cry & 0x000000F0UL) >> 4) ] +
        bc_bits[ (int)((__cry & 0x00000F00UL) >> 8) ] +
        bc_bits[ (int)((__cry & 0x0000F000UL) >> 12)] +
        bc_bits[ (int)((__cry & 0x000F0000UL) >> 16)] +
        bc_bits[ (int)((__cry & 0x00F00000UL) >> 20)] +
        bc_bits[ (int)((__cry & 0x0F000000UL) >> 24)] +
        bc_bits[ (int)((__cry & 0xF0000000UL) >> 28)] ;

uint32_t tmp_seed = __GET(_v_seed);
__GET(_v_seed) = tmp_seed + 13;
__GET(_v_iter)++;
if(__GET(_v_iter) < BITCOUNT_ITER)
   return 5;
else
   return 1;
)

__TASK(6, BW_BTBL,

union {
   unsigned char ch[4];
   long y;
} U;

U.y = __GET(_v_seed);
__GET(_v_n_4) += bc_bits[ U.ch[0] ] + bc_bits[ U.ch[1] ] +
        bc_bits[ U.ch[3] ] + bc_bits[ U.ch[2] ];

uint32_t tmp_seed = __GET(_v_seed);
__GET(_v_seed) = tmp_seed + 13;
__GET(_v_iter)++;
if(__GET(_v_iter) < BITCOUNT_ITER)
   return 6;
else
   return 1;
)

__TASK(7, AR_BTBL,

unsigned char * Ptr = (unsigned char *) &__GET(_v_seed);
int Accu  = bc_bits[ *Ptr++ ];
Accu += bc_bits[ *Ptr++ ];
Accu += bc_bits[ *Ptr++ ];
Accu += bc_bits[ *Ptr ];
__GET(_v_n_5) += Accu;

uint32_t tmp_seed = __GET(_v_seed);
__GET(_v_seed) = tmp_seed + 13;
__GET(_v_iter)++;
if(__GET(_v_iter) < BITCOUNT_ITER)
   return 7;
else
   return 1;
)

__TASK(8, Bit_Shifter,

uint32_t tmp_seed = __GET(_v_seed);
unsigned i, nn;
for (i = nn = 0;
       tmp_seed && (i < (sizeof(long) * BITCOUNT_CHARBIT));
       ++i, tmp_seed >>= 1)
{
   nn += (unsigned)(tmp_seed & 1L);
}

__GET(_v_n_6) += nn;

tmp_seed = __GET(_v_seed);
__GET(_v_seed) = tmp_seed + 13;
__GET(_v_iter)++;
if(__GET(_v_iter) < BITCOUNT_ITER)
   return 8;
else
   return 1;
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
