#include <testbench/alpaca.h>
#include <testbench/global_declaration.h>
#include <testbench/testbench_api.h>
//pre_commit down

typedef uint32_t WORD;

//__GLOBAL_SCALAR or ARRAY
static __nv WORD msg[16];  // message block for 512-bit.

static __nv uint16_t ei, ri;
static __nv WORD msg_exp[64];       // expanded message blocks.
static __nv WORD msg_hash[8];       // hash results.
static __nv WORD round_regs[64][8]; // round registers.

//privatize
static __nv uint16_t ei_priv, ri_priv;

static __nv WORD msg_exp_priv[64];
static __nv WORD msg_exp_vbm[64];
static __nv WORD round_regs_priv[64][8];
static __nv WORD round_regs_vbm[64][8];

static __nv uint16_t  status = 0; 


void alpaca_sha256_main()
{

    switch(__GET_CURTASK) {
        case 0: goto vector_init;
        case 1: goto message_padding;
        case 2: goto message_expansion;
        case 3: goto message_round_init;
        case 4: goto message_round;
        case 5: goto message_round_final;
        }

__TASK(0,vector_init);
    for (uint16_t i = 0; i < 8; ++i){
        __GET(msg_hash[i]) = INIT_VEC[i];
    }

__TASK(1,message_padding);
    // Set message to all 0-bits.
    memset(__GET(msg), 0, sizeof(WORD) * 16);      

    // Copy HASH_MSG to the beginning of message.
    uint16_t msg_len = strlen(HASH_MSG);
    for (uint16_t i = 0; i < msg_len; ++i){
        ((uint8_t*)__GET(msg))[i] = HASH_MSG[i];
    }

    // Pad 1 in the following bit.
    uint16_t word_index = msg_len / 4;
    uint16_t bit_index = 8 * (msg_len % 4);
    BIT_SET(__GET(msg[word_index]), bit_index);
    // The 0 is automatically padded at first.
    // Pad len of the message.
    __GET(msg[14]) = 0;
    __GET(msg[15]) = (WORD)(8 * msg_len);

    __GET(ei) = 0;
    __TRANSITION_TO(2,message_expansion);


__TASK(2,message_expansion);
    //array msg_exp 
    //scalar ei
    ei_priv = __GET(ei);

    if (ei_priv < 16) {

        __GET(msg_exp_priv[ei_priv]) = __GET(msg[ei_priv]);  //wt
        
        //after wt
        if (!vbm_test(msg_exp_vbm[ei_priv])) {
            vbm_set(msg_exp_vbm[ei_priv]);
            __PRE_COMMIT(&(msg_exp_priv[ei_priv]), &msg_exp[ei_priv], sizeof(msg_exp[ei_priv]));
        }

    } else {

        if (!vbm_test(msg_exp_vbm[ei_priv-2])) {
            msg_exp_priv[ei_priv-2] = __GET(msg_exp[ei_priv-2]);
        }
        if (!vbm_test(msg_exp_vbm[ei_priv-7])) {
            msg_exp_priv[ei_priv-7] = __GET(msg_exp[ei_priv-7]);
        }
        if (!vbm_test(msg_exp_vbm[ei_priv-15])) {
            msg_exp_priv[ei_priv-15] = __GET(msg_exp[ei_priv-15]);
        }
        if (!vbm_test(msg_exp_vbm[ei_priv-17])) {
            msg_exp_priv[ei_priv-17] = __GET(msg_exp[ei_priv-17]);
        }
        
        //error
        __GET(msg_exp_priv[ei_priv]) = SIG1(__GET(msg_exp_priv[ei_priv - 2]))
                                    + __GET(msg_exp_priv[ei_priv - 7]) 
                                    + SIG0(__GET(msg_exp_priv[ei_priv - 15]))
                                    + SIG0(__GET(msg_exp_priv[ei_priv - 16]));  //rd and wt

        //after wt
        if (!vbm_test(msg_exp_vbm[ei_priv])) {
            vbm_set(msg_exp_vbm[ei_priv]);
            __PRE_COMMIT(&(msg_exp_priv[ei_priv]), &msg_exp[ei_priv], sizeof(msg_exp[ei_priv]));
        }                         
    }

    ei_priv++;
    if (ei_priv >= 64){
        __PRE_COMMIT(&ei_priv, &ei, sizeof(ei));
        __TRANSITION_TO(4, message_round);
    }
    else
    {
        __PRE_COMMIT(&ei_priv, &ei, sizeof(ei));
        __TRANSITION_TO(3,message_round_init);
    }

__TASK(3,message_round_init);
    for (uint16_t i = 0; i < 8; ++i) {
        __GET(round_regs[0][i]) = __GET(msg_hash[i]);
    }
    __GET(ri) = 0;
    __TRANSITION_TO(4,message_round);


__TASK(4,message_round);
    //array round_regs
    //scalar ri
    ri_priv = __GET(ri);

    //before rd1
    if (!vbm_test(round_regs_vbm[ri_priv][7])) {
        round_regs_priv[ri_priv][7] = __GET(round_regs[ri_priv][7]);
    }    
    if (!vbm_test(round_regs_vbm[ri_priv][4])) {
        round_regs_priv[ri_priv][4] = __GET(round_regs[ri_priv][4]);
    }     
    if (!vbm_test(round_regs_vbm[ri_priv][4])) {
        round_regs_priv[ri_priv][4] = __GET(round_regs[ri_priv][4]);
    } 
    if (!vbm_test(round_regs_vbm[ri_priv][6])) {
        round_regs_priv[ri_priv][6] = __GET(round_regs[ri_priv][6]);
    } 
    if (!vbm_test(round_regs_vbm[ri_priv][7])) {
        round_regs_priv[ri_priv][7] = __GET(round_regs[ri_priv][7]);
    } 
    //rd1
    WORD t1 = __GET(round_regs[ri_priv][7]) 
                + BIGSIG1(__GET(round_regs[ri_priv][4]))
                + CH(__GET(round_regs[ri_priv][4]), 
                        __GET(round_regs[ri_priv][5]), 
                        __GET(round_regs[ri_priv][6])) 
                + RCST[ri_priv] 
                + __GET(msg_exp[ri_priv]);

    //before rd2
    if (!vbm_test(round_regs_vbm[ri_priv][0])) {
        round_regs_priv[ri_priv][0] = __GET(round_regs[ri_priv][0]);
    } 
        //before rd1
    if (!vbm_test(round_regs_vbm[ri_priv][0])) {
        round_regs_priv[ri_priv][0] = __GET(round_regs[ri_priv][0]);
    } 
        //before rd1
    if (!vbm_test(round_regs_vbm[ri_priv][1])) {
        round_regs_priv[ri_priv][1] = __GET(round_regs[ri_priv][1]);
    } 
        //before rd1
    if (!vbm_test(round_regs_vbm[ri_priv][2])) {
        round_regs_priv[ri_priv][2] = __GET(round_regs[ri_priv][2]);
    } 
    //rd2
    WORD t2 = BIGSIG0(__GET(round_regs[ri_priv][0])) 
                + MA(__GET(round_regs[ri_priv][0]), 
                        __GET(round_regs[ri_priv][1]), 
                        __GET(round_regs[ri_priv][2]));

    //before rd3
    if (!vbm_test(round_regs_vbm[ri_priv][6])) {
        round_regs_priv[ri_priv][6] = __GET(round_regs[ri_priv][6]);
    }     
    if (!vbm_test(round_regs_vbm[ri_priv][5])) {
        round_regs_priv[ri_priv][5] = __GET(round_regs[ri_priv][5]);
    }  
    if (!vbm_test(round_regs_vbm[ri_priv][4])) {
        round_regs_priv[ri_priv][4] = __GET(round_regs[ri_priv][4]);
    }  
    if (!vbm_test(round_regs_vbm[ri_priv][3])) {
        round_regs_priv[ri_priv][3] = __GET(round_regs[ri_priv][3]);
    }  
    if (!vbm_test(round_regs_vbm[ri_priv][2])) {
        round_regs_priv[ri_priv][2] = __GET(round_regs[ri_priv][2]);
    }  
    if (!vbm_test(round_regs_vbm[ri_priv][1])) {
        round_regs_priv[ri_priv][1] = __GET(round_regs[ri_priv][1]);
    }  
    if (!vbm_test(round_regs_vbm[ri_priv][0])) {
        round_regs_priv[ri_priv][0] = __GET(round_regs[ri_priv][0]);
    }  
    //rd3 and wt1
    __GET(round_regs[ri_priv][7]) = __GET(round_regs[ri_priv][6]);
    __GET(round_regs[ri_priv][6]) = __GET(round_regs[ri_priv][5]);
    __GET(round_regs[ri_priv][5]) = __GET(round_regs[ri_priv][4]);
    __GET(round_regs[ri_priv][4]) = __GET(round_regs[ri_priv][3]) + t1;
    __GET(round_regs[ri_priv][3]) = __GET(round_regs[ri_priv][2]);
    __GET(round_regs[ri_priv][2]) = __GET(round_regs[ri_priv][1]);
    __GET(round_regs[ri_priv][1]) = __GET(round_regs[ri_priv][0]);
    __GET(round_regs[ri_priv][0]) = t1 + t2;

    //after wt1
    if (!vbm_test(round_regs_vbm[ri_priv][7])) {
        vbm_set(round_regs_vbm[ri_priv][7]);
        __PRE_COMMIT(&round_regs_priv[ri_priv][7], &round_regs[ri_priv][7], sizeof(round_regs[ri_priv][7]));
    }  
    if (!vbm_test(round_regs_vbm[ri_priv][6])) {
        vbm_set(round_regs_vbm[ri_priv][6]);
        __PRE_COMMIT(&(round_regs_priv[ri_priv][6]), &round_regs[ri_priv][6], sizeof(round_regs[ri_priv][6]));
    }  
    if (!vbm_test(round_regs_vbm[ri_priv][5])) {
        vbm_set(round_regs_vbm[ri_priv][5]);
        __PRE_COMMIT(&(round_regs_priv[ri_priv][5]), &round_regs[ri_priv][5], sizeof(round_regs[ri_priv][5]));
    }  
    if (!vbm_test(round_regs_vbm[ri_priv][4])) {
        vbm_set(round_regs_vbm[ri_priv][4]);
        __PRE_COMMIT(&(round_regs_priv[ri_priv][4]), &round_regs[ri_priv][4], sizeof(round_regs[ri_priv][4]));
    }   
    if (!vbm_test(round_regs_vbm[ri_priv][3])) {
        vbm_set(round_regs_vbm[ri_priv][3]);
        __PRE_COMMIT(&(round_regs_priv[ri_priv][3]), &round_regs[ri_priv][3], sizeof(round_regs[ri_priv][3]));
    }  
    if (!vbm_test(round_regs_vbm[ri_priv][2])) {
        vbm_set(round_regs_vbm[ri_priv][2]);
        __PRE_COMMIT(&(round_regs_priv[ri_priv][2]), &round_regs[ri_priv][2], sizeof(round_regs[ri_priv][2]));
    }  
    if (!vbm_test(round_regs_vbm[ri_priv][1])) {
        vbm_set(round_regs_vbm[ri_priv][1]);
        __PRE_COMMIT(&(round_regs_priv[ri_priv][1]), &round_regs[ri_priv][1], sizeof(round_regs[ri_priv][1]));
    }  
    if (!vbm_test(round_regs_vbm[ri_priv][0])) {
        vbm_set(round_regs_vbm[ri_priv][0]);
        __PRE_COMMIT(&(round_regs_priv[ri_priv][0]), &round_regs[ri_priv][0], sizeof(round_regs[ri_priv][0]));
    }     

    ri_priv++;
    if (ri_priv >= 64) {
        __PRE_COMMIT(&ri_priv, &ri, sizeof(ri));
        __TRANSITION_TO(5,message_round_final);
    }
    else {
        __PRE_COMMIT(&ri_priv, &ri, sizeof(ri));
        __TRANSITION_TO(4,message_round);
    }

__TASK(5,message_round_final);
    for (uint16_t i = 0; i < 8; ++i) {
        __GET(msg_hash[i]) = __GET(round_regs[63][i]);
    }
    __TASK_DOWN;

}
