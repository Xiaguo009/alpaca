#ifndef ALPACA_H
#define ALPACA_H

#include <driver/nv.h>
#include <stddef.h>
#include <stdint.h>


//1.for vbm
//
//cur_version ,for vbm. ++ when reboot and transition_to
extern volatile unsigned _numBoots;
//test and set for wt and rd of war array
#define vbm_test(v) v == _numBoots //cur_version
#define vbm_set(v) v = _numBoots


//2.pre_commit
extern unsigned long data_src_base[100];
extern unsigned long data_dest_base[100];
extern unsigned long data_size_base[100];

extern volatile unsigned num_dirty_gv;
void write_to_gbuf(unsigned long data_src, unsigned long data_dest, size_t var_size);

//3.commit --task_prologue
extern volatile uint8_t needCommit;
extern volatile unsigned gv_index;
void commit();


//4. for task
//
#define __COMMIT commit() //call

//for count
#define __PRE_COMMIT(data_src, data_dest, var_size) \
    write_to_gbuf((unsigned long)(data_src), (unsigned long)(data_dest), var_size)

#define __TASK(id, name) \
    name:                \
    ++_numBoots;         \
    __COMMIT


#define __TRANSITION_TO(id, name) \
    needCommit = 1;                             \
    status = id;                  \
    goto name

#define __TASK_DOWN return

#define __GET_CURTASK  (status & 0x0FFF) 


// SHARED->
#define __GLOBAL_SCALAR(type, name)  static __nv type name
#define __GLOBAL_ARRAY(type, name, size)  static __nv type name[size]


#define __GET(item) item


#endif // ALPACA_H
