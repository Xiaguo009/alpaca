
#include <testbench/global_declaration.h>
#include <testbench/Periodic_scheduler.h>
#include <testbench/testbench_api.h>

// #define MSG "hello"
// #define MSG_LEN 5

// char * msgPt = MSG;

__SHARED_VAR(
    long int i;         //uint32_t
    long int k;

    long int en_k;  //53
    long int en_cnt;
    long int en_j;  

    long int de_k;  //59
    long int de_cnt;
    long int de_j;   

    long int p;         //1
    long int q;
    long int n;
    long int t;         //4
    
    long int j; //8
    
    long int flag;      //
    long int e[10];     //18
    long int d[10];     //10  
    long int m[10];     //38
    long int temp[10];  //48
    long int en[10];    //13  
    long int en_pt;
    long int en_ct;
    long int en_key;    //52
    
    long int de_pt;
    long int de_ct;
    long int de_key;    //58
  
)

static __nv bool      first_run = 1;
static __nv uint16_t  status = 0;

static const uint16_t global_war_size = 16;
static __nv uint16_t  backup_buf[16] = {};

//1 6 8 9 12 13 [0-13]
static const bool backup_needed[] = {
    false, true, false, false, false, false, true, false, true,true,
    //0
    false,false,true,true
    //10
};

//for test
static __nv uint16_t task_id = 0;  //task_id
//count for current bench
static __nv uint16_t bench_task_count = 0; //total execution times for all tasks in a bench
static __nv uint32_t bench_commit = 0; //total pre_commit size in a bench
//count for task[i]
static const uint8_t TASK_NUM = RSA_TASK_NUM;
static __nv uint16_t task_count[TASK_NUM] = {0}; // total execution times for task[i]
static __nv uint32_t task_commit[TASK_NUM] = {0}; // total pre_commit size for all execution times of task[i]



void pc_rsa_main()
{

    if (first_run == 1) { status = 0; first_run = 0;}
    else {
        if (__IS_TASK_RUNNING && backup_needed[__GET_CURTASK] == true) {
            BUILDIN_UNDO;
        }
    }

    PREPARE_FOR_BACKUP;


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
    case 10: goto decrypt_init; //     
    case 11: goto decrypt_inner_loop; 
    case 12: goto decrypt_finish; //       
    }

__BUILDIN_TASK_BOUNDARY(0,initTask); // 0


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


    __NEXT(1, ce_1);


__BUILDIN_TASK_BOUNDARY(1,ce_1); // 1

    __GET(i)++; // start with i=2

    if (__GET(i) >= __GET(t)) {
        __NEXT(7, encrypt_init);
    } else {
        __NEXT(2, ce_2);
    }



__BUILDIN_TASK_BOUNDARY(2, ce_2); // 2

    if (__GET(t) % __GET(i) == 0) {
        __NEXT(1, ce_1);
    } else {
        __NEXT(3, is_i_prime);
    }



__BUILDIN_TASK_BOUNDARY(3, is_i_prime); // 3

    int c;
    c=sqrt16(__GET(i));
    __GET(j) = c;
    for(c=2; c <= __GET( j) ;c++)
    {
        if( __GET(i) % c==0)
        {
            __GET(flag)=0;
            __NEXT(1, ce_1);
            //return;
        }
    }
    __GET(flag) = 1;


   __NEXT(4, ce_3);



__BUILDIN_TASK_BOUNDARY(4, ce_3); // 4

    long int in_i = __GET(i);
    if( __GET(flag) == 1 && in_i != __GET(p) && in_i != __GET(q) )
    {
        __GET(e[__GET(k)]) = in_i ;
    } else {
        __NEXT(1, ce_1);
        //return;
    }

    __NEXT(5, cd);




__BUILDIN_TASK_BOUNDARY(5, cd); // 5

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

    __NEXT(6, ce_4);



__BUILDIN_TASK_BOUNDARY(6, ce_4); // 6
    //int 
    __cry = __GET(flag);
    if(__cry > 0)
    {
        __GET(d[ __GET(k) ]) =__cry;
        __GET(k)++;
    }

    if (__GET(k) < 9) {
        __NEXT(1, ce_1);
    } else {
        __NEXT(7, encrypt_init);
    }


__BUILDIN_TASK_BOUNDARY(7, encrypt_init); // 7

  //long int __cry;
   __cry = __GET(m[ __GET(en_cnt) ]) ;
   __GET(en_pt) = __cry;
   __GET(en_pt) -=96;
   __GET(en_k)  = 1;
   __GET(en_j)  = 0;
   __cry = __GET(e[0]) ;
   __GET(en_key) = __cry;


    __NEXT(8, encrypt_inner_loop);




__BUILDIN_TASK_BOUNDARY(8, encrypt_inner_loop); // 8



   //long int __cry;
    if (__GET(en_j) < __GET(en_key)) {
        __cry = __GET(en_k) * __GET(en_pt);
        __GET(en_k) = __cry;
        __cry = __GET(en_k) % __GET(n);
        __GET(en_k) = __cry;
        __GET(en_j)++;
        __NEXT(8, encrypt_inner_loop);
    } else {
        __NEXT(9, encrypt_finish);
    }


__BUILDIN_TASK_BOUNDARY(9, encrypt_finish); // 9


   //long int __cry;
   __cry = __GET(en_k);
   __GET(temp[ __GET(en_cnt) ]) = __cry;
   __cry = __GET(en_k) + 96;
   __GET(en_ct) = __cry;
   __cry = __GET(en_ct);
   __GET(en[ __GET(en_cnt) ]) = __cry;

    if (__GET(en_cnt) < MSG_LEN) {
        __GET(en_cnt)++;
        __NEXT(7, encrypt_init);
    } else {
        __GET(en[ __GET(en_cnt) ]) = -1;
        __NEXT(10, decrypt_init);
    }



__BUILDIN_TASK_BOUNDARY(10, decrypt_init); // 11


   //long int __cry;
   __GET(de_k)  = 1;
   __GET(de_j)  = 0;
   __cry =__GET(d[0]);
   __GET(de_key) = __cry;

    __NEXT(11, decrypt_inner_loop);



__BUILDIN_TASK_BOUNDARY(11, decrypt_inner_loop); // 12
// war de_k de_j

   //long int __cry;
   __cry =  __GET(temp[ __GET(de_cnt) ]);
   __GET(de_ct) = __cry;

    if( __GET(de_j) < __GET(de_key) )
    {
        __cry = __GET(de_k) * __GET(de_ct);
        __GET(de_k) = __cry;
        __cry = __GET(de_k) % __GET(n);
        __GET(de_k) = __cry;
        __GET(de_j)++;
         __NEXT(11, decrypt_inner_loop);
    } else {
         __NEXT(12, decrypt_finish);
    }





__BUILDIN_TASK_BOUNDARY(12, decrypt_finish); // 13

//long int de_cnt;

   //long int __cry;
   __cry = __GET(de_k) + 96;
   __GET(de_pt) = __cry;
   __GET(m[ __GET(de_cnt)]) = __cry;

    if (__GET(en[ __GET(de_cnt) ]) != -1) {
        __GET(de_cnt)++;
        __NEXT(10, decrypt_init);
    } else {
         __FINISH;  //down
    }


}
