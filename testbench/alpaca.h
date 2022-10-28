#ifndef ALPACA_H
#define ALPACA_H

#include <stdint.h>

//1.for vbm
//test and set for wt and rd of war array
#define vbm_test(v) v == _numBoots //cur_version
#define vbm_set(v) v = _numBoots

//cur_version ,for vbm. ++ when reboot and transition_to
extern volatile unsigned _numBoots;

//2.pre_commit

extern uint8_t* data_src[];  //extern from??
extern uint8_t* data_dest[];
extern unsigned data_size[];
extern uint8_t** data_src_base;
extern uint8_t** data_dest_base;
extern unsigned* data_size_base;
extern volatile unsigned num_dirty_gv;
void write_to_gbuf(uint8_t *data_src, uint8_t *data_dest, size_t var_size);


//3.commit --task_prologue?
//needCommit
extern volatile uint8_t needCommit;




#endif // ALPACA_H