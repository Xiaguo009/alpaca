#ifndef ALPACA_H
#define ALPACA_H

#include <stdint.h>

//1.for vbm
//
//cur_version ,for vbm. ++ when reboot and transition_to
extern volatile unsigned _numBoots;
//test and set for wt and rd of war array
#define vbm_test(v) v == _numBoots //cur_version
#define vbm_set(v) v = _numBoots


//2.pre_commit
//
extern uint8_t* data_src[];  //extern from??
extern uint8_t* data_dest[];
extern unsigned data_size[];
extern uint8_t** data_src_base;
extern uint8_t** data_dest_base;
extern unsigned* data_size_base;
extern volatile unsigned num_dirty_gv;
void write_to_gbuf(uint8_t *data_src, uint8_t *data_dest, size_t var_size);


//3.commit --task_prologue?
//
extern volatile uint8_t needCommit;
extern volatile unsigned gv_index;
void commit();


//4. for benchmark
//
#define __COMMIT commit() //inline or call? like cpu_wirte()

#define __TASK(id, name) \
    name:                \
    ++_numBoots;         \
    __COMMIT;   //inline?             
    //status |= 0xF000  //?

#define __TRANSITION_TO(id, name) \
    needCommit = 1;                             \
    status = id;                  \
    goto name

#define __TASK_DOWN return

#define __GET_CURTASK  (status & 0x0FFF) 
// 


#endif // ALPACA_H