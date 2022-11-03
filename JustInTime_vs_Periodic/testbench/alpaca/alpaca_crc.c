#include <testbench/alpaca.h>
#include <testbench/global_declaration.h>
#include <testbench/testbench_api.h>

    __GLOBAL_SCALAR(uint16_t, SW_Results);
    __GLOBAL_SCALAR(uint16_t, cnt);

    // 0.
    static __nv uint16_t SW_Results_priv;
    static __nv uint16_t cnt_priv;

static __nv uint16_t  status = 0;  //cur_task->id


void alpaca_crc_main()
{

switch(__GET_CURTASK) {

        case 0:
            goto init;
        case 1:
            goto main;
        }

    __TASK(0, init);

                 // 1.
                 // SW_Results_priv = __GET(SW_Results);

                 __GET(cnt) = 0;
                 __GET(SW_Results) = CRC_INIT;

                 // 3
                 // write_to_gbuf(&SW_Results_priv, &SW_Results, sizeof(SW_Results));

                 __TRANSITION_TO( 1,main);

    

    __TASK(1, main);
    // 1.
    SW_Results_priv = __GET(SW_Results);
    cnt_priv = __GET(cnt);

    SW_Results_priv = CRCheck_CCITT_Update(SW_Results_priv, CRC_Input[cnt_priv] & 0xFF);
    SW_Results_priv = CRCheck_CCITT_Update(SW_Results_priv, (CRC_Input[cnt_priv] >> 8) & 0xFF);
    ++cnt_priv;

    if (cnt_priv < CRC_LENGTH)
    {
        // 3
        write_to_gbuf(&SW_Results_priv, &SW_Results, sizeof(SW_Results));
        write_to_gbuf(&cnt_priv, &cnt, sizeof(cnt));
        __TRANSITION_TO(1, main);
    }
    else
    {
        // 3
        write_to_gbuf(&SW_Results_priv, &SW_Results, sizeof(SW_Results));
        write_to_gbuf(&cnt_priv, &cnt, sizeof(cnt));
        __TASK_DOWN;  //return    __TRANSITION_TO( TASK_FINISH;
    }

    


}//mian
