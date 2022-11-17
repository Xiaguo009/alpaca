
#include <driver/nv.h>
#include <testbench/alpaca.h>
#include <driver/driverlib_include.h>
#include <stddef.h>

//1 for vbm
//
//cur_version++, when reboot and transition_to
__nv volatile unsigned _numBoots = 0;


//3 for commit
//
// var for dirtylist commit iteration
__nv volatile unsigned gv_index=0;

//len of dirtylist  --pre_commit-->dirtylist
__nv volatile unsigned num_dirty_gv=0;

//+  needCommit
__nv volatile uint8_t needCommit=0;


__nv unsigned long data_src_base[100];
__nv unsigned long data_dest_base[100];
__nv unsigned long data_size_base[100];

//2 pre_commit
//
//data_src=&war_priv, data_dest=&war, var_size=sizeof(war)
void write_to_gbuf(unsigned long data_src, unsigned long data_dest, size_t var_size)
{
    data_size_base[num_dirty_gv] = var_size;
    data_dest_base[num_dirty_gv] = data_dest;
    data_src_base[num_dirty_gv] = data_src;

	num_dirty_gv++;
}


// 3 cur_version++ and commit  --> inline
//
void commit(){
	if (needCommit == 1)
	{
		while (gv_index < num_dirty_gv)
		{
		    unsigned long w_data_dest = data_dest_base[gv_index];
		    unsigned long w_data_src = data_src_base[gv_index];
		    unsigned w_data_size = data_size_base[gv_index];

			memcpy((void *)w_data_dest, (void *)w_data_src, w_data_size);
			++gv_index;
		}
		num_dirty_gv = 0;
		gv_index = 0;
		// commit bit = 0
		needCommit = 0;
	}
	else {
		num_dirty_gv=0;
	}
}


