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
// //
// extern uint8_t* data_src[];  //extern from??
// extern uint8_t* data_dest[];
// extern unsigned data_size[];
//extern uint16_t** data_src_base;
//extern uint16_t** data_dest_base;
//extern unsigned* data_size_base;
extern unsigned long data_src_base[100];
extern unsigned long data_dest_base[100];
extern unsigned long data_size_base[100];

extern volatile unsigned num_dirty_gv;
void write_to_gbuf(unsigned long data_src, unsigned long data_dest, size_t var_size);
//void write_to_gbuf(uint16_t *data_src, uint16_t *data_dest, size_t var_size);

// template<class T1,class T2>
// void write_to_gbuf(T1 *data_src, T1 *data_dest, T2 var_size)
// {
// 	// save to dirtylist
// 	*(data_size_base + num_dirty_gv) = var_size;
// 	*(data_dest_base + num_dirty_gv) = data_dest;
// 	*(data_src_base + num_dirty_gv) = data_src;
// 	// increment count, len of dirty list
// 	num_dirty_gv++;
// }


//3.commit --task_prologue?
//
extern volatile uint8_t needCommit;
extern volatile unsigned gv_index;
void commit();


//4. for task
//
#define __COMMIT commit() //inline or call? like cpu_wirte()

#define __TASK(id, name) \
    name:                \
    ++_numBoots;         \
    __COMMIT;            \
    ++task_count //for testing
    //printf("id: %d. task count: %d. \n",id,++task_count); //     \


#define __TRANSITION_TO(id, name) \
    needCommit = 1;                             \
    status = id;                  \
    goto name

#define __TASK_DOWN return

#define __GET_CURTASK  (status & 0x0FFF) 
// 

//5. from PC

#define __SHARED1(type, name) static __nv type name
#define __SHARED2(type, name, size) static __nv type name[size]

// SHARED->
#define __GLOBAL_SCALAR(type, name)  static __nv type name
#define __GLOBAL_ARRAY(type, name, size)  static __nv type name[size]


#define __GET(item) item

// #define GV(item) item //for blowfish
// //GV(item,size)? -->__GET(sorted[temp])


#endif // ALPACA_H
