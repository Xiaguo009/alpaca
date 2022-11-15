#include <testbench/alpaca.h>
#include <testbench/global_declaration.h>
#include <testbench/testbench_api.h>

// #define MSG "hello"
// #define MSG_LEN 5

// char * msgPt = MSG;

//static __nv uint16_t  status = 0;  //cur_task->id
//for test
static __nv uint16_t  status = 0;  //task_id
//count for current bench
static __nv uint16_t bench_task_count = 0; //total execution times for all tasks in a bench
static __nv uint16_t bench_commit = 0; //total pre_commit times in a bench
//count for task[i]
static const uint8_t TASK_NUM = RSA_TASK_NUM;
static __nv uint16_t task_count[TASK_NUM] = {0}; //total execution times for task[i]
static __nv uint16_t task_commit[TASK_NUM] = {0}; //total pre_commit times for all execution times of task[i]


__GLOBAL_SCALAR(long int, p);         //1
__GLOBAL_SCALAR(long int, q);
__GLOBAL_SCALAR(long int, n);
__GLOBAL_SCALAR(long int, t);         //4
__GLOBAL_SCALAR(long int, k);
__GLOBAL_SCALAR(long int, j); //8
__GLOBAL_SCALAR(long int, i);         //
__GLOBAL_SCALAR(long int, flag);      //

__GLOBAL_ARRAY(long int, e, 10);     //18
__GLOBAL_ARRAY(long int, d, 10);     //10  
__GLOBAL_ARRAY(long int, m, 10);     //38
__GLOBAL_ARRAY(long int, temp, 10);  //48
__GLOBAL_ARRAY(long int, en, 10);    //13  

__GLOBAL_SCALAR(long int, en_pt);
__GLOBAL_SCALAR(long int, en_ct);
__GLOBAL_SCALAR(long int, en_key);    //52
__GLOBAL_SCALAR(long int, en_k);  //53
__GLOBAL_SCALAR(long int, en_cnt);
__GLOBAL_SCALAR(long int, en_j);      
__GLOBAL_SCALAR(long int, de_pt);
__GLOBAL_SCALAR(long int, de_ct);
__GLOBAL_SCALAR(long int, de_key);    //58
__GLOBAL_SCALAR(long int, de_k);  //59
__GLOBAL_SCALAR(long int, de_cnt);
__GLOBAL_SCALAR(long int, de_j);      


// scalar: declaration of the buffer 
static __nv long int i_priv;
static __nv long int k_priv;

static __nv long int en_k_priv;
static __nv long int en_cnt_priv;
static __nv long int en_j_priv;

static __nv long int de_k_priv;
static __nv long int de_cnt_priv;
static __nv long int de_j_priv;


void alpaca_rsa_main()
{

 switch(__GET_CURTASK) {
    case 0: goto initTask; //     
    case 1: goto ce_1; //         
    case 2: goto ce_2; //              
    case 3: goto is_i_prime; //    //3
    case 4: goto ce_3; //          //4
    case 5: goto cd; //            //5
    case 6: goto ce_4; //          //6     
    case 7: goto encrypt_init; //  //7
    case 8: goto encrypt_inner_loop;
    case 9: goto encrypt_finish; //     
    //case 10: goto encrypt_print; //     
    case 11: goto decrypt_init; //     
    case 12: goto decrypt_inner_loop; 
    case 13: goto decrypt_finish; //       
    }


__TASK(0, initTask); // 0


    int in_p = 7;
    int in_q = 17;
    int ii = 0;

    __GET(p)= in_p;
    __GET(q)= in_q;
    __GET(n)= in_p * in_q;
    __GET(t)= (in_p-1) * (in_q-1);
    __GET(i)=1;
    __GET(k)=0;
    __GET(flag)=0;
    for (ii = 0; ii < MSG_LEN; ii++)
    {
        __GET(m[ii]) = *(msgPt+ii);
    }


    __TRANSITION_TO(1, ce_1);


__TASK(1, ce_1); // 1
// war i
i_priv = __GET(i);



    __GET(i_priv)++; // start with i=2

    if (__GET(i_priv) >= __GET(t)) {
        __PRE_COMMIT(&i_priv, &i, sizeof(i));
        __TRANSITION_TO(7, encrypt_init);
    } else {
        __PRE_COMMIT(&i_priv, &i, sizeof(i));
        __TRANSITION_TO(2, ce_2);
    }



__TASK(2, ce_2); // 2

    if (__GET(t) % __GET(i) == 0) {
        __TRANSITION_TO(1, ce_1);
    } else {
        __TRANSITION_TO(3, is_i_prime);
    }



__TASK(3, is_i_prime); // 3

    int c;
    c=sqrt16(__GET(i));
    __GET(j) = c;
    for(c=2; c <= __GET( j) ;c++)
    {
        if( __GET(i) % c==0)
        {
            __GET(flag)=0;
            __TRANSITION_TO(1, ce_1);
            //return;
        }
    }
    __GET(flag) = 1;


   __TRANSITION_TO(4, ce_3);



__TASK(4, ce_3); // 4

    long int in_i = __GET(i);
    if( __GET(flag) == 1 && in_i != __GET(p) && in_i != __GET(q) )
    {
        __GET(e[__GET(k)]) = in_i ;
    } else {
        __TRANSITION_TO(1, ce_1);
        //return;
    }

    __TRANSITION_TO(5, cd);




__TASK(5, cd); // 5

    long int kk=1, __cry;
    while(1)
    {
        kk=kk +  __GET(t);
        if(kk % __GET( e[__GET(k)] ) ==0){
            __cry = (kk/ __GET( e[ __GET(k) ]) );
            __GET(flag) = __cry;
            break;
        }
    }

    __TRANSITION_TO(6, ce_4);



__TASK(6, ce_4); // 6
// war k
k_priv = __GET(k);

    //int 
    __cry = __GET(flag);
    if(__cry > 0)
    {
        __GET(d[ __GET(k_priv) ]) =__cry;
        __GET(k_priv)++;
    }

    if (__GET(k_priv) < 9) {
        __PRE_COMMIT(&k_priv, &k, sizeof(k));
        __TRANSITION_TO(1, ce_1);
    } else {
        __PRE_COMMIT(&k_priv, &k, sizeof(k));
        __TRANSITION_TO(7, encrypt_init);
    }


__TASK(7, encrypt_init); // 7

  //long int __cry;
   __cry = __GET(m[ __GET(en_cnt) ]) ;
   __GET(en_pt) = __cry;
   __GET(en_pt) -=96;
   __GET(en_k)  = 1;
   __GET(en_j)  = 0;
   __cry = __GET(e[0]) ;
   __GET(en_key) = __cry;


    __TRANSITION_TO(8, encrypt_inner_loop);




__TASK(8, encrypt_inner_loop); // 8
// war en_k en_j
en_k_priv = __GET(en_k);
en_j_priv = __GET(en_j);


   //long int __cry;
    if (__GET(en_j_priv) < __GET(en_key)) {
        __cry = __GET(en_k_priv) * __GET(en_pt);
        __GET(en_k_priv) = __cry;
        __cry = __GET(en_k_priv) % __GET(n);
        __GET(en_k_priv) = __cry;
        __GET(en_j_priv)++;
        //
        __PRE_COMMIT(&en_k_priv, &en_k, sizeof(en_k));
        __PRE_COMMIT(&en_j_priv, &en_j, sizeof(en_j));
        __TRANSITION_TO(8, encrypt_inner_loop);
    } else {
        __PRE_COMMIT(&en_k_priv, &en_k, sizeof(en_k));
        __PRE_COMMIT(&en_j_priv, &en_j, sizeof(en_j));
        __TRANSITION_TO(9, encrypt_finish);
    }


__TASK(9, encrypt_finish); // 9
// war en_cnt
en_cnt_priv = __GET(en_cnt);


   //long int __cry;
   __cry = __GET(en_k);
   __GET(temp[ __GET(en_cnt_priv) ]) = __cry;
   __cry = __GET(en_k) + 96;
   __GET(en_ct) = __cry;
   __cry = __GET(en_ct);
   __GET(en[ __GET(en_cnt_priv) ]) = __cry;

    if (__GET(en_cnt_priv) < MSG_LEN) {
        __GET(en_cnt_priv)++;
        //
        __PRE_COMMIT(&en_cnt_priv, &en_cnt, sizeof(en_cnt));
        __TRANSITION_TO(7, encrypt_init);
    } else {
        __GET(en[ __GET(en_cnt_priv) ]) = -1;
        //
        __PRE_COMMIT(&en_cnt_priv, &en_cnt, sizeof(en_cnt));
        __TRANSITION_TO(11, decrypt_init);
    }



__TASK(11, decrypt_init); // 11


   //long int __cry;
   __GET(de_k)  = 1;
   __GET(de_j)  = 0;
   __cry =__GET(d[0]);
   __GET(de_key) = __cry;

    __TRANSITION_TO(12, decrypt_inner_loop);



__TASK(12, decrypt_inner_loop); // 12
// war de_k de_j
de_k_priv = __GET(de_k);
de_j_priv = __GET(de_j);

   //long int __cry;
   __cry =  __GET(temp[ __GET(de_cnt) ]);
   __GET(de_ct) = __cry;

    if( __GET(de_j_priv) < __GET(de_key) )
    {
        __cry = __GET(de_k_priv) * __GET(de_ct);
        __GET(de_k_priv) = __cry;
        __cry = __GET(de_k_priv) % __GET(n);
        __GET(de_k_priv) = __cry;
        __GET(de_j_priv)++;
        //
        __PRE_COMMIT(&de_k_priv, &de_k, sizeof(de_k));
        __PRE_COMMIT(&de_j_priv, &de_j, sizeof(de_j));
        __TRANSITION_TO(12, decrypt_inner_loop);
    } else {
        __PRE_COMMIT(&de_k_priv, &de_k, sizeof(de_k));
        __PRE_COMMIT(&de_j_priv, &de_j, sizeof(de_j));
        __TRANSITION_TO(13, decrypt_finish);
    }





__TASK(13, decrypt_finish); // 13
// war de_cnt
de_cnt_priv = __GET(de_cnt);

//long int de_cnt;

   //long int __cry;
   __cry = __GET(de_k) + 96;
   __GET(de_pt) = __cry;
   __GET(m[ __GET(de_cnt_priv)]) = __cry;

    if (__GET(en[ __GET(de_cnt_priv) ]) != -1) {
        __GET(de_cnt_priv)++;
        //
        __PRE_COMMIT(&de_cnt_priv, &de_cnt, sizeof(de_cnt));
        __TRANSITION_TO(11, decrypt_init);
    } else {
        __PRE_COMMIT(&de_cnt_priv, &de_cnt, sizeof(de_cnt));
        __TASK_DOWN;  //down
    }

}
