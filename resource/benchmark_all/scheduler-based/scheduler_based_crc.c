
#include <app/app_api.h>
#include <app/app_global.h>
#include <scheduling/scheduler.h>
#include <scheduling/scheduler_based_task.h>

__SHARED_VAR(
uint16_t SW_Results;
uint16_t cnt;
)

//0.
__nv uint16_t SW_Results_priv;
__nv uint16_t cnt_priv;


__TASK_ENTRY(init,

    //1.
    //SW_Results_priv = __GET(SW_Results);

__GET(cnt) = 0;
__GET(SW_Results) = CRC_INIT;

//3
//write_to_gbuf(&SW_Results_priv, &SW_Results, sizeof(SW_Results));

return 1;

)


__TASK(1, main,
    //1.
SW_Results_priv = __GET(SW_Results);
cnt_priv = = __GET(cnt);



SW_Results_priv = CRCheck_CCITT_Update(SW_Results_priv, CRC_Input[cnt_priv] & 0xFF);
SW_Results_priv = CRCheck_CCITT_Update(SW_Results_priv, (CRC_Input[cnt_priv] >> 8) & 0xFF);
++cnt_priv;

if (cnt_priv < CRC_LENGTH) {
    //3
    write_to_gbuf(&SW_Results_priv, &SW_Results, sizeof(SW_Results));
    write_to_gbuf(&cnt_priv, &cnt, sizeof(cnt));
    return 1;
}
else {
    //3
    write_to_gbuf(&SW_Results_priv, &SW_Results, sizeof(SW_Results));
    write_to_gbuf(&cnt_priv, &cnt, sizeof(cnt));
    return TASK_FINISH;
}

)


void crc_regist()
{
    task_regist(0, init,       false);
    task_regist(1, main,       true );

    WAR_REGIST(2);
}
